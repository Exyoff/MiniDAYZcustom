# 27.2 and 27.3: the map track's own account, for Progress.md

Written by the map track (work/map, built twice, reviewed, fixed). Its fix's `progress_entry` is the final text for
Progress.md's 27.2 and 27.3 (place it after 27.1 and before 27.4 in the Stage 27 section, as 27.1 was placed);
then add the merge (work/merge27: commit 27e6d3b's message, and the fixups e1599c3, 23a5165, 3dee752), the dated
notes into 27.5, the TOC row and "Where this stands". Progress 27.1 (commit 696144e) is the model to follow.

## Progress entry (fix:map.progress_entry)

### 27.2 A world five seasons wide and sixteen lots tall, built as the player reaches it

*"Increase the height"* (the generated world's, when asked) and *"make the biomes wider, like
100 across, just for testing"* (100 ground tiles, 6000 px, a season). A generated world is now
26 lots of 1200 px across -- five seasons of 100 cells and the coast's lot -- and 16 down:
31200 by 19200 px, six and a half times the lots of the 8x8 world. Built whole, as every
world was, it is 2,567 entities and 2,561 bodies at seed 7 against 598, and a START and an
autosave a phone would feel. So the map table is still generated whole at START, but a lot's
buildings, furniture, cars, loot and zombies are put down the first time the player comes
within two lots of it, and kept; and nothing a frame walks grows with what is kept. The
original streams its world too, and erases what it leaves behind; here nothing is erased.

- [x] 27.2.1 **The size from the seasons and the rows.** *"Make the biomes wider, like 100
      across."* data/ground.lua's `seasons.cells` (100) is each band's width in ground cells
      from the coast's lot line, where it was a fraction of 9600 px (1680 px a band).
      `generate.extent` works the land's width out from it, and it is the one place that does:
      five bands of 6000 px and the coast's lot, 26 lots. data/lots.lua's `rows` (16) is the
      height. The original's map is 16 locations down too (`Map_size_Y`, 17850x20000 px).
      Bands 2-5 begin at 7200, 13200, 19200 and 25200 px. The ragged wander, the road rims
      and the beach are 26.4's and 26.3's, unchanged, and so are the spawn and its home.
      `bands.cold` ramps from the green's east edge to where the snow begins, 7200 to 25200
      px, as it did from 2880 to 7920: the middle of the map, 15600 px, is 0.45 of `cold_east`,
      where the old middle, 4800, was 0.33.
- [x] 27.2.2 **A Stage 26 save is refused**, as 26.3.9 refused Stage 25's. Every world
      changed, so `generate.VERSION` is 3 (4 since 27.3's places). CONTINUE on a Stage 26 save shows "Generating
      world", builds the seed's new world, finds another one and stays on the menu with
      CONTINUE dark and "An update changed this save's world, so it cannot continue". The
      file is kept until START's first save writes over it.
- [x] 27.2.3 **Measured whole, against the 8x8 world.** Seed 7, natively with the JIT off.
      Built whole, START is 2,567 entities, 2,561 bodies and 477 zombies against 598, 597
      and 109. `app.set_map` takes 45 ms against 7-9, and the autosave is 297 KB against 95.
      The frame walked everything that grew: the update every entity, the copy-back, the
      draw, the targets and the bars over heads every zombie, `door.near` every leaf, E's
      reach every container, `interior.update` every building, `render.floors` every room.
      Two of the walks did harm only when something was added: the render grid binned every
      prop again for each spawn, and `entity.flush` took each queued spawn off the front of
      the queue, a walk of the whole queue every time.
- [x] 27.2.4 **Lots built as the player reaches them** (src/map/loader.lua). The loader
      buckets the map's objects, containers, items and zombie points by lot (`map.lots`).
      A lot is spawned the first time the player comes within `reach` (2) lots of it,
      `per_frame` (40) entries a frame, and kept. The 3x5 lots round the spawn are built at
      once, behind "Generating world". Two lots is past the zombies' sleep range (2000 px) and
      the population's spawn band (1020), so nothing a lot holds is wanted before it is
      built, and `population.update` refills no point in a lot not yet built. `map_index` is
      an entity's index in the map's own list, as before, so a container's state and the save
      are what they were. The original's "Location loader" (Game_events 3.5) creates every
      location within 1800 px of the camera once a second and erases each one past it, its
      elements' state written back to an array (docs/original_data.md). Here a lot built is
      kept: *"I want every item in the world to be permanent"* (Stage 6).
- [x] 27.2.5 **A save with lots unreached.** The save writes which lots the run reached and
      how far each got (`loader.record`). A lot never reached has nothing in the file. A
      load stands the reached lots' buildings and doors again, as far as the file got,
      before the file's own entities (`loader.restore`), and puts any lot the rebuild built
      past the file back fresh. A lot saved half-built goes on from where it stopped. A car's
      trunk is stood only for a car that stands. At seed 7's START the save is 7 KB against
      95; after a tour of a town, a base, a city and a road, 92 KB; with every lot built,
      302 KB.
- [x] 27.2.6 **Nothing a frame walks grows with the world.** The rule in
      docs/conventions.md now says so. `app.update` walks `entity.actors`, the entities of the
      kinds that act, kept by the flush beside `state.entities`. It merges in the zombies
      awake, by id, so everything still runs in the order it was spawned. Whatever walks the
      zombies for what is near the player walks `entity.acting`, the awake ones that
      `population.sleep` gathers, asking the sleepers every fifteenth frame: the copy-back,
      the draw, the targets, the bars over heads, the actions, the animation and the
      minimap's dots. One asleep wakes up to a quarter of a second late, 2000 px off.
      Containers (E's reach, the board's tabs), door leaves, buildings and rooms, and floors
      are asked by cell. The render grid bins only what a lot added. `entity.flush` drains its
      queue by index, and the save's writer counts its output and keeps the names it has
      checked.
- [x] 27.2.7 **Measured after**, natively with the JIT off, in thousands of Lua VM
      instructions a frame (the steadiest count on a shared machine):
      - The 8x8 world: 59k at the spawn, 66-75k in a town, a base, a city and on a road.
      - The 26x16 world toured: 26k at the spawn, 39-45k at the four.
      - The 26x16 world explored end to end: 41-48k at the four.

      love.graphics.draw calls a frame are 38-84 toured and 74-100 explored, against 54-106.
      Generating seed 7 takes 25-40 ms against 13-19, and `app.set_map` 2-3 ms against 7-9.
      START is 61 entities, 61 bodies and 10 zombies. Building every lot at once takes 13-32 ms.

      In the web build at 4x CPU, 844x390, touch, a frame's Lua in ms as p50/p95, arriving
      then staying (`tools/web_world.cjs`, new, beside 26.5's `tools/web_perf.cjs`):

      | | 8x8 world | 26x16, toured | 26x16, explored |
      |---|---|---|---|
      | town | 12.8/27.4, 12.8/29.7 | 11.1/37.3, 10.6/23.0 | 11.3/24.9, 11.8/23.4 |
      | base | 15.5/33.8, 13.6/35.6 | 11.9/21.8, 10.9/24.2 | 12.3/27.1, 13.9/28.9 |
      | city | 12.6/27.3, 14.4/30.0 | 11.2/26.0, 11.4/25.9 | -- |
      | road | 12.0/26.4, 10.6/20.4 | 10.7/20.6, 11.0/25.5 | 12.6/30.7, 12.7/35.1 |

      Driven east at three times a walk for 20 s, 25 lots built on the way: p50 9.5 and p95
      51.7, against 11.3 and 69.7 for the same drive across the 8x8 world. The worst frame
      of either drive is 200-233 ms; that is the ground's batches being made as the view first
      reaches them (25.2.3), and it is the same with lots or without.

      Generating at 4x: 239-310 ms against 95-199, behind "Generating world". `set_map`: 27
      ms against 65. The autosave: 12-24 ms at START against 55-85; 47-124 ms after the tour
      (111 KB) against 63-75 (97 KB); 131-163 ms with every lot built (305 KB). It saves every
      three minutes.
- [x] 27.2.8 **The minimap, laid once.** The minimap shows a window 900 px each way, so a
      world 31200 px across reads at 844x390 as the 8x8 one did. What grew is its standing
      batch: seed 7's 2,039 roads, marks and country buildings. That batch was laid again
      whenever the prop bucket changed, which with lots built on the way would have been every
      few steps. The symbols now come from the map table, so a building shows before its lot
      is built, as a map should. The batch is laid once a world, and again only for a new
      world, scale or size, or a reload of data/minimap_symbols.lua (`minimap.laid`).
- [ ] 27.2.9 **Found, and left.**
      - Over a world six and a half times the size, data/places.lua's counts are still the
        8x8 world's: two cities, a base or two, three to five towns, a gas station and a cafe.
        Seed 7's first city stands eleven lots east of the beach. More places, and roads
        joining every one, are 27.3's.
      - At 844x390 a thin sand-coloured line runs across the screen along the foot of a dirt
        track's south edge pieces. In seed 7's town it is at y=4260, under the lane at 4200.
        It is not there at 1280x720. It looks like `tilemap_sand` sampling the next row of
        its sheet at a scale that is not whole, which would make it older than this stage.
        Not looked into further.
      - With every lot built, the autosave is 131-163 ms at 4x, two to three times the 8x8
        world's. A lot can only be reached by walking to it, so that takes hours of play.
        27.3's places doubled it; 27.3.9 takes a third back.

*Verified by capture, `--world 7` at 1280x720 and 844x390:*
- *the spawn on the beach, at (630, 11068) of 31200x19200, with 61 entities standing;*
- *a town in the brown, 40 of 416 lots built;*
- *the snow's edge at 25290 px with the church of a city in it;*
- *a city in the frosted band, lvl3's icy asphalt, its blocks on the minimap.*

*Scenarios: six new ones:*
- *the world's size from the cells and the rows, every band 100 cells over five seeds, and
  the cold ramp;*
- *a far lot empty until the player comes within two lots, then whole and nothing twice;*
- *a save with lots unbuilt and one half built, loading as it was and building the rest when
  reached;*
- *a Stage 26 save refused with seed 7's real Stage 26 fingerprint, 443543276;*
- *the minimap laid once while a walk across the world builds hundreds of props;*
- *the perf budgets at the spawn, in a city, and in a city with every lot built.*

*Changed:*
- *every generated-world scenario that asks about every building, door, car or zombie
  builds the world whole first (`BUILD_ALL`);*
- *the cold's x positions and the seasons' at six positions over 20 seeds, and the
  boundary's 320 rows;*
- *road sites 12-30 a world;*
- *seed 7's doors 36 and 108 hits;*
- *the car-trunk scenario on seed 14, which parks both kinds;*
- *the soak's tour across the wider world.*

*Run by name: 110 scenarios -- every generated-world one in world, save, hud, items, play and
settings, and the ones the frame's walks reach in controls, board, phone, combat, render and
tooling (doors, rooms, E's reach, NEARBY's tabs, the collector's perf, the soak) -- 108 passed,
and the two that failed pass now: the perf scenario read `state.rooms`, which nothing gathers
since the rooms are asked by cell, and the minimap one read its first numbers off a state its
last step had rebuilt. Lint 12/12. Not run: the full suite.*

### 27.3 More places, and roads that join every one of them

*"More interest points with roads connecting them"*, and asked what *"make the map grid
uniform"* meant: *"roads connect every interest point to each other"*. On the 26x16 world
27.2 left eleven places and most of the land without a road: the network was laid first, two
highways edge to edge with mains and branches, and the places stood at its junctions. Now
the places come first, planned on whole lots to the original's own quota scaled to the land,
and the network is laid to join every one of them on the seams between the lots.

- [x] 27.3.1 **The original's quota, scaled.** Read off `generate_array_locations` (Game_events
      3.8) and checked in a live run of the original: its first map holds 20 villages, 3
      cities, 2 military bases, a hospital and a fire station of their own, 4 gas stations and
      4 roadside cafes on its roads, one secret location, a crashed humvee (3.14) and a
      checkpoint on every crossing (3.16, location 14). `data/places.lua` had read the globals'
      initial values, 9-5-3-2-2-1-1, which that function overwrites (docs/original_data.md).
      The land is 2.2 times the original's map and a place here is bigger than a location, so:
      12-16 towns, 3 cities, 3 bases, 2 hospitals, 2 fire stations, 8-9 gas stations, 8-9
      cafes, 2 camps, 2 crashes, and checkpoints on up to 4 crossings. Over 150 seeds (6653
      apart, `app.fresh_seed`'s range) a world has 43-52 places, against 11; each kind's
      places are spread a season at a time, and none stands on the coast.
- [x] 27.3.2 **The kinds it had that we lacked**, each from the original's own layout: the
      HOSPITAL with its car park and the FIRE STATION with two houses, a shed and a pump,
      places of their own beside a big road (a city no longer holds them); a CAMP, the secret
      location -- one of its nineteen with art in the rip, from hunters' camp to castle, at the
      original's positions in a ring of twelve trees, each with the crate the original leaves
      (`data/maps/prefabs/camps.lua`); a CRASH, the humvee on its nose (`hammer_crash`, cut
      from the original's sheet into `Assets/Sprites`, frame 1 as it stands there) with its
      crew's kit; a CHECKPOINT, one of its five, round the corners of a crossing. A town is one
      of the original's four villages: one in four has a school and one in four a church (the
      city church of 25.2 goes).
- [x] 27.3.3 **The places first, then the roads that join them** (`src/map/places.lua`,
      `src/map/links.lua`). A city round a lot corner, its avenues the two lot lines through
      it; a base's two lots and the lot of line out of its gate; a town's lot or two along a
      road of its own; a camp's lot. Then the network on the lattice of lot corners, so every
      road is on a seam every prefab keeps clear: a highway from the first road down to the
      beach through every city, the cheapest next each time; every other place by the cheapest
      way onto what is down, in its own class (Prim's tree on the cost of a route: a lot of
      road already laid costs a quarter of a new one, a turn 0.6 more); then up to six loops
      between places a few lots apart that the tree joins the long way round. Each line's road
      is one run of the biggest class among its lots, so no two roads meet end to end; a route
      turns where it must, and at a bend each road runs on to the other's far edge so its
      outside corner is square and the blob's piece rounds it. A base's wall either side of
      its gate is shut to routes, so its road comes in straight. Then each place lays its own
      streets against the network, and the kinds the original puts on a road take a lot
      beside one, the checkpoints a crossing.
- [x] 27.3.4 **Two roads down to the beach**: a main road and a track whose sand runs on into
      the beach's; the spawn and its farmhouse beside whichever passes nearer a town.
- [x] 27.3.5 **The minimap in the original's symbols** (`generate_minimap`, 3.17): a base 27, a
      gas station 16 or 17 and a cafe 19 or 20 by its road's way (16 and 17 had been read as a
      gate in a wall and drew one by every base), the hospital 8, the fire station 9; a city one
      symbol a lot, as the original marks one a location.
- [x] 27.3.6 **A drop is drawn only while it is in view.** `item.draw_all` drew every item in
      the world every frame, a draw call each: in seed 7's city explored end to end 130 calls a
      frame against 25.2's budget of 140. Past the camera's view, padded by the widest ground
      icon, a drop is skipped: 53 there, 50 toured, 35 at the spawn.
- [x] 27.3.7 **A seed is one world on the desktop and in the browser** (the review). The heap
      the routes are found with broke a tie in cost by the order the states went on, and the
      sources went on in `pairs()` order over a set of corner ids, which LuaJIT and PUC Lua 5.1
      walk differently: seeds 4, 7, 15, 20, 35 and 37 of 1-40 built one world natively and
      another in the web build, seed 7 among them, so every scenario and capture that named
      seed 7 checked a world the published game would never build. A tie is broken by the
      state's number now. Seeds 1-40 hash the same natively and in the web export (roads,
      places, the save's fingerprint); natively, seed 7 is now the world the web build made.
      docs/conventions.md: nothing a generator makes may follow a table's `pairs()` order.
- [x] 27.3.8 **Building a world, a phone's wait cut by more than half** (the review: 27.3 had
      made `generate.build(7)` 1,582-1,721 ms in the web build at 4x, against 134-161 for the
      8x8 world, and the README said a phone started it as quickly). Measured in the web build
      with a time-weighted sample, and every change checked to build the same worlds: seeds
      1-40 hash the same before and after, roads, places, fingerprint and every tile laid.
      - `links.join`, 600 ms of it: the tree keeps its costs from one place to the next and
        spreads only from the corners new to it (one spread over the lattice, not one a place),
        `open` and `seg_of` written out in its loop; a loop's pairs are walked by road once, in
        rings rather than through the heap, and then only the pair that looks best; a loop's
        route stops at its goal. About 85 ms now.
      - `roads.assemble` and `rasterize` ask a point or a road's end only the roads of its own
        row and column (`lines_of`), not all three hundred; `rasterize` lays only the cells it
        painted, chooses a piece off the raster's own index, asks a column's band once and
        counts its lists.
      - `places`: a way to stand is put in its season once, not once a place, and a place's
        ground in lots kept; `fill` asks a sprite's size of the atlas once.
      Seed 7 natively with the JIT off: 129 ms to 56. In the web build at 4x
      (`tools/web_world.cjs`): 619-727 ms, behind "Generating world"; the README says so.
- [x] 27.3.9 **The autosave of a world explored end to end** (the review): 3,372 entities, a
      454 KB file, 266-351 ms at 4x every three minutes. The writer sorted a record's keys with
      a Lua comparison where all of them are names, put every scalar through another call, made
      a key's indent, name and " = " three pieces and a string's `%q` again each time. The same
      456,050 bytes for seed 7 explored and for a table of every odd case; natively 18 ms to
      10.5; at 4x 198-273 ms. What is left is the VM's own pace, about sixteen times the
      desktop interpreter's, and a float's text through sprintf.
- [x] 27.3.10 **A death wakes nobody** (the review). `population.sleep` put every zombie past
      sleep range of the LIVING player to sleep, so a death woke all of them: in seed 7
      explored, 571 zombies thinking and wandering unbuilt lots through the death screen, the
      frame 1.86 ms to 5.02 at the median natively with the JIT off. They sleep by where he
      lies: 3 awake, 1.86 ms.
- [x] 27.3.11 **A checkpoint in every world** (the review). Every crossing of two asphalt roads
      can be a city's street or by a town: five seeds of 150 had none. Where no crossing or tee
      takes one, the first stands on a straight stretch of the network's asphalt, two lots
      from its ends and the map's edges, and a track is laid a lot either side across the road
      through it, settled into the network like a place's street. A world that had one is
      untouched (seeds 1-40 hash the same).
- [x] 27.3.12 **The crashed humvee lies in the middle of its road** (the review). The original's
      `hammercrash_point` is a location's middle (3.14.8), on a road location the middle of the
      road, the wreck on the asphalt among the cars. Here it stood mid-lot, 500 px off the road,
      and a player on the road could pass it unseen. A prefab entry may say `on_road`: it stands
      off where the place joins its road, `x` along it and `y` across it away from the lot,
      whichever side of a north-south road the lot is (`fill.stamp_road`). The wreck is on the
      asphalt; its crate and its soldier at the road's edge on the lot's side, inside the
      place's ground. "Nothing stands on a road" holds for everything else.
- [ ] 27.3.13 **Found, and left.**
      - The hospital and the fire station stand on bare ground. The original's
        `create_map_location` lays asphalt tile 15 for the hospital's car park (about 7x3 +
        3x12 + 12x6 tiles) and the fire station's driveways, and a paved strip to a road
        location beside either (Game_events 3.5.5.1.24.7, 27.7). Here that wants a paved
        surface the road raster paints from a place (a pad in `net`, laid before `rasterize`),
        and the "nothing on a road" rule taught that a car park's cars stand on it.
      - The checkpoints keep their crossing clear. The original's (location 14) stands an APC
        in the middle of it, concrete blocks on the carriageway, a watchtower and chain-link
        fences at right angles at each corner, and a car on the road; the remake puts every
        prop in the corners. It wants each of the five variants read off 3.16.1.39's arrays
        with the original's own coordinates.
      - The gas stations and cafes are each in four of the five seasons at least, not all
        five: in the far east the big roads are often only the ones its towns lie along.
      - The autosave of a world explored end to end is still 200-270 ms at a phone's pace.
      - Merging with the integration branch wants hands, not hunks (the review): its
        `generate.VERSION` is 3 for 27.1's rooms, so the merge keeps 4 and rewrites the history
        comment; and Stage 28's animals (published) were spawned only on the whole-map path
        this branch replaced -- the lot loader must bucket and stand `map.animals`,
        `population.sleep` must put animals to sleep by 27.3.10's rule, an animal must not be
        an actor walked every frame wherever it stands, and `refill_animals` must refill no
        point in a lot not yet built, as the zombies' refill does.

*Verified by capture: seed 7's whole network in the minimap's own symbols, and seeds 3 and 21;
a hospital, a fire station, a crash, two camps and a checkpoint at 1280x720, a hospital and a
checkpoint at 844x390, each beside the original's own (its map page, its hospital car park,
its fire station, a crossing with sandbags). After the review: seeds 66530 and 252814's
checkpoints on a straight stretch, a track across a highway and across a main road, at
1280x720 and 252814's at 844x390; seed 7's two
crashes on their roads, a main road running north-south at 1280x720 and a highway at
844x390.*

*Measured after the review, seed 7: 49 places, 123 roads, 306 nodes and 6 loops, every place
reached. Seeds 1-40 hash the same in the web export as natively (roads, places, fingerprint).
In the web build at 4x (`tools/web_world.cjs --all`): `generate.build` 619-727 ms against
1,582-1,721; `set_map` 43; START 89 entities; the frame's Lua p50 8.5-11.8 ms toured and
11.5-14.9 explored, against 27.2's 10.6-11.4 and 11.3-13.9; the autosave 12-24 ms at START, 65-89 after the tour (145 KB),
198-273 explored (452 KB) against 266-351.*

*Scenarios: 27.3's new one, over twenty seeds every place reaching every other by road and the
spawn's road with them; changed with the quota, each kind's count in its range, a town in four
with its school and one in four with its church, the hospital and the fire station places of
their own, every place's buildings its kind's, loot and points per place of each new kind, the
minimap's symbols, and the counts fifty places moved (doors 108, buildings with no room 157,
bars 9, the door walk's flood to ten thousand cells for the house behind a fire station); the
perf scenario holds a frame's draws under 100. After the review, two new: a death in an
explored world wakes no zombie past sleep range (fails on the old rule), and the five seeds
whose every asphalt crossing was taken each with a checkpoint on a track across a road; one
changed: nothing stands on a road but a crash, whose two wrecks are on one. Run by name after
the review: the 68 scenarios that build a generated world -- 67 passed, and the global-rng one
needs its partner run first, 2/2 with it -- then the 16 the crash reaches, and lint 12/12. Not
run: the full suite.*

Choices, each reversed by one change:
- Where no crossing is free, the checkpoint's road across is a dirt track. Reverse: `across`
  in `data/places.lua`'s checkpoint (`"street"` for asphalt, or remove it for a world with
  none).
- The crash's crate and soldier stand at the road's edge, not on the carriageway. Reverse:
  their `y` in `data/maps/prefabs/crash.lua` (positive is across the road).

## Dated notes on earlier items (fix:map.dated_notes)

2026-10-05 (map track, after the review). All seven commits are on work/map; nothing was pushed and Progress.md was not edited.

Notes for whoever merges this into claude/minioutbreak-exploration-jhs27h:
- generate.VERSION is 2 on origin/main (published, with fauna), 3 on the integration branch (27.1's rooms, not yet published) and 4 here. Keep 4, and rewrite the history comment: 1 Stages 21-25, 2 the coast, 3 the rooms, 4 the wide seasons, the tall grid and the places-first network.
- Fauna only spawns map.animals in loader.load's whole-map path (integration loader.lua:66-70). This branch's lot path must bucket 'animals' entries and stand them through entity.spawn('animal', {species, spawn_id}).
- generate's wildlife.build(map, layout, size, grid, wild) needs this branch's deal() layout.
- Integration's population.sleep has an animal block; port it using this branch's `here` rule (asleep by distance from the player, living or dead), not `alive`.
- 'animal' is in kinds.moves but is neither still nor the sleeper kind, so it would become an entity.actor walked every frame. Gather the awake animals the way zombies are gathered, or make kinds.sleeper a list.
- refill_animals needs the same loader.built gate that the zombies' refill has.
- The 40-seed native-vs-web hash check in this track's notes should be repeated after the merge, since fauna's wildlife stream is new.

Measurement tools left in the scratchpad (env/map/fx), not committed:
- webprof.cjs: per-stage web timing and the 40-seed web hash.
- websave.cjs: the autosave timed in parts.
- time.lua: native timing plus a 40-seed extended hash that includes the tiles.
- hot.lua: a native sampling profile.

The time-weighted web sampler worked by putting a temporary game/zz_hot.lua into the build. It was deleted before every commit and never committed.

## Dated notes from the builds

- **26.3.2** (2026-10-03): "The world stays 9600 px square, so a phone builds what it did" no longer holds. Since 27.2 the world is 31200x19200 px (26x16 lots), and a phone builds only the lots round the player. The coast is still one column of lots.
- **26.3.9** (2026-10-03): generate.VERSION is 3 since 27.2, which refuses a Stage 26 save the same way.
- **26.3.10** (2026-10-03): "Seed 7 at the spawn is 598 entities" is now 61 entities at START, with lots built as they are reached. Built whole the world is 2,567.
- **26.4** (2026-10-03): the bands are 100 ground cells (6000 px) each, not 1680 px each at fractions of the width. They begin at 7200, 13200, 19200 and 25200 px.
- **26.4.1** (2026-10-03): bands.cold ramps from 7200 to 25200 px, not from 2880 to 7920. The map's middle is 15600 px, where the cold is 0.45 of cold_east; the old middle (4800) was 0.325.
- **25.2.2** (2026-10-03): "interior.update the buildings, gathered once" is still true, and since 27.2 they are also binned by cell, so a frame asks only those near the player. render.floors now reads the static grid instead of walking every room.

- **21.3.1** (2026-10-05): "straight roads meeting at right angles... no bend" no longer holds for the network: a route that must turn now makes a bend (27.3.5). It is counted, and the minimap draws it as two straights running into the corner. No two roads still meet end to end.
- **21.3.10** (2026-10-05): minimap_tile 16, 17, 19 and 20 are a gas station (a fuel can on a road) and a bar (a beer mug on a road), per the original's generate_minimap, not bridges, gates or overpasses.
- **21.4.1** (2026-10-05): the hospital and the fire station are no longer inside cities but places of their own beside a big road, two of each. Gas stations and cafes are 8-9 each, not one.
- **21.4.5** (2026-10-05): a base is marked with the outpost and its flag (27), not a base symbol plus a gate in its wall (16/17). A city is marked one symbol a lot, not one a block.
- **25.2.8** (2026-10-05): the one church a world in a city block is gone. Churches stand in towns, one town in four, as the original's villages have them (27.3.3).
- **26.4.5** (2026-10-05): seed 7's boundary draws are now 7 on the boundary, 4 mid-band and 6 at the spawn, since no road is in view at those spots once the places come first.

## The review's verdict

Track map (27.2 + 27.3, b180a9d..d7df9e1) works but is not ready to merge: two majors. I made no edits or commits; the worktree is clean at d7df9e1.

What holds up:
- All 37 scenarios the builder added or changed pass (17m23s), and lint passes 12/12.
- The test map is pixel-identical to b180a9d after walking, at 1280x720 and at 844x390 with touch.
- Lots never pop in. I drove the player along seed 7's longest road at 300 and 900 px/s. Every spawn the loader made was at least 2485 px away, and none was in or near the view.
- Lot-build frames cost 2.7 ms at p50 natively with the JIT off, against 1.8 ms for other frames. In the web build at 4x, the 300 px/s drive gave max 97 ms against 109 for the 8x8 base. Garbage on frames with no lot build is 2.7 KB/frame against 2.46 on the base.
- Save and load of a toured world (93 lots reached) gives the same counts back: 861 entities, kind for kind, and the same lots.
- Roads, my own flood fill of the road cells over 200 seeds:
  - the whole network is one connected piece, and every place's ground touches it;
  - no two places overlap and none stands on the coast;
  - the only road on the sand is the intended track to the beach;
  - the farmhouse always exists, at most 1134 px from the spawn, and the spawn is always on sand.
- Built whole in 8 seeds, no body of any prop or container and no item stands on a road.
- Every sprite and frame named by camps, the crash and the checkpoints exists in the atlas.
- The cold ramp is a smoothstep from the green's east edge (7200 px) to where the snow begins (25200 px); mid-map gives 0.45, matching config.lua.
- At 4x the frames are no worse than the 8x8 base:

| Place | HEAD, toured (ms) | Base 8x8 (ms) | HEAD, every lot built (ms) |
|---|---|---|---|
| Town | 10.2 / 17.6 | 12.1 / 20.1 | 12.7 / 20.1 |
| Base | 11.8 / 19.0 | 12.8 / 22.4 | 12.8 / 19.7 |
| City | 10.4 / 18.3 | 12.0 / 23.1 | not measured |
| Road | 8.8 / 15.0 | 11.5 / 21.7 | 11.1 / 19.1 |

  Each cell is the frame's median and 95th percentile (p50 / p95). START has 89 entities against 598 on the base.

The two majors are both in 27.3, and neither was measured after it:
- The same seed now builds a different world in the web build than on the desktop. This is seed 7 among others, the seed every scenario and capture uses.
- Generating a world in the web build at 4x went from about 0.15 s to about 1.65 s.

Against the original, I ran it and took screenshots (/tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad/env/map/rv/orig/o_*.png):
- The hospital and fire station grounds are not paved, as the original's are.
- The crash is not on its road.
- The checkpoint does not block its crossing.
No ask from the user covers these differences.

## The review's findings

- {"severity": "major", "title": "A seed builds a different world in the web build than natively (6 of seeds 1-40, seed 7 among them)", "where": "/home/user/wt/map/game/src/map/links.lua:136 (spread: `for id in pairs(from) do ... push(heap, 0, s)`)", "detail": "Heap ties are broken by insertion order, and the sources are pushed in pairs() order over the integer-keyed set `joined`. LuaJIT on the desktop and PUC Lua 5.1 in love.js iterate that set in different orders, so equal-cost routes resolve differently.\n\nRepro, roads.hash : places.hash : save.fingerprint of generate.build(s):\n- Seed 7 natively: 1633887802.5:245526930:64885673.\n- Seed 7 in the web export (rv/web_HEAD/dist/web via rv/webhash40.cjs): 240125334.5:694587042:2118016364.\n- Over seeds 1-40, HEAD differs between web and native at 4, 7, 15, 20, 35 and 37. The base b180a9d differs at none.\n\nProof of cause: in a scratch copy I sorted the sources before pushing.\n- Ascending order reproduces the web's seed-7 world exactly.\n- Descending order reproduces the native world exactly.\n- Seeds 15, 20, 35 and 37 also change with the order.\n\nConsequences:\n- Every scenario and capture that names seed 7 checks a world the published game never generates for that seed. This includes 'city,379', the lots and perf brackets, and the fingerprint 443543276 used by the Stage 26 save scenario.\n- tools/web_world.cjs measures a different seed-7 world than the native captures.\n- A seed is not one world across the two runtimes the project targets.\n\nFix: iterate the sorted ids in spread, or break heap ties by state index."}
- {"severity": "major", "title": "27.3 made generation ~10x slower in the web build at 4x, and the README's phone claim is now false", "where": "/home/user/wt/map/game/src/map/places.lua (places.build), /home/user/wt/map/game/src/map/links.lua (links.join); README.md:135-136", "detail": "tools/web_world.cjs at 4x CPU, same machine, run back to back:\n- HEAD: generate.build(7) takes 1582, 1659 and 1721 ms; set_map 55 ms.\n- Base b180a9d: 161, 135 and 134 ms; set_map 90 ms.\n- The builder's own 27.2 measurement was 239-310 ms, and nothing was re-measured after 27.3.\n\nNatively with the JIT off, in my probe rv/probe5.lua:\n- Seed 7 takes 129 ms (places.build 106, of which links.join 49; roads.rasterize 25; fill.build 15). Seed 8 takes 94 ms.\n- The base takes 35 ms and 15 ms for the same seeds.\n- With the JIT on, 50 seeds average 96 ms, against 27.2's '25-40 ms'.\n\nSTART, RESTART and a cold CONTINUE each pay this behind 'Generating world'. That is about 1.7 s on a phone-paced CPU, against about 0.25 s before.\n\nREADME.md:135 says 'a phone starts a world this size as quickly as it did one of 8x8 lots'. That is no longer true."}
- {"severity": "minor", "title": "With every lot built, the autosave freezes ~0.3 s at 4x", "where": "/home/user/wt/map/game/src/game/save.lua (save.write), every 180 s (data/config.lua autosave_seconds)", "detail": "tools/web_world.cjs --all, at 4x:\n- At START: 22, 16 and 12 ms (14 KB).\n- After the tour (65 lots): 76, 73 and 72 ms (146 KB).\n- With every lot built (3372 entities): 351, 282 and 266 ms (450 KB).\n- The 8x8 base: 72, 35 and 63 ms at START and 94, 69 and 45 ms after its tour (97 KB).\n\nThe builder's 27.2 figure for this case was '131-163 ms'; 27.3's extra places doubled it. In a much-explored run, that is a third of a second's stall every three minutes on a phone. No numbers for this were reported after 27.3."}
- {"severity": "minor", "title": "~3% of seeds have no checkpoint, and some have 43 places", "where": "/home/user/wt/map/game/data/places.lua:448 (checkpoint count {1,4}, 'every seed of twenty one at least'); places.lua crossings()", "detail": "Probe rv/probe4.lua over 150 seeds (6653*i, the range of app.fresh_seed's love.math.random(1,999999)):\n- No checkpoint at all in seeds 66530, 252814, 419139, 538893 and 565505, though each has 56-63 'cross' nodes.\n- Only 43 places in seeds 86489 and 565505.\n\nThe builder's account says 'every kind is in every world' and '44 to 50 places'. The 20-seed scenario passes only because seeds 1-20 happen to avoid both cases."}
- {"severity": "minor", "title": "Death wakes every zombie in an explored world (571), a ~2.7x frame cost on the death screen", "where": "/home/user/wt/map/game/src/game/systems/population.lua:179 (no living player means asleep = false for all)", "detail": "Repro, rv/probe6.lua, seed 7 with every lot built, natively with the JIT off:\n1. Alive: p50 1.86 ms, p95 2.80 ms.\n2. Kill the player with combat.hit_player(state, 100000).\n3. The next 120 frames: p50 5.02 ms, p95 7.13 ms, max 9.52 ms, with all 571 zombies awake and thinking.\n\nThe logic is from 25.2, but it scales with the world, which is now 4.7 times as many zombies as the 8x8 one. They also wander, with no player, across lots that are not built. Keeping the sleep state while the player is dead avoids all of this."}
- {"severity": "minor", "title": "vs original: the hospital and fire station stand on bare ground; the original paves them", "where": "/home/user/wt/map/game/data/maps/prefabs/hospital.lua, fire_station.lua", "detail": "The original's create_map_location for location 11 (hospital, Game_events 3.5.5.1.24.7) and 15 (fire station, 3.5.5.1.27.7) lays asphalt tile 15 on the location's ground tilemap:\n- for the hospital, a car park of about 7x3 + 3x12 + 12x6 tiles;\n- for the fire station, driveways of 5x2, 15x1, 1x7, 8x1 and 3x7;\n- for both, a paved strip of 2x3 or 9x2 to any neighbouring road location (12, 13 or 14).\n\nIn my run of the original, rv/orig/o_hosp_c.png shows the hospital in a large paved car park with the cars in rows south of it. The remake parks its cars on grass or dirt either side of the hospital (caps/w7c_hospital_1.png, caps/r_ph_hospital.png). No user ask covers this."}
- {"severity": "minor", "title": "vs original: the crashed humvee lies on a road in the original; here it is mid-lot, ~600 px from any road", "where": "/home/user/wt/map/game/data/maps/prefabs/crash.lua (hammer_crash at 600,640 of the lot)", "detail": "Game_events 3.14.8 puts hammercrash_point only on locations of type 13 (a road location), 15 or 5, at +500,+600: the middle of the road. In my run, rv/orig/o_crash.png shows the wreck on the asphalt among wrecked cars.\n\nThe remake's wreck stands in the middle of a lot. The road on the lot seam is about 500 px away, at the screen's edge in caps/w7c_crash_1.png and caps/r_ph_crash.png, so a player on the road can pass it unseen.\n\n'Nothing stands on a road' is the builder's own rule, not a user ask."}
- {"severity": "minor", "title": "vs original: the checkpoint keeps its crossing clear; the original's blocks it", "where": "/home/user/wt/map/game/data/places.lua checkpoint.corners (comment: 'the original stood its blocks and its APC across the road')", "detail": "In my run, rv/orig/o_check_c.png (location 14) shows:\n- an APC in the middle of the crossing;\n- concrete blocks on the carriageway;\n- a watchtower;\n- chain-link fences with posts at right angles at each corner;\n- a car parked on the road.\n\nThe remake puts every prop off the asphalt: a few fence panels along the verges, with the APC and the car in the corner quarters (caps/r_ph_checkpoint.png). The divergence is acknowledged in a comment but traces to no user ask."}
- {"severity": "minor", "title": "Merge hazards with the integration branch (claude/minioutbreak-exploration-jhs27h)", "where": "/home/user/wt/map/game/src/map/generate.lua:84; game/src/map/loader.lua (bucket); game/src/game/systems/population.lua (sleep); game/src/game/entity.lua (actors)", "detail": "(a) VERSION collision. The integration branch already has generate.VERSION = 3 'for the rooms' (ee9394d). This branch's history comment says 3 is the wide seasons and 4 the places. The merge must keep at least 4 and rewrite that history comment.\n\n(b) Fauna (Stage 28, already published), all three parts of which this branch rewrote:\n- On integration, map.animals is spawned only in loader.load's whole-map path (integration loader.lua:66-70). This branch's lot path buckets only objects, containers, items and zombies, so after a naive merge no animal spawns in a generated world.\n- Integration's population.sleep has an animal block, which this rewrite drops.\n- 'animal' is in kinds.moves and is neither still nor the sleeper kind, so every animal becomes an actor walked each frame wherever it stands.\n\nrefill_animals will also need the same loader.built gate as the zombies' refill."}
- {"severity": "nit", "title": "Docs give 27.2's explored-world numbers, which 27.3 changed", "where": "/home/user/wt/map/docs/architecture.md:347, /home/user/wt/map/docs/conventions.md:135-136", "detail": "Both say a world explored end to end is '2,500 entities and 477 zombies'. Seed 7 with every lot built is now 3372 entities and 571 zombies (web_world --all and rv/probe6.lua). Over 8 seeds built whole it is 3325-3481 entities and 554-575 zombies."}

## What the fix fixed

- major: a seed built a different world in the web build than natively (seeds 4, 7, 15, 20, 35, 37 of 1-40). The heap in game/src/map/links.lua now orders by cost, then by state number, so the pop order no longer depends on pairs() over the sources. Seeds 1-40 hash the same in the web export and natively at HEAD (checked twice, the second time after every other fix). docs/conventions.md gained the rule. Commit f34e1ae.
- major: 27.3 made generation about 10x slower in the web build. It was measured there with a time-weighted sampler and fixed in links (incremental Dijkstra for the tree, lazy-greedy loops with a BFS, early exit), roads (row and column buckets in assemble and in rasterize's per-end checks, laying only painted cells, choosing pieces from the raster index, band per column, counted lists), places (band cached per plan, no per-place list, lot zones and grid cached) and fill (sprite sizes cached, a running total of weights). generate.build(7) at 4x: 1,582-1,721 ms down to 619-727. Natively with the JIT off: 129 ms down to 56. Seeds 1-40 give identical roads, places, fingerprint and every tile before and after. README now states the measured cost (commits f34e1ae, c894111, feeb859).
- minor: the autosave froze about 0.3 s with every lot built. game/src/core/serialize.lua now sorts single-type keys with the C comparator, writes scalars inline, caches key heads, indents and quoted strings. The output is byte-identical (456,050 bytes, plus an odd-case table). Natively 18 ms down to 10.5; at 4x 266-351 ms down to 198-273 (commit 9af3b90). Partly fixed: see not_fixed.
- minor: about 3% of seeds had no checkpoint. Where no crossing or tee is free, the first checkpoint now stands on a straight stretch of network asphalt, with a track (data/places.lua `across` = dirt) laid across it and settled into the network. Over 150 seeds every world now has one. Seeds 1-40 are untouched (commit 379c56c). The place count is 43-52 over 150 seeds; the account's '44 to 50' was wrong, and the progress entry and docs now say 43-52.
- minor: death woke every zombie in an explored world. population.sleep now measures from the player's position whether he is alive or dead. After a death 3 are awake instead of 571, and the frame is 1.86 ms against 5.02 (commit 3aea02f).
- minor vs original: the crashed humvee was mid-lot, about 600 px off the road. Prefab entries can now say `on_road` and are placed off the place's road point (fill.stamp_road). The wreck sits on the asphalt; its crate and soldier are at the road's edge, inside the place's ground (commit 2f97966).

## What the fix left

- minor vs original: the hospital and fire station stand on bare ground; the original paves a car park and driveways (asphalt tile 15). Not cheap. It needs a paved 'pad' surface that places.build registers on the network before rasterize, and the 'nothing on a road' rule taught that car-park cars stand on it. Recorded as 27.3.13.
- minor vs original: the checkpoint keeps its crossing clear, where the original blocks it with an APC, concrete blocks, a watchtower and a car on the road. Not cheap. Each of the five variants has to be transcribed from Game_events 3.16.1.39's element arrays (class/kind/x/y) with the original's own coordinates, and the props exempted from the road rule. Recorded as 27.3.13.
- minor (partly): the autosave of a world explored end to end is still 200-270 ms at 4x. What remains is the web VM's pace (about 16x the desktop interpreter) and float formatting through sprintf. GC is not the cause; measured with it stopped: 186-205 ms. A further cut needs a different design: encoding across frames from a snapshot, or not saving pristine entities.
- minor: merge hazards with claude/minioutbreak-exploration-jhs27h. They cannot be fixed on this branch, because the fauna and rooms code is not here; they are written up for the merger in the notes and in 27.3.13.

## The merge's running notes (work/merge27)

# merge27 notes (work/map into integration cf6154c), running
State: git merge work/map in progress in /home/user/wt/merge27. Do not abort.
Per-file resolution (status: reviewed? added?):
- animation.lua: prev agent's resolution OK (theirs' entity.acting + ours' beast.asleep check). reviewed.
- targeting.lua: OK (SHOOTABLE loop over entity.acting). reviewed.
- interior.lua: OK (theirs' cells + ours' update_room/ride/more_doors; `was` room gets update_room). reviewed.
- loader.lua: prev agent did it: loader.place(state, object, index, props_only) is the one builder (frame_key, more_doors), spawn_object calls it; "a" animals bucketed and spawn_animal in whole path. TODO: drop duplicate `local NONE`.
- PLAN population.lua: sleep generalised to zombie+animal (state.acting.animal), `here` rule; refill_animals gated by loader.built.
- PLAN kinds.sleeper -> set (zombie, animal); entity.acts; app.update 3-way merge by id.
- PLAN generate.lua: VERSION 4 (integration head's VERSION-3 save is an 8x8 world), history rewritten; bounds w,h + lots + animals={}; wildlife.build(map, layout, size, cols, rows, wild).
- PLAN ai.animal_in_sight + fauna nearest_* -> entity.acting.
- DONE population.lua (settle() per kind; changed() per kind; refill_animals gated by loader.built).
- DONE kinds.sleepers/sleeps, entity.acts, app.update 3-way merge; ai.animal_in_sight + fauna nearest_* on entity.acting.
- DONE generate.lua VERSION 4; wildlife.build(map, layout, size, cols, rows, gen) with objects bucketed by lot (same result).
- DONE save.py VERSION 4 expectations; world.py wildlife.build call takes 3,3.
- DONE loader.lua duplicate NONE removed. README/places.lua/minimap.lua/hud.py/render.lua reviewed OK.
- NEXT: parse check, git add, merge commit; then scenarios.
- MERGE COMMITTED: 27e6d3b. Next: smoke, build atlas/icons, scenarios (runner copy run_named_m27.py), fixups.
- world.py door scenario had @@N@@/@@R@@ placeholders from prev agent: measured 255 leaves, 765 hits; filled.
- test map pixel-compared vs cf6154c (spawn after 400 frames with zombies, gas station, inside red house, NE animals): world identical; only the real-time-pulsed map tip ring differs.
- fill door-walk: k=0 passes; door-count bracket 25-160 stale (255 leaves w/ rooms) -> 150-400 (scenario asked an old question)
- fill loot per place: hospital 3-3->4-4, fire station 3-6->7-9, camp/crash/checkpoint 1-6,1-1,5-10 -> 1-4,1-1,3-6 (27.1 rooms' loot points); bad-table counts 0 (old question)
- fixup commit 1: e1599c3 scenarios+docs
- hud map page scenario rewritten for 26x16 (1.33 window; test map fitted 2.00) -- verify
- web export (dist/web, built from 27e6d3b game files) vs native: seeds 1-40 roads:places:fingerprint:animals-hash:count identical (hash_native.txt, hash_web.txt in env/merge27)
- PENDING (uncommitted): hud.py map page rewrite; new world.py scenario "lots: a world's animals are its built lots'..." (ANIMAL_LOTS helper); loader.lua header comment. To apply after batches: m27_roomover.py (interior.room_over by cells). Then rerun: doors 255, fill door walk, fill loot, map page, new lots/animals scenario, fire/build scenarios.
- captures: env/merge27/caps/m27_town.png, m27_wild.png, m27_map.png (1280x720), m27_town_phone.png, m27_map_phone.png (844x390)
- commits: 3dee752 23a5165  room_over + scenarios
- FINAL: batches A 63/65 + B 60/62 (4 fails, all stale scenario numbers, fixed and rerun 7/7 incl. new lots/animals scenario after a frames bump and carcass-aware count); build-mode reruns 4/4; lint 12/12 (twice). Thin sand line at y=625 in m27_town.png is pre-existing (same pixels on work/map 2f97966).
- Commits: 27e6d3b merge, e1599c3 scenarios+docs, 23a5165 room_over by cell, 3dee752 scenarios (map page, animals by lot).
