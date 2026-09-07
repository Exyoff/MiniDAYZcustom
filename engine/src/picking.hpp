// The picking engine: Construct 2's "selected object list" (SOL) semantics.
//
// This is the part of the runtime that decides *which instances* a condition
// filtered down to and therefore which instances the following actions apply
// to. Almost all behavioural fidelity in a C2 game lives here rather than in
// the individual actions, so it is deliberately isolated and independently
// testable: the engine never evaluates a real condition itself, it calls out
// to a predicate supplied by the caller.
//
// The rules implemented, as Construct 2 defines them:
//
//   1. Every object type has a SOL. At the start of each top-level event all
//      SOLs reset to "all instances selected".
//   2. A condition on type T filters T's SOL. The first condition sees every
//      instance; later conditions only see what earlier ones left.
//   3. An *inverted* condition is a gate, not a filter. It passes when nothing
//      matches, and leaves the SOL untouched.
//   4. Sub-events inherit a copy of the parent SOL. Filtering inside a
//      sub-event cannot leak back out to siblings.
//   5. An OR block unions the picks of each true condition instead of
//      intersecting them.
//   6. Picking a family picks its members, and picking a member narrows the
//      families it belongs to.
#pragma once

#include <functional>
#include <vector>

#include "project.hpp"

namespace mdz {

struct Sol {
    bool select_all = true;             // true => `instances` is ignored
    std::vector<int> instances;         // indices into PickingEngine::instances
    std::vector<int> else_instances;    // what the last filter rejected
};

// Decides whether one instance satisfies one condition. Supplied by the
// caller so the picking rules can be tested without any plugin code.
using InstancePredicate = std::function<bool(const Condition&, const Instance&)>;

// Evaluates a condition that does not belong to an object (the System object).
using SystemPredicate = std::function<bool(const Condition&)>;

class PickingEngine {
public:
    explicit PickingEngine(const Project& project);

    // --- world -------------------------------------------------------------
    int add_instance(const Instance& inst);   // returns the instance index
    void load_layout(const Layout& layout);

    std::vector<Instance> instances;

    // Instances of a concrete type, or of every member of a family.
    const std::vector<int>& instances_of(int object_type) const;

    // --- SOL state ---------------------------------------------------------
    void reset_all();                 // start of a top-level event
    void push_scope();                // entering a sub-event
    void pop_scope();                 // leaving a sub-event

    Sol& sol(int object_type);
    const Sol& sol(int object_type) const;

    // The instances a condition or action currently applies to.
    std::vector<int> picked(int object_type) const;

    // Forces a specific pick, as a trigger does with the instance that fired.
    void pick_single(int object_type, int instance_index);
    void pick_set(int object_type, const std::vector<int>& set);

    // --- the rules ---------------------------------------------------------

    // Rule 2/3: filter (or gate) the SOL. Returns whether the event continues.
    bool apply_condition(const Condition& c, const InstancePredicate& pred);

    // Rule 5: evaluate an OR block's conditions, unioning their picks.
    bool apply_or_block(const std::vector<Condition>& conditions,
                        const InstancePredicate& pred,
                        const SystemPredicate& sys_pred);

    const Project& project() const { return project_; }

private:
    const Project& project_;
    std::vector<std::vector<int>> by_type_;   // concrete type -> instance indices
    mutable std::vector<std::vector<int>> family_cache_;
    std::vector<std::vector<Sol>> sol_stack_; // per object type, a stack of SOLs

    // Rule 6: keep families and their members consistent after a pick.
    void propagate_family_pick(int family, const std::vector<int>& kept);
    void propagate_member_pick(int member, const std::vector<int>& kept);

    void rebuild_family_cache(int family) const;
};

}  // namespace mdz
