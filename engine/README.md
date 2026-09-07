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
| `src/main.cpp` | Loads the real export and drives every event through the engine |
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
```

## Status

Passing: 27 checks across the seven picking rules, and a full traversal of the
real project — 15,896 / 15,896 event blocks, 21,436 conditions, 44,297 actions,
nesting depth 13/13, in 181 ms.

Not started: rendering, the 346 plugin ACEs, behaviors, audio, save/load.
