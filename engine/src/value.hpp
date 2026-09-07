// Construct 2's runtime value: a number or a string, coerced on demand.
//
// The event system is loosely typed -- the same variable is compared against a
// number in one event and concatenated with a string in the next -- so values
// carry their type and convert the way the original does rather than being
// pinned to one representation.
#pragma once

#include <cmath>
#include <cstdlib>
#include <string>

namespace mdz {

class Value {
public:
    Value() = default;
    explicit Value(double n) : number_(n), is_text_(false) {}
    explicit Value(const std::string& s) : text_(s), is_text_(true) {}

    bool is_text() const { return is_text_; }

    double as_number() const {
        if (!is_text_) return number_;
        // Construct 2 parses a leading number and yields 0 when there is none.
        try { return std::stod(text_); } catch (...) { return 0.0; }
    }

    std::string as_text() const {
        if (is_text_) return text_;
        // Whole numbers print without a decimal point, as the original does.
        if (number_ == std::floor(number_) && std::abs(number_) < 1e15)
            return std::to_string(static_cast<long long>(number_));
        return std::to_string(number_);
    }

    bool truthy() const { return is_text_ ? !text_.empty() : number_ != 0.0; }

    // Comparison follows the original: two numbers compare numerically, and
    // anything involving text compares as text.
    int compare(const Value& other) const {
        if (is_text_ || other.is_text_) {
            const std::string a = as_text(), b = other.as_text();
            return a < b ? -1 : (a > b ? 1 : 0);
        }
        const double a = number_, b = other.number_;
        return a < b ? -1 : (a > b ? 1 : 0);
    }

private:
    double number_ = 0.0;
    std::string text_;
    bool is_text_ = false;
};

}  // namespace mdz
