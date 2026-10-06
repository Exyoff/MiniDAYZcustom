# Handoff: MiniOutbreak, from the cloud session to a desktop session (2026-10-06)

Read this first, then `toolkit/README.md`, `toolkit/comparison/README.md` and the remake's `Progress.md`
("Where this stands" and Stages 26 to 28).

## The two repositories

- **Exyoff/minioutbreak** (private): the remake, a LÖVE 11.5 game (Lua 5.1) with a love.js web build.
  `.github/workflows/publish-web.yml` builds and publishes it to exyoff.github.io/MiniOutbreak on every push
  to `main`. `Progress.md` is the project's record: every stage, every ask in the user's words, every item.
- **Exyoff/MiniDAYZcustom** (this one): the original game. `MiniDayZ+1.0` is the **official** browser build
  (Construct 2; `data.js` is the whole project, `images/` every sprite sheet) and the only reference.
  `MiniDayZ+1.2` and `MiniDayZ+Reloaded` are fan mods: never compare against them. `toolkit/` reads and
  drives the official build (event sheets dumped readable, objects, globals, a frame cutter, a headless
  driver). Clone the remake beside this repo (`../minioutbreak`) and the toolkit finds it.

## Where things stand

| branch (minioutbreak) | commit | what |
|---|---|---|
| `main` | `696144e` | **Published** (publish run succeeded 2026-10-05 20:53 UTC): Stage 28 (a zombie's blow is a hit; fronts hidden only while inside; wolves, deer, rabbits), 27.4 (the map as the original's notebook, opened by its notepad button), 27.1 (every room the original has) |
| `claude/minioutbreak-exploration-jhs27h` (integration) | `7871bda` | main + **27.2** (a 26x16-lot world, 31200x19200 px, five seasons of 100 tiles, lots built as the player nears them) + **27.3** (44-52 places incl. hospital, fire station, camps, crashes, checkpoints; roads join every place). Merged and tested, **not published**: Progress.md has no 27.2/27.3 entries yet, and a review's findings are open (below) |

Every other branch the cloud session used (work/map, work/rooms, work/fauna, work/mapscreen, work/merge27)
is contained in the integration branch; none was pushed and none is needed.

Tests on the integration tree: the merge ran 132 scenarios by name (4 asked pre-merge numbers and were
updated), then 69 more (the tracks' scenarios it had not run, and every combat, save, play and render one)
plus the lints: 77/77. Lint 12/12 again after the final merge. The merging agent also hashed seeds 1-40
natively and in the web build: identical. `generate.VERSION` is 4: saves made before this world are refused
by CONTINUE with their reason.

## Next, in order

1. **Fix the merge review's findings** (M1-M5 below) on the integration branch; run the scenarios each
   touches plus lint. Two of the review's four lenses never ran (a usage limit): **saves and
   determinism**, and **screens and per-frame cost in the big world** -- run those reviews.
2. **Write Progress.md 27.2 and 27.3**: the map track's own entry and notes are in
   `toolkit/handoff/progress_27_2_27_3.md`; follow how 27.1 was written (remake commit `696144e`):
   entry after 27.1 and before 27.4, the merge as "where it met the rest" items, its dated notes in 27.5,
   the TOC row, "Where this stands" and its Latest table (rows for "Increase the height", "make the map
   grid uniform" = roads connect every place, "more interest points with roads", "biomes 100 across").
3. **Publish**: fast-forward `main` to the integration branch, push, and check the "publish web build" run.
4. **Finish the comparison** (`toolkit/comparison/`): menus, hud, inventory, character, zombies and combat
   are surveyed (268 differences, about 206 "make identical"); **items, survival, world, audio, events**
   are not. Then turn all of it into a backlog of fix tracks, ordered, keeping every user ask
   (`toolkit/comparison/user_asks.md`: anything not traceable to one of them is to be made identical).
5. **Build the fix tracks**, two at a time, each: build, adversarial review, fix, merge, test set, Progress,
   publish. Already known, beyond the surveys: `game/data/generated/original_globals.lua` was taken from the
   1.2 fan mod (29 of 290 values differ from 1.0 -- `toolkit/globals.txt`'s end lists them); the corner
   minimap (the original has none; the user asked for the map as an openable screen); the camera (the
   original shows about 1280 px across at 1280x720 and closes in to ~2x indoors, the remake is ~2.3x
   everywhere: 27.1.11); the map track's leftovers (27.3.13: hospital/fire-station pads unpaved, checkpoint
   props on the roadway, roadside kinds in 4 of 5 seasons, the explored-world autosave on a slow phone);
   27.1.10 (buildings with several doors hang one leaf); the map screen's Guides/Crafting/Tasks tabs.

## The user's standing instructions

- *"continue, compare against official minidayz, use screenshots and source code to compare. Do not stop
  working until it is identical except for the changes i had asked for specifically"* (run as a loop).
- *"continue, less agents"*: at most two agents at once.
- Publish to `main` when a stage is done (Progress.md ground rule 8); *"publish it too"*.
- No pull requests. Push only to the integration branch and `main`.
- Agents run only their own scenarios plus lint: never the full suite, never `--changed`, never a whole
  subject but lint. The orchestrator runs a test set at each merge.
- No model names in commits, code or docs. End commit messages with the attribution lines your own
  session is given.

## How the work was run (what to reuse)

- **Tracks in worktrees**: `git worktree add -b work/<track> ../wt/<track> <integration head>`, copy
  `game/built`, and give it `OriginalData/` (gitignored in the remake; `tools/c2data.py`, the build's metadata
  step and `tools/extract_rooms.py` read `OriginalData/C2SourceData.js`, the official `data.js` with its
  BOM stripped, and `OriginalData/images` = `MiniDayZ+1.0/images`). Paths in the findings below that start
  `/tmp/claude-0/...` were the cloud session's and did not come along.
- **Per-agent save dirs** so captures never collide: each agent its own `APPDATA` (with `APPDATA/LOVE` ->
  `$XDG_DATA_HOME/love`, which verify.py's reload scenario needs) and `XDG_DATA_HOME`; on Linux headless,
  `Xvfb :99`, `ALSOFT_DRIVERS=null`, `SDL_AUDIODRIVER=dummy`.
- **Scenarios by name**: `toolkit/handoff/run_named.py NAMESFILE [--subjects lint]` (one name or prefix
  per line; `REMAKE=<checkout>` picks the tree).
- **One capture**: `love game --shot --frames N --beat B --res WxH --script "step;step" [--world SEED]`
  from the remake's root; `toolkit/handoff/cap.py` wraps it (set its `WT`).
- **The original, running**: `toolkit/serve.sh`, then `node toolkit/orig.cjs --out DIR --touch --steps
  "wait:22000;tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;..."` (headless, the menu answers
  taps only: New game 640,353, Novice 540,317, Start 740,428). Never assign a bare global in a `js:` step.
- **Workflows**: `toolkit/handoff/track.js` (build, review, fix for one track) and `survey.js` (the
  area-by-area comparison) are the scripts the cloud session ran; their `SP` constant points at its
  scratchpad -- change it, and the paths in their prompts, before reuse.

## The merge review's findings (open)

Found by reviewers trying to break the merge of 27.2/27.3 into 27.1/27.4/28; their verification did not
run (usage limit), so treat each as a lead to confirm. None blocks a run; M1 and M2 come from the merge.

### M1. A zombie-bucket change resets the shared sleep clock, so the animals' 15-frame whole pass can be put off indefinitely and asleep animals stay asleep next to the player (minor, unverified)

- **Where:** `game/src/game/systems/population.lua`:260
- **Introduced by:** The merge resolution of population.lua. On work/map, `state.sleep_tick = whole and 0 or tick` was correct because `whole` included the (only) zombie bucket's change. Generalising to two kinds tied the clock reset to the zombie kind alone.

**Failure:** Seed 7, every lot built, refills shut. The player approaches an asleep rabbit at 10 px a frame (below JUMP, so no jump pass) while a zombie is spawned somewhere asleep every 10 frames. Each zombie flush makes changed(zombie) true, so `zombies` is whole and `state.sleep_tick = zombies and 0 or tick` resets the one shared clock. `animals` is only `whole or changed(animal)`, so the animals never get their whole pass. The rabbit's asleep flag is never re-asked. It is then 110 px from the player and still asleep for as long as the churn lasts. It is not drawn (missing from render.draw_order), not a target (missing from targeting.candidates), not updated and not copied back, but its dynamic body still collides. The code comment and docs/architecture.md promise at most a quarter-second late wake.

**Evidence:** population.lua:253-266: `local whole = tick >= SLEEP_EVERY or alive ~= ... or jumped; local zombies = whole or changed(state, "zombie"); local animals = whole or changed(state, "animal"); state.sleep_tick = zombies and 0 or tick`. Probe rvA/probes/starve_z10.py (one zombie every 10 frames) printed `41,nil,680,false,0`: the player came within 2000 px at hook frame 41, the rabbit never woke during the 300 hooked frames, the player passed over it, and the animal count did not change. The control starve_ctl.py (no churn) printed `41,49,...`, a wake 8 frames after coming in range. starve_near.py (player stops 110 px away) printed `true,110,false,false` (asleep, distance, in draw list, target candidate). 20 frames after the churn stopped it printed `false,110,true,true`. In a natural 10,000 px walk at 2 px/frame (natural_2.py) the result was `0,14,15,...`: no late wake, with only 14 zombie-bucket changes. So this needs sustained churn: kills, refills or town-lot builds at intervals under 15 frames.

**Proposed fix:** Reset the shared clock only on a pass that covers both kinds: `state.sleep_tick = whole and 0 or tick`. A single kind's bucket change then still gives that kind an extra whole pass without moving the other kind's schedule. Alternatively keep a tick per kind (state.sleep_tick_zombie / state.sleep_tick_animal). A scenario could approach an asleep animal at sub-JUMP steps while zombies are spawned every 10 frames, expecting a wake within 15 frames of coming in range.

### M2. save.fingerprint ignores map.animals, but the save's lots record now counts each lot's animal ('a') entries, so a change to animal placement lets an old save stand points twice or never (minor, unverified)

- **Where:** `game/src/game/save.lua`:757
- **Introduced by:** The merge. The fauna track's fingerprint deliberately excluded animals in a whole-map world, where a load simply restored the file's animals. The merge put the animals into the lot buckets, whose progress the save records by entry count. Not reachable today, because VERSION 4 refuses every older save; it is latent until the next animal tweak.

**Failure:** Tune data/animals.lua spawn (chance, points, scatter, clear or gap) and CONTINUE a save made before the change. The fingerprint is unchanged, so the save is accepted, but each lot's entry list now ends in a different 'a' tail than the k the file recorded. Example: a lot whose old tail was 1 deer (k = m+1) and new tail is 5 wolves. loader.restore builds entries m+2..m+5, giving 4 wolves; the first wolf's entry counts as done and is never stood. The file's deer comes back with a spawn_id that now names another point. That point counts as filled, so it never refills, and when its own lot is first built its entry stands a second animal for it. The reverse case: an old 5-wolf tail and a new 1-deer tail clamps k, restores the file's 5 wolves, and refill_animals adds a deer (live[deer index] = 0). wildlife.lua's header still says the fingerprint leaves the animals out so that 'a run saved before they came still loads, and finds them'.

**Evidence:** save.lua:757-774 mixes objects, containers, zombies, items, bounds, spawn and version, but not map.animals. loader.lua:176 `add("a", map.animals)` puts each animal point at the tail of its lot's entries. loader.record writes k per lot, and loader.restore:347 clamps it against the new total(L, lot) and builds k+1..n. Probe rvA/probes/fp.py: generate.build(7) with spawn.chance 0.5 gives fingerprint 1163085545 and 304 animals; with spawn.chance 0.6 it gives fingerprint 1163085545 and 357 animals.

**Proposed fix:** Mix map.animals (kind, x, y) into save.fingerprint and correct wildlife.lua's header comment. Or keep animals out of the per-lot count (bucket them separately), and on load stand a built lot's animal points by occupancy (spawn_id) rather than by entry count.

### M3. The first frame after a rebuild or load updates every zombie and animal in the world, since state.acting is nil and every fresh brain starts awake (minor, unverified)

- **Where:** `game/src/app.lua`:896
- **Introduced by:** Pre-existing for zombies on work/map, whose app.update had the same fallback and no sleep pass at rebuild. The merge extends it to the 304 animals and the zombie-by-animal sight walks, about 7-13 ms of the 27-33 ms measured natively.

**Failure:** CONTINUE (or the console's load) of an explored seed-7 world: 571 zombies and 304 animals. app.rebuild and save.apply leave state.acting nil, so entity.acting falls back to the whole buckets (entity.lua:91). Every brain was just made with asleep=false, so the first gameplay frame runs ai.update for all 571 zombies and fauna.update for all 304 animals. That includes each zombie's animal_in_sight over all 304 animals and each thinking wolf's nearest_prey over all 571 zombies. Far wolves and infected may spot each other and start chases or flights before the frame's population.sleep puts them to sleep.

**Evidence:** Probe rvA/probes/firstframe.py (frame time / zombie updates / animal updates): `22:93.63/3/1 23:33.02/571/304 24:1.75/3/1`. A second run gave 27.5 ms for frame 23. The same probe with every animal removed before saving (firstframe_noanimals.py): `23:20.36/571/0 24:1.06/3/0`. Normal frames are about 0.3-2 ms natively with the JIT. A phone or the browser will be several times slower.

**Proposed fix:** Call population.sleep(state) at the end of app.rebuild and after save.apply's last flush. state.acting then exists and everything past sleep_range is asleep before the first update, so the walk, animal_in_sight and nearest_prey see only the awake few from frame one.

### M4. Leaves with big-page art (the church's door, the fire station's three garage doors) are never drawn when they have a look: from inside a shut one is invisible, and from the street an open one is a see-through hole (minor, unverified)

- **Where:** `game/src/game/door.lua`:246
- **Introduced by:** Pre-existing on parent 1 (cf6154c). door.draw has not changed since 23.2 and is byte-identical on cf6154c. 27.1 gave the church its leaf and the fire station its garage leaves (more_doors). The merge only makes the buildings common: 27.3 stands two fire stations in every world and a church in one town in four.

**Failure:** Seed 7, `tp 16040 12990`: the player is inside the fire station with every door shut. The side door (firestation2) lies across its gap as a red bar. The three garage doorways show bare floor although their bodies are active, so the player walks into an invisible wall. Open the garage doors from the street (`tp 16100 13200`, then door.set_open): the bays show the gravel through the facade's holes, where the side door shows the dark doorway. Seed 7's church at (19560,3480) behaves the same way: from inside with its door shut nothing lies across the doorway, and from the street with it open the arch shows the ground behind the facade.

**Evidence:** Code: door.draw does `local page = atlas.page(key); local image = page and atlas.page_image(page); local quad = image and atlas.quad(key); if not quad then return end`. For a frame packed on the big page, atlas.page() returns "big". atlas.page_image("big") is nil (atlas.lua:99-111): pages[] holds only the sm and md pages, and big frames are separate images loaded through atlas.big_image. So door.draw returns without drawing either look. The only door frames on the big page are door/church_0 and _1 and door/firestation_0 and _1 (data/generated/atlas.lua:177-180).

Probe p4 (env/rvB/p4.lua, output in p4_out.txt): the fire station's more1-3 report key=door/firestation_0, look=across, body active. The church door reports look=across, active.

Captures in /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad/env/rvB/caps/:
- fs_inside.png and fs_inside_zoom.png: no bar across the garage gaps; the side door has one (fs_inside_zoom2.png).
- ch_inside.png and ch_inside_zoom.png: the church's shut doorway is empty.
- ch_open.png against ch_shut.png: the opened church door shows the ground through the arch.
- fs_open.png and fs_open_zoom.png: with all doors open, the side door is dark and the garage bays show the gravel.

The same church test (probe p7: place b_church, open its door) on a cf6154c copy gives env/rvBcf6/caps/p7_cf6.png, which matches the merge's env/rvB/caps/p7_merge.png (both in p7_both.png).

**Proposed fix:** In door.draw, when atlas.page(key) == "big", use image = atlas.big_image(key) and a quad over the whole image (cache love.graphics.newQuad(0, 0, w, h, w, h) per key), then draw the "dark" and "across" looks exactly as for paged frames. Add the church and a fire station garage door to the scenario that reads the shut door's across bar, or count door.draw's draws there.

### M5. The lot builder stands each map zombie at its point with no wall check, and 27.3's camp and checkpoint points fall inside rooms' walls and props: a castle camp's zombie is shoved into the castle (minor, unverified)

- **Where:** `game/src/map/loader.lua`:100
- **Introduced by:** Pre-existing on parent 2 (work/map 2f97966): fill.lua and spawn_zombie are unchanged by the merge. On work/map the castle and the shed stood on solid bands as deep as their plates, so those points were inside a band. The merge (27.1's rooms meeting 27.3's camps) turns the castle and shed cases into points inside a room's walls, and the ejected zombie can land inside the room.

**Failure:** Seed 15's castle camp: its one zombie point is at (-25,-129) from the castle's foot (19790,719), inside a wall rect of b_castle_int. When the lot is built, spawn_zombie makes the zombie inside that wall and Box2D shoves it out into the castle's room: it ends at (-9,-114), and interior.room_over files that spot in b_castle. In seed 4 the radio camp's point lies in the small shed's walls (3,-122 from its foot). At checkpoints, the corner points (corner + 40,40) fall inside the police cars and sandbags the same variant stands at (50,40) and (60,50), 2 to 4 points a seed.

**Evidence:** Probe p2 (env/rvB/p2.lua, output in p2_out.txt), seeds 1-22, testPoint against WALL and WALL_INSIDE fixtures: 39 zombie points lie inside a wall. Two of them are inside a room's walls box (seed 4 b_small_shed, seed 15 b_castle). The rest are at checkpoints (police, sandbags) and camps (drone, predator tree, radio mast). No animal point is affected.

Probe p11 (p11_out.txt) follows seed 15's zombie (id 140): at frames 30 and 150 it is inside the castle room, idle.

Code: population's refill tries population.clear (population.lua:144) before spawning. loader.spawn_zombie (loader.lua:100) spawns at z.x, z.y unconditionally. fill.camp keeps its ring of trees off `footprints(stood, f.clear)` but rolls its zombie points anywhere in 450-750 of the lot (fill.lua:567). fill.checkpoint puts them at the corner + 40,40 (fill.lua:609) without testing the props it just stood.

**Proposed fix:** In fill.camp, roll a point again while it falls inside `footprints(stood, pad)`, reading a building's room walls (colliders[entry.interior]) where it has one. In fill.checkpoint, test the corner point against the footprints of the props the variant stood, and move it out if it is inside one. Or, in one place, have loader.spawn_zombie try population.clear(z.x, z.y) and nudge to the nearest clear spot (population already does this for refills).

