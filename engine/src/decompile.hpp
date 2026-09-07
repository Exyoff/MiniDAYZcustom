// Renders event sheets as readable, indented pseudocode.
//
// The export stores actions and conditions as bare numeric indices into plugin
// tables that the minified runtime no longer names. Everything else, though,
// survives or can be reconstructed: object names (from sprite filenames),
// variable names, behavior names, group names, and full expression trees. So
// the dump is readable even where an ACE is still anonymous, and the raw index
// is always printed so nothing is lost.
//
// ACE names come from a plain-text table (see `data/ace_names.txt`) rather
// than being hardcoded, so identifications can be added as they are worked out
// without touching this code.
#pragma once

#include <map>
#include <ostream>
#include <string>

#include "project.hpp"

namespace mdz {

// Maps (kind, plugin, ace) to a human name. `kind` is 'C' or 'A'.
class AceNames {
public:
    // Reads a table file. Missing file is not an error -- the dump just falls
    // back to numeric indices. Returns the number of names loaded.
    size_t load(const std::string& path);

    // Empty when the ACE has no known name.
    std::string condition(int plugin, int ace) const;
    std::string action(int plugin, int ace) const;

private:
    std::map<std::pair<int, int>, std::string> conditions_;
    std::map<std::pair<int, int>, std::string> actions_;
};

// Renders one expression as infix text, parenthesised by precedence.
std::string format_expression(const Project& p, const Expr& e);

// Renders one parameter, using its slot tag to decode object references,
// comparison operators, instance variable indices and variable names.
std::string format_param(const Project& p, const Param& param);

void dump_sheet(std::ostream& out, const Project& p, const EventSheet& sheet,
                const AceNames& names);

void dump_project(std::ostream& out, const Project& p, const AceNames& names);

}  // namespace mdz
