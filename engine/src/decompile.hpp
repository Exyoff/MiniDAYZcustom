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
#include <tuple>
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

    // Expression names, from a separate table. Expressions are a third index
    // space, keyed by kind ('S' system, 'O' object, 'B' behavior) rather than
    // by the condition/action split.
    size_t load_expressions(const std::string& path);
    std::string expression(char kind, int plugin, int index,
                           const std::string& behavior = "") const;

    // Empty when the ACE has no known name. A behavior-scoped entry wins over
    // an unscoped one: the same (plugin, ace) pair means different things
    // across behaviors, so "SetSpeed" on Bullet is not "SetSpeed" on Car.
    std::string condition(int plugin, int ace, const std::string& behavior = "") const;
    std::string action(int plugin, int ace, const std::string& behavior = "") const;

private:
    // (plugin, ace, behavior) -> name; behavior is empty for unscoped entries.
    using Key = std::tuple<int, int, std::string>;
    std::map<Key, std::string> conditions_;
    std::map<Key, std::string> actions_;
    std::map<std::tuple<char, int, int, std::string>, std::string> expressions_;
    std::string lookup(const std::map<Key, std::string>& table, int plugin, int ace,
                       const std::string& behavior) const;
};

// Renders one expression as infix text, parenthesised by precedence.
std::string format_expression(const Project& p, const Expr& e,
                              const AceNames* names = nullptr);

// Renders one parameter, using its slot tag to decode object references,
// comparison operators, instance variable indices and variable names.
std::string format_param(const Project& p, const Param& param);
std::string format_param(const Project& p, const Param& param, const AceNames* names);

void dump_sheet(std::ostream& out, const Project& p, const EventSheet& sheet,
                const AceNames& names);

void dump_project(std::ostream& out, const Project& p, const AceNames& names);

}  // namespace mdz
