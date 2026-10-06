# world: the remake against the official 1.0

*Area surveyed:* The world: how the map is built (its layout, the location grid, what decides each location, procedural or fixed, its size), its places (villages, cities, military bases, the hospital, the fire station, gas stations, roadside cafes, the secret location, the humvee crash, checkpoints, the bunker, heli crashes, helipads), buildings and what stands in and around them (facades, rooms, entering, doors, furniture, fences, props, trees, vehicles, ground clutter), roads, ground tiles and seasons, water and the coast, the map's edges, the camera's scale outside and inside, lighting and shadows, the map screen's page and the corner minimap.

Surveyed 2026-10-06 against the remake at 75d9a88 (Stages 27.1 to 28 merged) in the read-only worktree `wt/survey`. Progress.md's 27.3.13 text is in the main checkout's working copy, not yet committed.

Shots and helpers are under `S = C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/world`:
- `S/orig/*.png`: the original, driven by `../orig.cjs` (headless Edge, 1280x720, `--touch`, Novice, noon). The JS helpers are:
  - `S/grid.js`: the 16x16 location grid, live;
  - `S/census.js`: location kinds counted;
  - `S/tpk.js`: teleport to a location of a kind;
  - `S/tpin.js`: into the nearest warmzone;
  - `S/tptree.js`, `S/tpface.js`: beside a tree, behind a facade;
  - `S/tiles.js`.
- `S/rm/*.png`, `S/rm/*.log`: the remake, generated world 7, driven by `S/rrun.sh` and `S/rhelp.sh` (`lovec.exe game --shot --res 1280x720 --world 7`). The `r*` runs are at the remake's own scale; the `v*` runs use `--view 720` so the world is 1:1 like the original's.
- `S/cmp/m_*.png`: side by side, original left, remake right. There are 23: spawn, village_town, village17_town, city, city6, base, hospital, fire, gas, cafe, checkpoint, road, forest, field, secret_camp, pond, north_edge, east_edge, mappage, scale, inside, behind, tree.
- `S/ref/`: the original's ground tiles and decals with their indices (`ground_lvl1_tiles.png`, `ground_env_tiles.png`), `minimap_tile_sheet.png`, and `pillar`, `light_tower`, `b_exit` and `tree_block` cut by `../cut.py`.
- Readers of the events: `S/elements.py` (what each `create_map_element` class and kind creates), `S/locs.py` (what each `generate_location` kind lays), `S/cml.py` (each kind's ground and decals), `S/placed.py` (the placed instances of a layout).

## Summary

**What I read.** Original, Game_events:
- 3.6 (layout start: the mapgen grid, the generation order);
- 3.8 (`generate_array_locations`, every level's quota);
- 3.10 and 3.11 (roads, the secret location);
- 3.12 (`generate_array_tomap`: each location's kind);
- 3.13 to 3.15 (crashes, the bunker entrance);
- 3.16 (`generate_location`: every kind's layout);
- 3.5.5 (`create_map_location`: ground tiles, paving, decals, crows);
- 3.5.7 (`create_map_element`: what each class and kind makes);
- 3.17 (`generate_minimap`), 3.20 to 3.23 (the map tip);
- 2.11 (`b_exit`), 2.13 (the spawn), 8.8 (`Level_change`);
- 12.6.4 (the pad's hint markers), 12.13.1 (`Doors_auto`), 16.1.6 (felling trees), 20.1 to 20.4 (autozoom, facades, obstacles).

Also data.js's placed tilemaps in the Map layout (the coast, the forest borders, the ground templates), objects.txt, globals.txt and missing.txt.

Remake: Progress 21, 25 to 28 (27.1.11, 27.3.13, 27.4.9 and 27.4.10 among them) and docs/original_data.md. Files:
- `data/lots.lua`, `data/places.lua`, `data/roads.lua`, `data/ground.lua`, `data/minimap_symbols.lua`, and every prefab in `data/maps/prefabs/`;
- `src/map/*.lua` (generate, places, links, fill, coast, bands, loader);
- `src/game/systems/interior.lua` and `render.lua`;
- `src/core/camera.lua` and `viewport.lua`;
- `src/ui/minimap.lua`.

**What I ran.**
- Original: two first maps, each grid dumped and counted. In one map:
  - teleported to one location of every kind and shot it at noon (24 shots);
  - the map's page;
  - the four edges;
  - inside a garage's warmzone (layer scale read live: 2.0, against 1.0 outside);
  - behind a village house (the player's outline);
  - beside a tree;
  - its ground tilemaps read.
- Remake, generated world 7: all 49 places listed. Then:
  - teleported to each place kind, at its own scale and at 1:1, and shot it;
  - the spawn, the four edges, the map's page;
  - behind, in front of and inside a farmhouse, beside a tree;
  - every object in the map table counted (2,193 objects, 38 parked cars, 1,308 pines and 45 leaf trees in 416 lots).

**The biggest gaps.**
1. **The camera.** The original draws the world at 1:1 (1280x720 world px on a 1280x720 screen) and zooms to 2x in a room. The remake is 2.25x everywhere (569x320 world px). Progress left this as 27.1.11.
2. **How a place is built.** In the original every place is one 1020 px location with a fixed layout. That layout comes from `generate_location`'s own arrays: a village is 5 to 11 buildings, a pump, cars and trees round asphalt streets. In the remake a place is 2 to 4 lots of 1200 px, filled with rows of buildings along sand lanes, every facade fronting a road. This holds for villages, cities, the base, the hospital, the fire station, gas stations, cafes and checkpoints alike. The paving and checkpoint items of 27.3.13 are two of these.
3. **The country between the places.** An original wild location is one of four things:
   - a forest of 16 slots (tree clumps, deer stands);
   - a wood (pillboxes);
   - a field;
   - a pond with fish nests.

   Each has a berry bush and 200 to 250 ground decals (rocks, stumps, tall grass, tyres, blood, bodies). The roads carry telegraph poles, abandoned cars, buses, roadside compounds and crows. The remake's countryside is four pines a lot on bare grass, its roads are empty, and nothing ever stands on a road.
4. **The roads.** The original has one road: four cells of asphalt, about 200 px. The streets in its villages, cities and bases are the same asphalt. The remake has five classes, two of them sand tracks (town and base lanes, the coast track). It also uses asphalt and grass variant pieces the original never lays.
5. **The map's frame.** The original is bounded:
   - sea and sand on both the west and the east coast;
   - a solid forest wall along the north and south edges.

   The original is a chain of maps, one season each, joined by a boat on the east coast. The remake has sea on the west only and open edges. It is one world with the five seasons side by side (kept: the user's asks).
6. **The map screen.**
   - The original's page shows the whole 16x16 map, with forest, field and pond glyphs and the secret location's red "?".
   - The remake shows a 12-lot window. Its woods alternate with the hunters' camp glyph, it uses the pond glyph for fields, and it has no markers.
   - The original has no corner minimap.

**What is kept and why.**
- The world's size and its five seasons side by side, colder going east (Stage 26, Stage 27: "Increase the height", "biomes ... 100 across").
- More places than the original's quota, and roads joining every one of them (Stage 27).
- The beach on the west with the spawn on it (Stage 26).
- The outside going dark from inside a room (Stages 26 and 28).
- A door button instead of doors that open by themselves (Stage 23).
- A piece of furniture on each loot point (Stage 23).
- Lots built once and kept (Stage 6, every item permanent).

Two questions need the user:
- Should each season band carry its own map's content? The original's later maps have more cities and bases, heli crashes, a bunker and gun-shop locations.
- Should the page show the whole of the much wider world, or a window?

**Covered in other surveys, not repeated here.**
- The camera's follow lag, the body centred, and the view clamped at the edges (character 9 to 11).
- Draw order against trees and zombies (character 24).
- Night darkness, lights and lamp-post cones (survival 18 to 20), and the cold east (survival 14).
- Zombie spawn points per location and their groups (zombies).
- Loot tables and the gun shop's rack (items).
- The corner minimap is also hud 12; it is listed here as finding 35 because it is the world's map.

## Findings

Verdicts: make identical 33, keep (user asked) 8, unclear 2

### 1. Scale on screen: the original draws the world 1:1

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Progress 27.1.11 says so)
- **Files:** `game/data/config.lua` (`world_view_height`), `game/src/core/viewport.lua`, `game/src/core/camera.lua`

**Original:** The world layers stay at scale 1 (`Current_zoom_lvl` 1; `layerscale(0)` read live = 1 outdoors, `S/grid.js`). The project is in fullscreen mode "crop" (project[12] = 1), so a 1280x720 window shows 1280x720 world px. The 30 px player is 30 px tall (`S/orig/o1_spawn_noon.png`). Character 7 measured 844x390 world px on the phone.

**Remake:** `world_view_height` is 320 and `viewport.world_scale()` is the window's height / 320. That is 2.25 at 720 (569x320 world px, the player about 67 px tall, `S/rm/r1_spawn.png`) and 1.22 at 390 (692x320). See `S/cmp/m_scale.png`. Every place shot at 1:1 (`S/rm/v*`) needed `--view 720`.

### 2. No 2x zoom in a room

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (the Stage 26 and 28 asks are about what is hidden, not the zoom)
- **Files:** `game/src/core/viewport.lua`, `game/src/game/systems/interior.lua`

**Original:** In `autozoom` (20.1), every 0.5 s, if `player_base` overlaps a `warmzone`, group `zoomin` adds 0.05 every 0.01 s to layers 0 to 50 up to `Current_zoom_lvl + 1` = 2. `zoomout` takes them back to 1 when you leave. I teleported into a garage's warmzone and read the layer scales live: 2.000 (`S/orig/o4_v9_inside.png`). At 1280x720 that is 640x360 world px of room and street.

**Remake:** `viewport.world_scale` depends only on the window's height, and there is no zoom in `src/`. A room is shown at the same 2.25 as the street (`S/rm/r3_house_in.png`; `S/cmp/m_inside.png`). This is the same finding as character 8.

### 3. One world with the seasons side by side, against a chain of maps joined by a boat

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 26 "Traveling right slowly gets colder and eventually the tiles change"; Stage 27 "make the biomes wider, like 100 across"
- **Files:** `game/src/map/bands.lua`, `game/data/ground.lua`, `game/src/map/generate.lua`

**Original:** The Map layout is generated afresh for each `CurrentLevel`, each level on its own ground sheet: `ground_lvl1` on map 0 up to `ground_lvl5`'s snow on map 4 and after (3.5.5.1.x.1-6). `b_exit` stands in the east sea, at x 17241 and y 700 + random(15900) (2.11). It is a wooden boat on map 0, then rubber, gas, a bridge on map 3 and "Buy" (the helicopter) from map 4. Touching it runs `check_lvl_change` and `Level_change` (8.8.1, 8.8.5: `CurrentLevel` + 1), which builds the next map. So the original's five seasons are five maps in a row, west to east, joined by the boat.

**Remake:** One 31200 px world, with the five sheets in 6000 px bands from the coast, edges ragged (`data/ground.lua` seasons, `src/map/bands.lua`), and no exit.

### 4. Each later map's own quota and kinds

- **Verdict:** unclear  **Effort:** L  **User's ask:** none directly; Stages 26 and 27 put the five maps' seasons side by side in one world
- **Files:** `game/data/places.lua`, `game/src/map/places.lua`, `game/src/map/fill.lua`

**Original:** `generate_array_locations` sets each map's own counts (3.8.2 to 3.8.6). Columns are villages, military, cities, hospitals, fire stations, gas stations, cafes:

| map | villages | military | cities | hospitals | fire stations | gas | cafes | other |
|---|---|---|---|---|---|---|---|---|
| 1 | 15 | 4 | 5 | 3 | 3 | 5 | 5 | |
| 2 | 10 | 6 | 10 | 4 | 4 | 6 | 6 | |
| 3 | 8 | 7 | 11 | 5 | 5 | 5 | 5 | rains_level 2, tanks |
| 4 | 9 | 10 | 16 | 6 | 6 | 7 | 14 | Day_cold 9 |

Map 1 is `CurrentLevel` 1, the second map; the first map's quota is finding 7.

From map 1 on, the generator also adds:
- a gun-shop location (32: the gun shop, a shed, five cars) in place of a wild one (3.12.4);
- military kind 5 (barracks, tents, hesco walls, sandbags) and, from map 3, kind 70 (the HQ, barracks, hesco) in place of the first map's kind 22 (3.12.5-9);
- a bunker entrance (3.15);
- 2 to 5 heli crashes and 3 to 8 humvee crashes (3.14).

Bandits, hordes and camps of bots are switched on too; those are out of scope.

**Remake:** One quota for the whole world, the same in every band (`data/places.lua`). There is no bunker, heli crash or gun-shop location. The base mixes buildings of maps 0 to 3 (finding 13). It is for the user whether band N should hold map N's quota and kinds.

### 5. The world's size

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 27 "Increase the height ... make the biomes wider, like 100 across" (100 ground tiles a biome)
- **Files:** `game/data/lots.lua`, `game/src/map/generate.lua`

**Original:** The Map layout is 17850x20000. `Map_size_X` and `Map_size_Y` are 16: a 16x16 grid of locations from x 700 to 17020 and y 320 to 16640 (3.6.1). Map 0 counted live had 256 locations (`S/grid.js`).

**Remake:** 26 lots across and 16 down, 31200x19200 (`generate.extent`). The 16 rows match `Map_size_Y`.

### 6. A location is 1020 px, a lot 1200

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 27's 100 is in ground cells, which this does not change)
- **Files:** `game/data/lots.lua` (`size`), every prefab in `game/data/maps/prefabs/`, `game/src/map/fill.lua`

**Original:** `mapgen` points are 1020 apart (3.6.1: 700 + col x 1020, 320 + row x 1020). Each location's ground is a 17x17 tilemap of 60 px cells (3.5.5: Repeat(289), SetTileRange over 17), and every offset in 3.16's layouts is 0 to 1000.

**Remake:** `size = 1200`, commented "roughly 1080; 1200 is that, rounded to something readable". A 100-cell band is 5 lots here and would be 5.9 locations at 1020.

### 7. How many places

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 27 "More interest points with roads connecting them"
- **Files:** `game/data/places.lua`

**Original:** Map 0 (3.8.1) has:
- 20 villages, 3 cities, 2 military, 1 hospital and 1 fire station;
- 4 gas stations and 4 cafes on road locations (3.12.2-3);
- 1 secret location (3.11.4) and 1 humvee crash (3.14.1);
- no heli crash;
- a checkpoint on every crossing.

Counted live on one map:
- villages: kinds 9: 4, 4: 4, 16: 5, 17: 7;
- 3 cities, 2 bases, 1 hospital, 1 fire station;
- gas stations 30: 1, 31: 3; cafes 50: 3, 51: 1;
- secret location 48;
- 4 crossings among 35 road locations.

**Remake:** Seed 7 has 49 places: 3 cities, 3 bases, 16 towns, 2 camps, 2 checkpoints, 2 hospitals, 2 fire stations, 8 gas stations, 9 cafes and 2 crashes (`S/rm/r1.log`).

### 8. Which places the roads join

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 27 "roads connect every interest point to each other"
- **Files:** `game/src/map/links.lua`

**Original:** `generate_array_roads_1` and `_2` (3.10, 3.11) work row by row and then column by column. In each, a straight road runs between the first two villages, cities, hospitals or fire stations (9, 6, 11, 15) found; where a column's road crosses a row's, the location is a crossing (14). Military bases and the secret location are never joined, and a third place in a row or column gets no road. On the live page, many places have no road (`S/orig/o4_mappage.png`).

**Remake:** Every place and the spawn are joined, with loops (27.3.3).

### 9. One class of road, four cells of asphalt; no sand tracks

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 21 "start with roads"; Stage 27 is about which places are joined, not how roads look)
- **Files:** `game/data/roads.lua`, `game/src/map/roads.lua`, `game/src/map/places.lua`, `game/data/places.lua`

**Original:**
- A road is a location's two middle rows of tile 15, the full asphalt piece (SetTileRange(x, y + 8, 17, 2, 15), 3.5.5.1.5.7). Arms of the same run to the neighbours that are roads or places.
- The autotiler turns the grass cells on either side into the frame's edge pieces (tiles 0 to 5 and 7 to 12, 3.5.5.1.5.7.5). That makes four cells, about 200 px of asphalt (`S/orig/o3_road13.png`).
- The streets inside villages, cities, the base and the hospital are the same tile 15, one or two cells wide (3.5.5.1.18, .20, .23, .24).
- No road anywhere is sand: `tilemap_sand` is only the beach.

**Remake:** Five classes in `data/roads.lua`:

| class | cells | surface |
|---|---|---|
| highway | 4 | 204 px of asphalt |
| main | 3 | 144 px |
| street | 2 | 84 px |
| dirt | 2 | `tilemap_sand` |
| lane | 2 | `tilemap_sand` |

Towns and bases lay sand lanes every 600 px (`S/rm/v1_town_a.png`, `S/rm/v1_base_a.png`, `S/rm/v2_base_b2.png`), and the coast and the camps have sand tracks. The original's road is the remake's highway.

### 10. What stands on and beside a road

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/map/fill.lua`, `game/src/map/places.lua`, `game/data/roads.lua`

**Original:** A road location (3.16.1.37 east-west, .38 north-south) holds:
- wooden telegraph poles (`pillar` frame 0) at 250 and 750 along it, 200 px off the asphalt;
- half the time one of three things: three cars on the carriageway (sedans and vans at y 520 ± 50), the bus (`b_car_bus`, 3.16.1.37.2.2.2), or one car and four trees either side;
- one time in five, a roadside compound: a garage, a tent, two trash containers, four concrete blocks and 24 chain-link panels (3.16.1.37.1);
- one time in three, a zombie point (frame 10) and three crows (3.5.5.1.5.8);
- 210 ground decals (finding 27).

`S/orig/o3_road13.png` shows the poles, cars on the asphalt and two crows.

**Remake:**
- Nothing stands on a road: the network keeps every prefab off it (27.3.12), and only the crash's humvee is on the asphalt.
- `data/roads.lua` lists "lamp posts beside a road" as not in the rip, but `Assets/Sprites/pillar` holds the pole (finding 24).
- The bus is in no place's pool (27.1, found in review).
- Seed 7 parks 38 cars in the whole world (`S/rm/r5.log`), none of them on a road.

### 11. Villages: one location, four layouts

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (Stage 21 "Interest points range from basic towns to cities to military bases" names the kinds, not their make-up)
- **Files:** `game/data/places.lua` (town), `game/src/map/fill.lua`, `game/src/map/places.lua`

**Original:** A village is one location, one of four layouts picked at random (3.12: choose(9, 4, 16, 17)), with fixed offsets:

| layout | event | buildings | other |
|---|---|---|---|
| 9 | 3.16.1.34 | 4 houses (brown, red, green, yellow or the hostel); 6 garages in two rows of three; a shed or the supermarket | 3 cars, a water pump, 9 trees |
| 4 | .27 | 5 houses; 2 garages; one of the yard house, the piano house, the police station, red2 or the gun shop | 3 cars, a pump, 9 trees |
| 16 | .40 | 2 houses; the school; 2 sheds | a car, 3 trees, 14 chain-link panels round the school yard, 2 benches, 2 trash containers, 2 signs |
| 17 | .41 | 2 houses; 2 small sheds; the church or, one time in two, the red-brick house | 3 cars, 8 trees, 3 benches |

All four have:
- asphalt streets: a 2-cell cross with 1-cell lanes (3.5.5.1.23, .16, .25, .26);
- 250 ground decals;
- one zombie point (frame 0).

See `S/orig/o3_v9.png`, `o3_v17.png`, `o4_v9_street.png`.

**Remake:** A town is one or two lots along its road and a lot deep either side: 2 to 4 lots of 1200 px (data/places.lua town). Its make-up:
- the red farmhouse first, then rows along sand lanes every 600 px;
- 15% of plots are gardens, a fence across the front and trees behind;
- a landmark: none (2), the school (1) or the church (1), never the red-brick alternative;
- a pool of village houses, the yard, red2, the red-brick house, the hostel, sheds, garages and small sheds. There is no supermarket, piano house, police station or gun shop.
- No cars, no pump, no benches.

See `S/rm/v1_town_a.png`, `v2_town_c2.png`; `S/cmp/m_village_town.png`, `m_village17_town.png`.

### 12. Cities: one location each

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/places.lua` (city), `game/src/map/places.lua`, `game/src/map/fill.lua`

**Original:** A city is one location, kind 6 or 21 (3.12):
- **6** (3.16.1.31): five city houses (1, 2 or 4) in two rows at y 248 and 750; one of the piano house, the supermarket, the police station, red2 or the gun shop; 2 cars; 4 trees. Its streets are an asphalt cross with 1-cell lanes (3.5.5.1.20).
- **21** (.32): two of city house 3 and two of 1, 2 or 4; two garages, or none (kind 6 of class 7 makes nothing); 4 cars; 3 trees. A 5x5 asphalt square sits at its crossing with a 3x3 grass island and two trees in it (3.5.5.1.21; `S/orig/o3_c21.png`).

Map 0 has 3 such locations.

**Remake:** Each city is 2x2 lots (2400 px square) with avenues on the two lot lines through it, a street every 600 px, and rows of 13 kinds of building. Those include the hostel, the red-brick house and three garages; the church is not among them. A car stands in one gap in four, and each lot has 5 zombie points (`data/places.lua` city; `S/rm/v1_city_a.png`, `v2_city_b2.png`; `S/cmp/m_city.png`, `m_city6.png`).

### 13. The military base on the first map

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/places.lua` (base), `game/src/map/fill.lua`, `game/src/map/places.lua`

**Original:** On map 0 every base is kind 22; 3.12 turns 22 into kind 5 or 70 only from map 1. Kind 22 (3.16.1.29) holds:
- 4 brick barracks (`b_military_barrack2`) in a row at y 296 and 2 east tents;
- 2 BTRs, 2 trucks and 4 trees;
- a lamp post (`pillar` 12 to 15 with `light_tower`);
- chain-link `fence_horizontal` along the top, with a gap where the road comes through, and down both sides for the top third only;
- an asphalt cross (3.5.5.1.18; `S/orig/o3_mil22.png`).

It has no HQ, no `b_military_barrack`, no tent of the other kind, no pillbox, no hesco and no UAZ.

**Remake:** Each base is two lots inside a full wall of `fence_horizontal` and `fence_vertical`, 150 px in, with a 300 px gate. Inside:
- the HQ first (`b_military_shtab`, a map 3 building in the original);
- barracks of both kinds and tents of both kinds;
- a pillbox (`b_dot`);
- 2 to 4 vehicles including the UAZ;
- sand lanes.

See `data/places.lua` base; `S/rm/v1_base_a.png`, `v2_base_b2.png`; `S/cmp/m_base.png`.

### 14. The hospital: its car park, paved, and its layout (27.3.13)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/maps/prefabs/hospital.lua`, `game/src/map/roads.lua` (a paved pad), `game/src/map/fill.lua`

**Original:** Location 11 (3.16.1.35) holds:
- the hospital at (726, 212), at the top right of the location;
- eight cars in two rows at the bottom left (x 171 to 418, y 651 and 765), two more on the west side and one in the middle;
- 6 trees.

Its ground is asphalt in tile rects (2,1,7,3), (2,1,3,12) and (2,7,12,6): a car park round the building and down its west side. A paved strip joins each neighbouring road (3.5.5.1.24.7). See `S/orig/o3_hosp.png`.

**Remake:** The hospital fronts the road on the lot's south edge, with ten cars in rows either side on its line and six trees, all on bare ground (`hospital.lua`; `S/rm/v1_hosp_a.png`; `S/cmp/m_hospital.png`). 27.3.13 left the paving.

### 15. The fire station: its paving and layout (27.3.13)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/maps/prefabs/fire_station.lua`, `game/src/map/roads.lua`

**Original:** Location 15 (3.16.1.36) holds:
- the station at (318, 269);
- houses at (909, 336) and (316, 747) and a small shed at (122, 722);
- a car at (710, 147) and the pump at (657, 653);
- 6 trees.

It is paved in tile rects (8,2,5,2), (1,8,15,1), (8,8,1,7), (1,14,8,1) and (10,2,3,7), plus arms to its roads (3.5.5.1.27.7). That makes driveways to the garage doors and a yard (`S/orig/o3_fire.png`).

**Remake:** The same contents, but fronting the south road and on bare ground (`fire_station.lua`; `S/rm/v1_fire_a.png`; `S/cmp/m_fire.png`). 27.3.13 left the paving.

### 16. Checkpoints stand on the crossing, and every crossing has one (27.3.13)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/places.lua` (checkpoint), `game/src/map/fill.lua`, `game/src/map/places.lua`

**Original:** Every crossing location (14) is a checkpoint (3.16.1.39), one of five variants (3.16.1.39.5.1-5), and stands on the crossing itself:
- **Variant 1:** a BTR in the middle of the crossing (505, 535), with:
  - eight concrete blocks (`obst_block`) on the four carriageways, at (514, 247), (575, 258), (509, 861), (570, 856), (227, 507), (231, 576), (828, 501) and (815, 572);
  - 24 chain-link panels at right angles round the four corners;
  - four east tents;
  - four cars, two of them on the road;
  - two trash containers, two benches and a lamp post.
- **Variant 2:** the same with plain tents.
- **Variant 3:** 20 sandbags, four tents, a truck or BTR, three cars and four trees.
- **Variant 4:** the police, with two police cars each way on the road, a truck and eight trees.
- **Variant 5:** east tents.

Its zombie point is frame 3, or 12 for the police. See `S/orig/o3_chk14.png`.

**Remake:**
- Up to four crossings a world, 4800 px apart, never by a city or a town.
- Every prop is in a corner quarter, clear of the asphalt; the crossing is empty.
- Its five variants are the remake's own sketches of the original's (`data/places.lua` checkpoint corners; `S/rm/v1_chk_a.png`; `S/cmp/m_checkpoint.png`).

### 17. The gas station

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (its crates are kept, finding 42)
- **Files:** `game/data/maps/prefabs/gas_station.lua`

**Original:** Location 30 or 31 is a road location with the station beside its road (3.16.1.42, .43). It holds:
- the shop at (107, 151), the canopy at (335, 366) and a small shed at (248, 105);
- two cars and two telegraph poles;
- a paved forecourt (3.5.5.1.7);
- one time in three, crows.

Its three or four trees are given absolute coordinates with no `hotspotX`, so they land near the map's corner and none stands at the station. See `S/orig/o3_gas30.png`.

**Remake:** The shop and the canopy stand on the front line, with four chain-link panels behind them, a tree, two crates and a water bottle. There is no shed, no car, no pole and no paving (`gas_station.lua`; `S/rm/v1_gas_a.png`, `v2_gas_c.png`; `S/cmp/m_gas.png`).

### 18. The roadside cafe

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/maps/prefabs/cafe.lua`

**Original:** Location 50 or 51 (3.16.1.1, .2) holds:
- the bar at (237, 283);
- two telegraph poles;
- three cars parked at x 734 to 898, y 240 to 340, and one on the road;
- a paved lot of tile rects (3,5,2,5), (12,4,4,4) and (3,8,10,3) (3.5.5.1.2.7);
- its trees at absolute coordinates, the same bug as the gas station's;
- a zombie point (frame 11).

See `S/orig/o3_cafe51.png`.

**Remake:** The bar, a shed (`b_shed`) and a tree, with kvas and an apple on the ground. There are no cars, poles or paving (`cafe.lua`; `S/rm/v1_cafe_a.png`; `S/cmp/m_cafe.png`).

### 19. The countryside: forests, woods and fields

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/maps/prefabs/field.lua`, `game/data/lots.lua` (`filler`), `game/src/map/fill.lua`, `game/src/map/wildlife.lua`

**Original:** A wild location on maps 0 to 3 is choose(3, 1, 2, 8, 10, 1, 2, 8, 10, 52, 52) (3.12):
- **1 or 10, a forest** (3.16.1.4): 16 slots on a 4x4 grid 250 px apart. Each slot is a tree 14 times in 15 and a deer stand otherwise; the tree is one of ten kinds, of which 7 to 10 are `tree_block` clumps of four to six trees, 146x166. It also has a red berry bush, and one time in two an animal point (frame 5, 6 or 7).
- **2 or 8, a wood** (.5): 16 slots, each a tree four times in five. One time in nine a pillbox (`b_dot`), plus a red or blue berry bush.
- **52, a field** (.3): 16 slots, each a tree one time in two. One time in nine a pillbox, plus a black berry bush, with 200 tall-grass decals (3.5.5.1.4.9).
- **3, a pond** (finding 20).

One map counted live: 66 forests, 61 woods, 35 fields and 23 ponds of 256 locations. See `S/orig/o3_forest1.png`, `o3_forest2.png`, `o3_field52.png`.

**Remake:** Every lot no place covers is the `field` prefab: four pines and one zombie point, plus the wildlife points (28.2). Seed 7's map table has 1,308 pines and 45 leaf trees in its 416 lots (`S/rm/r5.log`). See `S/rm/v2_field_a.png`, `v2_field_b.png`; `S/cmp/m_forest.png`, `m_field.png`.

### 20. Ponds, with fish nests

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/map/coast.lua` (the only water), `game/src/map/fill.lua`, `game/data/lots.lua`

**Original:** Location 3 is one in eleven of the wild ones. Its pond is solid `tilemap_water`, laid in tile rects (4,4,8,8), (3,5,10,5) and (6,3,4,10): about 600 px across with stepped corners (3.5.5.1.15.15). It also has:
- four `fish_nest`s at its edges (3.5.5.1.15.13-14), used for fishing;
- yellow berry bushes round the edge (3.16.1.26);
- 250 decals;
- the page glyph 7.

One map had 23 of them, one beside the spawn (`S/orig/o3_pond3.png`, `o1_spawn_noon.png`; `S/cmp/m_pond.png`).

**Remake:** No inland water. The only water is the west sea.

### 21. Tree kinds: the clumps and the two newer trees are never placed

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/places.lua`, `game/data/maps/prefabs/*.lua`, `game/data/colliders.lua`

**Original:** Class 1 has ten kinds (3.5.7.1.1.1-10): `tree_pine`, `tree_leaves`, `tree_leaves_2`, `tree_pine2`, `tree_leaves_new2`, `tree_leaves_new3`, and `tree_block` frames 0 to 3, a clump each with a `big_obstacle_base`. Villages and roads draw from kinds 1 to 6, forests from 1 to 10.

**Remake:** Only `tree_pine`, `tree_pine2`, `tree_leaves` and `tree_leaves_2` are placed. `tree_leaves_new2`, `tree_leaves_new3` and `tree_block` are in `Assets/Sprites` but placed nowhere.

### 22. Trees can be felled

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 24's campfire kit needs wood and sticks, which a fallen tree drops in the original)
- **Files:** `game/src/game/systems/combat.lua`, `game/data/colliders.lua`

**Original:** In 16.1.6, a melee bullet (`var#7` set) hitting a tree's `obst_base` (frame 10) takes one of the tree's 3 points (`var#0`) with a chop sound. At 0:
- it plays `treefall_02` or `_03` and shows frame 1, the stump (78x32);
- it drops two sticks (item 38) and a wood (50), plus item 37 for `tree_leaves_2`;
- its obstacle becomes frame 8.

**Remake:** Trees are scenery: frame `default_1` (the stump) is unused and no tree takes a hit. "chop" is only the axe's swing sound. The ingredients come from the remake's own hearths and makings instead (finding 33).

### 23. Water pumps and berry bushes

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/places.lua`, `game/data/maps/prefabs/*.lua`

**Original:**
- A `waterpump` stands in villages 9 and 4 and at the fire station (3.16.1.34, .27, .36).
- A berry bush stands in every forest, wood and field (red, blue or black), and yellow ones stand round every pond (3.16.1.3-5, .26).
- Both carry an outline in reach (hud 27).

**Remake:** There is one pump, at the fire station. Berry bushes are only in two camp variants (`camps.lua`).

### 24. Telegraph poles, road signs and lamp posts

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/roads.lua`, `game/src/map/fill.lua`, `game/data/places.lua`

**Original:** The `pillar` frames (3.5.7.1.4.1-5; `S/ref/pillar_sheet.png`):
- **0:** a wooden telegraph pole, at roads, gas stations and cafes, 500 px apart.
- **2 and 8:** a road sign and another post, in village 16.
- **12 to 15:** a lamp post with a `light_tower` cone (12 and 14 lit, 13 at frame 0, 15 without), at bases and checkpoints.

**Remake:** None is placed. `data/roads.lua` says lamp posts were "searched and not there", yet `Assets/Sprites/pillar` holds all 13 frames and `light_tower` its two. The lamps' light at night is survival 20.

### 25. Parked car kinds: the hatchbacks and the bus

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (driving is out of scope; parked cars are props)
- **Files:** `game/data/places.lua`, `game/data/maps/prefabs/*.lua`

**Original:** Class 6 has 17 kinds (3.5.7.1.5.1-17):
- sedans in blue, red and gray each way, and green vertical;
- three vans;
- police cars each way;
- the BTR and the truck;
- three hatchbacks (`car_hatch_grey`, `_red`, `_white`).

Roads, gas stations and checkpoints choose from lists that include the hatchbacks (choose(1, 3, 5, 8, 9, 10, 11, 15, 16, 17)). The bus is a building (class 7, kind 31) parked on roads.

**Remake:** No hatchback anywhere, and the bus is unplaced. The UAZ stands in bases (`car_uaz_anims`); the original's UAZ is a drivable car from `Car_respawn_uaz`, and vehicles are out of scope.

### 26. Crows on the roads

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/map/fill.lua`, `game/src/map/wildlife.lua`, `game/data/animals.lua`

**Original:**
- `crow_skin` (idle and flying animations, group 13.5.1 `crow_fly`) is created three at a time, one time in three, on road, gas station, cafe and hospital locations (3.5.5.1.x.8).
- 14 come with one secret location and 5 with another (3.5.5.1.45.2, .32.1).

See the two crows in `S/orig/o3_road13.png`.

**Remake:** None. The `crow_skin` art is missing from `Assets/Sprites` (missing.txt). How they fly is the animals area.

### 27. Ground decals: rocks, stumps, grass, tyres, blood and bodies

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/render.lua`, `game/src/map/generate.lua`, `game/data/ground.lua`

**Original:** Every location gets a `ground_enviroment_tilemap` with 30 px cells, 34 by 34 (3.5.5.1.x.9). It is scattered with 150 to 250 decals from a list per kind:
- 210 on roads and places;
- 200 in the wild;
- 250 in villages and at the fire station;
- 150 at the gun-shop location.

The decals are rocks, sticks, stumps, bushes, tall grass, reeds, tyres, manholes, blood splats, bodies and litter (`S/ref/ground_env_tiles.png` numbers them). They are what makes every original shot busy.

**Remake:** The sheet is packed (`tilesets/ground_enviroment_tilemap` in the atlas) and drawn nowhere. The ground is bare tiles. Compare `S/orig/o3_forest1.png` and `o3_road13.png` with `S/rm/v2_field_a.png`.

### 28. The grass: one plain piece

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 26's ask is the seasons' sheets, kept)
- **Files:** `game/data/ground.lua` (each band's `grass`)

**Original:** A location's ground tilemap is made from the template filled with tile 6 alone, the plain grass at (60,60). The Map layout's templates are `289x6` (data.js), and 3.5.5 sets only tile 15 and the edge pieces over it. The variety is the decals (finding 27).

**Remake:** Each band lays (60,60) five times in eight and (120,180), (60,240) and (120,240) three times in eight: a dark patch and two kinds of flowers. These read as repeating blotches (`S/rm/v2_field_a.png`).

### 29. Asphalt: one piece

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ground.lua` (each band's `asphalt`), `game/data/roads.lua` (`full`)

**Original:** A road's and a street's full cells are tile 15 alone, (0,180). SetTileRange with value 15 appears 145 times in 3.5.5, and the autotiler sets only edges.

**Remake:** Four variants in each band: (0,180), (0,240), (0,180) and (60,180). That puts a starburst crack on one cell in four, and lvl3's ice and lvl4's and lvl5's slabs on others.

### 30. The west coast: a narrow beach and a straight shore

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 26 "On the left side of the map, there should be a beach are. Players spawns on the beach" (the beach is kept; its width and shape are not asked)
- **Files:** `game/data/lots.lua` (coast), `game/src/map/coast.lua`

**Original:** The coast is placed tilemaps in the Map layout (data.js, `S/placed.py`):
- `tilemap_water` "Left", solid, from x -95 to 569;
- `tilemap_sand` from 389 to 767, under the water;
- grass from 767, and the location grid from x 700.

The shore is one straight line the full height of the map. In `S/orig/o1_spawn_noon.png` the sea ends at 555 and the sand at 715: about 160 to 200 px of beach.

**Remake:** The coast takes a whole 1200 px lot: sea to 300 to 420 px, sand to 840 to 960, and grass to the lot line. Each edge is a seeded walk that steps a cell every 3 to 8 rows. That gives about 500 px of sand and a ragged shore (`S/rm/v1_spawn.png`; `S/cmp/m_spawn.png`).

### 31. The east coast

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 26 made the east cold, not its edge; the original's snow map has this same coast)
- **Files:** `game/src/map/coast.lua`, `game/data/lots.lua`

**Original:** `tilemap_sand` "Right" runs from x 16956 to 17814 and solid `tilemap_water` "Right" from 17235 to 17914, the full height. The level's exit boat (`b_exit`) is in that sea (`S/orig/o4_edge_e.png`; finding 3).

**Remake:** Snow runs to the east bounds wall at 31200, with no sea (`S/rm/v2_edge_e.png`; `S/cmp/m_east_edge.png`).

### 32. The north and south edges: a forest wall

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/map/loader.lua` (`build_bounds`), `game/src/map/generate.lua`

**Original:** `tilemap_forest` is solid, with 150 px tiles of dense trees. It runs "Up" from y -39 to 420 and "Down" from 16504 to 17126, the full width (data.js; `S/orig/o4_edge_n.png`, `o4_edge_s.png`).

**Remake:** Plain grass runs to an invisible static wall (`loader.build_bounds`), and the view goes past it (`S/rm/v1_edge_n.png`; `S/cmp/m_north_edge.png`).

### 33. Where the player starts

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 26 "Players spawns on the beach" (kept: both start on the west beach)
- **Files:** `game/data/places.lua` (`spawn`, `makings`, `hearth`), `game/src/map/coast.lua`

**Original:** `player_base.SetPos(600, 1500 + random(layoutheight - 5000))` (2.13). That is x 600 on the sand at the water's edge, y anywhere from 1500 to 16500. Nothing is placed for the player: no road, house or kit beyond the start kit (character 6).

**Remake:**
- The spawn is on the sand 210 px from the sea and 240 to 600 px from where one of the two coast roads reaches the beach.
- A red farmhouse stands by that road within 480 px of the coast lot.
- Wood, sticks and a newspaper lie at the beach's edge.
- Every town's home has the same by its door (26.3, 25.2).

### 34. The player's outline behind a facade

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/interior.lua`, `game/src/game/systems/render.lua`

**Original:** While the player is behind a facade (`var#42`, set in 20.2.1.1.2.1.1), 20.2.6 turns on the `Outline` effect of the player's skin: a white outline round him through the faded house (`S/orig/o5_house_behind_crop.png`).

**Remake:** The facade fades to 0.25 over its half-dark plate, as the original's does (28.1.4), but the player has no outline (`S/rm/r6_behind_80_crop.png`; `S/cmp/m_behind.png`).

### 35. The corner minimap

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 27 "make the map into an openable menu, clicking a button opens and closed the map" points the other way; hud 12; Progress 27.4.10)
- **Files:** `game/data/ui/hud.lua`, `game/src/ui/minimap.lua`

**Original:** No minimap on the HUD. `minimap_tile` instances are parked at -1000, and the map is the notebook alone.

**Remake:** An always-on 202x152 minimap at the top right at 720p. It draws season tints, a blue sea, sand, the roads, places' symbols, country buildings and zombie dots (`S/rm/r1_spawn.png`).

### 36. The page shows the whole map; the remake shows a window

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 27 asks for the world's size, which a whole-map page would shrink
- **Files:** `game/src/ui/minimap.lua` (`page_lots`), `game/data/config.lua`

**Original:** `generate_minimap` (3.17.1) lays one `minimap_tile` per location for all 16x16, `minimap_tile.Width` apart. The whole map is on one page: 16 glyphs across, about 20 px each at 1280x720 (`S/orig/o4_mappage.png`).

**Remake:** 12 lots each way round the player (`minimap.page_lots`), about 11x12 glyphs at about 33 px (`S/rm/r4_mappage.png`; `S/cmp/m_mappage.png`). The whole 26x16 world would be 26 glyphs across at about 12 px.

### 37. The page's glyphs for woods, fields, ponds, camps and cities

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/minimap_symbols.lua` (`page`), `game/data/places.lua` (`mark`)

**Original:** In 3.17.1 (`S/ref/minimap_tile_sheet.png`):
- forests and woods (1, 2, 8, 10): frame 0;
- fields (52): 29;
- ponds (3): 7;
- the hunters' camp (23): 14; the other secret kinds get no frame set and show 0, a forest;
- cities (6, 21): 6;
- the gun-shop location (32): 26.

Frame 31 is never set.

**Remake:**
- woods alternate 0 and 14;
- fields are 7, the original's pond;
- every camp is 14;
- cities alternate 31 and 6.

### 38. The page's markers: the secret location's "?", found crashes, tents, stashes

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/ui/minimap.lua`, `game/data/ui/map.lua`

**Original:** `Pad_load_page` (12.6.4.1.3.1.1.1-13) lays `minimap_hint` marks on the page at (x - 700) / 51 and (y - 320) / 51:
- the secret location's red "?" (frame 7) whenever `secret_location_revealed`, which is 1 at the start in 1.0 (globals.txt);
- a found humvee crash (12) and a found heli crash (11);
- a deployed tent (2) and a stash (13);
- the exit once known (5, 14);
- quests, the friend, the car.

`S/orig/o4_mappage.png` shows the "?".

**Remake:** Only the player's cross. The `minimap_hint` art is missing (missing.txt). Quests, bots and vehicles are out of scope; the "?", the crashes, tents and stashes are not.

### 39. The page's other tabs and the arrow to GLOBAL MAP

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (Progress 27.4.9)
- **Files:** `game/data/ui/map.lua`

**Original:** `Draw_pad` lays four tabs, Map, Guides, Crafting and Tasks, and an arrow to the GLOBAL MAP page (`l_eng_ui.xml`; `S/orig/o4_mappage.png`).

**Remake:** The Map tab only (`S/rm/r4_mappage.png`).

### 40. Doors open by themselves

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 23 "Door button with door sprite when near a door"
- **Files:** `game/src/game/door.lua`

**Original:** In `Doors_auto` (12.13.1), a door opens when the player walks into it from outside: its blocker goes, frame 1, `open_door` or `open_door_metal`. It shuts behind him once he is north of its "in" point.

**Remake:** Doors open and shut with the door button or F (23.2).

### 41. Inside a room, the outside goes dark

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 26 "entering the building hiding the outside"; Stage 28 "Make it hidden only when the player is inside"
- **Files:** `game/src/game/systems/interior.lua`, `game/src/game/systems/render.lua`

**Original:** In the warmzone (20.2.1.2):
- the facade goes to opacity 0;
- the plate shows its "in" frame;
- doors in the room go to 25.

The street stays lit and zooms with the room (`S/orig/o4_v9_inside.png`).

**Remake:** The outside eases to black (`S/rm/r3_house_in.png`). The zoom is finding 2.

### 42. A piece of furniture on each loot point

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 23 "appropriate container scales; boxes, wardrobe etc can just be copied from interior sprites"
- **Files:** `game/data/buildings.lua`, `game/data/maps/prefabs/*.lua`

**Original:** A plate's `spawn_loot_point`s each drop one item on the floor (`Trigger_spawn`, 14.1.4). There are no furniture objects; the furniture is painted on the plate.

**Remake:** One searchable piece stands on each loot point (27.1.3). There are crates at the gas station, the crash and the camps.

### 43. Lots are built once and kept

- **Verdict:** keep (user asked)  **Effort:** -  **User's ask:** Stage 6 "I want every item in the world to be permanent"
- **Files:** `game/src/map/loader.lua`

**Original:** The `Location loader` (3.5) runs once a second after the camera has moved 400 px. It creates every location within 1800 px and erases every one past it, writing each element's state back to its array. Collisions are off past 1500 px.

**Remake:** A lot is built when the player comes within two lots, and kept (27.2.4). Nothing differs on screen.

## Already identical

- Every room is read from the original's own events: its plate (pixel-identical to the rip), walls, doors at image points 3 to 6 and loot points (27.1). The fuel canopy has no room, and the bus's plate is its "out" frame.
- `b_shadows` lies beside every building with a room but the pillbox (27.1.6). See the garages' shadows in `S/orig/o4_v9_street.png` and the farmhouse's in `S/rm/r6_behind_120.png`.
- From behind, the facade is at opacity 25 over its plate tinted (0.5, 0.5, 0.5) (28.1.4). The facade goes only while the player is in the room, the original's warmzone rule (28.1.3).
- The five ground sheets, `tilemap_sand` and `tilemap_water` are the original's. The roads' edge pieces come from the same terrain set.
- The beach is on the west with the spawn on it, and the map is 16 locations or lots down.
- Page and minimap glyphs that match:
  - a village (4), the base on the first map (27), the hospital (8), the fire station (9);
  - gas stations (16, 17) and cafes (19, 20) by their road's direction;
  - the road straights, crossing and tees;
  - the player's orange cross.
- The notebook: `pad_list`, the LOCAL MAP title, the Map tab, the cross, the notepad under the gear, the `walk_marker` tip and its sounds (27.4).
- The humvee lies on its nose (`hammer_crash` frame 1) in the middle of its road (27.3.12).
- The secret locations' props are at the original's own offsets in a ring of twelve trees (`camps.lua`). I checked the hunters' camp: the hut, the deer stand and the van at the original's relative positions (481,501), (605,422), (609,604), shifted by the larger lot.
- Trees' shadows are baked into the same frames in both.

## Not checked

- The original's maps 1 to 4: kinds 5, 70 and 32, the bunker, heli crashes and the exit's later forms were read from the events, not reached in a run.
- The phone: every world shot here is 1280x720. The 844x390 scale is character 7's measurement.
- Night: the lamp posts' cones and the darkness are survival 18 to 20.
- Whether the remake's door leaves go to 25% inside a room, as the original's do.
- Felling a tree and fishing at a pond were read from the events, not played.
- How crows fly and flee (the animals area).
- The decal and tree counts per location come from the events' loops, not from counting a live map.
- The original's ground tilemaps are 1050 px each on a 1020 pitch, overlapping by 30 px; I did not look for seams.
- Rain and snow on the ground, and the tutorial map.
- The remake's hand-made test map, and seeds other than 7.
