// Ties the pieces together: project data, live instances, the picking engine,
// variables, expressions, and the ACE implementations events actually invoke.
//
// ACEs dispatch BY NAME, not by numeric index. Every one of the 423 ACEs in
// this project resolves to a name (see data/ace_names.txt), so implementing
// "SetPosition" once covers every call site that resolves to it, and adding a
// new one means writing a function and registering it under its name. The
// numeric indices stay an implementation detail of the export format.
//
// Anything not yet implemented is counted rather than silently ignored, so
// `Stats` says exactly how much of the game is actually running.
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "decompile.hpp"
#include "picking.hpp"
#include "project.hpp"
#include "runner.hpp"
#include "value.hpp"

namespace mdz {

class Runtime {
public:
    Runtime(const Project& project, const AceNames& names);

    // Creates the layout's instances and seeds every sheet's variables.
    void load_layout(const Layout& layout);

    // Runs one frame of the layout's event sheet.
    void tick(double dt_seconds);

    // --- state -------------------------------------------------------------
    PickingEngine& engine() { return engine_; }
    const PickingEngine& engine() const { return engine_; }

    Value get_variable(const std::string& name) const;
    void set_variable(const std::string& name, const Value& v);
    const std::unordered_map<std::string, Value>& variables() const { return variables_; }

    double time() const { return time_; }
    double dt() const { return dt_; }

    // The visible region. Viewport expressions are read by startup logic that
    // sizes the UI, so a sensible default matters even with no window attached.
    struct Viewport { double left = 0, top = 0, right = 1024, bottom = 768; };
    Viewport viewport;
    const Layout* layout() const { return layout_; }

    // --- expressions -------------------------------------------------------
    // `self` supplies the instance context for instance-variable references.
    Value eval(const Expr& e, const Instance* self) const;

    // --- ACE implementations ----------------------------------------------
    // Conditions answer for one instance; system conditions get no instance.
    using ConditionFn = std::function<bool(Runtime&, const Condition&, const Instance*)>;
    // Actions receive the instances the picking engine selected.
    using ActionFn = std::function<void(Runtime&, const Action&, const std::vector<int>&)>;

    // Expressions return a value. `self` is the instance in context, when
    // there is one.
    using ExpressionFn = std::function<Value(Runtime&, const Expr&, const Instance*)>;

    void register_condition(const std::string& name, ConditionFn fn);
    void register_action(const std::string& name, ActionFn fn);
    void register_expression(const std::string& name, ExpressionFn fn);

    // Name an ACE the way the decompiler would, for dispatch and diagnostics.
    std::string condition_name(const Condition& c) const;
    std::string action_name(const Action& a) const;

    RunnerHooks make_hooks();

    // "Trigger once while true": true only on the first tick a given call site
    // asks, identified by its SID.
    bool trigger_once(long long sid);

    // "Every X seconds" accumulates per call site, so it needs the SID too.
    bool every_seconds(long long sid, double interval);

    // Whether the event immediately before this one, at the same level, passed.
    bool last_sibling_passed() const { return last_sibling_passed_; }

    // --- functions ---------------------------------------------------------
    // A function call runs the events under the matching OnFunction trigger.
    // Those events are triggers, so the normal top-down pass skips them; they
    // execute only from here. Calls nest, so parameters and the return value
    // live on a stack rather than in one slot.
    // Named CallFrame, not Frame: Frame is already an animation frame, and a
    // nested type of that name shadows it inside every member function here.
    struct CallFrame {
        std::vector<Value> args;
        Value ret;
    };
    Value call_function(const std::string& name, std::vector<Value> args);
    const CallFrame* current_frame() const { return frames_.empty() ? nullptr : &frames_.back(); }
    CallFrame* current_frame() { return frames_.empty() ? nullptr : &frames_.back(); }

    // Creates an instance at runtime and picks it, as CreateObject does.
    int create_instance(int object_type, double x, double y, int layer);

    struct Stats {
        size_t conditions_run = 0, actions_run = 0;
        size_t unimplemented_conditions = 0, unimplemented_actions = 0;
        size_t unknown_expressions = 0;
        std::unordered_map<std::string, size_t> missing_conditions;
        std::unordered_map<std::string, size_t> missing_actions;
        std::unordered_map<std::string, size_t> missing_expressions;
    };
    const Stats& stats() const { return stats_; }
    void reset_stats() { stats_ = Stats{}; }

    // Helpers used by the ACE implementations.
    Value param(const Action& a, size_t index, const Instance* self) const;
    Value param(const Condition& c, size_t index, const Instance* self) const;
    Instance* instance(int index);

private:
    const Project& project_;
    const AceNames& names_;
    PickingEngine engine_;
    const Layout* layout_ = nullptr;
    const EventSheet* sheet_ = nullptr;

    std::unordered_map<std::string, Value> variables_;
    std::unordered_map<std::string, ConditionFn> conditions_;
    std::unordered_map<std::string, ActionFn> actions_;
    std::unordered_map<std::string, ExpressionFn> expressions_;

    // Function name -> the events under its OnFunction trigger.
    std::unordered_map<std::string, std::vector<const EventBlock*>> functions_;
    std::vector<CallFrame> frames_;
    int next_uid_ = 1;

    std::unordered_set<long long> fired_;
    std::unordered_map<long long, double> accumulators_;
    bool last_sibling_passed_ = false;
    double time_ = 0.0, dt_ = 0.0;
    mutable Stats stats_;

    void index_functions();
    // Advances behavior timers and records which fired this tick.
    void update_timers(double dt);
    void register_builtins();
    void register_expression_builtins();
    // Resolves a call node to its name, then to a handler.
    Value call_expression(const Expr& e, const Instance* self) const;
};

}  // namespace mdz
