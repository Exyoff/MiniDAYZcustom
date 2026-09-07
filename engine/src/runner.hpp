// Walks event blocks and drives the picking engine.
//
// The runner owns control flow -- condition order, OR blocks, sub-event
// scoping, "for each" loops -- and delegates every actual decision to
// callbacks. That keeps it independent of which plugin ACEs have been
// implemented, so the picking semantics can be exercised on the real event
// data long before any plugin exists.
#pragma once

#include <functional>
#include <vector>

#include "picking.hpp"
#include "project.hpp"

namespace mdz {

// Called with the instances an action applies to. In a finished engine this
// is where the ACE implementation runs; here it is also the observation point
// the tests assert on.
using ActionSink = std::function<void(const Action&, const std::vector<int>& applies_to)>;

// A "for each"-style looping condition names its target object type through a
// parameter. This resolves that target; return -1 if the loop is not
// instance-based (e.g. "Repeat N times").
using LoopTargetResolver = std::function<int(const Condition&)>;

struct RunnerHooks {
    InstancePredicate instance_condition;
    SystemPredicate system_condition;
    ActionSink action;
    LoopTargetResolver loop_target;

    // "Else" passes when the preceding sibling event failed, so the runner
    // reports each sibling's outcome as it goes. Called with false when a new
    // sibling list begins, so an Else with nothing before it does not inherit
    // a stale result from an unrelated branch.
    std::function<void(bool)> sibling_result;
};

struct RunStats {
    size_t blocks_entered = 0;
    size_t blocks_passed = 0;
    size_t conditions_evaluated = 0;
    size_t actions_run = 0;
    size_t or_blocks = 0;
    size_t foreach_loops = 0;
    size_t triggers_skipped = 0;
    int deepest_scope = 0;
};

class EventRunner {
public:
    EventRunner(PickingEngine& engine, RunnerHooks hooks);

    // Runs every top-level block of a sheet, as one tick would.
    void run_sheet(const EventSheet& sheet);

    // Runs a single block. Returns whether its conditions passed.
    bool run_block(const EventBlock& block, int depth);

    const RunStats& stats() const { return stats_; }
    void reset_stats() { stats_ = RunStats{}; }

    // Trigger blocks are skipped during a normal tick; they fire only when
    // their event actually occurs. Set this to run them anyway (tests do).
    bool run_triggers = false;

private:
    PickingEngine& engine_;
    RunnerHooks hooks_;
    RunStats stats_;

    // Evaluates a block's conditions in order. `skip` lets the for-each path
    // exclude the looping condition, which iterates rather than filters.
    bool evaluate_conditions(const EventBlock& block, const Condition* skip = nullptr);
    void run_actions_and_subevents(const EventBlock& block, int depth);
    void run_foreach(const EventBlock& block, const Condition& loop, int depth);
};

}  // namespace mdz
