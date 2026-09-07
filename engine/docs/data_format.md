# The `data.js` format, as recovered from MiniDayZ+1.2

Everything here was derived empirically from the shipped export. Construct 2's
exporter minified `c2runtime.js` past recognition (no plugin names survive), so
the runtime was no help; the layouts below come from statistical analysis of
the data plus a handful of confirmations noted inline.

## Top level

`data.js` is plain JSON: `{"project": [ ...29 slots... ]}`. The slots this
engine reads:

| Slot | Contents |
|------|----------|
| 3 | object types (1070) |
| 4 | families (39) |
| 5 | layouts (7) |
| 6 | event sheets (6) |
| 7 | sound/music files (489) |

## Object types — slot 3

```
[name, plugin_index, ?, [instance_var_sids], ?, ?, null, plugin_data]
```

Names are minified to `t0`…`t1069`. `plugin_data` for sprites (plugin 18, 907
of the 1070 types) is the animation list, and each frame names its source
image — which is how readable names are reconstructed:
`images/gui_panel-sheet0.png` → `gui_panel`. This recovers 85% of all types;
the remainder are non-sprite plugins with no image to name them after.

The length of the instance-variable SID list gives the instance variable count.

## Families — slot 4

```
[family_object_type_index, member_index, member_index, ...]
```

Families occupy object type indices 1030–1068. The largest has 236 members.

## Layouts — slot 5

```
[name, width, height, ?, event_sheet_name, sid, layers[]]
layer:    [name, index, sid, visible, bg_colour, transparent, opacity,
           ?, ?, ?, ?, ?, ?, ?, instances[]]      <- instances at slot 14
instance: [[x, y, z, w, h, angle, ...], object_type_index, uid, ...]
```

## Event sheets — slot 6

```
[name, body[]]
```

Body entries are tagged by their first element:

- `0` — event block
- `1` — `[1, name, type, initial_value, ?, ?, sid, ?]` variable (type 1 = text)
- `2` — `[2, "SheetName", false]` include

### Event blocks

```
[0, group_info|null, is_or_block, null, sid, conditions[], actions[], subevents[]?]
```

`group_info` is `[true, "Group Name"]` on the 219 group blocks. Group names
survive; per-event comments do not.

### Conditions

```
[object_type, ace_index, behavior|null, trigger_mode, looping, inverted,
 type_flag, sid, false, params[]]
```

`object_type` is `-1` for the System object. Three booleans are stored, and
telling them apart matters:

- `looping` (slot 4) — **fixed per condition type**. Concentrated on 11 ACEs,
  almost all System (`For`, `Repeat`, `For Each`).
- `inverted` (slot 5) — **varies per use**. This is the real negation flag:
  349 uses spread across 96 ACEs and 73 object types.
- `type_flag` (slot 6) — **fixed per condition type**. 2763 uses; a property of
  the ACE, not of the call site.

Determinism was tested by keying every condition on `(plugin, ace, behavior)`
and checking whether both boolean values ever occur for the same key. Only
slot 5 varies, which is what identifies it as the negation flag.

`trigger_mode`: 0 normal, 1 trigger, 2 fast trigger. Cross-check: 578 blocks
have `is_or_block`-adjacent slot 2 set, and 578 conditions have a non-zero
trigger mode.

### Actions

```
[object_type, ace_index, behavior|null, sid, false, params[]]
```

Behavior names were **not** minified — `Pin`, `Bullet`, `Timer`, `Turret`, and
the mod authors' own `z_walker`, `attack`, `MainLook` all survive intact.

## Parameters and expressions

A parameter is `[slot_tag, payload]`. The payload is an expression tree, or a
bare string for name slots (variable names), or a bare integer for combo slots
(comparison-operator selectors).

Expression nodes are `[opcode, ...operands]`:

| Op | Meaning | Op | Meaning |
|----|---------|----|---------|
| 0 | int literal | 12 | `<>` |
| 1 | float literal | 13 | `<` |
| 2 | string literal | 14 | `<=` |
| 3 | unary minus | 15 | `>` |
| 4 | `+` | 16 | `>=` |
| 5 | `-` | 17 | `\|` (or) |
| 6 | `*` | 18 | `cond ? a : b` |
| 7 | `/` | 19 | system expression |
| 8 | `%` | 20 | object expression |
| 9 | `^` | 21 | instance variable |
| 10 | `&` (and / concat) | 22 | behavior expression |
| 11 | `=` | 23 | event variable |

The arithmetic and comparison assignments were confirmed by an idiom that
appears throughout the real data:

```
[18, [14, X, [0,0]], [3, X], X]     ==     X <= 0 ? -X : X     ==     abs(X)
```

which simultaneously pins op 18 as the conditional, op 14 as `<=`, and op 3 as
unary minus. Op 10 is confirmed by `[10, [2,"+"], ...]` — string concatenation.

Call-shaped nodes carry their own nested parameter lists:

```
[19, exp_index, params?]                                system
[20, object_type, exp_index, bool, null, params?]       object
[21, object_type, bool, null, var_index]                instance variable
[22, object_type, "Behavior", exp_index, bool, null, params?]
```

Note that parameter slot tags and expression opcodes are **different
namespaces** that both use small integers. Conflating them is the easiest
mistake to make when reading this format.

## What is not recoverable

- Object type names (minified; reconstructed approximately from image paths)
- Per-event comments
- ACE *identities* — actions and conditions are numeric indices into plugin
  tables that the minified runtime no longer names. The engine surface is
  bounded at 117 distinct conditions and 229 distinct actions.
