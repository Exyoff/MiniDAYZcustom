// Renders a Construct 2 export's event sheets as readable pseudocode.
//
//   ./mdz_dump <data.js> [output_dir] [ace_names.txt]
//
// Writes one .txt per event sheet plus an object index, and prints a summary
// of how much of the dump came out named rather than numeric.

#include <cstdio>
#include <fstream>
#include <map>
#include <string>

#include "decompile.hpp"
#include "project.hpp"

using namespace mdz;

namespace {

struct Coverage {
    size_t conditions = 0, named_conditions = 0;
    size_t actions = 0, named_actions = 0;
};

void measure(const Project& p, const EventBlock& b, const AceNames& names, Coverage* cov) {
    for (const Condition& c : b.conditions) {
        ++cov->conditions;
        int plugin = c.object_type < 0 ? -1 : p.type(c.object_type).plugin;
        if (!names.condition(plugin, c.ace, c.behavior).empty()) ++cov->named_conditions;
    }
    for (const Action& a : b.actions) {
        ++cov->actions;
        int plugin = a.object_type < 0 ? -1 : p.type(a.object_type).plugin;
        if (!names.action(plugin, a.ace, a.behavior).empty()) ++cov->named_actions;
    }
    for (const EventBlock& sub : b.subevents) measure(p, sub, names, cov);
}

void write_object_index(const std::string& path, const Project& p) {
    std::ofstream out(path);
    out << "# Object type index\n#\n";
    out << "# Exported names are minified (t0..t" << p.object_types.size() - 1
        << "). The readable name is reconstructed from the first sprite image\n"
        << "# the type references, so it describes the artwork rather than the\n"
        << "# author's original identifier.\n\n";
    for (const ObjectType& t : p.object_types) {
        out << t.name << "\t" << (t.derived_name.empty() ? "-" : t.derived_name)
            << "\tplugin=" << t.plugin << "\tvars=" << t.instance_var_count;
        if (t.is_family) out << "\tFAMILY members=" << t.family_members.size();
        if (!t.member_of.empty()) {
            out << "\tin_families=";
            for (size_t i = 0; i < t.member_of.size(); ++i)
                out << (i ? "," : "") << p.type(t.member_of[i]).name;
        }
        out << "\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::string data_path = argc > 1 ? argv[1] : "../MiniDayZ+1.2/data.js";
    const std::string out_dir   = argc > 2 ? argv[2] : "dump";
    const std::string names_path = argc > 3 ? argv[3] : "data/ace_names.txt";

    try {
        Project p = Project::load(data_path);

        AceNames names;
        size_t loaded = names.load(names_path);
        const size_t exprs = names.load_expressions("data/expr_names.txt");
        std::printf("ace names: %zu, expression names: %zu\n", loaded, exprs);

        Coverage cov;
        for (const EventSheet& s : p.sheets)
            for (const EventBlock& b : s.blocks) measure(p, b, names, &cov);

        for (const EventSheet& s : p.sheets) {
            const std::string path = out_dir + "/" + s.name + ".txt";
            std::ofstream out(path);
            if (!out) {
                std::fprintf(stderr, "cannot write %s (does %s exist?)\n",
                             path.c_str(), out_dir.c_str());
                return 1;
            }
            dump_sheet(out, p, s, names);
            std::printf("  wrote %-34s %zu top-level blocks\n", path.c_str(),
                        s.blocks.size());
        }
        write_object_index(out_dir + "/object_index.txt", p);
        std::printf("  wrote %s/object_index.txt\n\n", out_dir.c_str());

        auto pct = [](size_t a, size_t b) {
            return b == 0 ? 0.0 : 100.0 * static_cast<double>(a) / static_cast<double>(b);
        };
        std::printf("named coverage (by call site, not by distinct ACE):\n");
        std::printf("  conditions  %zu / %zu  (%.1f%%)\n", cov.named_conditions,
                    cov.conditions, pct(cov.named_conditions, cov.conditions));
        std::printf("  actions     %zu / %zu  (%.1f%%)\n", cov.named_actions,
                    cov.actions, pct(cov.named_actions, cov.actions));
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
