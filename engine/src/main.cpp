// Loads a real Construct 2 export and drives every event through the picking
// engine. This is the prototype's evidence: the semantics hold up not just on
// hand-built cases but on the shipped project, at full size and nesting depth.

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <map>
#include <string>

#include "picking.hpp"
#include "project.hpp"
#include "runner.hpp"

using namespace mdz;

namespace {

void report_project(const Project& p) {
    size_t families = 0, named = 0, sprites = 0;
    for (const ObjectType& t : p.object_types) {
        if (t.is_family) ++families;
        if (!t.derived_name.empty()) ++named;
        if (t.plugin == 18) ++sprites;
    }

    std::printf("=== project ===\n");
    std::printf("  object types      %zu  (%zu families, %zu sprites)\n",
                p.object_types.size(), families, sprites);
    std::printf("  names recovered   %zu / %zu  (%.1f%%)\n", named,
                p.object_types.size(),
                100.0 * static_cast<double>(named) / static_cast<double>(p.object_types.size()));
    std::printf("  layouts           %zu\n", p.layouts.size());
    std::printf("  event sheets      %zu\n", p.sheets.size());
    std::printf("  events            %zu\n", p.total_events());
    std::printf("  conditions        %zu\n", p.total_conditions());
    std::printf("  actions           %zu\n", p.total_actions());
    std::printf("  max nesting       %d\n\n", p.max_nesting());

    std::printf("=== layouts ===\n");
    for (const Layout& l : p.layouts)
        std::printf("  %-16s %5d x %-5d  sheet=%-16s instances=%zu\n", l.name.c_str(),
                    l.width, l.height, l.event_sheet.c_str(), l.instances.size());

    std::printf("\n=== event sheets ===\n");
    for (const EventSheet& s : p.sheets) {
        size_t ev = 0;
        for (const EventBlock& b : s.blocks) {
            (void)b;
            ++ev;
        }
        std::printf("  %-18s vars=%-4zu includes=%-2zu top-level blocks=%zu\n",
                    s.name.c_str(), s.variables.size(), s.includes.size(), ev);
    }

    std::printf("\n=== recovered names (sample) ===\n");
    int shown = 0;
    for (const ObjectType& t : p.object_types) {
        if (t.derived_name.empty() || t.is_family) continue;
        std::printf("  %-8s -> %s\n", t.name.c_str(), t.derived_name.c_str());
        if (++shown >= 12) break;
    }
    std::printf("  ... and %zu more\n\n", named - static_cast<size_t>(shown));
}

// Exercises the picking machinery over every event. Conditions answer in a
// fixed pseudo-random pattern so that picks genuinely vary and the engine has
// to do real work; nothing here claims to reproduce game behaviour, it proves
// the picking rules survive the real data's shape.
RunStats stress(const Project& p, PickingEngine& engine) {
    uint32_t seed = 12345;
    auto next = [&seed]() {
        seed = seed * 1664525u + 1013904223u;
        return seed;
    };

    RunnerHooks hooks;
    hooks.instance_condition = [&](const Condition&, const Instance&) {
        return (next() & 3u) != 0u;         // ~75% of instances match
    };
    hooks.system_condition = [&](const Condition&) {
        return (next() & 7u) != 0u;         // ~87% of system conditions pass
    };
    hooks.action = [](const Action&, const std::vector<int>&) {};
    hooks.loop_target = [](const Condition&) { return -1; };

    EventRunner runner(engine, hooks);
    for (const EventSheet& s : p.sheets) runner.run_sheet(s);
    return runner.stats();
}

// Builds a synthetic world holding one instance of every concrete object type.
// This is a coverage instrument, not a game state: with no instances of a type
// present, any condition on it correctly fails and prunes its whole subtree,
// which would leave most of the event graph unvisited.
void populate_one_of_each(const Project& p, PickingEngine& engine) {
    int uid = 1;
    for (const ObjectType& t : p.object_types) {
        if (t.is_family) continue;   // families hold no instances of their own
        Instance inst;
        inst.object_type = t.index;
        inst.uid = uid++;
        inst.vars.assign(static_cast<size_t>(t.instance_var_count), 0.0);
        engine.add_instance(inst);
    }
}

// Forces every block to be entered: all conditions pass and trigger blocks run
// too. This is the coverage pass -- it walks the entire event graph to full
// depth, which the randomised run cannot do because failed conditions prune
// whole subtrees.
RunStats traverse_all(const Project& p, PickingEngine& engine) {
    // Answer each condition in whichever direction lets the block pass. An
    // inverted condition passes only when *nothing* matches, so it needs the
    // opposite answer from a plain one.
    RunnerHooks hooks;
    hooks.instance_condition = [](const Condition& c, const Instance&) { return !c.inverted; };
    hooks.system_condition = [](const Condition& c) { return !c.inverted; };
    hooks.action = [](const Action&, const std::vector<int>&) {};
    hooks.loop_target = [](const Condition&) { return -1; };

    EventRunner runner(engine, hooks);
    runner.run_triggers = true;
    for (const EventSheet& s : p.sheets) runner.run_sheet(s);
    return runner.stats();
}

void print_stats(const char* title, const RunStats& st, long long ms) {
    std::printf("=== %s ===\n", title);
    std::printf("  blocks entered    %zu\n", st.blocks_entered);
    std::printf("  blocks passed     %zu\n", st.blocks_passed);
    std::printf("  conditions        %zu\n", st.conditions_evaluated);
    std::printf("  actions applied   %zu\n", st.actions_run);
    std::printf("  OR blocks         %zu\n", st.or_blocks);
    std::printf("  for-each loops    %zu\n", st.foreach_loops);
    std::printf("  trigger blocks    %zu skipped\n", st.triggers_skipped);
    std::printf("  deepest scope     %d\n", st.deepest_scope);
    std::printf("  elapsed           %lld ms\n\n", ms);
}

}  // namespace

int main(int argc, char** argv) {
    const std::string path =
        argc > 1 ? argv[1] : "../MiniDayZ+1.2/data.js";

    try {
        auto t0 = std::chrono::steady_clock::now();
        Project p = Project::load(path);
        auto t1 = std::chrono::steady_clock::now();
        std::printf("loaded %s in %lld ms\n\n", path.c_str(),
                    static_cast<long long>(
                        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()));

        report_project(p);

        // Use the largest layout so the engine has a realistic instance count.
        const Layout* biggest = nullptr;
        for (const Layout& l : p.layouts)
            if (!biggest || l.instances.size() > biggest->instances.size()) biggest = &l;

        PickingEngine engine(p);
        if (biggest) {
            engine.load_layout(*biggest);
            std::printf("=== picking stress run ===\n");
            std::printf("  layout            %s (%zu instances)\n", biggest->name.c_str(),
                        engine.instances.size());
        }

        using clock = std::chrono::steady_clock;
        auto ms = [](clock::time_point a, clock::time_point b) {
            return static_cast<long long>(
                std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count());
        };

        auto t2 = clock::now();
        RunStats randomised = stress(p, engine);
        auto t3 = clock::now();
        print_stats("picking run: randomised conditions", randomised, ms(t2, t3));

        PickingEngine coverage_engine(p);
        populate_one_of_each(p, coverage_engine);
        auto t4 = clock::now();
        RunStats full = traverse_all(p, coverage_engine);
        auto t5 = clock::now();
        std::printf("  world             one instance of each type (%zu)\n",
              coverage_engine.instances.size());
        print_stats("picking run: full traversal (every block entered)", full, ms(t4, t5));

        const size_t expected = p.total_events();
        std::printf("coverage: %zu / %zu blocks entered (%.1f%%), depth %d / %d\n",
                    full.blocks_entered, expected,
                    100.0 * static_cast<double>(full.blocks_entered) /
                        static_cast<double>(expected),
                    full.deepest_scope, p.max_nesting());
        if (full.blocks_entered != expected)
            std::printf("WARNING: %zu blocks were never entered\n",
                        expected - full.blocks_entered);
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
