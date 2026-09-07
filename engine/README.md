# MiniDayZ C++ engine — picking prototype

A C++ reimplementation of the Construct 2 runtime that MiniDayZ+1.2 was built
with, aimed at making the game's logic readable and modifiable.

This directory currently contains the **picking engine prototype**: the part of
the runtime that decides which object instances each condition and action
applies to. It was built first because that is where behavioural fidelity is
won or lost — the individual actions are easy, the instance selection rules are
not.

## Layout

| File | What it does |
|------|--------------|
| `src/json.{hpp,cpp}` | Arena-backed JSON reader (8.3 MB in ~220 ms, 40-byte nodes) |
| `src/project.{hpp,cpp}` | `data.js` → object types, families, layouts, event sheets |
| `src/picking.{hpp,cpp}` | **The picking engine.** SOL semantics, families, OR blocks |
| `src/runner.{hpp,cpp}` | Event walker: condition order, sub-event scoping, for-each |
| `src/decompile.{hpp,cpp}` | Renders event sheets as readable pseudocode |
| `src/main.cpp` | Loads the real export and drives every event through the engine |
| `src/dump_main.cpp` | The decompiler CLI |
| `data/ace_names.txt` | Editable ACE name table (all entries are inferences) |
| `tests/picking_tests.cpp` | One test per Construct 2 picking rule |
| `docs/data_format.md` | The recovered `data.js` format |

The engine never evaluates a real condition itself — it calls a predicate
supplied by the caller. That is what lets the picking rules be tested, and the
whole event graph be traversed, before a single plugin ACE exists.

## Build and run

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
./build/picking_tests
./build/mdz_probe ../MiniDayZ+1.2/data.js

mkdir -p dump
./build/mdz_dump ../MiniDayZ+1.2/data.js dump data/ace_names.txt
```

The dump is generated, not checked in -- it takes about a second to rebuild.
It writes one file per event sheet plus `object_index.txt`, which maps every
minified `tN` to its reconstructed name.

## Naming ACEs

Actions and conditions are numeric indices into plugin tables that the
minified runtime no longer names, so `data/ace_names.txt` maps them by hand.
Every entry there is an inference from the parameter signature, and the
decompiler always prints the raw index next to the name, so a wrong guess is
visible rather than silently misleading. The seeded table covers the heaviest
call sites; extend it as you identify more.

## Status

Passing: 27 checks across the seven picking rules, and a full traversal of the
real project — 15,896 / 15,896 event blocks, 21,436 conditions, 44,297 actions,
nesting depth 13/13, in 181 ms.

The decompiler renders all six sheets. 51% of condition call sites and 41% of
action call sites currently resolve to a name; the rest print as `#N`, with
their object, parameters and expressions still fully readable.

Not started: rendering, the 346 plugin ACE implementations, behaviors, audio,
save/load.
