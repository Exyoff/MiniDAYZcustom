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
| `tools/capture.js` | Boots the original build in Chromium and screenshots it |
| `tools/play.js` | Drives it through the menus into gameplay |
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

## Differential testing

`docs/differential.md` records what comparing state against the original
runtime has found, including one wrong answer that full coverage reported as
fine. Best result so far: 386/401 variables matching after booting the same
layout chain.

## Reference capture

`tools/` runs the original browser build in headless Chromium and captures what
it actually does. This is the fidelity oracle: it is what any reimplementation
has to be compared against, and it reports the console, failed requests and
404s that tell you whether the build loaded cleanly.

```sh
npm install playwright-core          # browsers are already present; do not re-download
NODE_PATH=<node_modules> node tools/capture.js ../MiniDayZ+1.2 /tmp/cap
NODE_PATH=<node_modules> node tools/play.js    ../MiniDayZ+1.2 /tmp/play
```

Captured frames are deliberately not committed -- they are game imagery, and
they regenerate on demand.

Verified working: the 1.2 build boots to the menu and plays, with WebGL
running under SwiftShader, no page errors and no failed requests. The one
console error is `EncodingError: Unable to decode audio data`, which is
headless Chromium lacking the AAC codec for the `.m4a` files, not a fault in
the build.

Navigating the menu needs exact targets: the main menu column sits near x=512
but the difficulty column shifts to x=411, and the Start button only appears
*after* a difficulty is chosen. Clicking a fixed centre point silently lands on
"Back" and bounces between screens.

## Naming ACEs

Actions and conditions are numeric indices into plugin tables that the minified
runtime no longer names. `data/ace_names.txt` maps all 423 of them.

These names are not guesses from parameter shapes. `tools/extract_aces.js`
pulls each ACE's real implementation out of the live runtime and
`tools/join_aces.py` pairs it with the (plugin, ace, behavior) key by SID; the
names were then read off those implementations. Many are pinned exactly by the
runtime's own `saveToJSON` property names -- the Car behavior serialises
`acc`/`dec`/`steerSpeed`/`driftRecover`, for instance, which settles four
setters that look identical from the outside.

Entries are scoped by behavior where needed, since the same (plugin, ace) pair
means different things across behaviors. 385 are high confidence, 31 medium and
7 are honest unknowns whose bodies did not identify themselves. The decompiler
always prints the raw index beside the name, so a wrong one stays visible.

## Status

Passing: 27 checks across the seven picking rules, and a full traversal of the
real project — 15,896 / 15,896 event blocks, 21,436 conditions, 44,297 actions,
nesting depth 13/13, in 181 ms.

The decompiler renders all six sheets, and every call site resolves to a name:
21435/21436 conditions and 44297/44297 actions. The single holdout is the one
ACE key whose implementation was not consistent across call sites.

Expressions are NOT resolved: they are a third index space that the runtime
discards the index for, and positional correlation was tried and shown to be
wrong. `docs/expressions.md` records the evidence and what to try next. Until
that is solved every object and system expression evaluates to zero, which
`mdz_run` counts rather than hiding.

Not started: audio, save/load, most behavior state.
