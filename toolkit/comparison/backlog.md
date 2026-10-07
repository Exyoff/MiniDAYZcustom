# The backlog: fix tracks that make the remake identical to the official 1.0

Built 2026-10-06 from the eleven surveys in this folder and checked against the remake as published (`wt/plan`,
cba0ea8: Stages 27 and 28 merged) and against the original's events (`toolkit/events/`, `toolkit/globals.txt`).
The user's standing instruction (Stage 28): *"compare against official minidayz, use screenshots and source code
to compare. Do not stop working until it is identical except for the changes i had asked for specifically."*
So every difference below is either the user's (kept, listed at the end) or a fix.

The last survey, `events.md` (events, progression, achievements, the islands, saving and every event-sheet group
the others leave), landed after the first version of this file and is folded in: its findings are `events#N`.

> **2026-10-07: the user's new asks (user_asks.md, "Stage 30") stand over this backlog where they meet.**
> - The corner minimap goes (HUD-TOP's part, and question 29's corner-minimap point, settled by the ask).
> - Loot is the user's split -- houses everyday clothing, bags and pistols; other houses the simpler guns; military bases the armour, gear and assault rifles; better-stat items only further east -- not 1.0's live lists (LOOT, CAMPS-TRUNKS keep 1.0's counts per point, condition and ammo only where the split allows).
> - Military bases only in the later seasons; military infected only at military places, civilian ones elsewhere (POPULATION, ZOMBIE-NUMBERS, the place tracks).
> - Every building's room is drawn under its front, and an open door shows its open leaf (the original does the first too).
> - A hit wears one worn garment, as 1.0's Player_get_hit does (15.3.5.10).
> - A container always offers USE, and searching one is a timed use with the bar, longer the more it holds.
> - A desktop editor for places (layout, tiles, infected), buildings, objects, mobs and loot tables.
> These are built first, as the remake's Stage 30.

## Summary

| | count |
|---|---|
| Findings in (eleven surveys) | **437** (menus 41, hud 45, inventory 60, character 27, zombies 38, combat 57, survival 37, items 48, world 43, audio 15, events 26) |
| Merged | **73** findings folded into another, in 56 merged differences; **364** distinct differences remain |
| Already done since Stage 27/28 | **4** differences (7 findings) |
| No difference after all | **2**: radiation and the red zone (survival#37) and the tutorial (events#25) are unreachable in 1.0 too |
| Kept, because the user asked | **43** differences (56 findings) |
| Platform | **5** |
| Questions for the user | **7**: six decide 11 differences (14 findings); one (Q1) can keep seven tracks out |
| To make identical | **299** differences (353 findings), in **96 tracks**. Four wait on a question (BOARD-LOOK on Q2; ISLANDS, BUNKER, ENDGAME on Q6). Seven are Progress' out-of-scope list, built last, which Q1 can keep out |

Of the surveys' 46 "unclear" verdicts, 32 are settled below by the asks, Progress or the events (nine of them, the
out-of-scope list's, become tracks under the standing instruction), and 14 wait on questions Q2-Q7. One
"platform" verdict (inventory#J3) becomes a fix once the original's tap-to-move exists.

How to read references: `hud#12` is finding 12 of `hud.md`. Inventory findings keep that survey's letters
(`inventory#B9`). An event path such as `8.10.3.29` is Game_events unless a sheet is named (ME = Menu_Events,
LE = Loading_events). `SCR/...` and `S/...` paths of the five later surveys are under the session scratchpad
`C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/<survival|items|world|audio|events>/`.
The `SP/...` paths of the six older surveys were a cloud scratchpad and are gone; retake those shots with
`toolkit/orig.cjs` and the remake's `--shot` harness.

## Corrections between surveys

Each was checked in the events (paths given), not taken from either survey.

1. **Vitamins double regeneration for 60 s; they give no immunity.** survival#4 and items#30 are right,
   inventory#G3 is wrong. 8.10.3.29 sets `var#33 = 1`, waits 60 s and clears it; 6.2.2.10.1 adds
   `Regeneration_current` a second time while `var#33 = 1`. Only tetracycline sets immunity (`var#51`, 8.10.3.40).
2. **Cloudberry and Nuko Cola double regeneration too.** items#27 is right; inventory#G1's "cloudberry +30 s
   immunity" is wrong and its "Nuko Cola sets water to 100" is incomplete. Cloudberry (8.10.3.61) gives 10/10, then
   `var#33 = 1` for 30 s. Nuko Cola (8.10.3.55.1) sets water to 100, then `var#33 = 1` for 60 s (5 s + 55 s).
3. **The original has a low-health grey.** hud#45 and survival#34 are right, combat#55 ("None") is wrong. 6.3.16
   sets `Grayscale` to `50 - hp` while 0 < hp < 50. It stays kept: the user asked for their own curve.
4. **The DAY label's click is silent.** 12.16 calls `menu_click` at -10 dB (hud#7), but `media/` has no
   `menu_click` file: it 404s and fails to decode (audio, "Reported in other areas"; menus "Already identical").
   Copy the label and no sound.
5. **A fire is lit with `match_burn`.** audio#9 is right, items#31 is wrong. Matches (8.10.3.24.2.1) and the lighter
   (8.10.3.109.2.1) play `match_burn` at 0 dB, flat. `matchstrike_2` and `lighter_0` light a cigarette
   (Check_fire_cigarets, 8.10.1.2.6 / 8.10.1.3.6).
6. **Adrenaline crashes.** survival#31 is right; inventory#G3 and items#30 leave the second half out. 8.10.3.39.1 sets
   hp to 100 and gives +20 px/s for 10 s. 6.18 then adds 50 hp a second for those 10 s, and 6.19 caps hp at 25 at
   the end.
7. **Damage numbers are for melee hits only, on the infected and on animals.** combat#29 and zombies#27 are right;
   hud#25 doesn't say which hits. Every `dmg_hint` call (13.2.8.1-9 `.1.1`) sits under
   `fam_m4_bullet.IsBoolInstanceVarSet(var#5)`, the melee bullet's flag. Rounds show none.
8. **The armour floor.** combat#40 is right; zombies' "Already identical: a floor of 2" overstates it. 15.3.2 takes
   the four armour values off, and only if the result is under 1 does 15.3.2.1 set it to 2. So 1.5 stays 1.5. The
   remake's `max(2, amount - armour)` is not that.
9. **A round goes 360 px, not 400.** combat#9 is right; Progress' reference table ("400 px, identical on all 29 of its
   bullet templates") is wrong. 16.2.1 is `CompareTravelled(>=, 360)`.
10. **Which gun takes which attachment.** items#40 is right; combat#8's "keep that table" is wrong. The Stage 23 ask
    ("not all attachments go on every gun") names no table, and the original's per-gun branches (8.10.3.64-108)
    meet it.
11. **A hit's lurch, beside the asked slow.** zombies#13 says make identical and combat#31 says unclear. Settled:
    both happen. An unaware zombie lurches for 1 s as warn_zed makes it (13.2.8.4), and the asked half-speed slow
    applies to that lurch too.
12. **Draw order.** zombies#29 says make identical; character#24 says unclear because it rests on a ground rule.
    Ground rules are the remake's, not asks. Settled: make identical.
13. **A garment's warmth by condition.** survival#8 corrects inventory#B9: the remake now scales warmth by
    condition, but linearly. The original rounds up: `ceil(var#9 / 100 × condition)`.
14. **Nothing turns with time alive but the buried infected.** survival#13 is wrong: no winter comes at 5000 s.
    Timeline 23.2-23.4 (180, 3000 and 5000 s) need `SEED_morezeds = 0`; every menu path into a run sets it to 1 (ME
    3.2.3, 3.2.6, 3.2.8.2, 3.9; Restart_game 26) and nothing sets it back. The tanks and the other special kinds are
    on from the first second anyway (Restart_game 26), and the bot switches are overridden by `SEED_pubg`'s tick
    (correction 16). What the clock still does: the buried infected come at 1500 s alive or on a later island (23.6;
    events#5, measured at 5014 s). The "seed more zeds" group (2.5) stays inactive, since it needs `SEED_RUN = 1`
    (2.1.1.1.1). This answers items' question for the caller.
15. **Map Notes reveal nothing new.** items#33 has it slightly wrong: `secret_location_revealed` is 1 from the start
    in 1.0 (globals.txt; world#38). Using the notes (8.10.3.100) only opens the notebook at its map tab.
16. **Progress 27.1.3's "the first branch is the battle-royale mode's" is wrong.** items#1 is right: `SEED_pubg = 1`
    in every normal run, so the first branch of Trigger_spawn (14.1.4.1) is the live one. The same switch runs the
    Bot Manager every 60 s, so every normal 1.0 run has bandits, a friend and camps from the first minute
    (13.4.1.1; events#1).
17. **Progress' reference table has more rows to fix.** "Zombie chase 80-90/90-100/100-110" are the warn speeds
    (zombies#1). "Bleeding 30 s, confirms" is the perk's branch; it is 60 without the perk (zombies#34). The melee
    cooldowns and the difficulty row are 1.2's. "Dispersion 0-25" is the start/melee row (combat#6). "Magazine size
    and the automatics' fire rate not recoverable" is wrong: both are in the events (combat#2, #5). "Loot respawn
    1480 s" is 1.2's; 1.0's is 1500. Each track fixes the rows it touches.
18. **Footsteps on the first map.** character#14's "anything else: forest_run" is the table's fallback. The first
    map's ground off the roads is tile 6 everywhere, so a step there plays `run_1`..`run_4`. The audio survey
    measured exactly that (o2).
19. **The airdrop is the flare gun's.** items' "Not checked" calls it a perk's. The flare gun (item 103) calls it
    (8.10.3.90; events#7); `gui_airdrop_icon` and `Airdrop_recharge` are dead code.
20. **Snow falls only on islands 4 and 5.** survival#13 puts it in "the frosted, dry and snow bands". `rains_level` is 2
    only from `CurrentLevel` 3 (3.8.4-3.8.6.1), the dry and the snow sheets; the frosted island rains (3.8.3;
    events#6).
21. **Radiation and the red zone are unreachable in 1.0** (survival#37). The seed button and the mode list are never
    on the menu, so `SEED_RUN` is 0 and groups 2.2-2.9 never run (events, "Already identical"). Neither game has
    them, so there is no difference.

## What Stage 27 and 28 settled

The six older surveys looked at Stage 26. This is every one of their findings that touches the world, rooms,
doors, the map screen, the minimap, animals, a zombie's blow or a house's front, checked against cba0ea8.

| Finding | Now | Where |
|---|---|---|
| hud#11 the notepad and its new-game tip | **done**: `gui_pad_btn` under the gear, `pad_open`/`pad_page`, `walk_marker` at 10 fps until first opened | 27.4.1, 27.4.6, 27.4.8 |
| hud#24, zombies#26, combat#28 hp bars over the infected | **done**, and over every animal | 28.2.7 |
| zombies#21 `jumper_spot` among the spotted groans | **done** | 28.1.2 |
| zombies#22, combat#30 every zombie screams on death | **done**: `zombie.death` is empty | 28.1.2 |
| zombies#20 the blow sound (keep) | **kept and done as asked**: the thud alone | 28.1.1 |
| zombies#24 wander groans (keep) | kept; the eight attack_N back | 28.1.2 |
| zombies "Not checked": zombies against wolves and deer | **done**: the infected hunt animals, and deer and rabbits run from them | 28.2.5 |
| hud#10 the gear's place | still open: gear and notepad sit at x -170, left of the corner minimap | HUD-TOP |
| hud#12 the corner minimap | still open (27.4.10); settled below: it goes | HUD-TOP |
| hud#14, inventory#J1 the bag button | still open: top-right under the minimap | HUD-TOUCH |
| hud#35, inventory#A4 HUD hidden round the board | still open; the board now carries its own notepad (27.4.7) | Q2 |
| hud#27 the trunk's outline | still open | REACH-CUES |
| inventory#C1, items#2, world#42 containers (keep) | kept; since 27.1.3 each loot point is one piece | — |
| character#7, character#8 scale and the room zoom | still open (27.1.11) | CAMERA |
| character#21 the player's outline behind a facade | **changed**: the facade fades to 0.25 over its half-dark plate as the original's (28.1.4); the outline is still missing (world#34) | REACH-CUES |
| character#24, zombies#29 draw order | still open; the facades' fade follows 28.1's rule | DRAW-ORDER |
| zombies#8, #16 sight blockers, walls | still open; the rooms' walls are the original's since 27.1 | ZOMBIE-BODY |
| zombies#35 groups per spawn point | **changed**: 27.3's places give city 5 points a lot, base 5, town 3, camp 1, checkpoint 3; still one zombie a point | POPULATION |
| zombies#36 where zombies are made and removed | **changed**: lots are built as reached (27.2.4) and zombies sleep past 2000 px by where the player is (27.2.6, 27.3.10); they are still made when a lot is built, kept and never removed | POPULATION |
| zombies#37, #38 the respawn band, far zombies | unchanged (760/1020; asleep past 2000); animals sleep the same way (27.3.14) | POPULATION |
| combat#11 auto-aim | **changed**: animals are targets now (28.2.2); still on-screen only, not 400 px | AIM |
| combat#26 the headshot skull | still open, and it now covers animals too: they show frame 0 where the original shows a wolf frame 1 and a deer frame 3 | HITFX |
| combat#36, hud#30 the hit flash and shake | still open; a wolf's bite shakes in the original too (audio#11) | HITFX |
| character#23 the death pose and sound | still open: `tutor_death` | DEATH |
| menus#3 "Generating world" | still the one plate (27.2 builds behind it) | MENU-MOTION |
| Known: 27.1.10 buildings with several doors | **done**: the HQ hangs 2 doors and the fire station 4 (`more_doors`); their looks were fixed in 27.3.22. Left: 27.1.14, the brown garage's walls 2-3 px off | VILLAGES |
| Known: 27.1 "found in review" | the hut, castle and school are placed now (27.3.2); the bus is still in no pool; the gun shop's rack and the hunter's stash are still open; the canopy's west pillar is solid over -66..41 where the original's runs from -130 | ROADSIDE, LOOT, CAMPS-TRUNKS, SMALL-PLACES |

## What the asks settle

Every "unclear" verdict, and every "make identical" that might collide with an ask, was checked against
user_asks.md and Progress (17.4's four board asks and Stage 16's #17 are asks too). Where the asks settle one, it
goes to a track or the kept list:

- **The corner minimap goes** (hud#12, world#35, 27.4.10). When the user asked (2026-09-28) *"also make the map into
  an openable menu, clicking a button opens and closed the map"*, the only map in the game was the always-on corner
  minimap (Stage 15); there was no map screen. So the ask is to turn that map into one that opens and shuts, and
  1.0 has no corner minimap either. HUD-TOP removes it and keeps `minimap.draw_world` for the notebook. Its zombie
  radar goes with it. Reversible by putting two hud.lua entries back.
- **The attack button keeps its trigger** (hud#18). 1.0's `gui_btn_attack` is a red crosshair, but the user's words
  name the picture: *"show weapon trigger sprite, gui_btn_attack, when holding a pistol or rifle"*. ART10 recuts
  every other 1.2 sheet and leaves this one.
- **"Bites can infect" stays on the card** (menus#22), as a bullet in the original's style on Veteran and Legend.
  The original's cards list each difficulty's differences, and infection is the user's (Stage 26).
- **The CONTINUE refusal line stays** (menus#41). It exists only because the asked generated worlds (Stage 25) can
  refuse a save.
- **The stick editor follows tap-to-move** (menus#26 → CONTROLS). No ask covers the floating stick. The original
  defaults to tap-to-move on a phone, with a fixed, placed stick as an option. A tap never fires, so the Stage 18
  ask ("tapping ... the left side of the screen fires the weapon") holds.
- **A melee weapon is not raised at a target** (character#20 → CHAR-LOOK). The Stage 20 ask is "the weapon is put up
  and aimed", which is what a gun does. The original's melee changes no pose (8.5.4.4.2).
- **Draw order** (character#24 → DRAW-ORDER), the lurch (combat#31 → ZOMBIE-AI) and the reticle's red reset
  (combat#14 → AIM): the original's.
- **The over-head lines' colours** (combat#44 → LINES). The user dictated the words. Each line takes the colour of
  the original's line it replaces: "It's empty" yellow like "Mag empty.", "I don't have ammo for this" red like
  "No more ammo.", "My inventory is full" red like "No free slots.".
- **The Ground row's hand** (inventory#C4 → BOARD-LOOK) goes from ordinary rows; the TAKE ALL rows keep theirs
  (17.4.4). Double tap takes (inventory#C5).
- **The tooltip's facts** (inventory#D2 → NAMES-TEXT): the tooltip is asked (17.4), and its stats become the
  original's own description text (l_eng_new; `12.11.2`'s popup content). There is no separate (i) popup.
- **NEARBY stays a drop target** (inventory#E2): the asked containers need one.
- **The respawn band and far zombies** (zombies#37, #38 → POPULATION): 1500/2000 once CAMERA lands, and zombies past
  1500 px blinded and past 2000 px removed, as the original does them. The remake's sleep was a phone's
  convenience the removal makes moot.
- **Cold sickness is caught as the original catches it** (survival#24 → HEAT): 44% a tick at heat 0. *"Cold sickness
  from staying out too cold"* fits either rule, and only the cure was specified.
- **Tetracycline also cures cold sickness** (survival#26 → MEDS). The original has one sickness and the pill cures
  it. The user's cure (75% for some time) stays alongside.
- **Unlock-gated items take the fresh profile's fallbacks** (items#21 → LOOT): bandage, F1, RDS, matches, army knife.
- **No trunks on the truck and the UAZ** (items#47 → CAMPS-TRUNKS). *"Lootable vehicle trunks"* is met by the
  original's own trunks (cars, vans, police cars, hatchbacks).
- **The map's page** (world#36 → MAP-PAGE, MAP-TABS). LOCAL MAP shows 16 lots each way round the player: the
  original's map is 16x16 at that glyph size, and 16 is the world's whole height. GLOBAL MAP (MAP-TABS) shows the
  whole world. The original's GLOBAL MAP shows its maps west to east, and here those are the five seasons.
- **The voice cap** (audio#14 → AUDIO). The 30 ms retrigger guard goes: the original starts two snarls and three
  thuds within 2 ms. A voice cap stays only as a phone's limit, sized so that measured play never drops a sound.
- **Progress' "Explicitly out of scope" list is not the user's.** It is in the project's first commit, the Stage 0
  plan (events). Under the standing instruction, bandits, vehicles, perks, achievements and the characters they
  unlock are made identical: PERKS, ACHIEVEMENTS, CHARACTERS, BANDITS-A, BANDITS-B, VEHICLES-A and VEHICLES-B, built
  last, after every track that fixes what the remake already has (menus#15, hud#9, character#26, events#1, #10-13,
  #18). Q1 asks once whether to keep them out instead. The list's login and game modes need nothing: 1.0 never
  reaches either.
- **The friend, survivor camps, karma, experience, the airdrop and the tent's horde** are not on that list: they are
  made identical (FRIEND, SURVIVORS, XP, AIRDROP, DEPLOY). The survivors' "wipe out a bandit camp" task comes with
  BANDITS-B.
- **The tutorial** (events#25): 1.0's menu never reaches its tutorial island, so neither game has one in play. When
  the user's *"there will be a tutorial later"* (Stage 23) comes, the original's island, script and fifteen tips are
  the source. No track now.
- **The scout book** (items#38) gives 200 XP (XP); the protector case is the survivors' lost loot case (SURVIVORS);
  cigarettes are MEDS'.
- Checked and **no collision**:
  - HUD visible round the board vs 17.4 "Place the health stats at the top": the HUD's plates are at the top (but see Q2).
  - One hand slot vs Stage 6 "a simple slot inventory, like the original game": they agree.
  - The 1:1 camera vs Stage 16's "player locked to the centre": the view stays unclamped.
  - Melee as the original's crescent vs Stage 22 "Less range on melee": the crescent stops at 22 px.
  - Fists at the start: the asked punch face shows.
  - The asked TARGET button still shows only with an enemy on screen while auto-aim reaches 400 px.
  - The bag bottom-left vs the TAB hint at the bottom centre: that hint is the keyboard's and stays.
  - The menu sky scrolling left at 12/24 px/s keeps the asked slow sky and faster skyline.
  - The fire's 180 s × 3 burns: the Stage 24/26 asks name the kits, build mode and lighting, not the times.
  - Permadeath: Progress' "Needs you: death and CONTINUE" is settled by the original's wipe.
  - "Interrupting a use": settled by the original. Nothing interrupts.

## Questions for the user

1. **Keep Progress' out-of-scope list out?** Bandits, vehicles, perks, achievements, and the 20 characters with
   their kits and Unlocks pages were left out by the Stage 0 plan, not by any ask of yours (menus#15, hud#9,
   character#26, events#1, #10-13, #18). Under your instruction they are made identical, as the last tracks (PERKS,
   ACHIEVEMENTS, CHARACTERS, BANDITS-A and -B, VEHICLES-A and -B). The menu's Facebook and MINI DayZ 2 plates, which
   link to the original makers' pages, come with ACHIEVEMENTS. Say which of them, if any, to keep out instead.
2. **The inventory board** (inventory#A1, #A6, hud#35; BOARD-LOOK waits on this): rebuild it on the original's
   single `gui_inventory` picture at its size (about 700x547 at 1280x720), with no captions or names on the cards
   and the HUD left up and usable round it, keeping your tabs, TAKE ALL, tooltip, foot pager and weapon cells? Or
   keep today's larger, labelled rust board you picked in Stage 17, and make only its behaviour and numbers identical?
3. **Report lines** (inventory#F6, items#45): the original says what just happened over the head ("I've eaten
   Canned beans.", "Bottle empty.", "I found something in this trunk.", "Blood transfusion completed.") and, while
   the board is open, shows a 2x copy rising in the middle of the screen. Copy both, only the over-head ones, or
   neither (your Stage 20 "remove inventory hints")?
4. **The makings by the spawn and every home's door** (items#13): the original places no wood, sticks or newspaper
   anywhere. Its buildings never drop papers; only about one parked car in fifty does, so without the makings your
   campfire starter kit would be rare. Keep them, or remove them?
5. **Item 97** (items#23): in 1.0, eating "Canned tuna" (which runners drop) clears the save and reloads the page.
   Copy that, or make it an ordinary can? Its art becomes the original's either way.
6. **The islands, the crossings and the ending** (world#4, events#14-17, #19). 1.0 is five islands in a row. Each
   has more places than the last (cities, bases of kinds 5 and 70, a gun-shop location), helicopter and humvee
   crashes, a bunker from island 2 and an albino deer from island 4. A boat repaired with wood piles, then duct tape,
   then gasoline, and a bridge crossed by car lead east with no way back. On island 5 a radio calls a rescue for one
   more day under attack, and "You successfully escaped." ends the run. Your one world has the islands' five seasons
   side by side (Stages 26, 27). Which?
   - (a) Each band is its island (its places, crashes, bunker and deer), and the radio, the final day and the rescue
     stand at the far east as the run's end, with no crossings.
   - (b) As (a), with the crossings too: each band's east edge shut until its boat is repaired or its bridge driven,
     and no way back.
   - (c) None of it: every band keeps the first island's mix, and a run ends only in death, as now.
7. **The menu's title** (menus#10): the original's MINIDAYZ+ logo sprite, or your own name MINIOUTBREAK set in the
   original's lettering?

## The tracks

### For every track

- **Where.** Work in a worktree of your own branched from the integration branch. Never edit, commit or push in
  `wt/plan`.
- **The original.** It lives in `MiniDAYZcustom/MiniDayZ+1.0` and the toolkit beside it (README.md). Run its Python
  with `PYTHONUTF8=1`.
  - Find an event with `grep -n '^<path>\b' toolkit/events/Game_events.txt`: `show.py` fails on Windows over
    `signal.SIGPIPE`.
  - `objects.txt` names every object and its instance variables. `globals.txt` holds the globals and the layers.
  - `orig.cjs` drives the running game (`bash serve.sh` first; the menu answers touch taps only).
  - `cut.py <object> OUT/` cuts any sheet's frames in the rip's layout, so missing art goes into
    `Assets/Sprites/<object>/`, as 27.4 and 28.2 did. Ground rule 2 is kept: art comes from `Assets/`.
- **Ground rule 3 is overridden.** "Approximate, don't replicate" yields to the user's standing instruction: copy the
  original's numbers, offsets and timings from the events.
- **The remake.** `"$LOCALAPPDATA/Programs/love-11.5/lovec.exe" game --shot --frames N --beat B --res 1280x720
  [--world 7] --script "..."`. `--view 720` shows the world 1:1 until CAMERA lands. Check the web build with the
  headless Edge probe (memory: verify-live-web-build) and `tools/web_perf.cjs` / `tools/web_world.cjs` at 4x CPU.
- **Scenarios.**
  - Put them in the subject module named in the brief (`tools/scenarios/<subject>.py`).
  - See each new one fail with its fix undone.
  - Run `python tools/verify.py --changed`, the subjects named and the lints. **Not the full suite**: *"don't have
    every agent run a full test"* and *"Don't run the full suite"* (Stages 20, 21). The merger runs everything at once.
- **Progress.md.** Write the track's items, with the original's evidence and what was measured. Fix the rows of
  "Reference numbers recovered from the original" that the track touches, and the "What X overturns" notes.
- **Worlds and saves.** A change to what a seed builds raises `generate.VERSION`, and older saves are refused with
  their reason (27.2.2). Anything whose saved shape changes needs `save.lua` to load an older file without loss.
- **Generators.** Nothing a generator makes may follow `pairs()` order or `math.random` (27.3.7): every seed must
  hash the same natively and in the web export.
- **Asks.** Keep every kept difference (the list at the end); each brief names the asks near it.

### The order

| # | key | title | effort | depends on | runs beside |
|---|---|---|---|---|---|
| 1 | DATA10 | Data from the official 1.0 | M | — | AUDIO, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE |
| 2 | AUDIO | The sound engine and the mix (the one-ear panning first), and a hidden page | M | — | DATA10, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE |
| 3 | SURVIVAL | Survival numbers, a run's start, and its saves | M | — | DATA10, AUDIO, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE |
| 4 | CAMERA | The original's camera: 1:1, 2x in a room, no lag | L | — | DATA10, AUDIO, SURVIVAL, HUD-TOP, ZOMBIE-NUMBERS, TYPE |
| 5 | HUD-TOP | The HUD's top as the original's; the corner minimap gone | M | — | DATA10, AUDIO, SURVIVAL, CAMERA, ZOMBIE-NUMBERS, TYPE |
| 6 | ZOMBIE-NUMBERS | The infected's speeds, bites and corpses | S | — | DATA10, AUDIO, SURVIVAL, CAMERA, HUD-TOP, TYPE |
| 7 | TYPE | The original's typefaces | S | — | DATA10, AUDIO, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, HITFX, NIGHT |
| 8 | HUD-TOUCH | The buttons where the original has them | M | HUD-TOP | HITFX, LINES, CLOCK, NIGHT, ART10 |
| 9 | HITFX | Being hit, and hitting: flash, shake, blood, skull, shield | M | AUDIO | HUD-TOUCH, LINES, CLOCK, NIGHT, ART10 |
| 10 | MELEE | The swing as the original's: one target, crits, damage numbers | M | DATA10 | HUD-TOUCH, LINES, CLOCK, NIGHT, ART10 |
| 11 | LINES | Lines over the head | M | TYPE | HUD-TOUCH, HITFX, MELEE, CLOCK, NIGHT, ART10 |
| 12 | CLOCK | The clock tab and "DAY N" | S | HUD-TOP, TYPE | HITFX, MELEE, LINES, NIGHT, ART10 |
| 13 | NIGHT | Night's hours, depth and tints | S | — | HUD-TOUCH, HITFX, MELEE, LINES, CLOCK, ART10, ZOMBIE-AI |
| 14 | ART10 | 1.0's art for the sheets that are 1.2's | M | DATA10 | HUD-TOUCH, HITFX, MELEE, LINES, CLOCK, NIGHT, ZOMBIE-AI, GUN-NUMBERS |
| 15 | CHAR-LOOK | The character as the original draws him | M | ART10 | ZOMBIE-AI, GUN-NUMBERS, USE |
| 16 | ZOMBIE-AI | The infected's senses and moves, and the animals' flee | L | ZOMBIE-NUMBERS | CHAR-LOOK, GUN-NUMBERS, USE, DISPERSION |
| 17 | GUN-NUMBERS | Each gun's cadence, damage, rounds, magazine and noise | M | DATA10 | CHAR-LOOK, ZOMBIE-AI, USE |
| 18 | USE | A timed use as the original's | S | — | CHAR-LOOK, ZOMBIE-AI, GUN-NUMBERS, DISPERSION |
| 19 | DISPERSION | Each gun's own dispersion and its colours | M | GUN-NUMBERS | ZOMBIE-AI, USE, ZOMBIE-BODY |
| 20 | AIM | The reticle, the lines and what is aimed at | M | DISPERSION, CAMERA | ZOMBIE-BODY, POPULATION, HEAT |
| 21 | RELOAD | Reload, switch, Load and Eject | M | GUN-NUMBERS | ZOMBIE-BODY, POPULATION, HEAT |
| 22 | ZOMBIE-BODY | The infected's body: no shoving, a bouncing box, what hides you | M | — | DISPERSION, AIM, RELOAD, SHOT |
| 23 | SHOT | What a shot looks and sounds like | L | GUN-NUMBERS, AUDIO | ZOMBIE-BODY, POPULATION, HEAT |
| 24 | BALLISTICS | How far a round goes and what stops it | M | — | POPULATION, HEAT, FOOD |
| 25 | POPULATION | Groups at points, made out of sight, gone past 2000 | L | CAMERA, ZOMBIE-NUMBERS | SHOT, BALLISTICS, HEAT, FOOD |
| 26 | HEAT | Heat as the original's temperature | M | SURVIVAL, DATA10 | SHOT, BALLISTICS, POPULATION, MISSING-ITEMS |
| 27 | FIRE | The campfire's life | S | — | BALLISTICS, POPULATION, FOOD, MISSING-ITEMS |
| 28 | FOOD | What food and drink do | M | SURVIVAL | BALLISTICS, POPULATION, FIRE |
| 29 | MEDS | What medicine and cigarettes do | M | FOOD | LOOT, CAMPS-TRUNKS, MENU-LOOK |
| 30 | HUD-INDICATORS | Regeneration heart, icons' animations, boost's place | S | MEDS, HUD-TOP | LOOT, CAMPS-TRUNKS, MENU-LOOK |
| 31 | MISSING-ITEMS | Eleven items that share an icon name | M | DATA10 | HEAT, FIRE |
| 32 | LOOT | One thing a point, from the live lists | L | MISSING-ITEMS | MEDS, HUD-INDICATORS, MENU-LOOK |
| 33 | CAMPS-TRUNKS | Trunks, the camps' boxes and the crash | M | LOOT | MEDS, HUD-INDICATORS, CAPACITY, MENU-LOOK |
| 34 | CAPACITY | One hand slot, one-row bags, the original's capacities and stacks | M | — | CAMPS-TRUNKS, MENU-LOOK, GROUND |
| 35 | BOARD-FEEL | Taking, dropping and dragging as the original's | M | CAPACITY | MENU-LOOK, MENU-MOTION, DIFFICULTY, GROUND |
| 36 | BOARD-LOOK | The board as the original's picture (**waits on Q2**) | L | Q2, BOARD-FEEL, HUD-TOUCH | MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, GROUND, LOTS |
| 37 | NAMES-TEXT | The original's names and words | M | TYPE | MENU-LOOK, MENU-MOTION, DIFFICULTY, GROUND, LOTS |
| 38 | CONTROLS | Tap to move, and a stick you place | L | HUD-TOUCH | NAMES-TEXT, MENU-LOOK, GROUND, LOTS |
| 39 | MENU-LOOK | The menu as the original's plates | M | TYPE, HUD-TOP | CAPACITY, BOARD-FEEL, GROUND, LOTS |
| 40 | MENU-MOTION | The loader, the curtain and the fades | M | MENU-LOOK | BOARD-FEEL, GROUND, LOTS |
| 41 | DIFFICULTY | Four difficulties, chosen, then Start | M | MENU-LOOK, SURVIVAL, ZOMBIE-NUMBERS | PAUSE, GROUND, LOTS |
| 42 | PAUSE | The pause and Options as the original's | S | MENU-LOOK | DIFFICULTY, GROUND, LOTS |
| 43 | XP | Experience: the Score, and what earns it | M | — | DIFFICULTY, PAUSE, AMBIENT, GROUND |
| 44 | DEATH | The death screen, and death for good | M | MENU-MOTION, XP | AMBIENT, GROUND, LOTS |
| 45 | AMBIENT | Footsteps by ground, and the beds | M | AUDIO | DEATH, GROUND, LOTS |
| 46 | GROUND | Plain grass and asphalt, and the decals | M | ART10 | CAPACITY, BOARD-FEEL, BOARD-LOOK, NAMES-TEXT, CONTROLS, MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, LOTS |
| 47 | LOTS | Locations of 1020 px | M | — | CAPACITY, BOARD-FEEL, BOARD-LOOK, NAMES-TEXT, CONTROLS, MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, GROUND |
| 48 | ROADS | One road: four cells of asphalt | M | LOTS | COAST, REACH-CUES, ATTACH |
| 49 | COAST | The coasts, the forest wall and the spawn | M | LOTS | ROADS, REACH-CUES, ATTACH |
| 50 | ROADSIDE | Poles, cars, the bus and compounds on the roads | M | ROADS | LIGHTS, REACH-CUES, ATTACH |
| 51 | VILLAGES | Villages as one location of four layouts | L | ROADS, GROUND | LIGHTS, REACH-CUES, ATTACH |
| 52 | CITIES | Cities as one location | M | ROADS | LIGHTS, REACH-CUES, ATTACH |
| 53 | BASE | The first map's military base | M | ROADS, ROADSIDE | LIGHTS, REACH-CUES, ATTACH |
| 54 | SMALL-PLACES | Hospital, fire station, gas station and cafe, paved (27.3.13) | L | ROADS, ROADSIDE | LIGHTS, REACH-CUES, ATTACH |
| 55 | CHECKPOINT | Checkpoints on the crossing (27.3.13) | M | ROADS, ROADSIDE | LIGHTS, REACH-CUES, ATTACH |
| 56 | COUNTRYSIDE | Forests, woods and fields | L | LOTS, GROUND | LIGHTS, REACH-CUES, ATTACH |
| 57 | PONDS | Ponds with fish nests | M | COUNTRYSIDE | LIGHTS, REACH-CUES, ATTACH |
| 58 | MAP-PAGE | The map's page: its window, glyphs and marks | M | COUNTRYSIDE, PONDS | LIGHTS, REACH-CUES, ATTACH |
| 59 | LIGHTS | Lights cut out of the dark | L | NIGHT | ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, REACH-CUES |
| 60 | REACH-CUES | Outlines and names in reach | M | — | ROADS, COAST, ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, LIGHTS, ATTACH |
| 61 | DRAW-ORDER | Layers as the original's | M | CHAR-LOOK, REACH-CUES | ATTACH, FELLING, CROWS, VARIANTS |
| 62 | ATTACH | Attachments as the original's | M | DISPERSION, MISSING-ITEMS | ROADS, COAST, ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, LIGHTS, REACH-CUES |
| 63 | FELLING | Trees that fall to an axe | M | MELEE, COUNTRYSIDE | DRAW-ORDER, CROWS, VARIANTS |
| 64 | CROWS | Crows on the roads, and their caws | M | ROADSIDE | DRAW-ORDER, FELLING, VARIANTS |
| 65 | VARIANTS | Colours at spawn | M | — | DRAW-ORDER, FELLING, CROWS, WATER |
| 66 | WATER | Bottles that hold water; pumps and bushes | M | FOOD, LOOT | VARIANTS, CRAFT |
| 67 | CRAFT | The original's recipes | L | BOARD-FEEL | VARIANTS, WATER, SLEEP |
| 68 | MENDING | Patch, sharpen, saw off | M | CRAFT | SLEEP, RAIN |
| 69 | KNIVES | Knives as melee weapons | M | MELEE, CRAFT | SLEEP, RAIN |
| 70 | SLEEP | Sleeping | L | HEAT, CLOCK, FIRE | CRAFT, MENDING, KNIVES |
| 71 | RAIN | Rain | L | HEAT, NIGHT | MENDING, KNIVES, MAP-TABS |
| 72 | SNOW | Snow in the cold bands | M | RAIN | MAP-TABS, THROWABLES |
| 73 | MAP-TABS | The notebook's Guides, Crafting, Tasks and GLOBAL MAP (27.4.9) | L | MAP-PAGE, CRAFT, ART10 | RAIN, SNOW, THROWABLES |
| 74 | THROWABLES | Grenades, smoke and the molotov | L | CRAFT | SNOW, MAP-TABS, LIGHT-ITEMS |
| 75 | DEPLOY | The tent, traps, wire and mines | L | THROWABLES, POPULATION | LIGHT-ITEMS, GARDEN-FISH |
| 76 | LAUNCHERS | The underbarrel launchers | M | THROWABLES, ATTACH | LIGHT-ITEMS, GARDEN-FISH |
| 77 | LIGHT-ITEMS | Headlamp, NVG, flares, batteries, radio | L | LIGHTS | THROWABLES, DEPLOY, LAUNCHERS, GARDEN-FISH |
| 78 | AIRDROP | The flare gun's airdrop | M | LIGHT-ITEMS, CAMPS-TRUNKS | GARDEN-FISH, SPECIALS-A, PORTRAIT |
| 79 | GARDEN-FISH | Planting and fishing | L | PONDS, WATER | DEPLOY, LAUNCHERS, LIGHT-ITEMS |
| 80 | SPECIALS-A | Screamer, buried, tank, jumper | L | ZOMBIE-AI, POPULATION | LIGHT-ITEMS, GARDEN-FISH, PORTRAIT |
| 81 | SPECIALS-B | Spitters, shooters, skins 7-11, acid | L | SPECIALS-A | PORTRAIT, PERF-27 |
| 82 | PORTRAIT | The portrait with its gear and XP | M | ART10, VARIANTS, HUD-TOP, XP | SPECIALS-A, SPECIALS-B, PERF-27 |
| 83 | FRIEND | The Bot Manager and the friend | L | ZOMBIE-AI, HITFX, XP | PORTRAIT, PERF-27 |
| 84 | SURVIVORS | Survivor camps, their tasks and gifts, and karma | L | FRIEND, MAP-TABS, XP | PERF-27, ISLANDS |
| 85 | PERF-27 | 27.3.31: the sleep pass, the drops' walks, the autosave | M | POPULATION | SPECIALS-A, SPECIALS-B, PORTRAIT |
| 86 | ISLANDS | Each band its island's content (**waits on Q6**) | L | Q6, POPULATION, the place tracks | SURVIVORS, PERF-27 |
| 87 | BUNKER | The underground bunker (**waits on Q6**) | L | Q6, CAMPS-TRUNKS, POPULATION | PERKS, ACHIEVEMENTS |
| 88 | ENDGAME | The crossings, the radio and the rescue (**waits on Q6**) | L | Q6, ISLANDS, DEPLOY, XP, DEATH | PERKS, ACHIEVEMENTS |
| 89 | PERKS | Perks, and the Stats tab (Q1 may keep it out) | L | XP, PORTRAIT | BUNKER, ENDGAME |
| 90 | ACHIEVEMENTS | Achievements, their toasts and the lifetime stats (Q1 may keep it out) | L | XP, MENU-LOOK | BUNKER, ENDGAME |
| 91 | CHARACTERS | Twenty characters and the Unlocks pages (Q1 may keep it out) | L | ACHIEVEMENTS, PERKS, ART10 | VEHICLES-A |
| 92 | BANDITS-A | Bandits: the wanderers (Q1 may keep it out) | L | FRIEND, SURVIVORS, XP | VEHICLES-A |
| 93 | BANDITS-B | Bandit camps and the boss (Q1 may keep it out) | L | BANDITS-A | VEHICLES-A, VEHICLES-B |
| 94 | VEHICLES-A | Drivable cars (Q1 may keep it out) | L | ROADSIDE, AUDIO | CHARACTERS, BANDITS-A, BANDITS-B |
| 95 | VEHICLES-B | The Humvee's gun, the friend riding, fuel and repair (Q1 may keep it out) | M | VEHICLES-A, FRIEND | BANDITS-B |
| 96 | LANGUAGE | The original's Language option | L | NAMES-TEXT, LINES, MENU-LOOK, and every track that adds text | VEHICLES-B |

Why this order:
- **1-6 can all run at once.** They are the foundation and what is felt from the first minute:
  - the 1.0 data under everything (melee is three times too slow on 1.2's numbers);
  - the one-ear sound;
  - hunger six times too slow, and a pistol in hand at the start;
  - a camera 2.25 times too close;
  - a HUD in another place;
  - the infected's speeds and bites.
- **7-14 are the rest of what is seen every second:** text, buttons, hits and swings, lines, the clock, night and
  the art.
- **15-25 are the fight;** 26-33 survival and loot; 34-38 the board and the controls; 39-45 the menus, the Score and
  the beds.
- **46-58 rebuild the world on the original's 1020 px locations:** LOTS before every place track. The place tracks
  touch `data/places.lua` and `src/map/fill.lua`, so they run **one at a time**, each beside non-world tracks.
- **59-85 are systems the remake does not have yet,** among them the friend and the survivors (83, 84).
- **86-88 wait on Q6,** the islands.
- **89-95 are Progress' out-of-scope list,** after every track that fixes what the remake already has. Q1 can keep
  them out. LANGUAGE (96) is last because it carries every track's text.

---

### 1. DATA10 — Data from the official 1.0

**Findings:**
- the known item: `original_globals.lua` and the other generated files were extracted from the 1.2 fan mod;
- inventory#B9;
- combat#1 and character#19, the melee cooldowns;
- combat#3.

**Effort** M. **Depends on** nothing. **Beside** AUDIO, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE.

**Make identical.** Every extractor reads `OriginalData/C2SourceData.js`: `tools/c2data.py` `ORIGINAL_DATA`, the `SRC`
of `extract_combat_stats.py` and `extract_clothing_stats.py`, `extract_anim_speeds.py` `SOURCE`, and
`extract_rooms.py`. That file is a 1.2-like export: 8,629,978 bytes, where the official `data.js` is 8,564,509
(toolkit README, "Surprises").
- **Point them at the official `MiniDayZ+1.0/data.js`, read past its BOM.** The extractors also read
  `OriginalData/dump/Game_events.txt` and `object_index.txt`. Regenerate those from 1.0, or teach the extractors the
  toolkit's `events/` format.
- **Run `python tools/build.py`, then diff `game/data/generated/` file by file.**
- `original_globals.lua`: the 29 values at the end of `toolkit/globals.txt`, for example Novice_starving 0.25,
  Regular_regeneration 6, Zed_fast_dmg 16, Player_Hear_radius 2000, LOOT_RESPAWN_TIME 1500, Zed_per_base 15,
  secret_location_revealed 1. Nothing reads this file (`tools/scenarios/__init__.py` says so); the numbers that act
  live in `data/config.lua`, and SURVIVAL and ZOMBIE-NUMBERS move them.
- `weapon_stats.lua`, melee: the swing lock is the item's `var#4` (8.7.1.5.1.2.2.2.24).
  - 1.0's values: Split Axe 0.5, Shovel 0.6, Pipe Wrench 0.3, Bat 0.4, Fireaxe 0.6, Crowbar 0.2, Pickaxe 0.3,
    Pitchfork 0.4, Sledgehammer 0.8, Sword 0.5, Pan 0.3, Katana 0.5. The remake has 1.2's 1.5/1.2/1.3/1/1.6/...
  - Fists 0.3 (`Wait(0.3)`, 8.7.1.5.1.2.1.7). It may be a constant in the extractor.
  - `combat.swing` waits `max(fire_delay, melee_recover_time 0.25)`. 1.0 has no such floor: drop it, or the fists
    and the crowbar are slowed.
- `weapon_stats.lua`, guns:
  - wear per shot is combat#3's table;
  - Remington's lock is 0.3 and Chigur's 0.4 (`var#4`), where 1.2 has 0.9;
  - FN FAL's and RPK's 0.4 is 1.2's.
- `clothing_stats.lua`: inventory#B9's 16 garments, heat/armour:

  | garment | heat/armour |
  |---|---|
  | helmet_army | 3/5 |
  | gorka_helmet | 3/8 |
  | razgruz | 0/5 |
  | razgruz_big | 0/6 |
  | vest_bulletproof | 0/15 |
  | vest_press | 0/7 |
  | gorka jacket and pants | 7/3 |
  | orel jacket and pants | 5/2 |
  | gasmask | 4/1 |
  | greathelm | 4/6 |
  | helmet_hard | 2/4 |
  | helmet_nvg | 4/4 |
  | headlamp | 2/2 |
  | pilot_helmet | 3/3 |

- `anim_speeds.lua`, `animations.lua`, `atlas.lua`, `sprite_points.lua`. Progress 27.3's last note: a build over 1.0
  changes these four. `gui_btn_talk/default_4` drops; nothing reads it (the board uses `default_3`).
- `zombie_stats.lua` should not change: the per-zombie hp is the same in both builds.

**Watch.**
- `sprite_points.lua` feeds the doors (image points 3-6), the shadows (s1/s2, 27.1.6) and the bars (28.2.7). A moved
  point moves a door or a shadow: run the world and render subjects and `extract_rooms.py`'s `_sanity()`.
- Update `docs/original_data.md` to say which file is 1.0.

**Keep.**
- *"melee has no cool down"* (Stage 18): that was a request for a cooldown, and the original's own values satisfy it.
- Melee's clock apart from the gun's (18.2).

**Scenarios.**
- lint: `build.py --verify` over 1.0 changes nothing.
- combat: an axe swings every 0.5 s, fists every 0.3 s, a crowbar every 0.2 s.
- items: an army helmet reads heat 3, armour 5; helmet plus assault vest is armour 10, as in inventory's g8_wood.
- world/render: every door and shadow where 27.1.7's walk-in expects it.

### 2. AUDIO — The sound engine and the mix, and a hidden page

**Findings:**
- audio#1: the known web-build one-ear panning;
- audio#2, #3, #4, #6, #8, #14;
- audio#5, with zombies#23 and combat#25;
- zombies#25;
- the pause on a hidden page, events#23.

**Effort** M. **Depends on** nothing. **Beside** DATA10, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE.

**Make identical:**
- **Panning (fix first; it is heard on every headphone).**
  - The original: the Audio plugin's HRTF panner with the listener 600 px above the plane, `listener.setPosition(x,
    y, -600)`, from t187's `[0,0,0,1,1,600,600,10000,10]` and `c2runtime.js`. A sound 300 px to the side is only
    2.2 dB louder in the near ear (audio's `S/pan_measure.log`).
  - The remake: `audio.lua` `set_pan` gives a mono source `(dx/260 × 0.85, 0, 0)`. love.js passes that to an
    equal-power PannerNode, where any non-zero x is 90° to the side: one ear.
  - Fix: place each source at its real offset with the 600 px height: `(dx, dy, 600)` against a listener at 0.
- **Distance.** C2's inverse model counts the 600 in the distance: `gain = 600 / (600 + 10 × (d - 600))`, with
  `d = √(dx² + dy² + 600²)`, floored at the reference.
  - That gives 0.46 at 300 px, 0.19 at 600, 0.10 at 1000 and 0.04 at 2000 (zombies#25).
  - The remake's `attenuate` is full inside 48 px and silent past 640.
- **The ears are the camera's** (2.13 `SetListenerObject(camera)`). `app.lua:998` and `player.lua:317` set them to the
  player. Once CAMERA lands, the focus button moves the ears too.
- **A sound follows what made it.** 577 of the original's 985 Play actions are PlayAtObject. `play_at` takes a point;
  let it take an entity and re-place the source each frame, as `loop_at` already does.
- **No pitch randomising.** `pitch_var` 0.06 goes: no 1.0 action sets a playback rate.
- **The mix.**
  - Master goes from 0.8 to 1.0. Each event plays at `10^(dB/20)` of the original's own dB (audio#5's table).
  - At 0 dB: gunshots, `empty_click`, shotgun shells, `Punch_Swipes`, spotted groans, the wolf, `pad_open` and
    `pad_page`, `meat_cook`, the fire's `match_burn`, the beds.
  - At -5 dB: reload stages, footsteps, the bite's thud on the player, every lit campfire's `fireplace_loop` at its
    own place (not only the nearest within 180 px).
  - At -10 dB: the menu bed.
  - A hit on an infected or an animal is -15 dB at the body, and only within `Player_Hear_radius` 2000
    (13.2.8.1-4.3): measured 0.178 against the remake's 0.56.
  - The player's volume slider (Stage 17) multiplies over all of it.
- **Doors** (12.13.1, 12.13.3.1.4) play flat (PlayByName), never at the door.
  - `open_door` and `close_door` are 0 dB; `_metal` -5 dB.
  - Metal leaves are `firestation`, `ga_brown` and `ga_blue`, and the wooden sound plays for every leaf that is not
    `firestation`. So the two garage doors play both.
  - `door.set_open` plays at the doorway at 0.7. `sounds.lua` also counts `firestation2`, `barrack` and `shtab` as
    metal.
- **Hidden page.** The plugin suspends its context and pauses every sound when the page is hidden ("Play in
  background" off, t187 property 2; `S/orig/o6_hidden.log`). The remake has no `love.visible` or `love.focus`
  handler, and `index.html` listens only to persist saves.
  The same moment opens the pause (Browser.OnPageHidden, 8.19): while alive on the map with no menu up, the
  notebook, the perks screen and the bag shut, the stick hides, and Options opens with the world stopped. A
  player coming back to the tab finds the pause waiting.
- **The guard and the cap.** Remove the 30 ms `retrigger_guard`. Keep a voice cap only as a phone's limit, raised
  until a soak drops nothing.

**Not here:** per-gun samples (SHOT), footsteps and beds (AMBIENT), the death's beds (AMBIENT).

**Keep:** the volume slider (Stage 17); the thud as a zombie's blow (Stage 28); the wolf's own sounds (Stage 28);
the wander groans.

**Scenarios** (in the subject holding today's audio scenarios; grep `audio` in `tools/scenarios`):
- a source 300 px right is 2.2 dB louder right, and never silent left;
- the gains at 300, 600, 1000 and 2000 px;
- a groan at a running zombie follows it;
- every rate is 1;
- hiding the page pauses every sound and opens the pause, and showing it resumes the sound;
- a wooden door plays 1.0 flat, a metal one 0.56, a garage both;
- a hit 0.178 at 100 px, none past 2000 px;
- re-run the audio survey's `S/pan.cjs` on the new web build.

### 3. SURVIVAL — Survival numbers, a run's start, and its saves

**Findings:**
- survival#1, #2, #3, #16, #33;
- the bleed, survival#35 with zombies#34 and combat#38;
- the start kit, hud#41 with inventory#B11, character#6 and combat#46;
- the saves, events#22.

**Effort** M (the start kit moves many scenarios). **Depends on** nothing. **Beside** DATA10, AUDIO, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, TYPE.

**Make identical:**
- **Starting meters.** Food, water and heat start at 75, hp at 100 (`player_collision_base`'s `var#13-16`,
  objects.txt t181; measured `S/orig/nov_start.png`).
- **Hunger and thirst.** Every 5 s, food -= `Starving_current` and water -= `Thirsty_current` (6.2.2.2), set at layout
  start (2.11.9-12):

  | difficulty | food and water a tick | regeneration |
  |---|---|---|
  | Novice | 0.25 | 8 |
  | Regular | 1 | 6 |
  | Veteran | 1 | 4 |
  | Legend | as Veteran | as Veteran |

  A bar of 75 empties in 6.25 minutes on Regular. The remake's `hunger_rate 0.15` × 1.2's scalars lasts six times
  longer.
- **Regeneration** needs food, water and heat all >= 50, no bleeding and no sickness (6.2.2.10, unchanged).
- **An empty bar** costs 1 hp only when the drain takes it below zero (6.2.2.4/6/8). `run_tick` clamps first and
  charges at <= 0, so the tick landing on 0 costs too.
- **A bleed lasts 60 s** (15.3.5.2; 30 s is the metabolism perk's).
- **A run starts at 06:00.** `Timer_ALL` is 361 on level 0 (3.8.1). `state.lua:26` starts at 08:00.
- **The start kit** (2.13.1.1): Spawn_drop 350 (T-shirt) and 300 (jeans), worn at 100%, nothing in hand.
  `player.spawn` (`player.lua:257-269`) gives an FNX in hand.
- **Saves** (Save_run_progress, 29.1): every 90 s of play while alive (event 32); as soon as a world is
  made or an island reached (3.6.3.7, 3.7.3.5); 2 s after a load (2.14.14); a retry 5 s after a failed
  save (Everywere 3); and on leaving to the menu, as now. The remake saves every 180 s
  (`cfg.save.autosave_seconds`) and not at START: 10 s into a new run its slot is still empty (events'
  `S/rm/r4_save/run.log`), so a page shut in a run's first three minutes leaves CONTINUE on the run before.
  A new game's first save writes over the old one, as the original's Start wipes it (ME 3.2.2.11). The
  save on taking a task is SURVIVORS'.

**Remake:** `data/config.lua` survival (`start_*`, `hunger_rate`, `thirst_rate`, the novice/regular/veteran
scalars, `regen_rate`, `bleed_duration`), `survival.tick_drain`, `run_tick`, `state.lua`, `player.spawn`;
`data/config.lua` `save.autosave_seconds`, `app.lua`, `save.lua`.

**Watch.** Every scenario that expects the FNX at the start (every combat capture did) must now give itself a gun.
Heat at 75 stays at 75 under today's model, which has no day cold; HEAT changes that.

**Keep:**
- the difficulty on its own screen;
- infection on VETERAN, its 12% unchanged;
- cold sickness's cure at 75%;
- the punch face on the attack button with fists (Stage 20).

**Scenarios:**
- play: a new run reads 100/75/75/75 at minute 361, wears a T-shirt and jeans at 100%, and holds nothing;
- play: Regular food 75 → 71 in 20 s, Novice 75 → 74 in 20 s;
- play: regen +6 a tick at >= 50 on Regular;
- play: a bleed stops at 60 s;
- play: a tick landing on 0 costs nothing and the next costs 1;
- save: a new run is saved at once, then every 90 s, and again 2 s after a load;
- save: an older save keeps its own kit.

### 4. CAMERA — The original's camera: 1:1, 2x in a room, no lag

**Findings:**
- the scale, character#7 with world#1;
- the room zoom, character#8 with world#2;
- character#9, character#10;
- the focus, hud#21 with character#12 and combat#17;
- the known item 27.1.11.

**Effort** L. **Depends on** nothing. **Beside** DATA10, AUDIO, SURVIVAL, HUD-TOP, ZOMBIE-NUMBERS, TYPE.

**Make identical:**
- **1 world px per CSS px.** C2 crop mode (`project[12] = 1`), with world layers at scale 1. A 1280x720 window shows
  1280x720 world px, and a phone at 844x390 shows 844x390 (character#7 measured the GUI at 0.63 and the world at 1).
  `world_view_height` 320 (`viewport.world_scale`, 136-139) gives 2.25 and 1.22.
- **2x in a room** (autozoom, 20.1). Every 0.5 s, while `player_base` overlaps a warmzone, group zoomin adds 0.05
  per step to layers 0-50 up to `Current_zoom_lvl + 1` = 2, and zoomout takes it back on leaving. Read the step's
  timing in 20.1. A garage read 2.000 live (`S/orig/o4_v9_inside.png`). The GUI stays at its own scale.
- **The camera is pinned to `player_collision_base`** (2.13): the centre of the 30x30 body (origin 15,15), exact every
  tick. `camera.update` eases at `follow_speed` 20 and centres the feet.
- **Focus** (8.5.6.1, 8.5.8.2.1). The camera is the exact midpoint of player and target every tick, uncapped, with no
  ease; it lets go on a second tap or when the target is lost. `targeting.update_focus` has `focus_ease` 4 and
  `focus_reach` 0.5.

**Watch:**
- What 320 scaled moves: the population band (POPULATION goes to 1500/2000), auto-aim (AIM), the drop culling
  (27.3.6), the loader's reach of 2 lots, the dark round a room, and the perf budgets. A PC at 1280x720 sees about
  5x the area and a phone 1.5x: measure the web build at 4x CPU in a town and a city.
- Whole-number scales keep the pixels crisp.

**Keep:**
- the view not clamped at the map's edges (character#11; Stage 16 #17, *"The player is locked to the centre of the
  screen"*);
- the focus button (Stage 20);
- the outside going dark from a room (Stages 26, 28): the zoom applies over it.

**Scenarios:**
- render/controls: 1280x720 world px at 1280x720 and 844x390 at the phone;
- render/controls: in a warmzone the scale ramps to 2 at the original's pace and back;
- render/controls: the camera on the body's centre with zero lag while running;
- render/controls: focus on the exact midpoint;
- phone: the layout checks pass at 844x390;
- update every scenario that read the 320 view.

### 5. HUD-TOP — The HUD's top as the original's; the corner minimap gone

**Findings:**
- hud#1, #2, #3, #10, #39;
- the minimap, hud#12 with world#35 (27.4.10).

**Effort** M. **Depends on** nothing. **Beside** DATA10, AUDIO, SURVIVAL, CAMERA, ZOMBIE-NUMBERS, TYPE.

**Make identical:**
- **The GUI scale** (1.2.1 Device_check): 1 when the viewport is >= 590 px high, else `1 - (590 - h) × 0.00185`
  (0.63 at 390). Add this to `viewport` beside `ui_scale` (min(w/960, h/540) = 1.333 and 0.722), as a "gui" rule, and
  add a "menu" rule of 1 CSS px always, for MENU-LOOK. Opt `hud.lua` into the gui rule and write its numbers in the
  original's px. Other screens opt in through their own tracks.
- **The four plates** (12.13.2.1.1.1-.4): `gui_panel` frames 0..3 at `X_mid` -195/-65/+65/+195, top +30, ordered heart,
  water, food, heat.

  | screen | plate x | y | size |
  |---|---|---|---|
  | 1280x720 | 385, 515, 645, 775 | 10 | 120x40 |
  | 844x390 | 261, 343, 425, 507 | 6 | 76x26 |

  `METERS` today is a stack at the top-left.
- **The fill** is 0.72 px a point at (43,14), 72x10 at most, and never changes colour (6.3.12-15). Drop `low_at`.
- **The gear** `gui_help` is 100x66 at the right edge, top+45 (1180,45; phone 781,28). The notepad sits 75 below it
  (27.4.1). Today both are at x -170.
- **No corner minimap.** Remove `minimap_paper` and `minimap` from `hud.lua`, and keep `minimap.lua`'s page for the
  notebook. Leave the top-right for CLOCK's tab (`gui_time_bg` 118x48 at the right edge, top+5).
- **No debug tab in the player's build** (`debug.lua`; Progress "the debug tab in the web build").

**Keep:**
- the status icons at the end of their bars, and the armour shield at the end of the HP row (Stage 17);
- the notepad and M (27.4);
- the held weapon's picture bottom-left (Stage 22);
- the TAB hint (Stage 17);
- the dead zones, which inset the right column (Stage 18).

**Scenarios:**
- hud: the plates' rects in that order at both sizes;
- hud: a fill of 50 is 36 px with no tint;
- hud: the gear and notepad rects;
- hud: no minimap drawn, and the notebook's page still opens;
- phone: F12 finds no overlap at 16:9, 16:10, 4:3 and 844x390 with the dead zone at none and at its widest;
- the 26.3.8, 26.4.6 and 27.2.8 minimap scenarios move to the page or go.

### 6. ZOMBIE-NUMBERS — The infected's speeds, bites and corpses

**Findings:** zombies#1, #2, #3, #30, #31.

**Effort** S. **Depends on** nothing. **Beside** DATA10, AUDIO, SURVIVAL, CAMERA, HUD-TOP, TYPE.

**Make identical:**
- **Chase speeds** (`attack.SetSpeed`): normal random(90,100) (13.1.1.1), army random(90,100) (13.1.1.9), runners
  random(120,140) (13.1.1.3). Measured 92/94/130 (zombies o1.log). The remake's 80-90/90-100/100-110 are the warn
  speeds, which ZOMBIE-AI uses.
- **Bites.** A runner bites for 16 on every difficulty (`Zed_fast_dmg`; `data/zombies.lua` has `fast.damage_scale`
  6/9, which is 1.2's 6). Normal and army bites by difficulty (2.11.9-12):

  | difficulty | normal | army |
  |---|---|---|
  | Novice | 5 | 8 |
  | Regular | 9 | 13 |
  | Veteran | 10 | 15 |
  | Legend | 10 | 15 |

- **Corpses.** Every `*_dead` has `Fade [1,0,60,5,1]`: it stays 60 s and fades over 5 (config `corpse.life` 90, fade
  2.5). Half are mirrored: `choose(1,2) = 1` → SetMirrored (13.3.1.2.2.x.1; `zombie.corpse_spawn`).

**Remake:** `config.lua` zombie and corpse, `data/zombies.lua`, `ai.zombie_tuned("damage")`, `zombie.corpse_spawn`.
Fix the reference table's chase row.

**Keep:** the slow on hit (Stage 22); the blow as the thud (Stage 28); the wander groans.

**Scenarios:**
- combat: chase speeds in band per kind;
- combat: a bite by difficulty and kind with no armour;
- world: a corpse opaque until 60 s and gone by 65 s;
- world: about half of 40 seeded corpses mirrored.

### 7. TYPE — The original's typefaces

**Findings:** menus#37.

**Effort** S. **Depends on** nothing. **Beside** DATA10, AUDIO, SURVIVAL, CAMERA, HUD-TOP, ZOMBIE-NUMBERS, HITFX, NIGHT.

**Make identical.** The original's faces, from the layouts' instance fonts in `data.js`:

| face | used for |
|---|---|
| Times New Roman | plate labels (16 pt; 22 pt on the pause's), "You are dead" |
| Arial | the death table and the loader's status line (16/12 pt), the over-head lines and item labels (10 pt, black outline), the clock (18 pt), damage numbers (12/18 pt) |
| Tahoma, outlined | the difficulty description (10 pt) |
| Trebuchet MS, bold | "DAY N" (24 pt) |

`widgets.font_at` uses LÖVE's default font. Ship metric-compatible free faces in `game/`: Liberation Serif or Tinos
for Times, Liberation Sans or Arimo for Arial, and free faces matching Tahoma and Trebuchet MS. Note their licences
in `docs/`. Give `font_at` a face, and add one outline helper (eight offset black copies, as C2's Outline effect
draws). Find C2's pt-to-px from measured heights in the original's shots. Each screen's own track picks its face and
size; this track only moves the obvious callers.

**Keep:** text drawn at the size it lands (17.5).

**Scenarios:**
- render/lint: the faces load natively and in the web build;
- render/lint: "Continue" at 16 pt is Times' width within a pixel;
- render/lint: the web build's size noted.

### 8. HUD-TOUCH — The buttons where the original has them

**Findings:**
- the layout, hud#14 with inventory#J1;
- hud#16, #34, #38.

**Effort** M. **Depends on** HUD-TOP (the scale rule). **Beside** HITFX, LINES, CLOCK, NIGHT, ART10.

**Make identical.** The original's places (12.13.2.1, 1.2.10; hud v2_run1280.log, v2_runphone.log):

| button | size | 1280x720 | 844x390 | shown |
|---|---|---|---|---|
| interact | 100x100 | 1180,288 | 781,134 | |
| attack | 100x80 | 1180,413 | 781,213 | with a gun |
| zoom | 100x80 | 1080,413 | 718,213 | with a target |
| reload | 100x100 | 1180,518 | 781,279 | with a gun |
| switch | 100x100 | 1180,620 | 781,327 | always |
| backpack | 100x100 | 0,620 | 0,327 | always, board open too |

- **The switch is always drawn, at 50% with no gun** (12.24.2/.4/.11/.12). A tap with none calls the line "I have no
  ranged weapon." (12.13.14.1.2.2). LINES owns the line; wire the call here.
- **Pressed buttons** turn to HSL lightness 80% (Touch_fade, 12.3.x), not × 0.72.
- **Build mode's buttons** are `gui_btn_builder` at 2x (80x80), centred: the tick at mid-80, the cross at mid+80, the
  rotation at mid, all at mid_y+100 (12.8.2.1-.3).

**Remake:** `touch_controls.lua` (the anchors, `visible_when` `player.can_switch`), `widgets.button`, `key_hints.lua`
(PC: keep the TAB hint).

**Keep, placed so nothing overlaps:**
- USE only with something in reach (Stage 22), at interact's place;
- TARGET only with an enemy on screen (Stage 20), by the attack button;
- the door button (Stage 23), by interact;
- the punch face with melee (Stage 20);
- RELOAD with a gun only (identical);
- the held weapon's picture beside the bag, bottom-left (Stage 22);
- the dead zones (Stage 18);
- the touch toggle (Stage 17);
- 27.4.5's buttons over the notebook.

The bag stays on the HUD while the board is open. The board's copy of the bag (22.4.6) and its notepad (27.4.7) are
BOARD-LOOK's (Q2).

**Scenarios:**
- hud/phone: every rect at both sizes;
- hud/phone: switch at 50% with no gun, and the call on a tap;
- hud/phone: 80% lightness while held;
- hud/phone: build buttons centred;
- hud/phone: F12 finds no overlap at 16:9, 16:10, 4:3 and 844x390 with the dead zones at none and widest;
- 27.4.5's scenario still passes.

### 9. HITFX — Being hit, and hitting

**Findings:**
- the flash and shake, hud#30 with character#13, zombies#33 and combat#36;
- the blood burst, zombies#28 with combat#24;
- combat#26, #37, #39, #40;
- survival#32, audio#11.

**Effort** M. **Depends on** AUDIO (levels). **Beside** HUD-TOUCH, LINES, CLOCK, NIGHT, ART10; not beside MELEE (both edit `effects.lua` and
`combat.lua`).

**Make identical:**
- **A landed hit on the player** (Player_get_hit 15.3.5):
  - `Hit_effect` (15.4) sets the layout's Grayscale to 100 for 0.2 s, HUD included;
  - `ScrollTo.Shake(3, 0.4)`, decaying;
  - `body_1/2` at -5 dB on the player.
- **Armour** (15.3.2): the damage minus the four armour values, set to 2 only if it falls under 1. The remake uses
  `combat.armour` max(2, …).
- **Clothes tear** (15.3.5.10): `choose(2..7)`; 3-7 pick a worn garment (player vars #8, #10, #18, #12, #20) and take
  round(random(7,13)) off its condition.
- **A block** (15.3.6.2) shows the green `shield_icon` 10x10 rising (Bullet angle 270, about 25 px/s) and fading.
- **A round's or swing's blood** (13.2.8.4): `test_bodyhit` at the round (layer static_cars), anim a1 or a2 at random,
  angle `choose(0,90,180,245)`, Fade wait 0.3 and out 0.2, one per round (a pellet each). `effects.hit` draws one a
  body, always a1, unrotated, at `hit_height` 14.
- **The headshot skull** (13.2.8.4.1.2.5.1.4): `headshoticon` at the target's origin, frame 0 (the human skull) for
  the infected, 1 for wolves, 3 for deer. Fade 0.4/0.1, no movement, never mirrored, no particles. The remake shows
  frame 1, rising, mirrored, with 8 particles.
- **Bleeding drops** (6.2.5): one a second, a random frame of three, a random angle, 15 px below the body's centre.
  The remake drops two a second at the feet.
- **A wolf's bite** (13.2.4.1, .1.1, .11): the shake, then Player_get_hit's thud, `wolf_attack` and a thud at the
  player, then `wolf_attack` and a thud at the wolf. That is five starts within 2 ms (audio o5). `fauna.bite` plays
  one snarl.

**Remake:** `combat.hit_player`, `combat.armour`, `combat.record_hit` (`impact.block = {}`), `effects.hit`,
`effects.headshot`, `effects.drop`, `config.lua` effects (`headshot_icon`, `drop_every`), `fauna.bite`, and
`camera.lua`, which gains a shake on the view, not the follow. `render.begin_grey`/`end_grey` already exist for the
user's grey; the flash covers world and HUD.

**Keep:**
- the low-health grey (Stage 20): the flash is 100% over whatever it shows;
- blood and a hit sound on every hit (Stage 20);
- blood drops that stay on the ground (Stage 20): no fade, and the 120 cap stays.

**Scenarios** (combat):
- a bite greys the whole screen for 0.2 s and shakes the view 3 px over 0.4 s;
- a wolf's bite starts five sounds;
- armour 5 against 6 leaves 1, against 5 leaves 2;
- 5 hits in 6 tear a garment by 7-13 (seeded);
- a block's shield rises;
- blood at the round, in one of four angles, gone by 0.5 s;
- the skull's frame per kind, still;
- drops one a second, 15 px down.

### 10. MELEE — The swing as the original's

**Findings:**
- the swing, combat#33 with character#19's sound half;
- combat#34, #35;
- damage numbers, hud#25 with zombies#27 and combat#29.

**Effort** M. **Depends on** DATA10. **Beside** HUD-TOUCH, LINES, CLOCK, NIGHT, ART10.

**Make identical:**
- **A swing is `melee_swing_general`**: a 10x21 white crescent from the player's collision origin, aimed at
  `gui_target`, at 100 px/s with Fade 0.1/0.1. It is a `fam_m4_bullet` destroyed on its first hit (13.2.8.4.5), so it
  hits **one** body per swing. Every swing, fists or weapon, plays `Punch_Swipes_0/1` at 0 dB (8.7.1.5.1.2.1,
  .2.2.2). `chop` is FELLING's tree blow.
- **Crits** double the damage (8.7.1.5.1.2.2.2.17-22):

  | chance | weapons |
  |---|---|
  | 10% | knife 9 |
  | 15% | knife 8, shovel, pickaxe, pitchfork, sledgehammer |
  | 20% | knife 7 |
  | 25% | Split Axe, Pipe Wrench, Bat, Fireaxe, Crowbar |
  | 35% | Sword, Katana |
  | 40% | Pan |

- **A ruined weapon** still swings, for 15 (.23), and still blocks: the block roll reads only the weapon's kind
  (15.3.1).
- **Damage numbers**, on melee hits on the infected and on animals only (`dmg_hint` 12.29): "-N" in 12 pt Arial,
  rgb(255,196,68), outlined, flying away from the player at about 40 px/s, gone by 1.5 s (Fade [1,0,1.5,0.001,1]). A
  crit is 18 pt, red, "Crit -N".

**Remake:** `combat.swing` (a 90° wedge, every zombie in `melee_range` 22, a dry click when ruined),
`effects.swoosh`, `combat.block_chance` (0 when ruined), `combat.roll_damage`, `sounds.lua` `melee.axe`.

**Keep:**
- melee on the attack button with the fist face (Stage 20), and no right-click melee (Stage 19);
- the white swoosh going forward *"like a wave"* (Stage 19): the original's crescent is that;
- the 22 px reach (Stage 22, *"Less range on melee"*): end the crescent's travel at 22;
- no melee with a gun in hand (Stage 20).

**Scenarios:**
- combat: one of two touching zombies is hit;
- combat: Punch_Swipes for fists and the axe;
- combat: the pan crits about 40% over a seeded thousand, each "Crit -N";
- combat: a ruined axe swings for 15 and blocks;
- combat: "-N" leaves the player and is gone by 1.5 s;
- combat: a round shows none.

### 11. LINES — Lines over the head

**Findings:**
- the style, hud#28 with combat#44;
- hud#29, combat#43.

**Effort** M. **Depends on** TYPE. **Beside** HUD-TOUCH, HITFX, MELEE, CLOCK, NIGHT, ART10.

**Make identical:**
- **client_log** (12.18): Text t474, 500x24, 10 pt Arial with a black outline, a colour per call, made 30 px over the
  player and pinned. Two lines at most, an older one pushed up 18 px. Fade in 0.5 s, hold 5 s, fade out 1 s
  (measured).
- **With a menu open:** while the pad or perks are open (and the board too, if Q3 says so), lines show at the
  screen's centre at 2x, rising 48 px/s.
- **Survival lines** (hud#29):
  - red every 10 s: "I can feel blood dripping." (6.2.9), "I'm freezing." (6.2.10), "I'm dying of starvation." and
    "…dehydration." (6.2.11/12);
  - yellow: "I want to eat something." every 30 s at food <= 25, "I want to drink something." every 25 s at water
    <= 25, "It's pretty cold." every 20 s at heat <= 25 (6.2.15.1);
  - green: "I'm no longer bleeding." (6.2.7), "I feel healthy." (6.2.8).
- **Gun lines** (combat#43): "Weapon ruined!" red on a ruined gun's trigger (8.6.2.2.1.1); "Weapon fully loaded."
  white on reloading a full gun (8.10.3.11.1.1); "I have no ranged weapon." on switching with none (12.13.14.1.2.2).
- **Text and colours** come from `l_eng_log.xml` (`Arr[t544]`) and from each `client_log` call's `rgb()`: grep them
  in the events, since hud's `client_log_calls.txt` is gone.

**Remake:** `status.draw_say` (one cream line with a shadow, 2 s, not while the board is open), `combat.say`,
`config.lua` `status.say_time` and `say_fade`.

**Keep:**
- the user's words, in the colour of the line each replaces: "It's empty" yellow, "I don't have ammo for this" red,
  "My inventory is full" red (Stages 20, 22, 23);
- the remake's own lines that came with asks ("No campfire nearby", build mode's "Something's in the way"), in the
  style.

"It's getting colder" goes with HEAT. The report lines after uses and searches wait on Q3.

**Scenarios** (hud):
- two lines stack 18 px apart and a third pushes the oldest out;
- 0.5/5/1 s fades;
- each colour;
- the survival lines at their thresholds and intervals;
- the three gun lines.

### 12. CLOCK — The clock tab and "DAY N"

**Findings:** hud#6; the day label, hud#7 with menus#4.

**Effort** S. **Depends on** HUD-TOP, TYPE. **Beside** HITFX, MELEE, LINES, NIGHT, ART10.

**Make identical:**
- **The clock tab:** `gui_time_bg` 118x48 at the right edge, top+5 (1166,5; phone 772,3). Text t287 shows "HH:MM" in
  18 pt Arial, black, over an rgb(150,140,131) copy 1 px off. It updates every game minute (17.2.6).
- **The day label** (Day_show_label, 12.16.1): "D", "A", "Y ", N typed one every 0.3 s in bold 24 pt Trebuchet, white,
  at GUI (200,500) (328,476 at 720p). It waits 1 s and fades over 2. It shows 2 s after a run starts (3.6.3.7; after
  MENU-MOTION's curtain lifts) and at 06:00 each day (17.2.7). The `menu_click` it asks for is silent (correction 4).

**Remake:** `hud.lua`, `daynight.clock`, `daynight.day`.

**Scenarios** (hud):
- 06:00 at the start, ticking each game minute;
- DAY 1 typed in 1.2 s, held 1 s, faded over 2;
- DAY 2 at the next 06:00;
- the rects at both sizes.

### 13. NIGHT — Night's hours, depth and tints

**Findings:** survival#17, #18, #19, #21.

**Effort** S. **Depends on** nothing. **Beside** HUD-TOUCH, HITFX, MELEE, LINES, CLOCK, ART10, ZOMBIE-AI.

**Make identical:**
- **The night layer** shows at 00:00 (17.1.3.1) and hides at 06:00 (17.1.9.1). `night_overlay`'s opacity is linear:
  minutes × 1.69 through 00:00-00:59 (17.1.1), 100 from 01:00 to 04:59 (17.1.4), and (60 - minutes) × 1.69 through
  05:00-05:59 (17.1.2). Nothing before midnight.
- **The veil** is rgba(0,0,34) at 230/255 over the view, pinned to the camera, over trees and hp bars and under the
  HUD (2.13). It is an alpha veil, not a multiply. The sand reads:

  | time | sand at a fixed spot |
  |---|---|
  | noon | (179,169,137) |
  | 00:17 | (133,126,110) |
  | 00:46 | (55,51,64) |
  | 02:02 | (21,19,44) |
  | 05:31 | (102,94,91) |

  Source: survival's `S/orig` light dumps; `S/cmp/m_*.png`.
- **Dusk and dawn** (`dawnsunset`, on Color_effect, pinned to the player): at 23:30 (17.2.8) frame 0, rgba(255,119,34)
  at 15/255; at 05:30 (17.2.9) frame 1, rgba(238,187,17). Fade in 65 s and out 25 s (`[1,65,0,25,1]`).
- **The reticle** (Sunset_Dawn, 48) is above the night (46) at full strength, and the aim lines (Rain, 44) are under
  it. `app.lua:1101-1115` does it the other way round.

**Remake:** `daynight.night_factor_at` (a smoothstep from 23:00), `darkness_at`, `draw_world_overlay` (a multiply by
0.78), `config.lua` daynight. The campfire's glow stays until LIGHTS.

**Scenarios** (render):
- the overlay's alpha at 23:59, 00:17, 00:46, 02:02, 05:31 and 06:00;
- the sand pixel at 02:02;
- the 23:30 tint rising over 65 s;
- the reticle over the night, the lines under it.

### 14. ART10 — 1.0's art for the sheets that are 1.2's

**Findings:** character#1 with items#24.

**Effort** M. **Depends on** DATA10. **Beside** HUD-TOUCH, HITFX, MELEE, LINES, CLOCK, NIGHT, ZOMBIE-AI, GUN-NUMBERS.

**Make identical.** 62 sheets in `Assets/images` equal MiniDayZ+1.2's (md5). The character survey's
`sheet_diff.txt` is gone, so recompute it: md5 `MiniDayZ+1.0/images/*.png` against `Assets/images`.
- **Worn and held** (character#1):
  - helmet_army: 1.0's has a black mask face;
  - helmet_hard: a dark helmet, not the yellow hard hat;
  - helmet_nvg: a dark-green full helmet;
  - gasmask: a white full mask;
  - gorka_helmet: a closed visor;
  - pilot_helmet: black;
  - vest_bulletproof;
  - r670_shotgun: a black stock;
  - madsen_mg, crossbow;
  - hunter_backpack: 25 frames differ, and the rip lacks 1.0's four big-pack `twohand_*` frames;
  - player_skin_dead, guard and jager.
- **The rest:** gui_item_helmet, gui_item_vest, gui_portrait_helmet, gui_portrait_vest, gui_pistol (frames items_4 and
  items_13, the held weapon's picture), gui_firearm, gui_btn_perks, gui_inventory, main_menu_wpn_icon, pad_controls,
  b_car_bus, b_gunshop, ee_bpla, ee_kv2, ee_ruins, zed_tank_skin, ammo_5x45, ammo_7x62, ground_enviroment_tilemap,
  shroom.
- **Eleven ground sprites** in `Assets/Icons/ground` (items#24's table; `SCR/ground_diff_11.png`), and item 97's icon
  (the opened can).

Cut each with `toolkit/cut.py` into its `Assets/Sprites/<object>/` frames, then rebuild (`atlas.lua` and the built
atlas change).

**Keep:** `gui_btn_attack`, the trigger (Stage 20; see "What the asks settle").

**Scenarios** (lint/render):
- every replaced sheet's frames match a cut of 1.0 (a checksum table);
- the worn army helmet shows the mask;
- the attack button is still the trigger.

### 15. CHAR-LOOK — The character as the original draws him

**Findings:**
- character#2, #3, #20, #22;
- the crouch's bag, character#4 with inventory#F4.

**Effort** M. **Depends on** ART10 (the big pack's frames). **Beside** ZOMBIE-AI, GUN-NUMBERS, USE.

**Make identical:**
- **Weapons not in hand are drawn** on player_weapons, in "h_" plus the pose:
  - `h_twohand_*` standing and aiming (7.6.x.3.2, 8.5.4.4.1.1.1.2);
  - `h_run_*` running (7.5.1.x.2);
  - `h_axe_*` swinging (8.7.1.5.1.2.3);
  - `using_item` crouching (7.1.1.2-4).

  Today `render.held_key` draws only `e.weapon` (Progress 20.2.5: slung guns never drawn).
- **A lowered gun** (`two_handed_in_hands` 7.2.1; 12.26.1, 12.27.1): body and hands `twohand_<dir>`, outerwear
  `twohand_<dir>`, pants, vest, helmet and pack `idle_<dir>`, and the weapon its own `idle_<dir>`. Melee is back to
  idle (12.28, 7.2.2). `player.pose` plays idle and says twohand was dropped (601-659).
- **The pack is hidden while `using_item` plays** (7.1.1.9), and shown again at 7.1.2.5. `render.worn_pose` draws it
  7 px lower (`crouch_drop`).
- **A melee weapon changes no pose at a target** (8.5.4.4.2); the player turns only for the swing (8.7.1.5.1.1).
- **Breath:** every 3 s (21.2), a 6x6 `player_breathe` at (x, y-10), drifting 15 px/s right or along the move, fading.
  While smoking (MEDS): one a second, scale 1 to 2.5 (21.3).

**Keep:**
- shooting on the move as the raised upper body over running legs (Stage 22);
- the gun lowered without a target and raised at one (Stage 20);
- the crouch and its bar (Stage 25);
- the held weapon in the hand (Stage 20, *"the weapon shows up on their back even tho it should be in their hand"*,
  was about the held one).

**Scenarios** (render):
- with a rifle, a pistol and an axe carried and the axe in hand, the two guns are drawn in their `h_` frames in every
  pose;
- a lowered pistol's body is twohand;
- the crouch hides the pack;
- a melee target changes no pose;
- a puff every 3 s.

### 16. ZOMBIE-AI — The infected's senses and moves, and the animals' flee

**Findings:**
- zombies#5, #6, #7, #9, #10, #11, #15, #18, #19;
- the noise, zombies#12;
- the lurch, zombies#13 with combat#31;
- a deer's or a rabbit's one flee, events#21.

**Effort** L. **Depends on** ZOMBIE-NUMBERS. **Beside** CHAR-LOOK, GUN-NUMBERS, USE, DISPERSION.

**Make identical:**
- **Sight.**
  - The cone is 180° (MainLook `[1,335,180,1]`; ShortLook 70/360 unchanged).
  - Aggro is gained and dropped only once a second (`System.Every(1)`, 13.3.1.3.1-5).
  - At night (Day_mode 0, hours 0-5) sight needs the player within 175 px or in light: overlapping fam_car_light
    (13.3.1.3.x.2.1.1, .3.1.1.1). Until LIGHTS, SHOT and ROADSIDE make the other lights, a lit campfire's 250 px
    light is that.
- **Losing sight.** At the next check the zombie stands idle where it is, with no memory (13.3.1.3.x.1). It does not
  run to the last seen point, investigate and wander.
- **Spotting.** OnTargetAcquired calls `noice(target, 400)`, which warns every non-aggro zombie within 400 px
  (13.3.2.4.1…; 13.6.1.4-7).
- **A noise** (warn_zed mode 0, 13.3.1.1) moves only a zombie that is not aggro, not warned and not biting. It runs
  to the noise ±50 px at the warn speeds (80-90, 90-100, 100-110) for 2 s, then stops. It plays run while warned
  (13.3.2.2; army and runners always run).
- **What makes noise.** A moving player makes `noice(player, 65)` every 0.5 s (7.3.1). Melee makes none, so
  `combat.swing`'s `ai.noise(150)` goes.
- **A hit** calls `warn_zed(uid,0,0,1,kind)` (13.2.8.4): a non-aggro zombie lurches at a random angle at warn speed
  for 1 s, with the asked slow on top.
- **Idle.** The Walk timer repeats every `ceil(random(4,9))` s. Half the ticks, the zombie walks 20 px/s at a random
  angle for 3 s, about 60 px, then stands. It has no home (13.3.2.5).
- **Leading a target.** Three in four lead a moving target (Turret predictive aim, projectile speed random(0,100),
  13.1.1.1.1).
- **Bites.** The first lands on contact (13.3.1.4-7). After that, one global `Every(1)` (13.2.5.1) bites with every
  overlapping zombie the player's 70 px LOS sees, all on one tick. A zombie stops on contact and moves again at the
  next check after contact ends.
- **A deer or a rabbit flees once** until it loses sight (TriggerOnce, 13.5.3.1.1.1; the rabbit's 13.5.4).
  After its 2 s run its sight is 0 until the next second's check. `fauna.lua` flees again every second it
  still sees one (Progress 28.2, found and not fixed).

**Remake:** `ai.lua` (`in_cone`, `can_see` every frame, `lose_sight_time` 3, `investigate_*`, `wander_*`,
`attack_cooldown` per zombie, `ai.noise`, `ai.hit`), `config.lua` ai, `combat.swing`, `player.lua` footsteps. The
infected's hunt of animals (28.2.5) follows the same senses.

**Keep:**
- the slow on hit, half speed for 1 s, restarted (Stage 22);
- the wander groans' own timer (Stage 22);
- the thud as the blow (Stage 28);
- infection on Veteran.

**Scenarios** (world/combat):
- a player at 90° is seen and at 100° is not;
- the reaction comes within 0-1 s;
- at 01:00: none at 250 px, one at 150, one at 250 beside a lit fire;
- lost sight stops the zombie within a second;
- one spotting warns all within 400 px;
- a warned zombie runs ±50 px of the noise for 2 s and stops;
- footsteps make noise and a swing none;
- idle bouts of about 60 px;
- three biters land on one tick;
- a deer flees once, and not again until it has lost sight.

### 17. GUN-NUMBERS — Each gun's cadence, damage, rounds, magazine and noise

**Findings:** combat#2, #4, #5, #23.

**Effort** M. **Depends on** DATA10. **Beside** CHAR-LOOK, ZOMBIE-AI, USE.

**Make identical:**
- **Automatics fire while held, at their own Every** (8.6.3):

  | rate (s) | guns |
  |---|---|
  | 0.05 | Vector |
  | 0.07 | MP5 |
  | 0.08 | AKS-74U, MAC-10 |
  | 0.09 | UMP, AUG |
  | 0.1 | M4, AK-74, RPK, Groza, VSS |
  | 0.11 | FN FAL, Bizon |
  | 0.12 | L85, Madsen |
  | 0.13 | AKM |

- **Special cadences.** The Saiga is automatic at 0.3, with 5 pellets and dispersion added twice. The AN-94 fires
  2-round and the M16 3-round bursts, 0.05 s apart (with MISSING-ITEMS). The BM-16, the sawed IZH and both bows have
  no lock.
- **Damage.** Amphibia 20, Deagle 40. Arrows: handmade 50 at 500 px/s, composite 100 at 550, either in either bow.
  Chigur fires 5 pellets.
- **Bullet speeds** (px/s):

  | speed | guns |
  |---|---|
  | 600 | the pistol round (t186) |
  | 700 | the Colt's round (t259), 12-gauge, Sporter, VSS |
  | 750 | Madsen |
  | 800 | Glock, MP5, UMP, Bizon, the M4 family, AKM, Magnum, Deagle, Repeater, Groza, AN-94 |
  | 1000 | SVD, Mosin, SKS, SV-98 |

- **No `spread_bonus`.**
- **Magazines, calibres and reload times:** combat#5's list, from 8.10.3's `Mag_reload(time, type)` branches and
  12.13.4's calibre search. For example, the FNX holds 15 rounds of .45 ACP and reloads in 2 s; the Amphibia 10
  rounds of .22 LR. Quickdraw's shorter times are ATTACH's.
- **Noise radius by gun** (13.6):

  | radius | guns |
  |---|---|
  | 600 | SVD, Mosin, BM-16, Remington, SV-98, Magnum, sawed IZH, sawed Mosin |
  | 500 | most automatics and pistols, SKS, AN-94, M16 |
  | 400 | Repeater, PM, Deagle |
  | 100 | Sporter |
  | none | bows, Amphibia, PB, VSS |

  Silenced shots are ATTACH's.

**Remake:** `data/weapons.lua`, `weapon_stats.lua`, `combat.fire`, `combat.reload_time`, `config.lua`
`gunshot_noise_radius` 600. New calibres need their ammo items (LOOT stocks them). On load, clamp a gun holding
rounds of an old calibre. Fix Progress' "not recoverable" paragraph.

**Keep:** only the fire button or Space fires (Stage 18); no auto reload (Stage 20); pistols' halved dispersion
(DISPERSION).

**Scenarios** (combat):
- an AK held fires about 10 rounds a second;
- the Saiga's 5 pellets at 0.3 s;
- the BM-16 as fast as tapped;
- the FNX holds 15 rounds of .45;
- each gun's bullet speed (table-driven);
- a zombie 450 px away hears an AK and not a Repeater.

### 18. USE — A timed use as the original's

**Findings:** inventory#F1 with character#5.

**Effort** S. **Depends on** nothing. **Beside** CHAR-LOOK, ZOMBIE-AI, GUN-NUMBERS, DISPERSION.

**Make identical.** Every 8.10.3 entry takes the item before it waits (`SetReturnValue(param-1)` first): the beans are
gone at 0.3 s (inventory e1_eat_03). The effect lands at the end.
- `var#21 = 1` gates walking (12.13.5/6/7; `AltMove.Stop()` 7.1.1), attacking (8.6.x, 8.7.1.5), interacting and every
  board action, so nothing cancels a use.
- Crafts take both ingredients first, then wait (12.10.17.33.3).
- Read 8.6's `var#21` checks to see whether firing and reloading are gated too.

**Remake:** `player.lua:369-374, 545-576` (a step or an attack cancels with nothing spent), `interaction.lua`,
`crafting.lua:9-10`, `inventory.use`. Progress' "Needs you: Interrupting a use" is answered by the original: nothing
interrupts.

**Keep:**
- the crouch with the bar over the head, and a use going on behind the shut board (Stage 25);
- only the used item locked (Stage 21);
- the bar on the item (Stage 20).

**Scenarios** (board/items):
- the beans are spent at the start;
- the stick and WASD don't move the player during a use;
- attack does nothing during a use;
- a craft spends both parts at its start.

### 19. DISPERSION — Each gun's own dispersion and its colours

**Findings:** combat#6, #13.

**Effort** M. **Depends on** GUN-NUMBERS. **Beside** ZOMBIE-AI, USE, ZOMBIE-BODY.

**Make identical.** Switching to a gun sets its default/max/pershot/cooldown/run: 12.30 for rifles (`var#1`), 12.31
for pistols (`var#44`). Every 0.1 s (8.5.2), standing still subtracts cooldown down to the default, moving adds run up
to the max, and a shot adds pershot. Pistols:

| pistol | default/max/pershot/cooldown/run |
|---|---|
| FNX | 7/20/9/4/2 |
| Colt | 6/20/10/4/2 |
| Magnum | 6/25/15/2/3 |
| Glock | 5/20/10/4/2 |
| Amphibia | 4/20/9/4/3 |
| sawed IZH | 13/30/20/3/2 |
| sawed Mosin | 12/25/25/3/1 |
| MAC-10 | 10/20/5/2.5/1 |
| Deagle | 6/20/13/3/2 |

Rifles and shotguns:

| gun | default/max/pershot/cooldown/run |
|---|---|
| Mosin | 3/25/20/1/5 |
| M4 | 4/25/3/1.5/2 |
| BM-16 | 13/30/20/1/2 |
| AK-74 | 5/25/4/1.5/2 |
| AKS-74U | 8/25/4/2/1.5 |
| SKS | 4/25/10/1.5/4 |
| Remington | 15/30/20/1/2 |
| SVD | 3/25/13/1/5 |
| MP5 | 10/20/4/2/1 |
| Saiga | 17/30/19/1/2 |
| VSS | 2/25/4/1.5/2 |
| SV-98 | 3/25/9/1.5/5 |

- The rest are in 12.30/12.31. Melee and the start are `[0,0,25,5,1,10]`.
- **Settled means dispersion at the gun's default**, not 0.
- **Tiers** (8.5.2.3, every 0.1 s, against the gun's own default and max):

  | dispersion | frame |
  |---|---|
  | <= default | 4, green |
  | default to max/3 - 1 | 3 |
  | max/3 to 2max/3 | 2 |
  | 2max/3 to max - 1 | 1 |
  | >= max | 0, red |

  The reticle and both lines share the frame.

**Remake:** `config.lua` combat (0..25 +5/-1/+10 for every gun), `combat.spread`, `add_dispersion`,
`update_dispersion`, `combat.settled`, `status.reticle_tier`, `status.cone_tier` (the two disagree today: R/r_head_4).

**Keep:**
- *"Reduce dispersion for pistols"* (Stage 20): `dispersion_scale` 0.5 over each pistol's own row;
- the cone (Stage 20), only at a target (Stage 22).

**Scenarios** (combat):
- the FNX starts at 7, climbs 9 a shot to 20 and settles 4 per 0.1 s;
- an AK held climbs 4 a shot to 25;
- moving adds run;
- the reticle and lines share each band's frame;
- pistols at half.

### 20. AIM — The reticle, the lines and what is aimed at

**Findings:**
- the reticle, hud#22 with combat#12;
- the aim lines, hud#23 with combat#15;
- melee's reticle, hud#20 with combat#16;
- combat#11, #14.

**Effort** M. **Depends on** DISPERSION, CAMERA. **Beside** ZOMBIE-BODY, POPULATION, HEAT.

**Make identical:**
- **The reticle.** `gui_target` is 40x40 at native size, on the target's origin, which is the body's centre
  (8.5.4.4.1.3.1). The remake's is 0.7x, on the feet.
- **The lines.** Two `aim_line_1`, 139x2 at opacity 0.5, pinned at the body and turned to the target ± dispersion.
  They run from about 46 px out to about 185 px, past the target (8.5.7.2), and show only with a target. The remake's
  are opaque and run from the muzzle to 72 px.
- **Melee.** With melee in hand, `gui_target` and the lines are hidden (8.5.4.4.2.1.1) and so is the zoom button
  (8.7.1.6); zoom shows only with a gun and a target. The red `gui_meleefight_ring_ramka` (46x46) is pinned to the
  target from the run's start until the first switch (2.11, 12.28/12.28.4).
- **Targets.** Player_check_targets runs every 0.2 s (8.5.1). It picks the nearest of the target family (infected,
  wolves, deer, rabbits) with PlayerAim LOS within 400 px, on screen or not. The remake keeps to the screen, within
  `edge_margin` 40.
- **The red reset.** Each re-pick restarts `gui_target` at frame 0, red, and so does the shot lock's end (8.5.5). The
  next 0.1 s tick re-tiers it.

**Remake:** `status.draw`, `status.draw_aim`, `status.cone`, `targeting.candidates`, `touch_controls.lua` `btn_focus`
(shown with any target), `player.lua` `e.focus_face`, `config.lua` targeting.

**Keep:**
- the TARGET button and the sticky target it needs (Stage 20);
- the cone only at an actual target (Stage 22);
- FOCUS (Stage 20), shown as the original's zoom button is.

The night's layering is NIGHT's.

**Scenarios** (combat):
- the reticle is 40x40 on the body;
- two half-opacity lines run 46 to 185 px;
- with fists: no reticle, lines or FOCUS;
- the red ring until the first switch;
- at 844x390 a zombie 380 px off screen is targeted and one at 420 px is not;
- the reticle is red on a re-pick.

### 21. RELOAD — Reload, switch, Load and Eject

**Findings:**
- combat#41, #45, inventory#D3;
- the reload icon, combat#42 with hud#40.

**Effort** M. **Depends on** GUN-NUMBERS. **Beside** ZOMBIE-BODY, POPULATION, HEAT.

**Make identical:**
- **Reload** (8.10.3, 8.3).
  - Rounds go in at the **start** of a reload, taken from the hands, then pants, outerwear, vest and backpack
    (12.13.4.1.2-6).
  - `Mag_reload` then locks firing for the reload's time.
  - It plays three stages at -5 dB: type 0 `pistol_magout/magin/charge`, type 1 `ak_magout/magin/chamber`, type 2
    `m4_*`, type 3 `shotgun_reload` ×6 and a pump; also `smg_*`, `sniper_*`, `mosin_*` and `magnum_reload`. Bows are
    silent.
  - Switching is refused during a reload.
- **Switching** (12.13.14, 12.26-28) takes 0.1 s. The gun's lock is global and survives a switch: Q-Q no longer skips
  a slow gun's lock.
- **The icon over the head** (8.3.1): `reloading_icon` 25x36 and `reloading_spinner` 47x47 (19 frames at 19/T fps), at
  native size, the ring's centre about 80 px over the feet. The remake draws both at 0.6x, 46 px up.
- **Load and Eject** (inventory#D3).
  - "Load main" / "Load secondary" on every ammo type, or ammo dragged onto a gun, tops up the magazine with the
    `Mag_reload` animation: 2.5 s, or 1.75 s with a belt.
  - "Eject" empties a rifle or pistol with the `eject` sound.
  - The rest of D3's list belongs to the item tracks.

**Remake:** `combat.reload` and `tick_reload` (count at the end, one sound at the start), `combat.equip` (cancels the
reload, zeroes `fire_cooldown`), `player.hold`, `player.switch_weapon`, `status.draw_head` (`reload_scale` 0.6, lift
32), `sounds.lua` `reload_*`.

**Keep:**
- no auto reload (Stage 20);
- *"when reloading a weapon, lock the ammo so it can't be moved. add a reloading icon over it"* (Stage 20): the
  board's lock and its icon;
- the switch cycle melee → pistol → rifle (Stage 20);
- "It's empty" and "I don't have ammo for this".

**Scenarios** (combat):
- the rounds go in at the start, from the hands first;
- three stages at -5 dB;
- firing and switching refused mid-reload;
- the lock survives Q-Q;
- a switch takes 0.1 s;
- the icon's size and height;
- ammo dragged onto a gun loads it;
- Eject empties it into storage.

### 22. ZOMBIE-BODY — The infected's body: no shoving, a bouncing box, what hides you

**Findings:** zombies#8, #16, #17.

**Effort** M. **Depends on** nothing. **Beside** DISPERSION, AIM, RELOAD, SHOT.

**Make identical:**
- **What blocks a zombie's sight** (13.1.1.1, AddObstacle): only `fam_b_bar_int` (room walls), `big_obstacle_base`
  and grenade smoke. Big obstacles are the `tree_block` forests, vans, trucks, the BTR, `fence_horizontal` and
  `fence_vertical`, hesco, the heli and hammer crashes, shut doors and some building parts. `obst_base` blocks walking
  only: single trees, regular cars, `obst_fence`, benches, trash containers, pillars, sandbags. Today
  `physics.sight_blocked` stops at every WALL fixture, so a pine hides you.
- **A zombie moves as its 10x10 eyes** at the centre of its 30x30 body. Its Bullet bounces off walls, forest, water and
  both obstacle kinds, and the Turret turns it back at 600°/s, so a blocked zombie jitters (zombies o1). It does not
  slide on a radius-7 circle at the feet. Progress 3.4's "the original slides" is wrong.
- **Bodies collide with nothing.** Zombies pile on one spot and onto the player, and he walks through them (o5).
  Today the ZOMBIE mask holds PLAYER and ZOMBIE, so they ring and box him in.
- Read whether the player's own target LOS (PlayerAim, 2.13/8.5) uses the same obstacles.

**Remake:** `src/physics/categories.lua`, `world.lua` `sight_ray`, `data/colliders.lua`, `config.lua`
`zombie.radius`, `ai.lua`.

**Keep:** room walls solid and blocking sight (Stage 23; identical); shut doors; no zombie made in a wall.

**Scenarios** (world/combat):
- a zombie sees past a pine and a sedan, not past a van or hesco;
- three zombies end on one spot on the player;
- the player walks through one;
- one chasing into a wall bounces rather than slides.

### 23. SHOT — What a shot looks and sounds like

**Findings:** combat#18, #19, #20, #21, #22.

**Effort** L. **Depends on** GUN-NUMBERS, AUDIO. **Beside** ZOMBIE-BODY, POPULATION, HEAT.

**Make identical:**
- **Every shot** shows `muzzle_sprite` (3 frames at 24 fps, Fade) and `muzzle_dust` at the muzzle (8.6.2.2.x).
  `muzzle_flash` (night layer, Overlay, fade 0.2) shows at night, and always for the SVD, Mosin, SV-98, sawed Mosin
  and sawed Repeater. Its hole in the dark is LIGHTS'.
- **Shells.** `test_shell` (`test_shell_12cal` for 12-gauge) lands at the player ±2 px: 7 frames at 10 fps, stays 10
  s, fades 2. None for the Magnum, BM-16, sawed IZH or bows. Bolt and pump guns eject at the end of their cycle.
- **The bar after a single shot.** After every non-automatic shot, `Wpn_cooldown(var#4)` (8.6.4/8.6.5) shows the
  30x3 bar at the centre -19, its fill a sine of period 4× the cooldown, hidden when full.
- **Each gun's own sample and offset** (combat#21's list), and `empty_click`. The remake's `combat.weapon_class` plays
  four classes, and plays `ak74_2` and `izh_3`, which no 1.0 event plays.
- **Cycles,** mid-cooldown: `mosin_cycle` at T/5, `shotgun_pump` at T/3, `repeater_cycle` and `sniper_cycle` at T/4.

**Remake:** `combat.fire`, `weapon_class`, `effects.lua`, `status.draw_head` (the bar only for melee and uses),
`sounds.lua`. All 279 samples are in the build. AUDIO sets the levels.

**Keep:** the bar for timed uses (Stage 20); the silenced samples (ATTACH).

**Scenarios** (combat):
- a shot's sprite and dust;
- a shell per AK round, a 12cal shell per Benelli shot, none for the Magnum;
- an SVD flashes at noon;
- the FNX's bar refills over its cooldown;
- each gun plays its own sample (table over `weapons.lua`);
- the Mosin cycles at T/5.

### 24. BALLISTICS — How far a round goes and what stops it

**Findings:** combat#9, #10, #32.

**Effort** M. **Depends on** nothing. **Beside** POPULATION, HEAT, FOOD; not beside SHOT (`effects.lua`).

**Make identical:**
- **Range.** A round dies at 360 px (16.2.1) with a `test_groundhit` puff (a1, 6 frames at 10 fps, fading) and
  `muzzle_dust`, silently (correction 9).
- **Range tiers** (16.3 Range_less_power). Tier 3 drops to 2 after 100 px and to 1 after 220.
  - Headshot power is 15/10/5 by tier (13.2.8.4.1.2.1-3), where the remake has a flat 15.
  - An `obst_base` stops a round only if its tier <= `choose(1,2)`, so close shots pass fences and bushes.
- **Hitting an obstacle** (16.1.1): `test_bullet_hit` (4 frames at 15 fps, fade 0.2) and `hard_ground_1/2`. Tag
  `ground_hit`: one at a time, within 2000 px.
- **Arrows** that hit drop back as an item: 1 in 4 from the bow, 1 in 2 from the crossbow (16.1.7).

**Remake:** `cfg.combat.bullet_range` 400, `bullet.lua` (silent at range; every WALL stops every round;
`impact.wall = {}`), `categories.lua`, `combat.headshot`.

**Scenarios** (combat):
- a round dies at 360 px with a puff;
- a fence passes every round at 80 px, half of them at 150 and none at 300;
- a wall hit's puff and one `hard_ground`;
- headshot power by range;
- an arrow drops 1 in 4.

### 25. POPULATION — Groups at points, made out of sight, gone past 2000

**Findings:** zombies#35, #36, #37, #38.

**Effort** L. **Depends on** CAMERA, ZOMBIE-NUMBERS. **Beside** SHOT, BALLISTICS, HEAT, FOOD.

**Make identical:**
- **Points.** One `zed_resp_point` per location at hotspot+500 (3.16.1.N), its frame set by the location's kind.
  Zed_check (13.7.1.21) fills it from a table:

  | frame | kinds | fill |
  |---|---|---|
  | 0 | village houses 24, also 7, 9 | 4 normal at ±350, then 1 of `choose(1,2,3,4,6,2,5,7,16,17)` at ±250; 1 time in 6 instead 4 buried + a screamer |
  | 1 | | 5 normal-or-runner at ±400 + a screamer, or 3 of skin7/skin8/spitter |
  | 2 | | 5 normal + 2 mixed |
  | 3 | 14 | 2 army at ±400 + a shooter |
  | 4 | 5, 22 | 5 army + a heavy |
  | 8 | 26, 40, 41 | 2 army + 3 shooters |
  | 16 | 70 | 5 army + 10 shooters |
  | 5-7 | | wolves, deer, rabbits |

  Kinds the remake lacks come with SPECIALS; leave them a hook and spawn nothing in their place.
- **Made only 1500-2000 px from the player:** at layout start and at each Zed_check (once a second after the camera
  has moved 400 px, 3.5.1). A fresh run has none near you.
- **Removed past 2000 px** (`respawn_radius_npc`), freeing the point (13.7.1.9-14).
- **Blinded past 1500 px:** `npc_blinder` (20.7) sets their sight to 0 every 3 s; they still wander.
- Check `ZED_RESPAWN_TIME` (10 in 1.0). `Zed_per_base` is written in 31 places and read in none (events#5).

**Remake:** `data/places.lua` spawns, `config.lua` population (`per_point` 1, scatter 40, `spawn_inner`/`outer`
760/1020), `src/map/loader.lua:97-110` (a point's zombie made when its lot is built), `population.update`,
`sweep_due`, `sleep`, `ai.set_asleep`, and 28.2.10's animal points (the forests' frames 5-7; COUNTRYSIDE). A save
keeps the live zombies.

**Keep:** lots and their items kept (Stage 6). That ask is about items; zombies come and go as the original's do.

**Scenarios** (world):
- no zombie within 1500 px at START;
- walking toward a village fills its point with 4 normal and 1 more at 2000-1500 px;
- a zombie 2100 px off is gone and its point refills on return;
- one past 1500 px sees nothing;
- re-measure 27.3.31's sleep pass.

### 26. HEAT — Heat as the original's temperature

**Findings:**
- survival#5, #6, #7, #8, #9, #15, #24;
- the heat text, hud#5.

**Effort** M. **Depends on** SURVIVAL, DATA10. **Beside** SHOT, BALLISTICS, POPULATION, MISSING-ITEMS.

**Make identical.** Update_Stats (6.3) rebuilds `Player_temperature` from 0 each call, and each 5 s tick heat +=
`Player_temperature / 2` (6.2.2.3), capped at 100. The terms:

| term | value | event |
|---|---|---|
| night | -Night_cold | 6.3.4 |
| rain outside a warmzone | -Rain_cold | 6.3.5; RAIN |
| a lit fire | +15 | 6.3.6, within `fire_place_warm`'s 100x100 |
| a room's warmzone | +var#0 | 6.3.8 |
| a car | +4 | 6.3.10; out of scope |
| always | -(Day_cold - worn warmth) | 6.3.11 |

- **Day_cold** is 6 Novice, 7 Regular, 8 Veteran and Legend (2.11.9-12), and 9 on levels 4 and up (band 5).
- **Night_cold** is 1/2/3/3, flat from 00:00 to 06:00, indoors too.
- **Worn warmth** counts only the helmet (`var#8`), outerwear (`var#18`) and pants (`var#20`), each worth
  `ceil(var#9 / 100 × condition)`. A raincoat in condition also cancels Rain_cold.
- **A room's `var#0`:**

  | warmth | rooms |
  |---|---|
  | 1 | army tents |
  | 2 | bus, garages, gas station, gun shop |
  | 3 | castle, pillbox, hospital, hunter's shelter, sheds, supermarket |
  | 4 | bar, hostel, barracks, school, village houses, yard, a deployed tent |
  | 5 | church, city houses, fire station, barracks 2, HQ, piano house, police station, red brick |

  A room has no cap, and the night still counts inside.
- **The east** (kept, Stage 26) becomes points of Day_cold rising across the bands, reaching the original's 9 in the
  snow.
- **Cold sickness.** At heat <= 0, each tick has a 44% chance (`random(50) > 28`) to make the player sick unless
  immune (6.2.2.14).
- **The bar's text.** `gui_heat_text` centred on the heat bar shows "+N" in orange while the temperature is > 0, "-N"
  in blue while < 0, nothing at 0 (6.3.18.2, 6.3.20.2). There is no arrow: `gui_heat_arrow` is destroyed at layout
  start (2.11.4).
- **No "It's getting colder."**
- **Measured:** `S/orig/nov_start.png` (+1), `reg_naked.png` (-7), `reg_night_naked.png` (-9), `in_house_day.png`
  (+4), `sleep_pad.png` (+5), `fire_night.png` (13).

**Remake:**
- `survival.tick_drain` (`heat_rate` 0), `survival.shelter` (+1.5 to 80), `survival.by_fire` (+5 within 56);
- `survival.insulation` (/40, cap 0.75), `survival.east_cold`, `note_band`, `daynight.heat_drain`;
- `config.lua` `sick_cold_*`, `hud.lua` `icon_warming`, `data/buildings.lua` (each room's warmth).

**Keep:**
- heat by a fire and indoors (Stage 24), as their two terms;
- cold sickness's cure (Stage 23);
- colder going east (Stage 26).

**Scenarios** (play/items):
- naked by day on Regular: -3.5 a tick;
- in the kit by day: 0 on Regular, +0.5 on Novice;
- naked at 02:00: -4.5;
- a village house: +2 in the kit;
- a fire: +15 at 50 px, nothing at 120;
- the bar reads "+4" and "-7";
- 44% a tick at heat 0 (seeded);
- band 5's Day_cold.

### 27. FIRE — The campfire's life

**Findings:** survival#10, audio#9, #10.

**Effort** S. **Depends on** nothing (HEAT owns the +15). **Beside** BALLISTICS, POPULATION, FOOD, MISSING-ITEMS.

**Make identical:**
- **Lighting** takes 3 s with `match_burn` at 0 dB, flat (8.10.3.24.2.1).
- **A fire burns 180 s a time, three times.** A new fireplace has `var#1 = 3` (8.10.3.23.2). Each burn's end takes
  one, and at 0 the fire is destroyed (18.2.1, .1.3).
- **Setting the kit down** takes 3 s and makes no sound (8.10.3.23.2). Build mode's placements are silent but the
  tent's (12.8.3.1.2-7).
- **Sleeping burns a fire down** by the time slept (SLEEP).

**Remake:** `campfire.lua` (`light_time` 1.5, `burn_seconds` 600, relit forever), `build.place` (plays `craft`),
`sounds.lua` `fire.light` = `matchstrike_2`.

**Keep:**
- the two kits and their recipes, and build mode (Stage 24);
- lighting by the fire from the kit only, with no button (Stage 26).

**Scenarios** (fire):
- lighting takes 3 s with `match_burn`;
- a fire burns 180 s three times and then is gone;
- placing plays nothing and takes 3 s after the tick.

### 28. FOOD — What food and drink do

**Findings:**
- the values, inventory#G1 with items#27;
- inventory#F5, #I3;
- character#16, items#28.

**Effort** M. **Depends on** SURVIVAL. **Beside** BALLISTICS, POPULATION, FIRE.

**Make identical:**
- **Values** (8.10.3; 2 s each unless noted; food/water):

  | item | food/water | also |
  |---|---|---|
  | beans | 30/5 | |
  | tuna | 20/0 | |
  | bacon | 35 | |
  | rice | 65 | `EatingCrunchy_0` |
  | tomato, apple, banana | 15/5 | |
  | Pipsi, Spite, Nota-Cola | 0/30 | |
  | kvas | 0/50 | |
  | beer | 0/30 | +10 heat |
  | whiskey | 0/20 | +40 heat |
  | cranberry | 15/15 | |
  | cloudberry | 10/10 | then 30 s of doubled regeneration |
  | bilberry | 10/20 | |
  | elderberry | 10/10 | +2 hp |
  | zucchini | 20/20 | |
  | bell pepper | 20/5 | |
  | orange | 15/15 | |
  | MRE | 40/40 a half | 3 s, two halves |
  | small fillet | 20/10 | +5 hp, 3 s |
  | big fillet | 30/10 | +10 hp, 3 s |
  | Nuko Cola | water to 100 | then 60 s of doubled regeneration |
  | energy drink | 0/20 | +10 px/s for 60 s |

- **Doubled regeneration** is a survival flag (`var#33`): the tick adds regeneration twice (6.2.2.10.1). MEDS uses it
  for vitamins.
- **The energy drink's** +10 px/s does not speed the run cycle, which stays at 10 fps. The remake's is ×1.25 on both.
- **Sounds:**
  - `EatingSoft_0` at -10 dB at the player, `EatingCrunchy_0` for rice;
  - `DrinkSoda_0` for sodas;
  - `Whiskey` for whiskey, the bottle and the canteen;
  - `nuko` at 0 dB for Nuko Cola.
- **Raw fish** (8.10.3.78-81). At a burning fire, a herring or ruffe cooks into a small fillet and a salmon or perch
  into a big one: 5 s, `meat_cook`. Eaten raw (8.10.5.17-20): 3 s, 20/10, a 1 in 2 chance of sickness. Fillets come
  only from cooking (LOOT takes them out of kitchens).
- Cooked steak, cooked rabbit and raw meat are already identical (28.2).

**Remake:** `data/items.lua` (food, water, `use_time`), `inventory.use`, `survival.eat`, `survival.boost`,
`player.lua` `e.anim.rate`.

**Keep:** the energy drink's speed (Stage 23), refreshed not stacked.

**Scenarios** (items):
- each food's values (table-driven against the original's);
- 2 s uses and their sounds;
- the drink at 110 px/s for 60 s with the run cycle at 10 fps;
- Nuko Cola and cloudberry's doubled regeneration;
- raw fish cooked into a fillet.

### 29. MEDS — What medicine and cigarettes do

**Findings:**
- medicine, inventory#G3 with survival#31 and items#30;
- inventory#G4;
- survival#4, #26, #27, #30.

**Effort** M. **Depends on** FOOD. **Beside** LOOT, CAMPS-TRUNKS, MENU-LOOK.

**Make identical** (8.10.3):

| item | the original |
|---|---|
| bandage | only while bleeding: 2 s, `bandage` -5 dB; otherwise "I'm not bleeding." and kept (.18.2) |
| rags | the same, in 5 s |
| adrenaline (the morphine icon) | hp to 100, 1 s, `adrenaline_use` -10 dB; +20 px/s for 10 s, the boost icon's "Adrenaline"; +50 hp a second for those 10 s (6.18), then capped at 25 (6.19) |
| blood bag | +50 hp, 6 s |
| saline | +25 hp, 6 s |
| IV kit | needs no sickness, no bleeding and over 50 hp; 6 s; takes 50 hp and makes a blood bag, "Blood transfusion completed." (.96) |
| tetracycline | always taken, 3 s; clears the sickness, then 300 s immune (`var#51`, .40, 8.10.10): no sickness from cold, raw food, acid or spitters |
| vitamins | `vitamins` -10 dB, 2 s, 60 s of doubled regeneration (correction 1) |
| heatpack | +25 heat, 3 s |
| cigarettes | need a fire in reach, matches or a lighter (8.10.1; `matchstrike_2` / `lighter_0`); 2 s, `Smoking_1/2`; 60 s "Smoked": no food or water taken (6.2.2.1/2), a puff every second (CHAR-LOOK), `gui_smoke` on the plates (HUD-INDICATORS) |

- The adhesive plaster is unlock-gated: a fresh profile gets a bandage (LOOT).
- The lines go through LINES; the report lines wait on Q3.

**Remake:** `data/items.lua`, `inventory.use` (bandage spent unconditionally), `survival.cure` (infection only, 23.7.3),
`survival.heal`, `survival.boost`.

**Keep:**
- infection cured by tetracycline on Veteran (Stages 23, 26): the pill cures both kinds of sickness now;
- cold sickness's cure at 75% for 90 s (Stage 23);
- the use bar (Stage 20).

**Scenarios** (items):
- a bandage refused and kept when not bleeding;
- adrenaline: 100, then speed, then capped at 25 at 10 s;
- the IV kit takes 50 and makes a blood bag;
- tetracycline taken with nothing to cure gives 300 s of immunity;
- vitamins double regeneration;
- a cigarette with matches stops hunger for 60 s.

### 30. HUD-INDICATORS — Regeneration heart, icons' animations, boost's place

**Findings:** hud#4, #31, #32, survival#28.

**Effort** S. **Depends on** MEDS, HUD-TOP. **Beside** LOOT, CAMPS-TRUNKS, MENU-LOOK.

**Make identical:**
- **The regeneration heart.** `gui_regeneration` over the plate's heart: frame 1 (green, arrow up) while
  regenerating (6.2.2.10), frame 2 while doubled (6.2.2.10.3), frame 0 (grey) otherwise (6.2.2.11, 6.2.5).
- **The sick icon.** `gui_sick` plays "sick", 2 frames at 1 fps, and "immune" while immune (6.3.25-27).
- **The boost icon.** `gui_boost` plays "Boost" or "Adrenaline", 2 frames at 3 fps, the whole time (8.10.3.56.1,
  .39.1). It is pinned to `gui_regeneration`, 83 px left of the heart, left of the shield.
- **No arrow badge on the sick icon.** `icon_sick_chilled` and `icon_sick_mending` (23.8.3) go.
- **`gui_smoke`** on the plates while smoking.

**Remake:** `hud.lua` (`sick_0` static; the boost under the plates, pulsing at 2 Hz in its last 10 s).

**Keep:** the status icons at the end of their bars (Stage 17); the infection icon's stand-in (23.7.5).

**Scenarios** (hud):
- the heart's three frames;
- the skull at 1 fps;
- the boost at 3 fps, 83 px left of the heart;
- no badge.

### 31. MISSING-ITEMS — Eleven items that share an icon name

**Findings:** items#20 with combat#47 (the guns; the rest of #47 is in KNIVES, LAUNCHERS, THROWABLES and DEPLOY).

**Effort** M. **Depends on** DATA10. **Beside** HEAT, FIRE.

**Make identical.** Eleven object types draw on another's sheet, each with its own drop id, name and icon (objects.txt
t71, t140, t139, t117, t149, t143, t148, t126, t119, t134, t457):

| id | item | drawn on |
|---|---|---|
| 152 | Colt 1911 | fnx_pistol |
| 181 | PM | fnx_pistol |
| 182 | PB | fnx_pistol |
| 173 | UMP-45 | mp5_smg |
| 192 | Vector | mp5_smg |
| 188 | AN-94 | ak74_rifle |
| 191 | M16A2 | m4_rifle |
| 180 | Mare's Leg | sawed_mosin (never spawned in 1.0) |
| 404 | Kevlar vest | razgruz_big, armour 10 |
| 405 | Soviet vest | razgruz_big, armour 8 |
| 457 | Tortilla backpack | hunter_backpack, 6 cells |

`items_index.lua` is keyed by the file name after the id, so `152_fnx_pistol` and `151_fnx_pistol` collide. Key by
drop id. All 11 icons are in `Assets/Icons`. Each item's numbers are in combat#2-6 (bursts, rates, magazines,
calibres) and combat#23 (noise), and their attachments in items#40.

**Remake:** `tools/build.py`, `items_index.lua`, `data/items.lua`, `weapons.lua`, `weapon_slots.lua`.

**Scenarios** (items):
- each of the 11 spawns with its own icon and name, worn or held on its host's art;
- the AN-94 bursts 2 and the M16 3.

### 32. LOOT — One thing a point, from the live lists

**Findings:**
- items#1, #3, #4, #5, #6, #7, #8, #14, #15, #16, #19, #21;
- found condition, inventory#B7 with items#26;
- the infected's drops, zombies#32 with items#44.

**Effort** L. **Depends on** MISSING-ITEMS. **Beside** MEDS, HUD-INDICATORS, MENU-LOOK.

**Make identical:**
- **The live lists are Trigger_spawn's first branch** (14.1.4.1; correction 16). `extract_rooms.py`'s `TYPE_PIECE`
  comment, the gun shop table and Progress 10, 16.2.14 and 27.1.3 read the dead branch.
- **One drop per point**, from its type's short list (items#4's table of 22). Types 2, 4, 13 and 16 can drop a gun with
  its rounds. The chance by type:

  | chance | types |
  |---|---|
  | 100% | 1, 5, 7, 8, 9, 11-16, 18-22 |
  | 75% | 10 |
  | 50% | 2, 4, 6 |
  | 37.5% | 3 (one of its four is item 351, which does nothing) |
  | 1% | 17 |
  | 0% | 23, 24 |

  No building drops canned food, fruit or sodas. The remake's 1-3 draws a piece from ten invented tables go
  (items#3's table: a house holds 1.4-4x the original's).
- **Types 23 and 24** get no piece: nothing ever lies there.
- **Points to add:** the deer stand's type-2 point, 15 px below its foot (3.5.7.1.6.1.1; `b_deer_stand` in
  `buildings.lua`), and the pillbox's 1 in 100 Nuko Cola (`round(random(1,100)) = 55`).
- **The gun shop's rack** (type 19) is always one of 15: Bandolier, FNX, Colt 1911, Magnum, IZH-43, Benelli, Mosin, SKS,
  Sporter 22, Repeater, Glock, MAC-10, Saiga 12k, PU scope, PM.
- **Unlock-gated items** take the fresh profile's fallbacks (items#21).
- **Found guns** are at 100%, and half the time hold round(random(N)) rounds (items#14's table); otherwise 0.
  `clamp_ammo` fills them today.
- **Counts, charges and fills** (items#15's table): for example 5.56 boxes of 15-25 rounds, matches 1-5, water bottles
  0/25/50/75/100% full. Fills and charges act once WATER and LIGHT-ITEMS land.
- **Condition.** Everything is at 100% but the raider jacket (`choose(25,25,50,50,75,100)`). Knives roll
  25/50/75/100 and fishing rods 50/75/100 (`config.lua` `condition_roll` goes).
- **Respawn.** Every 1500 s (`LOOT_RESPAWN_TIME`), each location more than 1800 px away re-fires its points (14.1.2.7).
  Here that means each piece and trunk past 1800 px rolls its point again into its free room. The cull of loose
  items stays out (Stage 6).
- **The infected's drops,** within 500 px of the player, at the corpse's y+15 (13.3.1.2.2.13, .4.5, .5.6):

  | kind | chance | list |
  |---|---|---|
  | normal | 2 in 7 | one of 19 |
  | army | 1 in 7 | one of 12 |
  | runner | 2 in 7 | one of 20, item 97 among them (Q5) |

**Remake:** `loot_tables.lua`, `containers.lua` and `trunks.lua` (`rolls`), `systems/loot.lua` (`stock`, `place`,
`update`), `buildings.lua` (each spot's `table =`), `config.lua` loot and items, `inventory.new_item`, `combat.kill`.

**Keep:**
- a piece of furniture on each point (Stage 23), with the piece's kind as today;
- no cull of loose items (Stage 6).

**Waits on Q4:** the makings by the spawn and the homes.

**Scenarios** (items/world):
- 600 seeded points a type match 14.1.4.1's rate and list;
- a house's expected count matches items#3's table;
- a found gun is at 100% with 0 or up to N rounds;
- 600 kills give the drop rates;
- a refill comes after 1500 s, and only past 1800 px.

### 33. CAMPS-TRUNKS — Trunks, the camps' boxes and the crash

**Findings:** items#9, #10, #11, #47.

**Effort** M. **Depends on** LOOT. **Beside** MEDS, HUD-INDICATORS, CAPACITY, MENU-LOOK.

**Make identical:**
- **Car trunks.** Every regular car, van, police car and hatchback has a `car_loot_point` while unsearched
  (3.5.7.1.5.N.1). Opening one (12.13.3.1.2) plays `open` at -5 dB and lifts the lid (frame 1). Below 50 on
  `random(100)`, it gives one item from its frame's list; otherwise nothing:

  | frames | cars | one of |
  |---|---|---|
  | 0, 3, 5 | regular cars | 25 |
  | 1 | vans | 41 |
  | 2, 4 | police cars | 27 |

  The lines "I found something in this trunk." and "There is nothing in this trunk." wait on Q3. No trunk on the
  truck, UAZ or BTR.
- **The camps' boxes** (3.5.5.1.33-48). Each `bunker_lootbox` has a fixed kind with fixed contents (items#10's table:
  an SVD with ammo, an M60, the Engraved Colt, a Groza with a pan…), some with things on the ground beside it. The
  tents camp lights a fireplace and drops 8 things round it, and the stones camp has berry bushes. The hunters' camp
  has its hut's one-time stash (3.5.7.1.6.32.1.1; 27.1's found in review) and the deer stand's point (LOOT).
- **The humvee crash:** three type-18 points at the wreck's image points 1-3 (3.5.5.1.N.10), each one of 43 military
  things.

**Remake:** `trunks.lua` (every car, van, police car, truck and UAZ, from `car`, `police_car` and `convoy`),
`camps.lua` (a military crate in every camp), `crash.lua` (one convoy crate).

**Keep:**
- *"Lootable vehicle trunks"* (Stage 23): a trunk opened from its tab, its stock the original's single roll;
- the crash count of 27.3's quota.

**Scenarios** (items):
- about half of 400 seeded trunks of each kind hold one item from their list;
- trucks and UAZs have no trunk;
- each camp's box holds its kind's things;
- the crash's three points.

### 34. CAPACITY — One hand slot, one-row bags, the original's capacities and stacks

**Findings:**
- inventory#A10, #B1, #B2, #B3;
- stacks, inventory#B4 with items#25.

**Effort** M. **Depends on** nothing. **Beside** CAMPS-TRUNKS, MENU-LOOK, GROUND.

**Make identical:**
- **One hand slot** (`Arr[t388]` size 1, `hand_slot`) that takes anything. The remake has six pockets
  (`pocket_slots` 6).
- **Backpacks**, one row on a 7-cell card (14.2.175-183):

  | bag | cells |
  |---|---|
  | crossbody (barsetka) | 1 |
  | school | 2 |
  | taloon | 3 |
  | improvised bag | 3 |
  | hunting | 5 |
  | improvised backpack | 5 |
  | tortilla | 6 |
  | mountain | 7 |
  | civilian tent | 0 |

- **Garments** (14.2.184-232):

  | slot | cells |
  |---|---|
  | outerwear | T-shirt 0, shirt 1, raincoat 2, raider jacket 1, sweater 1, awesome hoodie 3, gorka 3, hoodie 2, cloak 1, paramedic 2, orel 2, tracksuit 1, dress 1, down 2, hunter 3 |
  | vests | bulletproof 0, press 1, assault 2, high-capacity 3, kevlar 3, soviet 2, apron 1 |
  | pants | jeans 2, worker 3, hunter 3, tracksuit 1, gorka 3, orel 2, paramedic 2 |

- **Pick-up fill order** (8.12.2-6): hand, trousers, outerwear, vest, backpack. In each, a same-item stack with room
  first, else the first empty cell.
- **Stacks** (8.12.7):

  | stack | items |
  |---|---|
  | 3 | bandage, rags, fruit, vegetables |
  | 5 | flare, berries, wood sticks, arrows, 40mm, VOG |
  | 10 | matches |
  | 15 | 12-gauge |
  | 20 | 7.62 |
  | 30 | 5.56, 7.62x39, 5.45, .357, 9x39 |
  | 40 | .45, 9mm, 9x18 |
  | 50 | .22, .308 |
  | 1 | everything else |

**Remake:** `config.lua` items, `data/items.lua:464-473, 505-543`, `inventory.lua` (`containers` order, `grid_cols`,
`may_hold`), `data/ui/inventory.lua` (the Pockets card and the backpack card), `save.lua`. An old save with six full
pockets loads with nothing lost: move the extra to the bag or the feet.

**Keep:**
- weapons and spare clothes in backpack cells at 1x2 and 1x3 (Stage 20); in a 7-cell row a rifle takes 3;
- durable things never stack (17.4.14; identical);
- auto-equip into an empty slot (Stage 20).

The board's look is BOARD-LOOK's (Q2). This track draws one hand cell and one-row bags in today's board.

**Scenarios** (inventory/board):
- one hand cell;
- a mountain backpack holds 7 in a row;
- jeans hold 2;
- a pick-up fills in the original's order;
- each stack's cap;
- an old save loses nothing.

### 35. BOARD-FEEL — Taking, dropping and dragging as the original's

**Findings:**
- inventory#C2, #C5, #C6, #C7, #C8, #E1, #E3, #E4, #E5, #I1, #I2;
- items#46 (the trunk's `open`; the bush's sound is WATER's and attaching ATTACH's).

**Effort** M. **Depends on** CAPACITY. **Beside** MENU-LOOK, MENU-MOTION, DIFFICULTY, GROUND.

**Make identical:**
- **The Ground list** holds only what overlaps the player's 30x30 box (12.15.1.1-2), where `reach_range` is 90 px.
  Gear and weapons first, then the rest in instance order. Today it is nearest first.
- **A double tap on a row takes it** (12.15.4): a weapon or garment straight on, anything else into storage. A tap
  still opens the asked tooltip.
- **The HUD's pick-up** with several things underfoot takes one at random (`PickRandom`, 12.13.3.1.5.1).
- **A drop lands at the feet,** 11-15 px below the body's centre (8.13.2), not 22 px ahead.
- **Dragging off the board drops the thing** (12.14.5.1 Drop_slot, with `drop1/2`; worn gear and weapons through
  12.14.4.1.1, with `geardrop1/2`). Today it snaps back.
- **While dragging, only the cell under the finger lights** (`gui_slot_bg`, 12.14.1): green empty, blue occupied,
  yellow to combine or craft.
- **Dropping the same item onto itself** asks "Stack" (12.10.10.1.3-28).
- **Dropping onto a partner** asks its recipe ("Craft fireplace kit", or two rows; 12.10.10).
- **Sounds.**
  - Picking up: `pick1/pick2` at -5 dB into a cell (8.12.2.2), `gearpick` for clothes and helmets, `pick_rifle` and
    `pick_pistol` for guns (8.11).
  - Dropping: `drop1/drop2` (8.13.1.1), `geardrop1/2` for worn gear.
  - A trunk's first opening: `open` at -5 dB (12.13.3.1.2.1).

**Remake:** `board.lua` (merge on release, `craft_plan`, every target lit), `data/ui/inventory.lua` (snaps back),
`item.lua` (`nearest`, `within`, `drop_from`), `config.lua` items, `container.lua`, `sounds.lua`.

**Keep:**
- NEARBY's tabs, ground first then by range (Stage 22);
- containers as drop targets;
- TAKE ALL (17.4);
- the tooltip on a tap, and MAKE on it;
- the ghost under the finger (Stage 20);
- a weapon landing where its ghost is (Stage 22);
- only a red tint on refusal (Stage 20);
- a pick-up never opening the board (Stage 20);
- "My inventory is full" (Stage 22).

**Scenarios** (board):
- only what touches the box is listed;
- a double tap takes;
- a drag off the board drops at the feet with the sound;
- one cell lit;
- "Stack" and the recipe asked;
- the pick and drop sounds by kind.

### 36. BOARD-LOOK — The board as the original's picture (waits on Q2)

**Findings:**
- inventory#A2, #A3, #A5, #A7, #A8, #A9, #A12, #C3, #C4;
- the HUD round it, inventory#A4 with hud#35;
- Q2's inventory#A1 and #A6.

**Effort** L. **Depends on** Q2, BOARD-FEEL, HUD-TOUCH. **Beside** MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, GROUND, LOTS.

**If Q2 says rebuild:**
- **One picture.** `gui_inventory-sheet0.png` (362x283 art px, origin 181,146) at the screen's centre
  (12.13.10.1.1): about 700x547 at 1280x720 (x 288-988, y 72-619) and 435x342 at 844x390.
- **No dim** round it. The HUD stays up and usable round it but the stick (12.13.10.1.3).
- **Cards at its image points:**
  - `card_0..3` at x 128: vest y 12, outerwear 78, pants 142, backpack 208;
  - helmet (237,32), hand slot (239,91), melee (226,146);
  - pistol (319,33) and firearm (320,129), upright;
  - the backpack's 7 cells along the bottom, Ground the left third.
- **No captions and no names.**
- **Text formats.**
  - Garments: "(100%)" and "Heat +5" / "Armor +5" in a dark bold serif.
  - Guns: a band with "100%" left and the rounds right.
  - Counts: white, centred under the icon, with no "x". A bottle shows its water ("75%").
  - The Ground list writes "x25" and "(100%)".
- **Attachments are drawn on the gun's picture** at its image points (8.10.3.64+), with no empty mounts.
- **7 rows a page** (12.15.2).
- **Ground rows** are `gui_pickpile_panel` (108x33 art): the icon left, the name over its numbers in white. Ordinary
  rows lose the hand.

**If Q2 says keep the remake's board:** only A3, the formats of A7 and A12, A9, C3 and C4 go ahead.

**Remake:** `data/ui/inventory.lua` (W, H 848x478, `inv_dim`, `bg_rust`, `vit_*`, `inv_bag`, `inv_pad`, `NEAR_ROWS`),
`board.lua` (`lay_out`, the rows' text, `rounds_of`), `attachments.fill`. Opt into HUD-TOP's gui scale.

**Keep:**
- NEARBY's tabs of the containers' icons, the ground first (Stage 22), 50x62 (Stage 26);
- TAKE ALL (17.4);
- the foot pager, darkened, with no page number (Stage 22);
- the tooltip (17.4);
- weapons as one wide well (Stages 20, 21);
- items centred (Stage 25);
- condition at 100% and the rounds shown (Stage 22);
- only the used item locked (Stage 21);
- the bag opening and shutting the board (Stage 22);
- *"shrink the menu a little"* and *"Place the health stats at the top"* (17.4): the HUD's plates are at the top once
  it stays up.

**Scenarios** (board/phone):
- the board's rect at both sizes;
- the HUD pressable round it;
- no dim;
- cards at the points;
- 7 rows;
- the formats;
- a scope drawn on the gun;
- F12 finds no overlap.

### 37. NAMES-TEXT — The original's names and words

**Findings:**
- names, inventory#K1 with items#22;
- inventory#D4, #D2.

**Effort** M. **Depends on** TYPE. **Beside** MENU-LOOK, MENU-MOTION, DIFFICULTY, GROUND, LOTS.

**Make identical:**
- **Every item's name from `l_eng_items.xml`** by drop id: "AK-74", "Hatchet", "FNX pistol", "T-shirt", "Worker
  pants", "Assault vest", "Combat Gasmask", "Water bottle", "5.45x39 ammo", "Adrenaline", "Crossbody bag", "Hunting
  backpack", "Wood piles", "Wood sticks", "Papers", "Campfire kit" (item 23)… 140 of 213 differ. The campfire starter
  kit is the remake's own item and keeps its name.
- **The tooltip's buttons say the original's words,** in sentence case: "Eat", "Drink", "Apply bandage", "Use
  heatpack", "Load main", "Stack", "Craft …", "Detach …".
- **Its facts are the original's description text** (12.11.2, `l_eng_new`). Apple: "Food +15 / Water +5 / Max stack:
  3". AK-74: "Damage: 18-32 / Accuracy: 7 / Aim Speed: 5 / Rate of fire: 600 / Ammo: 5.45 / Mag size: 30".

**Remake:** `item.title` (the file name made readable), `item.label`, `data/items.lua`, `board.lua` (`facts_of`,
`info_of`, `verb_of`, `where_of`), `item_tip.lua`.

**Keep:** the tooltip and its buttons (17.4). The user's words "wood", "stick" and "newspaper" were not renames.

**Scenarios** (items):
- every title equals `l_eng_items`' name by id (table-driven);
- the apple's and the AK's facts;
- the buttons' words.

### 38. CONTROLS — Tap to move, and a stick you place

**Findings:** hud#36 with character#17, menus#26 and inventory#J3.

**Effort** L. **Depends on** HUD-TOUCH. **Beside** NAMES-TEXT, MENU-LOOK, GROUND, LOTS; not beside PAUSE (`settings.lua`).

**Make identical:**
- **The phone's default is tap-to-move** (`GUI_control_type` 0, 3.6.3.1). Holding anywhere walks toward the finger; a
  tap drops `walk_marker` (6 frames at 10 fps) and walks to it (12.13.5).
- **Option: a fixed stick** (`gui_dpad_field` 250x250).
  - Its place is set in the menu's "Stick position": it hides the sky and logo, dims 75% and shows the HUD's buttons,
    with a ring to drag and Save and Reset plates (ME 3.2.14, 3.2.2.32-33; LocalStorage `stick_pos_X/Y`).
  - Any push is full speed (`vector × 100`, capped at 100; 12.13.6.4.1). EightDir accelerates at 600 px/s², 0 to 100
    in 0.17 s, and letting go stops at once.
- **Option: WASD** (12.13.7).
- **The pause's "Movement: By tap / Stick / Keyboard (WASD)"** cycles the three (12.9.3.5).
- **In tap mode, a tap on the world shuts the board** (12.13.5.3.1).

**Remake:** `touch_controls.lua` `move_stick` (floating, analog, deadzone 0.18), `player.heading` (half tilt is half
speed, no ramp), `ui.lua`, `settings.lua`, `core/settings.lua`.

**Keep:**
- the touch toggle (Stage 17);
- *"Only the fire button should fire the weapon"* (Stage 18): a tap moves, never fires;
- the dead zones (Stage 18);
- a press on a button is the button's (27.4.5);
- the notebook shut by a press off it (27.4.3).

**Scenarios** (controls/phone):
- tap mode walks to the finger and to the marker;
- the placed stick sits at its saved spot and any push reaches full speed after a 0.17 s ramp;
- the pause cycles the three;
- a world tap shuts the board;
- the editor saves and resets.

### 39. MENU-LOOK — The menu as the original's plates

**Findings:** menus#6, #7, #8, #9, #11, #12, #13, #14, #36.

**Effort** M. **Depends on** TYPE, HUD-TOP. **Beside** CAPACITY, BOARD-FEEL, GROUND, LOTS.

**Make identical:**
- **The sky and skyline scroll left** (Bullet angle 180, LE 3; the city wraps at 711, LE 5) at 12 and 24 CSS px/s at
  every size (t665, t661).
- **The sky fills 0.75 of the height** with the skyline at its foot and black below (LE 3, GE 10.3, 29.2).
- **No panel** behind the plates.
- **The plates are `menu_btn_wood`:** a wood plank with a grey shadow, labelled in white 16 pt Times
  (`Default_selected` has a green outline; Start is green). The art is only in `Assets/images`: cut it with `cut.py`.
- **160x39 plates at every size,** 36 px apart (32 when the screen is under 590 px high, ME 2.2.1). The column starts at
  `(X_mid, Y_mid - 36)` (ME 3.1.1): at 720p Continue is at 324 and the next plate at 360.
- **Continue only with a save,** one slot above (ME 3.1.4.1.1).
- **Title case.**
- **Menus at 1 CSS px a unit,** using HUD-TOP's menu rule.

**Remake:** `data/ui/menu.lua` (`menu_bg`, `menu_city` at 8/16 drifting right, ph 1, +137, `menu_panel`, BTN_W/H
260x40, `LABEL`, `menu_continue_off`), `widgets.lua`.

**Keep:**
- the red sky and the faster skyline behind the start menu (Stage 22), in the loader's composition;
- the three buttons Continue, Start, Settings and their words (Stage 22);
- the difficulty on its own screen (Stage 26);
- the refusal line;
- MINIOUTBREAK as text until Q7 is answered.

**Scenarios** (settings/phone):
- the plates' rects and pitch at both sizes;
- the scroll left at 12 and 24 px/s;
- no Continue without a save, and one slot up with one.

### 40. MENU-MOTION — The loader, the curtain and the fades

**Findings:** menus#1, #2, #3, #17, #31.

**Effort** M. **Depends on** MENU-LOOK. **Beside** BOARD-FEEL, GROUND, LOTS; not beside DIFFICULTY (`app.lua`).

**Make identical:**
- **The page's loader.** The red sky at 0.75 of the height, the skyline and the logo (Q7). A grey `tiledbg_loader`
  track W-50 by 10 at H-30, filled with white `ninepatch_loader`. When loaded, the sky, skyline and logo slide up at
  H/2 px/s into the menu (ME 1.1-1.3).
- **The menu fades in** from black over 2 s (ME 3.1.1).
- **A run's start or continue.**
  - The plates fade, and the curtain and logo slide down while the screen fades to black over 1 s (ME 49.1).
  - 2 s later the red sky shows a white 12 pt Arial line at 80% height, stepping 0.3 s apart through "Generating
    world", "Placing roads (step 1)", "Placing roads (step 2)", "Adding secret places", "Tilemap drawings",
    "Creating minimap" (ui:16, 18-22; GE 3.6.3, 3.7.3).
  - Then the curtain lifts and the world fades in over 2 s.
- **Screen changes** wait 0.1 s, fade the old plates over 0.2 s, wait 0.2 s and fade the new ones in over 0.1 s.
  Taps are ignored meanwhile (ME 3.2.2.x, 3.2.13).
- **Go to menu** saves, then darkens and slides the curtain down (GE 29.2).

**Remake:** `tools/web_template/index.html` (#loading), `app.lua` (`show_menu`, `defer`/`run_pending`,
`choose_difficulty`, `open/close_settings`, `quit_to_menu`), `data/ui/generating.lua`. A world takes 0.6-0.7 s to
build in the web build at 4x (27.3.8): spread its steps under the lines.

**Keep:**
- START building a fresh world (Stage 25);
- a double tap on START starting nothing (26.2.2): the switching lock does it.

**Scenarios** (settings/play):
- the menu fades in over 2 s;
- a switch fades and ignores taps;
- START plays the curtain and six lines;
- Go to menu saves and plays the curtain;
- the web shell's loader passes the layout checks.

### 41. DIFFICULTY — Four difficulties, chosen, then Start

**Findings:** menus#18, #19, #20, #21, #23.

**Effort** M. **Depends on** MENU-LOOK, SURVIVAL, ZOMBIE-NUMBERS. **Beside** PAUSE, GROUND, LOTS.

**Make identical:**
- **Four wood plates** at `X_mid - 100` from `Menu_hotspot_Y` (Novice 324, Regular 360, Veteran 396, Legend 432 at
  720p; 159-255 at 390), and Back at `hotspot + 4 × pitch`.
- **The description** at `(X_mid + 10, hotspot - 15)`, 281 px wide, white 10 pt Tahoma, outlined (Text t426, ME
  3.2.6). No title and no panel.
- **Legend** (new:178) plays Veteran's numbers (2.11.9) and hides unexplored areas on the map (new:620), here on the
  notebook's page (save an explored mask with the run). The death screen prints "Legend".
- **A tap selects** (`Default_selected`) and shows that difficulty's bullets. A green "Start" plate (new:621) appears at
  `(X_mid + 100, hotspot + 3 × pitch)` and starts the run (ME 3.2.2.7-11).
- **The texts:** new:155 before a choice, then new:617-620.
- **Nothing is highlighted** on opening, and nothing is remembered.

**Remake:** `data/ui/difficulty.lua` (panel, cards with their numbers lines, `diff_*_mark`), `app.lua`
(`start_<name>`), `core/settings.lua`, `config.lua` (`*_infect`, `ui.difficulty_settle`, which goes),
`src/ui/minimap.lua` (the page's mask).

**Keep:**
- the screen of its own after Start (Stage 26), with BACK, Escape and the back key;
- infection on hard mode only (Stage 26): Veteran and Legend;
- the "• Bites can infect" bullet on both.

**Scenarios** (settings):
- the plates' rects at both sizes;
- a tap selects and shows Start;
- Start begins at that difficulty;
- Legend's numbers and its hidden lots;
- nothing highlighted.

### 42. PAUSE — The pause and Options as the original's

**Findings:** menus#25, #28, #29, #30.

**Effort** S. **Depends on** MENU-LOOK. **Beside** DIFFICULTY, GROUND, LOTS.

**Make identical:**
- **Options from the main menu** is a column of plates on the backdrop, with no window or title.
- **The pause** has no dim and no title.
- **The pause's panel** is `options_menu` at 2x: 256x422 at 720p, and gui_scale on a phone (about 160x265). Its plates
  are 2x planks (320x78) in white 22 pt Times (GE 12.9.2).
- **"Go to menu" sits at the top and "Resume" at the bottom,** with the settings between.

**Remake:** `data/ui/settings.lua` (`set_dim`, `set_title`, `set_panel_menu`, `set_title_menu`, `set_resume`,
`set_main_menu`, the sliders).

**Keep:** the window's contents (Stages 17, 18): the touch toggle, the volume, the two dead zones, main menu and
resume. CONTROLS adds "Movement".

**Scenarios** (settings):
- no dim or title;
- the order;
- the panel's size;
- the menu's Options as plates.

### 43. XP — Experience: the Score, and what earns it

**Findings:** events#9; items#38 (the scout book; its cigarettes are MEDS' through survival#30, and its protector
case is SURVIVORS').

**Effort** M. **Depends on** nothing; each later source comes with its own track. **Beside** DIFFICULTY, PAUSE,
AMBIENT, GROUND.

**Make identical.** `expierence_points` (events#9's table):

| source | XP | event |
|---|---|---|
| an infected killed, by a round or a blow | 8 | 13.2.8.4.4.5-.9, 13.3.1.2.7-.8 |
| a deer / a rabbit / a wolf | 10 / 5 / 5 | 13.2.8.1-3 |
| a car trunk searched | 2 | 12.13.3.1.2.1.1 |
| the secret location found | 50 | 22.3 |
| a humvee crash found | 15 | 6.1.3 |
| Nuko Cola drunk | 200 | 8.10.3.55 |
| the Scout book read (10 s) | 200 | 8.10.3.102 |
| a tank / a jumper | 50 / 15 | SPECIALS-A |
| bandits: 15, 50; camps' 25, 50, 100 | | BANDITS-A, BANDITS-B |
| a survivor killed (and -20 karma) | 30 | SURVIVORS |
| a task done | 50 / 100 / 150 | SURVIVORS |
| a helicopter crash found | 25 | ISLANDS |
| reaching island 2 / 3 / 4 / 5 / later | 100 / 150 / 200 / 250 / 300 | ISLANDS, ENDGAME |

- **The start value** is 0 on the first run after the page loads, and 16 after any return to the menu (Restart_game 26
  sets 16; nothing resets it at the start). Copy that.
- **Shown** as "Score" at death (DEATH; events' `S/orig/d1_dead.png`), under the portrait (PORTRAIT) and in the Stats
  tab (PERKS). Saved with the run.

**Remake:** `state.stats` holds `zombies_killed` alone (`src/game/state.lua:30`); `combat.kill`, `container.lua`
(the trunk), `inventory.use` (Nuko Cola, the Scout book).

**Scenarios** (play/save):
- a kill gives 8, a deer 10, a trunk 2, the secret location 50;
- 0 on a first run, 16 after a return to the menu;
- saved and loaded.

### 44. DEATH — The death screen, and death for good

**Findings:** the screen, menus#32 with character#23; menus#33, #34, #35.

**Effort** M. **Depends on** MENU-MOTION, XP. **Beside** AMBIENT, GROUND, LOTS.

**Make identical:**
- **The screen** (Dead_screen 10.3). It destroys the HUD, resets gui_scale to 1 and fades from black over 2 s to the
  red sky at 0.75 of the height with black below. The world is not seen, and there is no dying pose or sound:
  `tutor_death` goes.
- **The texts.** "You are dead" (ui:542) in white 16 pt Times at `Y_mid - 100`. Below it a table in white 16 pt Arial:
  "Score / Minutes alive / infected killed / Bandits killed / Difficulty" (ui:533, 12-15), showing experience points,
  round(time lived / 60), kills, bandits and the difficulty's name (10.3.17). The cause is never shown. The happy
  end's text is new:655.
- **The button.** One "Go to menu" plate 1 s after death, at about 501 at 720p (10.3.11). A tap fades the texts, fades
  the logo in, and 2.5 s later shows the menu (10.4, 10.4.2).
- **Permadeath.** `gamesav_v7 = '0'` on death (6.2.3.1.2.1.1, 6.2.3.1.2.2), so the menu has no Continue.
- **Score** is XP's experience points. "Bandits killed" stays 0 until BANDITS-A counts them. The happy end
  is ENDGAME's.

**Remake:** `data/ui/death.lua` (dim, `bg_rust` panel, "YOU DIED", cause, days, kills, RESTART into a new world),
`app.restart`, `save.lua` (`save.tick` refuses while dead; nothing clears the slot), `player.lua:327-345`,
`sounds.lua` `player.death`.

**Keep:** the pause over the death screen on Escape (menus#39, platform); the map shutting on death (27.4.3).

**Scenarios** (play/save):
- on death the HUD and world go and the sky fades in over 2 s;
- the five rows;
- "Go to menu" after 1 s;
- no Continue afterwards, and the slot cleared.

### 45. AMBIENT — Footsteps by ground, and the beds

**Findings:** character#14, menus#16, survival#22, audio#7.

**Effort** M. **Depends on** AUDIO. **Beside** DEATH, GROUND, LOTS.

**Make identical:**
- **A step every 0.25 s while moving,** at -5 dB at the player (7.8), by ground:

  | ground | sound |
  |---|---|
  | sand | `sand_run` |
  | indoors, a wood floor (warmzone `var#1` = 1) | `wood_1..4` |
  | indoors, concrete (`var#1` = 2) | `concrete_run` |
  | swamp tiles 13/14 | `swamp_step` |
  | grass, tile 6 | `run_1..4` |
  | anything else | `forest_run` |
  | levels 4+ (band 5) | `snow_run`, `road_run` |

- **The menu** loops `ambient` at -10 dB (ME 3.1.1).
- **At 00:00** (17.1.3.1) the night bed starts: `MD_Winter_night_amb` on levels 0-1, `night_loop` on 2 and up. The day
  bed stops. **At 06:00** (17.1.9.1) `ambient` returns (`MD_winter_amb` on levels 3 and up). Both play at 0 dB,
  switched outright. Map levels to the band the player stands in (band N is level N-1).
- **Death stops no bed** (10.3) until Go to menu's StopAll, 2.5 s after the tap (10.4.2).

**Remake:** `player.lua` `footsteps()` (a step every 32 px at 0.3, always `forest_run`), `sounds.lua` footstep (the
other surfaces listed, never played), `ambience.update` (a cross-fade, each bed at most 0.5, faded on death),
`app.lua`'s menu branch (returns before the ambience).

**Scenarios** (play):
- steps every 0.25 s at 0.562: `sand_run` on the beach, `run_N` on grass, wood in a village house;
- the menu plays `ambient` at 0.316;
- at 00:00 the night bed starts in the same frame the day bed stops;
- beds play on after death.

### 46. GROUND — Plain grass and asphalt, and the decals

**Findings:** world#27, #28, #29.

**Effort** M. **Depends on** ART10 (the decals' sheet is a 1.2 one). **Beside** CAPACITY, BOARD-FEEL, BOARD-LOOK, NAMES-TEXT, CONTROLS, MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, LOTS.

**Make identical:**
- **Decals.** Every location has a `ground_enviroment_tilemap` of 30 px cells, 34 by 34 (3.5.5.1.x.9), scattered with
  150-250 decals from a list per kind: 210 on roads and places, 200 in the wild, 250 in villages and at the fire
  station, 150 at the gun-shop location. Rocks, sticks, stumps, bushes, tall grass, reeds, tyres, manholes, blood,
  bodies, litter (`S/ref/ground_env_tiles.png` numbers them).
- **Grass is tile 6 alone** (60,60): no flowers or dark patches.
- **Asphalt is tile 15 alone** (0,180): no crack or slab variants.

**Remake:** `data/ground.lua` (each band's grass and asphalt variants), `render.ground`, `ground_chunk`,
`variant_for`, `src/map/generate.lua`. The decal sheet is packed and drawn nowhere. Lay decals per lot by its kind
until LOTS, seeded, in static batches (25.2.3).

**Keep:** each season's own sheet (Stages 26, 27).

**Scenarios** (world/render):
- a lot's grass all (60,60) and its asphalt (0,180);
- 150-250 decals by kind;
- the same hash natively and in the web build.

### 47. LOTS — Locations of 1020 px

**Findings:** world#6.

**Effort** M. **Depends on** nothing; every world track after it. **Beside** CAPACITY, BOARD-FEEL, BOARD-LOOK, NAMES-TEXT, CONTROLS, MENU-LOOK, MENU-MOTION, DIFFICULTY, PAUSE, DEATH, AMBIENT, GROUND.

**Make identical.** `mapgen` points are 1020 apart (3.6.1: 700 + col × 1020, 320 + row × 1020). A location's ground is
17x17 cells of 60 px (3.5.5), and every offset in 3.16's layouts is 0-1000. `data/lots.lua` has `size = 1200`
("roughly 1080").

**Remake:** `data/lots.lua`, `generate.extent`, every prefab in `data/maps/prefabs/`, `fill.lua`, `places.lua`,
`links.lua`, `loader.lua`, `coast.lua`, `bands.lua`, `minimap.lua` (one glyph a location).

**Keep:**
- 16 rows (Stage 27, *"Increase the height"*);
- five bands of 100 cells each (Stage 27): about 29.4 locations across, the bands off the lot lines as before;
- the quota of places and the roads joining them (Stage 27).

Raise `generate.VERSION`.

**Scenarios** (world):
- 1020 px lots on the original's origin;
- every prefab inside 0-1000;
- every place still reached;
- bands of 100 cells;
- the same hash natively and on the web.

### 48. ROADS — One road: four cells of asphalt

**Findings:** world#9.

**Effort** M. **Depends on** LOTS. **Beside** COAST, REACH-CUES, ATTACH.

**Make identical.**
- **A road** is a location's two middle rows of tile 15 (`SetTileRange(x, y + 8, 17, 2, 15)`, 3.5.5.1.5.7), with arms
  to its neighbours. The autotiler turns the grass either side into edge pieces (tiles 0-5 and 7-12): four cells,
  about 200 px.
- **Streets inside places** are the same tile 15, one or two cells wide (3.5.5.1.18, .20, .23, .24).
- **No road is sand.**

**Remake:** `data/roads.lua` (highway, main, street, dirt, lane), `src/map/roads.lua`, `places.lua` (lanes every 600
px in towns and bases), the coast's and camps' sand tracks.

**Keep:** a road joining every place (Stage 27).

**Scenarios** (world):
- every road four cells of tile 15 with edges;
- streets of 1-2 cells;
- no sand road;
- every place reached.

### 49. COAST — The coasts, the forest wall and the spawn

**Findings:** world#30, #31, #32, #33.

**Effort** M. **Depends on** LOTS. **Beside** ROADS, REACH-CUES, ATTACH.

**Make identical:**
- **The west coast:** solid `tilemap_water` "Left" from -95 to 569, `tilemap_sand` from 389 to 767 under it, grass
  from 767, and the grid from 700. One straight shore the map's full height, about 160-200 px of beach
  (`S/orig/o1_spawn_noon.png`).
- **The east coast:** sand from 16956 to 17814 and solid water from 17235 to 17914 (`o4_edge_e.png`), with no exit
  boat (one world, kept).
- **North and south:** solid `tilemap_forest` of 150 px dense trees, the full width (`o4_edge_n.png`, `o4_edge_s.png`).
- **The spawn** is `SetPos(600, 1500 + random(layoutheight - 5000))` (2.13): at the water's edge, at any height, with
  nothing placed for the player. The farmhouse and the coast roads made for him go.

**Remake:** `data/lots.lua` coast, `src/map/coast.lua` (a whole lot, a ragged walk), `loader.build_bounds`,
`places.lua` spawn (26.3.6, 26.3.11).

**Keep:** the beach in the west with the spawn on it (Stage 26); colder going east (Stage 26).

**Waits on Q4:** the makings.

**Scenarios** (world):
- a straight shore with about 180 px of sand;
- solid sea and forest walls;
- the east sea;
- over 100 seeds the spawn at the water's edge, at a random height, with nothing placed for it.

### 50. ROADSIDE — Poles, cars, the bus and compounds on the roads

**Findings:** world#10, #24, #25.

**Effort** M. **Depends on** ROADS. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical:**
- **A road location** (3.16.1.37 east-west, .38 north-south):
  - telegraph poles (`pillar` frame 0) at 250 and 750 along it, 200 px off the asphalt;
  - half the time one of three: three cars on the carriageway (y 520 ± 50), the bus (`b_car_bus`), or one car and
    four trees either side;
  - 1 in 5 a compound: a garage, a tent, two bins, four blocks and 24 chain-link panels;
  - 1 in 3 a zombie point (frame 10) and three crows (CROWS).
- **`pillar`'s other frames:** 2 and 8 are signs in village 16; 12-15 are lamp posts with `light_tower` at bases and
  checkpoints. All 13 frames are in `Assets/Sprites/pillar`.
- **Parked cars** (class 6, 17 kinds): include the three hatchbacks (`car_hatch_grey/_red/_white`). The bus is placed
  (27.1's found in review).

**Remake:** `fill.lua`, `places.lua`, `data/roads.lua` ("lamp posts … not there" is wrong), the "nothing stands on a
road" rule (27.3.12), which goes for what the original stands on the carriageway.

**Scenarios** (world):
- poles at 250 and 750, 200 px off;
- the seeded halves;
- compounds 1 in 5;
- hatchbacks among the cars;
- the bus placed.

### 51. VILLAGES — Villages as one location of four layouts

**Findings:** world#11, and 27.1.14.

**Effort** L. **Depends on** ROADS, GROUND. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** A village is one location, one of four layouts at fixed offsets (3.12, `choose(9,4,16,17)`):

| layout | event | buildings | other |
|---|---|---|---|
| 9 | 3.16.1.34 | 4 houses (brown, red, green, yellow or the hostel); 6 garages in two rows of three; a shed or the supermarket | 3 cars, a pump, 9 trees |
| 4 | .27 | 5 houses; 2 garages; one of the yard house, piano house, police station, red2 or gun shop | 3 cars, a pump, 9 trees |
| 16 | .40 | 2 houses, the school, 2 sheds | a car, 3 trees, 14 chain-link panels round the yard, 2 benches, 2 bins, 2 signs |
| 17 | .41 | 2 houses, 2 small sheds, the church or (1 in 2) the red-brick house | 3 cars, 8 trees, 3 benches |

- All four have an asphalt cross of 2 cells with 1-cell lanes (3.5.5.1.23, .16, .25, .26), 250 decals and one zombie
  point (frame 0) (`S/orig/o3_v9.png`, `o3_v17.png`).
- The brown garage takes t249's own walls and shadow (27.1.14).

**Remake:** `data/places.lua` town (1-2 lots, the farmhouse first, sand lanes every 600 px), `fill.lua`,
`buildings.lua`, `colliders.lua`.

**Keep:** 12-16 towns (27.3.1's quota), joined by roads; every room (27.1); doors and their button.

**Scenarios** (world):
- each layout at its offsets (table-driven against 3.16);
- streets of tile 15;
- pumps in 9 and 4;
- the brown garage's walls.

### 52. CITIES — Cities as one location

**Findings:** world#12.

**Effort** M. **Depends on** ROADS. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** A city is one location of kind 6 or 21 (3.12):
- **6** (3.16.1.31): five city houses (1, 2 or 4) in two rows at y 248 and 750; one of the piano house, supermarket,
  police station, red2 or gun shop; 2 cars, 4 trees; an asphalt cross with 1-cell lanes.
- **21** (.32): two city house 3s and two of 1, 2 or 4; two garages or none; 4 cars, 3 trees; a 5x5 asphalt square at
  the crossing with a 3x3 island and two trees (`S/orig/o3_c21.png`).

**Remake:** `data/places.lua` city (2x2 lots, avenues, a street every 600 px, 13 kinds, 5 points a lot).

**Keep:** 3 cities (quota).

**Scenarios** (world): a city is kind 6 or 21 at its offsets.

### 53. BASE — The first map's military base

**Findings:** world#13.

**Effort** M. **Depends on** ROADS, ROADSIDE. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** Map 0's bases are kind 22 (3.16.1.29; `S/orig/o3_mil22.png`):
- 4 brick barracks (`b_military_barrack2`) in a row at y 296 and 2 east tents;
- 2 BTRs, 2 trucks, 4 trees, a lamp post;
- `fence_horizontal` along the top with a gap for the road, and down both sides for the top third only;
- an asphalt cross.

No HQ, other barracks or tents, pillbox, hesco or UAZ.

**Remake:** `data/places.lua` base (a full wall with a 300 px gate, the HQ first, both barracks and tents, a pillbox,
2-4 vehicles with the UAZ, sand lanes).

**Keep:** 3 bases (quota). Q6 covers the later kinds.

**Scenarios** (world): kind 22 at its offsets, with the fence's shape.

### 54. SMALL-PLACES — Hospital, fire station, gas station and cafe, paved (27.3.13)

**Findings:** world#14, #15, #17, #18, items#12.

**Effort** L. **Depends on** ROADS, ROADSIDE. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical**, each from its 3.16 layout at the original's offsets:
- **The hospital** (location 11, 3.16.1.35): the building at (726,212). Eight cars in two rows bottom left (x 171-418,
  y 651, 765), two on the west side and one in the middle; 6 trees. Asphalt rects (2,1,7,3), (2,1,3,12), (2,7,12,6),
  with a paved strip to each road (3.5.5.1.24.7; `S/orig/o3_hosp.png`).
- **The fire station** (15, .36): the station at (318,269), houses at (909,336) and (316,747), a small shed at
  (122,722), a car at (710,147), the pump at (657,653), 6 trees. Paved rects (8,2,5,2), (1,8,15,1), (8,8,1,7),
  (1,14,8,1), (10,2,3,7), with arms (3.5.5.1.27.7; `o3_fire.png`).
- **The gas station** (30/31, .42/.43), a road location: the shop at (107,151), the canopy at (335,366), a shed at
  (248,105), two cars, two poles, a paved forecourt (3.5.5.1.7), crows 1 in 3. Its trees fall at absolute
  coordinates, away from the station (`o3_gas30.png`).
- **The cafe** (50/51, .1/.2): the bar at (237,283), two poles, three cars at x 734-898, y 240-340, and one on the road.
  Paved rects (3,5,2,5), (12,4,4,4), (3,8,10,3), and a zombie point (frame 11) (`o3_cafe51.png`).
- **No extra loot outside:** the crate and water bottle at the gas station, the kvas and apple at the cafe.
- **The canopy's west pillar** is solid from -130 (27.1).

**Remake:** the four prefabs; `src/map/roads.lua` (a paved pad in `net` laid before `rasterize`, as 27.3.13 sketched);
`fill.lua`.

**Keep:** 2 hospitals, 2 fire stations, 8-9 gas stations and cafes (quota); furniture on loot points.

**Scenarios** (world):
- each prefab at its offsets;
- the paving in tile 15;
- the car park's cars;
- nothing loose outside.

### 55. CHECKPOINT — Checkpoints on the crossing (27.3.13)

**Findings:** world#16.

**Effort** M. **Depends on** ROADS, ROADSIDE. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** Every crossing location (14) is a checkpoint (3.16.1.39), one of five variants, standing on the
crossing (`S/orig/o3_chk14.png`):

| variant | what stands there |
|---|---|
| 1 | a BTR mid-crossing at (505,535); eight `obst_block` on the carriageways at (514,247), (575,258), (509,861), (570,856), (227,507), (231,576), (828,501), (815,572); 24 chain-link panels round the corners; four east tents; four cars, two on the road; two bins, two benches, a lamp post |
| 2 | variant 1 with plain tents |
| 3 | 20 sandbags, four tents, a truck or BTR, three cars, four trees |
| 4 | the police: two police cars each way on the road, a truck, eight trees |
| 5 | east tents |

The zombie point is frame 3, or 12 for the police.

**Remake:** `data/places.lua` checkpoint (everything in the corners, the crossing clear), `places.lua`, `fill.lua`.

**Keep:** 27.3.11's checkpoint in every world; zombie points off walls (27.3.23).

**Scenarios** (world): each variant's props at their coordinates on the crossing.

### 56. COUNTRYSIDE — Forests, woods and fields

**Findings:** world#19, #21, #23.

**Effort** L. **Depends on** LOTS, GROUND. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** A wild location is `choose(3,1,2,8,10,1,2,8,10,52,52)` (3.12):

| kind | event | what it holds |
|---|---|---|
| 1, 10: forest | 3.16.1.4 | 16 slots on a 4x4 grid 250 apart; each a tree 14 in 15, else a deer stand. The tree is one of ten kinds, 7-10 the `tree_block` clumps (146x166, big obstacles). A red berry bush; 1 in 2 an animal point (frame 5-7) |
| 2, 8: wood | .5 | 16 slots, each a tree 4 in 5; 1 in 9 a pillbox; a red or blue bush |
| 52: field | .3 | 16 slots, each a tree 1 in 2; 1 in 9 a pillbox; a black bush; 200 tall-grass decals |
| 3: pond | | PONDS |

- **Tree kinds** (class 1, 3.5.7.1.1.1-10): `tree_pine`, `tree_leaves`, `tree_leaves_2`, `tree_pine2`,
  `tree_leaves_new2`, `tree_leaves_new3`, and `tree_block` 0-3. Villages and roads draw kinds 1-6.
- **Berry bushes** stand in every forest, wood and field.
- One map counted 66 forests, 61 woods, 35 fields and 23 ponds (`S/orig/o3_forest1.png`, `o3_field52.png`).

**Remake:** `field.lua` (four pines and a point), `lots.lua` filler, `fill.lua`, `wildlife.lua` (28.2.10's points).

**Keep:** the animals (Stage 28), on the forests' points; deer stands (LOOT).

**Scenarios** (world):
- the kinds' shares over seeds;
- a forest's 16 slots;
- clumps placed;
- bushes.

### 57. PONDS — Ponds with fish nests

**Findings:** world#20.

**Effort** M. **Depends on** COUNTRYSIDE. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical.** Location 3 is solid `tilemap_water` in rects (4,4,8,8), (3,5,10,5), (6,3,4,10): about 600 px with
stepped corners (3.5.5.1.15.15). Four `fish_nest`s sit at its edges and yellow berry bushes round it (3.16.1.26),
with 250 decals and page glyph 7 (`S/orig/o3_pond3.png`).

**Remake:** `coast.lua` (the only water), `fill.lua`, `render.coast`.

**Scenarios** (world):
- solid water in the rects;
- the nests;
- the bushes.

### 58. MAP-PAGE — The map's page: its window, glyphs and marks

**Findings:** world#36, #37, #38, items#33.

**Effort** M. **Depends on** COUNTRYSIDE, PONDS. **Beside** LIGHTS, REACH-CUES, ATTACH.

**Make identical:**
- **The window.** LOCAL MAP shows 16 lots each way round the player (settled).
- **Glyphs** (3.17.1):

  | location | frame |
  |---|---|
  | forests and woods | 0 |
  | fields | 29 |
  | ponds | 7 |
  | the hunters' camp | 14 |
  | other secret kinds | 0 |
  | cities | 6 |
  | the gun-shop location | 26 |

  Frame 31 is never used.
- **Marks** (`Pad_load_page` 12.6.4.1.3.1.1.1-13; `minimap_hint` at (x-700)/51 and (y-320)/51; its art is missing, so
  cut it):
  - the secret location's red "?" (frame 7), shown from the start;
  - a found humvee crash (12) and heli crash (11);
  - a deployed tent (2) and a stash (13).
- **Map Notes** open the notebook at its map (8.10.3.100; correction 15).

**Remake:** `minimap.lua` (`page_lots` 12, `draw_world`; woods alternate 0 and 14, fields 7, camps 14, cities 31 and
6), `data/minimap_symbols.lua`, `data/places.lua` `mark`, `data/ui/map.lua`.

**Keep:** the notebook from the notepad and M, with the world going on under it (Stages 27, 28).

**Scenarios** (hud):
- 16 lots each way;
- the frames by kind;
- the "?" from the start;
- a found crash marked;
- Map Notes open the map.

### 59. LIGHTS — Lights cut out of the dark

**Findings:** survival#20, #11.

**Effort** L. **Depends on** NIGHT. **Beside** ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, REACH-CUES.

**Make identical.**
- **The night layer has its own texture,** and lights are sprites with blend 8 (destination out) that cut holes in it:
  - a lit fire's 250 px `light_sprite` (8.10.3.24.2.1);
  - a flare's (LIGHT-ITEMS) and the headlamp's;
  - the lamp posts' `light_tower` cones (3.5.7.1.4.2-4);
  - every night shot's `muzzle_flash` (8.6.2.x; SHOT spawns it);
  - explosions.
- **A lit fire** also shows `light_color`, a 150 px red disc up to 49/255, AdjustHSL 10, on Color_effect, by day too.
  Both lights wait 170 s and fade over 10 (`S/orig/fire_night.png`).
- **There is no light round the player.**

**Remake:** `daynight.draw_world_overlay` (one multiply rectangle), `campfire.draw_glow` (an additive orange disc,
only in the dark), `app.lua`'s draw order. Draw the veil into a canvas and erase the lights from it. Measure what the
canvas costs a phone in the web build at 4x, at night in a town.

**Scenarios** (render):
- at 02:00 the ground within 125 px of a fire shows its noon colours;
- the red disc by day;
- a night shot's hole for 0.2 s;
- the perf budget.

### 60. REACH-CUES — Outlines and names in reach

**Findings:** hud#26, #27; the outline, character#21 with world#34.

**Effort** M. **Depends on** nothing. **Beside** ROADS, COAST, ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, LIGHTS, ATTACH.

**Make identical:**
- **The nearest pickable within 100 px** with line of sight wears its name (Text t513, 10 pt Arial, outlined black;
  12.13.3.5.1.1-.2): rgb(200,255,200) for items, white for weapons, refreshed every 0.5 s.
- **A car's trunk** is outlined white within 50 px (12.13.3.5.1.3.3). Pumps, berry bushes, plants and panel buttons
  in reach get the Outline effect.
- **Behind a facade** (`var#42`), the player gets a white 1 px outline (20.2.6, 20.2.7;
  `S/orig/o5_house_behind_crop.png`).

**Remake:** `status.lua`, `render.lua`, `container.lua`, `interior.lua`. Add one outline shader.

**Keep:** no pick-up icon over the player (Stage 22). The name is at the item, not over the player.

**Scenarios** (render):
- the nearest item's name in its colour;
- a trunk outlined within 50 px;
- the player outlined behind a house.

### 61. DRAW-ORDER — Layers as the original's

**Findings:** character#24 with zombies#29.

**Effort** M. **Depends on** CHAR-LOOK, REACH-CUES. **Beside** ATTACH, FELLING, CROWS, VARIANTS.

**Make identical.** The original draws by layer, not by y:

| layer | holds |
|---|---|
| items_on_ground [18] | corpses, moved to the bottom, under dropped items |
| Buildings_shadows [9] | a zombie behind a building (13.3.1.8) |
| Zeds [29] | zombies |
| [32-39] | the player |
| [40] | blood |
| [41] | facades, fading by y (20.2, 20.3) |
| [43] | trees: a canopy always covers the player |
| [44] | hp bars |
| [46] | night |
| [48] | Sunset_Dawn |

**Remake:** `render.lua` (`by_ground_y`, `render.world`, `draw_order`). Progress' ground rule "Draw order sorts by
ground-y" changes with it.

**Keep:** a facade from the street, a room from inside, a facade faded from behind (28.1); the dark outside a room.

**Scenarios** (render):
- a zombie 10 px south of the player drawn under him;
- a corpse under a dropped item;
- a tree over the player wherever he stands.

### 62. ATTACH — Attachments as the original's

**Findings:**
- the effects, combat#8;
- combat#27, items#40, #41.

**Effort** M. **Depends on** DISPERSION, MISSING-ITEMS. **Beside** ROADS, COAST, ROADSIDE, VILLAGES, CITIES, BASE, SMALL-PLACES, CHECKPOINT, COUNTRYSIDE, PONDS, MAP-PAGE, LIGHTS, REACH-CUES.

**Make identical:**
- **Scopes** (by `var#17`, 12.30.1.30-36): 1 lowers `dispersion_default` by 1, 2-4 by 2, 5 by 3. A scope adds
  +5/+15/+25 to the headshot roll: `round(random(100)) < HS_power + var#9` (13.2.8.4.1.2.5.1).
- **Grips** (`var#20`): 3 and 4 lower the dispersion max by 10; 5 and 6 multiply the cooldown step by 1.3.
- **A silencer** makes no noise (13.6) and plays the silenced sample.
- **Belts** shorten each gun's reload to its own time (combat#5's brackets): Mosin 4 → 2.8, RPK 3 → 2.25, L85 3 → 2,
  and so on. Read the choke's branch; the remake's `choke_bor` ×0.6 has no original.
- **Which gun takes which** follows items#40's table (8.10.3.64-108). For example, the AUG takes no scope, the AKM no
  silencer, and the MP5k the ACOG. A gun with no branch says "It doesn't fit there."
- **The Bandolier and the Magpull are two belt attachments.** The Bandolier (83) fits the IZH-43, Silenced Remington,
  Benelli, Repeater, Mosin and SKS; the Magpull (89) fits the box-magazine rifles (items#41).
- **Attaching and detaching** play `reload_pistol` at -5 dB (items#46).

**Remake:** `data/attachments.lua` (by family, multipliers), `src/game/attachments.lua`, `combat.headshot`,
`combat.spread`.

**Keep:** *"not all attachments go on every gun"* (Stage 23); drag on and tap off (23.6); the four mounts.

**Scenarios** (combat):
- each gun's accepted set (table-driven);
- a PSO's -2 and +15;
- a silenced shot is silent;
- the Magpull on an AK and the Bandolier on a Mosin;
- the attach sound.

### 63. FELLING — Trees that fall to an axe

**Findings:** world#22 with audio#13.

**Effort** M. **Depends on** MELEE, COUNTRYSIDE. **Beside** DRAW-ORDER, CROWS, VARIANTS.

**Make identical.**
- **Which blows chop.** A melee bullet with `var#7` (the split axe's and fire axe's swings, 8.7.1.5.1.2.2.2.1, .5; and
  the interact button's chop, 12.13.3.1.8) hitting a tree's `obst_base` takes one of its 3 points (16.1.6), with
  `chop`..`chop4` at 0 dB at the blow.
- **At 0 the tree falls.** `treefall_02/03` plays at -5 dB, flat, and frame 1 (the 78x32 stump) shows. It drops two
  wood sticks (38) and a wood pile (50), and item 37 as well from `tree_leaves_2`. Its obstacle becomes frame 8.

**Remake:** trees are scenery (`default_1` unused), `combat.swing`, `colliders.lua`, `sounds.lua`. A felled tree is
saved.

**Keep:** the campfire's ingredients (Stage 24): felling is one more source.

**Scenarios** (combat):
- three axe blows fell a pine with chops and a treefall, leaving a stump, 2 sticks and a pile;
- fists don't chop;
- the felling is saved.

### 64. CROWS — Crows on the roads, and their caws

**Findings:** world#26 with audio#12.

**Effort** M. **Depends on** ROADSIDE. **Beside** DRAW-ORDER, FELLING, VARIANTS.

**Make identical.**
- **Where:** `crow_skin`, three at a time, 1 in 3 on road, gas station, cafe and hospital locations (3.5.5.1.x.8), and
  14 and 5 at two secret locations.
- **Taking off:** every 4 s, any crow within 300 px of the player takes off (13.5.1.2.1.1), and so does one hearing a
  noise (13.6.1.10.1).
- **Flight:** it flies off and fades, and caws after random(0-2) s (`crow_1`..`crow_5` at 0 dB at the crow).
- **Art:** missing; cut it from 1.0.

**Remake:** none. `wildlife.lua`, `data/animals.lua`, `fill.lua`.

**Scenarios** (world):
- crows on a third of road locations;
- they take off within 300 px and caw.

### 65. VARIANTS — Colours at spawn

**Findings:** inventory#B8.

**Effort** M. **Depends on** nothing. **Beside** DRAW-ORDER, FELLING, CROWS, WATER.

**Make identical.** 13 garments and bags take a random hue at spawn (`var#25`):

| item | hue choices |
|---|---|
| jeans | 0, 50, 75 |
| tshirt | 0, 35, 80 |
| mountain backpack | 8, 55, 0, 20 |
| taloon | 18, 50, 0, 65 |
| workpants, sportpants, sport_jacket, shirt_green, down_jacket, dress, hunter jacket and pants, school backpack | their own `choose()` in 14.2 |

The Colors effect applies the hue in the world, on the cards, the portrait and the info card (14.2.x, 12.11.2.3.1).
Read the start kit's hue in 2.13.1.1.

**Remake:** one colour per item. `item.lua` spawn, `render.lua` (worn layers), `widgets.lua` (icons), `save.lua` (the
hue with the item).

**Scenarios** (items):
- jeans take 0/50/75 over seeds;
- drawn hued in the world and on the board;
- saved.

### 66. WATER — Bottles that hold water; pumps and bushes

**Findings:** water containers, inventory#G2 with items#29; items#18.

**Effort** M. **Depends on** FOOD, LOOT. **Beside** VARIANTS, CRAFT.

**Make identical:**
- **The water bottle and canteen hold water:** 100 is four sips.
  - A sip takes 25: +12.5 from the bottle, +25 from the canteen, 2 s, the `Whiskey` sound at -10 dB.
  - An empty one is kept ("Bottle empty."; LINES, Q3).
  - "Fill with water" at a pump (8.10.3.47-48, 8.10.5.5-6; `md_canteen_fill`).
- **Berry bushes.** A bush at frame 1 can be searched (12.13.3.1.3): 3 s crouched, `bushsearch`, the bush to frame 0,
  and one berry of its colour (red 27, yellow 73, blue 74, black 114).
- **The respawn** (14.1.2.1-2): each bush 500 px or more away gets frame 0 or 1, and each pump 500 px or more away gets
  `choose(0,25,0,25,50,75,100,150,200)` water.

**Remake:** `data/items.lua` (single use +45/+50, stacks of 2 and 1), `inventory.use`; bushes and pumps are scenery.

**Scenarios** (items):
- a full bottle gives four sips of 12.5;
- an empty one is kept;
- filling at a pump with water lowers its water;
- a bush gives one berry and regrows at a respawn.

### 67. CRAFT — The original's recipes

**Findings:** recipes, inventory#H1 with items#43; inventory#H2, #H3.

**Effort** L. **Depends on** BOARD-FEEL. **Beside** VARIANTS, WATER, SLEEP.

**Make identical.** Check_craft (12.14.1.4) and Call_sub_menu_craft (12.10.10, 12.10.17):

| ingredients | result |
|---|---|
| flare + whiskey | molotov |
| battery + radio | loads it |
| ashwood stick + rope | bow (5 s) or fishing rod (4 s) |
| wood sticks + a knife | arrows (5 s) |
| 2 burlap sacks | canvas (5 s) |
| canvas + wood piles | a placed stash (its map mark is 13) |
| burlap sack + rope | improvised bag (5 s) |
| wood piles + rags, or + 3 wood sticks | campfire kit, or "Build fence" |
| 3 rags | rope |
| wood sticks onto an improvised bag | improvised backpack (5 s) |
| wood sticks | split |
| shirts, sweater, raincoat, one helmet | torn into rags |

- **The original's fireplace-kit recipe from papers and wood piles is not added:** the user's two replace it.
- **Results.** The campfire kit and rope go into the inventory, or at the feet when it is full. The bag, backpack,
  canvas and bow go at the feet (12.10.17.26.3).
- **Sounds.** Crafts play nothing; patching plays `taping` and tearing `tear_fabric`.

**Remake:** `data/recipes.lua`, `crafting.lua` (refused with "My inventory is full" when there is no room),
`board.lua` `craft_plan`, `sounds.lua` `craft`. Progress' "out of scope: the crafting system as built" asks for a
plain recipe table; `recipes.lua` is that.

**Keep:** the user's two recipes and their times (Stage 24); MAKE on the tooltip.

**Scenarios** (items/board):
- each recipe with its time and where its result lands;
- no crafting sound.

### 68. MENDING — Patch, sharpen, saw off

**Findings:** items#35 (gasoline and the toolbox come with VEHICLES-B).

**Effort** M. **Depends on** CRAFT. **Beside** SLEEP, RAIN.

**Make identical:**
- **Patching.** A sewing kit, duct tape or epoxy onto a worn garment, helmet, vest or backpack patches it: 3 s, with
  `taping` (12.10.17). Read the amount.
- **Sharpening.** A sharpening stone onto the melee weapon adds 25%: 5 s, "Weapon ruined." at 0 (8.10.3.97).
- **Sawing off.** A hacksaw onto a shotgun or rifle saws it off (8.10.3.70).

**Remake:** inert in `items.lua`; `recipes.lua`, `crafting.lua`.

**Scenarios** (items): each of the three.

### 69. KNIVES — Knives as melee weapons

**Findings:** items#39, character#25 (the knife half; the flare's is LIGHT-ITEMS).

**Effort** M. **Depends on** MELEE, CRAFT. **Beside** SLEEP, RAIN.

**Make identical.**
- **A knife dropped on the melee card is taken as melee:** Knife-melee-1/2/3 deal 20/19/21 (12.10.17.41-43), with
  "Holster knife" to put it back.
- **Crits** are MELEE's table (knife 9 10%, 8 15%, 7 20%).
- **Condition:** knives carry it (LOOT).
- **Art:** `knife_any` is drawn in hand; cut it from 1.0.

**Remake:** `items.lua` (the three knives only flay, 28.2), `weapons.lua`.

**Keep:** flaying (28.2).

**Scenarios** (combat): a knife on the card swings for its damage and crit, drawn in hand.

### 70. SLEEP — Sleeping

**Findings:** sleep, hud#13 with survival#23.

**Effort** L. **Depends on** HEAT, CLOCK, FIRE. **Beside** CRAFT, MENDING, KNIVES.

**Make identical:**
- **The button.** `gui_sleep` (100x66) at the right edge, top+185, from 00:00 to 06:00 (17.2.5, 17.2.7), and when a run
  starts before 06:00. It opens the pad (12.1.1).
- **The pad's texts:** "You can sleep in the night time. If you in good condition you can sleep until morning 06:00.
  You will wake up earlier if hunger or thirst will be too high.", "Current resources allows you to sleep until
  HH:MM", and "Go to sleep".
- **Refusals** (12.1.3):
  - by day: "I can't sleep in daytime.";
  - outside a room: "I need a roof at least for sleeping.";
  - with the temperature below 0: "It's too cold here to sleep.";
  - with food or water at 10 or less: "Currently you too hungry or thirsty to go to sleep.";
  - when sick or bleeding.
- **Sleeping.**
  - Fades to black over 1 s.
  - Applies every tick of the skipped time at once (12.1.4.2).
  - Runs to 06:00, or until food or water would reach 10 (12.1.5).
  - Burns fires down (12.1.4.4) and removes the dusk tint.
  - Runs at timescale 0.33 with the sound at -15 dB for 1 s.
- **Measured:** 00:02 → 03:26, food and water 71 → 9, heat 77.5 → 100 (`S/orig/sleep_*.png`).

**Remake:** none. `hud.lua`, `daynight.set_time`, `survival`, `campfire`.

**Scenarios** (hud/play):
- the button's hours;
- each refusal;
- a sleep applies its ticks;
- an early wake;
- fires burned down.

### 71. RAIN — Rain

**Findings:** survival#12.

**Effort** L. **Depends on** HEAT, NIGHT. **Beside** MENDING, KNIVES, MAP-TABS.

**Make identical.**
- **When.** The Rain timer fires every 360 s (2.11). Rain stops if it was raining; otherwise it starts 1 time in 4
  (17.3.1).
- **While it rains** (17.3.2.1.1):
  - four `rain_particle`s every 0.4 s in screen space, over the night: scale 1-2, 700-1500 px/s at 80-110°;
  - a `rain_ground` splash within 300 px;
  - `rain_fade`, rgba(81,112,173) at 50/255, over the whole view, HUD included, fading in and out over 10 s.
- **Sound:** `rain_loop` at -5 dB outdoors and `rainroof` under a roof, swapped as the player goes in and out.
- **Cold:** -Rain_cold outside a room unless a raincoat is worn (HEAT's term).
- `S/orig/rain_3s.png`, `rain_13s.png`, `rain_night.png`.

**Remake:** none (Progress 12: "rain_loop still waits on weather"). The art and sounds are in Assets. Save the rain
with the run.

**Scenarios** (play/render):
- rain one period in four (seeded);
- particles, splashes and tint;
- the roof swap;
- -1 a tick on Regular outdoors, none in a raincoat.

### 72. SNOW — Snow in the cold bands

**Findings:** survival#13 with events#6.

**Effort** M. **Depends on** RAIN. **Beside** MAP-TABS, THROWABLES.

**Make identical.**
- **Where:** `rains_level` 2, from `CurrentLevel` 3 (3.8.4-3.8.6.1): here bands 4 and 5, the dry and the snow
  sheets. The frosted band (island 3) has rain. Nothing changes with time alive (corrections 14, 20).
- **What** (17.3.2.2.1):
  - two particles every 0.4 s, frame 1, scale 1-3, 50-150 px/s;
  - `snow_ground` settling within 300 px;
  - `rain_fade` frame 1, white at 50/255.
- **Sound:** `blizzard`, and no roof sound.
- **Cold:** the same Rain_cold.
- `S/orig/snow_3s.png`, `snow_15s.png`.

**Scenarios** (render): bands 4 and 5 snow instead of rain.

### 73. MAP-TABS — The notebook's Guides, Crafting, Tasks and GLOBAL MAP (27.4.9)

**Findings:** world#39.

**Effort** L. **Depends on** MAP-PAGE, CRAFT, ART10 (`pad_controls` is a 1.2 sheet). **Beside** RAIN, SNOW, THROWABLES.

**Make identical.** Draw_pad always lays four tabs and an arrow to GLOBAL MAP (`l_eng_ui.xml`;
`S/orig/o4_mappage.png`):
- **Map:** the page.
- **Guides:** control guides with the `pad_controls` pictures.
- **Crafting:** the recipes, the user's two among them.
- **Tasks:** the survivors' tasks (12.6.4.1.4); SURVIVORS fills the page.
- **GLOBAL MAP:** the whole world with its five seasons (settled).

**Remake:** `data/ui/map.lua` (the Map tab only), `minimap.lua`.

**Scenarios** (hud): the four tabs open their pages, and GLOBAL MAP shows the whole world.

### 74. THROWABLES — Grenades, smoke and the molotov

**Findings:** items#36; also survival#36's burning room and zombies#8's smoke.

**Effort** L. **Depends on** CRAFT (the molotov's recipe). **Beside** SNOW, MAP-TABS, LIGHT-ITEMS.

**Make identical:**
- **Throwables:** the F1 (8.10.3.21), the smoke grenade (.104; unlock-gated, so F1 on a fresh profile) and the molotov
  (.89).
- **Explosions** need area damage, which the remake lacks ("There is no area damage in the game"), with
  `f1_explode`, `gl_explode`, `smokegrenade` and `flare_throw`.
- **A burning room.** A molotov sets the room burning (warmzone `var#4`, 8.9.2.4.3): -5 hp every 1.5 s with the grey
  flash (6.2.17).
- **Smoke** blocks the infected's sight.

**Scenarios** (combat):
- an F1's fuse and blast;
- a room burning;
- smoke hiding the player.

### 75. DEPLOY — The tent, traps, wire and mines

**Findings:** the tent, inventory#B10 with items#37; character#15; items#36's deployables; the tent's
horde, events#8.

**Effort** L. **Depends on** THROWABLES (explosions), POPULATION (the waves' points). **Beside** LIGHT-ITEMS, GARDEN-FISH.

**Make identical:**
- **The civilian tent** is worn in the backpack slot (0 cells), deployed and rolled (12.10.9.1, 12.10.15-16). Its room
  warms by 4 (HEAT).
- **A landmine, a claymore and barbed wire** go down through build mode (8.10.3.49, .86, .58).
- **A bear trap** is set in 3 s (.50; `trap_bear_0`).
- **Barbed wire slows** the player to 55 px/s for 1 s (`var#6 = 45`, 20.6.2), and the infected to a quarter for 2 s.
  The first map lays no swamp tiles, so swamp slowing does not arise.
- **A tent draws a horde** (17.5, 17.8). Every 3500 s, with a tent deployed, `Spawn_attack_wave` makes a
  frame-15 zombie point 1550 px from a random tent, toward the map's middle (8899, 6788 in the original), at
  the first of five angles (45, 22, 0, -22, -45° off that line) not in water, and its group walks in.
  Standing within 300 px of a tent makes a 1500 px noise every 5 s (17.6). ENDGAME's final day uses the same
  waves; `hordes_spawns_on` is never read.

**Remake:** `build.lua` (the campfire only), `items.lua`.

**Scenarios** (items/combat):
- a tent deployed and rolled;
- wire slows;
- a trap catches;
- a mine explodes;
- a deployed tent draws a wave at 3500 s, and standing by it makes noise.

### 76. LAUNCHERS — The underbarrel launchers

**Findings:** items#42.

**Effort** M. **Depends on** THROWABLES, ATTACH. **Beside** LIGHT-ITEMS, GARDEN-FISH.

**Make identical.**
- **Which guns:** the M203 fits the AUG, L85, FN-CAL, M4 and M16A2, and the GP-25 the AK-74, AN-94 and AKM, on the grip
  mount (8.10.3.94, .95).
- **What they fire:** 40mm and VOG (8.6.1.2), with `gui_btn_attack_2` and `Mag_reload` type 4.
- **The Laser Sight** is unlock-gated: an RDS on a fresh profile.

**Remake:** `data/attachments.lua` `launchers`.

**Scenarios** (combat): an M203 on an M4 fires a 40mm round that explodes.

### 77. LIGHT-ITEMS — Headlamp, NVG, flares, batteries, radio

**Findings:** items#32 (and character#25's lit flare).

**Effort** L. **Depends on** LIGHTS. **Beside** THROWABLES, DEPLOY, LAUNCHERS, GARDEN-FISH.

**Make identical:**
- **A headlamp or NVG, worn and switched on** ("Toggle torch", 12.10.17.39), drains 0.1 charge each 0.5 s (8.14, 8.15)
  and goes out at 0. The headlamp cuts a light in the night. The NVG lays `light_nvg_color_tiled` over the view and
  halves the night's opacity (12.10.17.40, 17.7.3.1).
- **Batteries** recharge them ("Attach battery", 8.10.3.22).
- **A flare** is held lit for 100 s, then thrown with `flare_throw`, and burns where it lands (8.10.5.4, 8.10.3.20).
  Cut `flare_hand`.
- **The flare gun** fires a flare that calls an airdrop (8.10.3.90; AIRDROP).
- **The radio** scans 5000 px for military crashes with batteries (8.18); ISLANDS marks what it finds.
- **Spawn charges:** batteries 10-100, a headlamp 0/15/30, the radio 50 (LOOT).

**Remake:** the headlamp and NVG are worn hats with no light (`items.lua` 593, 597); the rest is inert.

**Scenarios** (render):
- a lit headlamp's hole drains;
- the NVG halves the dark;
- a thrown flare burns 100 s.

### 78. AIRDROP — The flare gun's airdrop

**Findings:** events#7.

**Effort** M. **Depends on** LIGHT-ITEMS (the flare's light), CAMPS-TRUNKS (the lootbox). **Beside** GARDEN-FISH,
SPECIALS-A, PORTRAIT.

**Make identical.** The flare gun is item 103: "Flare gun. After shot it calls airdrop. Airdrop will be delivered in
20 seconds." (new 103).
- **Where it works:** outdoors and on foot (8.10.3.90). Indoors or in a bunker it says "I can't do it here"; in a car
  it is refused.
- **The shot** fires a flare up with a 500 px light fading over 15 s, leaves an `airdrop_landing` at the player, and
  makes a 1500 px noise every 2 s.
- **The drop.** After 15 s a crate (`airdrop_drop`) comes down by parachute on that spot, or into the middle of the
  room if the spot is in a warm zone (12.4.10). It becomes `bunker_lootbox` kind 53 with an `airdrop_parachute`
  (12.4.3), opened on walking into it (3.1.10), holding items#19's list for kind 53. Here it is a crate container,
  as CAMPS-TRUNKS makes the camps' boxes.
- **Live:** the box lay 24 px from the player 16 s after the shot (events' `S/orig/p5_flare.png`,
  `p6_airdrop_fall.png`).
- **Not to copy:** the `gui_airdrop_icon` picker, `Airdrop_recharge` and `airdrop_box` are dead code (12.4.1, .5-.9).

**Remake:** the flare gun is inert and unplaced (`data/items.lua`). New `src/game/airdrop.lua`; `data/containers.lua`.

**Scenarios** (items/world):
- the shot's light and its noise every 2 s;
- the crate 15 s later on the spot, or in the room's middle;
- kind 53's contents;
- refused indoors.

### 79. GARDEN-FISH — Planting and fishing

**Findings:** items#34; the sea's fish, events#20.

**Effort** L. **Depends on** PONDS, WATER. **Beside** DEPLOY, LAUNCHERS, LIGHT-ITEMS.

**Make identical:**
- **Planting.** Seed packs (8.10.3.41-43), and a tomato's, pepper's or zucchini's second action (8.10.5.7-9), plant on
  grass with a digging tool in hand: 5 s, `planting_seeds`, then a plant that grows (8.16).
- **Fertilizer** regrows a plant or a berry bush (.107.2).
- **Fishing.** The two rods fish while overlapping a nest (.73, .74): 4 s, taking it. One time in five
  "Damn, slipped away."; otherwise "Gotcha!" with a herring or salmon from the sea, or a ruffe or perch
  from a pond. With no nest: "I don't see any fish nearby.".
- **The sea's nests** (8.20, 8.21). Every 20 s, while the player is within 1000 px of the west edge, a
  `fish_nest` is made in the sea beside him, replacing the last; the same at the east edge (COAST). Within
  400 px a `fish_sprite` ripples.

**Scenarios** (items):
- a seed grows;
- a rod at a nest gives a fish, or slips one time in five;
- a nest appears in the sea beside a player on the beach.

### 80. SPECIALS-A — Screamer, buried, tank, jumper

**Findings:** zombies#4 (the first half); the Timeline, events#5.

**Effort** L. **Depends on** ZOMBIE-AI, POPULATION. **Beside** LIGHT-ITEMS, GARDEN-FISH, PORTRAIT.

**Make identical:**
- **Buried** (kind 2): `zed_buried_hidden` lies in the ground and jumps out on any noise, with `deerrun` (13.1.1.2,
  13.3.3, 13.6.1.11).
- **Screamer** (6): 50 hp, sight 400 px over 180°. It screams `attack_8` every 2.5 s and calls `noice(player, 1700)`
  every 0.5 s (13.3.7; one screamer at a time).
- **Tank** (12): 800 hp, bites for `Zed_dmg × 2`, with `zed_tank_1-3` and `zed_tank_scream` (13.3.8, 13.2.6). Its sheet
  is a 1.2 one (ART10).
- **Jumper** (16): 150 hp, with `jumper_spot` and `jumper_jump` (13.3.9).
- **Where:** POPULATION's tables (frame 0's 1 in 6, frame 1's screamer). The `Zombies_active_*` switches are 1 in 1.0.
  The tank, the jumper and the rest are on from a run's first second (Restart_game 26; live at 11 s). Only
  the buried wait: `Zombies_active_sleeper2` comes on at 1500 s alive, or on any later island (23.6;
  correction 14).

**Scenarios** (combat/world): each kind's hp, senses, sounds and place.

### 81. SPECIALS-B — Spitters, shooters, skins 7-11, acid

**Findings:** zombies#4 (the second half), survival#36.

**Effort** L. **Depends on** SPECIALS-A. **Beside** PORTRAIT, PERF-27.

**Make identical:**
- **Shooters and spitters** (4, 10, 11, 18): 100-175 hp, firing `zed_bullet` (13.3.6, 13.2.9). Their rounds make the
  player sick (15.2.1.1).
- **Skin 7:** 80 hp, a bio splash on death.
- **Skin 8:** 120 hp, runner speed, a bio explosion and splashes on death, and a trail every 0.5 s.
- **Skin 9:** 200 hp, always drops item 268.
- **Skins 10 and 11** ("fresh"): 110-120 px/s, dodging sideways at 300 px/s (13.3.2.6), and their bites always bleed
  (15.3.5.3).
- **Acid.** Standing in a `zed_bio_splash` costs 5 hp a second with the grey flash and a 50% chance a second to fall
  sick; the death reason is "Acid" (6.9).

**Scenarios** (combat):
- each kind;
- acid's damage, sickness and death reason.

### 82. PORTRAIT — The portrait with its gear and XP

**Findings:** hud#8; IDDQD, events#24.

**Effort** M. **Depends on** ART10, VARIANTS, HUD-TOP, XP. **Beside** SPECIALS-A, SPECIALS-B, PERF-27.

**Make identical:**
- **The frame:** `gui_portrait_bg` (78x106) at -2,0, holding `gui_btn_perks` (the face, 58x64 at 8,6).
- **The gear:** the helmet, vest, outerwear and backpack portraits layered over it (66x66).
- **The XP:** Text t550 in white 10 pt at 3,77 (updated 22.2).
- **`gui_btn_perks_notify`** flashes when a perk is affordable (PERKS).
- **IDDQD**, typed on a keyboard (event 33): the player says "I feel like immortal, but im not." and the
  portrait's helmet changes (frame 2 for characters 7 and 12, 3 for 20, 1 otherwise). Nothing else.

**Remake:** none in `hud.lua`. Progress 6 has the 96 portraits.

**Scenarios** (hud):
- the portrait shows what is worn, in its hue;
- the XP number;
- IDDQD's line and helmet.

### 83. FRIEND — The Bot Manager and the friend

**Findings:** events#2.

**Effort** L. **Depends on** ZOMBIE-AI (senses), HITFX, XP. **Beside** PORTRAIT, PERF-27.

**Make identical.**
- **The Bot Manager** (13.4.1.1) ticks every 60 s in every normal run, since `SEED_pubg` is 1 (correction 16), whatever
  the spawn switches say. It places bots on the map's `bot_respawn_point`s (18 on a 16x16 map; read where 3.5 puts
  them and scale them to the world). This track builds the human NPC the bandits and survivors also use: its body
  and frames (cut from 1.0; `missing.txt`), its hp bar, its sight and fight.
- **The friend** (`Check_bot_friend`, 13.4.1.9, .15): `bot_friend` on a point 2000-5000 px away when there is none,
  removed past 4000 px. From 600 s (Timeline 23.5) it is also looked for every 500 - 3 × karma s (13.4.1.4).
- **What it does** ("Veteran friend", 13.4.7): follows and fights; is told to follow or guard with `gui_friend_btn`
  (13.4.7.2.21, .22); rides in a car (VEHICLES-B); wears an hp bar and a mark on the map's page.
- **Live:** a friend 39 s into a run left alone (events' `S/orig/probes.txt`, bots3).
- Meeting one counts for "BRO" (ACHIEVEMENTS).

**Remake:** no human NPC. New `src/game/systems/bots.lua` and `data/bots.lua`; `data/ui/hud.lua` (the friend
button); `targeting.lua` (the friend is no target); `population.lua`.

**Keep:** the infected's and animals' rules; the target button shows only for enemies (Stage 20).

**Scenarios** (world/combat):
- a friend appears 2000-5000 px off within a minute and is gone past 4000;
- it follows, and fights an infected;
- follow and guard;
- saved.

### 84. SURVIVORS — Survivor camps, their tasks and gifts, and karma

**Findings:** events#3, #4.

**Effort** L. **Depends on** FRIEND (the bots), MAP-TABS (the Tasks tab), XP. **Beside** PERF-27, ISLANDS.

**Make identical.**
- **Camps.** `Bot_team_check_situation` runs on the 60 s tick and every 300 s (13.4.1.19, .20). It flags the nearest
  wild location for a camp of three `bot_enemy_teammate` (events' `S/orig/c1_camp.png`).
- **Tasks.** The talk button (`gui_btn_talk`, 12.13.3.4) opens `Quest_menu` (12.2.1), one task at a time:
  - bring an item from a list ("Hello there! Our group is looking for ...");
  - wipe out a bandit camp (with BANDITS-B; until then that kind is not offered);
  - bring back a lost loot case, which may be opened and robbed instead (the Protector case, Questbox_crack
    8.10.3.75; items#38).
- **Rewards** are named up front, from 42 (12.2.1.1.1.1.5): food, ammunition, guns, medicine, or coordinates of a
  drivable vehicle, the bunker or the boat (those three come with VEHICLES-A and Q6).
- **The rest of a task:** a 500 s timer and an entry in the Tasks tab (12.6.4.1.4); 50, 100 or 150 XP and +10, +10 or
  +20 karma when done (12.2.2.3-.5); cancelling -10 karma, stealing -20 (12.2.10, .11). Accepting saves the run.
- **Gifts.** At top karma a camp gives one of 20 items instead (`Gift_menu`, 12.2.3), with its text and its mark on
  the map's page (6.16).
- **Karma** (events#4): `karma_points` from -111 to 89, shown +11, so -100 to +100 (6.11, 12.7.2.2). +10 a bandit
  killed, -20 a survivor killed (13.2.8.10.4, .11.5), -20 for turning a camp hostile (13.4.3.2.1.1.1), and the tasks'
  amounts. It sets the friend's cadence (500 - 3 × karma s), the bandit camps' (400 + 2 × karma s), the gifts, and
  Jasmine's unlock (CHARACTERS).
- **No traders** in either game; these camps are the nearest thing.

**Remake:** none. `bots.lua` (FRIEND's), new `data/tasks.lua`, `data/ui/map.lua` (the Tasks tab), `state.lua` (karma).

**Scenarios** (world/hud):
- a camp of three in a wild location;
- a task taken saves, shows in Tasks, and when done gives XP and karma;
- a cancel costs karma;
- a gift at the top.

### 85. PERF-27 — 27.3.31: the sleep pass, the drops' walks, the autosave

**Findings:** none; Progress 27.3.31 and 27.3.13's autosave.

**Effort** M. **Depends on** POPULATION. **Beside** SPECIALS-A, SPECIALS-B, PORTRAIT.

**What to do:**
- **The sleep pass.** Spread the whole sleep pass, today every fifteenth frame (15,860 instructions explored, 1.4-2.6
  ms at a phone's pace), over its frames. Re-measure after POPULATION removes far zombies first.
- **Drops.** Bin drops by cell so the four walks a frame (draw, `item.nearest`, `item.within`, the render list) don't
  grow with them.
- **The autosave.** Cut the explored world's autosave (625 KB, 215-365 ms every three minutes).
- The corner minimap's batch goes with HUD-TOP.

**Scenarios** (tooling/perf): the instruction counts before and after, in the web build at 4x.

### 86. ISLANDS — Each band its island's content (waits on Q6)

**Findings** (decided by Q6): world#4, events#16, events#19.

**Effort** L. **Depends on** Q6, POPULATION and the place tracks. **Beside** SURVIVORS, PERF-27.

**If Q6 says (a) or (b), make identical, band N as island N:**
- **Quota and kinds** (world#4's table; 3.8.2-3.8.6): more cities, bases, hospitals, fire stations, gas stations and
  cafes on each later island; from island 2 a gun-shop location (32: the gun shop, a shed, five cars) in place of a
  wild one (3.12.4); military kind 5 from island 2 and kind 70 from island 4, where island 1 has kind 22 (3.12.5-9).
- **Crashes** (3.14; events#16): none and 1 humvee on island 1; then 2/3, 3/5, 4/6 and 5/8 helicopter/humvee crashes on
  islands 2-5. Helicopter crashes stand in location kinds 1, 2, 8, 10 and 52, humvee crashes in 5, 13 and 15.
  Walking into a helicopter crash's `ach_heli_counter` gives +25 XP and counts for "Blackbox Collector" (6.1.2); a
  humvee crash +15 (6.1.3). Found crashes are marked on the page (world#38). The radio's scan (8.18) answers "Looks
  like a military crash site is nearby." and marks them, or "Looks like there are no military crashes nearby.".
- **The albino deer** (events#19): from island 4, one deer in ten from a deer point is kind 20 (13.7.1.21.1.1.7,
  13.1.1.20). It runs with the `run_*2` frames and leaves `deer_rare_dead`, flayed like a deer (cut both from 1.0;
  Progress 28.2.1 left them out).
- **Per island:** its spawn switches; Day_cold 9 from island 5 (HEAT); snow from island 4 (SNOW); the clock +90 minutes
  and 100-300 XP on arriving (3.8.2-6, 12.16.1.9-.13) — in one world, on first entering a band.

**If Q6 says (c),** nothing here.

**Remake:** `data/places.lua` (one quota for every band), `crash.lua`, `wildlife.lua`, `animals.lua`, `minimap.lua`.

**Scenarios** (world):
- each band's counts and kinds over seeds;
- crashes found give XP and their marks;
- an albino deer one deer in ten in bands 4 and 5.

### 87. BUNKER — The underground bunker (waits on Q6)

**Findings** (decided by Q6): events#17.

**Effort** L (split the map from its contents if it grows). **Depends on** Q6, CAMPS-TRUNKS (lootboxes 51-53),
POPULATION (its spawners). **Beside** PERKS, ACHIEVEMENTS.

**If Q6 says (a) or (b), make identical:**
- **The entrance.** From island 2, a `bunker_entrance` stands in a random location of kind 1, 2, 8 or 10 not already
  holding a crash (3.15; live at (14928, 12420) on island 2). Touching it asks "Enter this bunker? You will be able to
  get back." (Level_menu 11).
- **Inside** (Bunker_generate(level), 3.1.4): a separate underground tilemap (at (2306, 17464) in the original).
  - Its own ambience (`bunker_ambient`), no rain or night, the map's marker hidden.
  - Sliding doors (`bunker_door`): some broken ("This door is completely broken."), some needing the Officer's
    keycard ("This door requires "Officer's Keycard".", 3.1.6; `card`).
  - Crates: `bunker_lootbox` kinds 51, 52 and 53 (items#19's lists).
  - Zombie spawners checked every 3 s (3.1.7).
- **Leaving** by `bunker_exit` (Level_menu 12) runs Bunker_erase, and the entrance shuts for 1800 s: "Bunker will be
  avaliable after N minutes." (Level_menu 13).
- **Its place in the run:** on island 5 its elevator leads on (ENDGAME); "🚪" counts it (ACHIEVEMENTS); a task can
  reward its coordinates (SURVIVORS).
- **Art:** the bunker's sprites from 1.0 (cut.py; `missing.txt`).

**Scenarios** (world):
- the entrance in band 2 or later;
- in and out, and shut 1800 s after;
- a keycard door and a broken one;
- the lootboxes' lists.

### 88. ENDGAME — The crossings, the radio and the rescue (waits on Q6)

**Findings** (decided by Q6): events#14, events#15.

**Effort** L. **Depends on** Q6, ISLANDS, DEPLOY (the waves), XP, DEATH; VEHICLES-A for (b)'s bridge. **Beside**
PERKS, ACHIEVEMENTS.

**If Q6 says (b), the crossings** (events#14):
- **The boat** (`b_exit`, in the east sea, 2.11). Touching it with no bandit within 300 px ("There are enemies
  nearby!", new 202) opens a dialog (8.8.1, 12.5.1). Island 1: "You need wood piles to repair this boat.", and
  Repair spends one (12.5.10.5). Island 2: duct tape. Island 3: gasoline, "Fuel". Then "Boat is repaired, are you
  ready to move to the next island? You will be unable to come back." with Ready and Not ready (events'
  `S/orig/l2_boat_dialog.png`, `l3_boat_ready.png`).
- **Island 4's exit is a bridge,** crossed only in a car ("I can try to jump overthere on a car.", 12.5.11; the bridge
  level 3.2), then "Are you ready to go on 5th island? There will be no way back.".
- **Leaving** (Level_change, 8.8.5): Map Notes and Officer's keycards are taken from the hand slot; Save_loot (5.2)
  keeps health, the meters and everything worn and held; the task is cancelled; the curtain falls. In one world:
  each band's east edge is shut until its crossing is made, and stays shut behind.
- **Arriving** on the west beach beside a `boat_arrive` (2.13.2-.4) or across the bridge (2.13.5): the clock +90
  minutes, "DAY N", 100-300 XP and an autosave.

**If Q6 says (a) or (b), the end** (events#15):
- **The radio** on island 5 (`b_exit`'s "Buy" frame, 3.8.5): "Try to contact someone with a radio?" (new 640); Yes plays
  `radio_noise` and "...Copy! We hear you! Will be on your position in one day, hold on!..." (new 639), and
  `Final_Day` is 1 (12.5.10.20). In (a) it stands at the far east of band 5.
- **The final day** (8.2.1, 8.2.2), one in-game day (36 minutes):
  - a 1500 px noise every 10 s, and every 3 s in the last few minutes;
  - an attack wave (DEPLOY's) every 90 s, and every 30 s after 30 minutes;
  - every 4 s, `warn_zed` sends each flagged infected at the player (8.2.1.3);
  - the bunker refused (3.1.1.2).
- **The rescue.** A helicopter flies over ("I need to return to the radio!", new 656; 8.1.1, `heli_engine`). At the
  radio, The_End (8.2.4): every enemy frozen, nearby bandits shelled, the helicopter lands ("Hey! I'm Boris, welcome
  onboard!", "Nadia, fly to the East, to 4316 signal.", new 653, 654), a fade, then the happy end.
- **The happy end** (10.3.1, 10.3.16): a teal sky and "You successfully escaped." (new 655) over the death screen's
  stats; the item unlocks for characters 4-19 (10.3.1.1-.5; CHARACTERS); the save wiped (8.2.3.1).
- **The elevator** from island 5's bunker to "unknown islands" (Level_menu 99) belongs to (b) with BUNKER.

**If Q6 says (c),** nothing here.

**Art:** `b_exit`'s frames (boat, rubber, gas, bridge, Buy), `boat_arrive`, the helicopter, cut from 1.0.

**Scenarios** (play):
- the radio starts the final day, whose waves keep their cadence;
- the helicopter, the landing and "You successfully escaped.";
- the save wiped after;
- in (b), each repair and the crossing with no way back.

### 89. PERKS — Perks, and the Stats tab (Q1 may keep it out)

**Findings:** the perks screen, hud#9 with events#10.

**Effort** L. **Depends on** XP, PORTRAIT. **Beside** BUNKER, ENDGAME.

**Make identical.**
- **16 perks of two levels each** (ui 400-415): Sharpshooter, Scout, Sprinter, Camel, Hamster, Snowborn, The Bullet
  Farmer, Survivalist, Blocker, Puncher, Trunk Digger, Hunter, Sweet home, Red+, Vigilance, Agronomist (12.7.11). For
  example, Red+ lvl 1 halves blood loss, and lvl 2 stops bleeding twice as fast. The surveys' "Not checked" lists name
  the numbers they change (survival: camel, hamster, regen, snowborn, sweethome, metabolism, eye; combat: Crit_bonus,
  perk_header, perk_meleedef, perk_metabolism): each brings back the perk's branch the earlier tracks left out.
- **Bought** from the portrait's screen (12.7.5, 12.7.11) with `MD_Perk_1` (events' `S/orig/p1_perks.png`,
  `p2_perks_xp.png`).
- **`perk_cost`** is 200 on a page's first run and 300 after any return to the menu (Restart_game 26); each buy adds
  100 + 20 × perks learnt (12.7.11.18). DATA10 brings 1.0's 200 into the globals.
- **The Stats tab:** Score, Minutes alive, infected killed, Bandits killed, Karma, Days survived, Difficulty
  (12.7.2.2).
- Characters 4-20 start with some (CHARACTERS); all of them earns "Survivor God" (12.7.14; ACHIEVEMENTS).

**Remake:** none. Progress "Perks (revisit after Stage 15)". New `data/perks.lua`, `data/ui/perks.lua`.

**Scenarios** (hud/play):
- a perk bought at its cost, and the next one dearer;
- each perk's effect (table-driven);
- the Stats tab's seven rows.

### 90. ACHIEVEMENTS — Achievements, their toasts and the lifetime stats (Q1 may keep it out)

**Findings:** the menu's buttons, menus#15 with events#11.

**Effort** L. **Depends on** XP, MENU-LOOK. **Beside** BUNKER, ENDGAME.

**Make identical.**
- **30 achievements** (the `fam_ach_24` family; thresholds in var#4-6; names ui 101-130, tasks ui 151-180): 24 hours
  (survive 1, 2 or 3 days), Zedkiller (20, 40, 80 infected), Blackbox Collector, Tourist, BRO, Deerhunter and the
  rest.
- **Tiers:** bronze on Novice, up to silver on Regular, gold on Veteran and Legend (12.17.1, 10.1.1). Counts are per
  run (Restart_game zeroes them), and nothing counts in a seed run or off the map.
- **The toast:** reaching a tier pops its badge at the top centre for 4 s with `md_achievment unlocked` (12.17.2;
  events' `S/orig/p3_ach_toast.png`).
- **Kept** across runs: ranks recounted at death and on leaving to the menu (10.1.1.2-.4), in localStorage
  "achieves" in the original; here a profile file beside `core/settings.lua`.
- **The screen** (ME 3.2.2.15; `S/orig/m1_achievements.png`): a full-screen `bg_rust` panel with the rule ("To
  unlock achievements, you must play a whole session until your character dies ..."), overall stats (the best
  minutes alive, infected and bandits killed in all, deaths; 10.3.12-.15), the list with ranks, drag-scrolled, and
  an X to close.
- **The menu's other plates** (menus#15): Achievements (ui:3) and Unlocks (ui:390, CHARACTERS); the blue Facebook
  plate at the left edge (ME 3.2.3.7 → facebook.com/MINIDAYZGAME) and the flashing red "MINI DayZ 2" plate at the
  right (ME 3.2.3.5 → minidayz.com), which link to the original makers' pages (Q1 lets the user leave these out).

**Remake:** none. New `data/achievements.lua`, `data/ui/achievements.lua`; `data/ui/menu.lua`.

**Keep:** the menu's Continue, Start and Settings and their words (Stage 22).

**Scenarios** (settings/play):
- a tier earned pops its toast, by difficulty;
- ranks kept across runs;
- the screen's stats;
- the plates' places.

### 91. CHARACTERS — Twenty characters and the Unlocks pages (Q1 may keep it out)

**Findings:** character#26 with events#12; events#13.

**Effort** L. **Depends on** ACHIEVEMENTS (shared counters and toasts), PERKS (their starting perks), ART10 (their
art). **Beside** VEHICLES-A.

**Make identical.**
- **Twenty characters** with their kits, perks and unlock rules: events#12's table, from Survivor 1-3 (jeans and a
  T-shirt, white, yellow or black hands; free) to Mike "Designer" (reach the unknown island, Legend). Their skins and
  hands are not in `Assets/Sprites`; cut them from 1.0 (`missing.txt`).
- **Character select** under Unlocks (events' `S/orig/m2_characters.png`, `m3_char_locked.png`): 20 portraits, the
  locked ones grey; a tap shows the kit or the requirement; Choose and Current. The pick is kept ("Player_skin", ME
  4.1.1), and a run's start dresses and arms him (2.13.7-.26). SURVIVAL's T-shirt and jeans are Survivor 1's kit.
- **Unlocking** (4.1-4.17): `unlock_char_N` stores "char_N", pings Unlocks and toasts "New character unlocked!" with its
  sound (12.17.3); only on the map, never in a seed run, only on the difficulty named.
- **The Unlocks menu** (events#13): Character, Weapons, Items and Back, each with an orange ping until visited.
  - Weapons (Deagle, MAC-10, Saiga-12k, AUG, VSS, SV-98, Katana) are all unlocked in 1.0: a fresh profile reads
    "Unlocked. Locations: ..." and no gun is gated.
  - Items (plaster, lighter, smoke grenade, laser sight, Officer's keycard) unlock by escaping with sets of characters
    (10.3.1.1-.5; ENDGAME). Until then LOOT's fallbacks hold (items#21).
  - The page has 1.0's own slip: it appends the MAC-10's "0/200" to the plaster's line (ME 3.2.2.28.1.6).

**Remake:** one character (`player_skin_def_1`), no select screen. `player.lua`, new `data/characters.lua`,
`data/ui/menu.lua`.

**Scenarios** (settings/play):
- the strip, a free survivor chosen and his kit at the start;
- an unlock earned by its rule on its difficulty, with its toast;
- the pick remembered.

### 92. BANDITS-A — Bandits: the wanderers (Q1 may keep it out)

**Findings:** events#1 (the wanderers; the camps are BANDITS-B).

**Effort** L. **Depends on** FRIEND (the bots), SURVIVORS (karma), XP. **Beside** VEHICLES-A.

**Make identical.**
- **In every normal run, from the first minute** (correction 16): the Bot Manager's 60 s tick (13.4.1.1) runs
  `Check_bot_puncher` and `Check_bot_firearm`, ignoring the spawn switches. They put a melee bandit
  (`bot_enemy_puncher`) and an armed one (`bot_enemy_shooter`) on a random bot point 700-3000 px from the player;
  each is removed past 2600 px (13.4.1.7, .8, .13, .14). Punchers also come every 180 s and gunmen every 300 s.
- **What they do:** hunt the player and the survivors (their AI is 13.4.4-13.4.6; read it, the survey skimmed it).
  A bandit walking onto a stash within 400 px of the player destroys it (8.23.3).
- **Each kill** counts in "Bandits killed" (DEATH, ACHIEVEMENTS) and gives 15 or 50 XP (13.2.8.8, .9) and +10 karma.
- **They are targets** (combat#11's family names bandits) and the boat refuses with one near (ENDGAME).
- **Live on Novice:** a melee bandit, an armed one and a friend 74 s in (events' `S/orig/k1_bandit.png`).

**Remake:** none. `bots.lua` (FRIEND's), `targeting.lua`, `combat.lua`, `population.lua`.

**Scenarios** (combat/world):
- a melee and an armed bandit 700-3000 px off within a minute, gone past 2600;
- they hunt and fight;
- a kill's XP, karma and count.

### 93. BANDITS-B — Bandit camps and the boss (Q1 may keep it out)

**Findings:** none of its own; events#1's camps.

**Effort** L. **Depends on** BANDITS-A. **Beside** VEHICLES-A, VEHICLES-B.

**Make identical.**
- `Bot_bandits_check_situation` flags the nearest wild location (kinds 9, 11-14, 17, 21) for a bandit camp when none
  stands (13.4.1.22, .23). Its three bandits appear when the location is built: rifle, pistol or the boss
  (`Boss_bandit_spawns_on` 1 in 1.0's globals).
- Camps also come every 400 + 2 × karma s from island 2 (13.4.1.2-.6; band 2 under Q6).
- XP 25, 50 or 100 (13.2.8.10); the survivors' "wipe out a bandit camp" task (SURVIVORS).
- Live: a camp of three when walked near (events' `S/orig/k1_bandit.png`).

**Scenarios** (world/combat):
- a camp of three in a wild location;
- the boss's XP;
- the task completed.

### 94. VEHICLES-A — Drivable cars (Q1 may keep it out)

**Findings:** events#18.

**Effort** L. **Depends on** ROADSIDE (the roads' cars), AUDIO. **Beside** CHARACTERS, BANDITS-A, BANDITS-B.

**Make identical.**
- **Where:** at every map's generation `Car_respawn_uaz`, `_volga` and `_hammer` (3.6.3, 9.40-9.42) put drivable
  cars on road locations, with random fuel and condition. Live on island 1: one UAZ and one Volga (events'
  `S/orig/v1_car.png`).
- **Driving** (group 9): getting in and out, collisions, a car HUD with fuel and condition (`gui_car_bg`,
  `gui_car_condition`, `gui_car_fuel`), the engines' sounds (`car_*`, the UAZ and Volga engines), aiming from a car
  (8.5.3, 8.5.4.3), and the camera in one.
- **A running car warms** by 4 (6.3.10; HEAT's term).

**Remake:** cars are parked props with trunks. New vehicle module; `data/trunks.lua`.

**Keep:** the parked cars' trunks (Stage 23); the TARGET button's `gui_btn_car_shoot` face (Stage 20) beside the car
HUD's own use of that sheet.

**Scenarios** (play):
- a drivable car on a road;
- in, drive, fuel falls, a crash costs condition, out.

### 95. VEHICLES-B — The Humvee's gun, the friend riding, fuel and repair (Q1 may keep it out)

**Findings:** none of its own; events#18's rest and items#35's gasoline and toolbox.

**Effort** M. **Depends on** VEHICLES-A, FRIEND. **Beside** BANDITS-B.

**Make identical.**
- the Humvee's machine gun (`car_hammer_hmg_anims`);
- the friend getting in and bailing out (13.4.7.2.19, .20);
- gasoline refuels (8.10.3.46: 6 s, `action_refuel_0`) and the car toolbox repairs (8.10.3.52: 5 s,
  `action_repair_0`);
- a task's "Coordinates where a drivable vehicle is located" (SURVIVORS).

**Scenarios** (play): the gun fires from the Humvee; the friend rides; fuel and repair.

### 96. LANGUAGE — The original's Language option

**Findings:** menus#27.

**Effort** L. **Depends on** NAMES-TEXT, LINES, MENU-LOOK. **Beside** VEHICLES-B. Last, so every track's text is in its tables.

**Make identical.**
- **The plates.** Options' "Language" opens two plates, "English" (ui:500) and "Español" (ui:505), with no Back. Read
  which files `var 55` loads: the survey found Russian (ME 3.2.2.22).
- **The files.** The build ships eight languages' xml (`l_cz_*`, `l_de_*`, `l_es_*` …).
- **First,** move all text into string tables from `l_eng_*`.

**Scenarios** (settings): switching loads the second set, and every screen draws it.

---

## Kept: the differences the user asked for

One line each, the ask quoted. Where several findings are one difference, all are named.

1. **The start menu's full-screen red sky and faster skyline** (menus#5): *"Slow red background, a little faster
   scrolling background of sky scrapper."* (Stage 22)
2. **"Bites can infect" on the Veteran and Legend cards** (menus#22, settled): follows *"limit the infected status to
   only hard mode"* (Stage 26).
3. **The settings window's contents** (menus#24): *"Settings window that toggles touchscreen on/off, volume slider,
   return to main menu and resume"* (Stage 17); *"add dead zone to settings ... Add vertical and horizontal dead
   zone."* (Stage 18)
4. **CONTINUE's refusal line** (menus#41, settled): follows *"just finish generated worlds and make it work when
   clicking start"* (Stage 25).
5. **USE only with something in reach** (hud#15): *"the pick up button should only appear when there's an item to
   pick up"* (Stage 22)
6. **The switch cycle melee, pistol, rifle** (hud#17, combat#49): *"switch button goes from Melee, Pistol then Rifle
   and back to melee"* (Stage 20)
7. **The attack button's trigger art** (hud#18, settled): *"show weapon trigger sprite, gui_btn_attack, when holding
   a pistol or rifle"* (Stage 20)
8. **Melee on the attack button, with a fist and a swoosh** (hud#19, character#18, combat#50): *"and a fist sprite if
   holding melee"* (Stage 20); *"remove right click as melee and make it so that using your melee displays a white
   swoosh sprite"* (Stage 19)
9. **Status icons at the end of their bars** (hud#33): *"Fix the bleeding icon, starving icon and other icons to be
   at the end of the appropriate bar"* (Stage 17)
10. **The PC keys, Space alone firing, the TAB bag hint** (hud#37, inventory#J4, combat#51): *"Keybinds, E to pickup
    ..., Tab for inventory backpack icon at the bottom with Tab written underneath it"* (Stage 17); *"Only the fire
    button should fire the weapon, and on pc, only space bar."* (Stage 18)
11. **The held weapon's picture bottom-left** (hud#42): *"Instead of the name of the weapon on the bottom left,
    include the sprite of the weapon, no name, no ammo, no durability"* (Stage 22)
12. **The TARGET button** (hud#43, combat#53): *"the red circle sprite for target switching, gui_btn_car_shoot_sheet0,
    it should only show if there's an enemy on screen"* (Stage 20)
13. **The door button, doors that don't open by themselves** (hud#44, world#40): *"Door button with door sprite when
    near a door"* (Stage 23)
14. **The low-health grey from 75%** (hud#45, combat#55, survival#34): *"Make screen gradually gray when health is
    low. 50% monkchrome when below 10% health. effect doesn't start until under 75% health."* (Stage 20)
15. **Weapons and spare clothes in backpack cells** (inventory#B5, items#48): *"Pistols, Rifles, Melee should only go
    on backpack slots, and they should be 1x2 for pistols, 1x3 for rifles and 1x2 for melee. Clothing shouldn't go
    on other clothing."* (Stage 20)
16. **Auto-equip only into an empty slot** (inventory#B6): *"grabbing item from ground or container should auto equip
    if slot is empty"* (Stage 20)
17. **Containers: furniture, trunks, NEARBY's tabs, TAKE ALL** (inventory#C1, items#2, world#42): *"just include tabs
    on the left hand side with the icons of the container"* (Stage 22); *"Lootable vehicle trunks. appropriate
    container scales"* (Stage 23); *"increase the size of the containers tabs"* (Stage 26); *"The take all should
    consume a container slot"* (17.4)
18. **The tooltip with stats and actions** (inventory#D1): *"Tapping something opens a tool tip with item stats and
    actions, dragging allows moving items"* (17.4)
19. **NEARBY as a drop target** (inventory#E2, settled): follows the asked containers.
20. **The drag ghost under the finger** (inventory#E6): *"Just make it so that they're where the touch input began,
    same with dripping items."* (Stage 20)
21. **Only the used item locked** (inventory#F2): *"when using an item, don't freeze the inventory, just the item that
    is being used."* (Stage 21)
22. **The use bar on the item and over the head** (inventory#F3, combat#57): *"use gui_wpn_cooldown_bg-sheet0.png for
    any interaction"* (Stage 20); *"crouched down with a bar above his head, matching the one that's under an item"*
    (Stage 25)
23. **The view not clamped at the map's edges** (character#11): *"The player is locked to the centre of the screen"*
    (Stage 16, 16.1.2)
24. **Shooting on the move** (character#27): *"Shooting while running does not play shooting animation."* (Stage 22)
25. **A hit slows a zombie for a second** (zombies#14): *"hitting a zombie should slow it down for a second."* (Stage
    22)
26. **A zombie's blow is the thud alone** (zombies#20): *"the zombie attacking sound should just sound like a hit
    sound."* (Stage 28)
27. **Wander groans** (zombies#24): *"Zombies should randomly make noises when wandering"* (Stage 22)
28. **Pistols' dispersion halved** (combat#7): *"Reduce dispersion for pistols."* (Stage 20)
29. **Melee reach 22** (combat#48): *"Less range on melee."* (Stage 22)
30. **"It's empty" and "I don't have ammo for this"** (combat#52): *"have a text say above the player "It's empty",
    and if there's no ammo "I don't have ammo for this"."* (Stage 20)
31. **A gun lowered without a target, raised at one** (combat#54): *"When there's no target, the weapon is lowered;
    when there's a target, the weapon is put up and aimed."* (Stage 20)
32. **Blood drops that stay** (combat#56): *"when bleeding, add blood drops that stay on the ground."* (Stage 20)
33. **Colder going east** (survival#14): *"Traveling right slowly gets colder and eventually the tiles change"* (Stage
    26)
34. **Cold sickness cured at 75% for a while** (survival#25): *"curable by keeping food water and heat above 75% for
    some time."* (Stage 23)
35. **Infection, on Veteran (and Legend), cured by tetracycline** (survival#29): *"effects, like infection, which
    needs to be cured by tetracycline"* (Stage 23); *"limit the infected status to only hard mode"* (Stage 26)
36. **No cull of loose items** (items#17): *"I want every item in the world to be permanent, and items that have
    pockets remember what was in them when they got dropped."* (Stage 6)
37. **The starter kit lights the fire, not matches** (items#31): *"lighting a campfire should only be done by standing
    near the campfire and using the campfire lighter kit from the inventory"* (Stage 26)
38. **One world with the seasons side by side** (world#3): *"Traveling right slowly gets colder and eventually the
    tiles change"* (Stage 26); *"make the biomes wider, like 100 across"* (Stage 27)
39. **The world's size** (world#5): *"Increase the height ... make the biomes wider, like 100 across"* (Stage 27)
40. **More places than the original's** (world#7): *"More interest points with roads connecting them."* (Stage 27)
41. **Roads joining every place** (world#8): *"make the map grid uniform"* (asked: *"roads connect every interest
    point to each other"*) (Stage 27)
42. **The outside dark from inside a room** (world#41): *"entering the building hiding the outside"* (Stage 26);
    *"Make it hidden only when the player is inside"* (Stage 28)
43. **Lots built once and kept** (world#43): *"I want every item in the world to be permanent"* (Stage 6)

**Platform** (5):
- the exit dialogs on Android's back key (menus#38);
- Escape over the death screen (menus#39);
- the portrait prompt (menus#40);
- world sounds folded to mono and loaded ahead (audio#15);
- where saves live in the browser: LÖVE's save directory copied to IndexedDB (events#26).

AUDIO keeps a voice cap only as a phone's limit.

## events.md, folded in

`events.md` landed after the first version of this backlog and is folded in above. Its 26 findings are in the
summary's counts; its corrections are 14 (rewritten), 16 (extended), 19, 20 and 21; its central question is Q1 and
its islands are Q6. Where each finding went:
- events#1, #10, #11, #12, #13, #18 → BANDITS-A, PERKS, ACHIEVEMENTS, CHARACTERS (#12, #13) and VEHICLES-A: Progress'
  out-of-scope list, built last; Q1 can keep them out;
- events#2 → FRIEND; #3 and #4 → SURVIVORS; #7 → AIRDROP; #9 → XP;
- events#5 → SPECIALS-A; #6 → SNOW; #8 → DEPLOY; #20 → GARDEN-FISH; #21 → ZOMBIE-AI; #22 → SURVIVAL; #23 → AUDIO;
  #24 → PORTRAIT;
- events#14, #15, #16, #17, #19 → Q6, with ISLANDS, BUNKER and ENDGAME waiting on its answer;
- events#25 → no difference (the tutorial is unreachable in 1.0); events#26 → platform.
