# events: the remake against the official 1.0

*Area surveyed:* Events, progression and everything else. World events (airdrops, helicopter crashes, the bunker, hordes, bandits, survivors and the friend, traders, fishing, hunting); experience, perks, karma; achievements, unlocks and what each unlocks (characters, perks and `perk_cost`, start kits, weapons, items); character select; the islands and how a run moves from one to the next, and how it ends; saving and continuing; score and stats at death; the tutorial, tips and hints; login and anything account-like; and every event-sheet group no other survey covers.

Surveyed 2026-10-06 against the remake at 75d9a88 (Stages 27.1 to 28 merged), in the read-only worktree `wt/survey`.

Shots, logs and helpers are under `S = C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/events`:
- `S/orig/*.png`: the original, driven by `../orig.cjs` (headless Edge, 1280x720, `--touch`, Novice, noon). `S/orig/a1.log` and `S/orig/probes.txt` hold the state dumps; the page scripts are `S/probe.js`, `S/bots2.js`, `S/bots3.js`, `S/bleed.js`, `S/drop.js`, `S/level.js`, `S/cars.js`, `S/tpcar.js`, `S/tpcamp.js`, `S/tpbandit.js`, `S/seedbtn.js`.
- `S/rm/*/shot_*.png` and `run.log`: the remake, through `S/rrun.sh` (`lovec.exe game --shot --res 1280x720 [--world 7]`, APPDATA under `S/appdata`).
- `S/cmp/m_menu.png`, `S/cmp/m_death.png`: side by side, original left.
- `S/ev.py` and `S/tx.py` print an event path from the dumps (`show.py` fails on Windows), `tx.py` with the `Arr[t543]` / `Arr[t911]` / `Arr[t544]` texts filled in from `S/l_eng_ui.tsv`, `l_eng_new.tsv`, `l_eng_log.tsv`, `l_eng_items.tsv` (made by `S/xml.py`).

Event paths are Game_events unless a sheet is named (ME = Menu_Events). `ui N`, `new N` are ids in `l_eng_ui.xml`, `l_eng_new.xml`.

## Summary

**What I read.** Original, Game_events:
- 2.1 to 2.9 (the seed groups), 2.10, 2.12 to 2.16 (the start, the character's kit, a load);
- 3.1 to 3.3 (the bunker, the bridge level, bunker spawners), 3.6.3 (the end of generation), 3.8 (each island's settings), 3.14, 3.15 (crashes, bunker entrance);
- 4 (every character and weapon unlock), 5 (Save_loot, Load_loot), 6.1, 6.4 to 6.19, 6.11, 6.12 (counters, karma, experience);
- 8.1, 8.2 (the helicopter, the final day), 8.4, 8.8 (Level_change), 8.16 to 8.23 (gardening, the radio, page hidden, fish, the stash);
- 9.40 to 9.42 (car respawns), 10 (death screen, achievement recount), 11 (the tutorial), 12.2 (tasks and gifts), 12.4 (airdrop), 12.5 (the boat and bunker dialogs), 12.6.4 (the notebook's pages), 12.7 (perks and stats), 12.12, 12.16, 12.17 (toasts);
- 13.4 (the Bot Manager and spawn functions; the bots' AI skimmed), 13.5.3 and 13.7.1.21 (deer, the rare deer), 17.5 to 17.8 (attack waves), 19, 22, 23 (Timeline), 26 (Restart_game), 29 (saving), 31 to 33.

Menu_Events 3.2.2 (every menu button), 3.2.3, 3.2.6 to 3.2.8, 3.2.21, 3.9, 4 to 7, 8, 30 to 37, 47, 50, 51; Login_events; Everywere; Tutorial_events; `globals.txt`, `objects.txt` (the 30 achievement objects), `missing.txt`, and the English texts. Remake: Progress.md (where this stands, ground rules, out of scope, 25.1, 27.1.13, 28.2), `src/game/save.lua`, `src/app.lua`, `data/config.lua` (save), `src/game/state.lua`, `data/ui/death.lua`, `data/animals.lua`, `src/map/wildlife.lua`, `data/items.lua`, `src/core/persist.lua`, and a search of `game/` for every system named here.

**What I ran.** In the original:
- the menu, Achievements, Unlocks and its Character, Weapons and Items pages, a locked and a free character;
- a Novice run with the seed switches and the Timeline read as `Stats_time_lived` was set to 200, 610, 1510, 3010 and 5010;
- the Bot Manager watched for 150 s with nothing touched, then near a bot spawn point;
- the perks screen with and without XP, an achievement toast, a character-unlock toast;
- the flare gun fired through the game's own `item_activate`, and its airdrop landing;
- the boat touched, its dialogs, Ready pressed, and the arrival on island 2 read and shot;
- the drivable cars counted and one visited; a survivor camp flagged and walked to;
- a first zombie contact (the tutorial's bleeding tip); a death with stats set, and Go to menu.

In the remake: the start menu, a death on Novice in world 7, and whether START saves (it does not).

**The biggest gaps.**
1. **Every normal 1.0 run has bandits, survivors and a friend from the first minute.** The menu sets `SEED_pubg = 1` and `SEED_morezeds = 1` for every run with `SEED_RUN = 0`. The seed groups stay off, but `SEED_pubg` alone makes the Bot Manager (13.4.1.1) spawn a melee bandit, an armed bandit, a friend, a survivor camp and a bandit camp every 60 s, whatever the spawn switches say. Live: a melee bandit, an armed one and a friend 74 s into a Novice run (finding 1). The remake has no human NPC at all.
2. **The original is a journey with an end.** Repair the boat on the east coast (wood piles, then duct tape, gasoline), cross five islands, take the bridge by car, call for rescue on the radio, survive one more day, and "You successfully escaped." The remake's run ends only in death (findings 14, 15).
3. **No progression layer.** Experience (the Score), 16 perks, karma, 30 achievements with tiers by difficulty, 20 characters with kits and perks, item unlocks, lifetime stats: none in the remake (findings 4, 9 to 13).
4. **World events of a normal run are missing.** The flare gun's airdrop, a horde on a deployed tent every 3500 s, survivor tasks and gifts, drivable cars on every island. And on later islands: helicopter crashes, the bunker, the rare deer (findings 3, 7, 8, 16 to 19).
5. **Saving.** The original saves every 90 s, as soon as a world is made or loaded, and on taking a task. The remake saves every 180 s and not at START, so a page shut in a new run's first three minutes leaves CONTINUE on the previous run (finding 22).

What `SEED_morezeds = 1` does, the other open lead: it blocks the Timeline's 180, 3000 and 5000 s steps (23.2 to 23.4). Two of the variables those steps write, `Zed_per_base` and `fast_zeds_spawns_on`, are never read anywhere; the bot switches are overridden by `SEED_pubg`'s tick; the tanks and the boss bandit are on from the start. So its one effect in play is that the first island never turns to snow after 5000 s, which corrects survival.md 13 (findings 5, 6).

**What is kept and why.** Nothing in this area is a plain keep, but three things depend on the user's words:
- The five islands are one world with the seasons side by side (Stage 26, Stage 27; world 3 keep). How the boat, the islands' arrival and the radio ending map onto that world is the user's call (findings 14 to 19).
- The tutorial: the user said "there will be a tutorial later" (Stage 23). 1.0 ships one that its menu never reaches (finding 25).
- Progress.md's "Explicitly out of scope" lists bandits, vehicles, achievements, perks, login and the game modes. That list is in the project's first commit, the Stage 0 plan, not one of the user's quoted asks. The Stage 28 instruction ("identical except for the changes i had asked for specifically") would bring them in, so the user has to say whether the list still stands. Findings on that list are "unclear"; those it does not name (the friend, survivor camps, karma, experience, the airdrop, hordes) are "make identical".

**Corrections to earlier surveys.**
- items.md's Not checked (line 965) calls the airdrop "a perk's". It is the flare gun's (finding 7); `gui_airdrop_icon` and `Airdrop_recharge` are dead.
- survival.md 13: no snow from staying alive (finding 6).
- survival.md 37: radiation and the red zone are unreachable in 1.0, so that one is already identical (see below).

**Covered in other surveys, cited not repeated.**
- The death screen's look, texts, button and permadeath (menus 32 to 35); the Achievements, Unlocks, Facebook and MINI DayZ 2 buttons (menus 15).
- The portrait and its XP number (hud 8), the perks screen itself (hud 9), the notepad and its new-game tip (hud 11), the DAY label (hud 7), the sleep button (hud 13).
- The notebook's Guides, Crafting and Tasks tabs and GLOBAL MAP (world 39); its markers (world 38); each later island's quota and kinds (world 4); the east coast (world 31).
- One character (character 26); unlock-gated items (items 21); the camps', crashes' and bunker's loot (items 10, 11, 19); the tent, traps and fishing rods (items 34, 36, 37).
- The zombie kinds behind the `Zombies_active_*` switches (zombies 4); sound when the page is hidden (audio 6).

## Findings

Verdicts: make identical 13, unclear 12, platform 1

### 1. Bandits from the first minute: wanderers with fists and with guns, bandit camps, the boss

- **Verdict:** unclear  **Effort:** XL  **User's ask:** none. Progress.md "Explicitly out of scope" lists "The bandit faction AI (~2,500 lines in the original)". That list dates from the Stage 0 plan (first commit), not from the user's words. Whether it still stands after the Stage 28 instruction is for the user to decide.
- **Files:** new `game/src/game/systems/bots.lua` and `game/data/bots.lua`; `game/src/game/systems/population.lua`, `targeting.lua`, `combat.lua`

**Original:**
- **Why they appear in every run.** Every normal run has `SEED_pubg = 1` (ME 3.2.3, 3.2.6, 3.2.8.2; Restart_game 26). While it is 1, the Bot Manager's tick runs every 60 s (13.4.1.1) and ignores the spawn switches.
- **The wanderers.** `Check_bot_puncher` and `Check_bot_firearm` put a melee bandit (`bot_enemy_puncher`) and an armed one (`bot_enemy_shooter`) on a random one of the map's 18 `bot_respawn_point`s. The point must be 700 to 3000 px from the player, and a bandit past 2600 px is removed (13.4.1.7, .8, .13, .14).
- **The camps.** `Bot_bandits_check_situation` flags the nearest wild location (kinds 9, 11 to 14, 17, 21) for a bandit camp when none stands (13.4.1.22, .23). Its three bandits appear when the location loads: rifle, pistol or the boss.
- **The other cadences only add:** punchers every 180 s, gunmen every 300 s, camps every 400 + 2 x karma s from island 2 (13.4.1.2 to .6).
- **What they do.** Bandits hunt the player and the survivors. One that walks onto a stash within 400 px of the player destroys it (8.23.3). Each kill counts in "Bandits killed" and gives XP: 15 for a melee wanderer, 50 for an armed one, 25, 50 or 100 for a camp's pistol, rifle or boss (13.2.8.8 to .10). The boat refuses with "There are enemies nearby!" (new 202) when one is within 300 px (12.5.1).
- **Live on Novice.** 74 s in: a melee bandit, an armed one and a friend; then a camp of three when walked near (`S/orig/k1_bandit.png`; `S/orig/probes.txt`, k1).

**Remake:** No bandits of any kind. Only the infected and the animals move.

### 2. A friend: the survivor who joins you

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (not on the out-of-scope list; it shares finding 1's bot code)
- **Files:** new `game/src/game/systems/bots.lua`; `game/data/ui/hud.lua` (the friend button)

**Original:**
- **When.** The same 60 s tick's `Check_bot_friend` puts `bot_friend` on a bot point 2000 to 5000 px away when there is none, and removes it past 4000 px (13.4.1.9, .15). 13.4.1.4 also checks every 500 - 3 x karma s once `friend_bot_spawns_on` is set, 600 s in (Timeline 23.5).
- **What it does** ("Veteran friend", 13.4.7):
  - follows and fights;
  - is told to follow or guard through `gui_friend_btn` (13.4.7.2.21, .22);
  - gets into a car with you and bails out (13.4.7.2.19, .20);
  - has an hp bar and a minimap marker.
- **Achievement.** Meeting one counts for "BRO" (ui 176 "Meet a friendly survivor").
- **Live.** A friend spawned 39 s into a run left alone (`S/orig/probes.txt`, bots3).

**Remake:** None.

### 3. Survivor camps, their tasks and their gifts

- **Verdict:** make identical  **Effort:** L  **User's ask:** none. The "deal with bandits" task needs finding 1's camps; without bandits that task kind goes.
- **Files:** new `game/src/game/systems/bots.lua`, `game/data/tasks.lua`; `game/data/ui/map.lua` (the Tasks tab, world 39)

**Original:**
- **Camps.** `Bot_team_check_situation` runs on the 60 s tick and every 300 s (13.4.1.19, .20). It flags the nearest wild location for a camp of three `bot_enemy_teammate`. Live: three after walking to it (`S/orig/c1_camp.png`, probes.txt c1).
- **Tasks.** The talk button (`gui_btn_talk`, 12.13.3.4) opens `Quest_menu` (12.2.1), one task at a time:
  - bring an item from a list ("Hello there! Our group is looking for ...");
  - wipe out a bandit camp;
  - bring back a lost loot case, which you may open and steal from instead.
- **Rewards.** Named up front, from 42 (12.2.1.1.1.1.5): food, ammunition, guns, medicine, or "Coordinates where a drivable vehicle is located", "... of an underground bunker entrance", "... of a boat we saw on the east coast".
- **The rest of a task.**
  - A 500 s timer and an entry in the notebook's Tasks tab (12.6.4.1.4).
  - Completion gives 50, 100 or 150 XP and +10, +10 or +20 karma (12.2.2.3 to .5).
  - Cancelling costs -10 karma and stealing -20 (12.2.10, 12.2.11).
  - Accepting saves the run.
- **Gifts.** At top karma, a camp gives a gift instead (`Gift_menu`, 12.2.3): one of 20 items, "Hi! We heard about you! You was helping people in trouble, and we want to help you! Take this:". The minimap shows its mark (6.16).
- **Traders.** There are none; this is the nearest thing.

**Remake:** None. The notebook has no Tasks tab (world 39).

### 4. Karma

- **Verdict:** make identical  **Effort:** S (on top of 1 to 3)  **User's ask:** none
- **Files:** `game/src/game/state.lua`, the bots module

**Original:**
- **Range.** `karma_points` runs from -111 to 89 and is shown +11, so -100 to +100 in the Stats tab (6.11, 12.7.2.2).
- **What moves it.** +10 per bandit killed and -20 per survivor killed (13.2.8.10.4, .11.5); -20 for turning a camp hostile (13.4.3.2.1.1.1); the task amounts in finding 3.
- **What it moves.**
  - How often a friend is looked for (500 - 3 x karma s) and a bandit camp (400 + 2 x karma s).
  - Gifts at the top (finding 3).
  - Jasmine's unlock: down to -100 then up to +100 in one run (6.11.1.1, 6.11.2.1).

**Remake:** None.

### 5. The Timeline: what a run's clock switches on, and what SEED_morezeds stops

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/population.lua`, `game/data/zombies.lua`

**Original:** Timeline (23) acts on `Stats_time_lived` (Analytics 19 adds 1 a second).
- **Steps that fire.**
  - 12 s on day 1: `melee_bot_spawns_on` (23.1).
  - 600 s: the friend and gunman switches (23.5).
  - 1500 s, or any later island: jumper, red hazmat, rifleman, riot, sleeper2 and spitter all set to 1 (23.6).
- **Steps that never fire.** 180 s (fast infected, punchers), 3000 s (gunmen, boss bandit, tanks) and 5000 s (`rains_level` 2) need `SEED_morezeds = 0` (23.2 to 23.4). Every normal run has it at 1 (ME 3.2.3, 3.2.6, 3.2.8.2; 26).
- **Measured** (`S/orig/a1.log`):
  - at 204 s `fast_zeds_spawns_on` stayed 0;
  - at 614 s the friend and gunman switches went to 1;
  - at 1514 s `Zombies_active_sleeper2` went to 1;
  - at 5014 s `rains_level` stayed 1.
- **Dead variables.** `Zed_per_base` is written in 31 places and read in none; so are `fast_zeds_spawns_on`, `hordes_spawns_on` and `zed_horde_alive`.
- **In play**, the clock does two things: the bot switches (already overridden by finding 1's 60 s tick), and from 1500 s the buried infected (sleeper2). The other six kinds are on from the first second (Restart_game 26; live at 11 s).

**Remake:** Nothing changes with time alive. The kinds behind the switches are missing (zombies 4). When they come, the buried infected should wait for 1500 s alive or a later island; the others start on.

### 6. No snow from staying alive: survival 13's "after 5000 s" does not happen

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** the weather system survival 12 and 13 propose; `game/src/map/bands.lua`

**Original:**
- **On the first island,** `rains_level` stays 1 after 5000 s, because 23.4 is blocked (finding 5; `S/orig/a1.log` at 5014 s).
- **Snow falls only where `rains_level` is 2:** islands 4 and later (`CurrentLevel` 3 and up, 3.8.4 to 3.8.6.1). Those are the `ground_lvl4` (dry) and `ground_lvl5` (snow) sheets. Island 3 (`ground_lvl3`, frosted) has `rains_level` 1, rain (3.8.3).

**Remake:** No weather yet. survival 13 proposes snow "in the frosted, dry and snow bands ... and everywhere after 5000 s". Both parts should change: no time rule, and rain, not snow, in the frosted band.

### 7. The flare gun calls an airdrop

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/items.lua` (flaregun), new `game/src/game/airdrop.lua`, `game/data/containers.lua`

**Original:** The flare gun is item 103: "Flare gun. After shot it calls airdrop. Airdrop will be delivered in 20 seconds." (new 103).
- **Where it works.** Used outdoors and on foot (8.10.3.90). Indoors or during a bunker raid it says "I can't do it here"; in a car it is refused.
- **The shot.** It fires a flare upward with a 500 px light fading over 15 s. It leaves an `airdrop_landing` at the player and makes a 1500 px noise every 2 s.
- **The drop.** After 15 s a crate (`airdrop_drop`) comes down by parachute on that spot, or into the middle of the room if the spot is in a warm zone (12.4.10). It becomes `bunker_lootbox` kind 53 with an `airdrop_parachute` (12.4.3), which opens on walking into it (3.1.10). Its contents are items 19's list for 53.
- **Live.** The type-53 box lay 24 px from the player 16 s after the shot (`S/orig/p5_flare.png`, `p6_airdrop_fall.png`, probes.txt drop).
- **Dead code.** The `gui_airdrop_icon` picker and `Airdrop_recharge` per difficulty (12.4.1, 12.4.6, 12.4.9) are leftovers: the icons are placed only on GUI_layout, and `airdrop_box` (12.4.5, 12.4.7) is never created.

**Remake:** The flare gun is inert and unplaced (items 32). No airdrop.

### 8. A deployed tent draws a horde

- **Verdict:** make identical  **Effort:** M (the tent itself is items 37)  **User's ask:** none
- **Files:** `game/src/game/systems/population.lua`, the tent of items 37

**Original:**
- **The wave.** Every 3500 s, if a civilian tent is deployed, `Spawn_attack_wave` runs at a random one (17.5). It makes a frame-15 `zed_resp_point` 1550 px from the tent, toward (8899, 6788), at the first of five angles (45, 22, 0, -22, -45 degrees off that line) not in water (17.8); its group walks in.
- **The noise.** Standing within 300 px of a tent makes a 1500 px noise every 5 s (17.6).
- **No other hordes.** The final day uses the same waves (finding 15); `hordes_spawns_on` is never read.

**Remake:** No tent, no attack waves.

### 9. Experience: the Score, and what earns it

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (not on the out-of-scope list; the death screen's Score, menus 33, needs it)
- **Files:** `game/src/game/state.lua`, `game/src/game/systems/combat.lua`, `game/data/ui/death.lua`, `game/data/ui/hud.lua`

**Original:** `expierence_points` comes from:

| Source | XP | Event |
|---|---|---|
| An infected killed, by a round or a blow | 8 | 13.2.8.4.4.5 to .9, 13.3.1.2.7, .8 |
| A tank / a jumper | 50 / 15 | 13.2.8.6.4, 13.3.8.1.4 / 13.2.8.7.4 |
| A deer / a rabbit / a wolf | 10 / 5 / 5 | 13.2.8.1 to .3 |
| A melee bandit / an armed one | 15 / 50 | 13.2.8.8.4, .9.4 |
| A camp bandit with a pistol / rifle / the boss | 25 / 50 / 100 | 13.2.8.10.4 |
| A survivor (and -20 karma) | 30 | 13.2.8.11.5.1 |
| Reaching island 2 / 3 / 4 / 5 / later | 100 / 150 / 200 / 250 / 300 | 12.16.1.9 to .13 |
| A task done | 50 / 100 / 150 | 12.2.2.3 to .5 |
| The secret location found | 50 | 22.3 |
| A helicopter crash / a humvee crash found | 25 / 15 | 6.1.2, 6.1.3 |
| A car trunk searched | 2 | 12.13.3.1.2.1.1 |
| Nuko Cola drunk / the Scout book read | 200 / 200 | 8.10.3.55, 8.10.3.102 |

- **Shown** under the portrait (hud 8), in the Stats tab (finding 10), and as "Score" at death (menus 33; `S/orig/d1_dead.png`). Spent on perks.
- **Start value.** 0 on the first run after the page loads, and 16 after any return to the menu (Restart_game 26 sets 16; nothing resets it at the start).
- **Live.** 100 on arriving on island 2 (probes.txt level).

**Remake:** None. `state.stats` holds `zombies_killed` alone (`src/game/state.lua` 30).

### 10. Perks, and the Stats tab

- **Verdict:** unclear  **Effort:** L  **User's ask:** none. Progress.md "Explicitly out of scope" lists "Perks (revisit after Stage 15)" (the Stage 0 plan). hud 9 left the screen unclear for the same reason.
- **Files:** new `game/data/perks.lua`, `game/data/ui/perks.lua`; the systems each perk touches

**Original:** 16 perks of two levels each (ui 400 to 415): Sharpshooter, Scout, Sprinter, Camel, Hamster, Snowborn, The Bullet Farmer, Survivalist, Blocker, Puncher, Trunk Digger, Hunter, Sweet home, Red+, Vigilance, Agronomist. For example, Red+ lvl 1 halves blood loss when bleeding, and lvl 2 stops bleeding twice as fast.
- **The screen.** Bought from the portrait's screen (12.7.5, 12.7.11) with "MD_Perk_1" (`S/orig/p1_perks.png`, `p2_perks_xp.png`).
- **`perk_cost`.**
  - 200 on a page's first run (globals) and 300 after any return to the menu (Restart_game 26).
  - Each buy adds 100 + 20 x perks learnt (12.7.11.18).
  - The remake's `original_globals.lua` carries 1.2's 100 (README).
- **Where perks come from.** Characters 4 to 20 start with some (finding 12). Learning all of them earns "Survivor God" (12.7.14).
- **The Stats tab:** Score, Minutes alive, infected killed, Bandits killed, Karma, Days survived, Difficulty (12.7.2.2).

**Remake:** None.

### 11. Achievements, their toasts and the lifetime stats

- **Verdict:** unclear  **Effort:** L  **User's ask:** none. Progress.md's out-of-scope list names "Achievements" (Stage 0 plan); menus 15 left the button unclear.
- **Files:** new `game/data/achievements.lua`, `game/data/ui/achievements.lua`; a profile file beside `game/src/core/settings.lua`

**Original:**
- **The list.** 30 achievements (the `fam_ach_24` family, thresholds in var#4 to #6; names ui 101 to 130, tasks ui 151 to 180). For example:
  - 24 hours: survive 1, 2 or 3 days;
  - Zedkiller: kill 20, 40 or 80 infected;
  - Blackbox Collector: find 1, 2 or 3 helicopter crash sites;
  - Tourist: visit a city on 1, 2 or 3 islands;
  - BRO: meet a friendly survivor;
  - Deerhunter: flay 1, 2 or 3 animals.
- **Tiers.** Bronze on Novice, up to silver on Regular, gold on Veteran and Legend (12.17.1, 10.1.1). Counts are per run: Restart_game zeroes them.
- **Mid-run.** Reaching a tier pops the badge at top centre for 4 s with "md_achievment unlocked" (12.17.2; `S/orig/p3_ach_toast.png`).
- **Kept.** Ranks are recounted at death and on leaving to the menu, and kept in localStorage "achieves" (10.1.1.2 to .4).
- **The screen** (`S/orig/m1_achievements.png`):
  - the rule ("To unlock achievements, you must play a whole session until your character dies ...");
  - Overall stats: Minutes alive (the best), Infected killed total, Bandits killed total, Deaths (10.3.12 to .15);
  - the list with ranks.
- **Not counted** in a seed run, or off the Map.

**Remake:** None. Nothing is kept across runs but settings.

### 12. Twenty characters, their kits and perks, and how each is unlocked

- **Verdict:** unclear  **Effort:** L  **User's ask:** none. character 26 left one character unclear, and the unlocks ride on the achievements and perks above. The three free survivors need neither and could come in alone.
- **Files:** `game/src/game/player.lua`, new `game/data/characters.lua`, new character select under the menu

**Original:** Unlocks, then Character (`S/orig/m2_characters.png`, `m3_char_locked.png`):
- **The strip.** 20 portraits; the locked ones are grey. A tap shows the kit, or the requirement; Choose and Current mark the pick.
- **The pick is kept** in localStorage "Player_skin" (ME 4.1.1), and the start sets the portrait and the kit (2.13.7 to .26).
- **Unlocking** (4.1 to 4.17):
  - `unlock_char_N` stores "char_N" and pings the Unlocks button;
  - the toast "New character unlocked!" plays with its sound (12.17.3; `S/orig/p4_char_unlock.png`);
  - only on the Map, never in a seed run, only on the difficulty named.

| # | Character | Start kit and perks (ui 80 to 94, new 180 to 186, 633) | To unlock (ui 71 to 79, 140 to 142, new 181 to 187, 632) |
|---|---|---|---|
| 1-3 | Survivor 1, 2, 3 | jeans, t-shirt; none (white, yellow, black hands) | free |
| 4 | Billy "Red" | jeans, shirt, bandana; Sharpshooter | 5 shotgun kills, any difficulty |
| 5 | Emilio "Doc" | paramedic pants and jacket, bandage; Red+ | stop bleeding 6 times, any |
| 6 | Dave "Boxer" | jeans, tracksuit jacket, rags; Blocker | 10 bare-hand kills, any |
| 7 | Lee "Maniac" | jeans, cloak, crowbar, molotov; Sweet home | kill a wandering melee bandit with an axe, Regular+ |
| 8 | John "Biker" | jeans, rider jacket, moto helmet, pipe wrench; Sprinter | carry those three at once, Regular+ |
| 9 | Sergey "Slav" | tracksuit pants, down jacket, balaclava, bat; Puncher | kill 3 survivors in a run, Regular+ |
| 10 | Ganzo "Dragon" | hunter pants, heavy crossbow, 10 arrows; Sprinter, Snowborn | kill 4 armed survivors, Veteran+ |
| 11 | Tom "Survivor" | jeans, sweater, hunter knife, improvised bag, pan; Hunter, Survivalist | survive 7 days, Veteran+ |
| 12 | Garry "Veteran" | jeans, t-shirt, cap, complex M armour, MP5k, ammo; Sharpshooter, Scout | carry the Gorka set at once, Veteran+ |
| 13 | Kate "Tourist" | jeans, raincoat, civilian tent, pickaxe; Hunter, Vigilance | find two drivable vehicles on island 4+, Veteran+ |
| 14 | Jasmine "Lumber" | hunter pants, t-shirt, beret, hatchet, duct tape; Hamster, Camel, Snowborn | karma -100 then +100 in a run, Veteran+ |
| 15 | Aika "Stringer" | jeans, t-shirt, cap, press vest, canteen, radio, hammer; Sprinter, Scout | 3 tasks in a run, Veteran+ |
| 16 | Xavier "Pilot" | gorka pants, sweater, tactical helmet, M60, flare gun; Survivalist 2, Scout | 13 helicopter crashes in a run, Legend |
| 17 | Rudolf "Jager" | Mosin, 10 rounds, hunter knife, lighter, hunter jacket, pants, backpack; Hunter 2, Vigilance | kill 3 albino deer, Legend |
| 18 | Taman "Guard" | police hat, orel jacket and pants, balaclava, AKS-74u, 5.45 silencer; Sharpshooter 2, Bullet farmer | kill a Tank with the Humvee's machine gun, Legend |
| 19 | Daria "Scientist" | worker pants, paramedic jacket, AN-94, RDS, tetracycline, vitamins, adrenaline, 30 5.45; Red+ 2, Sprinter | reach island 5, Legend |
| 20 | Mike "Designer" | awesome hoodie, worker pants, sword, M16A2 with magpull, ACOG, silencer, M203, 31 grenades, 90 5.56; Sharpshooter, Puncher, Sprinter, Scout | reach the unknown island, Legend |

**Remake:** One character (`player_skin_def_1`) and no select screen. The other skins are not in `Assets/Sprites`, but cut.py cuts them from 1.0 (missing.txt).

### 13. The Unlocks menu: Character, Weapons and Items pages, and their pings

- **Verdict:** unclear  **Effort:** M  **User's ask:** Stage 22 "Also work on the start menu; Continue, Start, Settings." does not say whether those three are the whole menu (menus 15).
- **Files:** `game/data/ui/menu.lua`, new pages

**Original:** Unlocks offers Character, Weapons, Items and Back, each with an orange ping until visited (char/wpn/item_update_avalible; `S/orig/o1_unlocks.png`).
- **Weapons** (`S/orig/m4_weapons.png`): Deagle, MAC-10, Saiga-12k, AUG, VSS, SV-98 and Katana, each with its requirement or "Unlocked. Locations: ..." (ui 392 to 397, 416 to 419, new 189 to 192).
  - All seven start unlocked in 1.0: the globals are 1, and the storage events never set them to 0 (ME 7). So a fresh profile reads "Unlocked" and no gun is gated.
- **Items** (`S/orig/m5_items.png`): plaster, lighter, smoke grenade, laser sight and Officer's keycard. Each is unlocked by escaping (finding 15) with a set of characters (10.3.1.1 to .5); the gating itself is items 21.
  - A 1.0 bug: the page appends the MAC-10's "0/200" to the plaster's line (ME 3.2.2.28.1.6).

**Remake:** None.

### 14. The boat on the east coast, and the next island

- **Verdict:** unclear  **Effort:** XL  **User's ask:**
  - Stage 26 "Traveling right slowly gets colder and eventually the tiles change" and Stage 27 "make the biomes wider, like 100 across" put the islands' seasons side by side in one world (world 3, keep).
  - world 31 makes the east coast, where the boat lies, identical.
  - Whether a boat there ends the run, leads somewhere, or is left out is the user's call.
- **Files:** `game/src/map/coast.lua`, `game/src/map/generate.lua`, `game/src/app.lua`, `game/src/game/save.lua`

**Original:**
- **The boat.** `b_exit` lies in the east sea (x 17241, 2.11). Touching it with no bandit within 300 px opens a dialog (8.8.1, 12.5.1):
  - island 1: "You need wood piles to repair this boat."; with wood piles, Repair spends one (12.5.10.5);
  - island 2: duct tape; island 3: gasoline, Fuel;
  - then "Boat is repaired, are you ready to move to the next island? You will be unable to come back." Ready or Not ready (`S/orig/l2_boat_dialog.png`, `l3_boat_ready.png`).
- **The other crossings.**
  - Island 4's exit is a bridge, crossed only in a car ("I can try to jump overthere on a car.", 12.5.11; the bridge level 3.2), then "Are you ready to go on 5th island? There will be no way back."
  - From island 5 the bunker's elevator (Level_menu 99, "Are you ready to use elevator? There is no way back.") leads on to "unknown islands".
- **Leaving** (Level_change, 8.8.5):
  - `CurrentLevel` + 1;
  - Map Notes and Officer's keycards taken from the hand slot;
  - Save_loot (5.2) writes the player's health and meters, and everything worn and held with its contents;
  - the task is cancelled; the sky curtain falls; the map is generated anew (`S/orig/l4_changing.png`).
- **Arriving.**
  - On the west beach beside a `boat_arrive` (2.13.2 to .4), across the bridge (2.13.5), or out of the evac elevator at the top (2.13.6).
  - Load_loot puts the gear back on; the clock moves on 90 minutes (3.8.2 to .6).
  - "DAY N" and 100 to 300 XP (12.16.1.9 to .13); an autosave.
- **Each island has** its own ground sheet (lvl1 to lvl5), quota and place kinds (world 4), its own spawn switches, crashes (finding 16) and bunker (finding 17). Rain turns to snow from island 4 (finding 6), and Day_cold is 9 from island 5.
- **Live, island 1 to 2** (probes.txt level; `S/orig/l6_island2b.png`): +100 XP, 2 helicopter crashes, 3 humvee crashes, a bunker entrance, the brown lvl2 ground, the clock on.

**Remake:** One world with the five sheets in bands; no boat and no exit. East of the snow band is the bounds wall (world 31).

### 15. The radio, the final day and the rescue helicopter

- **Verdict:** unclear  **Effort:** L  **User's ask:** the same as 14: in the original this is the last island's exit, so where it stands in one world is the user's call
- **Files:** new `game/src/game/systems/endgame.lua`; `game/data/ui/death.lua`

**Original:**
- **The radio.** From island 5 the exit is a radio (the "Buy" frame of `b_exit`, 3.8.5). "Try to contact someone with a radio?" (new 640) and Yes plays radio_noise: "...Copy! We hear you! Will be on your position in one day, hold on!..." (new 639). `Final_Day` is set to 1 (12.5.10.20).
- **The final day** (8.2.1, 8.2.2). For one in-game day (1440 ticks of 1.5 s, 36 minutes):
  - a 1500 px noise every 10 s, and every 3 s in the last few minutes;
  - an attack wave (finding 8) every 90 s, and every 30 s after 30 minutes;
  - every 4 s, `warn_zed` sends each infected whose eyes carry the flag var#6 (#7 for shooters) at the player (8.2.1.3);
  - the bunker refused ("I can't go to bunker, i can skip rescue helicopter.", 3.1.1.2).
- **The rescue.** At the end a helicopter flies over ("I need to return to the radio!", new 656, Spoiler_Heli 8.1.1). Back at the radio, The_End (8.2.4):
  - every enemy is frozen, and nearby bandits are shelled;
  - the helicopter lands: "Hey! I'm Boris, welcome onboard!", then "Nadia, fly to the East, to 4316 signal." (new 653, 654);
  - a fade, then Dead_screen("Happyend").
- **The happy end** (10.3.1, 10.3.16):
  - a teal sky and "You successfully escaped." over the same stats;
  - the item unlocks for characters 4 to 19 (10.3.1.1 to .5);
  - the save wiped (8.2.3.1).

**Remake:** A run ends only in death.

### 16. Helicopter crash sites and the radio's scan

- **Verdict:** unclear  **Effort:** M  **User's ask:** none directly; it follows world 4 (should each season band carry its island's content?)
- **Files:** `game/data/maps/prefabs/crash.lua`, `game/src/map/places.lua`, `game/data/items.lua` (radio)

**Original:**
- **How many.** `generate_helicrashes` (3.14) makes no helicopter crash and 1 humvee crash on island 1. Islands 2 to 5 get 2/3, 3/5, 4/6 and 5/8 (helicopter/humvee).
- **Where.** Helicopter crashes go in location kinds 1, 2, 8, 10 and 52; humvee crashes in kinds 5, 13 and 15.
- **Finding a helicopter crash.** Its loot point is type 15 (items 4). Walking into its `ach_heli_counter` gives (6.1.2):
  - +25 XP and "Blackbox Collector";
  - VSS progress (6 on Veteran and up) and Pilot progress (13 on Legend).
- **Finding a humvee crash** gives +15 XP (6.1.3). Found crashes are marked on the notebook's page (world 38).
- **The radio item** scans 5000 px (8.18):
  - "Looks like a military crash site is nearby.", marking them;
  - or "Looks like there are no military crashes nearby.".

**Remake:** World 7 has two humvee crashes (world 7, items 11), no helicopter crash. The radio is inert.

### 17. The underground bunker

- **Verdict:** unclear  **Effort:** XL  **User's ask:** none directly; it follows world 4
- **Files:** new `game/src/map/bunker.lua`, `game/data/maps/prefabs/bunker.lua`

**Original:**
- **The entrance.** From island 2 a `bunker_entrance` stands in a random location of kind 1, 2, 8 or 10 not already holding a crash (3.15; live at (14928, 12420) on island 2, probes.txt). Touching it asks "Enter this bunker? You will be able to get back." (Level_menu 11).
- **Inside** (Bunker_generate(level), 3.1.4) is a separate underground tilemap at (2306, 17464):
  - its own ambience (bunker_ambient), no rain or night, the minimap marker hidden;
  - sliding doors, some broken ("This door is completely broken."), some needing the Officer's keycard ("This door requires "Officer's Keycard".", 3.1.6);
  - crates, `bunker_lootbox` kinds 51, 52 and 53 (items 19), and zombie spawners checked every 3 s (3.1.7).
- **Leaving** by `bunker_exit` (Level_menu 12) runs Bunker_erase. The entrance then shuts for 1800 s: "Bunker will be avaliable after N minutes." (Level_menu 13).
- **Its place in the run.** On island 5 its elevator leads on (finding 14). The achievement "🚪" (ach_bunker) counts it, and a task reward can give its coordinates.

**Remake:** None.

### 18. Drivable vehicles, on every island

- **Verdict:** unclear  **Effort:** XL  **User's ask:** none. Progress.md's out-of-scope list names "Vehicles" (Stage 0 plan).
- **Files:** new vehicle module; `game/data/trunks.lua` (the trunks already there)

**Original:**
- **Where.** At every map's generation, `Car_respawn_uaz`, `_volga` and `_hammer` (3.6.3, 9.40 to 9.42) put drivable cars on road locations, with random fuel and condition. Live on island 1: one UAZ and one Volga (`S/orig/v1_car.png`, probes.txt cars).
- **Driving** (group 9): collisions, a car HUD with fuel and condition, the friend riding along, the Humvee's machine gun.
- **What needs a car:** island 4's bridge (finding 14), Kate's and Taman's unlocks, and a task reward.

**Remake:** Cars are parked props with trunks.

### 19. The albino deer

- **Verdict:** unclear  **Effort:** S  **User's ask:** none directly; it rides on the islands (world 4, finding 14)
- **Files:** `game/data/animals.lua`, `game/src/map/wildlife.lua`

**Original:**
- **When.** From island 4 (`CurrentLevel` 3 and up), one deer in ten from a deer point is the rare one: Spawn_NPC kind 20 (13.7.1.21.1.1.7, 13.1.1.20 sets var#7).
- **How it differs.** It runs with the `run_*2` animations and leaves `deer_rare_dead` (13.5.3.4.1), flayed like a deer (8.10.3.36 to .38).
- **What it unlocks.** Three kills unlock Rudolf "Jager" on Legend.

**Remake:** Left out on purpose: "the rare deer comes only on the original's later levels" (Progress 28.2.1).

### 20. Fish off the coast

- **Verdict:** make identical  **Effort:** M (with items 34's rods)  **User's ask:** none. The beach is Stage 26's ask; the fish in its sea are the original's.
- **Files:** `game/src/map/coast.lua`, `game/data/items.lua` (fishing_rod, spinning)

**Original:**
- **Sea nests.** Every 20 s, while the player is within 1000 px of the west edge, a `fish_nest` is made in the sea at x 500 beside him, replacing the last (8.21). The same happens at the east edge, at x 17283. Within 400 px a `fish_sprite` ripples (8.20).
- **The rods** (8.10.3.73, .74). Used while overlapping a nest, they take it:
  - one time in five "Damn, slipped away.";
  - otherwise "Gotcha!" with a Herring or Salmon from the sea, or a Ruffe or Perch from a pond's nest (world 20);
  - with no nest: "I don't see any fish nearby.".

**Remake:** No fish in the sea or ponds; the rods are inert (items 34).

### 21. A deer or a rabbit flees once until it loses sight

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 28 asked for animals ("add animals, wolf chasing player or deer ..."), not this rule
- **Files:** `game/src/game/systems/fauna.lua`

**Original:** The flee sits under a TriggerOnce (13.5.3.1.1.1; the rabbit's 13.5.4 the same). After its 2 s run, the animal's sight is set to 0 until the next second's check sets it again.

**Remake:** It flees again every second it still sees one. Progress 28.2 lists this under "Found, not fixed".

### 22. Autosave every 90 s, and a save as soon as a run starts or loads

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua` (save.autosave_seconds), `game/src/app.lua`, `game/src/game/save.lua`

**Original:** Save_run_progress (29.1: localStorage `gamesav_v7` = "1" and SaveState "mysave") runs:
- every 90 s of play, while alive on the Map and not loading (event 32);
- after a world is generated or an island reached (3.6.3.7, 3.7.3.5), and 2 s after a load (2.14.14);
- on accepting a task or taking a gift (12.2.1.1.1.2, 12.2.3.1.1.2);
- 5 s after a failed save, as a retry (Everywere 3);
- when leaving to the menu (menus, "Already identical").

A new game wipes the old save at Start (ME 3.2.2.11).

**Remake:**
- **Cadence.** Every 180 s (`cfg.save.autosave_seconds`), and on MAIN MENU.
- **Nothing at START.** 10 s into a new run, slot 3 is still empty (`S/rm/r4_save/run.log`: `save.exists(3)` false).
- **So** a page shut in a new run's first three minutes leaves CONTINUE on the previous run's last autosave, or on nothing.

### 23. Leaving the page opens the pause

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/main.lua` (love.focus / love.visible), `game/src/app.lua`

**Original:** On Browser.OnPageHidden (8.19), while alive on the Map, not loading and with no menu up:
- the notebook, perks and bag are shut, and the stick hidden;
- Options opens with the timescale at 0.

So coming back to the tab or the phone's browser finds the pause window waiting.

**Remake:** No `love.focus` or `love.visible` handler. The page's `visibilitychange` only copies saves (`tools/web_template/index.html` 643). On return the run goes straight on; audio 6 covers the sound side.

### 24. IDDQD

- **Verdict:** make identical  **Effort:** S (needs hud 8's portrait)  **User's ask:** none
- **Files:** `game/src/core/input.lua`, `game/data/ui/hud.lua`

**Original:** Typing IDDQD on a keyboard (event 33) does two things:
- the player says "I feel like immortal, but im not.";
- the portrait's helmet changes (frame 2 for characters 7 and 12, 3 for 20, 1 otherwise).

Nothing else.

**Remake:** None.

### 25. The tutorial island

- **Verdict:** unclear  **Effort:** XL  **User's ask:** Stage 23: "no need to hint a pc player as there will be a tutorial later"
- **Files:** new tutorial map and script

**Original:**
- **What exists.** A Tutorial layout (8300x2000) and group 11 script a tutorial island:
  - script zones and a scripted encounter with camera rolls (11.2);
  - fifteen tips (11.6), from "Welcome, survivor. You are trapped on a small island, and your only chance to get to another one is to find a boat." through controls, inventory, firearm, perks and bleeding to death;
  - a boat repaired with wood piles to finish.
- **1.0's menu never reaches it.** ME 8 sets `Tutorial_map_finished` = 1 once the texts load, and nothing asks localStorage for it, so ME 30 to 36 never fire.
- **No tip in a run.** On the Map the group is switched off (3.6). Live: a zombie's first touch armed no bleeding tip (probes.txt bleed).

**Remake:** None. When the asked-for tutorial is made, the original's island, script and texts are there to copy. Whether to copy them is the user's call.

### 26. Where saves live in the browser

- **Verdict:** platform  **Effort:** -  **User's ask:** none
- **Files:** `game/src/core/persist.lua`, `tools/web_template/index.html`

**Original:** Construct 2's SaveState goes into IndexedDB (`_C2SaveStates`, ME 34). Flags and the profile are in localStorage: `gamesav_v7`, "achieves", "char_N", "Player_skin", the stick's place, the language.

**Remake:** LÖVE's save directory, which love.js holds in memory. The page copies it to IndexedDB after each write and when the page hides (persist.lua, index.html). That is the browser's need, and there is nothing for the player to see.

## Already identical

- **The special game modes:** Casual, Radiation, Battle Royale, Public enemy, Shootout, Invasion, Wildlife, Breach. None is reachable in 1.0:
  - `seed_btn` and its code box are never on the menu (0 instances live, `S/seedbtn.js`);
  - `menu_draw_new` (Survival / Special) is never called, so button 1001 to the mode list (ME 3.2.8, 3.9) is never made;
  - the Breach button 111 is never created.
  - So `SEED_RUN` is 0 in every run and groups 2.2 to 2.9 stay off. Neither game has them, which makes survival 37 (radiation, the red zone) already identical.
- **No login, account or store.** The Login layout (Bohemia account, Login_events) is never gone to (menus).
- **No analytics.** DeltaDNA (ME 47, 11.19) only makes IDs, and `GameCenter_achieves_report` has no handler.
- **No traders** in either; survivor camps' tasks and gifts are the nearest thing (finding 3).
- **No dog in a run.** `dog_skin` is destroyed at the Map's start (3.6, 3.7).
- **No gated weapons.** All seven weapon unlocks are 1 from the start (finding 13).
- **Dead code in 1.0:**
  - the shooting range (Polygon layout, Button 8.4) and AddLog (12.12) are never reached;
  - the death-count `EP_counter` / `Adrenaline` flags (29.6, 29.7) are set but never acted on;
  - `Spoiler_Helicopter_ready` is never set.
- **The animals' numbers, carcasses, flaying and cooking** come from the events (Progress 28.2). A carcass walked onto without a knife says "I need a knife to flay prey." in both (8.17).
- **A new run starts at 06:00** on the first island (3.8.1; survival 16).
- **Weather and night are survival's.** A new game generates a fresh world in both (ME 3.2.2.11; remake START, Stage 25).

## Not checked

- **Not played, only read:** the final day, the radio and the helicopter landing (they need island 5); the bridge level (island 4); the elevator; the "unknown islands".
- **The bunker:** its entrance was seen on island 2, but it was not entered, nor its doors and lootboxes opened.
- **Helicopter crashes and the radio's scan:** counted on island 2, not visited or used.
- **Tasks and gifts:** read. A survivor camp was spawned and counted, but not talked to, and no task was done.
- **The friend's commands and car riding:** not exercised. The drivable cars were counted and one looked at, not driven.
- **Bandits' AI** (13.4.4 to 13.4.6) and the friend's (13.4.7): skimmed for when they spawn and what they count, not compared move by move.
- **Achievements:** one toast was forced; no tier was earned by play, and the recount at death ran with nothing counted. The character kits come from the texts (ui 80 to 94, new 180 to 186, 633) and 2.13's structure, not from starting as each character.
- **Not triggered:** the tent's horde (no tent was deployed), the sea's fish nests, IDDQD.
- **The remake's web build:** not run for the page-hidden case; the desktop build was read only.
- **The animals of Stage 28:** not re-measured beyond finding 21.
- **Sound:** headless Edge could not decode the original's audio, so the toasts', airdrop's and radio's sounds come from the events.
- **The phone:** every shot is 1280x720.
