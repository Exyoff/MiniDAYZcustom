#include "runner.hpp"

#include <algorithm>

namespace mdz {

EventRunner::EventRunner(PickingEngine& engine, RunnerHooks hooks)
    : engine_(engine), hooks_(std::move(hooks)) {}

void EventRunner::run_sheet(const EventSheet& sheet) {
    if (hooks_.sibling_result) hooks_.sibling_result(false);
    for (const EventBlock& block : sheet.blocks) {
        // Rule 1: each top-level event starts from a clean slate.
        engine_.reset_all();
        const bool passed = run_block(block, 0);
        if (hooks_.sibling_result) hooks_.sibling_result(passed);
    }
}

bool EventRunner::run_block(const EventBlock& block, int depth) {
    ++stats_.blocks_entered;
    stats_.deepest_scope = std::max(stats_.deepest_scope, depth);

    // A disabled group and everything under it is skipped entirely.
    if (block.is_group && hooks_.group_active && !hooks_.group_active(block.group_name))
        return false;

    // A group is a container: it has no conditions of its own to satisfy.
    if (block.is_group && block.conditions.empty()) {
        run_actions_and_subevents(block, depth);
        ++stats_.blocks_passed;
        return true;
    }

    // Trigger-driven blocks do not run as part of the normal top-down pass.
    const bool has_trigger =
        std::any_of(block.conditions.begin(), block.conditions.end(),
                    [](const Condition& c) { return c.trigger_mode != 0; });
    if (has_trigger && !run_triggers) {
        ++stats_.triggers_skipped;
        return false;
    }

    // A looping condition turns the block into an iteration over its picks.
    auto loop_it = std::find_if(block.conditions.begin(), block.conditions.end(),
                                [](const Condition& c) { return c.looping; });
    if (loop_it != block.conditions.end()) {
        run_foreach(block, *loop_it, depth);
        ++stats_.blocks_passed;
        return true;
    }

    if (!evaluate_conditions(block)) return false;

    ++stats_.blocks_passed;
    run_actions_and_subevents(block, depth);
    return true;
}

bool EventRunner::evaluate_conditions(const EventBlock& block, const Condition* skip) {
    stats_.conditions_evaluated += block.conditions.size();

    if (block.is_or_block) {
        ++stats_.or_blocks;
        return engine_.apply_or_block(block.conditions, hooks_.instance_condition,
                                      hooks_.system_condition);
    }

    // Rule 2: conditions apply in order, each narrowing what the next sees.
    for (const Condition& c : block.conditions) {
        if (&c == skip) continue;
        if (c.object_type < 0) {
            bool ok = hooks_.system_condition(c);
            if (c.inverted) ok = !ok;
            if (!ok) return false;
        } else if (!engine_.apply_condition(c, hooks_.instance_condition)) {
            return false;
        }
    }
    return true;
}

void EventRunner::run_actions_and_subevents(const EventBlock& block, int depth) {
    for (const Action& a : block.actions) {
        ++stats_.actions_run;
        // An action applies to whatever its object type currently has picked.
        hooks_.action(a, a.object_type < 0 ? std::vector<int>{}
                                           : engine_.picked(a.object_type));
    }

    // Rule 4: sub-events inherit a copy of the SOL; siblings stay independent.
    if (hooks_.sibling_result) hooks_.sibling_result(false);
    for (const EventBlock& sub : block.subevents) {
        engine_.push_scope();
        const bool passed = run_block(sub, depth + 1);
        engine_.pop_scope();
        if (hooks_.sibling_result) hooks_.sibling_result(passed);
    }
}

void EventRunner::run_foreach(const EventBlock& block, const Condition& loop, int depth) {
    ++stats_.foreach_loops;

    int target = loop.object_type >= 0 ? loop.object_type
                                       : (hooks_.loop_target ? hooks_.loop_target(loop) : -1);
    if (target < 0) {
        // A count-based loop ("Repeat N times"): no per-instance picking, so
        // just run the body once. A full engine would run it N times.
        if (evaluate_conditions(block, &loop)) run_actions_and_subevents(block, depth);
        return;
    }

    // Iterate the instances picked *before* the loop, one at a time, so the
    // body sees exactly one instance selected on each pass.
    const std::vector<int> subjects = engine_.picked(target);
    for (int instance : subjects) {
        engine_.push_scope();
        engine_.pick_single(target, instance);

        if (evaluate_conditions(block, &loop)) run_actions_and_subevents(block, depth);
        engine_.pop_scope();
    }
}

}  // namespace mdz
