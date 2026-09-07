#include "json.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace mdz {

void JsonDoc::fail(const char* what) const {
    std::ostringstream os;
    os << "json: " << what << " at byte " << pos_;
    throw std::runtime_error(os.str());
}

JsonDoc JsonDoc::from_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot open " + path);
    std::ostringstream buf;
    buf << in.rdbuf();
    JsonDoc doc;
    doc.parse(buf.str());
    return doc;
}

void JsonDoc::parse(std::string text) {
    src_ = std::move(text);
    pos_ = 0;
    // Construct 2 writes a UTF-8 BOM on some exported files.
    if (src_.size() >= 3 && static_cast<unsigned char>(src_[0]) == 0xEF &&
        static_cast<unsigned char>(src_[1]) == 0xBB &&
        static_cast<unsigned char>(src_[2]) == 0xBF) {
        pos_ = 3;
    }
    nodes_.reserve(src_.size() / 6);   // empirically close for this data
    strings_.reserve(src_.size() / 8);
    root_ = parse_value();
    src_.clear();
    src_.shrink_to_fit();
}

void JsonDoc::skip_ws() {
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') ++pos_;
        else break;
    }
}

uint32_t JsonDoc::parse_value() {
    skip_ws();
    if (pos_ >= src_.size()) fail("unexpected end of input");
    switch (src_[pos_]) {
        case '[': return parse_array();
        case '{': return parse_object();
        case '"': return parse_string_node();
        case 't': case 'f': case 'n': return parse_literal();
        default:  return parse_number();
    }
}

uint32_t JsonDoc::parse_array() {
    uint32_t self = static_cast<uint32_t>(nodes_.size());
    nodes_.push_back(Node{});
    nodes_[self].type = Type::Array;

    ++pos_;  // consume '['
    skip_ws();
    if (pos_ < src_.size() && src_[pos_] == ']') { ++pos_; return self; }

    uint32_t prev = kNone;
    for (;;) {
        uint32_t item = parse_value();
        if (prev == kNone) nodes_[self].child = item;
        else nodes_[prev].next = item;
        prev = item;

        skip_ws();
        if (pos_ >= src_.size()) fail("unterminated array");
        if (src_[pos_] == ',') { ++pos_; continue; }
        if (src_[pos_] == ']') { ++pos_; return self; }
        fail("expected ',' or ']'");
    }
}

uint32_t JsonDoc::parse_object() {
    uint32_t self = static_cast<uint32_t>(nodes_.size());
    nodes_.push_back(Node{});
    nodes_[self].type = Type::Object;

    ++pos_;  // consume '{'
    skip_ws();
    if (pos_ < src_.size() && src_[pos_] == '}') { ++pos_; return self; }

    uint32_t prev = kNone;
    for (;;) {
        skip_ws();
        if (pos_ >= src_.size() || src_[pos_] != '"') fail("expected object key");
        uint32_t koff = 0, klen = 0;
        read_string_into(&koff, &klen);

        skip_ws();
        if (pos_ >= src_.size() || src_[pos_] != ':') fail("expected ':'");
        ++pos_;

        uint32_t val = parse_value();
        nodes_[val].key_off = koff;
        nodes_[val].key_len = klen;
        if (prev == kNone) nodes_[self].child = val;
        else nodes_[prev].next = val;
        prev = val;

        skip_ws();
        if (pos_ >= src_.size()) fail("unterminated object");
        if (src_[pos_] == ',') { ++pos_; continue; }
        if (src_[pos_] == '}') { ++pos_; return self; }
        fail("expected ',' or '}'");
    }
}

void JsonDoc::read_string_into(uint32_t* off, uint32_t* len) {
    ++pos_;  // consume opening quote
    uint32_t start = static_cast<uint32_t>(strings_.size());
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if (c == '"') {
            ++pos_;
            *off = start;
            *len = static_cast<uint32_t>(strings_.size() - start);
            return;
        }
        if (c != '\\') { strings_.push_back(c); ++pos_; continue; }

        if (pos_ + 1 >= src_.size()) fail("bad escape");
        char e = src_[pos_ + 1];
        pos_ += 2;
        switch (e) {
            case 'n': strings_.push_back('\n'); break;
            case 't': strings_.push_back('\t'); break;
            case 'r': strings_.push_back('\r'); break;
            case 'b': strings_.push_back('\b'); break;
            case 'f': strings_.push_back('\f'); break;
            case '/': strings_.push_back('/');  break;
            case '"': strings_.push_back('"');  break;
            case '\\': strings_.push_back('\\'); break;
            case 'u': {
                if (pos_ + 4 > src_.size()) fail("bad \\u escape");
                unsigned cp = static_cast<unsigned>(
                    std::strtoul(src_.substr(pos_, 4).c_str(), nullptr, 16));
                pos_ += 4;
                // Encode as UTF-8. Surrogate pairs are passed through as the
                // replacement character; the project data has none.
                if (cp < 0x80) {
                    strings_.push_back(static_cast<char>(cp));
                } else if (cp < 0x800) {
                    strings_.push_back(static_cast<char>(0xC0 | (cp >> 6)));
                    strings_.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
                } else if (cp >= 0xD800 && cp <= 0xDFFF) {
                    strings_ += "\xEF\xBF\xBD";
                } else {
                    strings_.push_back(static_cast<char>(0xE0 | (cp >> 12)));
                    strings_.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                    strings_.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
                }
                break;
            }
            default: fail("unknown escape");
        }
    }
    fail("unterminated string");
}

uint32_t JsonDoc::parse_string_node() {
    uint32_t self = static_cast<uint32_t>(nodes_.size());
    nodes_.push_back(Node{});
    nodes_[self].type = Type::String;
    uint32_t off = 0, len = 0;
    read_string_into(&off, &len);
    nodes_[self].str_off = off;
    nodes_[self].str_len = len;
    return self;
}

uint32_t JsonDoc::parse_number() {
    size_t start = pos_;
    if (pos_ < src_.size() && (src_[pos_] == '-' || src_[pos_] == '+')) ++pos_;
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E' ||
            c == '-' || c == '+') ++pos_;
        else break;
    }
    if (start == pos_) fail("expected number");
    uint32_t self = static_cast<uint32_t>(nodes_.size());
    nodes_.push_back(Node{});
    nodes_[self].type = Type::Number;
    nodes_[self].number = std::strtod(src_.c_str() + start, nullptr);
    return self;
}

uint32_t JsonDoc::parse_literal() {
    uint32_t self = static_cast<uint32_t>(nodes_.size());
    nodes_.push_back(Node{});
    if (src_.compare(pos_, 4, "true") == 0) {
        nodes_[self].type = Type::Bool; nodes_[self].boolean = true;  pos_ += 4;
    } else if (src_.compare(pos_, 5, "false") == 0) {
        nodes_[self].type = Type::Bool; nodes_[self].boolean = false; pos_ += 5;
    } else if (src_.compare(pos_, 4, "null") == 0) {
        nodes_[self].type = Type::Null; pos_ += 4;
    } else {
        fail("unknown literal");
    }
    return self;
}

uint32_t JsonDoc::child_at(uint32_t node, uint32_t index) const {
    uint32_t c = nodes_[node].child;
    while (c != kNone && index-- > 0) c = nodes_[c].next;
    return c;
}

uint32_t JsonDoc::member(uint32_t node, std::string_view name) const {
    for (uint32_t c = nodes_[node].child; c != kNone; c = nodes_[c].next) {
        if (key(nodes_[c]) == name) return c;
    }
    return kNone;
}

uint32_t JsonDoc::size(uint32_t node) const {
    uint32_t n = 0;
    for (uint32_t c = nodes_[node].child; c != kNone; c = nodes_[c].next) ++n;
    return n;
}

}  // namespace mdz
