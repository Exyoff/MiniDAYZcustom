// Semantic tests for the picking engine.
//
// Each test encodes a documented Construct 2 picking rule. They run against a
// small hand-built project rather than the real export, so a failure points at
// a specific rule instead of at 15,000 events.

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "../src/picking.hpp"
#include "../src/runner.hpp"

using namespace mdz;

namespace {

int g_failures = 0;
int g_checks = 0;

void check(bool ok, const std::string& what) {
    ++g_checks;
    if (!ok) {
        ++g_failures;
        std::printf("  FAIL  %s\n", what.c_str());
    }
}

void check_set(const std::vector<int>& got, const std::vector<int>& want,
               const std::string& what) {
    std::vector<int> a = got, b = want;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ++g_checks;
    if (a != b) {
        ++g_failures;
        std::printf("  FAIL  %s\n        got  {", what.c_str());
        for (int i : a) std::printf("%d ", i);
        std::printf("}\n        want {");
        for (int i : b) std::printf("%d ", i);
        std::printf("}\n");
    }
}

// --- a tiny world ----------------------------------------------------------
//
//   type 0: Zombie   (3 instances)
//   type 1: Survivor (2 instances)
//   type 2: family "Actor" = {Zombie, Survivor}
//
// Instance variable 0 is used as "health" throughout.

constexpr int kZombie = 0, kSurvivor = 1, kActor = 2;

Project make_project() {
    Project p;
    for (int i = 0; i < 3; ++i) {
        ObjectType t;
        t.index = i;
        t.name = "t" + std::to_string(i);
        t.instance_var_count = 1;
        p.object_types.push_back(t);
    }
    p.object_types[kZombie].derived_name = "zombie";
    p.object_types[kSurvivor].derived_name = "survivor";
    p.object_types[kActor].derived_name = "actor";

    p.object_types[kActor].is_family = true;
    p.object_types[kActor].family_members = {kZombie, kSurvivor};
    p.object_types[kZombie].member_of = {kActor};
    p.object_types[kSurvivor].member_of = {kActor};
    return p;
}

// Fills the engine with 3 zombies (health 10/20/30) and 2 survivors (40/50).
void populate(PickingEngine& e) {
    for (int i = 0; i < 3; ++i) {
        Instance z;
        z.object_type = kZombie;
        z.uid = 100 + i;
        z.vars = {10.0 * (i + 1)};
        e.add_instance(z);
    }
    for (int i = 0; i < 2; ++i) {
        Instance s;
        s.object_type = kSurvivor;
        s.uid = 200 + i;
        s.vars = {40.0 + 10.0 * i};
        e.add_instance(s);
    }
}

// A stand-in for a real ACE: "instance variable 0 < params[0]" when ace == 1,
// "instance variable 0 > params[0]" when ace == 2.
Condition make_cond(int object_type, int ace, double threshold, bool inverted = false) {
    Condition c;
    c.object_type = object_type;
    c.ace = ace;
    c.inverted = inverted;
    Param p;
    p.value.op = ExpOp::Int;
    p.value.number = threshold;
    c.params.push_back(p);
    return c;
}

InstancePredicate make_predicate() {
    return [](const Condition& c, const Instance& inst) {
        double threshold = c.params.empty() ? 0.0 : c.params[0].value.number;
        double health = inst.vars.empty() ? 0.0 : inst.vars[0];
        if (c.ace == 1) return health < threshold;
        if (c.ace == 2) return health > threshold;
        return true;
    };
}

// --- the rules -------------------------------------------------------------

void test_first_condition_sees_everything() {
    std::printf("rule 2a: first condition picks from all instances\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);

    check_set(e.picked(kZombie), {0, 1, 2}, "unfiltered SOL selects all zombies");
    e.apply_condition(make_cond(kZombie, 1, 25.0), make_predicate());
    check_set(e.picked(kZombie), {0, 1}, "health < 25 picks the first two zombies");
}

void test_conditions_narrow_in_sequence() {
    std::printf("rule 2b: later conditions only see earlier picks\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);
    auto pred = make_predicate();

    e.apply_condition(make_cond(kZombie, 1, 25.0), pred);   // health < 25 -> {0,1}
    e.apply_condition(make_cond(kZombie, 2, 15.0), pred);   // health > 15 -> {1}
    check_set(e.picked(kZombie), {1}, "sequential conditions intersect");

    bool still_true = e.apply_condition(make_cond(kZombie, 2, 999.0), pred);
    check(!still_true, "a condition that matches nothing fails the event");
    check_set(e.picked(kZombie), {}, "and leaves the SOL empty");
}

void test_inverted_is_a_gate_not_a_filter() {
    std::printf("rule 3: inverted conditions gate, they do not pick\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);
    auto pred = make_predicate();

    // "Zombie health is NOT > 999" -- true, because none are.
    bool passed = e.apply_condition(make_cond(kZombie, 2, 999.0, true), pred);
    check(passed, "inverted condition passes when nothing matches");
    check_set(e.picked(kZombie), {0, 1, 2},
              "inverted condition leaves every zombie picked");

    // "Zombie health is NOT > 15" -- false, because two of them are.
    bool failed = e.apply_condition(make_cond(kZombie, 2, 15.0, true), pred);
    check(!failed, "inverted condition fails when something matches");
    check_set(e.picked(kZombie), {0, 1, 2}, "and still does not narrow the SOL");
}

void test_subevents_do_not_leak_to_siblings() {
    std::printf("rule 4: sub-events inherit a copy, siblings stay independent\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);
    auto pred = make_predicate();

    e.apply_condition(make_cond(kZombie, 1, 35.0), pred);   // -> {0,1,2}

    e.push_scope();
    e.apply_condition(make_cond(kZombie, 1, 15.0), pred);   // -> {0}
    check_set(e.picked(kZombie), {0}, "sub-event narrows within its scope");
    e.pop_scope();

    check_set(e.picked(kZombie), {0, 1, 2},
              "parent scope is unchanged after the sub-event");

    e.push_scope();
    check_set(e.picked(kZombie), {0, 1, 2}, "the next sibling starts from the parent");
    e.pop_scope();
}

void test_or_block_unions_picks() {
    std::printf("rule 5: OR blocks union instead of intersect\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);

    std::vector<Condition> conds = {
        make_cond(kZombie, 1, 15.0),   // health < 15 -> {0}
        make_cond(kZombie, 2, 25.0),   // health > 25 -> {2}
    };
    bool ok = e.apply_or_block(conds, make_predicate(), [](const Condition&) { return false; });
    check(ok, "OR block passes when a branch is true");
    check_set(e.picked(kZombie), {0, 2}, "OR block picks the union of both branches");
}

void test_family_and_member_picking_stay_consistent() {
    std::printf("rule 6: families and members share picking\n");
    {
        Project p = make_project();
        PickingEngine e(p);
        populate(e);
        check_set(e.instances_of(kActor), {0, 1, 2, 3, 4}, "family covers both members");

        // Picking on the family must narrow the concrete types too.
        e.apply_condition(make_cond(kActor, 1, 35.0), make_predicate());
        check_set(e.picked(kActor), {0, 1, 2}, "family picks the low-health actors");
        check_set(e.picked(kZombie), {0, 1, 2}, "member type sees the family's pick");
        check_set(e.picked(kSurvivor), {}, "the other member is emptied");
    }
    {
        Project p = make_project();
        PickingEngine e(p);
        populate(e);

        // Picking on a member must narrow the family, without disturbing the
        // family's other members.
        e.apply_condition(make_cond(kZombie, 1, 15.0), make_predicate());
        check_set(e.picked(kZombie), {0}, "member narrows itself");
        check_set(e.picked(kActor), {0, 3, 4},
                  "family keeps the member's pick plus untouched survivors");
    }
}

void test_reset_between_top_level_events() {
    std::printf("rule 1: each top-level event starts unfiltered\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);

    e.apply_condition(make_cond(kZombie, 1, 15.0), make_predicate());
    check_set(e.picked(kZombie), {0}, "narrowed during the event");
    e.reset_all();
    check_set(e.picked(kZombie), {0, 1, 2}, "reset restores all instances");
}

// --- runner-level behaviour ------------------------------------------------

void test_actions_apply_only_to_picked_instances() {
    std::printf("runner: actions receive exactly the picked instances\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);

    EventBlock block;
    block.conditions.push_back(make_cond(kZombie, 1, 25.0));   // -> {0,1}
    Action a;
    a.object_type = kZombie;
    a.ace = 7;
    block.actions.push_back(a);

    std::vector<int> applied;
    RunnerHooks hooks;
    hooks.instance_condition = make_predicate();
    hooks.system_condition = [](const Condition&) { return true; };
    hooks.action = [&](const Action&, const std::vector<int>& to) { applied = to; };
    hooks.loop_target = [](const Condition&) { return -1; };

    EventRunner runner(e, hooks);
    runner.run_block(block, 0);
    check_set(applied, {0, 1}, "action applied to the two picked zombies");
}

void test_foreach_isolates_each_instance() {
    std::printf("runner: for-each runs the body once per instance\n");
    Project p = make_project();
    PickingEngine e(p);
    populate(e);

    EventBlock block;
    Condition loop = make_cond(kZombie, 0, 0.0);
    loop.looping = true;
    block.conditions.push_back(loop);
    Action a;
    a.object_type = kZombie;
    block.actions.push_back(a);

    std::vector<std::vector<int>> per_pass;
    RunnerHooks hooks;
    hooks.instance_condition = make_predicate();
    hooks.system_condition = [](const Condition&) { return true; };
    hooks.action = [&](const Action&, const std::vector<int>& to) { per_pass.push_back(to); };
    hooks.loop_target = [](const Condition&) { return -1; };

    EventRunner runner(e, hooks);
    runner.run_block(block, 0);

    check(per_pass.size() == 3, "for-each ran once per zombie");
    if (per_pass.size() == 3) {
        check_set(per_pass[0], {0}, "pass 1 sees only zombie 0");
        check_set(per_pass[1], {1}, "pass 2 sees only zombie 1");
        check_set(per_pass[2], {2}, "pass 3 sees only zombie 2");
    }
}

}  // namespace

int main() {
    std::printf("=== picking engine semantics ===\n\n");
    test_reset_between_top_level_events();
    test_first_condition_sees_everything();
    test_conditions_narrow_in_sequence();
    test_inverted_is_a_gate_not_a_filter();
    test_subevents_do_not_leak_to_siblings();
    test_or_block_unions_picks();
    test_family_and_member_picking_stay_consistent();
    test_actions_apply_only_to_picked_instances();
    test_foreach_isolates_each_instance();

    std::printf("\n%d checks, %d failures\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
