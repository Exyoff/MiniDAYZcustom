# items: the remake against the official 1.0

*Area surveyed:* Items and loot. This covers every item the original has (from data.js, its events and l_eng_items.xml) against the remake's data: names, stats, stack sizes, condition, sizes in the inventory, icons and ground sprites. It also covers the loot lists by building, loot point, container and vehicle, with their rates, counts and rare items; attachments and which gun takes which; food, drink and medical values; tools; crafting; and how loot respawns.

`SCR` below is this survey's scratch directory,
`C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/items/`.
Event paths are Game_events unless a sheet is named. Item ids are the original's drop ids. They are the same as the `<id>` in l_eng_items.xml and the frame of the inventory icon sheet.

## Summary

**What I read.**
- In the original:
  - Spawn_drop (14.2), the whole drop-id-to-item table, with what each spawn sets (counts, charge, fill, condition, loaded rounds).
  - Trigger_spawn (14.1.4) and Global_respawn (14.1.2).
  - Every loot point a building places (3.5.7.1.6.N), the crash sites (3.5.5.1.N.10) and the camps (3.5.5.1.29 to .48).
  - The lootbox contents (3.1.10), the car trunks (12.13.3.1.2), berry bushes (12.13.3.1.3) and the airdrop (12.4).
  - item_activate and item_activate2 (8.10.3, 8.10.5), the drag-combine table (Check_craft, 12.14.1.4) and the headlamp and NVG (8.14, 8.15).
  - l_eng_items.xml, l_eng_log.xml and globals.txt.
- In the remake: Progress.md (stages 6, 10, 16.2, 23.4 to 23.6, 24, 25.2, 27.1, 28.2, the reference table and out of scope), plus:
  - data: `items.lua`, `loot_tables.lua`, `containers.lua`, `trunks.lua`, `attachments.lua`, `recipes.lua`, `item_art.lua`, `buildings.lua`, `places.lua`, the prefabs (`camps`, `crash`, `gas_station`, `cafe`), `config.lua` (`loot`, `items`) and `generated/items_index.lua`;
  - src: `systems/loot.lua`, `game/inventory.lua` and `game/attachments.lua`;
  - `tools/extract_rooms.py`.

**What I ran in the original** (1.0 served locally, headless Edge, touch, Novice, midday):
- I gave four guns with Spawn_drop and looked at their Ground-list icons: `SCR/orig/guns_inv.png`, `SCR/orig/guns_inv_crop.png`.
- I fired 48 loot points in a grid with the game's own Trigger_spawn: `SCR/orig/lootgrid_9_1_3_13.png`.
- I fired 60 points each of types 1, 3, 9, 13, 19, 23 and 24 and counted what landed (log in the run output, numbers in finding 1).
- I read `SEED_pubg` in a normal run.
- I dropped the same 14 items as in the remake: `SCR/orig/ground_row.png`.
- I dropped knives and tools and opened the board: `SCR/orig/tools_inv.png`.
- I cut every item object's frames with `cut.py` (`SCR/cut/`) and compared them pixel by pixel with the remake's `Assets/Icons` and `Assets/Icons/ground`.

**What I ran in the remake** (`--world 7`, Novice, god):
- The start: the `shot_900` capture.
- The same 14 items dropped: `SCR/remake/ground_row.png`.
- A locker stocked by `loot.stock` and opened: `SCR/remake/locker_board.png`.
- The same tools dropped and the board opened: `SCR/remake/tools_board.png`.

**Side by sides:**
- `SCR/sbs_ground_row.png`: the ground sprites, the original above.
- `SCR/ground_diff_11.png`: the 11 ground sprites with different art, the original on the left.
- `SCR/sbs_tools_ground_list.png`: the Ground list rows, the original on the left.
- `SCR/rip_icons_4.png` beside `SCR/orig/guns_inv_crop.png`: the four weapon icons.
- `SCR/shroom_cmp.png`: item 97's icon.

**The biggest gaps.**
- **The remake read the wrong half of Trigger_spawn.**
  - 1.0's main menu sets `SEED_pubg = 1` for every normal run (Menu_Events 3.2.3, 3.2.6, 3.2.8.2), and nothing ever sets it back to 0.
  - So the live loot lists are Trigger_spawn's *first* branch (14.1.4.1). 27.1 called that branch the battle-royale mode's.
  - The else branch (14.1.4.2) that `tools/extract_rooms.py` and `loot_tables.lua`'s gun shop were read from never runs.
  - Checked live: SEED_pubg was 1; type 9 and type 1 points dropped 60 of 60; type 13 dropped 91 objects from 60 points (gun plus ammo pairs); types 23 and 24 dropped nothing.
- **The loot tables are still invented.** Progress.md 10 and 16.2 say the original's tables "are not recoverable". They are: every list and every chance is in 14.1.4.1.
  - The original drops **one** thing per loot point, from a short list for that point's type. The chance is 100% for most types and 37 to 75% for a few.
  - The remake draws 1 to 3 times per piece from ten invented tables. A house holds 1.4x to 4x what the original's does: the farmhouse 1.52 against 0.38, the bar 5.9 against 2.4, the school 5.7 against 3.4.
  - The items found differ too: in the original no building drops canned food, fruit or sodas.
- **The original already has lootable car trunks** (12.13.3.1.2): a 50% chance of one thing from one of three per-car lists, with "I found something in this trunk." The remake built its own, with invented tables, and gave trucks and the UAZ trunks the original does not.
- **The camps' boxes and the humvee crash** have fixed or typed contents in the original: an SVD with ammo, an M60, the Engraved Colt, a Groza with a pan. In the remake every camp has a crate on the remake's `military` table.
- **Eleven items are missing** because the item index is keyed by file name and their icons share a name with another gun or vest: Colt 1911, PM, PB, UMP-45, Vector, AN-94, M16A2, Mare's Leg, Kevlar vest, Soviet vest, Tortilla backpack. Their icons are in the rip. Ten of them can be found in 1.0; the Mare's Leg is never spawned.
- **Which gun takes which attachment is in the events** (8.10.3.64 to .108), gun by gun. Examples:
  - The AUG takes no scope.
  - The AKM takes no silencer.
  - The MP5k takes the ACOG, not a silencer.
  - The shotguns, Mosin, SKS and Repeater take the bandolier, not the Magpull.

  The remake's table follows the real-world gun families instead.
- **Still to do, as inventory.md listed: about 40 usable items do nothing in the remake.** Some medical items work differently:
  - The IV kit *draws* 50 hp of blood into a blood bag.
  - Adrenaline sets health to 100 and gives 10 s of speed.
  - Tetracycline is always taken, and gives 300 s of immunity.
  - A bandage is refused when you are not bleeding.
- **Found guns differ:** in the original they are at 100% with no rounds, or (one time in two) 0 to N rounds. In the remake they come with a full magazine at a rolled condition.
- **Ground sprites:** 11 have different art in the remake (it matches what character.md found for the worn sheets): the tactical helmet is a yellow hard hat, plus the NVG, the riot helmet, both gas masks, the M60, the Benelli, the Beretta, the 5.45 box and item 97.

**What is kept, and why.**
- **Loot inside furniture and car trunks**, opened from a tab: "Lootable vehicle trunks. appropriate container scales; boxes, wardrobe etc can just be copied from interior sprites". Only what is *in* them goes back to the original's.
- **Every item permanent**, so no cull of loose items: "I want every item in the world to be permanent, and items that have pockets remember what was in them when they got dropped".
- **The campfire kit and starter kit recipes, and lighting the fire only with the starter kit**: Stage 24 and Stage 26.
- **Weapons in the backpack at 1x2 or 1x3**: Stage 20.

**Overlaps with other surveys.** The rows in inventory.md (B4, B7, B9, B10, D3, G1 to G4, H1 to H3, I1 to I3, K1), combat.md (#5, #8, #47) and zombies.md (#32) are cross-referenced, not repeated. They are re-checked against today's tree: 28.2 since made the cooked meats and raw meat identical.

**Outside this area, for the caller.** The same menu events set `SEED_morezeds = 1` for every normal run. Whoever owns zombies and world should check what that switches on.

## Findings

Verdicts: make identical 38, keep (user asked) 4, unclear 6

### 1. The live loot lists are Trigger_spawn's first branch, not the else branch the remake read

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `tools/extract_rooms.py` (TYPE_PIECE comment), `game/data/loot_tables.lua` (gunshop), `game/data/buildings.lua` (header), `Progress.md` 10 and 16.2.14

**Original:**
- The main menu's `menu_draw_main` (Menu_Events 3.2.3) sets `SEED_RUN = 0`, `SEED_morezeds = 1` and `SEED_pubg = 1`. `menu_draw_new_game` (3.2.6) and the new-game list (3.2.8.2, 3.9, 3.9.3) do the same.
- `SEED_pubg` is never set to anything but 1, and `globals.txt` shows its initial value is 0.
- Trigger_spawn (14.1.4) runs `14.1.4.1` when `SEED_pubg = 1`, and the else branch `14.1.4.2` only otherwise.

Checked live: after New game, Novice, Start, `ORIG.global('SEED_pubg')` was 1. Then 60 loot points of each type were fired through the game's own `Trigger_spawn`:

| Type | Result | Matches |
|---|---|---|
| 1 | 60 of 60 dropped | first branch; the else branch would give 80% |
| 3 | 38 of 60 empty | first branch predicts 62.5% empty |
| 9 | 60 of 60 dropped | first branch; the else branch would give 2/3 |
| 13 | 91 objects from 60 points | first branch's gun-plus-ammo pairs |
| 19 | 60 of 60 dropped | |
| 23 and 24 | nothing | the first branch has no case for them |

Screenshot: `SCR/orig/lootgrid_9_1_3_13.png`.

**Remake:**
- `tools/extract_rooms.py` TYPE_PIECE: "The lists are the normal game's, `Trigger_spawn`'s else branch (14.1.4.2): its first branch is the battle-royale mode's, SEED_pubg."
- `loot_tables.lua` gunshop: "what the original's loot type 19 drops ... one of eighteen: civilian guns, a scope, a katana". That is the else branch's list; the live list has 15 entries and no Desert Eagle, katana or Vector.
- Progress.md 10 and 16.2.14 say the tables "are not recoverable" because the call sites read `?op7`. The current dump has every argument.

### 2. Loot lies on the floor in the original; the remake keeps it in furniture and trunks

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Lootable vehicle trunks. appropriate container scales; boxes, wardrobe etc can just be copied from interior sprites." (Stage 23); "Reduce the range for opening containers and increase the size of the containers tabs." (Stage 26)
- **Files:** `game/data/containers.lua`, `game/data/trunks.lua`, `game/src/game/container.lua`

**Original:** Each loot point drops its item with `Spawn_drop` at the point, onto the `items_on_ground` layer, and destroys the point (14.1.4.1.1.23). The item shows on the floor with its name over it when you are near (`SCR/orig/lootgrid_9_1_3_13.png`). The only containers are the camps' and the bunker's `bunker_lootbox`: you walk into one and it drops its items around it (3.1.10). Car trunks also drop their one item on the ground (12.13.3.1.2).

**Remake:** Each loot point is a piece of furniture (27.1.3), and each car has a trunk container (23.4). The loot stays inside until it is taken from the piece's NEARBY tab (`SCR/remake/locker_board.png`). Findings 3 to 11 are about what goes inside.

### 3. One thing per loot point, at the type's own chance, not one to three draws

- **Verdict:** make identical  **Effort:** S  **User's ask:** none. The containers are asked (finding 2); how much they hold is not.
- **Files:** `game/data/containers.lua` (`rolls`), `game/data/trunks.lua` (`rolls`), `game/src/game/systems/loot.lua` (`loot.stock`)

**Original:** One `Spawn_drop` per point (14.1.4.1.1.N). Types 2, 4, 13 and 16 can drop a pair, a gun with its box of rounds. The chance of a drop by type:

| Chance | Types |
|---|---|
| 100% | 1, 5, 7, 8, 9, 11, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22 |
| 75% | 10 |
| 50% | 2, 4, 6 |
| 37.5% | 3 (half the time `choose(1,2)`, then one of the four is item 351, which Spawn_drop does not know) |
| 1% | 17 |
| 0% | 23, 24 |

Expected items per building, counting a gun and its ammo as two:

| Building | Original | Remake |
|---|---|---|
| Brown house | 1.00 | 1.39 |
| Farmhouse (red) | 0.38 | 1.52 |
| Green or yellow house | 1.38 | 2.91 |
| Piano house, church | 2.75 | 4.99 |
| School | 3.38 | 5.68 |
| Military barrack | 4.0 | 5.4 |
| HQ | 4.0 | 6.4 |
| Hospital | 2.5 | 4.7 |
| Bar | 2.4 | 5.9 |

**Remake:**
- Every piece draws `math.random(rolls)` times against a table with an `empty` weight (`loot.stock`). Crate 1 to 2; cupboard, kitchen, locker and wardrobe 1 to 3 (`containers.lua`). A car trunk 1 to 2 and a van 1 to 3 (`trunks.lua`).
- The chance of something per draw is 50 to 79% (house 69%, kitchen 79%, military 67%, clothes 76%, gas station 70%, hospital 59%, car 64%, police car 53%, convoy 63%, gunshop 50%).
- A stocked locker gave four stacks: a Glock, 12 gauge x10, 12 gauge x4 and 5.45 x22 (`SCR/remake/locker_board.png`).

### 4. What each loot type holds: 22 short typed lists, not 10 invented tables

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/loot_tables.lua`, `tools/extract_rooms.py` (TYPE_PIECE), `game/data/buildings.lua` (`table =` per spot)

**Original:** The lists from 14.1.4.1, with English names from l_eng_items. With no unlocks earned, the Smoke grenade spawns as an F1 grenade, the Lighter as Matches and the Laser Sight as an RDS scope (finding 21).

| Type | Where | What |
|---|---|---|
| T1 | houses, hostel, church, school, police, fire station, bar, city houses | one of 20: Molotov x2, Scout book x2, Sewing kit x2, Sharpening stone, Ruffe, Epoxy, Battery, Rice, both fishing rods, Duct tape, Kvas, Bandage, Flare, Hunter and Butcher knife, Smoke grenade |
| T2 | deer stand | raw steak + bear trap / rabbit meat + 12ga / sawn-off IZH + 12ga / tactical helmet |
| T3 | red house and the other houses | Shirt (351, nothing) / Jacket / Orel jacket / Tracksuit jacket |
| T4 | yard, piano, church, bus, red brick | Battery / Crowbar / PM + 9x18 / Canvas + Wood piles / Tactical helmet / Assault vest / Down jacket / Desert Eagle + .357 |
| T5 | fire station | one of 27 work kit, Colt and FNX, Magnum, Fire axe, Gasoline, Car toolbox |
| T6 | hospital | one of 10: scrubs x4, Sharpening stone, Sewing kit, Cleaning kit, Fishing rod, Lighter, Bandage |
| T7 | garages, bus | one of 58: tools, clothes, .45, .357, 12ga, sticks, wood, crossbow, Hacksaw, Gasoline, Toolbox |
| T8 | army tent, barrack | one of 24: .45, 5.56, 9mm, gas mask, beret, FNX, Colt, Glock, Army knife, Barbed wire, Tactical bacon |
| T9 | pillbox, barrack, HQ, gun shop | one of 37: L85, MP5k, UMP, M4, AUG, Glock, FNX, Colt, armour, vests, NVG, Claymore, Bandolier, RDS, ACOG, .308, F1 |
| T10 | sheds | one of 35: farm tools, clothes, seeds x3, Bear trap, Barbed wire, sticks, wood |
| T11 | police station | one of 40: police and civilian guns (Benelli, Mosin, SKS, AKS-74u, Beretta, Sporter, MP5k, Glock, UMP, PM), their ammo, Complex M armour, Press vest, Orel kit |
| T12 | school | one of 46: School backpack x3, clothes, Civilian tent, a few guns |
| T13 | city houses | Colt + .45 / Mosin + 7.62 / Repeater + .357 / Benelli + 12ga / Gasoline / Hunting backpack / Moto helmet / Press vest / Assault vest |
| T14 | supermarket | one of 33: Duct tape x3, Cleaning kit x2, Molotov, knives, fishing rods, Protector case, Beer, Energy drink, Water bottle, Whiskey |
| T15 | heli crash | one of 48 military |
| T16 | gas station, fuel canopy | Gasoline / Toolbox / 40mm + VOG / Magnum + .357 / Tactical helmet / Assault vest / Down jacket / SKS + 7.62x39 |
| T17 | pillbox | 1-in-100 Nuko Cola |
| T18 | humvee crash | one of 43 military |
| T19 | gun shop | finding 8 |
| T20 | barrack 2, east tent | one of 22: 7.62, 5.45, 9x18, PM, Laser Sight, launcher rounds |
| T21 | HQ, barrack 2 | one of 44: SVD x2, M16A2, Vector, AKM, AK-74, AKS-74u, SKS, Saiga, Bizon, PB, PM, Groza, PSO |
| T22 | hospital | one of 8: Bandage x2, Blood bag, Saline bag, Vitamins, Adrenaline, Tetracycline, IV kit |

**No live type drops canned beans, canned tuna, fruit, vegetables, Pipsi, Spite or Nota-Cola.** Those come from the tents camp (3.5.5.1.32.1), berry bushes and the tutorial.

**Remake:** Ten tables, each of 9 to 66 rows: house, kitchen, military, clothes, gas_station, hospital, car, police_car, convoy and gunshop.
- Pieces are mapped by TYPE_PIECE: 1 and 12 to cupboard/house, 3 and 13 to wardrobe/clothes, 4 and 10 to crate/house, 5, 8, 9, 20 and 21 to locker/military, and so on.
- The tables draw on every described item: kitchens stock beans, tuna, apples, sodas and fish fillets, and wardrobes stock 33 garments and four backpacks.
- The house table's knives, sword, katana, Sporter and so on are the remake's own choices, as the file says ("Curiosities. Someone owned these").

### 5. The bar's fourth point and the HQ's fifth (types 23, 24) give nothing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/buildings.lua` (b_bar kitchen spot, b_military_shtab crate spot), `tools/extract_rooms.py`

**Original:** 14.1.4.1 has no case for types 23 and 24. The point is created and destroyed with nothing dropped: 60 of 60 empty each, live.

**Remake:** The bar's type-23 point is a `kitchen` (kitchen table, 1 to 3 draws, about 1.6 stacks). The HQ's type-24 point is a crate on the `military` table (about 1.0 stack).

### 6. The deer stand's loot point is missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/buildings.lua`, `game/data/maps/prefabs/camps.lua` (hunters)

**Original:** `b_deer_stand` spawns a type-2 loot point 15 px below its foot (3.5.7.1.6.1.1). Half the time it drops raw steak and a bear trap, rabbit meat and 12ga, a sawn-off IZH and 12ga, or a tactical helmet.

**Remake:** The deer stand has no entry in `buildings.lua` and no container. The hunters' camp puts a `military` crate beside it instead (finding 10).

### 7. The pillbox's one-in-a-hundred Nuko Cola (type 17)

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `tools/extract_rooms.py` (TYPE_PIECE 17), `game/data/buildings.lua` (b_dot)

**Original:** The pillbox's third point (3.5.7.1.6.2.1.1) drops a Nuko Cola when `round(random(1,100)) = 55`, on the floor.

**Remake:** No piece: "a piece of furniture would make a certainty". A 1% drop is possible without one, as a drop on the floor or a piece whose table is `empty` 99 to 1.

### 8. The gun shop's rack: always one of 15

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/loot_tables.lua` (gunshop), `game/data/buildings.lua` (b_gunshop)

**Original:** Type 19 (14.1.4.1.1.21) always drops one of: Bandolier, FNX, Colt 1911, Magnum, IZH-43, Benelli, Mosin, SKS, Sporter 22, Repeater, Glock, MAC-10, Saiga 12k, PU scope or PM. There is no Desert Eagle, katana or Vector. The rack's other two points are type 9.

**Remake:** `gunshop` has `empty = 18` against 18 weight and 14 rows, including a Desert Eagle and a katana, and it is drawn 1 to 3 times. 27.1's review measured about 29% of racks empty and some with two or three guns.

### 9. Car trunks: the original has them, with a 50% chance of one thing from three lists

- **Verdict:** make identical  **Effort:** M  **User's ask:** "Lootable vehicle trunks." (Stage 23). The original's own trunks meet it; the ask says nothing about contents or rates.
- **Files:** `game/data/trunks.lua`, `game/data/loot_tables.lua` (car, police_car, convoy), `game/src/game/systems/loot.lua`

**Original:** Every parked car map element spawns a `car_loot_point` while the element's searched flag is 0 (3.5.7.1.5.N.1). Interacting (12.13.3.1.2) does the following:
- It destroys the point, plays `open` at -5 dB and sets the car to frame 1 (lid up).
- It rolls `trunk_rng = random(100)`. Below 50, it drops one item at the point's image point 3 and says "I found something in this trunk." in yellow. Otherwise it says "There is nothing in this trunk." in white.

The list depends on the point's frame:
- **Frames 0, 3, 5** (the regular cars, side and nose-in, and the hatchbacks): one of 25. Molotov, red moto helmet, Cleaning kit x2, Sewing kit, Water bottle, Protector case, Duct tape x3, Ruffe, MRE, Tactical helmet, Matches, Flare, Warm hat, Cap, Jeans, Tracksuit pants, T-shirt, Shirt, Pipewrench, Baseball bat, Papers, Butcher knife.
- **Frame 1** (the vans): one of 41. Helmets and clothes, Rice, Whiskey, .357, 12ga, Matches, Flare, Battery, Magnum, IZH-43, Benelli, Taloon backpack, Pipewrench, Shovel, Hatchet, Bat, Crowbar, Crossbow, Sporter, Composite arrows, Duct tape, Burlap sack, Rope, Wood piles, .22 x2, Crossbody bag.
- **Frames 2, 4** (the police cars): one of 27. Cap, Balaclava x2, Jeans x2, Orel pants, .45 x2, .357 x2, 12ga x2, Shirt x2, Jacket x2, PM, Bizon, Pipewrench, Police hat, Rope, 9x18 x2, Benelli, Flare, Orel jacket.

The truck, the UAZ and the BTR get no loot point.

**Remake:**
- `trunks.lua` gives every car, van, police car, truck and UAZ a container stocked from `car`, `police_car` or `convoy`. Those are three invented tables of 15 to 18 rows at 53 to 64% per draw, drawn 1 to 2 times, or 1 to 3 for vans and the truck.
- The trunk refills (finding 15).
- There is no line over the head. The trucks' and UAZ's trunks are finding 47.

### 10. The camps' boxes hold fixed things; every remake camp holds a military crate

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/maps/prefabs/camps.lua`, `game/data/loot_tables.lua`

**Original:** Each secret location (3.5.5.1.33 to .48) places a `bunker_lootbox` with a fixed kind. Walking into it opens it (3.1.10, OnCollision) and drops:

| Camp | Lootbox kind | Drops |
|---|---|---|
| army (26) | 110 | one of M4, AKM, AK-74, AKS-74u, L85, MP5k; two boxes of matching ammo; one of Canteen, Landmine, F1, Blood bag, Claymore |
| tank (27) | 109 | M60 + 2x 7.62; a K6-3 helmet lies on the ground beside it (3.5.5.1.34.1) |
| radio (28) | 103 | M60; a pitchfork, duct tape, matches and a battery lie on the ground (3.5.5.1.35.1) |
| drone (45) | 103 | M60 |
| wreck (29) | 108 | 2x Nuko Cola |
| typhoon (40) | 107 | Civilian tent; an AN-94 and 2x 5.45 lie on the ground (3.5.5.1.37.1) |
| odd car (42) | 106 | Blood bag, Tetracycline, Vitamins |
| castle (43) | 104, 105 | Helm and Sword |
| tower (44) | 104 | Helm |
| strange tree (46) | 101 | Engraved Colt |
| drop (47) | 102 | Pan, Groza, 9x39 |
| stalkers (48) | 111 | SVD, Bandage, Matches, 7.62 x20 |
| ruins (49) | 112 | M60 + 3x 7.62 x20 |
| predator (60) | 113 | Flare gun |
| chigurh (61) | 114 | Silenced Remington + 12ga |
| paratrooper (62) | 115 | Map Notes + Tactical helmet |

The camps with no box:
- The tents camp (24) lights a fireplace and drops 8 things around it (3.5.5.1.32.1): three from a hunter's list, three from a police list and two food.
- The stones camp has berry bushes.
- The hunters' camp is its hut's one-time stash (3.5.7.1.6.32.1.1: one of six guns, a kit item, a steak; already in Progress 27.1 as found and not fixed) and the deer stand's point (finding 6).

**Remake:** Every camp prefab, all 20, has `{ sprite = "crate", table = "military" }`. That is the same 66-row table every time, drawn 1 to 2 times.

### 11. The humvee crash: three typed drops on the ground, one crash on the first map

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/maps/prefabs/crash.lua`, `game/data/places.lua` (crash count)

**Original:**
- `hammer_crash` spawns three loot points of type 18 at its image points 1 to 3 (3.5.5.1.N.10). Each always drops one of 43 military things (finding 4).
- `generate_helicrashes` (3.14.1) makes 0 helicopter crashes and 1 humvee crash on the first map (`CurrentLevel = 0`). More appear on later levels.

**Remake:** One crate on the `convoy` table (1 to 2 draws). The seed 7 log reads `crash x2`. There are no helicopter crashes, as on the original's first map.

### 12. Loot the original does not have at the gas station and the cafe

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/maps/prefabs/gas_station.lua`, `game/data/maps/prefabs/cafe.lua`

**Original:** The gas station's room and the fuel canopy have one type-16 point each (3.5.7.1.6.33, .34). The bar has its four points (3, 1, 1, 23). Nothing lies on the ground outside either.

**Remake:**
- The gas station prefab adds "a crate of stock outside the shop" on the gas_station table and a water bottle on the forecourt.
- The cafe prefab puts 2 kvas and an apple "on the ground out front".

### 13. The makings of a fire at the spawn and in every home's yard

- **Verdict:** unclear  **Effort:** S  **User's ask:** "To build a campfire the player needs 1 wood, 1 stick. That makes a campfire kit. Then with 1 stick and 1 newspaper, the player can make a campfire starter kit." (Stage 24). The recipes are asked; where the ingredients come from is not.
- **Files:** `game/data/places.lua` (`makings`, `hearth`)

**Original:** No item is placed at the spawn (`SCR/orig/start.png`) or in yards. Wood sticks drop from types 7 and 10 (garages, sheds), wood piles from 4, 7 and 10, and papers only from car trunks (finding 9). The live lists have no papers in any building type.

**Remake:** 25.2 puts a wood pile, 2 sticks and a newspaper beside the spawn and in every home's front yard, "which the houses' tables roll too rarely to count on". The user's recipe needs papers, which the original's live buildings never drop. Whether the yard piles stay is the user's call.

### 14. Found guns: 100% and empty, or loaded with 0 to N rounds one time in two

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/inventory.lua` (`clamp_ammo`, `new_item`), `game/src/game/systems/loot.lua` (`place`)

**Original:** Spawn_drop sets every gun's condition `var#6` to `choose(100,...)` (100) and `var#5 = -1`. Then `if choose(1,2) = 1: var#3 = round(random(N))` (14.2.121 to .159). N per gun:

| N | Guns |
|---|---|
| 2 | IZH-43, Sawed-off IZH |
| 5 | Mosin, Sawed-off Mosin |
| 6 | Magnum, Benelli, Silenced Remington |
| 7 | Colt, Repeater, Engraved Colt, Mare's Leg |
| 8 | Saiga, PM, PB |
| 9 | Desert Eagle |
| 10 | SKS, SVD, Beretta, SV-98 |
| 15 | FNX, Glock, AUG |
| 20 | FN-CAL, MAC-10, VSS, M60, M16A2, Vector |
| 30 | M4, AKM, AK-74, AKS-74u, L85, Sporter, MP5k, UMP, Bizon, Groza, AN-94 |
| 40 | RPK |

Otherwise there are 0 rounds. Live: the VSS lay with "Ammo:3" (`SCR/orig/guns_inv.png`) and the Glock with no count, so 0 (`SCR/orig/tools_inv.png`).

**Remake:**
- `inventory.new_item` calls `clamp_ammo(name, nil)`, which returns the full magazine, so every gun found or dropped is full. The stocked locker's Glock was "50% 17/17" (`SCR/remake/locker_board.png`).
- Condition is inventory.md B7: rolled `{25,25,50,50,75,100}`.

### 15. Spawn quantities, charges and fill levels

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/loot_tables.lua` (min, max), `game/src/game/systems/loot.lua`, `game/data/items.lua`

**Original:** Spawn_drop sets the count, charge, fill or condition on spawn (14.2):

| Kind | Items and values |
|---|---|
| Ammo, a box | 5.56 15-25; .45 10-20; 7.62 5-15; 7.62x39 15-25; 5.45 15-25; .357 10-15; 12ga 5-10; 9mm 10-25; .22 one of 10/15/20/25; 9x18 one of 10/15/20; 9x39 one of 15/20/25; .308 50; 40mm and VOG 1-2 |
| Other counts | handmade arrows 1-5; composite arrows 2-5; flare 1-3; matches 1-5; wood sticks 1-5; cigarettes 1-3 |
| Charge and fill | batteries 10-100 (charge); headlamp 0/15/30 (charge); radio 50; water bottle and canteen 0/25/50/75/100 (fill); fertilizer 75/100; MRE 100 (two halves) |
| Condition | knives 25/50/75/100; fishing rods 50/75/100 |

Seen live: "Battery 96%", "Canteen 25%", "Flare x2", "Hunter knife 50%", "Matches x2", "Water bottle 75%" (`SCR/orig/tools_inv.png`).

**Remake:**
- Table counts are invented: 12ga 4-12 (military), 4-8 (car); 5.45 10-30; 9mm 12-36; 7.62 10-30; .308 8-24; .357 6-18; .22 8-24; arrows 3-9.
- Batteries, the canteen, the bottle, knives and rods carry no charge, fill or condition, and flares and matches come one at a time. The same items dropped read "Batteries", "Canteen", "Flare", "Knife hunter", "Matches", "Waterbottle" with no numbers (`SCR/remake/tools_board.png`; side by side `SCR/sbs_tools_ground_list.png`).

### 16. Loot respawn: every 1500 s, re-rolled when the place is more than 1800 px away

- **Verdict:** make identical  **Effort:** M  **User's ask:** none for the timing. The cull is finding 17.
- **Files:** `game/data/config.lua` (`loot.respawn_minutes`, `respawn_distance`, `sweep_*`), `game/src/game/systems/loot.lua`

**Original:**
- `timers_sprite` repeats a `Check_loot` timer every `LOOT_RESPAWN_TIME = 1500` s (2.11; 1.0's value, not the 1.2 mod's 1480). It calls `Global_respawn` (14.1.2).
- Every location (`mapgen`) more than `respawn_radius2 = 1800` px from the player has its elements' searched flags cleared (14.1.2.7, `Arr[t525]` column 5 to 0).
- So when the camera next comes within 1800 px and the location is rebuilt (3.5.2), every building's loot point, and every car's trunk point, fires again from scratch.
- Heli crash points (type 15) more than 1000 px away re-fire too (14.1.2.6).

**Remake:**
- Each container tops up its free slots (`loot.update`) once 1480 in-game minutes have passed since it was stocked. That is 2220 s at 1.5 s a minute.
- The container must be more than 500 px from the player, and the camera must have moved 400 px since the last sweep.
- `config.lua` explains the 500 by the 1600x1200 test map. The world is 26x16 lots now (27.2), so 1800 would fit.

### 17. Loose items far away are destroyed at each respawn in the original

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "I want every item in the world to be permanent, and items that have pockets remember what was in them when they got dropped." (Stage 6)
- **Files:** `game/src/game/systems/loot.lua` (header)

**Original:** `Global_respawn` destroys every item 1500 px or more from the player (14.1.2.3 to .5). It also destroys the inventory arrays of bags and garments that hold things. It spares items in a deployed tent, a stash, at the heli counters, or flagged var#2 = 88.

**Remake:** Nothing is culled (Progress 6.7, 10).

### 18. Berry bushes: picked for berries, refilled on respawn; water pumps hold water

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/items.lua` (berries comment), `game/data/maps/prefabs/camps.lua`, `game/src/game/interaction.lua`

**Original:**
- A `berry_bush` at frame 1 can be searched (12.13.3.1.3): 3 s crouched, the `bushsearch` sound, the bush to frame 0, and one berry of its colour into the inventory. Red gives Cranberry (27), yellow Cloudberry (73), blue Bilberry (74), black Elderberry (114).
- Fertilizer used at a bare bush regrows it (8.10.3.107.2).
- Each respawn gives every bush 500 px or more away a random frame 0 or 1 (14.1.2.1).
- Each respawn sets every pump 500 px or more away to `choose(0,25,0,25,50,75,100,150,200)` water (14.1.2.2). Bottles and canteens are filled from it (8.10.5.5, .6).

**Remake:** `berry_bush` is scenery. items.lua says "there is no foraging here, so they are found picked", and berries come from kitchen rows instead. The pump is scenery too.

### 19. Rare items come from camps, crashes and the bunker, not from every locker

- **Verdict:** make identical  **Effort:** S  **User's ask:** none. This follows from findings 4 and 10.
- **Files:** `game/data/loot_tables.lua` (military, house)

**Original:**

| Item | Where it comes from |
|---|---|
| Engraved Colt | the strange-tree camp (101), bunker 51 |
| M60 | tank, radio, drone and ruins camps; bunker |
| VSS, SV-98 | only bunker lootbox 52 |
| Katana | only bunker 32 and 52 (the type-19 entry is in the dead branch) |
| Sword, Helm | the castle and tower camps; bunker |
| Groza | type 21, the drop camp, bunker |
| AN-94 | the typhoon camp's ground drop (3.5.5.1.37.1), bunker 52 |
| Desert Eagle | type 4's eighth case (1/16 of type-4 points), bunker 51 |
| Mountain backpack | type 15 (heli crash), bunker |
| Silencers, AK grip, rail grip, long scope | only the bunker (51, 53) |

**Remake:**
- `military` holds every one of these at weight 1, except the katana and sword, which are in `house` at weight 1. That includes the VSS, SV-98, M60, Engraved Colt, every scope, silencer and grip, and the Mountain backpack sits in `clothes`.
- So every locker in the world can produce them, at roughly the same odds as an ordinary rifle.

### 20. Eleven items are missing: they share an icon name with another item

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `tools/build.py`, `game/data/generated/items_index.lua`, `game/data/items.lua`, `game/data/weapons.lua`, `game/data/weapon_slots.lua`

**Original:** These are separate object types that draw on another's sheet, each with its own drop id, name and icon:

| Id | Item | Drawn on |
|---|---|---|
| 152 | Colt 1911 | fnx_pistol |
| 181 | PM | fnx_pistol |
| 182 | PB | fnx_pistol |
| 173 | UMP-45 | mp5_smg |
| 192 | Vector | mp5_smg |
| 188 | AN-94 | ak74_rifle |
| 191 | M16A2 | m4_rifle |
| 180 | Mare's Leg | sawed_mosin |
| 404 | Kevlar vest | razgruz_big (armour 10) |
| 405 | Soviet vest | razgruz_big (armour 8) |
| 457 | Tortilla backpack | hunter_backpack |

objects.txt t71, t140, t139, t117, t149, t143, t148, t126, t119, t134, t457.

Where they come from:
- Eight are in the live loot lists (finding 4): Colt 1911, PM, PB, UMP-45, Vector, M16A2, Soviet vest, Tortilla backpack.
- The AN-94 lies at the typhoon camp and is in bunker 52.
- The Kevlar vest is in bunker 53 and the airdrop.
- Nothing in 1.0 ever spawns the Mare's Leg (no `Spawn_drop` of 180 anywhere), so it is identical in play.

**Remake:**
- `items_index.lua` is keyed by the file name after the id. `152_fnx_pistol.png` and `151_fnx_pistol.png` both become `fnx_pistol`, so one wins.
- All 11 icons are in `Assets/Icons` (152, 173, 180, 181, 182, 188, 191, 192, 404, 405, 457), and their art differs. For example `152_fnx_pistol` is 45x30 and `151_fnx_pistol` is 41x31; `404_razgruz_big` is 28x26 and `403` is 26x29.
- combat.md #47 lists the eight guns from the combat side.

### 21. Items gated behind unlocks: plaster, lighter, smoke grenade, laser sight, keycard

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua`

**Original:**
- Drop ids 118, 119, 124, 125 and 126 spawn the Adhesive plaster, Smoke grenade, Laser Sight, Lighter and Officer's keycard only if `item_unlock_*` = 1 (14.2.112 to .120).
- Those are set only by the happy ending with characters 4 to 19 (10.3.1.1 to .5).
- On a fresh profile they spawn a Bandage, F1 grenade, RDS scope, Matches and Army knife (at 50%) instead.

**Remake:** None of the five exists. Characters and the ending's unlocks are outside the remake. Copying the lists in finding 4 with the fallbacks matches a fresh 1.0 profile exactly.

### 22. Item names

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (inventory.md K1)
- **Files:** `game/src/game/item.lua`, `game/data/items.lua`

**Original:** l_eng_items.xml names. Seen live: "Glock", "Battery", "Canteen", "Hunter knife", "Water bottle", "Bizon", "Groza", "Saiga 12k", "VSS" (`SCR/orig/tools_inv.png`, `SCR/orig/guns_inv.png`).

**Remake:** The file name made readable, for example "Glock pistol", "Batteries", "Knife hunter", "Waterbottle", "Ammo 12cal", "Ammo 5x45" (`SCR/remake/tools_board.png`, `SCR/remake/locker_board.png`). The full list is in inventory.md K1. It is unchanged since that survey.

### 23. Item 97 ("shroom")

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `Assets/Icons/97_shroom.png`, `Assets/Icons/ground/shroom.png`, `game/data/items.lua`

**Original:**
- Item 97 is named "Canned tuna" in l_eng_items.xml.
- Its inventory icon (t6 frame 97) is an opened can, 30x30, and its ground sprite is a small brown thing (`SCR/shroom_cmp.png`, `SCR/ground_diff_11.png`).
- Runners drop it (zombies.md #32).
- Eating it (8.10.3.84) plays EatingSoft, waits 1 s, **clears LocalStorage, deletes the `_C2SaveStates` database and reloads the page**: it wipes your progress.

**Remake:** A purple mushroom icon (8x8) and ground sprite, from another build's art. It is inert and never placed. Matching the art is easy. Whether to copy the save wipe is the user's call.

### 24. Eleven ground sprites are another build's art

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `Assets/Icons/ground/*.png`, `tools/build.py`

**Original:** Each item's `on_ground` (or `Default`) frame, cut with `cut.py` (`SCR/cut/`). Of the remake's 216 ground sprites:
- 147 are pixel-identical.
- 58 differ only by a 1 to 2 px transparent border. The trimmed art is identical, but the frame's anchor may shift by a pixel.
- 11 have different art:

| Item | Original 1.0 | Remake |
|---|---|---|
| Tactical helmet (helmet_hard) | dark tactical helmet | yellow hard hat |
| NVG | dark green helmet | goggles |
| Riot helmet | dark helmet with a grey visor | white helmet |
| K6-3 helmet | olive helmet, dark visor | grey visor |
| Combat Gasmask (helmet_army) | helmet with a mask's grey filter | plain dark helmet |
| GP-5 mask | side-on white mask with a green filter, 10x7 | front-on dark mask, 10x8 |
| M60 | flat-topped, 24x9 | different outline, 24x10 |
| Benelli (r670) | black | wood stock and fore-end |
| Beretta (amphibia) | long silencer down and right | silencer straight out |
| 5.45 box | box art differs | box art differs |
| item 97 | brown | purple mushroom |

Side by side: `SCR/ground_diff_11.png` (original on the left) and `SCR/sbs_ground_row.png` (the same 14 items dropped in each game).

**Remake:** These are the rip's `Assets/Icons/ground`. For these 11 the art is not 1.0's, as character.md #1 found for fourteen worn sheets. Which build they came from was not checked.

### 25. Stack sizes

- **Verdict:** make identical  **Effort:** S  **User's ask:** "items that have durability shouldn't stack" (Progress 17.4.14), which the original also does for gear (inventory.md B4)
- **Files:** `game/data/items.lua`

**Original:** Check_stack_size 8.12.7, as inventory.md B4 lists:

| Stack | Items |
|---|---|
| 3 | bandage, rags, the fruits and vegetables |
| 5 | flare, berries, wood sticks, arrows, 40mm, VOG |
| 10 | matches |
| 15 | 12ga |
| 20 | 7.62 |
| 30 | 5.56, 7.62x39, 5.45, .357, 9x39 |
| 40 | .45, 9mm, 9x18 |
| 50 | .22, .308 |
| 1 | everything else: tins, bottles, wood piles, papers, heatpack |

**Remake:** Unchanged since that survey: bandage 5, rags 4, fruit 6, berries 10, tins 3, sodas 4, wood piles 3, sticks 6, papers 5, heatpack 3, tetracycline 4, morphine 2, ammo 24 to 80, arrows 12.

### 26. Condition of what is found

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (inventory.md B7)
- **Files:** `game/data/config.lua` (`items.condition_roll`), `game/src/game/systems/loot.lua`, `game/data/items.lua`

**Original:**
- Every gun, melee weapon and garment spawns at 100% (14.2: `var#6 = choose(100,100,...)`). Only the raider jacket rolls `choose(25,25,50,50,75,100)`.
- Knives (25/50/75/100) and fishing rods (50/75/100) carry a condition too (finding 15).

**Remake:**
- `condition_roll = {25,25,50,50,75,100}` for everything that wears out. A locker's Glock came at 50% (`SCR/remake/locker_board.png`).
- Knives and rods have no condition.

### 27. Food and drink values

- **Verdict:** make identical  **Effort:** S  **User's ask:** "Speed boost from energy drinks." (Stage 23), which covers the drink's speed only
- **Files:** `game/data/items.lua`

**Original:** 8.10.3, 2 s each unless noted. Values are food/water.

| Item | Original |
|---|---|
| Beans | 30/5 |
| Tuna | 20/0 |
| Bacon | 35 |
| Rice | 65 (EatingCrunchy) |
| Tomato, apple, banana | 15/5 |
| Pipsi, Spite, Nota-Cola | 0/30 |
| Kvas | 0/50 |
| Beer | 0/30, +10 heat |
| Whiskey | 0/20, +40 heat |
| Cranberry | 15/15 |
| Cloudberry | 10/10, +30 s of doubled regeneration (var#33, 6.2.2.10.1) |
| Bilberry | 10/20 |
| Elderberry | 10/10, +2 hp |
| Zucchini | 20/20 |
| Bell pepper | 20/5 |
| Orange | 15/15 |
| MRE | 40/40, 3 s, two halves |
| Small fillet | 20/10, +5 hp, 3 s |
| Big fillet | 30/10, +10 hp, 3 s |
| Nuko Cola | water to 100, +60 s of doubled regeneration |
| Energy drink | 0/20, plus the 60 s boost |

**Remake:**

| Item | Remake |
|---|---|
| Beans | 40/0 |
| Tuna | 35/5 |
| Bacon | 45 |
| Rice | 30 |
| Tomato | 5/5 |
| Apple | 12/4 |
| Banana | 14/2 |
| Sodas and Nuko Cola | 4/20 |
| Kvas | 6/30 |
| Beer | 4/18 |
| Whiskey | inert |
| Berries, all four | 5/2 |
| Zucchini | 18/4 |
| Bell pepper | 5/3 |
| Orange | 10/8 |
| MRE | 50/10 once |
| Fillets | 18/0 and 30/0, no hp, 1.4 s and 1.8 s |
| Energy drink | 4/20 + 60 s |

Use times run 1.0 to 3.0 s. Cooked steak, cooked rabbit and raw meat are now the original's (28.2; see "Already identical").

### 28. Raw fish: eaten raw or cooked into fillets

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua` (fish_*), `game/src/game/campfire.lua`

**Original:**
- At a burning fireplace, a Herring or Ruffe cooks into a Small fish fillet and a Salmon or Perch into a Big one (8.10.3.78 to .81: 5 s, `meat_cook`, "I've cooked a steak."). Without a fire you get "I need an active fireplace for that."
- Eaten raw (8.10.5.17 to .20): 3 s, 20/10, and a 1-in-2 chance of falling sick.
- Fillets come only from cooking, and fish from fishing or type 1 (Ruffe).

**Remake:** The four fish are inert. Fillets are found ready to eat in kitchens.

### 29. Water bottle and canteen hold water

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (inventory.md G2)
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`

**Original:**
- They spawn 0 to 100% full.
- A sip takes 25: +12.5 water from the bottle, +25 from the canteen, 2 s, the `Whiskey` sound at -10 dB.
- An empty one is kept, with "Bottle empty.".
- Fill with water at a pump (8.10.3.47 and .48, 8.10.5.5 and .6).

**Remake:** Single use, +45 and +50, then gone. They stack 2 and 1.

### 30. Medical items

- **Verdict:** make identical  **Effort:** M  **User's ask:** "effects, like infection, which needs to be cured by tetracycline." (Stage 23). The cure stays. The survival survey covers the states themselves.
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua` (`inventory.use`)

**Original:** 8.10.3.

| Item | Original |
|---|---|
| Bandage | only while bleeding: 2 s, `bandage` -5 dB. Otherwise "I'm not bleeding." and it is kept (.18.2) |
| Rags | the same, 5 s |
| Adhesive plaster | 0.1 s |
| Adrenaline (the morphine icon) | health set to 100, 1 s, `adrenaline_use`, 10 s of speed (`Edrink` timer, `player_base.var#4 = 20`, the Adrenaline boost icon) (.39) |
| Blood bag | +50 hp, 6 s |
| Saline bag | +25 hp, 6 s |
| IV kit | needs no sickness, no bleeding and over 50 hp: 6 s, **50 hp taken** and a Blood bag made (`Check_space_inventory(-1, 19)`), "Blood transfusion completed." (.96) |
| Tetracycline | always taken, 3 s: clears sickness (var#35), 300 s of immunity to it (bool var#51, checked by raw meat and fish, 8.10.5.2.1) (.40) |
| Vitamins | 2 s, 60 s of doubled regeneration (var#33 = 1 makes 6.2.2.10.1 add the regeneration twice; inventory.md G3 called it immunity) |
| Heatpack | +25 heat, 3 s |

**Remake:**

| Item | Remake |
|---|---|
| Bandage | spent even when not bleeding (`inventory.use` sets `applied` for `stop_bleeding` without asking), 2.4 s |
| Rags | 3.2 s |
| Morphine | +35 hp, 1.8 s, no speed |
| Blood bag | +40, 4 s |
| IV kit | +45 hp and stops bleeding |
| Tetracycline | refused and kept with no infection, no immunity |
| Saline, vitamins | inert |
| Heatpack | +45, 1.4 s |

### 31. Matches and the lighter do not light fires in the remake

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "lighting a campfire should only be done by standing near the campfire and using the campfire lighter kit from the inventory so it doesn't need a button" (Stage 26); "Acting the campfire starter near the campfire turns it on." (Stage 24)
- **Files:** `game/data/items.lua` (matches: not described), `game/data/recipes.lua`

**Original:** Matches (8.10.3.24) and the lighter light a fireplace kit on the ground: 3 s, `matchstrike_2` or `lighter_0`. Otherwise "I need a fireplace kit on the ground to ignite.". They are also what cigarettes need.

**Remake:** The starter kit lights the fire. Matches are inert.

### 32. Light: headlamp, NVG, flares, flare gun; batteries and the radio

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/systems/daynight.lua`, `game/src/game/player.lua`

**Original:**
- A worn headlamp or NVG, switched on, drains 0.1 charge each 0.5 s, lights the night (`light_sprite`; the NVG a green tint) and goes out at 0 (8.14, 8.15). Batteries recharge it (8.10.3.22).
- A flare is held lit (100 s of fuel), then thrown with `flare_throw`. It keeps burning where it lands (8.10.5.4, 8.10.3.20).
- The flare gun fires a flare (8.10.3.90).
- The radio scans when it has batteries (8.18 `Radio_scan`).

**Remake:** A headlamp and NVG are worn hats with warmth and armour and no light. Flares, the flare gun, batteries and the radio are inert and unplaced (items.lua "NOT DESCRIBED").

### 33. Map Notes reveal the secret location

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua` (map_notes), `game/src/ui/map_screen.lua` (27.4)

**Original:** Using Map Notes (8.10.3.100) sets `secret_location_revealed = 1` and opens the notebook at its map tab (`Pad_last_tab = 4`). Map Notes come from the paratrooper camp's box (115), bunker 53 and runners' drops (zombies.md #32). The hospital list that has them is in the dead branch.

**Remake:** Inert and unplaced. The camps are on the map from the start or not; that part is for the world survey.

### 34. Gardening and fishing

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/items.lua`

**Original:**
- Seed packs (8.10.3.41 to .43) and a tomato, pepper or zucchini's second action (8.10.5.7 to .9) plant on grass with a digging tool in hand: 5 s, `planting_seeds`, a plant that grows (8.16).
- Fertilizer regrows a plant or a berry bush.
- The two rods fish at water: 4 s, a fish or nothing (8.10.3.73 and .74).

**Remake:** All inert and unplaced.

### 35. Mending and tools

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/data/recipes.lua`, `game/src/game/crafting.lua`

**Original:**
- A sewing kit, duct tape or epoxy onto a worn garment, helmet, vest or backpack patches it: 3 s, `taping` (12.10.17).
- A sharpening stone onto the melee weapon adds 25%: 5 s, "Weapon ruined." at 0 (8.10.3.97).
- A hacksaw onto a shotgun or rifle saws it off (8.10.3.70).
- Gasoline and the car toolbox refuel and repair a car: 6 s and 5 s (8.10.3.46, .52). The remake has no drivable cars (Progress out of scope), so those two would need vehicles first.

**Remake:** All inert. `repair` in items.lua means the cleaning kit on the gun in hand only.

### 36. Traps, explosives and throwables

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (combat.md #47)
- **Files:** `game/data/items.lua`, `game/src/game/build.lua`

**Original:**
- A landmine, claymore or barbed wire goes down through Building_mode (8.10.3.49, .86, .58).
- A bear trap is set in 3 s (.50).
- F1, smoke grenade and molotov are thrown (.21, .104, .89).
- Whiskey and a flare make a molotov (Check_craft).

**Remake:** All inert and unplaced. The remake's build mode exists for the campfire (24.2).

### 37. The civilian tent

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (inventory.md B10)
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`

**Original:** Worn in the backpack slot. Its sub-menu deploys and rolls it (12.10.15 and 16). Items left in a deployed tent are spared by the respawn cull (14.1.2.3).

**Remake:** Carried, inert, unplaced.

### 38. Cigarettes, the Scout book, the Protector case

- **Verdict:** unclear  **Effort:** M  **User's ask:** none. The Scout book's XP and the case's quests depend on perks and quests, which the remake has left out on its own call (Progress "Explicitly out of scope").
- **Files:** `game/data/items.lua`

**Original:**
- Cigarettes need a fire source (Check_fire_cigarets): 2 s, `Smoking_1` or `Smoking_2`, breath puffs, the `gui_smoke` icon, 60 s "Smoked" (8.10.3.105).
- The Scout book takes 10 s and gives +200 XP (.102).
- The Protector case is cracked open (Questbox_crack, .75).

**Remake:** All inert and unplaced.

### 39. Knives beyond flaying

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (combat.md #47, inventory.md H1)
- **Files:** `game/data/items.lua`, `game/data/recipes.lua`, `game/data/weapons.lua`

**Original:**
- A knife dropped on the melee card is taken as a melee weapon: 20, 19 or 21 damage (12.10.17.41 to .43).
- Knife plus wood sticks makes arrows. Knife plus a burlap sack is listed in Check_craft (12.14.1.4).
- Knives carry a condition (finding 15).

**Remake:** The three knives only flay (28.2).

### 40. Which gun takes which attachment

- **Verdict:** make identical  **Effort:** S  **User's ask:** "weapon attachments, not all attachments go on every gun." (Stage 23). The original's own table meets it. combat.md #8 read the ask as covering the remake's table; the ask does not name one.
- **Files:** `game/data/attachments.lua` (`guns`)

**Original:** Each attachment's activation branches by the gun in hand (8.10.3.64 to .108):

| Attachment | Guns |
|---|---|
| RDS | AK-74, AN-94, AKM, AKS-74u, Bizon, crossbow, L85, Sporter, SVD, MP5k, Mosin, UMP, RPK, FN-CAL, Saiga, Groza, VSS, SV-98, M4, M16A2, Vector, SKS (not the AUG) |
| PU | Mosin, SKS |
| PSO | AK-74, AN-94, AKM, AKS-74u, Bizon, SVD, RPK, Groza, VSS, SV-98 |
| ACOG | crossbow, L85, MP5k, UMP, FN-CAL, M4, M16A2, Vector (not the AUG) |
| Long scope | crossbow, Sporter, SVD, Mosin, SV-98, SKS |
| Silencer 5.45 | AK-74, AN-94, AKS-74u |
| Silencer 5.56 | AUG, L85, FN-CAL, M4, M16A2 |
| Choke | IZH-43, Benelli, Saiga |
| AK grip | AK-74, AKM, AKS-74u, RPK |
| Rail grip | AUG, L85, UMP, M4, Vector |
| Magpull (quickdraw) | AK-74, AN-94, AKM, AKS-74u, AUG, L85, RPK, FN-CAL, Saiga, Groza, VSS, M4, M16A2 |
| Bandolier | IZH-43, Silenced Remington, Benelli, Repeater, Mosin, SKS |

A gun with no branch says "It doesn't fit there.".

**Remake:** `attachments.lua` `guns`, by gun family:

| Gun | Difference from the original |
|---|---|
| AKM | adds a silencer |
| AKS-74u | lacks the PSO |
| RPK | lacks the RDS |
| Groza | adds a silencer |
| VSS | adds a silencer, lacks the RDS |
| Bizon | adds a silencer and the quickdraw, lacks the PSO |
| SVD | lacks the RDS |
| SKS | lacks the RDS and the long scope; has the quickdraw, not the bandolier |
| Mosin | lacks the RDS; has the quickdraw, not the bandolier |
| Saiga | adds the AK grip |
| AUG | adds the ACOG and the RDS |
| FN-CAL | adds the rail grip |
| MP5 | silencer instead of the ACOG |
| SV-98 | silencer and quickdraw instead of the RDS and PSO |
| Madsen | adds the quickdraw |
| Repeater, shotguns | quickdraw instead of the bandolier |
| Chigur | adds the choke |
| Crossbow | lacks the ACOG and long scope |

The file says so: "the real-world family where they only say 'a scope goes here'", and "the AKM and the MP5 take a silencer though their pictures place no muzzle point".

### 41. The bandolier and the Magpull are two belt attachments

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/attachments.lua`, `game/data/items.lua` (bandolier)

**Original:**
- The Bandolier (83), a shell holder, fits the IZH-43, Silenced Remington, Benelli, Repeater, Mosin and SKS.
- The Magpull (89, `quickdraw_grip`) fits the box-magazine rifles (finding 40).
- Both go on the belt mount (Detach_belt) and shorten the reload, with per-gun times as in combat.md #8.
- The Bandolier drops from types 9, 15, 19 and 21.

**Remake:** `quickdraw_grip` serves both: "a shell holder on the stock of a gun with no box magazine, a coupler at the well of one with". The Bandolier is inert ("an ammunition belt with no slot to wear it in").

### 42. The underbarrel launchers and the laser sight

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (combat.md #47)
- **Files:** `game/data/attachments.lua` (`launchers`), `game/data/items.lua`

**Original:** The M203 fits the AUG, L85, FN-CAL, M4 and M16A2, and the GP-25 fits the AK-74, AN-94 and AKM, on the grip mount (8.10.3.94, .95). They fire 40mm and VOG rounds (8.6.1.2). The Laser Sight (unlock-gated) also takes the grip mount (.108).

**Remake:** Not attachable: "There is no area damage in the game".

### 43. Crafting recipes

- **Verdict:** make identical  **Effort:** XL  **User's ask:** "To build a campfire the player needs 1 wood, 1 stick. That makes a campfire kit. Then with 1 stick and 1 newspaper, the player can make a campfire starter kit." (Stage 24). Those two stay. inventory.md H1 to H3 has the rest.
- **Files:** `game/data/recipes.lua`, `game/src/game/crafting.lua`

**Original:** Check_craft (12.14.1.4), cell onto cell, and Call_sub_menu_craft (12.10.10, 12.10.17). Pairs that craft:

| Pair | Makes |
|---|---|
| Flare + Whiskey | Molotov |
| Battery + Radio | (loads it) |
| Ashwood stick + Rope | bow or fishing rod |
| Wood sticks + a knife | arrows |
| 2 Burlap sacks | Canvas |
| Burlap sack + Rope | improvised bag |
| Burlap sack + a knife | (listed) |
| Papers + Wood piles | campfire kit |
| Wood piles + Canvas / Rags / Wood sticks | stash, campfire kit, fence |
| Wood sticks onto the improvised bag | improvised backpack |

Patching and sharpening are in finding 35.

**Remake:** The user's two recipes only.

### 44. Zombies drop nothing

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (zombies.md #32)
- **Files:** `game/src/game/systems/combat.lua`

**Original:** Within 500 px of the player:
- An ordinary zombie drops one of 19 two times in seven.
- A soldier drops one of 12 one time in seven.
- A runner drops one of 20 two times in seven.

The item lands at the corpse's y + 15 (13.3.1.2.2.13, .4.5, .5.6).

**Remake:** Only the corpse.

### 45. The search lines over the head

- **Verdict:** unclear  **Effort:** S  **User's ask:** "Remove inventory hints, it's unnecessary" and "remove the 'can't wear that' text or any type of hint" (Stage 20). These lines report what happened rather than give a hint (inventory.md F6).
- **Files:** `game/src/game/status.lua`, `game/src/game/container.lua`

**Original:**

| Line | Colour | Event |
|---|---|---|
| "I found something in this trunk." | yellow | 12.13.3.1.2 |
| "There is nothing in this trunk." | white | 12.13.3.1.2 |
| "Prey skinned." | yellow | 8.10.3.36 |
| "I've cooked a steak." | green | 8.10.3.53 |
| "I need an active fireplace for that." | red | 8.10.3.53 |
| "Blood transfusion completed." | | 8.10.3.96 |

**Remake:** No line for a search or a trunk. The flay says "Nothing to flay." only when refused.

### 46. Sounds of searching and of attaching

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (inventory.md I1 to I3 for pick-up, drop and use)
- **Files:** `game/data/sounds.lua`, `game/src/game/container.lua`, `game/src/game/attachments.lua`

**Original:**
- A trunk plays `open` at -5 dB (12.13.3.1.2.1).
- A bush plays `bushsearch` (12.13.3.1.3.1.1).
- Attaching or detaching anything plays `reload_pistol` at -5 dB (8.10.3.64 to .108).

**Remake:** Opening a container, searching and attaching are silent (no audio call in `container.lua` or `attachments.lua`).

### 47. The truck's and the UAZ's trunks

- **Verdict:** unclear  **Effort:** S  **User's ask:** "Lootable vehicle trunks." (Stage 23). It does not say which vehicles.
- **Files:** `game/data/trunks.lua` (`car_truck`, `car_uaz_anims`)

**Original:** No `car_loot_point` on the truck, UAZ or BTR (3.5.7.1.5.N only spawns one for the regular cars, vans, police cars and hatchbacks).

**Remake:** The truck bed (10 slots) and the UAZ trunk (6) on the `convoy` table. Whether the user's "vehicle trunks" means these too is theirs to say.

### 48. Sizes in the inventory: weapons and garments carried in cells

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Pistols, Rifles, Melee should only go on backpack slots, and they should be 1x2 for pistols, 1x3 for rifles and 1x2 for melee. Clothing shouldn't go on other clothing." (Stage 20)
- **Files:** `game/src/game/inventory.lua` (`may_hold`), `game/data/weapon_slots.lua`

**Original:** Everything that goes in a cell takes one cell. A weapon or garment is never in a cell: it is worn or held, or it lies on the ground (inventory.md B5).

**Remake:** Pistols and melee weapons lie across 2 backpack cells and rifles across 3. A spare garment takes a backpack cell. Everything else takes one cell, as in the original.

## Already identical

- **The item roster** beyond the 11 of finding 20. Each of the remake's 216 indexed items is the original's object, under its drop id.
- **Inventory icons.** 113 of 115 cell icons (ids under 127) are the original's `gui_item_backpack[t6]` frame for that id, modulo a transparent border. Item 97 is the exception.
- **The four weapon icons the remake re-points in `item_art.lua`** (Bizon, Groza, Saiga, VSS) are what 1.0 shows in its Ground list. Checked live: `SCR/orig/guns_inv_crop.png` against `SCR/rip_icons_4.png`.
- **147 ground sprites** are pixel-identical to the original's frames.
- **The number of pieces per building** equals the original's loot points (27.1). The deer stand (finding 6) and the zero-chance points (finding 5) are the exceptions.
- **A searched car shows its lid-up frame**, as `fam_car_btr.SetAnimFrame(1)` does (12.13.3.1.2.1.1).
- **Raw meat and flaying:**
  - Flaying (8.10.3.36 to .38): 3 s, two raw steaks from a deer, one rabbit meat from a rabbit, the knife kept.
  - Cooking raw meat at a burning fire: 5 s (8.10.3.53, .59).
  - Eating raw meat: 3 s, food 25, water 10, a 1-in-2 chance of sickness (8.10.5.2, .3).
- **Cooked meats:** cooked steak 50/10, +15 hp, 3 s; cooked rabbit 35/10, +30 hp, 3 s (8.10.3.54, .60).
- **The cleaning kit:** +25 condition to the gun in hand, refused at 0% and at 100%.
- **The energy drink's boost lasts 60 s**, refreshed and not stacked (8.10.3.56). character.md #16 covers its size.
- **Attachment mounts:**
  - No pistol, sawn-off or bow takes an attachment.
  - There are four mounts: scope, muzzle, grip and belt.
  - A launcher would share the grip mount.
  - Putting an attachment on an occupied mount swaps the old one out.
  - The Sporter (RDS, long), L85 and M4 (less the M203), and AK-74 (less the GP-25) take the same set.
- **Bags and garments keep their contents** on the ground. The original's carry their own arrays, which the cull destroys with them (14.1.2.4).
- **Gear never stacks.**
- **Heli crashes:** none on the original's first map (`helispawned = 0` at level 0, 3.14.1), and none in the remake.

## Not checked

- **The car trunks were read from the events only** (12.13.3.1.2); none was opened live in the original. Driving one needs an interact `walk_marker` on a parked car's loot point.
- **Camp lootboxes and the bunker** were read, not opened live. Neither was the hunter's stash, the tents camp's drops or the humvee crash's three points.
- **Of the 22 loot types, only 1, 3, 9, 13, 19, 23 and 24 were fired live** (60 each). The others are read from 14.1.4.1.
- **`Global_respawn` was read, not waited out** (1500 s). Whether its timer runs faster while sleeping was not checked.
- **Berry bushes, pumps, fertilizer, seeds, rods, the headlamp, NVG, flare and radio** were read from the events only.
- **The 58 ground sprites that differ only by a transparent border:** whether the border moves where the remake draws them (anchor) was not measured.
- **The airdrop** (12.4) is a perk's (`gui_airdrop_icon`, Airdrop_recharge), quest rewards (12.2.9) are quests, and bandit and bot drops (13.4.1.18) are bandits. All are outside the remake. Their loot lists were not compared.
- **The tutorial's own loot** (the tutorial car's beans, Pipsi, Spite, rice) was not compared; the remake has no tutorial yet.
- **Sounds** were read only. The original's headless audio fails to decode.
- **What `SEED_morezeds = 1` changes** belongs to the zombies and world surveys.
- **A real phone:** everything here was at 1280x720.
