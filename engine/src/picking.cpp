#include "picking.hpp"

#include <algorithm>

namespace mdz {

PickingEngine::PickingEngine(const Project& project) : project_(project) {
    const size_t n = project_.object_types.size();
    by_type_.resize(n);
    family_cache_.resize(n);
    sol_stack_.assign(n, std::vector<Sol>{Sol{}});
}

// ---------------------------------------------------------------------------
// World
// ---------------------------------------------------------------------------

int PickingEngine::add_instance(const Instance& inst) {
    int idx = static_cast<int>(instances.size());
    instances.push_back(inst);
    if (inst.object_type >= 0 && inst.object_type < static_cast<int>(by_type_.size()))
        by_type_[static_cast<size_t>(inst.object_type)].push_back(idx);
    // A family's instance list is derived, so any cache it holds is now stale.
    for (int fam : project_.type(inst.object_type).member_of)
        family_cache_[static_cast<size_t>(fam)].clear();
    return idx;
}

void PickingEngine::load_layout(const Layout& layout) {
    for (const Instance& inst : layout.instances) add_instance(inst);
}

void PickingEngine::rebuild_family_cache(int family) const {
    std::vector<int>& out = family_cache_[static_cast<size_t>(family)];
    out.clear();
    for (int member : project_.type(family).family_members) {
        const std::vector<int>& src = by_type_[static_cast<size_t>(member)];
        out.insert(out.end(), src.begin(), src.end());
    }
    std::sort(out.begin(), out.end());
}

const std::vector<int>& PickingEngine::instances_of(int object_type) const {
    const ObjectType& t = project_.type(object_type);
    if (!t.is_family) return by_type_[static_cast<size_t>(object_type)];
    std::vector<int>& cache = family_cache_[static_cast<size_t>(object_type)];
    if (cache.empty() && !t.family_members.empty()) rebuild_family_cache(object_type);
    return cache;
}

// ---------------------------------------------------------------------------
// SOL state
// ---------------------------------------------------------------------------

void PickingEngine::reset_all() {
    for (std::vector<Sol>& stack : sol_stack_) {
        stack.clear();
        stack.push_back(Sol{});
    }
}

void PickingEngine::push_scope() {
    // Rule 4: a sub-event works on a copy, so its filtering cannot leak out.
    for (std::vector<Sol>& stack : sol_stack_) stack.push_back(stack.back());
}

void PickingEngine::pop_scope() {
    for (std::vector<Sol>& stack : sol_stack_)
        if (stack.size() > 1) stack.pop_back();
}

Sol& PickingEngine::sol(int object_type) {
    return sol_stack_[static_cast<size_t>(object_type)].back();
}
const Sol& PickingEngine::sol(int object_type) const {
    return sol_stack_[static_cast<size_t>(object_type)].back();
}

std::vector<int> PickingEngine::picked(int object_type) const {
    if (object_type < 0) return {};
    const Sol& s = sol(object_type);
    if (s.select_all) return instances_of(object_type);
    return s.instances;
}

void PickingEngine::pick_single(int object_type, int instance_index) {
    pick_set(object_type, std::vector<int>{instance_index});
}

void PickingEngine::pick_set(int object_type, const std::vector<int>& set) {
    Sol& s = sol(object_type);
    s.select_all = false;
    s.instances = set;
    s.else_instances.clear();
    if (project_.type(object_type).is_family) propagate_family_pick(object_type, set);
    else propagate_member_pick(object_type, set);
}

// ---------------------------------------------------------------------------
// Rule 6: families and members share picking
// ---------------------------------------------------------------------------

void PickingEngine::propagate_family_pick(int family, const std::vector<int>& kept) {
    for (int member : project_.type(family).family_members) {
        std::vector<int> mine;
        for (int i : kept)
            if (instances[static_cast<size_t>(i)].object_type == member) mine.push_back(i);
        Sol& ms = sol(member);
        ms.select_all = false;
        ms.instances = std::move(mine);
    }
}

void PickingEngine::propagate_member_pick(int member, const std::vector<int>& kept) {
    for (int fam : project_.type(member).member_of) {
        Sol& fs = sol(fam);
        if (fs.select_all) {
            // The family was unfiltered: it now holds this pick plus every
            // instance of its other members, which stay unconstrained.
            std::vector<int> next;
            for (int other : project_.type(fam).family_members) {
                if (other == member) continue;
                const std::vector<int>& src = by_type_[static_cast<size_t>(other)];
                next.insert(next.end(), src.begin(), src.end());
            }
            next.insert(next.end(), kept.begin(), kept.end());
            std::sort(next.begin(), next.end());
            fs.select_all = false;
            fs.instances = std::move(next);
        } else {
            // Already filtered: drop the instances of this member that the
            // new pick excluded, and leave the other members alone.
            std::vector<int> next;
            for (int i : fs.instances) {
                if (instances[static_cast<size_t>(i)].object_type != member) {
                    next.push_back(i);
                } else if (std::find(kept.begin(), kept.end(), i) != kept.end()) {
                    next.push_back(i);
                }
            }
            fs.instances = std::move(next);
        }
    }
}

// ---------------------------------------------------------------------------
// Rules 2 and 3: filtering and gating
// ---------------------------------------------------------------------------

bool PickingEngine::apply_condition(const Condition& c, const InstancePredicate& pred) {
    const std::vector<int> candidates = picked(c.object_type);

    std::vector<int> matched, rejected;
    matched.reserve(candidates.size());
    for (int i : candidates) {
        if (pred(c, instances[static_cast<size_t>(i)])) matched.push_back(i);
        else rejected.push_back(i);
    }

    if (c.inverted) {
        // Rule 3: a gate. It passes only when nothing matched, and it must not
        // narrow the SOL -- "Zombie is not dead" leaves every zombie picked.
        return matched.empty();
    }

    Sol& s = sol(c.object_type);
    s.select_all = false;
    s.instances = matched;
    s.else_instances = std::move(rejected);

    if (project_.type(c.object_type).is_family) propagate_family_pick(c.object_type, s.instances);
    else propagate_member_pick(c.object_type, s.instances);

    return !s.instances.empty();
}

// ---------------------------------------------------------------------------
// Rule 5: OR blocks union rather than intersect
// ---------------------------------------------------------------------------

bool PickingEngine::apply_or_block(const std::vector<Condition>& conditions,
                                   const InstancePredicate& pred,
                                   const SystemPredicate& sys_pred) {
    // Snapshot the SOLs each branch should start from.
    std::vector<Sol> entry;
    entry.reserve(sol_stack_.size());
    for (const std::vector<Sol>& stack : sol_stack_) entry.push_back(stack.back());

    bool any_true = false;
    // object type -> union of everything its true branches picked
    std::vector<std::vector<int>> unioned(sol_stack_.size());
    std::vector<bool> touched(sol_stack_.size(), false);

    for (const Condition& c : conditions) {
        // Every branch of an OR sees the state the block started in.
        for (size_t t = 0; t < sol_stack_.size(); ++t) sol_stack_[t].back() = entry[t];

        bool ok;
        if (c.object_type < 0) {
            ok = sys_pred(c);
            if (c.inverted) ok = !ok;
        } else {
            ok = apply_condition(c, pred);
        }
        if (!ok) continue;
        any_true = true;

        if (c.object_type >= 0 && !c.inverted) {
            size_t t = static_cast<size_t>(c.object_type);
            touched[t] = true;
            const std::vector<int>& got = sol_stack_[t].back().instances;
            unioned[t].insert(unioned[t].end(), got.begin(), got.end());
        }
    }

    // Restore, then install the union for every type a branch picked on.
    for (size_t t = 0; t < sol_stack_.size(); ++t) sol_stack_[t].back() = entry[t];
    if (!any_true) return false;

    for (size_t t = 0; t < sol_stack_.size(); ++t) {
        if (!touched[t]) continue;
        std::vector<int>& u = unioned[t];
        std::sort(u.begin(), u.end());
        u.erase(std::unique(u.begin(), u.end()), u.end());
        pick_set(static_cast<int>(t), u);
    }
    return true;
}

}  // namespace mdz
