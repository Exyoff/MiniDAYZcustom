#include "decompile.hpp"

#include <fstream>
#include <sstream>

namespace mdz {
namespace {

// Operator precedence, loosest to tightest. Used only to decide parentheses.
int precedence(ExpOp op) {
    switch (op) {
        case ExpOp::Conditional:  return 1;
        case ExpOp::Or:           return 2;
        case ExpOp::AndConcat:    return 3;
        case ExpOp::Equal:
        case ExpOp::NotEqual:
        case ExpOp::Less:
        case ExpOp::LessEqual:
        case ExpOp::Greater:
        case ExpOp::GreaterEqual: return 4;
        case ExpOp::Add:
        case ExpOp::Subtract:     return 5;
        case ExpOp::Multiply:
        case ExpOp::Divide:
        case ExpOp::Modulo:       return 6;
        case ExpOp::Power:        return 7;
        case ExpOp::Negate:       return 8;
        default:                  return 9;   // literals and calls
    }
}

const char* op_symbol(ExpOp op) {
    switch (op) {
        case ExpOp::Add:          return " + ";
        case ExpOp::Subtract:     return " - ";
        case ExpOp::Multiply:     return " * ";
        case ExpOp::Divide:       return " / ";
        case ExpOp::Modulo:       return " % ";
        case ExpOp::Power:        return " ^ ";
        case ExpOp::AndConcat:    return " & ";
        case ExpOp::Equal:        return " = ";
        case ExpOp::NotEqual:     return " <> ";
        case ExpOp::Less:         return " < ";
        case ExpOp::LessEqual:    return " <= ";
        case ExpOp::Greater:      return " > ";
        case ExpOp::GreaterEqual: return " >= ";
        case ExpOp::Or:           return " | ";
        default:                  return " ? ";
    }
}

// Comparison selector values, as they appear in a tag-8 parameter slot.
const char* comparison_symbol(int value) {
    switch (value) {
        case 0: return "=";
        case 1: return "<>";
        case 2: return "<";
        case 3: return "<=";
        case 4: return ">";
        case 5: return ">=";
        default: return "?cmp";
    }
}

std::string number_text(double v) {
    if (v == static_cast<long long>(v)) return std::to_string(static_cast<long long>(v));
    std::ostringstream os;
    os << v;
    return os.str();
}

std::string quote(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    return out + "\"";
}

// Structural equality, used only by the peephole below.
bool same_expr(const Expr& a, const Expr& b) {
    if (a.op != b.op || a.number != b.number || a.text != b.text ||
        a.object_type != b.object_type || a.index != b.index ||
        a.args.size() != b.args.size())
        return false;
    for (size_t i = 0; i < a.args.size(); ++i)
        if (!same_expr(a.args[i], b.args[i])) return false;
    return true;
}

// Construct 2 has no abs() node; the editor emits `x <= 0 ? -x : x` instead,
// and it appears throughout this project. Printing it as abs(x) removes a lot
// of noise -- and recognising this idiom is what originally pinned down the
// conditional, <= and unary-minus opcodes.
bool match_abs(const Expr& e, const Expr** inner) {
    if (e.op != ExpOp::Conditional || e.args.size() != 3) return false;
    const Expr& cond = e.args[0];
    if (cond.op != ExpOp::LessEqual || cond.args.size() != 2) return false;
    if (cond.args[1].op != ExpOp::Int || cond.args[1].number != 0.0) return false;
    if (e.args[1].op != ExpOp::Negate || e.args[1].args.size() != 1) return false;
    if (!same_expr(cond.args[0], e.args[2])) return false;
    if (!same_expr(cond.args[0], e.args[1].args[0])) return false;
    *inner = &e.args[2];
    return true;
}

std::string join_args(const Project& p, const std::vector<Expr>& args, size_t from) {
    std::string out;
    for (size_t i = from; i < args.size(); ++i) {
        if (i > from) out += ", ";
        out += format_expression(p, args[i]);
    }
    return out;
}

}  // namespace

// ---------------------------------------------------------------------------
// ACE name table
// ---------------------------------------------------------------------------

size_t AceNames::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) return 0;

    size_t count = 0;
    std::string line;
    while (std::getline(in, line)) {
        size_t hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        std::istringstream ls(line);
        std::string kind;
        int plugin = 0, ace = 0;
        std::string name, behavior;
        if (!(ls >> kind >> plugin >> ace >> name)) continue;
        ls >> behavior;   // optional; absent for unscoped entries
        if (kind == "C") conditions_[Key{plugin, ace, behavior}] = name;
        else if (kind == "A") actions_[Key{plugin, ace, behavior}] = name;
        else continue;
        ++count;
    }
    return count;
}

std::string AceNames::lookup(const std::map<Key, std::string>& table, int plugin, int ace,
                             const std::string& behavior) const {
    if (!behavior.empty()) {
        auto it = table.find(Key{plugin, ace, behavior});
        if (it != table.end()) return it->second;
    }
    auto it = table.find(Key{plugin, ace, std::string()});
    return it == table.end() ? std::string() : it->second;
}

std::string AceNames::condition(int plugin, int ace, const std::string& behavior) const {
    return lookup(conditions_, plugin, ace, behavior);
}

std::string AceNames::action(int plugin, int ace, const std::string& behavior) const {
    return lookup(actions_, plugin, ace, behavior);
}

// ---------------------------------------------------------------------------
// Expressions
// ---------------------------------------------------------------------------

std::string format_expression(const Project& p, const Expr& e) {
    const Expr* abs_inner = nullptr;
    if (match_abs(e, &abs_inner)) return "abs(" + format_expression(p, *abs_inner) + ")";

    switch (e.op) {
        case ExpOp::Int:
        case ExpOp::Float:
            return number_text(e.number);

        case ExpOp::String:
            return quote(e.text);

        case ExpOp::EventVar:
            return e.text;

        case ExpOp::Negate:
            return "-" + (e.args.empty() ? std::string("?")
                                         : format_expression(p, e.args[0]));

        case ExpOp::Conditional:
            if (e.args.size() >= 3)
                return "(" + format_expression(p, e.args[0]) + " ? " +
                       format_expression(p, e.args[1]) + " : " +
                       format_expression(p, e.args[2]) + ")";
            return "?:";

        case ExpOp::SystemExp:
            return "System.exp#" + std::to_string(e.index) +
                   (e.args.empty() ? "" : "(" + join_args(p, e.args, 0) + ")");

        case ExpOp::ObjectExp:
            return p.label(e.object_type) + ".exp#" + std::to_string(e.index) +
                   (e.args.empty() ? "" : "(" + join_args(p, e.args, 0) + ")");

        case ExpOp::InstanceVar:
            return p.label(e.object_type) + ".var#" + std::to_string(e.index);

        case ExpOp::BehaviorExp:
            return p.label(e.object_type) + "." + e.text + ".exp#" +
                   std::to_string(e.index) +
                   (e.args.empty() ? "" : "(" + join_args(p, e.args, 0) + ")");

        default: {
            // Binary operator: parenthesise operands that bind more loosely.
            if (e.args.size() < 2) return "?op" + std::to_string(static_cast<int>(e.op));
            const int mine = precedence(e.op);
            std::string lhs = format_expression(p, e.args[0]);
            std::string rhs = format_expression(p, e.args[1]);
            if (precedence(e.args[0].op) < mine) lhs = "(" + lhs + ")";
            if (precedence(e.args[1].op) <= mine) rhs = "(" + rhs + ")";
            return lhs + op_symbol(e.op) + rhs;
        }
    }
}

// ---------------------------------------------------------------------------
// Parameters
// ---------------------------------------------------------------------------

std::string format_param(const Project& p, const Param& param) {
    switch (param.tag) {
        case 4:   // object reference -- the payload is an object type index
            return p.label(static_cast<int>(param.value.number));
        case 8:   // comparison operator selector
            return comparison_symbol(static_cast<int>(param.value.number));
        case 10:  // instance variable index
            return "var#" + number_text(param.value.number);
        case 11:  // event variable, by name
            return param.value.text;
        case 3:   // combo box selection
            return "opt#" + number_text(param.value.number);
        case 6:   // layout, by name
        case 12:  // file, by name
            return quote(param.value.text);
        default:
            return format_expression(p, param.value);
    }
}

// ---------------------------------------------------------------------------
// Event blocks
// ---------------------------------------------------------------------------

namespace {

std::string params_text(const Project& p, const std::vector<Param>& params) {
    std::string out;
    for (size_t i = 0; i < params.size(); ++i) {
        if (i) out += ", ";
        out += format_param(p, params[i]);
    }
    return out;
}

int plugin_of(const Project& p, int object_type) {
    return object_type < 0 ? -1 : p.type(object_type).plugin;
}

std::string condition_text(const Project& p, const Condition& c, const AceNames& names) {
    const std::string target = c.object_type < 0 ? "System" : p.label(c.object_type);
    std::string call = names.condition(plugin_of(p, c.object_type), c.ace, c.behavior);
    // Always keep the raw index: the name is an inference, the index is fact.
    const std::string raw = "#" + std::to_string(c.ace);
    if (call.empty()) call = raw;
    else call += raw;

    std::string out = target;
    if (!c.behavior.empty()) out += "." + c.behavior;
    out += "." + call + "(" + params_text(p, c.params) + ")";
    return out;
}

std::string action_text(const Project& p, const Action& a, const AceNames& names) {
    const std::string target = a.object_type < 0 ? "System" : p.label(a.object_type);
    std::string call = names.action(plugin_of(p, a.object_type), a.ace, a.behavior);
    const std::string raw = "#" + std::to_string(a.ace);
    if (call.empty()) call = raw;
    else call += raw;

    std::string out = target;
    if (!a.behavior.empty()) out += "." + a.behavior;
    out += "." + call + "(" + params_text(p, a.params) + ")";
    return out;
}

void dump_block(std::ostream& out, const Project& p, const EventBlock& b,
                const AceNames& names, int depth, int* counter) {
    const std::string pad(static_cast<size_t>(depth) * 4, ' ');
    const int id = ++*counter;

    out << pad << "[" << id << "]";
    if (b.is_group) out << "  group " << quote(b.group_name);
    if (b.is_or_block) out << "  (OR block)";
    out << "\n";

    for (size_t i = 0; i < b.conditions.size(); ++i) {
        const Condition& c = b.conditions[i];
        const char* keyword;
        if (c.looping)            keyword = "for each";
        else if (b.is_or_block && i > 0) keyword = "or      ";
        else if (c.inverted)      keyword = "if not  ";
        else                      keyword = "if      ";

        out << pad << "    " << keyword << " " << condition_text(p, c, names);
        if (c.trigger_mode != 0) out << "   [trigger]";
        out << "\n";
    }

    for (const Action& a : b.actions)
        out << pad << "    do       " << action_text(p, a, names) << "\n";

    if (b.conditions.empty() && b.actions.empty() && b.subevents.empty())
        out << pad << "    (empty)\n";

    for (const EventBlock& sub : b.subevents)
        dump_block(out, p, sub, names, depth + 1, counter);

    if (depth == 0) out << "\n";
}

}  // namespace

void dump_sheet(std::ostream& out, const Project& p, const EventSheet& sheet,
                const AceNames& names) {
    out << "================================================================\n";
    out << "  sheet: " << sheet.name << "\n";
    out << "  " << sheet.variables.size() << " variables, "
        << sheet.blocks.size() << " top-level blocks\n";
    if (!sheet.includes.empty()) {
        out << "  includes:";
        for (const std::string& inc : sheet.includes) out << " " << inc;
        out << "\n";
    }
    out << "================================================================\n\n";

    if (!sheet.variables.empty()) {
        out << "variables:\n";
        for (const EventVariable& v : sheet.variables) {
            out << "    " << (v.is_text ? "text   " : "number ") << v.name << " = ";
            if (v.is_text) out << quote(v.initial_text);
            else out << number_text(v.initial_number);
            out << "\n";
        }
        out << "\n";
    }

    out << "events:\n\n";
    int counter = 0;
    for (const EventBlock& b : sheet.blocks) dump_block(out, p, b, names, 0, &counter);
}

void dump_project(std::ostream& out, const Project& p, const AceNames& names) {
    for (const EventSheet& s : p.sheets) dump_sheet(out, p, s, names);
}

}  // namespace mdz
