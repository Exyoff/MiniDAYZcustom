### 31.12 The seasons as islands: the sea between them, the boats that cross it, the radio stations that find them, and waking on the starter island after a death

*"So maybe, a starter island ... player is able to respawn at starter island. Then after fixing
key locations in the island, like a radio station or something, unlocks the ability to locate
other islands. Then traveling all the way to the right and fixing the boat on shore allows the
player to travel to a new island. traveling back to the left shore in the new island sends player
back to starter island or traveling to right shore sends player to starter island as well. Dying
respawns player at shore of starter island. Zombies at starter island don't respawn."* (2026-10-10),
and decided the same day: the seasons become the islands, the gear carried drops where you died,
and the base is built anywhere on the starter island (31.14). Every season is an island of its own
with sea between each two and an east coast past the last; each island's roads are a network of
their own; every island has a radio station, broken, and mending it finds the next island east;
the starter island's boat, repaired with a wood pile, sails to any island found, and every other
island's two boats sail home. A death ends nothing: you wake on the starter island's beach in a
T-shirt and jeans, and what you had lies where you fell. The starter island's dead stay dead.
Seed 7 has 78 places where it had 72. `generate.VERSION` is 10. Branch `work/islands`.

- [x] 31.12.1 **What the first game and Mini DayZ 2 have, read.** The first game is islands too:
      *"Welcome, survivor. You are trapped on a small island, and your only chance to get to
      another one is to find a boat."* (1.0's `l_eng_ui.xml`). Its boat is `b_exit` in the east
      sea (Game_events 2.11) -- `wooden`, `rubber` and `gas`, each broken (0) and ready (1) --
      and touching it opens Level_menu (8.8.1, 12.5.9): first "There are enemies nearby!" with a
      bandit within 300 px (12.5.1), then "You need wood piles to repair this boat." (Ok) or "You
      can repair this boat with wood piles you have." (Repair, Leave), Repair spending one wood
      pile, item 50, at once (12.5.10.5); its second boat duct tape (item 39), its third a gas
      canister (53); then "Boat is repaired, are you ready to move to the next island? You will be
      unable to come back." (Ready, Not ready). Level_change (8.8.5) drops the menu's sky and
      skyline over the world, three quarters of the screen tall, the loader's line in 12 pt Arial
      light a fifth up from the bottom; the new map starts beside `boat_arrive`, the clock +90
      minutes, "DAY N" typed a letter every 0.3 s in bold 24 pt Trebuchet MS at (200, 500) of its
      GUI layer (Day_show_label, 12.16.1, `t558`), 100-300 XP and an autosave. Mini DayZ 2's
      `names.csv` has three "Radio" rows -- its base's `RadioTower`, "Radio Station", and two area
      presets, `PS_Radio_Tower_Airport` among them -- and none for boat, island, death, lost,
      stash or respawn. Its Radio Station (`base_buildings.json`) is three levels: the first 10
      Material and 600,000 ms, needing its Power Supply (5 Material, 30 s), the second 240 and 5
      Components, an hour, the third 1,440 and 20; a repair 5 Material.
- [x] 31.12.2 **The sea between the islands** (`src/map/shores.lua`, `src/map/coast.lua`,
      `data/islands.lua`). The world across is the coast's lot, five islands of five lots (each
      season's 100 cells), a lot of sea between each two and the east coast's lot: 31 lots,
      37200 px, by 16 down. Each channel's four shorelines are the coast's own seeded walks --
      60-120 px of grass's edge, 180-300 px of sand, 480-600 px of open sea, sand and grass again
      -- drawn by the coast's own pieces through `coast.piece` and `coast.at`, and stopped by the
      coast's own LOW sea collider, a rect down each sea, its face 22 px into the shallows. The
      east coast's sand and sea run to the map's edge. Each season's line runs straight down the
      middle of its sea (`bands.lua`, wander 0): the ground changes under the water. Over twenty
      seeds, from the map tables: 100 shores, no row of any channel whose middle is not sea, the
      narrowest sea 8 cells (480 px), no road cell on the sea, no road across or along a channel,
      no place on a lot of sea; over ten seeds, of 197,212 objects, 3,592 animals and 689
      containers, none stands on the sea, and over twenty seeds none of 15,763 zombie points.
- [x] 31.12.3 **Each island's roads its own** (`links.lua`, `places.lua`, `roads.lua`). The
      lattice's corners in a channel are `net.sea_lines`: `places` reserves the sea's lots and
      fronts nothing there, `links` lays no line across one, and grows one tree an island -- the
      starter island's from the coast's road, every other's from its own road down to its west
      beach (`places.shore_roads`: a main road on a lot line 3 lots or more inside the map's
      north and south edges, a lot inland, the lots either side kept clear) through its cities as
      the first's. A road's end on a beach is an exit. Every reach is asked island by island
      (`roads.reachable`, `places.reachable`). Over twenty seeds: 1,495 places, 71 to 79 a world,
      every one reached by road from every other on its island; 80 shore roads, four a world;
      seed 7's places by island 9, 13, 18, 18 and 20. Generation natively, the twenty averaged,
      185 ms a world. The five seeds of 150 whose every asphalt crossing is taken are now 13306,
      26612, 33265, 46571 and 106448 -- five of twenty-two.
- [x] 31.12.4 **A radio station on every island** (`data/places.lua`'s `radio`,
      `data/maps/prefabs/radio.lua`). A place of its own beside a road, one a season
      (`each_band`), placed before Mini DayZ 2's kinds so every one of theirs stands where it
      stood: Mini DayZ 2's `PS_Radio_Tower`, its airport's piece -- its point, its chest on the
      outbuildings' table, its loot on the ground -- with the first game's radio mast in its fence
      (`ee_radiotower`, `_part2`) and Mini DayZ 2's Radio Station beside it
      (`tools/mdz2/places.py`'s new `extra`). Its picture is the base's visuals 3, 4 and 5 composed
      on one 274x152 canvas, anchor (71, 74), as `radio_station/default_0..2` -- broken (the tent
      and the mast's feet), mended (half the mast, dark), working (the mast whole, its flag and
      second net) -- by the importer's new `states` and `skip` (`props.compose_prop`), its
      collider visual 5's (-73, -62, 192, 124). Over twenty seeds 100 stations, five a world, each
      in a radio place.
- [x] 31.12.5 **The boats** (`coast.boats`, from a stream of its own after the spawn). The
      starter island's on its east sea line, `b_exit` wooden, broken; every other island's on its
      west beach 240-420 px beside its shore road -- `boat_arrive` 0, the boat you came in -- and
      on its east shore, ready: the rubber boat on island 2, the speedboat on every island east
      of it. Each stands 40 px into the water and is reached from 110 px; none within a lot of
      the map's north and south edges. Over twenty seeds 180 boats, nine a world, every one on
      the sea and setting you down on sand. The first game's `b_exit` frames were cut from its
      sheets into `Assets/Sprites/b_exit/`, their origins in `data/sprite_anchors.lua`.
- [x] 31.12.6 **What a boat or a station says** (`src/game/islands.lua`, `data/ui/island.lua`).
      It opens once as you come within reach, and again only after you walk off; walking off, a
      use, the board, the map, Escape and a phone's back key shut it. Laid as the first game's
      running game lays its boat's, measured at 1280x720 in GUI pixels: `bg_rust` 350x224 on the
      middle, its words 16 pt Times in rgb(150, 140, 131) in a box 300 wide 39 px over the middle,
      one plank 154x30 66 px under it or two 97 px either side -- Mini DayZ 2's plain plank, the
      death screen's, with its ink. Its words are the first game's where it has them ("You need
      wood piles to repair this boat.", Repair and Leave, Ready and Not ready, "There are enemies
      nearby!" over the head with a bandit within 300 px), and for what it has none of: "The boat
      is repaired. Sail to island 2?", MORE stepping to the next island found; "The boat is
      repaired, but you know of no island to sail to. A radio station could find one."; "Are you
      ready to sail back to the starter island?"; "This radio station is broken. Mend it with 15
      material?"; "The radio station is working. It found island 3."; and on the last island
      "The radio station is mended. Only static: nothing further east answers." On an 844x390
      phone it fits at GUI scale 0.63.
- [x] 31.12.7 **Repaired and mended under the bar** (`interaction.lua`'s `isle_repair` and
      `isle_mend`, `islands.finish`). The boat: one wood pile, 5 s crouched by it, spent when the
      bar fills; the first game's is instant. The station: 15 Material -- its first level's 10 and
      its Power Supply's 5 -- paid as 31.11's recipes pay out of what the carried things break
      into (`crafting.settle`, split out of `crafting.make`), 3.7 s, 31.11's rule for its 10
      minutes. Island N's station mended locates island N+1 (`islands.locate`, the hook 31.14's
      base Radio Station will call; `islands.located`).
- [x] 31.12.8 **The crossing** (`islands.sail`, `data/ui/curtain.lua`, `data/ui/day.lua`). The
      start menu's sky and skyline fall over the screen in 0.8 s (`drop`, the whole screen moved
      by the share fallen), "Sailing to island 2" under the skyline where the first game types
      its loader's line; under it the player is set down on the other shore -- island N's west
      beach beside its `boat_arrive`, or home on the starter island's east beach beside its boat
      -- 150 px up the sand, the lots there built at once, what is left behind put to sleep, the
      camera snapped; the clock goes on 90 minutes (`daynight.advance`); after 1 s the sky lifts
      in 0.8 s and "DAY N" is typed where the first game types it -- measured, both at x 330, y
      478 at 1280x720 -- and fades. All on the real clock. The run is saved 2 s after. Every
      island reached is remembered.
- [x] 31.12.9 **A death wakes you on the starter island** (`islands.wake`, `data/ui/death.lua`,
      `data/containers.lua`'s `death_pile`). The death screen keeps its stats and says "Your
      things lie where you fell."; WAKE UP stands where RESTART did (RESTART stays on the test
      map). Everything carried and worn -- pockets, backpack, clothes, both guns, the melee
      weapon -- goes into a pile where he fell: the first game's covered body (`corpse_covered_3`),
      32 slots, never restocked (`keep`), walked over and reached from its body (`flat`); it is
      gone, and off the map, once emptied. He wakes at the spawn on the beach in a new run's
      T-shirt and jeans, fists, the meters a new run's, the clock and the day going on, the
      islands found still found. Looked at: the pile's board holding his M4 30/30, the T-shirt,
      the jeans at 88%, the mountain backpack and three bandages.
- [x] 31.12.10 **The starter island's dead stay dead** (`population.update`). No infected's or
      bandit's point west of the first sea's line is refilled; every other island's are, as ever.
      The animals refill everywhere: nothing in the words asks otherwise, and a hunt is how a
      starter island feeds you once its dead are gone.
- [x] 31.12.11 **The map's page shows the islands** (`src/ui/minimap.lua`,
      `data/minimap_symbols.lua`). The sea's lots in the first game's water -- the solid 12x12
      middle of `minimap_tile` 21 stretched over a lot, since its notched edges laid side by side
      left the sea in holes -- an island not found bare paper, one found and not reached at 0.45,
      and over the batch, as they stand, the boats and stations of the islands found and the
      skull of every pile (Mini DayZ 2's `fallen_icon`). The batch is laid again only when the
      record changes. In seed 7 by the first sea: 96 lots of sea, one boat, one station, no pile;
      island 2 found, laid once more, two boats.
- [x] 31.12.12 **The save** (`save.lua`, `islands.capture`, `islands.apply`). The islands found
      and reached, the starter boat repaired, each station broken, mended or working, and the
      piles, through a save and a load, the boat and the station showing their frames again. A
      save from before 31.12 is refused by CONTINUE with the reason, and kept.
- [x] 31.12.13 **"East of" reads by island.** A season's line is mid-sea, so an island's land is
      one season, and every rule that asks the season -- 30.3's loot tiers, 31.4's places, 31.7's
      infected and animals, 31.8's bandits -- asks the island. Over twenty seeds, of 1,495 places
      none lies outside its island's season and none before its kind's `east` (one a season west
      where its `west` allows). The cold rises from the first sea's middle to the last's, 7800 to
      29400 px: none on the starter island, all of it on the snow's.
- [x] 31.12.14 **Scenarios, written and not run.** world: three new over twenty seeds (the sea
      between the islands, each island's network, the stations and boats) and six moved (roads
      joined by island, places reached by island twice, the checkpoint's seeds, the tiles'
      fingerprint, the world's size); save: three new (a save before 31.12 refused, the islands'
      record and a pile through a save, the starter island's dead through a refill and a save)
      and the Stage 25 refusal's version; play: three new (the boats, the stations, a death and
      waking); hud: one new (the page of islands), one renamed.
- [ ] Every scenario: the final pass.
- [ ] The web build: the generation time and seeds 7, 8 and 11's fingerprints in the browser
      (`tools/web_world.cjs`), and a frame at a channel (`tools/web_perf.cjs`). README's 0.65 s is
      31.4's.
- [ ] The first game's later repairs -- duct tape for its rubber boat, a gas canister for its
      speedboat -- and its arrival XP: the remake's boats home are ready, and it has no XP.
- [ ] The first game's end -- its radio on the last island calling the rescue, the final day,
      the helicopter (backlog 88) -- is not in the user's plan.
- [ ] 31.14: its Radio Station calls `islands.locate`; what it keeps from a death is its own.
- [ ] Mini DayZ 2's `Radio_flag` waves; here it stands still in the picture.
- [ ] Twenty comments still call the base 31.12 (`git grep -n "31\.12" 52f7fd30`): left for
      31.14, whose files they are.

Choices, each reversed by one change:
- The words read as hub and spoke: the starter boat sails to any island found, every other island's two sail home. Reverse: the `home` action in `islands.press` sailing to `dl.dest`, with destinations for it.
- One lot of sea between islands and one of east coast. Reverse: `channel` and `east` in `data/islands.lua`.
- The sea 480-600 px wide. Reverse: `lines` in `data/islands.lua`.
- A season's line straight down the middle of its sea. Reverse: the `isles` line in `bands.build`.
- Every island's own road down to its west beach, a main road. Reverse: `shore_road` in `data/islands.lua`.
- One radio station an island, a place of its own, before Mini DayZ 2's kinds. Reverse: `count = { 0, 0 }` on `radio` in `data/places.lua`.
- Mending costs 15 Material in 3.7 s. Reverse: `radio` in `data/islands.lua`.
- The last island's station mended finds nothing. Reverse: `islands.locate(state, r.island + 1)` in `islands.finish`.
- The starter boat repaired by one wood pile in 5 s. Reverse: `repair` in `data/islands.lua` (`time` 0 is the first game's instant).
- The boats home ready, no repair. Reverse: `boats.east` frames to `_0` with a repair of their own.
- A bandit within 300 px stops a crossing, the infected do not (the first game's). Reverse: `bandit_near` in `islands.press`.
- +90 minutes and DAY N on every crossing, home too. Reverse: `sail.minutes` in `data/islands.lua`.
- Everything carried and worn goes into the pile, so the start kit is not a second T-shirt and jeans. Reverse: the equipment loop in `islands.drop_all`.
- The pile kept until emptied, then gone. Reverse: `keep` in `data/containers.lua`'s `death_pile`.
- The meters a new run's on waking. Reverse: the `survival.init` line in `islands.wake`.
- The animals refill on the starter island. Reverse: `dead_stay` asked of the animals' refill too.
- WAKE UP instead of RESTART in a world of islands. Reverse: `visible_when_not` off RESTART in `data/ui/death.lua`.
- An island not found blank on the page, one not reached faint. Reverse: `faint` in `data/minimap_symbols.lua`.
- The first game's MINIDAYZ logo left off the curtain. Reverse: an image row in `data/ui/curtain.lua`.
- The dialog's planks Mini DayZ 2's, the death screen's, not the first game's wood. Reverse: `PLATE` in `data/ui/island.lua`.

*Verified 2026-10-11:* `python tools/verify.py --subject lint` 20/20 after the last change;
`python tools/build.py --verify` (the generated files match: 9 atlas entries, the animations' 20
lines and `sheet_frames.json` without `b_exit`); `python tools/verify.py --list` loads all 544.
Through the console, natively, from the map tables, with probes that ask what the scenarios ask
and are not them: the sea, the roads and the places over twenty seeds, the stations and boats
over twenty, what stands on the sea over ten, the places' seasons over twenty, seed 7's numbers;
the repair, the mend and a crossing in seed 7 by hand ("boat is repaired", "island 2 located").
Looked at, captures at 1280x720: the channel between islands 1 and 2, the starter boat broken in
its shallows, the radio place with its station broken and, later, working, the dialog (and on an
844x390 phone), the sky falling and lifting, island 2's west beach beside `boat_arrive` with DAY
1, the death screen with WAKE UP, the beach woken on, the pile's board, the map's page with the
sea, island 2 faint, the boat and the skull; and the first game's running DAY 1, its curtain and
loader line, its boat dialog's Ok and its Repair and Leave, side by side. **Not run:** every
scenario -- the 10 new and the moved above, and since `generate.VERSION` moved every world, every
scenario on a generated world; the tooling subject (the editor's selftest and places lint over
the new radio prefab, the importer's plan); the web checks.

- **26.3.2** (2026-10-11): the coast widens the world now: the sea between the islands and the east coast are five lots more (31.12).
- **26.3.4** (2026-10-11): the sea's body has a rect down each channel and the east coast too (31.12).
- **26.3.5** (2026-10-11): every island's road down to its west beach is an exit as the coast's (31.12).
- **26.4.3** (2026-10-11): in a world of islands a season's line is straight, down the middle of the sea; nothing is ragged where nothing is drawn (31.12).
- **27.2.1** (2026-10-11): the world is 31 lots, 37200 px, across (31.12).
- **27.3.3** (2026-10-11): every place is joined to every other on its island; no road crosses the sea, one network an island (31.12).
- **27.3.11** (2026-10-11): the five seeds whose crossings are all taken are 13306, 26612, 33265, 46571 and 106448 (31.12).
- **27.4.4** (2026-10-11): in a world of islands the page lays the sea in the water glyph, an island not found bare and one not reached faint, and the boats, stations and piles over it (31.12).
- **25.1.1**, Stage 15's death screen (2026-10-11): in a world of islands a death ends nothing; WAKE UP wakes you on the starter island and RESTART is the test map's (31.12).
- **30.3.7** (2026-10-11): `generate.VERSION` is 10 (31.12).
- **31.11** (2026-10-11): its workshop waits for 31.14's base, not 31.12's; the stages were numbered again.
