// The Construct 2 project model, loaded from data.js.
//
// Field layouts below were recovered empirically from MiniDayZ+1.2's export
// (see docs/data_format.md). Names in the export are minified to t0..t1069;
// `ObjectType::derived_name` reconstructs something readable from the sprite
// filename each type points at, which is what makes the dumped logic legible.
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "json.hpp"

namespace mdz {

// ---------------------------------------------------------------------------
// Expressions
// ---------------------------------------------------------------------------

// Node opcodes as they appear in data.js. Verified against usage in the real
// export; the arithmetic block (Add..Concat) follows Construct 2's declaration
// order and was confirmed by inspecting operand shapes.
enum class ExpOp : int {
    Int          = 0,
    Float        = 1,
    String       = 2,
    Negate       = 3,
    Add          = 4,
    Subtract     = 5,
    Multiply     = 6,
    Divide       = 7,
    Modulo       = 8,
    Power        = 9,
    AndConcat    = 10,   // '&' -- logical and, and string concatenation
    Equal        = 11,
    NotEqual     = 12,
    Less         = 13,
    LessEqual    = 14,
    Greater      = 15,
    GreaterEqual = 16,
    Or           = 17,   // '|'
    Conditional  = 18,   // cond ? a : b
    SystemExp    = 19,   // [19, exp_index, params?]
    ObjectExp    = 20,   // [20, objtype, exp_index, bool, null, params?]
    InstanceVar  = 21,   // [21, objtype, bool, null, var_index]
    BehaviorExp  = 22,   // [22, objtype, "Behavior", exp_index, bool, null, params?]
    EventVar     = 23,   // [23, "name"]
};

struct Expr {
    ExpOp op = ExpOp::Int;
    double number = 0.0;
    std::string text;          // string literal, variable name, behavior name
    int object_type = -1;      // ObjectExp / InstanceVar / BehaviorExp
    int index = -1;            // system/object/behavior expression index
    std::vector<Expr> args;    // operands, then call parameters
};

// ---------------------------------------------------------------------------
// Conditions, actions, event blocks
// ---------------------------------------------------------------------------

// Construct 2 stores three distinct booleans per condition. Two are fixed
// properties of the condition *type* (looping, and a type flag), and only
// `inverted` varies per use -- that is the real negation flag.
// A parameter slot. `tag` is the slot type declared by the ACE; the payload is
// an expression tree, or a bare literal for name slots (variable names) and
// combo slots (comparison operator selectors).
struct Param {
    int tag = 0;
    Expr value;
};

struct Condition {
    int object_type = -1;      // -1 == the System object
    int ace = 0;               // index into the plugin's condition table
    std::string behavior;      // non-empty when the condition targets a behavior
    int trigger_mode = 0;      // 0 normal, 1 trigger, 2 fast trigger
    bool looping = false;      // fixed per condition type (For / Repeat / For Each)
    bool inverted = false;     // per-use negation
    bool type_flag = false;    // fixed per condition type
    std::vector<Param> params;
};

struct Action {
    int object_type = -1;
    int ace = 0;
    std::string behavior;
    std::vector<Param> params;
};

struct EventBlock {
    bool is_or_block = false;
    bool is_group = false;
    std::string group_name;
    std::vector<Condition> conditions;
    std::vector<Action> actions;
    std::vector<EventBlock> subevents;
};

struct EventVariable {
    std::string name;
    double initial_number = 0.0;
    std::string initial_text;
    bool is_text = false;
};

struct EventSheet {
    std::string name;
    std::vector<EventVariable> variables;
    std::vector<std::string> includes;
    std::vector<EventBlock> blocks;
};

// ---------------------------------------------------------------------------
// Object types, families, instances
// ---------------------------------------------------------------------------

// One frame of a sprite animation. Frames are packed into spritesheets, so a
// frame is a rect inside `image` rather than a whole file. Verified against the
// real PNGs: every frame rect fits inside its sheet's dimensions.
struct Frame {
    std::string image;              // path relative to the game directory
    int x = 0, y = 0, w = 0, h = 0; // rect within the spritesheet
    double hotspot_x = 0.5, hotspot_y = 0.5;  // origin, as a fraction of w/h
};

struct Animation {
    std::string name;
    std::vector<Frame> frames;
};

struct ObjectType {
    int index = -1;
    std::string name;           // as exported: "t0", "t1", ...
    std::string derived_name;   // reconstructed from the sprite filename
    int plugin = -1;
    bool is_family = false;
    int instance_var_count = 0;

    std::vector<int> family_members;   // set when is_family
    std::vector<int> member_of;        // families this type belongs to
    std::vector<Animation> animations; // sprites only

    // First frame of the first animation, or nullptr when the type has no art.
    const Frame* first_frame() const {
        if (animations.empty() || animations[0].frames.empty()) return nullptr;
        return &animations[0].frames[0];
    }

    // The frame an instance actually shows: named animation if it resolves,
    // otherwise the first, with the index clamped into range.
    const Frame* frame_for(const std::string& anim_name, int index) const {
        const Animation* anim = nullptr;
        if (!anim_name.empty())
            for (const Animation& a : animations)
                if (a.name == anim_name) { anim = &a; break; }
        if (!anim && !animations.empty()) anim = &animations[0];
        if (!anim || anim->frames.empty()) return nullptr;
        if (index < 0) index = 0;
        if (index >= static_cast<int>(anim->frames.size()))
            index = static_cast<int>(anim->frames.size()) - 1;
        return &anim->frames[static_cast<size_t>(index)];
    }
};

struct Instance {
    int uid = -1;
    int object_type = -1;
    int layer = 0;                  // draw order within the layout
    // Instances pick their own starting animation frame. Drawing frame 0 for
    // everything is wrong and very visible: the obstacle helper types keep a
    // flat colour on frame 0 and their real artwork on later frames.
    std::string animation;
    int frame = 0;
    bool destroyed = false;

    // Per-instance behavior state, keyed "Behavior.property". Behaviors keep
    // their own data per instance (a bullet's distance travelled, a timer's
    // elapsed time); this is the minimal store for it, keyed by the behavior
    // name from the export, which was never minified.
    std::unordered_map<std::string, double> behavior_state;
    double x = 0.0, y = 0.0;
    double width = 0.0, height = 0.0;
    double angle = 0.0;
    std::vector<double> vars;
};

// Layers carry their own visibility and opacity, and the project leans on both:
// spawn-marker layers ship with visible=false, and UI layers ship at opacity 0
// and are faded in by events. Ignoring these draws helper objects that a player
// never sees.
struct LayerInfo {
    std::string name;
    bool visible = true;
    double opacity = 1.0;
};

struct Layout {
    std::string name;
    int width = 0, height = 0;
    std::string event_sheet;
    std::vector<LayerInfo> layers;
    std::vector<Instance> instances;

    // True when the layer would actually put pixels on screen at layout load.
    bool layer_draws(int layer) const {
        if (layer < 0 || layer >= static_cast<int>(layers.size())) return true;
        return layers[static_cast<size_t>(layer)].visible &&
               layers[static_cast<size_t>(layer)].opacity > 0.0;
    }
};

// ---------------------------------------------------------------------------
// Project
// ---------------------------------------------------------------------------

class Project {
public:
    static Project load(const std::string& data_js_path);

    std::vector<ObjectType> object_types;
    std::vector<Layout> layouts;
    std::vector<EventSheet> sheets;

    const ObjectType& type(int i) const { return object_types.at(static_cast<size_t>(i)); }
    // Prefers the reconstructed name, falling back to the minified one.
    std::string label(int object_type) const;

    // Counts, for the loader report.
    size_t total_events() const;
    size_t total_conditions() const;
    size_t total_actions() const;
    int    max_nesting() const;

private:
    void load_object_types(const JsonDoc& doc, uint32_t node);
    void load_families(const JsonDoc& doc, uint32_t node);
    void load_layouts(const JsonDoc& doc, uint32_t node);
    void load_sheets(const JsonDoc& doc, uint32_t node);
};

// Exposed for tests.
Expr parse_expression(const JsonDoc& doc, uint32_t node);
Param parse_param(const JsonDoc& doc, uint32_t node);
EventBlock parse_event_block(const JsonDoc& doc, uint32_t node);

}  // namespace mdz
