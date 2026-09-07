// Runs a layout's event sheet against live state and reports what happened.
//
//   mdz_run <data.js> [--layout NAME] [--ticks N] [--names FILE]
//
// The point is an honest measure of how much of the game actually executes:
// which variables the events changed, how many instances were created or
// destroyed, and precisely which ACEs are still unimplemented, ranked by how
// often they were reached.

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "decompile.hpp"
#include "project.hpp"
#include "runtime.hpp"

using namespace mdz;

namespace {
bool has_flag(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc; ++i) if (std::strcmp(argv[i], flag) == 0) return true;
    return false;
}
const char* arg_value(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc - 1; ++i)
        if (std::strcmp(argv[i], flag) == 0) return argv[i + 1];
    return nullptr;
}
}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: mdz_run <data.js> [--layout NAME] [--ticks N] [--names FILE]\n");
        return 2;
    }
    const std::string data_js = argv[1];
    const char* want = arg_value(argc, argv, "--layout");
    const int ticks = arg_value(argc, argv, "--ticks")
                          ? std::atoi(arg_value(argc, argv, "--ticks")) : 60;
    const std::string names_path =
        arg_value(argc, argv, "--names") ? arg_value(argc, argv, "--names") : "data/ace_names.txt";

    try {
        Project project = Project::load(data_js);
        AceNames names;
        const size_t loaded = names.load(names_path);
        const size_t exprs = names.load_expressions("data/expr_names.txt");
        std::printf("ace names: %zu, expression names: %zu\n", loaded, exprs);

        const Layout* layout = nullptr;
        for (const Layout& l : project.layouts) {
            if (want) { if (l.name == want) { layout = &l; break; } }
            else if (!layout || l.instances.size() > layout->instances.size()) layout = &l;
        }
        if (!layout) { std::fprintf(stderr, "layout not found\n"); return 1; }

        Runtime rt(project, names);
        rt.strict_unimplemented = has_flag(argc, argv, "--strict");
        rt.load_layout(*layout);
        std::printf("layout %s, sheet %s, %zu instances, %zu variables\n\n",
                    layout->name.c_str(), layout->event_sheet.c_str(),
                    rt.engine().instances.size(), rt.variables().size());

        const std::map<std::string, std::string> before = [&] {
            std::map<std::string, std::string> m;
            for (const auto& kv : rt.variables()) m[kv.first] = kv.second.as_text();
            return m;
        }();
        size_t alive_before = 0;
        for (const Instance& i : rt.engine().instances) if (!i.destroyed) ++alive_before;

        // Optional synthetic click, in world coordinates, delivered halfway
        // through the run so the layout has settled first.
        double cx = 0, cy = 0;
        const bool do_click = arg_value(argc, argv, "--click") &&
            std::sscanf(arg_value(argc, argv, "--click"), "%lf,%lf", &cx, &cy) == 2;
        for (int t = 0; t < ticks; ++t) {
            if (do_click && t == ticks / 2) {
                rt.input.x = cx; rt.input.y = cy;
                rt.input.down = true; rt.input.pressed = true;
            } else if (do_click && t == ticks / 2 + 1) {
                rt.input.down = false; rt.input.released = true;
            }
            rt.tick(1.0 / 60.0);
        }

        size_t alive_after = 0;
        for (const Instance& i : rt.engine().instances) if (!i.destroyed) ++alive_after;

        std::vector<std::string> changed;
        for (const auto& kv : rt.variables()) {
            auto it = before.find(kv.first);
            if (it == before.end() || it->second != kv.second.as_text())
                changed.push_back(kv.first + ": " + (it == before.end() ? "<new>" : it->second) +
                                  " -> " + kv.second.as_text());
        }
        std::sort(changed.begin(), changed.end());

        const Runtime::Stats& s = rt.stats();
        std::printf("=== after %d ticks ===\n", ticks);
        std::printf("  conditions evaluated  %zu\n", s.conditions_run);
        std::printf("  actions executed      %zu\n", s.actions_run);
        std::printf("  instances             %zu -> %zu\n", alive_before, alive_after);
        std::printf("  variables changed     %zu\n", changed.size());
        for (size_t i = 0; i < changed.size() && i < 12; ++i)
            std::printf("      %s\n", changed[i].c_str());
        if (changed.size() > 12) std::printf("      ... and %zu more\n", changed.size() - 12);

        const double cond_pct = s.conditions_run
            ? 100.0 * (1.0 - static_cast<double>(s.unimplemented_conditions) / s.conditions_run) : 0.0;
        const double act_pct = s.actions_run
            ? 100.0 * (1.0 - static_cast<double>(s.unimplemented_actions) / s.actions_run) : 0.0;
        std::printf("\n=== coverage of what actually ran ===\n");
        std::printf("  conditions implemented  %.1f%%  (%zu unimplemented)\n",
                    cond_pct, s.unimplemented_conditions);
        std::printf("  actions implemented     %.1f%%  (%zu unimplemented)\n",
                    act_pct, s.unimplemented_actions);
        std::printf("  unresolved expressions  %zu  (named but not implemented)\n",
                    s.unknown_expressions);

        auto top = [](const std::unordered_map<std::string, size_t>& m, const char* title) {
            std::vector<std::pair<std::string, size_t>> v(m.begin(), m.end());
            std::sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second > b.second; });
            std::printf("\n  most-reached unimplemented %s:\n", title);
            for (size_t i = 0; i < v.size() && i < 10; ++i)
                std::printf("      %-28s %zu\n", v[i].first.c_str(), v[i].second);
            if (v.empty()) std::printf("      (none)\n");
        };
        top(s.missing_conditions, "conditions");
        top(s.missing_actions, "actions");
        top(s.missing_expressions, "expressions");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
