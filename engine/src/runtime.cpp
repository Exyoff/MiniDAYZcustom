#include "runtime.hpp"

#include <cmath>

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

int param_int(const std::vector<Param>& params, size_t i, int fallback = 0) {
    if (i >= params.size()) return fallback;
    return static_cast<int>(params[i].value.number);
}

}  // namespace

Runtime::Runtime(const Project& project, const AceNames& names)
    : project_(project), names_(names), engine_(project) {
    register_builtins();
}

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

void Runtime::load_layout(const Layout& layout) {
    layout_ = &layout;
    engine_.load_layout(layout);

    // Seed variables from every sheet: an event sheet can read variables that
    // another sheet declares, because sheets include one another.
    for (const EventSheet& s : project_.sheets) {
        for (const EventVariable& v : s.variables) {
            variables_[v.name] = v.is_text ? Value(v.initial_text) : Value(v.initial_number);
        }
        if (s.name == layout.event_sheet) sheet_ = &s;
    }
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
            ++stats_.unknown_expressions;
            return Value(0.0);

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
            // Unimplemented conditions pass. The alternative -- failing --
            // would stop almost every event from running and make coverage
            // unmeasurable. But it cuts the other way: events fire that the
            // real game would gate, so a run with unimplemented conditions
            // present is an UPPER BOUND on execution, not a faithful
            // simulation. Variable changes observed under it are not evidence
            // of correct behaviour until the gating conditions are real.
            return true;
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
            return true;
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

    // "For each" names its target through an object parameter (slot tag 4).
    hooks.loop_target = [](const Condition& c) {
        for (const Param& p : c.params)
            if (p.tag == 4) return static_cast<int>(p.value.number);
        return -1;
    };
    return hooks;
}

void Runtime::tick(double dt_seconds) {
    dt_ = dt_seconds;
    time_ += dt_seconds;
    if (!sheet_) return;
    RunnerHooks hooks = make_hooks();
    EventRunner runner(engine_, hooks);
    runner.run_sheet(*sheet_);
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
    register_condition("IsGroupActive", [](Runtime&, const Condition&, const Instance*) {
        return true;   // group activation is not modelled yet
    });

    // --- Instance conditions ----------------------------------------------
    register_condition("CompareInstanceVar", [](Runtime& rt, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int idx = param_int(c.params, 0);
        if (idx < 0 || idx >= static_cast<int>(self->vars.size())) return false;
        return compare_with(param_int(c.params, 1), Value(self->vars[static_cast<size_t>(idx)]),
                            rt.param(c, 2, self));
    });
    register_condition("IsBooleanInstanceVar", [](Runtime&, const Condition& c, const Instance* self) {
        if (!self) return false;
        const int idx = param_int(c.params, 0);
        if (idx < 0 || idx >= static_cast<int>(self->vars.size())) return false;
        return self->vars[static_cast<size_t>(idx)] != 0.0;
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

}  // namespace mdz
