# Reading the ORIGINAL Mini DayZ (official 1.0 build, Construct 2)

Everything here reads the official build beside this folder, `../MiniDayZ+1.0` (its `data.js`, read past its
BOM, and its `images/`; `ORIG_GAME=<dir>` points elsewhere). Never 1.2 / Reloaded: those are fan mods. Nothing here
writes into the remake's checkout; `make_index.py` and `missing.py` read it at `$REMAKE`, default `../../minioutbreak`
(the remake cloned beside this repo).

Setup: Python 3 with Pillow (`cut.py`), Node with Playwright and a Chromium for `orig.cjs` (`PW=<path to the
playwright package>` if `require('playwright')` does not find it). The generated files below (`events/`,
`objects.txt`, `globals.txt`, `missing.txt`) are not committed: make them with
`python3 dump_events.py && python3 make_index.py && python3 missing.py` (a few seconds).

| file | what it is |
|---|---|
| `events/<sheet>.txt` | the 6 event sheets, one line per condition/action, every line prefixed by its event path (`13.2.5.1`) |
| `events/OUTLINE.txt` | every group and every `Function` with its path: start here to find things |
| `events/ALL.txt` | all sheets concatenated (for grep) |
| `show.py` | `show.py 13.2.5` prints that event and all it contains; `show.py -f Player_get_hit` a Function and its callers; `show.py -g REGEX` greps all sheets |
| `objects.txt` | one line per object type: t-id, name, plugin, families, behaviours, instance-variable values of the first placed instance, where placed, effects, animations (frames, speed, loop), sheet files |
| `globals.txt` | every global variable with its initial value; layouts, their layers (flags) and what is placed on each; audio files; containers; and the 29 globals the remake took from the wrong build (see Surprises) |
| `missing.txt` | object art with no folder in the remake's `Assets/Sprites`, grouped, with where else in `Assets` it is |
| `cut.py` | `cut.py wolf_skin OUT/` cuts every frame to `OUT/<anim>_<n>.png` + `frames.json` (origin, image points, polygon), pixel-identical to the `Assets/Sprites` rip; `cut.py --list wolf` |
| `orig.cjs` + `orig_helpers.js` | drive the running original in headless Chromium; `window.ORIG` helpers in the page |
| `c2orig.py` | the shared reader (object names, expression printer) the scripts use |
| `ace/` | how condition/action/expression names were recovered (`match.py`, `acenames.json`) |

Regenerate the dumps: `python3 dump_events.py && python3 make_index.py && python3 missing.py`. The names in
`ace/acenames.json` are committed; recomputing them (`python3 ace/match.py`) needs the five public Construct 2
runtimes it compares in `ref/<repo>/c2runtime.js` (listed in `ace/match.py`'s `REFS`; not committed), and
`ace/refdump.cjs` re-dumps them, only needed if `ace/min.json` is lost.

## Reading the event dumps

```
13.2.5.1.1           event sid=167521387402389
13.2.5.1.1             if    fam_zed_army_skin1.IsOverlapping(player_collision_base)
13.2.5.1.1             if    player_base.interactor.HasLOSToObject(fam_zed_army_skin1)
13.2.5.1.1.1           event sid=775801302056762
13.2.5.1.1.1             loop  System.ForEach(fam_zed_army_skin1)
13.2.5.1.1.1             do    Audio.PlayAtObjectByName(opt:0, choose("attack_0", ...), opt:0, 0, fam_zed_army_skin1, 360, 360, 5, "zmb")
```
* Kinds: `group` (numbered like events), `event` (`[or]` = OR block, its later conditions read `or`), `on` trigger, `if`, `loop`
  (For each / Repeat / While...), `else`, `NOT` = inverted, `do` action, `var` local (or, at sheet root, global) variable, `incl`.
* `Object[.Behaviour].Name(params)`: `player_base.interactor.HasLOSToObject` is the LOS behaviour that `player_base` calls
  "interactor". Names are the C2 runtime names (`Sprite.cnds.IsOverlapping` prints as `obj.IsOverlapping`).
* Objects are named by their art: `zed_normal_skin1`. Several types drawing the same sheet get `[tN]`
  (`fnx_pistol[t139]`), objects with only a texture by its file (`ground_lvl1_tilemap`), objects without art
  `Plugin[tN]` (`Text[t233]`, `Arr[t235]`), singletons the plugin
  (`Function`, `Audio`, `Keyboard`, `Touch`), families `fam_<first member>`. `objects.txt` resolves any of them.
* `var#N` = instance variable N. **The export keeps no instance-variable names**, only indices; `objects.txt` shows each
  type's values on its first placed instance (zed bodies: `[hp, _, archetype, _, max_hp, ..., damage at #10, ...]`).
* `opt:N` combo choice (Audio: `opt:0` = not looping / sounds folder), `key:87(W)`, `layout:"Map"`, `sound:"x"`,
  comparisons `= <> < <= >= >`. Expressions are complete (the old dumps' `?op7` was expression node type 7: division); `&` is C2's
  concatenation / logical and, `a ? b : c` its conditional.
* Not in the export: comments, instance-variable names, disabled events. Every group carries a
  `System.IsGroupActive(<itself>)` condition from the exporter; the dump leaves it out.
* Names: ~65 were decided by hand from the minified code, 32 by the order a plugin fires its triggers, the rest by
  matching bodies against unminified C2 runtimes from other games (`ref/`) and voting (Closure renamed each property
  name to ONE name program-wide: `od` is SetEnabled on every behaviour). Bodies that differ only in a mangled field
  tie (viewportleft/bottom, SaveState/LoadState, SetMuted/SetLooping...); `python3 ace/match.py -a` lists the ties and
  every used one was settled by reading what the field is. `ace/acenames.json` says how each name was decided.
  Cross-check passed: each condition/action's parameter count in data.js matches the real C2 function's arity.

## Running the original: orig.cjs

`bash serve.sh` (serves 1.0 on 127.0.0.1:8731), then
`node orig.cjs --out DIR [--res 1280x720] [--touch] --steps "wait:22000;shot:menu;..."` -- steps in its header:
`wait shot click tap down up move key hold release js jsfile`. Boot to the menu takes ~20 s.

* **Menu buttons only answered touch taps** in headless runs (`--touch` + `tap:`); mouse `click:` did nothing.
  Novice run at 1280x720: `tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;wait:12000` (New game, Novice, Start)
  -> layout `Map`, HUD up. Regular/Veteran/Legend are at y 353/389/425 instead of 317.
* `js:EXPR` cannot contain `;` and **must never assign a bare global** (`z=...`): the minified runtime's own globals are
  1-3 letters (`z` is its clearArray) and the game breaks. Use `ORIG.v.x = ...`, or `jsfile:path.js` (any JS, `return` prints).
* `ORIG` (orig_helpers.js): `layout() tid(name) type(n) insts(n) player() where(n) create(n, layer, x, y)
  call(function, ...params) global(name[, value]) globals(regex) move(inst, x, y) teleport(x, y) spawnNPC(kind, dx, dy)`.
  Minified runtime names if you go further: `rt.S` types by index, `type.q` instances, `rt.wa` running layout,
  `layout.ua` layers, `rt.Yn(type, layer, x, y)` createInstance, `rt.tD` global vars `{name, data}`, `inst.cc` instance
  vars, `inst.u` angle (radians), `inst.P()` after moving, `c2_callFunction(name, [params])` runs a game Function.

## Three worked examples

**1. The zombie attack events.** `grep -n Zed events/OUTLINE.txt` finds group 13.2.5 "Zed_common_dmg_deal"; `python3 show.py 13.2.5`:
every 1 s (`System.Every(1)`), for each zombie body overlapping `player_collision_base` that the player's LOS behaviour
"interactor" can see, play one of `attack_0/1/10..13` at the zombie, then by its archetype `var#2` call
`Player_get_hit("zed_normal", Zed_dmg)` (2 = army, `Zed_army_dmg`; 3 = fast, `Zed_fast_dmg`; 7 = "zed_fresh"). Tanks and
jumpers are 13.2.6 / 13.2.7, wolves 13.2.4. Zombie movement/AI is group 13.3 ("Zed Normal Puncher" ...). The damage
globals are set per difficulty at layout start: `python3 show.py 2.11.12` (Novice: Zed_dmg 5), 2.11.11 Regular 9, Veteran/Legend 10.

**2. What sound plays when the player is hit.** `python3 show.py -f Player_get_hit` -> defined at 15.3, called from 34 places.
`python3 show.py 15.3.5`: unless armour saves it (`def_percent < random(100)`), it calls `Hit_effect` (15.4: layout
effect "Hit_effect" to 100 for 0.2 s), shakes the camera `ScrollTo.Shake(3, 0.4)` and plays
`choose("body_1", "body_2")` at -5 dB on the player (tag "zmb"); the attacker's own sound comes from the attack event
(zombies `attack_*`, wolves `wolf_attack*` in 13.2.4). Files: `media/body_1.ogg|m4a` (listed in globals.txt).

**3. Spawn a wolf next to the player in a run and screenshot it** (examples/wolf.png):
```
node orig.cjs --out /tmp/o --touch --steps "wait:22000;tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;wait:12000;js:[ORIG.global('Timer_ALL',720),ORIG.global('Timer_Hours',12)];wait:3000;js:ORIG.spawnNPC(13,220,-40);wait:500;shot:wolf;js:ORIG.where('wolf_eyes')"
```
`spawnNPC` creates a `zed_resp_point` beside the player and runs the game's own `Spawn_NPC(point, 0, 13, 0)` (13 = wolf,
events 13.1.1.13), so the wolf is pinned, bounced and given its LOS obstacles exactly as the game does it.
Kinds: 1 zed_normal, 3 fast, 4 shooter, 6 screamer, 9 army, 13 wolf, 14 deer, 15 rabbit (others in 13.1.1.N).
Setting `Timer_ALL` (minutes of the day, 360 = 06:00) moves the clock; a run starts at 06:00, set midday for a clear picture.

## Surprises worth knowing

* **The remake's `game/data/generated/original_globals.lua` is not from 1.0.** All 290 of its values equal the
  MiniDayZ+1.2 fan mod's; 29 differ from 1.0 (end of globals.txt): Novice/Regular/Veteran starving, thirst and
  regeneration rates; 1.0 has `Player_Hear_radius` 2000 (remake 1000), `ZED_RESPAWN_TIME` 10 (remake 60), `Zed_per_base`
  15 (remake 5), `perk_cost` 200 (remake 100), the `Zombies_active_*` switches 1 (remake 0). docs/original_data.md's C2SourceData.js (8,629,978 bytes) is
  not the official `data.js` (8,564,509 bytes = 1.0). Per-zombie hp/damage on the placed bodies are the same in both.
* The original's world is the `Map` layout (17850 x 20000, 69 layers) filled at run time by group 3 "Map generation"
  from `map_element` placement objects (`var#2` picks the building; see 3.5.7.1.6.N).
* Doors (docs/original_data.md checked): the plate spawns the facade at origin 0 and the facade spawns `door` at image
  point 3 and sets its animation (3.5.7.1.6.N) -- but the HQ (`b_military_shtab`) spawns TWO doors (points 3 and 4) and
  the fire station FOUR (3, 4, 5, 6), not one each. Everywere's "autosave loop" is a retry: OnSaveFailed, wait 5 s,
  save again if alive on Map. Zed_dmg is 24 initially but every difficulty overwrites it (Novice 5 ... Legend 10).
* Some building types draw another's interior art: the brown garage is `b_garage_blue_int[t249]` + `b_garage_brown`.
* Controls: groups `Movement_stick` (12.13.6) and `Movement_wasd` (12.13.7) start inactive; 3.6.3.x / 3.7.3.x switch
  one on at run start (`show.py -g 'Movement_(stick|wasd)'`). The menu only answered touch in headless Chromium.
* Every group's first condition is `IsGroupActive(<itself>)`; there are 219 groups, 24 start inactive (the Veteran enemy groups, ... -- `grep INACTIVE events/OUTLINE.txt`).
