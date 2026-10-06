# inventory: the remake against the official 1.0

*Area surveyed:* Inventory and crafting: the inventory board (layout, slots, equipment, capacities, ground and containers), tapping an item (sub-menu / info popup vs tooltip), drag and drop, timed uses, every item's use, crafting recipes, sounds and texts

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

The original 1.0 was driven live in headless Chromium at 1280x720 with touch, through a persistent driver at scratchpad/survey/inventory/tools/live.cjs, with items given by the game's own Spawn_drop. I also read its events: Game_events 8.10-8.13, 12.10, 12.11, 12.13.10/12, 12.14, 12.15, 12.18, 14.2, 7.1 and 8.6.4. The remake was captured with cap.py and its code read: data/ui/inventory.lua, src/game/board.lua, inventory.lua, item.lua, crafting.lua, data/items.lua, recipes.lua, sounds.lua and config.lua items.*.

The main findings are below.
- **Capacity.** The original has ONE hand slot (Arr t388, size 1), not six pockets. Its bags hold 1-7 cells in one row; the remake gives 3-14 in two rows. Garment capacities differ too, and the pick-up fill order differs (original: hand, trousers, outerwear, vest, backpack).
- **Item numbers.** Stack sizes and every consumable's numbers and use time differ: the remake calls them "invented", but the original's are all in 8.10.3 and 8.12.7. About 40 usable items are inert in the remake. Sixteen garments' armour/heat came from the wrong build.
- **Drag and drop.** The original drops a thing when you drag it off the board; the remake snaps it back. The original lights only the target under the finger, and asks for a "Stack"/"Craft" menu before merging or crafting.
- **Timed uses.** In the original the item is spent at the START and nothing (step or attack) can cancel the use.
- **Missing pieces.** There are no pick-up, drop or use sounds in the remake. Most of the original's recipes and item actions are missing (Load/Eject ammo, Fill with water, Tear into rags, Split, Craft Rope, Patch...). There is no (i) info popup.
- **Item state.** 140 item names differ. Things found spawn at 100% in the original (the remake rolls 25-100%). The original's random colour variants are missing. The start kit differs: T-shirt + jeans vs an FNX pistol.
- **Board look.** The board is bigger, dims the world, hides the HUD, moves MELEE and shortens the backpack card. It adds labels, names and "x / y" counts, and uses "warmth"/"armour" where the original says "Heat"/"Armor".

Several remake features are deliberate user asks: tabs and containers, TAKE ALL, the foot pager, the tooltip with actions, weapons and clothes in the backpack, auto-equip only into an empty slot, only the used item locked, the bar over the head and on the item, the two campfire recipes, and "My inventory is full".

Four asks about the board are recorded only in Progress.md 17.4 and are missing from user_asks.md: "i like this design more"; "shrink the menu a little / Place the health stats at the top / The take all should consume a container slot ... / Tapping something opens a tool tip with item stats and actions, dragging allows moving items"; and "items that have durability shouldn't stack".

The harness refused writing scratchpad/survey/inventory/report.md (subagents return findings as output), so this output is the report.

Evidence:
- Original screenshots: scratchpad/survey/inventory/orig/ (new this run: inv_open_start, hud, g1-g8, s1-s9, x_submenus2, i0/i1_ak_info, d0-d2 drag off, e0-e4 eating, f2_nofree; earlier runs: submenu_apple, info_apple, eat_*, drag_*, craft_*, p2_inv_ground).
- Remake captures: scratchpad/env/s3/caps/ (r_open_start, r_sv_gear, r_sv_dragoff, r_sv_craft, r_sv_craft_tip, inv_ground, inv_tip_*, inv_eat_open/closed, inv_phone, x_inv_drag_grid).
- Side by side: scratchpad/survey/inventory/sbs_board_start.png, sbs_board_gear.png, sbs_phone.png.
- Extracted tables: scratchpad/survey/inventory/slot_actions.txt, craft_menu_table.txt, item_activate_table.txt, consumables_cmp.txt, names_diff.txt, ev_*.txt.

## Findings

Verdicts: make identical 45, keep (user asked) 8, unclear 6, platform 1

### 1. A1 Board size and placement

- **Verdict:** unclear  **Effort:** M  **User's ask:** "shrink the menu a little" (Progress.md 17.4.1/17.4.2 - not in user_asks.md); satisfied relative to the remake's earlier board, silent on the original's size
- **Files:** `game/data/ui/inventory.lua`, `game/src/ui/ui.lua`

**Original:** One sprite gui_inventory 362x283 art px (origin 181,146) set at the screen centre (12.13.10.1.1 SetPos(user_device_X_mid, user_device_Y_mid)): ~700x547 px at 1280x720 (x 288-988, y 72-619), ~435x342 at 844x390 (orig/inv_open_start.png, orig/p2_inv_ground.png)

**Remake:** 848x478 design units (data/ui/inventory.lua W,H) = ~1130x637 px at 1280x720 (x 5-1135), ~716x343 at 844x390, slid left to clear the bag (screen.clear) (caps/r_open_start.png, caps/inv_phone.png; survey/inventory/sbs_board_start.png, sbs_phone.png)

### 2. A2 Board art: the original's single gui_inventory picture vs a rust nine-patch

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** gui_inventory-sheet0.png: dark riveted metal, the column dividers, the word 'Ground' and the hand-slot paper all baked into one image (survey/inventory/x_gui_inventory3.png)

**Remake:** tilesets/bg_rust nine-patch (insets 28) with gui_pickpile_panel recesses and separately drawn paper cards (data/ui/inventory.lua inv_board, near_recess)

### 3. A3 World dimmed and inert round the open board

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/inventory.lua`

**Original:** No dim: the world is drawn at full brightness round the board (orig/inv_open_start.png, orig/g1_ground.png)

**Remake:** inv_dim: 35% black, modal, bleed to the screen edges (data/ui/inventory.lua inv_dim color {0,0,0,0.35}); caps/r_open_start.png

### 4. A4 HUD hidden while the board is open

- **Verdict:** make identical  **Effort:** M  **User's ask:** "Place the health stats at the top" (Progress.md 17.4) covers the vitals on the board; nothing covers hiding the rest
- **Files:** `game/data/ui/touch_controls.lua`, `game/data/ui/inventory.lua`, `game/data/ui/hud.lua`

**Original:** Everything but the stick stays visible and usable: portrait, the four meter plates and their icons, clock, settings, notepad, pick-up hand, target, reload, weapon switch, the bag bottom-left. Only GUI_hide_dpad (12.13.10.1.3) and the car buttons go (orig/g1_ground.png, orig/s1_ammo_menu.png)

**Remake:** Every touch control has visible_when_not='inventory_open' (data/ui/touch_controls.lua). The vitals are re-drawn on the board (vit_*) and the bag is copied to the board's edge (inv_bag) (caps/r_open_start.png)

### 5. A5 Card arrangement: MELEE moved, backpack card shortened, two-row Pockets card

- **Verdict:** make identical  **Effort:** M  **User's ask:** "i like this design more" (Progress.md 17.4, of a mock-up of the original's locker); the moved MELEE and the shorter backpack are not asked
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** gui_inventory image points: card_0..3 at x128 (vest y12, outerwear y78, pants y142, backpack y208). Helmet (237,32), hand_slot (239,91), Melee (226,146) under the hand slot, Pistol (319,33), Firearm (320,129) upright in the right column. The backpack card (216 art px, 7 cells) spans the whole right part along the bottom; Ground takes the left third (orig/g8_wood.png)

**Remake:** VEST/JACKET/TROUSERS column, HEAD over a two-row Pockets card, PISTOL/RIFLE column, BACKPACK across two columns only, MELEE at the bottom-right (caps/r_sv_gear.png; survey/inventory/sbs_board_gear.png)

### 6. A6 Text labels on cards (VEST, HEAD, JACKET, TROUSERS, BACKPACK, RIFLE, MELEE, Pockets, item names)

- **Verdict:** unclear  **Effort:** S  **User's ask:** "Inventory is too disorganized, design inventory to be easier to understand" (Stage 17) and the user's pick of the Faithful mock-up, which carried labels
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** No labels except 'Ground' baked in the art. An empty card shows only its ghost (gui_card_cover); a worn card shows its picture and numbers only (orig/inv_open_start.png, orig/g8_wood.png)

**Remake:** A caption on every card, empty or worn, plus the worn item's name ('Tshirt', 'Fnx pistol') (caps/r_open_start.png, caps/r_sv_gear.png)

### 7. A7 Worn garment card text: '(100%) Heat +N / Armor +N' vs 'Name 100% warmth +N armour +N 0 / N'

- **Verdict:** make identical  **Effort:** S  **User's ask:** "durability should show up even when 100%" (the original shows it too, in brackets); "Remove the carrying x out of x items" (the board-wide line; the per-card count is the remake's own)
- **Files:** `game/src/game/board.lua`, `game/data/ui/inventory.lua`

**Original:** Portrait top-left, then in a dark bold serif top-right '(100%)' and 'Heat +5' or 'Armor +5'. A helmet shows 'Heat +3 / Armor +5', its picture and '100%' on a dark band. No name, no used/total count (orig/g6_bp_vest.png, orig/g8_wood.png)

**Remake:** 'Tshirt 100% warmth +2 0 / 1', 'Razgruz 100% armour +3 0 / 3', 'Helmet army 100% armour +4 warmth +2' in a sans face (board.lua 1231-1236 row.armour/row.warmth; caps/r_sv_gear.png)

### 8. A8 Weapon card text: name and 'rounds/capacity' vs rounds only

- **Verdict:** make identical  **Effort:** S  **User's ask:** "ammo count for items in inventory should also show up" (the original's count is the rounds alone)
- **Files:** `game/src/game/board.lua`, `game/data/ui/inventory.lua`

**Original:** Picture with a dark band: '100%' left, rounds in the gun right ('0'). No name (orig/g3_after_dtap2.png)

**Remake:** Name label on top ('Ak74 rifle', 'Fnx pistol'), '100%', '30/30' / '12/12' (board.lua rounds_of; caps/inv_ground.png)

### 9. A9 Attachment mounts drawn empty on the rifle card

- **Verdict:** make identical  **Effort:** M  **User's ask:** "weapon attachments, not all attachments go on every gun" (the feature, not its look)
- **Files:** `game/src/game/board.lua`, `game/src/game/attachments.lua`, `game/data/ui/inventory.lua`

**Original:** No empty mounts. An attached part is drawn ON the gun picture at its image point: gui_item_backpack[t768] scope SetPosToObject(gui_firearm,'scope') + Pin (8.10.3.64+); t769 silencer, t774 belt, t949 grip likewise. Detaching is from the gun's sub-menu 'Detach ...' (12.10.2.1.18-21)

**Remake:** Four mount pictures stacked down the RIFLE card, empty or filled (attachments.fill; caps/inv_ground.png, caps/inv_tip_ak.png)

### 10. A10 One hand slot vs six pockets

- **Verdict:** make identical  **Effort:** M  **User's ask:** "Let's go with a simple slot inventory, like the original game." (Stage 6)
- **Files:** `game/data/config.lua`, `game/src/game/inventory.lua`, `game/data/ui/inventory.lua`, `game/src/game/save.lua`

**Original:** Arr[t388] (hand slot) has size 1 (dumped live: t388 [0]): one paper cell in the board's middle, 'hand_slot' (239,91), taking anything (orig/inv_open_start.png)

**Remake:** cfg.items.pocket_slots = 6 (data/config.lua 596; inventory.new), a 'Pockets 0 / 6' card two rows tall (caps/r_open_start.png)

### 11. A12 Count and condition label format, font

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (text drawn at its landed size, 17.5, is a legibility fix, not an ask about format)
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** Stack count white, centred under the icon, no 'x' ('25', '3'). Garments '(100%)' in brackets. The water bottle shows its water '75%'. The Ground list writes 'x25' and '(100%)'. Serif for card stats (orig/g7_filled.png)

**Remake:** 'x23' in the cell's bottom-right corner with a halo, '100%' without brackets, one sans face throughout (caps/r_sv_gear.png; Progress 25.3.6, 17.5)

### 12. B1 Backpack capacities and their two-row grid

- **Verdict:** make identical  **Effort:** M  **User's ask:** "Pistols, Rifles, Melee should only go on backpack slots, and they should be 1x2 for pistols, 1x3 for rifles and 1x2 for melee" - one row satisfies it; capacities are not asked
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`, `game/data/ui/inventory.lua`, `game/data/config.lua`

**Original:** Spawn_drop 14.2.175-183, one row on a 7-cell card: taloon 3, hunting 5, tortilla 6, mountain 7, school 2, improvised bag 3, improvised backpack 5, crossbody (barsetka) 1, civilian tent 0 (orig/g8_wood.png: 7 cells)

**Remake:** data/items.lua 464-473, two rows: improvised bag 4, improvised backpack 6, school 8, hunter 10, taloon 12, mountain 14, barsetka 3 (caps/r_sv_gear.png '0 / 14'; inventory.grid_cols)

### 13. B2 Garment capacities

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua`

**Original:** 14.2.184-232: t-shirt 0, shirt 1, raincoat 2, raider jacket 1, sweater 1, awesome hoodie 3, gorka 3, hoodie 2/2, cloak 1, paramedic 2, orel 2, tracksuit 1, dress 1, down 2, hunter 3. Vests: bulletproof 0, press 1, assault 2, high-capacity 3, kevlar 3, soviet 2, apron 1. Pants: jeans 2, worker 3, hunter 3, tracksuit 1, gorka 3, orel 2, paramedic 2

**Remake:** data/items.lua 505-543: tshirt 1, raider 3, hoodie_red_true 2, cloak 2, paramedic_jacket 3, orel_jacket 3, sport_jacket 2, vest_bulletproof 2, vest_press 3, razgruz 3, razgruz_big 5, fartuk 2, sportpants 2, orel_pants 3 (the rest agree)

### 14. B3 Pick-up fill order

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/inventory.lua`

**Original:** Check_space_inventory 8.12.2-8.12.6: hand slot, then trousers, then outerwear, then vest, then backpack; in each, a same-item stack with room first, else the first empty cell (orig/g7_filled.png: ammo in hand, apples+bandage in jeans, beans+papers in hoodie, rags+bottle in vest, wood in bag)

**Remake:** inventory.containers: pockets, then jacket, vest, trousers, backpack (caps/r_sv_gear.png: six things all in Pockets)

### 15. B4 Stack sizes

- **Verdict:** make identical  **Effort:** S  **User's ask:** "items that have durability shouldn't stack" (Progress.md 17.4.14) - gear only, which the original never stacks either
- **Files:** `game/data/items.lua`

**Original:** Check_stack_size 8.12.7: 3 bandage/rags/tomato/apple/banana/bell pepper/orange; 5 flare, cranberry, wood sticks, arrows, cloudberry, bilberry, elderberry, 40mm, VOG; 10 matches; 15 12cal; 20 7.62; 30 5.56/7.62x39/5.45/.357/9x39; 40 .45/9mm/9x18; 50 .22/.308; everything else 1 (beans, tuna, bottles, wood piles, papers, heatpack...)

**Remake:** data/items.lua: bandage 5, rags 4, fruit 6, berries 10, beans/tuna/bacon/rice 3, sodas 4, wood 3, sticks 6, papers 5, heatpack 3, tetracycline 4, morphine 2, cleaning kit 2; ammo 24-80 (5x45 60, 12cal 24, 22lr 80, 7x62 40)

### 16. B5 Weapons and clothing carried in storage

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Pistols, Rifles, Melee should only go on backpack slots, and they should be 1x2 for pistols, 1x3 for rifles and 1x2 for melee. Clothing shouldn't go on other clothing."
- **Files:** `game/src/game/inventory.lua`

**Original:** Never: a weapon or garment is worn/held or on the ground. Dragged within the board it snaps back (12.14.4.1.2); nothing places one in a cell (orig/drag_rifle_over_backpack.png, orig/rifle_in_backpack.png)

**Remake:** Pistols and melee lie across 2 backpack cells, rifles across 3; spare garments go in the backpack (inventory.may_hold, 20.3)

### 17. B6 Picking up a wearable whose slot is occupied

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "grabbing item from ground or container should auto equip if slot is empty"
- **Files:** `game/src/game/item.lua`

**Original:** It is worn and the old one is dropped at the feet: double-tapping a Hoodie while wearing a T-shirt left the T-shirt under Ground (orig/g5_hoodie.png; 8.11 Pick_player_item). Weapons the same

**Remake:** Stored in the backpack; the worn one stays on (item.fit)

### 18. B7 Condition of things found

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/item.lua`

**Original:** Everything spawns at 100%: var#6 choose(100,100,...) x102 and '100' x7 in 14.2; loot passes -1 (that default). Only the raider jacket rolls choose(25,25,50,50,75,100)

**Remake:** Every piece of gear rolls {25,25,50,50,75,100} (data/config.lua items.condition_roll: 'the original's own roll ... we use this for everything')

### 19. B8 Random colour variants of garments and bags missing

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/item.lua`, `game/src/game/render.lua`, `game/src/ui/widgets.lua`

**Original:** 13 garments and bags get a random hue at spawn (var#25: jeans choose(0,50,75), tshirt choose(0,35,80), mountain backpack choose(8,55,0,20), taloon choose(18,50,0,65), workpants, sportpants, sport_jacket, shirt_green, down_jacket, dress, hunter jacket/pants, school backpack). The 'Colors' effect applies it in the world, on cards, the portrait and the info popup (14.2.x, 12.11.2.3.1)

**Remake:** One colour per item; no hue anywhere in src/game

### 20. B9 Armour/heat of 16 garments taken from the wrong build; heat not scaled by condition

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/generated/clothing_stats.lua`, `tools/extract_clothing_stats.py`, `game/src/game/inventory.lua`

**Original:** 1.0 placed instances (objects.txt var#9 heat, var#12-15 armour) and the live board ('Heat +3 Armor +5' Combat Gasmask, 'Armor +5' Assault vest, HUD armour 10; orig/g8_wood.png): helmet_army 3/5, gorka_helmet 3/8, razgruz 0/5, razgruz_big 0/6, vest_bulletproof 0/15, vest_press 0/7, gorka jacket/pants 7/3, orel jacket/pants 5/2, gasmask 4/1, greathelm 4/6, helmet_hard 2/4, helmet_nvg 4/4, headlamp 2/2, pilot_helmet 3/3. Heat is scaled by condition: var#8 = ceil(var#9/100*var#6)

**Remake:** data/generated/clothing_stats.lua: 2/4, 3/5, 0/3, 0/3, 0/8, 0/6, 7/2, 5/1, 2/0, 4/5, 2/2, 1/0, 1/0, 3/2 (card shows 'armour +4 warmth +2' on Helmet army, HUD 7; caps/r_sv_gear.png). No condition scaling

### 21. B10 Civilian tent not wearable / not deployable

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`

**Original:** Worn in the backpack slot (0 cells); its card's sub-menu offers 'Deploy' (12.10.9.1; Deploy_tent / Roll_tent 12.10.15-16)

**Remake:** Carried like rope, cannot be worn, does nothing (data/items.lua 'NOT DESCRIBED')

### 22. B11 Starting kit

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** Default character (2.13.1.1): T-shirt and jeans worn at 100%, nothing in the hands or the hand slot (orig/inv_open_start.png)

**Remake:** FNX pistol 12/12 in the PISTOL slot, nothing worn (src/game/player.lua 257-268; caps/r_open_start.png)

### 23. C1 NEARBY tabs, containers, TAKE ALL, foot pager, bigger tabs

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "just include tabs on the left hand side with the icons of the container ... On ground always being first"; "The take all should consume a container slot" (Progress.md 17.4); "Move scroll bar from side to bottom of left column, horizontal buttons ..."; "Lootable vehicle trunks. appropriate container scales"; "increase the size of the containers tabs"
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** One 'Ground' list, no containers or take-all; red up/down arrows on the board's left edge, hidden with one page (12.15.2.2) (orig/g1_ground.png)

**Remake:** Tabs per source, furniture and trunks, a TAKE ALL row, horizontal back/forward pager darkened at the ends, 50x62 tabs (data/ui/inventory.lua NEARBY; caps/inv_ground.png)

### 24. C2 How far the Ground list reaches

- **Verdict:** make identical  **Effort:** S  **User's ask:** "Reduce the range for opening containers" covers containers only (open_reach 16)
- **Files:** `game/data/config.lua`, `game/src/game/board.lua`

**Original:** Only items overlapping the player's 30x30 collision box (player_collision_base.IsOverlapping, 12.15.1.1-2): a few px round the feet

**Remake:** items.reach_range 90 px for the ground tab; E picks within pickup_range 28 (data/config.lua 600, 618)

### 25. C3 Rows per page

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/data/ui/inventory.lua`

**Original:** 7 (12.15.2: ceil(Arr[t539].Width / 7), 7 gui_pickpile_panel rows 71 px apart)

**Remake:** 8 (items.near_rows, data/ui/inventory.lua NEAR_ROWS)

### 26. C4 Ground row look and its pick-up hand

- **Verdict:** unclear  **Effort:** S  **User's ask:** The hand came with the asked TAKE ALL change (Progress 17.4.4) but is not itself asked
- **Files:** `game/data/ui/inventory.lua`

**Original:** gui_pickpile_panel 108x33 art (~209x64 px): icon left, the name over '(100%)' / 'x25' / '25%' in white (orig/g1_ground.png)

**Remake:** 192x44-unit rows: icon, name, 'x3'/'100%', and the pick-up hand at the right as a one-tap take (caps/inv_ground.png)

### 27. C5 Double-tap a ground row to take it

- **Verdict:** make identical  **Effort:** S  **User's ask:** The row tooltip is asked ("Tapping something opens a tool tip ..."); the missing double-tap take is not
- **Files:** `game/src/game/board.lua`, `game/src/ui/ui.lua`

**Original:** A double tap on a row takes it (12.15.4 OnDoubleTapGestureObject): a weapon or garment goes straight on, anything else into storage; a single tap does nothing (orig/g3_after_dtap2.png)

**Remake:** No double tap: one tap on the hand takes; a tap on the row opens a tooltip with TAKE / WEAR

### 28. C6 Order of things in the Ground list

- **Verdict:** make identical  **Effort:** S  **User's ask:** "anything else below it based on range" orders the tabs, not the rows
- **Files:** `game/src/game/item.lua`, `game/src/game/board.lua`

**Original:** Gear and weapons (fam_ak74_rifle) first, then other items, in instance order (12.15.1.1-2)

**Remake:** Nearest first (item.within sorted by distance)

### 29. C7 HUD pick-up with several things underfoot

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/item.lua`

**Original:** System.PickRandom among the overlapping items (12.13.3.1.5.1)

**Remake:** The nearest (item.nearest)

### 30. C8 Where a dropped thing lands

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/item.lua`

**Original:** At the feet, 11-15 px below the collision centre (Spawn_drop_from_player 8.13.2: Y + 15, apple + 11)

**Remake:** 22 px ahead in the facing direction (items.drop_distance)

### 31. D1 Tap: original sub-menu vs remake tooltip

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Tapping something opens a tool tip with item stats and actions, dragging allows moving items" (Progress.md 17.4)
- **Files:** `game/data/ui/item_tip.lua`, `game/src/game/board.lua`

**Original:** A small menu at the item's image point grows upward: a dark title bar with (i) and the name, then a paper row per action. Some items have only the title (gasmask, backpack, hatchet). It is not kept on screen (the pistol's runs off the top), has no Drop, and closes on a tap elsewhere (12.10.1, 12.10.11, 12.13.12; orig/submenu_apple.png, orig/x_submenus2.png)

**Remake:** A 264x180 tooltip beside the thing: picture, name, computed facts, a location line ('In your POCKETS'), and buttons EAT/DRINK/USE/BUILD/LIGHT/MAKE + DROP, TAKE/WEAR, TAKE OFF, HOLD (data/ui/item_tip.lua; caps/inv_tip_apple.png, caps/x_r_tips.png)

### 32. D2 The (i) info popup and the original's stat text

- **Verdict:** unclear  **Effort:** M  **User's ask:** The tooltip is asked; its stat wording is not
- **Files:** `game/src/game/board.lua`, `game/data/ui/item_tip.lua`

**Original:** (i) on the menu title opens a big paper card at the screen centre: the name, the item at 4x and its l_eng_new description. Apple: 'Food +15 / Water +5 / Max stack: 3'. AK-74: 'Damage: 18-32 / Accuracy: 7 / Aim Speed: 5 / Rate of fire: 600 / Ammo: 5.45 / Mag size: 30' (12.11.2; orig/info_apple.png, orig/i1_ak_info.png)

**Remake:** No popup; the tooltip's own facts: 'Food 100 (full)', 'Rifle / 30 / 30 in the magazine', 'No use for it yet', 'Ammunition' (board.lua facts)

### 33. D3 Item actions missing from the remake

- **Verdict:** make identical  **Effort:** XL  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`, `game/src/game/board.lua`, `game/src/game/systems/combat.lua`

**Original:** Per item (survey/inventory/slot_actions.txt, 12.13.12, 12.10.2-9):
- Load main / Load secondary on every ammo type, and by dragging ammo onto a gun: instant top-up with the Mag_reload animation (2.5 s, 1.75 s with a belt).
- Eject on a rifle or pistol (unload); Detach on attachments.
- Fill with water; Tear into rags (shirts, sweater, raincoat, one helmet); Split (wood sticks); Craft Rope (rags).
- Plant (seeds, vegetables); Throw / Light and throw / Light up; Deploy (traps, mine, wire, tent, campfire kit).
- Apply; Eat vitamins; Blood / Saline transfusion; Take as melee / Flay (knives); Cook; Smoke; Read; Scan Area / Eject (radio); Repair car; Fuel; Call Airdrop; Start fishing; Fertilize; Saw off rifle.
- Attach battery and Toggle (headlamp, NVG)

**Remake:** Only EAT/DRINK/USE (food, water, heal, cure, warm, stop_bleeding, repair, boost), BUILD/LIGHT for the user's kits, EQUIP/WEAR/TAKE OFF/HOLD/DROP/TAKE, MAKE, attach by drag and detach by tap (board.lua verb_of; data/items.lua)

### 34. D4 Action words

- **Verdict:** make identical  **Effort:** S  **User's ask:** The tooltip with actions is asked; the words are not
- **Files:** `game/src/game/board.lua`

**Original:** 'Eat', 'Drink', 'Apply bandage', 'Use heatpack', 'Blood transfusion', 'Load main' ... (sentence case, on paper rows)

**Remake:** EAT, DRINK, USE, BUILD, LIGHT, MAKE CAMPFIRE KIT, DROP, TAKE, WEAR, EQUIP, TAKE OFF, HOLD (upper case, on wooden planks)

### 35. E1 Dragging something off the board drops it

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/board.lua`, `game/src/ui/ui.lua`

**Original:** A cell's stack let go outside the board is dropped at the feet with drop1/drop2 (12.14.5.1 Drop_slot). Worn gear and weapons likewise via Drop_pants ... Drop_firearm with geardrop1/2 (12.14.4.1.1). Seen: the apple stack dragged right of the board appears as 'Apple x3' under Ground (orig/d0_before.png, d1_dragging_off.png, d2_dropped_off.png)

**Remake:** Let go outside the board and it goes back silently (data/ui/inventory.lua header; caps/r_sv_dragoff.png: the apple still in Pockets)

### 36. E2 Dropping onto the Ground column

- **Verdict:** unclear  **Effort:** S  **User's ask:** Containers are asked (C1) and need a drop target; dropping on the ground column itself is not asked
- **Files:** `game/data/ui/inventory.lua`, `game/src/game/board.lua`

**Original:** The Ground column is part of the board: a drop there snaps back (only off-board drops drop)

**Remake:** NEARBY is a drop target: DROP ON THE GROUND, PUT IN THE <container>, or a tab (caps/x_inv_drag_grid.png)

### 37. E3 Only the target under the finger lights while dragging

- **Verdict:** make identical  **Effort:** S  **User's ask:** "just the red tint is enough" (the refusal wash, keep); lighting every target is not asked
- **Files:** `game/data/ui/inventory.lua`, `game/src/ui/ui.lua`

**Original:** gui_slot_bg is spawned only on the cell the dragged thing overlaps (gui_dragger, 12.14.1): green empty, blue occupied, yellow combine/craft (orig/drag_onto_backpack_slot.png, orig/drag_onto_ammo.png)

**Remake:** Every place the thing could go lights at once (all empty wells green), and the whole NEARBY list turns green with a 'DROP ON THE GROUND' card; the one under the finger is brighter (caps/x_inv_drag_grid.png; drop_rest/drop_alpha)

### 38. E4 Stacking by drag needs a 'Stack' menu

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/board.lua`

**Original:** Same item onto same item lights yellow; on release a menu with one row 'Stack' (Combine, 12.10.10.1.3-28) must be tapped

**Remake:** Merges on release (board.lua merge)

### 39. E5 Crafting by drag needs the recipe menu

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 24's recipe ask says what the recipes are, not how they are confirmed. MAKE on the tooltip is a tooltip action (keep)
- **Files:** `game/src/game/board.lua`, `game/src/game/crafting.lua`

**Original:** On release over the partner a menu shows the recipe ('Craft fireplace kit'; 'Craft bow' + 'Craft fishing rod' when two) that must be tapped (12.10.10; orig/craft_menu.png)

**Remake:** Crafts on release (board.lua craft_plan -> crafting.begin; caps/r_sv_craft.png); MAKE on the tooltip too (caps/r_sv_craft_tip.png)

### 40. E6 Drag ghost held where the finger took it

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Just make it so that they're where the touch input began, same with dripping items."
- **Files:** `game/src/ui/ui.lua`

**Original:** The DragDrop behaviour moves the sprite itself with the touch

**Remake:** The ghost is carried at its own size with the pressed point under the finger (20.4.5)

### 41. F1 Spent at the start, and nothing interrupts a timed use

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (the asks cover the crouch, the bar, keeping the board up and the lock, not cancelling or late spending)
- **Files:** `game/src/game/player.lua`, `game/src/game/interaction.lua`, `game/src/game/crafting.lua`, `game/src/game/inventory.lua`

**Original:** Every 8.10.3 entry takes the item before System.Wait (SetReturnValue(param-1) first): the beans are gone from their cell at 0.3 s (orig/e1_eat_03.png). The effect lands at the end. Walking (12.13.5/6/7), attacks (8.6.x, 8.7.1.5), interact and every board action are gated on var#21=0, so nothing cancels. Crafts likewise remove both ingredients, then var#21=1 and Wait (12.10.17.33.3)

**Remake:** Nothing spent until the bar fills; a step or an attack cancels 'with nothing spent' (player.lua 369-374, 545-576; crafting.lua 9-10; Progress 25.3.2)

### 42. F2 Board during a use: everything at 50% and frozen vs only the used item

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "when using an item, don't freeze the inventory, just the item that is being used."
- **Files:** `game/data/ui/use_mark.lua`

**Original:** All icons on the board drop to 50% opacity (7.1.1.11) and every tap and drag is ignored until done (orig/e1_eat_03.png, orig/e2_eat_10.png, orig/eat_1s.png)

**Remake:** Only the item in use dims, with its bar; the rest of the board works (data/ui/use_mark.lua; caps/inv_eat_open.png)

### 43. F3 The use bar's place

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "use gui_wpn_cooldown_bg-sheet0.png for any interaction, such as eating etc." and "i want the character to be crouched down with a bar above his head, matching the one that's under an item"
- **Files:** `game/data/ui/use_mark.lua`, `game/src/ui/widgets.lua`

**Original:** gui_wpn_cooldown_bg 30x3 + fill pinned above the head (Wpn_cooldown 8.6.4) on the world's Rain layer, hidden under the open board. Linear fill (Sine width, triangle, period 4x time, magnitude 28), hidden at width > 28; no bar for crafts (orig/x_eat_closed_05_zoom.png)

**Remake:** The same art, on the item's well while open and over the head while shut, crafts included (caps/inv_eat_open.png, caps/inv_eat_closed.png)

### 44. F4 Backpack during the crouch

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/render.lua`

**Original:** Hidden: the worn bag gets SetVisible(0) while using (7.1.1.9)

**Remake:** Drawn 7 px lower (Progress 25.3.5, render.worn_pose crouch_drop)

### 45. F5 Use durations

- **Verdict:** make identical  **Effort:** S  **User's ask:** none for the item times (the two user recipes have no original time - keep theirs)
- **Files:** `game/data/items.lua`, `game/data/config.lua`

**Original:** 2 s for every food and drink, bandage 2, rags 5, heatpack 3, tetracycline 3, adrenaline 1, blood bag 6, saline 6, IV kit 6, cleaning kit 5, cooked meat and fish 3, MRE 3, scout book 10. Crafts: fireplace kit 3; bag, backpack, canvas, bow, arrows 5; fishing rod 4; patching 3 (8.10.3, 12.10.17)

**Remake:** Invented per item (data/items.lua use_time): beans 2.2, apple 1.2, tomato 1.0, rice 2.4, bandage 2.4, rags 3.2, heatpack 1.4, tetracycline 1.6, morphine 1.8, blood bag 4.0, cleaning kit 4.0; others 1.1 default. Campfire kit 2.5, starter kit 2.0 (data/recipes.lua)

### 46. F6 Messages after a use ('I've eaten Canned beans.', 'No free slots.')

- **Verdict:** unclear  **Effort:** S  **User's ask:** "Remove inventory hints, it's unnecessary"; "remove the \"can't wear that\" text or any type of hint" - these lines report what happened rather than hint
- **Files:** `game/src/game/status.lua`, `game/src/game/inventory.lua`, `game/src/game/crafting.lua`

**Original:** client_log (12.18): a line over the head (white; yellow for drinking from a bottle; red for refusals). While the board is open, a 2x copy at screen centre rises at 48 px/s and fades after 2 s. Examples: 'I've eaten Canned beans.', 'I've drunk Pipsi.', 'I've used Bandage.', 'I'm not bleeding.', 'Weapon fully loaded.', 'Bottle empty.', 'No free slots.' (red), 'I need more wooden sticks for that.' (orig/e3_eat_23.png, orig/e4_eat_31.png, orig/f2_nofree.png)

**Remake:** None, except the asked 'My inventory is full', 'It's empty', 'I don't have ammo for this' over the head

### 47. G1 Food and drink values

- **Verdict:** make identical  **Effort:** S  **User's ask:** "Speed boost from energy drinks" covers the speed only
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`

**Original:** 8.10.3, food/water:
- beans 30/5; tuna 20/0; bacon 35; rice 65
- tomato, apple, banana 15/5
- Pipsi, Spite, Nota-Cola 0/30; kvas 0/50; beer 0/30 +10 heat
- cranberry 15/15; cloudberry 10/10 +30 s immunity; bilberry 10/20; elderberry 10/10 +2 hp
- zucchini 20/20; bell pepper 20/5; orange 15/15
- MRE 40/40 per half (two meals)
- cooked steak 50/10 +15 hp; cooked rabbit 35/10 +30 hp; small fish fillet 20/10 +5 hp; big fillet 30/10 +10 hp
- Nuko Cola sets water to 100; energy drink 0/20 + 60 s speed

**Remake:** data/items.lua, food/water:
- beans 40/0; tuna 35/5; bacon 45; rice 30
- tomato 5/5, apple 12/4, banana 14/2
- sodas 4/20; kvas 6/30; beer 4/18
- all berries 5/2
- zucchini 18/4; bell pepper 5/3; orange 10/8
- MRE 50/10 once
- steak 55/0; rabbit 35/0; fillets 18/0, 30/0 (no hp)
- Nuko Cola 4/20; energy drink 4/20 + 60 s x1.25
(survey/inventory/consumables_cmp.txt)

### 48. G2 Water bottle and canteen are refillable containers

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`, `game/src/game/board.lua`

**Original:** They hold water (count 100 = 4 sips, shown '75%'). A drink takes 25 for +12.5 (bottle) / +25 (canteen). Empty, the item is kept ('Bottle empty.'). 'Fill with water' refills at a pump (8.10.3.47-48; orig/x_submenus2.png)

**Remake:** Single use, +45 / +50, then gone; stack 2 / 1 (data/items.lua)

### 49. G3 Medical items' effects

- **Verdict:** make identical  **Effort:** M  **User's ask:** "effects, like infection, which needs to be cured by tetracycline" (the cure exists either way)
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`, `game/src/game/systems/survival.lua`

**Original:** - Bandage: refused and kept when not bleeding ('I'm not bleeding.', 8.10.3.18.2).
- Rags: 5 s.
- Adrenaline (morphine icon): health to 100 + 10 s speed (8.10.3.39).
- Blood bag +50 hp in 6 s; saline +25 hp in 6 s.
- IV kit: transfusion needing blood ('I need more blood to do it.').
- Heatpack +25 heat in 3 s.
- Tetracycline: cure + 300 s immunity (8.10.3.40). Vitamins: 60 s immunity.
- Whiskey +40 heat +20 water

**Remake:** - Bandage spent even when not bleeding (inventory.use sets applied for stop_bleeding unconditionally); rags 3.2 s.
- Morphine heal 35; blood bag heal 40 in 4 s; saline inert; IV kit heal 45 + stop bleeding.
- Heatpack +45 in 1.4 s.
- Tetracycline: cure only. Vitamins and whiskey inert

### 50. G4 Items the original uses and the remake leaves inert

- **Verdict:** make identical  **Effort:** XL  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/inventory.lua`

**Original:** Usable via 8.10.3 / 12.10: whiskey, saline, vitamins, cigarettes, matches/lighter, every ammo (Load), batteries/radio, F1/smoke grenade, molotov, flare, flare gun, landmine, bear trap, claymore, barbed wire, seeds, fertilizer, fishing rods, raw meat and fish (Cook), knives (melee / Flay), sewing kit, duct tape, epoxy, sharpening stone, hacksaw, car toolbox, gasoline, scout book, protector case, map notes, bandolier, civilian tent

**Remake:** All inert (data/items.lua 'NOT DESCRIBED, and why' block)

### 51. H1 The original's crafting recipes are missing

- **Verdict:** make identical  **Effort:** XL  **User's ask:** "To build a campfire the player needs 1 wood, 1 stick. That makes a campfire kit. Then with 1 stick and 1 newspaper, the player can make a campfire starter kit." - keep those two (they replace the original's fireplace-kit recipes and match/lighter lighting); no ask covers dropping the rest
- **Files:** `game/data/recipes.lua`, `game/src/game/crafting.lua`, `game/data/items.lua`, `game/src/game/board.lua`

**Original:** survey/inventory/craft_menu_table.txt, 12.10.10 / 12.10.17:
- Whiskey + flare -> Molotov.
- 2 burlap sacks -> Canvas (Craft Stash, 5 s); canvas + wood piles -> Place Stash.
- Burlap sack + rope -> improvised bag (5 s).
- Rope + ashwood stick -> bow (5 s) or fishing rod (4 s).
- Wood sticks + knife -> arrows (5 s).
- Wood piles + papers -> campfire kit (3 s); wood piles + rags or + 3 wood sticks -> campfire kit or Build fence.
- 3 rags -> rope; battery + radio.
- Wood sticks onto an improvised bag -> improvised backpack (5 s).
- Sewing kit / duct tape / epoxy onto trousers, jacket, helmet, backpack, vest -> Patch (3 s).
- Sharpening stone onto melee -> +25%; hacksaw -> sawn-off; cleaning kit onto a gun -> Clean; knife onto the melee card -> Take as melee / Holster knife.
- Split sticks; Tear into rags

**Remake:** Two recipes only, the user's: wood + stick -> campfire kit, stick + newspaper -> starter kit (data/recipes.lua). Crafting is listed under Progress.md 'Explicitly out of scope' - the remake's own call, not the user's

### 52. H2 Where a craft's result goes

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (for recipes added under H1)
- **Files:** `game/src/game/crafting.lua`

**Original:** Campfire kit and rope into the inventory (Check_space_inventory; dropped at the feet when full). Bag, backpack, canvas and bow at the feet (Spawn_drop at the player, 12.10.17.26.3)

**Remake:** Into the inventory; refused with 'My inventory is full' when there is no room (crafting.lua 16)

### 53. H3 Crafting sound

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/crafting.lua`

**Original:** Craft_* play nothing; no 1.0 event plays media/crafting.ogg. Patching plays 'taping', tearing 'tear_fabric'

**Remake:** 'crafting' played as the bar starts (data/sounds.lua craft; crafting.lua 187)

### 54. I1 Pick-up sounds missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/item.lua`, `game/src/game/board.lua`

**Original:** pick1/pick2 at -5 dB for anything into a cell (8.12.2.2 and kin); gearpick for garments and helmets; pick_rifle / pick_pistol for guns (8.11)

**Remake:** Silent: no audio call in item.lua, inventory.lua or board.lua

### 55. I2 Drop sounds missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/item.lua`

**Original:** drop1/drop2 for a cell's stack (8.13.1.1); geardrop1/2 for worn gear (8.13.3-10)

**Remake:** Silent

### 56. I3 Use sounds missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/inventory.lua`, `game/src/game/interaction.lua`

**Original:** EatingSoft_0 (-10 dB at the player; EatingCrunchy_0 for rice), DrinkSoda_0, Whiskey (whiskey, bottle, canteen), bandage (-5), vitamins, adrenaline_use, meat_cook, eject (stack, split, unload), reload_pistol (-5, attachments), matchstrike_2 / lighter_0, torch_on, tentpack, tear_fabric, taping (8.10.3, 12.10.17)

**Remake:** No sound for any item use (data/sounds.lua has none; inventory.use plays nothing)

### 57. J1 Inventory button position (touch)

- **Verdict:** make identical  **Effort:** S  **User's ask:** "Tab for inventory backpack icon at the bottom with Tab written underneath it" is the keyboard hint; no ask places the touch button
- **Files:** `game/data/ui/touch_controls.lua`, `game/data/ui/inventory.lua`

**Original:** Bottom-left corner: SetX(viewportleft('GUI_controls')), SetY(viewportbottom(...)) (1.2.10, 12.13.2.1). 0-78 x 640-720 at 1280x720, and it stays there while the board is open (orig/hud.png, orig/g1_ground.png)

**Remake:** Top-right under the minimap (data/ui/touch_controls.lua btn_bag anchor topright x -18 y 126, 100x100 units), and a copy at the board's right edge while open (caps/inv_eat_closed.png, caps/r_open_start.png)

### 58. J3 A tap on the world shuts the board

- **Verdict:** platform  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/inventory.lua`

**Original:** In tap-to-move mode (the default Movement_tap group), a tap outside the board calls close_inventory (12.13.5.3.1). In stick mode the stick is hidden while open and a tap outside does nothing

**Remake:** The dim round the board is modal; only the bag, Tab or Escape shut it. The remake has only a floating stick

### 59. J4 Keyboard keys for the board

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Keybinds, E to pickup ..., Tab for inventory"
- **Files:** `game/data/input.lua`

**Original:** No inventory key at all; the only keyboard support is WASD in its keyboard mode (12.13.7)

**Remake:** Tab toggles; E picks up; Enter and G press the tooltip's buttons

### 60. K1 Item names

- **Verdict:** make identical  **Effort:** S  **User's ask:** none. The user's recipe ask used the words 'wood', 'stick', 'newspaper', 'campfire kit', 'campfire starter kit' (the remake's labels): unclear whether those were meant as renames
- **Files:** `game/src/game/item.lua`, `game/data/items.lua`

**Original:** English names from l_eng_items: 'AK-74', 'Hatchet', 'FNX pistol', 'T-shirt', 'Worker pants', 'Assault vest', 'Combat Gasmask', 'Hoodie', 'Water bottle', '5.45x39 ammo', 'Adrenaline', 'Crossbody bag', 'Hunting backpack', 'Wood piles', 'Papers' (orig/g1_ground.png)

**Remake:** item.title = file name made readable: 'Ak74 rifle', 'Axe red', 'Fnx pistol', 'Tshirt', 'Workpants', 'Razgruz', 'Helmet army', 'Hoodie gray', 'Waterbottle', 'Ammo 5x45', 'Morphine', 'Barsetka', 'Hunter backpack'. 140 of 213 differ (survey/inventory/names_diff.txt)

## Already identical

- Opening and shutting the board with the one bag button, acting on the press (12.13.10 OnTouchObject; also asked)
- The world keeps running while the board is open and the player cannot walk (12.13.10.1 AltMove.Stop; remake player.heading returns nil)
- Picking something up never opens the board (also asked)
- The board can be opened and shut during a timed use and the use carries on (12.13.10 not gated on var#21; remake 25.3)
- Empty-card ghosts (gui_card_cover art), worn-card paper, item icons and weapon pictures are the original's own art
- Drag highlight art and colours: gui_slot_bg green (empty), blue (occupied/swap), yellow (combine/craft)
- Bags and garments keep their contents when dropped and picked up again
- Condition shown at 100% and a gun's rounds shown on the board (format differs, see A7/A8)
- A tap on an empty card does nothing
- Crafting by dragging a ground row onto a carried partner (12.14.5.2.7.1.2; board.lua craft_plan accepts NEARBY rows)
- Timed-use bar art (gui_wpn_cooldown_bg + fill) with a linear fill, and the using_item crouch during a use
- Cleaning kit +25 condition, refused at 0% and at 100%
- Warmth (heat) numbers of 30 garments, e.g. t-shirt 2, jeans 5, hoodie 5, cloak 9, down jacket 8 (B9 lists the 16 that differ)

## Not checked

- The original with a mouse on PC: every original run was headless touch, so whether its inventory drags by mouse was not seen
- Dragging a Ground row onto an occupied cell: the original's Replace_slot_ground reads as a swap with the ground; the remake's answer to the same drop was not captured
- All ~120 item sub-menus live: they were read from 12.13.12 and eight were checked live
- Every recipe's timing live: only the campfire kit's was seen; the rest come from 12.10.17
- The original's inventory tutorial (11.8 Inventory_guide) and its tips
- Sounds were checked by reading only: the original's headless audio fails to decode, and the remake has no inventory audio calls
- The original's exact font faces
- A real phone: 844x390 was checked in emulation only
- The remake's 'My inventory is full' line was not captured (user-asked, so not compared)
- scratchpad/survey/inventory/report.md was not written: the harness refuses report files from subagents, so this output is the report
