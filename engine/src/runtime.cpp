#include "runtime.hpp"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace mdz {
namespace {

// Comparison selector values, as stored in a tag-8 parameter slot.
bool compare_with(int op, const Value& a, const Value& b) {
    const int c = a.compare(b);
    switch (op) {
        case 0: return c == 0;   // =
        case 1: return c != 0;   // <>
        case 2: return c < 0;    // <
        case 3: return c <= 0;   // <=
        case 4: return c > 0;    // >
        case 5: return c >= 0;   // >=
        default: return false;
    }
}

// The object a picking condition operates on is named by its tag-4 parameter,
// not by the condition's own object_type (which is System for these).
int object_param(const std::vector<Param>& params) {
    for (const Param& p : params)
        if (p.tag == 4) return static_cast<int>(p.value.number);
    return -1;
}

// Axis-aligned bounds. Construct 2 tests per-frame collision polygons; this is
// the bounding box only, so it over-reports overlap for non-rectangular art.
struct Bounds { double l, t, r, b; };
Bounds bounds_of(const Instance& i) {
    const double hw = (i.width != 0 ? i.width : 1.0) * 0.5;
    const double hh = (i.height != 0 ? i.height : 1.0) * 0.5;
    return Bounds{i.x - hw, i.y - hh, i.x + hw, i.y + hh};
}
bool overlaps(const Bounds& a, const Bounds& b) {
    return a.l < b.r && b.l < a.r && a.t < b.b && b.t < a.b;
}

int param_int(const std::vector<Param>& params, size_t i, int fallback = 0) {
    if (i >= params.size()) return fallback;
    return static_cast<int>(params[i].value.number);
}

}  // namespace

Runtime::Runtime(const Project& project, const AceNames& names)
    : project_(project), names_(names), engine_(project) {
    register_builtins();
    register_expression_builtins();
}

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

// Indexes every OnFunction trigger across all sheets. In this project all 275
// of them are the first condition of their event, so the name is unambiguous.
void Runtime::index_functions() {
    functions_.clear();
    const EventSheet* current_sheet = nullptr;
    std::function<void(const EventBlock&)> visit = [&](const EventBlock& b) {
        block_sheet_[&b] = current_sheet;
        if (!b.conditions.empty()) {
            const Condition& c = b.conditions.front();
            if (c.object_type >= 0 &&
                names_.condition(project_.type(c.object_type).plugin, c.ace, c.behavior) == "OnFunction" &&
                !c.params.empty()) {
                functions_[c.params[0].value.text].push_back(&b);
            }
        }
        // Index every block by the name of its first condition, so triggers
        // can be dispatched by name.
        if (!b.conditions.empty()) {
            const Condition& c0 = b.conditions.front();
            if (c0.trigger_mode != 0) {
                const int pl = c0.object_type < 0 ? -1 : project_.type(c0.object_type).plugin;
                const std::string n = names_.condition(pl, c0.ace, c0.behavior);
                if (!n.empty()) triggers_[n].push_back(&b);
                triggers_by_ace_[std::to_string(pl) + ":" + std::to_string(c0.ace)].push_back(&b);
                // A System trigger with no parameters and nothing else gating
                // the block is a layout-lifecycle trigger.
                if (pl == -1 && c0.params.empty()) start_of_layout_aces_.insert(c0.ace);
            }
        }
        for (const EventBlock& s : b.subevents) visit(s);
    };
    triggers_.clear();
    triggers_by_ace_.clear();
    start_of_layout_aces_.clear();
    block_sheet_.clear();
    for (const EventSheet& s : project_.sheets) {
        current_sheet = &s;
        for (const EventBlock& b : s.blocks) visit(b);
    }
}

// A layout runs its own sheet plus everything that sheet includes, transitively.
void Runtime::compute_active_sheets(const Layout& layout) {
    active_sheets_.clear();
    std::vector<std::string> pending{layout.event_sheet};
    while (!pending.empty()) {
        const std::string name = pending.back();
        pending.pop_back();
        for (const EventSheet& s : project_.sheets) {
            if (s.name != name || active_sheets_.count(&s)) continue;
            active_sheets_.insert(&s);
            for (const std::string& inc : s.includes) pending.push_back(inc);
        }
    }
}

bool Runtime::block_is_active(const EventBlock* b) const {
    auto it = block_sheet_.find(b);
    if (it == block_sheet_.end() || it->second == nullptr) return true;
    return active_sheets_.count(it->second) != 0;
}

namespace {
void run_trigger_blocks(PickingEngine& engine, RunnerHooks hooks,
                        const std::vector<const EventBlock*>& blocks) {
    EventRunner runner(engine, hooks);
    runner.run_triggers = true;
    for (const EventBlock* b : blocks) {
        engine.reset_all();
        runner.run_block(*b, 0);
    }
}
}  // namespace

void Runtime::fire_trigger(const std::string& ace_name) {
    auto it = triggers_.find(ace_name);
    if (it == triggers_.end()) return;
    std::vector<const EventBlock*> live;
    for (const EventBlock* b : it->second) if (block_is_active(b)) live.push_back(b);
    run_trigger_blocks(engine_, make_hooks(), live);
}

void Runtime::fire_trigger_by_ace(int plugin, int ace) {
    auto it = triggers_by_ace_.find(std::to_string(plugin) + ":" + std::to_string(ace));
    if (it == triggers_by_ace_.end()) return;
    std::vector<const EventBlock*> live;
    for (const EventBlock* b : it->second) if (block_is_active(b)) live.push_back(b);
    run_trigger_blocks(engine_, make_hooks(), live);
}

void Runtime::request_layout(const std::string& name) { pending_layout_ = name; }

bool Runtime::pointer_over(int object_type) const {
    if (object_type < 0) return false;
    for (int i : engine_.instances_of(object_type)) {
        const Instance& o = engine_.instances[static_cast<size_t>(i)];
        if (o.destroyed || !o.visible) continue;
        const Bounds b = bounds_of(o);
        if (input.x >= b.l && input.x <= b.r && input.y >= b.t && input.y <= b.b) return true;
    }
    return false;
}

Value Runtime::call_function(const std::string& name, std::vector<Value> args) {
    auto it = functions_.find(name);
    if (it == functions_.end()) return Value(0.0);
    // Recursion is legitimate here, so bound the depth rather than forbid it.
    if (frames_.size() > 32) return Value(0.0);

    frames_.push_back(CallFrame{std::move(args), Value(0.0)});
    RunnerHooks hooks = make_hooks();
    EventRunner runner(engine_, hooks);
    runner.run_triggers = true;          // the body IS a trigger block
    for (const EventBlock* b : it->second) {
        if (!block_is_active(b)) continue;
        engine_.push_scope();
        runner.run_block(*b, 0);
        engine_.pop_scope();
    }
    const Value ret = frames_.back().ret;
    frames_.pop_back();
    return ret;
}

int Runtime::create_instance(int object_type, double x, double y, int layer) {
    if (object_type < 0 || object_type >= static_cast<int>(project_.object_types.size()))
        return -1;
    const ObjectType& t = project_.type(object_type);
    if (t.is_family) return -1;          // families hold no instances of their own
    Instance inst;
    inst.object_type = object_type;
    inst.uid = next_uid_++;
    inst.x = x;
    inst.y = y;
    inst.layer = layer;
    inst.vars.assign(static_cast<size_t>(t.instance_var_count), 0.0);
    if (const Frame* f = t.first_frame()) {
        inst.width = f->w;
        inst.height = f->h;
    }
    const int index = engine_.add_instance(inst);
    engine_.pick_single(object_type, index);   // C2 picks what it just created
    return index;
}

void Runtime::load_layout(const Layout& layout) {
    // Variables are seeded once, from every sheet, and then persist across
    // layout changes.
    for (const EventSheet& s : project_.sheets)
        for (const EventVariable& v : s.variables)
            if (!variables_.count(v.name))
                variables_[v.name] = v.is_text ? Value(v.initial_text) : Value(v.initial_number);
    index_functions();
    load_layout_instances(layout);
}

void Runtime::load_layout_instances(const Layout& layout) {
    layout_ = &layout;
    engine_.load_layout(layout);

    // Construct 2 gives single-instance plugins -- Touch, Keyboard, Mouse,
    // Function, Audio and friends -- one implicit instance that never appears
    // in layout data. Without it their conditions have no candidate instances,
    // so the picking engine rejects them before the predicate is ever called
    // and every such condition silently reads false. That is not a small
    // detail: it is why OnFunction never fired, so function bodies did not run
    // even though CallFunction did, and why no touch or click was ever seen.
    //
    // They are identified structurally: a plugin used by exactly one object
    // type, where that type has no instances in the layout.
    std::unordered_map<int, int> types_per_plugin;
    for (const ObjectType& t : project_.object_types)
        if (!t.is_family) ++types_per_plugin[t.plugin];
    for (const ObjectType& t : project_.object_types) {
        if (t.is_family) continue;
        if (types_per_plugin[t.plugin] != 1) continue;
        if (!engine_.instances_of(t.index).empty()) continue;
        Instance inst;
        inst.object_type = t.index;
        inst.uid = -1;                      // assigned below, after uid scan
        inst.vars.assign(static_cast<size_t>(t.instance_var_count), 0.0);
        engine_.add_instance(inst);
    }
    for (const Instance& i : engine_.instances) next_uid_ = std::max(next_uid_, i.uid + 1);
    for (Instance& i : engine_.instances) if (i.uid < 0) i.uid = next_uid_++;
    // Variables are seeded once in load_layout and must NOT be re-seeded here:
    // they persist across layout changes, as they do in the original.
    sheet_ = nullptr;
    for (const EventSheet& s : project_.sheets)
        if (s.name == layout.event_sheet) sheet_ = &s;
    compute_active_sheets(layout);

    // Fire the System triggers that take no parameters. Construct 2 has
    // several that fire at or just after layout start (start of layout, loader
    // layout complete, and so on) and the name table cannot tell them apart:
    // 27 distinct triggers share one trivial "return true" body, so they
    // collapsed to a single name. Their blocks are otherwise unreachable,
    // because the normal pass skips triggers -- which is why the Loading sheet
    // never reached its GoToLayout.
    //
    // Firing all of them is a deliberate over-approximation, checked against
    // the original runtime by the differential test rather than assumed.
    if (std::getenv("MDZ_DEBUG_TRIGGERS")) {
        std::fprintf(stderr, "[layout %s] lifecycle triggers:", layout.name.c_str());
        for (int ace : start_of_layout_aces_) {
            auto it = triggers_by_ace_.find("-1:" + std::to_string(ace));
            std::fprintf(stderr, " #%d(%zu)", ace,
                         it == triggers_by_ace_.end() ? size_t(0) : it->second.size());
        }
        std::fprintf(stderr, "\n");
    }
    for (int ace : start_of_layout_aces_) fire_trigger_by_ace(-1, ace);
}

Value Runtime::get_variable(const std::string& name) const {
    auto it = variables_.find(name);
    return it == variables_.end() ? Value(0.0) : it->second;
}

void Runtime::set_variable(const std::string& name, const Value& v) { variables_[name] = v; }

Instance* Runtime::instance(int index) {
    if (index < 0 || index >= static_cast<int>(engine_.instances.size())) return nullptr;
    return &engine_.instances[static_cast<size_t>(index)];
}

// ---------------------------------------------------------------------------
// Expressions
// ---------------------------------------------------------------------------

Value Runtime::eval(const Expr& e, const Instance* self) const {
    switch (e.op) {
        case ExpOp::Int:
        case ExpOp::Float:
            return Value(e.number);
        case ExpOp::String:
            return Value(e.text);
        case ExpOp::EventVar:
            return get_variable(e.text);

        case ExpOp::InstanceVar: {
            // Prefer the instance in hand; otherwise the first one picked for
            // the referenced type, which is what the original resolves to.
            const Instance* target = self;
            if (!target || target->object_type != e.object_type) {
                const std::vector<int> picked = engine_.picked(e.object_type);
                if (picked.empty()) return Value(0.0);
                target = &engine_.instances[static_cast<size_t>(picked.front())];
            }
            if (e.index < 0 || e.index >= static_cast<int>(target->vars.size())) return Value(0.0);
            return Value(target->vars[static_cast<size_t>(e.index)]);
        }

        case ExpOp::Negate:
            return Value(-(e.args.empty() ? 0.0 : eval(e.args[0], self).as_number()));

        case ExpOp::Conditional:
            if (e.args.size() < 3) return Value(0.0);
            return eval(e.args[0], self).truthy() ? eval(e.args[1], self) : eval(e.args[2], self);

        // Expression tables were never named -- they are a third index space
        // alongside conditions and actions, and have not been extracted yet.
        // Counting them keeps their absence visible instead of silently
        // yielding zero.
        case ExpOp::SystemExp:
        case ExpOp::ObjectExp:
        case ExpOp::BehaviorExp:
            return call_expression(e, self);

        default: break;
    }

    if (e.args.size() < 2) return Value(0.0);
    const Value a = eval(e.args[0], self);
    const Value b = eval(e.args[1], self);
    switch (e.op) {
        case ExpOp::Add:
            if (a.is_text() && b.is_text()) return Value(a.as_text() + b.as_text());
            return Value(a.as_number() + b.as_number());
        case ExpOp::Subtract: return Value(a.as_number() - b.as_number());
        case ExpOp::Multiply: return Value(a.as_number() * b.as_number());
        case ExpOp::Divide: {
            const double d = b.as_number();
            return Value(d == 0.0 ? 0.0 : a.as_number() / d);
        }
        case ExpOp::Modulo: {
            const double d = b.as_number();
            return Value(d == 0.0 ? 0.0 : std::fmod(a.as_number(), d));
        }
        case ExpOp::Power: return Value(std::pow(a.as_number(), b.as_number()));
        case ExpOp::AndConcat:
            // '&' is string concatenation when either side is text, and
            // logical and when both are numeric.
            if (a.is_text() || b.is_text()) return Value(a.as_text() + b.as_text());
            return Value((a.truthy() && b.truthy()) ? 1.0 : 0.0);
        case ExpOp::Or:           return Value((a.truthy() || b.truthy()) ? 1.0 : 0.0);
        case ExpOp::Equal:        return Value(a.compare(b) == 0 ? 1.0 : 0.0);
        case ExpOp::NotEqual:     return Value(a.compare(b) != 0 ? 1.0 : 0.0);
        case ExpOp::Less:         return Value(a.compare(b) < 0 ? 1.0 : 0.0);
        case ExpOp::LessEqual:    return Value(a.compare(b) <= 0 ? 1.0 : 0.0);
        case ExpOp::Greater:      return Value(a.compare(b) > 0 ? 1.0 : 0.0);
        case ExpOp::GreaterEqual: return Value(a.compare(b) >= 0 ? 1.0 : 0.0);
        default: return Value(0.0);
    }
}

// Expressions carry no index at runtime, so they are named through the same
// table the decompiler uses and dispatched by that name.
Value Runtime::call_expression(const Expr& e, const Instance* self) const {
    char kind = 'S';
    int plugin = -1;
    if (e.op == ExpOp::ObjectExp) { kind = 'O'; plugin = project_.type(e.object_type).plugin; }
    else if (e.op == ExpOp::BehaviorExp) { kind = 'B'; plugin = project_.type(e.object_type).plugin; }

    const std::string name = names_.expression(kind, plugin, e.index,
                                               e.op == ExpOp::BehaviorExp ? e.text : "");
    // Expression names collide across plugins -- Touch.X and Sprite.X are both
    // called "X". A plugin-qualified registration wins over the bare name, so
    // the pointer position does not silently resolve to an instance position.
    auto it = expressions_.find(std::to_string(plugin) + ":" + name);
    if (it == expressions_.end()) it = expressions_.find(name);
    if (it == expressions_.end()) {
        ++stats_.unknown_expressions;
        // Identify unnamed ones by their key, so they can be looked up rather
        // than all collapsing into one anonymous bucket.
        ++stats_.missing_expressions[
            name.empty() ? (std::string("<unnamed ") + kind + " plugin=" +
                            std::to_string(plugin) + " index=" + std::to_string(e.index) + ">")
                         : name];
        return Value(0.0);
    }
    // The instance in context, or the first one picked for the referenced type.
    const Instance* target = self;
    if (e.op != ExpOp::SystemExp) {
        if (!target || target->object_type != e.object_type) {
            const std::vector<int> picked = engine_.picked(e.object_type);
            target = picked.empty() ? nullptr
                                    : &engine_.instances[static_cast<size_t>(picked.front())];
        }
    }
    return it->second(const_cast<Runtime&>(*this), e, target);
}

Value Runtime::param(const Action& a, size_t index, const Instance* self) const {
    if (index >= a.params.size()) return Value(0.0);
    return eval(a.params[index].value, self);
}
Value Runtime::param(const Condition& c, size_t index, const Instance* self) const {
    if (index >= c.params.size()) return Value(0.0);
    return eval(c.params[index].value, self);
}

// ---------------------------------------------------------------------------
// ACE dispatch
// ---------------------------------------------------------------------------

std::string Runtime::condition_name(const Condition& c) const {
    const int plugin = c.object_type < 0 ? -1 : project_.type(c.object_type).plugin;
    return names_.condition(plugin, c.ace, c.behavior);
}
std::string Runtime::action_name(const Action& a) const {
    const int plugin = a.object_type < 0 ? -1 : project_.type(a.object_type).plugin;
    return names_.action(plugin, a.ace, a.behavior);
}

void Runtime::register_expression(const std::string& name, ExpressionFn fn) {
    expressions_[name] = std::move(fn);
}

void Runtime::register_condition(const std::string& name, ConditionFn fn) {
    conditions_[name] = std::move(fn);
}
void Runtime::register_action(const std::string& name, ActionFn fn) {
    actions_[name] = std::move(fn);
}

RunnerHooks Runtime::make_hooks() {
    RunnerHooks hooks;

    hooks.instance_condition = [this](const Condition& c, const Instance& inst) {
        ++stats_.conditions_run;
        const std::string name = condition_name(c);
        auto it = conditions_.find(name);
        if (it == conditions_.end()) {
            ++stats_.unimplemented_conditions;
            ++stats_.missing_conditions[name.empty() ? "<unnamed>" : name];
            return !strict_unimplemented;
        }
        return it->second(*this, c, &inst);
    };

    hooks.system_condition = [this](const Condition& c) {
        ++stats_.conditions_run;
        const std::string name = condition_name(c);
        auto it = conditions_.find(name);
        if (it == conditions_.end()) {
            ++stats_.unimplemented_conditions;
            ++stats_.missing_conditions[name.empty() ? "<unnamed>" : name];
            return !strict_unimplemented;
        }
        return it->second(*this, c, nullptr);
    };

    hooks.action = [this](const Action& a, const std::vector<int>& picked) {
        ++stats_.actions_run;
        const std::string name = action_name(a);
        auto it = actions_.find(name);
        if (it == actions_.end()) {
            ++stats_.unimplemented_actions;
            ++stats_.missing_actions[name.empty() ? "<unnamed>" : name];
            return;
        }
        it->second(*this, a, picked);
    };

    hooks.sibling_result = [this](bool passed) { last_sibling_passed_ = passed; };
    hooks.group_active = [this](const std::string& n) { return group_active(n); };

    // "For each" names its target through an object parameter (slot tag 4).
    hooks.loop_target = [](const Condition& c) {
        for (const Param& p : c.params)
            if (p.tag == 4) return static_cast<int>(p.value.number);
        return -1;
    };
    return hooks;
}

// "On collision" is an edge, not a state: it fires on the tick an overlap
// begins. Overlap sets are computed once per type pair per tick (the first
// instance to ask triggers the work) and diffed against the previous tick.
bool Runtime::on_collision(int self_type, int other_type, int instance_index) {
    const TypePair key{self_type, other_type};
    if (!collision_done_.count(key)) {
        collision_done_.insert(key);
        PairSet current;
        for (int a : engine_.instances_of(self_type)) {
            const Instance& ia = engine_.instances[static_cast<size_t>(a)];
            if (ia.destroyed) continue;
            const Bounds ba = bounds_of(ia);
            for (int b : engine_.instances_of(other_type)) {
                if (a == b) continue;
                const Instance& ib = engine_.instances[static_cast<size_t>(b)];
                if (ib.destroyed) continue;
                if (overlaps(ba, bounds_of(ib))) current.insert({a, b});
            }
        }
        PairSet& prev = overlaps_prev_[key];
        PairSet began;
        for (const auto& p : current)
            if (!prev.count(p)) began.insert(p);
        overlaps_new_[key] = std::move(began);
        prev = std::move(current);
    }
    for (const auto& p : overlaps_new_[key])
        if (p.first == instance_index) return true;
    return false;
}

bool Runtime::every_seconds(long long sid, double interval) {
    if (interval <= 0.0) return true;
    double& acc = accumulators_[sid];
    acc += dt_;
    if (acc < interval) return false;
    // Subtract rather than zero, so long intervals do not drift.
    acc -= interval;
    return true;
}

// Timer state lives on the instance, keyed by behavior and tag so two
// behaviors on one instance keep separate timers. A timer that elapses sets a
// "fired" flag that OnTimer consumes during this tick.
void Runtime::update_timers(double dt) {
    for (Instance& inst : engine_.instances) {
        if (inst.destroyed || inst.behavior_state.empty()) continue;
        std::vector<std::pair<std::string, double>> fires;
        for (auto& kv : inst.behavior_state) {
            const std::string& key = kv.first;
            if (key.size() < 9 || key.compare(key.size() - 9, 9, ".duration") != 0) continue;
            const std::string base = key.substr(0, key.size() - 9);
            double& elapsed = inst.behavior_state[base + ".elapsed"];
            elapsed += dt;
            if (kv.second > 0.0 && elapsed >= kv.second) {
                elapsed -= kv.second;
                fires.emplace_back(base, 1.0);
            }
        }
        for (auto& f : fires) inst.behavior_state[f.first + ".fired"] = f.second;
    }
}

void Runtime::update_pins() {
    for (Instance& inst : engine_.instances) {
        if (inst.destroyed || inst.behavior_state.empty()) continue;
        auto uid_it = inst.behavior_state.find("Pin.uid");
        if (uid_it == inst.behavior_state.end()) continue;
        const int target_uid = static_cast<int>(uid_it->second);
        const Instance* target = nullptr;
        for (const Instance& o : engine_.instances)
            if (!o.destroyed && o.uid == target_uid) { target = &o; break; }
        if (!target) continue;
        inst.x = target->x + inst.behavior_state["Pin.dx"];
        inst.y = target->y + inst.behavior_state["Pin.dy"];
    }
}

bool Runtime::group_active(const std::string& name) const {
    auto it = groups_.find(name);
    return it == groups_.end() ? true : it->second;
}
void Runtime::set_group_active(const std::string& name, bool active) {
    groups_[name] = active;
}

bool Runtime::trigger_once(long long sid) {
    const bool was = fired_.count(sid) != 0;
    fired_.insert(sid);
    return !was;
}

void Runtime::tick(double dt_seconds) {
    dt_ = dt_seconds;
    time_ += dt_seconds;
    // Clear last tick's timer flags, then advance.
    for (Instance& inst : engine_.instances)
        for (auto& kv : inst.behavior_state)
            if (kv.first.size() > 6 && kv.first.compare(kv.first.size() - 6, 6, ".fired") == 0)
                kv.second = 0.0;
    update_timers(dt_seconds);
    update_pins();
    collision_done_.clear();   // overlap sets are recomputed per tick
    if (!sheet_) return;

    RunnerHooks hooks = make_hooks();
    EventRunner runner(engine_, hooks);
    runner.run_sheet(*sheet_);

    // Input triggers fire after the main pass, then the edge flags clear so a
    // press is delivered on exactly one tick.
    if (input.pressed) {
        fire_trigger("OnTouchObject");
        fire_trigger("OnObjectClicked");
        fire_trigger("OnClick");
    }
    if (input.released) fire_trigger("OnNthTouchEnd");
    if (!input.keys_pressed.empty()) fire_trigger("OnKey");
    input.pressed = false;
    input.released = false;
    input.keys_pressed.clear();

    // Apply any layout change the events asked for, now that the tick is over.
    if (!pending_layout_.empty()) {
        const std::string want = pending_layout_;
        pending_layout_.clear();
        for (const Layout& l : project_.layouts) {
            if (l.name != want) continue;
            // Globals persist; instances and per-instance state do not.
            engine_.clear();
            dictionaries_.clear();
            overlaps_prev_.clear();
            overlaps_new_.clear();
            collision_done_.clear();
            load_layout_instances(l);
            break;
        }
    }
}

// ---------------------------------------------------------------------------
// Built-in ACEs
//
// Registered by name, so each implementation covers every call site the name
// resolves to. Add a new one by writing a function and registering it here.
// ---------------------------------------------------------------------------

void Runtime::register_builtins() {
    // --- System conditions -------------------------------------------------
    register_condition("CompareVariable", [](Runtime& rt, const Condition& c, const Instance*) {
        const Value v = rt.get_variable(c.params.empty() ? "" : c.params[0].value.text);
        return compare_with(param_int(c.params, 1), v, rt.param(c, 2, nullptr));
    });
    register_condition("CompareTwoValues", [](Runtime& rt, const Condition& c, const Instance*) {
        return compare_with(param_int(c.params, 1), rt.param(c, 0, nullptr), rt.param(c, 2, nullptr));
    });
    register_condition("AlwaysTrueTrigger", [](Runtime&, const Condition&, const Instance*) {
        return true;
    });
    register_condition("EveryTick", [](Runtime&, const Condition&, const Instance*) { return true; });
    register_condition("IsGroupActive", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.group_active(c.params.empty() ? "" : c.params[0].value.text);
    });

    // --- Instance conditions ----------------------------------------------
    register_condition("CompareInstanceVar", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int idx = param_int(c.params, 0);
        if (idx < 0 || idx >= static_cast<int>(self->vars.size())) return false;
        return compare_with(param_int(c.params, 1), Value(self->vars[static_cast<size_t>(idx)]),
                            rt.param(c, 2, self));
    });
    register_condition("IsBooleanInstanceVarSet", [](Runtime&, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int idx = param_int(c.params, 0);
        if (idx < 0 || idx >= static_cast<int>(self->vars.size())) return false;
        return self->vars[static_cast<size_t>(idx)] != 0.0;
    });

    // --- Picking ----------------------------------------------------------
    // The single largest gap by call volume: PickByUID alone is reached ~80k
    // times in 30 ticks of Game_events. It filters like any other instance
    // condition, so the picking engine handles the SOL bookkeeping already.
    register_condition("PickByUID", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return static_cast<double>(self->uid) == rt.param(c, 0, self).as_number();
    });

    // These name their target through an object parameter and rewrite that
    // type's selection wholesale, rather than filtering the current one.
    register_condition("PickAll", [](Runtime& rt, const Condition& c, const Instance*) {
        const int t = object_param(c.params);
        if (t < 0) return false;
        const std::vector<int> all = rt.engine().instances_of(t);
        rt.engine().pick_set(t, all);
        return !all.empty();
    });
    register_condition("PickByComparison", [](Runtime& rt, const Condition& c, const Instance*) {
        const int t = object_param(c.params);
        if (t < 0) return false;
        std::vector<int> kept;
        for (int i : rt.engine().picked(t)) {
            const Instance* inst = &rt.engine().instances[static_cast<size_t>(i)];
            if (compare_with(param_int(c.params, 2), rt.param(c, 1, inst), rt.param(c, 3, inst)))
                kept.push_back(i);
        }
        rt.engine().pick_set(t, kept);
        return !kept.empty();
    });
    register_condition("PickRandomInstance", [](Runtime& rt, const Condition& c, const Instance*) {
        const int t = object_param(c.params);
        if (t < 0) return false;
        const std::vector<int> pool = rt.engine().picked(t);
        if (pool.empty()) return false;
        static uint32_t seed = 0x85EBCA6Bu;
        seed = seed * 1664525u + 1013904223u;
        rt.engine().pick_set(t, {pool[(seed >> 8) % pool.size()]});
        return true;
    });

    // Bounding-box only -- see bounds_of. Real collision needs the per-frame
    // polygons the export carries, so this over-reports on non-rectangular art.
    register_condition("IsOverlapping", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int other = object_param(c.params);
        if (other < 0) return false;
        const Bounds a = bounds_of(*self);
        for (int i : rt.engine().instances_of(other)) {
            const Instance& o = rt.engine().instances[static_cast<size_t>(i)];
            if (!o.destroyed && &o != self && overlaps(a, bounds_of(o))) return true;
        }
        return false;
    });

    register_condition("CompareX", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return compare_with(param_int(c.params, 0), Value(self->x), rt.param(c, 1, self));
    });
    register_condition("CompareY", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return compare_with(param_int(c.params, 0), Value(self->y), rt.param(c, 1, self));
    });
    register_condition("IsBetweenAngles", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const double deg = self->angle * 180.0 / 3.14159265358979323846;
        double lo = rt.param(c, 0, self).as_number(), hi = rt.param(c, 1, self).as_number();
        auto norm = [](double d) { d = std::fmod(d, 360.0); return d < 0 ? d + 360.0 : d; };
        const double a = norm(deg); lo = norm(lo); hi = norm(hi);
        return lo <= hi ? (a >= lo && a <= hi) : (a >= lo || a <= hi);
    });

    register_condition("Else", [](Runtime& rt, const Condition&, const Instance*) {
        return !rt.last_sibling_passed();
    });
    register_condition("CompareFrame", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return compare_with(param_int(c.params, 0), Value(static_cast<double>(self->frame)),
                            rt.param(c, 1, self));
    });
    register_condition("IsOnLayer", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return static_cast<double>(self->layer) == rt.param(c, 0, self).as_number();
    });
    // params are (nearest|furthest, x, y): reduce the selection to the single
    // closest or furthest instance from that point.
    register_condition("PickDistance", [](Runtime& rt, const Condition& c, const Instance*) {
        const int t = object_param(c.params);
        const std::vector<int> pool = rt.engine().picked(t >= 0 ? t : 0);
        if (t < 0 || pool.empty()) return false;
        const bool furthest = param_int(c.params, 0) != 0;
        const double px = rt.param(c, 1, nullptr).as_number();
        const double py = rt.param(c, 2, nullptr).as_number();
        int best = pool.front();
        double best_d = -1.0;
        for (int i : pool) {
            const Instance& o = rt.engine().instances[static_cast<size_t>(i)];
            const double dx = o.x - px, dy = o.y - py;
            const double d = dx * dx + dy * dy;
            if (best_d < 0 || (furthest ? d > best_d : d < best_d)) { best_d = d; best = i; }
        }
        rt.engine().pick_set(t, {best});
        return true;
    });
    // Passes the first time this call site is true and not again until it has
    // gone false, so identity has to be per call site -- hence the SID.
    register_condition("TriggerOnce", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.trigger_once(c.sid);
    });

    register_condition("EveryXSeconds", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.every_seconds(c.sid, rt.param(c, 0, nullptr).as_number());
    });

    // --- input --------------------------------------------------------------
    register_condition("IsInTouch", [](Runtime& rt, const Condition&, const Instance*) {
        return rt.input.down;
    });
    register_condition("IsTouchingObject", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.input.down && rt.pointer_over(object_param(c.params));
    });
    register_condition("IsOverObject", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.pointer_over(object_param(c.params));
    });
    // The three ACEs the table calls OnTouchObject are all marked medium
    // confidence and are probably touch-start/end variants of each other. All
    // three are treated as "a press began over this object", which is right for
    // at least one of them and wrong for any that is really a release.
    register_condition("OnTouchObject", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.input.pressed && rt.pointer_over(object_param(c.params));
    });
    register_condition("OnObjectClicked", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.input.pressed && rt.pointer_over(object_param(c.params));
    });
    register_condition("OnClick", [](Runtime& rt, const Condition&, const Instance*) {
        return rt.input.pressed;
    });
    register_condition("OnNthTouchEnd", [](Runtime& rt, const Condition&, const Instance*) {
        return rt.input.released;
    });
    register_condition("IsKeyDown", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.input.keys.count(param_int(c.params, 0)) != 0;
    });
    register_condition("OnKey", [](Runtime& rt, const Condition& c, const Instance*) {
        return rt.input.keys_pressed.count(param_int(c.params, 0)) != 0;
    });

    register_condition("OnCollision", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int other = object_param(c.params);
        if (other < 0) return false;
        // The index is needed, and Instance carries no back-pointer, so
        // recover it from the address within the engine's instance vector.
        const int index = static_cast<int>(self - rt.engine().instances.data());
        return rt.on_collision(c.object_type, other, index);
    });

    register_action("SetVisible", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const bool visible = param_int(a.params, 0) != 0;
        for (int i : picked) if (Instance* n = rt.instance(i)) n->visible = visible;
    });
    register_action("SetAnimation", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const std::string name = rt.param(a, 0, nullptr).as_text();
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->animation = name;
            if (param_int(a.params, 1) == 0) inst->frame = 0;   // from beginning
        }
    });
    register_action("MoveToLayer", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int layer = static_cast<int>(rt.param(a, 0, nullptr).as_number());
        for (int i : picked) if (Instance* n = rt.instance(i)) n->layer = layer;
    });
    // Deliberately inert. A faithful Wait defers the REST of the action list
    // and its sub-events, with the selection captured at the point of the
    // wait, which needs an action-scheduling queue. Running on immediately is
    // wrong, but it is wrong in a bounded and obvious way rather than silently
    // dropping the actions that follow.
    register_action("WaitSeconds", [](Runtime&, const Action&, const std::vector<int>&) {});
    // Visual only; the engine models no effects.
    register_action("SetEffectParam", [](Runtime&, const Action&, const std::vector<int>&) {});

    register_action("SetOpacity", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const double v = rt.param(a, 0, nullptr).as_number() / 100.0;   // C2 uses 0..100
        for (int i : picked) if (Instance* n = rt.instance(i)) n->opacity = v;
    });
    register_action("SetBoolInstanceVar", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int idx = param_int(a.params, 0);
        const double v = param_int(a.params, 1) != 0 ? 1.0 : 0.0;
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst || idx < 0 || idx >= static_cast<int>(inst->vars.size())) continue;
            inst->vars[static_cast<size_t>(idx)] = v;
        }
    });
    register_action("SetAngleTowardPosition", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            const double dx = rt.param(a, 0, inst).as_number() - inst->x;
            const double dy = rt.param(a, 1, inst).as_number() - inst->y;
            inst->angle = std::atan2(dy, dx);
        }
    });
    register_action("MoveAtAngle", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const double deg = rt.param(a, 0, nullptr).as_number();
        const double dist = rt.param(a, 1, nullptr).as_number();
        const double rad = deg * 3.14159265358979323846 / 180.0;
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->x += std::cos(rad) * dist;
            inst->y += std::sin(rad) * dist;
        }
    });
    register_action("SetActive", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const double v = param_int(a.params, 0) != 0 ? 1.0 : 0.0;
        for (int i : picked)
            if (Instance* n = rt.instance(i)) n->behavior_state[a.behavior + ".active"] = v;
    });
    // Layer scaling is not modelled; accepting it silently is better than
    // counting it as missing forever, but nothing reads the value yet.
    register_action("SetLayerScale", [](Runtime&, const Action&, const std::vector<int>&) {});

    register_condition("CompareSpeed", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        auto it = self->behavior_state.find(c.behavior + ".speed");
        const double speed = it == self->behavior_state.end() ? 0.0 : it->second;
        return compare_with(param_int(c.params, 0), Value(speed), rt.param(c, 1, self));
    });
    register_condition("CompareWidth", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        return compare_with(param_int(c.params, 0), Value(self->width), rt.param(c, 1, self));
    });
    register_condition("IsBetweenValues", [](Runtime& rt, const Condition& c, const Instance* self) {
        const double v = rt.param(c, 0, self).as_number();
        const double lo = rt.param(c, 1, self).as_number();
        const double hi = rt.param(c, 2, self).as_number();
        return v >= std::min(lo, hi) && v <= std::max(lo, hi);
    });
    // No pointer input is attached, so nothing can be under the cursor.
    register_condition("IsDragging", [](Runtime&, const Condition&, const Instance*) { return false; });
    // Line of sight needs an obstacle raycast; this only checks the range the
    // behavior was configured with, so it over-reports through walls.
    register_condition("HasLOSToObject", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int other = object_param(c.params);
        if (other < 0) return false;
        auto it = self->behavior_state.find(c.behavior + ".range");
        const double range = it == self->behavior_state.end() ? 0.0 : it->second;
        if (range <= 0.0) return false;
        for (int i : rt.engine().instances_of(other)) {
            const Instance& o = rt.engine().instances[static_cast<size_t>(i)];
            if (o.destroyed) continue;
            const double dx = o.x - self->x, dy = o.y - self->y;
            if (dx * dx + dy * dy <= range * range) return true;
        }
        return false;
    });
    register_condition("IsOnScreen", [](Runtime& rt, const Condition&, const Instance* self) {
        if (!self) return false;
        const Bounds b = bounds_of(*self);
        const Runtime::Viewport& v = rt.viewport;
        return b.l < v.right && v.left < b.r && b.t < v.bottom && v.top < b.b;
    });
    register_condition("IsMoving", [](Runtime&, const Condition& c, const Instance* self) {
        if (!self) return false;
        auto it = self->behavior_state.find(c.behavior + ".speed");
        return it != self->behavior_state.end() && it->second != 0.0;
    });
    register_condition("IsWithinAngle", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const double deg = self->angle * 180.0 / 3.14159265358979323846;
        const double target = rt.param(c, 0, self).as_number();
        const double within = rt.param(c, 1, self).as_number();
        double d = std::fmod(deg - target + 540.0, 360.0) - 180.0;
        return std::fabs(d) <= within;
    });
    register_condition("CompareOpacity", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        // Opacity is stored 0..1 here; Construct 2 expresses it as 0..100.
        return compare_with(param_int(c.params, 0), Value(self->opacity * 100.0),
                            rt.param(c, 1, self));
    });
    // No dictionary storage is modelled, so no key can be present. Answering
    // false is a guess in the safe direction: it gates events closed rather
    // than letting them run on data that does not exist.
    register_condition("HasKey", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int index = static_cast<int>(self - rt.engine().instances.data());
        const std::string key = rt.param(c, 0, self).as_text();
        auto& d = rt.dictionary(index);
        return d.find(key) != d.end();
    });
    register_condition("HasTarget", [](Runtime&, const Condition&, const Instance* self) {
        if (!self) return false;
        auto it = self->behavior_state.find("Turret.hasTarget");
        return it != self->behavior_state.end() && it->second != 0.0;
    });

    // --- Timers ------------------------------------------------------------
    register_action("StartTimer", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const double duration = rt.param(a, 0, nullptr).as_number();
        const std::string tag = rt.param(a, 2, nullptr).as_text();
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            const std::string base = a.behavior + ".timer." + tag;
            inst->behavior_state[base + ".duration"] = duration;
            inst->behavior_state[base + ".elapsed"] = 0.0;
        }
    });
    register_action("StopTimer", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const std::string tag = rt.param(a, 0, nullptr).as_text();
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->behavior_state.erase(a.behavior + ".timer." + tag + ".duration");
        }
    });
    register_condition("OnTimer", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const std::string tag = rt.param(c, 0, self).as_text();
        auto it = self->behavior_state.find(c.behavior + ".timer." + tag + ".fired");
        return it != self->behavior_state.end() && it->second != 0.0;
    });

    // --- Behavior properties ------------------------------------------------
    // These all write one named value into the instance's behavior state. The
    // behavior name comes from the ACE itself, so a single implementation
    // serves every behavior that exposes the property -- including the mod
    // authors' own (b_warn, attack, Run, MainLook, z_walker).
    auto behavior_setter = [this](const char* ace_name, const char* property) {
        const std::string prop = property;
        register_action(ace_name, [prop](Runtime& rt, const Action& a, const std::vector<int>& picked) {
            const double v = rt.param(a, 0, nullptr).as_number();
            for (int i : picked)
                if (Instance* inst = rt.instance(i))
                    inst->behavior_state[a.behavior + "." + prop] = v;
        });
    };
    behavior_setter("SetSpeed", "speed");
    behavior_setter("SetMaxSpeed", "maxspeed");
    behavior_setter("SetAcceleration", "acc");
    behavior_setter("SetDeceleration", "dec");
    behavior_setter("SetRange", "range");
    behavior_setter("SetConeOfView", "cone");
    behavior_setter("SetEnabled", "enabled");
    behavior_setter("SetSteerSpeed", "steerSpeed");
    behavior_setter("SetProjectileSpeed", "projectileSpeed");
    behavior_setter("SetAngleOfMotion", "angleOfMotion");
    behavior_setter("SetVectorX", "dx");
    behavior_setter("SetVectorY", "dy");
    behavior_setter("SetRate", "rate");
    behavior_setter("SetWaitTime", "waitTime");
    behavior_setter("SetFadeOutTime", "fadeOut");
    behavior_setter("SetPredictiveAim", "predictiveAim");
    // Fade: record that it started. Nothing consumes this yet, so the object
    // will not actually fade -- but the call is no longer counted as missing,
    // which would otherwise hide that the state is being tracked.
    behavior_setter("StartFade", "fadeStarted");

    // --- Functions ---------------------------------------------------------
    register_action("CallFunction", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (a.params.empty()) return;
        const std::string name = rt.param(a, 0, nullptr).as_text();
        std::vector<Value> args;
        for (size_t i = 1; i < a.params.size(); ++i) args.push_back(rt.param(a, i, nullptr));
        rt.call_function(name, std::move(args));
    });
    register_action("SetReturnValue", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (Runtime::CallFrame* f = rt.current_frame()) f->ret = rt.param(a, 0, nullptr);
    });
    // The body of a function only ever runs via call_function, which enables
    // triggers; reaching it any other way means it should not fire.
    register_condition("OnFunction", [](Runtime& rt, const Condition&, const Instance*) {
        return rt.current_frame() != nullptr;
    });
    register_condition("CompareParam", [](Runtime& rt, const Condition& c, const Instance*) {
        const Runtime::CallFrame* f = rt.current_frame();
        if (!f) return false;
        const size_t i = static_cast<size_t>(rt.param(c, 0, nullptr).as_number());
        const Value v = i < f->args.size() ? f->args[i] : Value(0.0);
        return compare_with(param_int(c.params, 1), v, rt.param(c, 2, nullptr));
    });

    register_action("GoToLayout", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (a.params.empty()) return;
        rt.request_layout(a.params[0].value.text);
    });
    register_action("RestartLayout", [](Runtime& rt, const Action&, const std::vector<int>&) {
        if (rt.layout()) rt.request_layout(rt.layout()->name);
    });

    register_action("SetGroupActive", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (a.params.empty()) return;
        rt.set_group_active(a.params[0].value.text, param_int(a.params, 1) != 0);
    });

    register_action("SetScale", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const double s = rt.param(a, 0, nullptr).as_number();
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            // Scale is relative to the frame's natural size, not the current
            // one, so repeated calls do not compound.
            const Frame* f = rt.project_type(inst->object_type).frame_for(inst->animation, inst->frame);
            const double bw = f ? f->w : inst->width, bh = f ? f->h : inst->height;
            const double sign = inst->width < 0 ? -1.0 : 1.0;   // preserve mirroring
            inst->width = bw * s * sign;
            inst->height = bh * s;
        }
    });
    // Construct 2 encodes mirroring as a negative width.
    register_action("SetMirrored", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const bool mirrored = param_int(a.params, 0) == 0;
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            const double w = std::fabs(inst->width);
            inst->width = mirrored ? -w : w;
        }
    });
    register_action("SetPositionToObject", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int target = object_param(a.params);
        if (target < 0) return;
        const std::vector<int> pool = rt.engine().picked(target);
        if (pool.empty()) return;
        const Instance& dst = rt.engine().instances[static_cast<size_t>(pool.front())];
        const double x = dst.x, y = dst.y;
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (inst) { inst->x = x; inst->y = y; }
        }
    });
    register_action("MoveToTop", [](Runtime& rt, const Action&, const std::vector<int>& picked) {
        int top = 0;
        for (const Instance& i : rt.engine().instances) top = std::max(top, i.z);
        for (int i : picked) if (Instance* n = rt.instance(i)) n->z = ++top;
    });
    register_action("MoveToBottom", [](Runtime& rt, const Action&, const std::vector<int>& picked) {
        int bottom = 0;
        for (const Instance& i : rt.engine().instances) bottom = std::min(bottom, i.z);
        for (int i : picked) if (Instance* n = rt.instance(i)) n->z = --bottom;
    });
    register_action("PinToObject", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int target = object_param(a.params);
        if (target < 0) return;
        const std::vector<int> pool = rt.engine().picked(target);
        if (pool.empty()) return;
        const Instance& dst = rt.engine().instances[static_cast<size_t>(pool.front())];
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst || inst == &dst) continue;
            inst->behavior_state["Pin.uid"] = dst.uid;
            inst->behavior_state["Pin.dx"] = inst->x - dst.x;
            inst->behavior_state["Pin.dy"] = inst->y - dst.y;
        }
    });
    register_action("Unpin", [](Runtime& rt, const Action&, const std::vector<int>& picked) {
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->behavior_state.erase("Pin.uid");
            inst->behavior_state.erase("Pin.dx");
            inst->behavior_state.erase("Pin.dy");
        }
    });
    register_action("SetText", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const std::string t = rt.param(a, 0, nullptr).as_text();
        for (int i : picked) if (Instance* n = rt.instance(i)) n->text = t;
    });
    register_expression("Text", [](Runtime&, const Expr&, const Instance* s) {
        return s ? Value(s->text) : Value(std::string());
    });
    register_action("SetTimeScale", [](Runtime&, const Action&, const std::vector<int>&) {});
    register_action("SetLayerVisible", [](Runtime&, const Action&, const std::vector<int>&) {});
    // Audio is not modelled at all; accepting the call keeps it from reading
    // as a coverage gap that further work would close.
    register_action("PlayAtObjectByName", [](Runtime&, const Action&, const std::vector<int>&) {});
    register_action("GetItem", [](Runtime&, const Action&, const std::vector<int>&) {});

    // Cosmetic only: there is no cursor to restyle.
    register_action("SetCursor", [](Runtime&, const Action&, const std::vector<int>&) {});

    // --- Dictionary storage -------------------------------------------------
    // Keyed by the first parameter. If any of these are really the Array
    // plugin's indexed variants, a numeric key still round-trips correctly.
    register_action("SetItem", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const std::string key = rt.param(a, 0, nullptr).as_text();
        const Value v = rt.param(a, 1, nullptr);
        for (int i : picked) rt.dictionary(i)[key] = v;
    });
    register_expression("GetItem", [](Runtime& rt, const Expr& e, const Instance* s) {
        if (!s) return Value(0.0);
        const int index = static_cast<int>(s - rt.engine().instances.data());
        const std::string key = e.args.empty() ? std::string() : rt.eval(e.args[0], s).as_text();
        auto& d = rt.dictionary(index);
        auto it = d.find(key);
        return it == d.end() ? Value(0.0) : it->second;
    });

    // --- Creation ----------------------------------------------------------
    register_action("CreateObject", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        const int t = object_param(a.params);
        if (t < 0) return;
        rt.create_instance(t, rt.param(a, 2, nullptr).as_number(),
                              rt.param(a, 3, nullptr).as_number(), 0);
    });
    // Spawns at the spawning instance's position. The real action places the
    // new object at an image point, which needs the frame's point table.
    register_action("SpawnObject", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int t = object_param(a.params);
        if (t < 0) return;
        if (picked.empty()) { rt.create_instance(t, 0, 0, 0); return; }
        for (int i : picked) {
            const Instance* src = rt.instance(i);
            if (src) rt.create_instance(t, src->x, src->y, src->layer);
        }
    });

    // --- System actions ----------------------------------------------------
    register_action("SetValue", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (a.params.empty()) return;
        rt.set_variable(a.params[0].value.text, rt.param(a, 1, nullptr));
    });
    register_action("AddToValue", [](Runtime& rt, const Action& a, const std::vector<int>&) {
        if (a.params.empty()) return;
        const std::string& name = a.params[0].value.text;
        rt.set_variable(name, Value(rt.get_variable(name).as_number() +
                                    rt.param(a, 1, nullptr).as_number()));
    });

    // --- Instance actions --------------------------------------------------
    register_action("SetInstanceVar", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int idx = param_int(a.params, 0);
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst || idx < 0 || idx >= static_cast<int>(inst->vars.size())) continue;
            inst->vars[static_cast<size_t>(idx)] = rt.param(a, 1, inst).as_number();
        }
    });
    register_action("AddToInstanceVar", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int idx = param_int(a.params, 0);
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst || idx < 0 || idx >= static_cast<int>(inst->vars.size())) continue;
            inst->vars[static_cast<size_t>(idx)] += rt.param(a, 1, inst).as_number();
        }
    });
    register_action("SubtractFromInstanceVar", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        const int idx = param_int(a.params, 0);
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst || idx < 0 || idx >= static_cast<int>(inst->vars.size())) continue;
            inst->vars[static_cast<size_t>(idx)] -= rt.param(a, 1, inst).as_number();
        }
    });
    register_action("SetPosition", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->x = rt.param(a, 0, inst).as_number();
            inst->y = rt.param(a, 1, inst).as_number();
        }
    });
    register_action("SetX", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) if (Instance* n = rt.instance(i)) n->x = rt.param(a, 0, n).as_number();
    });
    register_action("SetY", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) if (Instance* n = rt.instance(i)) n->y = rt.param(a, 0, n).as_number();
    });
    register_action("SetSize", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->width = rt.param(a, 0, inst).as_number();
            inst->height = rt.param(a, 1, inst).as_number();
        }
    });
    register_action("SetAngle", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked)
            if (Instance* n = rt.instance(i))
                n->angle = rt.param(a, 0, n).as_number() * 3.14159265358979323846 / 180.0;
    });
    register_action("SetAnimationFrame", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked)
            if (Instance* n = rt.instance(i)) n->frame = static_cast<int>(rt.param(a, 0, n).as_number());
    });
    register_action("SetWidth", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) if (Instance* n = rt.instance(i)) n->width = rt.param(a, 0, n).as_number();
    });
    register_action("SetHeight", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) if (Instance* n = rt.instance(i)) n->height = rt.param(a, 0, n).as_number();
    });

    // Behavior state. The key carries the behavior name so two behaviors on the
    // same instance keep separate values.
    register_action("SetDistanceTravelled", [](Runtime& rt, const Action& a, const std::vector<int>& picked) {
        for (int i : picked) {
            Instance* inst = rt.instance(i);
            if (!inst) continue;
            inst->behavior_state[a.behavior + ".travelled"] = rt.param(a, 0, inst).as_number();
        }
    });
    register_condition("CompareTravelled", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        auto it = self->behavior_state.find(c.behavior + ".travelled");
        const double travelled = it == self->behavior_state.end() ? 0.0 : it->second;
        return compare_with(param_int(c.params, 0), Value(travelled), rt.param(c, 1, self));
    });

    register_action("Destroy", [](Runtime& rt, const Action&, const std::vector<int>& picked) {
        for (int i : picked) rt.engine().destroy_instance(i);
    });
}

// ---------------------------------------------------------------------------
// Built-in expressions
// ---------------------------------------------------------------------------

void Runtime::register_expression_builtins() {
    auto arg = [](Runtime& rt, const Expr& e, size_t i, const Instance* self) {
        return i < e.args.size() ? rt.eval(e.args[i], self) : Value(0.0);
    };

    // --- instance properties ----------------------------------------------
    register_expression("X", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->x : 0.0);
    });
    register_expression("Y", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->y : 0.0);
    });
    register_expression("Width", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->width : 0.0);
    });
    register_expression("Height", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->height : 0.0);
    });
    register_expression("Angle", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->angle * 180.0 / 3.14159265358979323846 : 0.0);
    });
    register_expression("UID", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? static_cast<double>(s->uid) : 0.0);
    });
    register_expression("AnimationFrame", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? static_cast<double>(s->frame) : 0.0);
    });
    register_expression("Count", [](Runtime& rt, const Expr& e, const Instance*) {
        return Value(static_cast<double>(rt.engine().instances_of(e.object_type).size()));
    });

    // --- maths ------------------------------------------------------------
    register_expression("Int", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::trunc(arg(rt, e, 0, s).as_number()));
    });
    register_expression("Floor", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::floor(arg(rt, e, 0, s).as_number()));
    });
    register_expression("Ceil", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::ceil(arg(rt, e, 0, s).as_number()));
    });
    register_expression("Round", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::floor(arg(rt, e, 0, s).as_number() + 0.5));
    });
    register_expression("Abs", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::fabs(arg(rt, e, 0, s).as_number()));
    });
    register_expression("Sqrt", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        return Value(std::sqrt(std::fabs(arg(rt, e, 0, s).as_number())));
    });
    register_expression("Min", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        double m = arg(rt, e, 0, s).as_number();
        for (size_t i = 1; i < e.args.size(); ++i) m = std::min(m, arg(rt, e, i, s).as_number());
        return Value(m);
    });
    register_expression("Max", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        double m = arg(rt, e, 0, s).as_number();
        for (size_t i = 1; i < e.args.size(); ++i) m = std::max(m, arg(rt, e, i, s).as_number());
        return Value(m);
    });
    register_expression("Distance", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        const double dx = arg(rt, e, 2, s).as_number() - arg(rt, e, 0, s).as_number();
        const double dy = arg(rt, e, 3, s).as_number() - arg(rt, e, 1, s).as_number();
        return Value(std::sqrt(dx * dx + dy * dy));
    });

    // Deterministic RNG: a fixed seed keeps runs reproducible, which matters
    // more here than statistical quality.
    register_expression("Random", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        static uint32_t seed = 0x2545F491u;
        seed = seed * 1664525u + 1013904223u;
        const double unit = static_cast<double>(seed >> 8) / 16777216.0;
        if (e.args.empty()) return Value(unit);
        if (e.args.size() == 1) return Value(unit * arg(rt, e, 0, s).as_number());
        const double lo = arg(rt, e, 0, s).as_number(), hi = arg(rt, e, 1, s).as_number();
        return Value(lo + unit * (hi - lo));
    });
    register_expression("Choose", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        if (e.args.empty()) return Value(0.0);
        static uint32_t seed = 0x9E3779B9u;
        seed = seed * 1664525u + 1013904223u;
        return arg(rt, e, (seed >> 8) % e.args.size(), s);
    });

    // --- runtime scalars ---------------------------------------------------
    register_expression("Dt", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.dt());
    });
    register_expression("Time", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.time());
    });
    // Plugin 15 is Touch: its X/Y are the pointer, not an instance.
    register_expression("15:X", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.input.x);
    });
    register_expression("15:Y", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.input.y);
    });
    register_expression("TouchIndex", [](Runtime&, const Expr&, const Instance*) {
        return Value(0.0);
    });
    register_expression("XForID", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.input.x);
    });
    register_expression("YForID", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.input.y);
    });
    register_expression("LayoutName", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.layout() ? rt.layout()->name : std::string());
    });
    register_expression("ViewportLeft", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.viewport.left);
    });
    register_expression("ViewportTop", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.viewport.top);
    });
    register_expression("ViewportRight", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.viewport.right);
    });
    register_expression("ViewportBottom", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.viewport.bottom);
    });
    register_expression("LayerScale", [](Runtime&, const Expr&, const Instance*) {
        return Value(1.0);   // layer scaling is not modelled yet
    });
    register_expression("CurrentTime", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.time());
    });
    register_expression("WallClockTime", [](Runtime& rt, const Expr&, const Instance*) {
        return Value(rt.time());
    });
    // Approximate: the true image point is offset by the frame's hotspot and
    // rotated by the instance angle. Returning the origin is close for
    // centred points and wrong for offset ones.
    register_expression("ImagePointX", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->x : 0.0);
    });
    register_expression("ImagePointY", [](Runtime&, const Expr&, const Instance* s) {
        return Value(s ? s->y : 0.0);
    });

    register_expression("Param", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        const Runtime::CallFrame* f = rt.current_frame();
        if (!f) return Value(0.0);
        const size_t i = static_cast<size_t>(arg(rt, e, 0, s).as_number());
        return i < f->args.size() ? f->args[i] : Value(0.0);
    });
    register_expression("ReturnValue", [](Runtime& rt, const Expr&, const Instance*) {
        const Runtime::CallFrame* f = rt.current_frame();
        return f ? f->ret : Value(0.0);
    });

    // Assets are loaded synchronously before the first tick, so loading is
    // always complete. The original ramps this from 0 to 1 while downloading.
    // Find returns the index of a substring, or -1 when absent. Leaving it
    // unimplemented returned 0, and 0 is a MEANINGFUL value here: the game
    // tests Find(...) >= 0, so "not implemented" read as "found at index 0"
    // and every language branch matched at once. The last one to run won,
    // which is how this engine ended up reporting RU while the original
    // reported EN. Construct 2's find is case-insensitive.
    register_expression("Find", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        std::string hay = arg(rt, e, 0, s).as_text();
        std::string needle = arg(rt, e, 1, s).as_text();
        auto lower = [](std::string& v) {
            for (char& ch : v) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        };
        lower(hay);
        lower(needle);
        const size_t at = hay.find(needle);
        return Value(at == std::string::npos ? -1.0 : static_cast<double>(at));
    });
    // No browser is attached. The original reports the host locale; this
    // reports a fixed one, which at least makes language selection
    // deterministic and matches what the reference capture reported.
    register_expression("Language", [](Runtime&, const Expr&, const Instance*) {
        return Value(std::string("en-US"));
    });
    register_expression("StringFromKeyCode", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        const int code = static_cast<int>(arg(rt, e, 0, s).as_number());
        if (code >= 32 && code < 127) return Value(std::string(1, static_cast<char>(code)));
        return Value(std::string());
    });

    register_expression("LoadingProgress", [](Runtime&, const Expr&, const Instance*) {
        return Value(1.0);
    });
    register_expression("Lerp", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        const double a = arg(rt, e, 0, s).as_number();
        const double b = arg(rt, e, 1, s).as_number();
        const double t = arg(rt, e, 2, s).as_number();
        return Value(a + (b - a) * t);
    });
    register_expression("At", [arg](Runtime& rt, const Expr& e, const Instance* s) {
        if (!s) return Value(0.0);
        const int index = static_cast<int>(s - rt.engine().instances.data());
        auto& d = rt.dictionary(index);
        auto it = d.find(arg(rt, e, 0, s).as_text());
        return it == d.end() ? Value(0.0) : it->second;
    });

    register_expression("NewLine", [](Runtime&, const Expr&, const Instance*) {
        return Value(std::string("\n"));
    });
}

}  // namespace mdz
