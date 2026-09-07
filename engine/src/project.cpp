#include "project.hpp"

#include <algorithm>
#include <cctype>

namespace mdz {
namespace {

constexpr uint32_t kNone = JsonDoc::kNone;

// data.js top-level: {"project": [ ... 29 slots ... ]}
constexpr uint32_t kSlotObjectTypes = 3;
constexpr uint32_t kSlotFamilies    = 4;
constexpr uint32_t kSlotLayouts     = 5;
constexpr uint32_t kSlotSheets      = 6;

// Turns "images/gui_panel-sheet0.png" into "gui_panel".
std::string name_from_image_path(std::string_view path) {
    size_t slash = path.find_last_of('/');
    std::string_view file = (slash == std::string_view::npos) ? path : path.substr(slash + 1);
    size_t dot = file.find_last_of('.');
    if (dot != std::string_view::npos) file = file.substr(0, dot);
    // Strip the "-sheetN" spritesheet suffix the exporter appends.
    size_t dash = file.rfind("-sheet");
    if (dash != std::string_view::npos) file = file.substr(0, dash);
    return std::string(file);
}

bool is_num(const JsonDoc& d, uint32_t n) {
    return n != kNone && d.at(n).type == JsonDoc::Type::Number;
}
bool is_arr(const JsonDoc& d, uint32_t n) {
    return n != kNone && d.at(n).type == JsonDoc::Type::Array;
}
bool is_str(const JsonDoc& d, uint32_t n) {
    return n != kNone && d.at(n).type == JsonDoc::Type::String;
}
bool truthy(const JsonDoc& d, uint32_t n) {
    return n != kNone && d.at(n).type == JsonDoc::Type::Bool && d.at(n).boolean;
}

}  // namespace

// ---------------------------------------------------------------------------
// Expressions
// ---------------------------------------------------------------------------

Expr parse_expression(const JsonDoc& doc, uint32_t node) {
    Expr e;
    if (!is_arr(doc, node)) return e;

    uint32_t head = doc.child_at(node, 0);
    if (!is_num(doc, head)) return e;
    e.op = static_cast<ExpOp>(doc.as_int(head));

    switch (e.op) {
        case ExpOp::Int:
        case ExpOp::Float:
            e.number = doc.num(doc.child_at(node, 1));
            return e;

        case ExpOp::String:
        case ExpOp::EventVar:
            e.text = std::string(doc.text_of(doc.child_at(node, 1)));
            return e;

        // [19, exp_index, params?]
        case ExpOp::SystemExp: {
            e.index = doc.as_int(doc.child_at(node, 1));
            uint32_t params = doc.child_at(node, 2);
            if (is_arr(doc, params)) {
                for (uint32_t c = doc.at(params).child; c != kNone; c = doc.at(c).next)
                    e.args.push_back(parse_param(doc, c).value);
            }
            return e;
        }

        // [20, objtype, exp_index, bool, null, params?]
        case ExpOp::ObjectExp: {
            e.object_type = doc.as_int(doc.child_at(node, 1));
            e.index       = doc.as_int(doc.child_at(node, 2));
            uint32_t params = doc.child_at(node, 5);
            if (is_arr(doc, params)) {
                for (uint32_t c = doc.at(params).child; c != kNone; c = doc.at(c).next)
                    e.args.push_back(parse_param(doc, c).value);
            }
            return e;
        }

        // [21, objtype, bool, null, var_index]
        case ExpOp::InstanceVar:
            e.object_type = doc.as_int(doc.child_at(node, 1));
            e.index       = doc.as_int(doc.child_at(node, 4));
            return e;

        // [22, objtype, "Behavior", exp_index, bool, null, params?]
        case ExpOp::BehaviorExp: {
            e.object_type = doc.as_int(doc.child_at(node, 1));
            e.text        = std::string(doc.text_of(doc.child_at(node, 2)));
            e.index       = doc.as_int(doc.child_at(node, 3));
            uint32_t params = doc.child_at(node, 6);
            if (is_arr(doc, params)) {
                for (uint32_t c = doc.at(params).child; c != kNone; c = doc.at(c).next)
                    e.args.push_back(parse_param(doc, c).value);
            }
            return e;
        }

        // Everything else is a plain operator node: operands follow the opcode.
        default:
            for (uint32_t c = doc.child_at(node, 1); c != kNone; c = doc.at(c).next)
                e.args.push_back(parse_expression(doc, c));
            return e;
    }
}

Param parse_param(const JsonDoc& doc, uint32_t node) {
    Param p;
    if (!is_arr(doc, node)) return p;
    uint32_t tag = doc.child_at(node, 0);
    if (is_num(doc, tag)) p.tag = doc.as_int(tag);

    uint32_t payload = doc.child_at(node, 1);
    if (is_arr(doc, payload)) {
        p.value = parse_expression(doc, payload);
    } else if (is_str(doc, payload)) {
        // Name slots (variable names, behavior names) carry a bare string.
        p.value.op = ExpOp::String;
        p.value.text = std::string(doc.text_of(payload));
    } else if (is_num(doc, payload)) {
        // Combo slots (comparison selectors) carry a bare integer.
        p.value.op = ExpOp::Int;
        p.value.number = doc.num(payload);
    }
    return p;
}

// ---------------------------------------------------------------------------
// Event blocks
// ---------------------------------------------------------------------------

namespace {

Condition parse_condition(const JsonDoc& doc, uint32_t n) {
    Condition c;
    c.object_type = doc.as_int(doc.child_at(n, 0));
    c.ace         = doc.as_int(doc.child_at(n, 1));
    uint32_t beh  = doc.child_at(n, 2);
    if (is_str(doc, beh)) c.behavior = std::string(doc.text_of(beh));
    c.trigger_mode = doc.as_int(doc.child_at(n, 3));
    c.looping      = truthy(doc, doc.child_at(n, 4));
    c.inverted     = truthy(doc, doc.child_at(n, 5));
    c.type_flag    = truthy(doc, doc.child_at(n, 6));
    uint32_t params = doc.child_at(n, 9);   // absent on 9-element conditions
    if (is_arr(doc, params))
        for (uint32_t p = doc.at(params).child; p != kNone; p = doc.at(p).next)
            c.params.push_back(parse_param(doc, p));
    return c;
}

Action parse_action(const JsonDoc& doc, uint32_t n) {
    Action a;
    a.object_type = doc.as_int(doc.child_at(n, 0));
    a.ace         = doc.as_int(doc.child_at(n, 1));
    uint32_t beh  = doc.child_at(n, 2);
    if (is_str(doc, beh)) a.behavior = std::string(doc.text_of(beh));
    uint32_t params = doc.child_at(n, 5);   // absent on 5-element actions
    if (is_arr(doc, params))
        for (uint32_t p = doc.at(params).child; p != kNone; p = doc.at(p).next)
            a.params.push_back(parse_param(doc, p));
    return a;
}

}  // namespace

// [0, group|null, is_or_block, null, sid, conditions[], actions[], subevents[]?]
EventBlock parse_event_block(const JsonDoc& doc, uint32_t n) {
    EventBlock b;
    uint32_t group = doc.child_at(n, 1);
    if (is_arr(doc, group)) {
        b.is_group = truthy(doc, doc.child_at(group, 0));
        uint32_t gname = doc.child_at(group, 1);
        if (is_str(doc, gname)) b.group_name = std::string(doc.text_of(gname));
    }
    b.is_or_block = truthy(doc, doc.child_at(n, 2));

    uint32_t conds = doc.child_at(n, 5);
    if (is_arr(doc, conds))
        for (uint32_t c = doc.at(conds).child; c != kNone; c = doc.at(c).next)
            b.conditions.push_back(parse_condition(doc, c));

    uint32_t acts = doc.child_at(n, 6);
    if (is_arr(doc, acts))
        for (uint32_t a = doc.at(acts).child; a != kNone; a = doc.at(a).next)
            b.actions.push_back(parse_action(doc, a));

    uint32_t subs = doc.child_at(n, 7);
    if (is_arr(doc, subs))
        for (uint32_t s = doc.at(subs).child; s != kNone; s = doc.at(s).next)
            if (is_arr(doc, s) && is_num(doc, doc.child_at(s, 0)) &&
                doc.as_int(doc.child_at(s, 0)) == 0)
                b.subevents.push_back(parse_event_block(doc, s));

    return b;
}

// ---------------------------------------------------------------------------
// Project loading
// ---------------------------------------------------------------------------

void Project::load_object_types(const JsonDoc& doc, uint32_t node) {
    int index = 0;
    for (uint32_t t = doc.at(node).child; t != kNone; t = doc.at(t).next, ++index) {
        ObjectType ot;
        ot.index = index;
        ot.name = std::string(doc.text_of(doc.child_at(t, 0)));
        ot.plugin = doc.as_int(doc.child_at(t, 1));

        uint32_t sids = doc.child_at(t, 3);   // one SID per instance variable
        if (is_arr(doc, sids)) ot.instance_var_count = static_cast<int>(doc.size(sids));

        // Slot 7 holds plugin-specific data. For sprites that is the animation
        // list: [name, ?, ?, ?, ?, ?, sid, frames[]], where each frame is
        // [image, filesize, x, y, w, h, ?, hotspot_x, hotspot_y, ...].
        uint32_t plugin_data = doc.child_at(t, 7);
        if (is_arr(doc, plugin_data)) {
            for (uint32_t a = doc.at(plugin_data).child; a != kNone; a = doc.at(a).next) {
                if (!is_arr(doc, a)) continue;
                Animation anim;
                uint32_t aname = doc.child_at(a, 0);
                if (is_str(doc, aname)) anim.name = std::string(doc.text_of(aname));

                uint32_t frames = doc.child_at(a, 7);
                if (is_arr(doc, frames)) {
                    for (uint32_t f = doc.at(frames).child; f != kNone; f = doc.at(f).next) {
                        if (!is_arr(doc, f)) continue;
                        uint32_t path = doc.child_at(f, 0);
                        if (!is_str(doc, path)) continue;
                        Frame fr;
                        fr.image = std::string(doc.text_of(path));
                        fr.x = doc.as_int(doc.child_at(f, 2));
                        fr.y = doc.as_int(doc.child_at(f, 3));
                        fr.w = doc.as_int(doc.child_at(f, 4));
                        fr.h = doc.as_int(doc.child_at(f, 5));
                        uint32_t hx = doc.child_at(f, 7), hy = doc.child_at(f, 8);
                        if (is_num(doc, hx)) fr.hotspot_x = doc.num(hx);
                        if (is_num(doc, hy)) fr.hotspot_y = doc.num(hy);
                        anim.frames.push_back(std::move(fr));
                    }
                }
                ot.animations.push_back(std::move(anim));
            }
            const Frame* f0 = ot.first_frame();
            if (f0) ot.derived_name = name_from_image_path(f0->image);
        }
        object_types.push_back(std::move(ot));
    }
}

void Project::load_families(const JsonDoc& doc, uint32_t node) {
    for (uint32_t f = doc.at(node).child; f != kNone; f = doc.at(f).next) {
        uint32_t head = doc.child_at(f, 0);
        if (!is_num(doc, head)) continue;
        int fam = doc.as_int(head);
        if (fam < 0 || fam >= static_cast<int>(object_types.size())) continue;

        ObjectType& family = object_types[static_cast<size_t>(fam)];
        family.is_family = true;
        for (uint32_t m = doc.at(doc.child_at(f, 0)).next; m != kNone; m = doc.at(m).next) {
            int member = doc.as_int(m);
            if (member < 0 || member >= static_cast<int>(object_types.size())) continue;
            family.family_members.push_back(member);
            object_types[static_cast<size_t>(member)].member_of.push_back(fam);
        }
        // Families inherit a readable name from their first member.
        if (family.derived_name.empty() && !family.family_members.empty())
            family.derived_name =
                "fam_" + object_types[static_cast<size_t>(family.family_members[0])].derived_name;
    }
}

// [name, w, h, ?, event_sheet, sid, layers[]]
// layer: [name, index, sid, visible, bg, transparent, opacity, ...,  instances[]]
void Project::load_layouts(const JsonDoc& doc, uint32_t node) {
    for (uint32_t l = doc.at(node).child; l != kNone; l = doc.at(l).next) {
        Layout lay;
        lay.name = std::string(doc.text_of(doc.child_at(l, 0)));
        lay.width  = doc.as_int(doc.child_at(l, 1));
        lay.height = doc.as_int(doc.child_at(l, 2));
        uint32_t sheet = doc.child_at(l, 4);
        if (is_str(doc, sheet)) lay.event_sheet = std::string(doc.text_of(sheet));

        uint32_t layers = doc.child_at(l, 6);
        if (!is_arr(doc, layers)) { layouts.push_back(std::move(lay)); continue; }

        int layer_index = 0;
        for (uint32_t lyr = doc.at(layers).child; lyr != kNone; lyr = doc.at(lyr).next, ++layer_index) {
            LayerInfo info;
            uint32_t lname = doc.child_at(lyr, 0);
            if (is_str(doc, lname)) info.name = std::string(doc.text_of(lname));
            uint32_t vis = doc.child_at(lyr, 3);
            if (vis != kNone && doc.at(vis).type == JsonDoc::Type::Bool)
                info.visible = doc.at(vis).boolean;
            uint32_t op = doc.child_at(lyr, 6);
            if (is_num(doc, op)) info.opacity = doc.num(op);
            lay.layers.push_back(std::move(info));

            uint32_t insts = doc.child_at(lyr, 14);
            if (!is_arr(doc, insts)) continue;
            for (uint32_t i = doc.at(insts).child; i != kNone; i = doc.at(i).next) {
                uint32_t world = doc.child_at(i, 0);
                if (!is_arr(doc, world)) continue;
                Instance inst;
                inst.x      = doc.num(doc.child_at(world, 0));
                inst.y      = doc.num(doc.child_at(world, 1));
                inst.width  = doc.num(doc.child_at(world, 3));
                inst.height = doc.num(doc.child_at(world, 4));
                inst.angle  = doc.num(doc.child_at(world, 5));
                inst.object_type = doc.as_int(doc.child_at(i, 1));
                inst.uid         = doc.as_int(doc.child_at(i, 2));
                inst.layer       = layer_index;
                if (inst.object_type >= 0 &&
                    inst.object_type < static_cast<int>(object_types.size())) {
                    inst.vars.assign(static_cast<size_t>(
                        object_types[static_cast<size_t>(inst.object_type)].instance_var_count), 0.0);
                    lay.instances.push_back(std::move(inst));
                }
            }
        }
        layouts.push_back(std::move(lay));
    }
}

// Sheet body entries are tagged: 0 = event block, 1 = variable, 2 = include.
void Project::load_sheets(const JsonDoc& doc, uint32_t node) {
    for (uint32_t s = doc.at(node).child; s != kNone; s = doc.at(s).next) {
        EventSheet sheet;
        sheet.name = std::string(doc.text_of(doc.child_at(s, 0)));
        uint32_t body = doc.child_at(s, 1);
        if (!is_arr(doc, body)) { sheets.push_back(std::move(sheet)); continue; }

        for (uint32_t e = doc.at(body).child; e != kNone; e = doc.at(e).next) {
            uint32_t tag = doc.child_at(e, 0);
            if (!is_num(doc, tag)) continue;
            switch (doc.as_int(tag)) {
                case 0:
                    sheet.blocks.push_back(parse_event_block(doc, e));
                    break;
                case 1: {
                    EventVariable v;
                    v.name = std::string(doc.text_of(doc.child_at(e, 1)));
                    v.is_text = doc.as_int(doc.child_at(e, 2)) == 1;
                    uint32_t init = doc.child_at(e, 3);
                    if (is_str(doc, init)) v.initial_text = std::string(doc.text_of(init));
                    else if (is_num(doc, init)) v.initial_number = doc.num(init);
                    sheet.variables.push_back(std::move(v));
                    break;
                }
                case 2: {
                    uint32_t inc = doc.child_at(e, 1);
                    if (is_str(doc, inc)) sheet.includes.push_back(std::string(doc.text_of(inc)));
                    break;
                }
                default: break;
            }
        }
        sheets.push_back(std::move(sheet));
    }
}

Project Project::load(const std::string& path) {
    JsonDoc doc = JsonDoc::from_file(path);
    uint32_t project = doc.member(doc.root_index(), "project");
    if (project == kNone) throw std::runtime_error("data.js has no \"project\" key");

    Project p;
    p.load_object_types(doc, doc.child_at(project, kSlotObjectTypes));
    p.load_families(doc, doc.child_at(project, kSlotFamilies));
    p.load_layouts(doc, doc.child_at(project, kSlotLayouts));
    p.load_sheets(doc, doc.child_at(project, kSlotSheets));
    return p;
}

std::string Project::label(int object_type) const {
    if (object_type < 0) return "System";
    if (object_type >= static_cast<int>(object_types.size())) return "<bad type>";
    const ObjectType& t = object_types[static_cast<size_t>(object_type)];
    if (!t.derived_name.empty()) return t.derived_name + "[" + t.name + "]";
    return t.name;
}

namespace {
void tally(const EventBlock& b, size_t* ev, size_t* cn, size_t* ac, int depth, int* maxd) {
    ++*ev;
    *cn += b.conditions.size();
    *ac += b.actions.size();
    if (depth > *maxd) *maxd = depth;
    for (const EventBlock& s : b.subevents) tally(s, ev, cn, ac, depth + 1, maxd);
}
}  // namespace

size_t Project::total_events() const {
    size_t ev = 0, cn = 0, ac = 0; int d = 0;
    for (const EventSheet& s : sheets) for (const EventBlock& b : s.blocks) tally(b, &ev, &cn, &ac, 0, &d);
    return ev;
}
size_t Project::total_conditions() const {
    size_t ev = 0, cn = 0, ac = 0; int d = 0;
    for (const EventSheet& s : sheets) for (const EventBlock& b : s.blocks) tally(b, &ev, &cn, &ac, 0, &d);
    return cn;
}
size_t Project::total_actions() const {
    size_t ev = 0, cn = 0, ac = 0; int d = 0;
    for (const EventSheet& s : sheets) for (const EventBlock& b : s.blocks) tally(b, &ev, &cn, &ac, 0, &d);
    return ac;
}
int Project::max_nesting() const {
    size_t ev = 0, cn = 0, ac = 0; int d = 0;
    for (const EventSheet& s : sheets) for (const EventBlock& b : s.blocks) tally(b, &ev, &cn, &ac, 0, &d);
    return d;
}

}  // namespace mdz
