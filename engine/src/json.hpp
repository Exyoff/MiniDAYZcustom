// Minimal arena-backed JSON reader.
//
// data.js is ~8.3 MB of densely nested arrays. A node-per-object tree with
// std::string/std::vector members costs ~100 bytes a node and blows past a
// gigabyte; this keeps nodes at 40 bytes in one flat arena and stores decoded
// strings in a single side buffer. Parsing is read-only and one-shot: we walk
// the arena once to build the domain model, then drop the whole document.
#pragma once

#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace mdz {

class JsonDoc {
public:
    enum class Type : uint8_t { Null, Bool, Number, String, Array, Object };

    struct Node {
        Type type = Type::Null;
        bool boolean = false;
        double number = 0.0;
        uint32_t str_off = 0, str_len = 0;   // String payload
        uint32_t key_off = 0, key_len = 0;   // key, when this node is an object member
        uint32_t child = kNone;              // first child (Array/Object)
        uint32_t next = kNone;               // next sibling
    };

    static constexpr uint32_t kNone = 0xFFFFFFFFu;

    // Parses `text`. Throws std::runtime_error on malformed input.
    void parse(std::string text);
    static JsonDoc from_file(const std::string& path);

    const Node& root() const { return nodes_[root_]; }
    const Node& at(uint32_t index) const { return nodes_[index]; }
    uint32_t root_index() const { return root_; }

    std::string_view str(const Node& n) const {
        return std::string_view(strings_.data() + n.str_off, n.str_len);
    }
    std::string_view key(const Node& n) const {
        return std::string_view(strings_.data() + n.key_off, n.key_len);
    }

    // Array/object helpers. Index lookups are O(n) walks -- fine, because the
    // loader touches each node a constant number of times.
    uint32_t child_at(uint32_t node, uint32_t index) const;
    uint32_t member(uint32_t node, std::string_view name) const;
    uint32_t size(uint32_t node) const;

    bool is_null(uint32_t i) const { return nodes_[i].type == Type::Null; }
    double num(uint32_t i) const { return nodes_[i].number; }
    int    as_int(uint32_t i) const { return static_cast<int>(nodes_[i].number); }
    bool   flag(uint32_t i) const { return nodes_[i].boolean; }
    std::string_view text_of(uint32_t i) const { return str(nodes_[i]); }

    size_t node_count() const { return nodes_.size(); }
    size_t string_bytes() const { return strings_.size(); }

private:
    std::vector<Node> nodes_;
    std::string strings_;
    std::string src_;
    size_t pos_ = 0;
    uint32_t root_ = kNone;

    void skip_ws();
    uint32_t parse_value();
    uint32_t parse_array();
    uint32_t parse_object();
    uint32_t parse_string_node();
    uint32_t parse_number();
    uint32_t parse_literal();
    void read_string_into(uint32_t* off, uint32_t* len);
    [[noreturn]] void fail(const char* what) const;
};

}  // namespace mdz
