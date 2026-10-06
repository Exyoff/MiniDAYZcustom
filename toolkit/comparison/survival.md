# survival: the remake against the official 1.0

*Area surveyed:* Survival, time and weather: health, food, water and heat and their rates on every difficulty, regeneration and what it needs, bleeding, sickness, infection, cold and their cures, temperature and what clothing keeps out, rooms and campfires, the day/night cycle (length, start, darkness, dusk and dawn, lights in the dark), weather (rain, snow, fog), sleeping, radiation and the red zone, and the other things besides zombies that hurt or kill the player.

Surveyed 2026-10-06 against the remake at 75d9a88 (Stages 27.1 to 28 merged), in the read-only worktree `wt/survey`.

Shots and logs are under `S = C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/survival`:
- `S/orig/*.png` and `S/orig/*.out`: the original, driven by `../orig.cjs` (headless Edge, 1280x720, `--touch`), with the state dumps `S/dump.js`, `S/light.js`, `S/raindump.js`, `S/firedump.js` and the rain trigger `S/rain.js`.
- `S/rm/*.png` and `S/rm/*.log`: the remake, driven by `lovec.exe game --shot --res 1280x720 --world 7` (helper `S/rrun.sh`).
- `S/cmp/m_*.png`: side by side, original left.

## Summary

**What I read.** Original: event groups 6 (Player stats: the 5 s States tick 6.2, Update_Stats 6.3, bleeding, acid, timers), 2.11 (difficulty values), 2.13 (the start), 17 (day/night, clock, rain, dusk and dawn, Check_night), 18 (fireplace), 8.10.3 (vitamins, tetracycline, adrenaline, cigarettes, campfire kit and matches), 8.14/8.15 and 12.10.17.39-40 (headlamp, NVG), 12.1 (sleeping), 21 (breath), 23 (Timeline), 3.8 (per-level weather and cold), 2.2 and 2.8 (radiation, red zone), 13.2.2/13.2.3/20.6 (fire, traps, wire); the placed instances' blend modes and Fade properties in `data.js`; `l_eng_log.xml` and `l_eng_new.xml`. Remake: `survival.lua`, `daynight.lua`, `ambience.lua`, `campfire.lua`, `data/config.lua` (survival, daynight, campfire, effects), `data/ui/hud.lua`, `inventory.lua` (warmth), `app.lua` (draw order, holds), and Progress 5, 11, 23.7, 23.8, 24.3, 26.4.

**What I ran.** Original on Novice and Regular: the meters sampled every 10 s at the start; stripped of the start kit by day and at night; heat set to 0 until sick; noon, 23:31, 00:17, 00:46, 02:02, 05:31 and 05:35; rain and snow forced through the game's own "Rain" timer; a campfire placed and lit with the game's own item functions at 02:06, then stood at 60 and 120 px; inside a village house; midnight with the sleep button, its pad, and a sleep to 03:26; a zombie's reticle at night. Remake on Novice and Regular in generated world 7: the same six times of day, the meters over 20 s by day, at 02:00 and at heat 0, freezing and sick at 02:40, a lit campfire at 02:06, the snow band at noon, a zombie at night.

**The biggest gaps.**
1. The survival rates are the 1.2 fan mod's scalars times invented bases. On Regular the original takes 1 food and 1 water every 5 s (a full bar lasts 8 minutes) and regenerates 6 hp; the remake takes 0.165 and 0.22 and regenerates 1.75. Novice regenerates 8 in the original, 2 in the remake. The original also starts food, water and heat at 75.
2. Heat is a different model. The original sums points into Player_temperature (the day's cold by difficulty, the night's, the rain's, minus the clothes' warmth, plus a room's 1-5 or a fire's 15) and moves heat by half of it each tick, so a naked player cools by day and a warmly dressed one warms outdoors. The remake has no cold by day, a night cold the same on every difficulty, clothing as a capped fraction, and rooms and fires that switch the cold off.
3. No weather at all: the original rains (1 in 4 every 6 minutes), chills by Rain_cold outdoors unless a raincoat is worn, tints the screen and swaps rain sounds under a roof; snow replaces rain on later levels and after 5000 s alive.
4. Night looks and runs differently: the original is a flat 90% navy veil from 01:00 to 05:00 (ramping linearly through 00:00-01:00 and 05:00-06:00), sand at 12% of its noon brightness, with a faint orange/yellow tint at 23:30 and 05:30; the remake starts darkening at 23:00 and only reaches 48%. Lights in the original are holes cut in the dark (a fire's 250 px circle, flares, the headlamp, street lamps, muzzle flashes; NVG halves the dark); the remake has only an additive glow round a campfire.
5. No sleeping (time skipped to 06:00 at the cost of the meters), no vitamins' doubled regeneration, no immunity after tetracycline, no smoking pause, no adrenaline crash.

**What is kept and why.** Infection from bites, Veteran only, cured by tetracycline (Stages 23 and 26). Cold sickness cured by food, water and heat at 75% for a while (Stage 23). The east getting colder with the ground turning to snow (Stage 26). Heat from a campfire or indoors (Stage 24; the asks are met, only the numbers differ). The campfire kits, build mode and lighting from the kit (Stages 24 and 26). Blood drops that stay (Stage 20). The low-health grey (Stage 20). Two points are unclear and need the user: how "staying out too cold" catches cold sickness, and whether tetracycline should also cure it as the original's does. The STALKER radiation and PUBG red zone are game modes Progress keeps out of scope, which is not a user ask.

**Covered in other surveys, not repeated here.** The regeneration heart (hud 4), the heat '+N/-N' text against the red arrow (hud 5), the clock (hud 6), 'DAY N' (hud 7), the sleep button itself (hud 13), the survival lines over the head such as "I'm freezing." (hud 29), the status icons' animations and places (hud 31-33), the grey hit flash (hud 30, combat 36), food, drink and medical values and use times (inventory G1, G3, F5), inert items (inventory G4), garment warmth values (inventory B9), the start kit (character 6, inventory B11), the energy drink (character 16), breath puffs (character 22), the death screen's cause line (menus 33), Legend (menus 19), night sight of zombies (zombies 7) and the night muzzle flash (combat 18).

## Findings

Verdicts: make identical 30, keep (user asked) 4, unclear 3

### 1. Food, water and heat start at 75

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/systems/survival.lua`

**Original:** player_collision_base is placed with var#13 food 75, var#14 water 75, var#15 hp 100, var#16 heat 75 (objects.txt t181). Measured on Novice: food and water 74.5 at 9 s alive (two ticks of 0.25), heat 76 (S/orig/nov_start.png, `nov_start` dump). On Regular food 73 at 9 s, heat 75. The casual mode respawn sets 50/50/50 (6.2.18.1).

**Remake:** start_hp, start_food, start_water and start_heat are all 100 (config.lua survival). Measured: hp, food, water and heat 100.000 at the start (S/rm/reg_rates.log).

### 2. Hunger and thirst rates on every difficulty

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stages 25-26 asked where the difficulty is chosen, not its numbers)
- **Files:** `game/data/config.lua`, `game/src/game/systems/survival.lua`, `game/data/generated/original_globals.lua`

**Original:** Every 5 s (6.2.2) food -= Starving_current and water -= Thirsty_current (6.2.2.2). Layout start sets them by difficulty (2.11.9-2.11.12): Novice 0.25 and 0.25, Regular 1 and 1, Veteran 1 and 1, Legend uses Veteran's. So food and water fall together, and on Regular or Veteran a bar of 75 is empty in 75 ticks, 6.25 minutes; on Novice in 25 minutes. Measured on Regular: food and water 73 to 69 in 20 s (S/orig/reg_*.png dumps); on Novice 74.5 to 73 in 30 s.

**Remake:** food = hunger_rate 0.15 x the difficulty scalar, water = thirst_rate 0.2 x it (survival.tick_drain). The scalars are novice 0.75, regular 1.1, veteran 1.4: the 1.2 fan mod's Novice/Regular/Veteran_starving values (globals.txt's list of 29), and the base rates are invented (config comment). Per tick: Novice food 0.1125, water 0.15; Regular 0.165 and 0.22; Veteran 0.21 and 0.28. Measured on Regular: food 100 to 99.34 and water 100 to 99.12 in 20 s (S/rm/reg_rates.log). A full bar lasts 50 minutes of food and 38 of water on Regular, six times the original's.

### 3. Regeneration on every difficulty

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`

**Original:** When food, water and heat are all >= 50, not bleeding (var#22) and not sick (var#35), hp += Regeneration_current each 5 s tick (6.2.2.10): Novice 8, Regular 6, Veteran 4, Legend 4 (2.11.9-2.11.12). 50 to 100 hp takes 7 ticks (35 s) on Novice.

**Remake:** regen_rate 1 x novice_regen 2.0, regular 1.75, veteran 1.5 per tick (config.lua survival; the 1.2 mod's Novice/Regular/Veteran_regeneration). 50 to 100 takes 25 ticks (125 s) on Novice.

### 4. Vitamins double regeneration for 60 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/systems/survival.lua`, `game/data/ui/hud.lua`

**Original:** Vitamins (item 29, objects.txt t298) play 'vitamins' at -10 dB, take 2 s, then set var#33 = 1 for 60 s (8.10.3.29). While var#33 is 1 the tick adds Regeneration_current twice (6.2.2.10.1) and gui_regeneration shows frame 2 (6.2.2.10.3). (inventory.md G3 calls this a 60 s immunity; the events give no immunity for vitamins, only for tetracycline, 8.10.3.40.)

**Remake:** Vitamins are inert (data/items.lua's undescribed block); nothing doubles regeneration.

### 5. The heat model: points summed into a temperature, half of it each tick

- **Verdict:** make identical  **Effort:** M  **User's ask:** Stage 24 "Heat is acquired by being near a campfire or indoors" and Stage 26 "Traveling right slowly gets colder" are met by either model; the model itself was not asked
- **Files:** `game/src/game/systems/survival.lua`, `game/src/game/systems/daynight.lua`, `game/data/config.lua`

**Original:** Update_Stats (6.3) rebuilds Player_temperature from 0 every call: minus Night_cold at night (6.3.4); minus Rain_cold in rain outside a warmzone (6.3.5); plus 15 by a lit fire (6.3.6); plus the room's warmzone var#0 (6.3.8); plus 4 in a running car (6.3.10); and always minus (Day_cold - the worn warmth var#28 + var#29 + var#30) (6.3.11). Each 5 s tick heat += Player_temperature / 2 (6.2.2.3), capped at 100 (6.3.15). The sum can be positive anywhere: on Novice the t-shirt and jeans (2 + 5) beat Day_cold 6, so heat rises 0.5 a tick in the open by day (measured 76 to 79 in 30 s; '+1' on the bar, S/orig/nov_start.png). Heat at zero costs 0.25 hp a tick (6.2.2.8).

**Remake:** heat_rate 0, so nothing cools by day; out in the open heat -= (night drain + east cold) x (1 - insulation) (survival.tick_drain); by a fire or in a room the cold is switched off and a fixed amount is added up to a cap (survival.shelter, warmth). Heat can never rise in the open. Freezing costs 0.25 hp a tick (identical).

### 6. Cold by day, by difficulty

- **Verdict:** make identical  **Effort:** S (with 5)  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/systems/survival.lua`

**Original:** Day_cold is 6 Novice, 7 Regular, 8 Veteran and Legend (2.11.9-2.11.12), and 9 on levels 4 and up (3.8.5, 3.8.6.1). It applies day and night, indoors too. Naked by day on Regular: Player_temperature -7, heat -3.5 a tick, measured 71.5 to 57.5 in 20 s ('-7' on the bar, S/orig/reg_naked.png). In the start kit on Regular: 0, heat steady at 75.

**Remake:** No cold by day anywhere west of the cold bands: heat stays at 100 by day naked (S/rm/reg_rates.log, 08:00 to 08:14).

### 7. Cold at night: by difficulty, flat from 00:00 to 06:00, indoors too

- **Verdict:** make identical  **Effort:** S (with 5)  **User's ask:** none
- **Files:** `game/src/game/systems/daynight.lua`, `game/data/config.lua`

**Original:** Night_cold 1 Novice, 2 Regular, 3 Veteran and Legend (2.11.9-2.11.12), subtracted while Day_mode is 0 (6.3.4). Day_mode is 0 from 00:00 to 06:00 (17.1.3.1, 17.1.9.1), flat, with no easing, and a room does not stop it (6.3.4 has no warmzone test). Naked at 02:00 on Regular: Player_temperature -9, heat -4.5 a tick (measured 57.5 to 44 in 15 s, S/orig/reg_night_naked.png). In the start kit: -2 on Regular, so a full night (108 ticks) costs 108 heat.

**Remake:** heat_drain_night 1.5 plus heat_drain_deep up to 0.5 at 02:30, eased by the same smoothstep as the darkness from 23:00 (daynight.heat_drain), the same on every difficulty, and none at all in a room or by a fire. Measured naked at 02:00: 1.97 a tick (S/rm/reg_rates.log).

### 8. Clothing warmth: points against the cold, three slots, rounded up by condition

- **Verdict:** make identical  **Effort:** S (with 5)  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`, `game/src/game/inventory.lua`, `game/data/generated/clothing_stats.lua`, `tools/extract_clothing_stats.py`

**Original:** Only the helmet (var#8), outerwear (var#18) and pants (var#20) count (6.3.1-6.3.3, 6.3.11), each worth ceil(var#9 / 100 x condition) points subtracted straight from Day_cold. Enough of them turn the cold into warmth: a cloak 9, gorka pants 7 and an ushanka 6 make +14 by day on Veteran (+7 a tick). The raincoat (item 353) also cancels Rain_cold while its condition is above 0 (6.3.5).

**Remake:** survival.insulation sums every worn slot's warmth (the original's integer divided by 40) times condition, linear, not rounded up, caps it at 0.75 and multiplies the night's and the east's drain by (1 - it), so clothing can only slow the cold, never beat it (survival.lua 629-663; Progress 11 says the cap is there so a night "is never free"). Per-garment values are inventory.md B9's finding; the remake's code now does scale warmth by condition, which B9 says it did not.

### 9. Rooms warm by their building, on top of the clothes and the night

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 24 "Heat is acquired by being near a campfire or indoors" (met by both; the numbers were not asked)
- **Files:** `game/src/game/systems/survival.lua`, `game/data/config.lua`, `game/data/buildings.lua`

**Original:** Each room's warmzone carries var#0, added to Player_temperature while player_base overlaps it (6.3.8; set in 3.2.1.3.x and 3.5.7.1.6.x): 1 army tents; 2 bus, garages, gas station, gun shop; 3 castle, dot, hospital, hunter shelter, sheds, supermarket; 4 bar, hostel, barracks, school, village houses, yard, a deployed civilian tent; 5 church, city houses, fire station, barracks 2, HQ, piano house, police station, red brick. The night's and the day's cold still apply inside, there is no cap, and rain stops chilling. Measured in a village house on Regular in the start kit by day: Player_temperature +4, heat +2 a tick ('+4' on the bar, S/orig/in_house_day.png; '+5' in a city house, S/orig/sleep_pad.png). Naked in the same house by day it would be -3.

**Remake:** In any room, or its open doorway: +1.5 a tick (shelter_heat) up to 80 (shelter_cap), none of the night or the east reaching you, whatever is worn and whatever the building (survival.shelter, config.lua survival; Progress 24.3.2).

### 10. A campfire's heat, reach, burn time and three burns

- **Verdict:** make identical  **Effort:** M  **User's ask:** Stage 24 (the two kits, build mode, the starter lighting it) and Stage 26 (lit only from the kit, no button) stay; the numbers were not asked
- **Files:** `game/data/config.lua`, `game/src/game/campfire.lua`, `game/src/game/systems/survival.lua`, `game/src/game/save.lua`

**Original:** Lighting (matches, item 24, 8.10.3.24.2.1; the lighter likewise) takes 3 s, starts the 'Burn' timer at 180 s, and sets fire_place_warm on. While the player overlaps fire_place_warm (100x100, so about 65 px from the middle with the player's box) Player_temperature gets +15 (6.3.6), everything else still counting. A new fireplace has var#1 = 3 burns (8.10.3.23.2); each burn's end takes one, and at 0 the fireplace is destroyed (18.2.1, 18.2.1.3). Sleeping burns a lit fire down by the time slept (12.1.4.4). Measured at 02:06 on Regular in the start kit: Player_temperature 13, heat 74 to 87 in 2 ticks; still warmed 60 px off, not 120 (S/orig/fire_night.png, fire_night_60.png).

**Remake:** fire_heat +5 a tick to a full bar, night and east switched off, within fire_reach 56 px of the middle with no wall between (survival.by_fire); burns burn_seconds 600 and is relit forever (campfire.put_out leaves cold ashes); light_time 1.5 s (config.lua campfire). Measured: heat 74 to 84 in 2 ticks (S/rm/r_fire_night.log).

### 11. A campfire's light and sound

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/campfire.lua`, `game/data/config.lua`, `game/src/app.lua`

**Original:** A lit fire spawns light_sprite on the night layer at 250x250 with blend mode 8 (destination out) on a layer with its own texture, so it cuts a soft hole in the dark and the ground inside shows at full daylight (data.js placed instances; S/orig/fire_night.png). It also spawns light_color, 150 px, a red disc up to alpha 49/255 with AdjustHSL 10, on Color_effect over the night, there by day as well; both wait 170 s and fade over 10 s. 'fireplace_loop' plays at the fire at -5 dB, looped (8.10.3.24.2.1).

**Remake:** campfire.draw_glow adds the light_sprite disc in orange (1, 0.55, 0.2), 150 world px, at alpha darkness x 0.55 with a flicker, only when it is dark, over the multiply darkness: an orange haze on a dim blue ground, not a hole showing true colours (S/rm/r_fire_night.png; S/cmp/m_fire_night.png). The crackle is heard within 180 px at 0.35.

### 12. Rain

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** new weather system; `game/src/game/systems/survival.lua`, `game/data/sounds.lua`, `game/src/app.lua`

**Original:** Group 17.3. The 'Rain' timer fires every Rain_speed 360 s (2.11). If raining, it stops (17.3.1.1); then, if not raining, rains_level > 0 and not in a raid, choose(1,2,3,4) = 1 starts it (17.3.1.2), so each 6-minute period rains one time in four. Level 0 sets rains_level 1 (3.8.1). While it rains:
- 4 rain_particle every 0.4 s in screen space (Debug_panel layer, over the night), scale 1-2, 700-1500 px/s at 80-110 degrees, and a rain_ground splash within 300 px of the player (17.3.2.1.1);
- rain_fade, rgba(81,112,173) at alpha 50/255, over the whole view on the GUI_controls layer, so the HUD bars and buttons are tinted too; fades in over 10 s and out over 10 s (data.js Fade props);
- 'rain_loop' at -5 dB outdoors and 'rainroof' under a roof, swapped as the player goes in and out (17.3.1.2.1-2, 20.2.1.1.3, 20.2.1.2.2);
- Player_temperature -Rain_cold (1/2/3 by difficulty) outside a warmzone, unless the worn outerwear is the raincoat in condition (6.3.5). Measured on Regular in the start kit: -2, heat 73 to 71 in 10 s.
S/orig/rain_3s.png, rain_13s.png, rain_night.png; S/orig/rain.out.

**Remake:** No weather. Progress 12 says "rain_loop still waits on weather". The art (rain_particle, rain_ground, rain_fade) and the sounds (rain_loop, rainroof) are in Assets.

### 13. Snow, and the turn to winter after 5000 s

- **Verdict:** make identical  **Effort:** M (on top of 12)  **User's ask:** none (Stage 26 asked for the ground to change going east, not for snowfall)
- **Files:** the weather system of 12; `game/src/map/bands.lua`

**Original:** With rains_level 2 (levels 3 and up, 3.8.4-3.8.6.1, and on every level once Stats_time_lived reaches 5000 s, about 2.3 days, 23.4) the rain is snow: 2 particles every 0.4 s, frame 1, scale 1-3, 50-150 px/s (17.3.2.2.1); snow_ground settling within 300 px; rain_fade frame 1, white at alpha 50/255; 'blizzard' instead of rain_loop and no roof sound; the same Rain_cold. Levels 4 and up also have Day_cold 9. Measured: S/orig/snow_3s.png, snow_15s.png.

**Remake:** None. The east's snow is ground tiles only, with no flakes, tint or sound (S/rm/r_snowband.png; S/cmp/m_weather.png). A natural mapping would be snow instead of rain in the frosted, dry and snow bands (the original's lvl3-lvl5 sheets), and everywhere after 5000 s.

### 14. The east gets colder, the ground turning to snow

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 26 "On the left side of the map, there should be a beach are. Players spawns on the beach. Traveling right slowly gets colder and eventually the tiles change, art is in sprites."
- **Files:** `game/src/map/bands.lua`, `game/src/game/systems/survival.lua`

**Original:** One map per level; later levels are colder through Day_cold 9 (levels 4+) and snowfall (levels 3+), not across a map. The beach and the spawn on it are the original's own (S/orig/nov_start.png).

**Remake:** cold_east 1.0 a tick at most, by bands.cold(x), on top of the night's, with clothes keeping off their share (survival.east_cold). Measured in the snow at noon: heat 100 to 98 in 10 s (S/rm/r_snowband.log). When 5 is made identical, this term would become points in Player_temperature (for example up to 2 points, -1 a tick, naked).

### 15. "It's getting colder" over the head

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 26 asked for the cold, not a line)
- **Files:** `game/src/game/systems/survival.lua`

**Original:** No such line. Its cold lines are yellow "It's pretty cold." every 20 s while 0 < heat <= 25 (6.2.15.1, log id 80) and red "I'm freezing." every 10 s at heat 0 (6.2.10.1, log id 4); see hud.md 29.

**Remake:** survival.note_band says "It's getting colder" the first time the feet reach a colder band (Progress 26.4.2).

### 16. A run starts at 06:00

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/state.lua`

**Original:** Timer_ALL is 360 at load and generate_array_locations sets it to 361 on level 0 (3.8.1); measured 06:07 at 9 s alive (S/orig/nov_start.png). The first night falls 27 minutes into a run. Each level change adds 90 minutes (3.8.2-3.8.6).

**Remake:** time.minutes = 8 x 60, 08:00 (state.lua 26); measured minute 480 at the start (S/rm/reg_rates.log). The first dark is 22.5 minutes in.

### 17. When it gets dark: a linear hour after midnight and before six

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/daynight.lua`, `game/data/config.lua`

**Original:** The night layer is shown at 00:00 (17.1.3.1) and hidden at 06:00 (17.1.9.1). night_overlay's opacity is minutes x 1.69 through 00:00-00:59 (17.1.1), 100 from 01:00 to 04:59 (17.1.4), (60 - minutes) x 1.69 through 05:00-05:59 (17.1.2): linear, nothing before midnight. Measured opacity 0.29 at 00:17, 0.78 at 00:46, 1 at 02:02, 0.49 at 05:31 (S/orig light dumps). Sand at a fixed spot: noon (179,169,137), 23:31 (179,168,137), 00:17 (133,126,110), 00:46 (55,51,64), 02:02 (21,19,44), 05:31 (102,94,91).

**Remake:** A smoothstep across a 60-minute twilight centred on sunset 23:30 and sunrise 05:30 (daynight.night_factor_at), so it darkens from 23:00 and is fully dark from 00:00. Sand: noon (179,170,138), 23:31 (120,120,113), 00:17, 00:46 and 02:02 (81,87,97), 05:31 (140,137,121). S/cmp/m_dusk_2331.png, m_0017.png, m_dawn_0531.png.

### 18. How dark the night is, and its colour

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/daynight.lua`, `game/data/config.lua`

**Original:** night_overlay is a 2x2 sprite of rgba(0,0,34) at alpha 230/255, stretched over the view (device + 200) and pinned to the camera, on the night layer over the trees and the hp bars and under the HUD (2.13, 17.1.3.1). At full night the world is a navy veil: the sand at 12% of noon and blue-black, the player hard to make out (S/orig/o_0202.png).

**Remake:** A multiply toward (0.30, 0.38, 0.62) by max_darkness 0.78 (config.lua daynight; Progress 11 calls pitch black "a black screen"): the sand at 48% and grey-blue, the player and a zombie plain to see (S/rm/r_0202.png; S/cmp/m_night_0202.png). The HUD is undimmed in both.

### 19. Dusk and dawn tints

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/daynight.lua`

**Original:** At 23:30 (Timer_ALL 1410, 17.2.8) a dawnsunset sprite, frame 0 rgba(255,119,34) at alpha 15/255, is stretched over the view on Color_effect (over the night) and pinned to the player; at 05:30 (330, 17.2.9) frame 1, rgba(238,187,17). Its Fade fades in over 65 s and out over 25 s, then it is destroyed (data.js: [1, 65, 0, 25, 1]); measured opacity 0.04 rising to 0.10 over 3 s at 23:31. Sleeping destroys it (12.1.4.15).

**Remake:** No tint: the multiply darkens grey-blue straight from the day (daynight.draw_world_overlay).

### 20. Lights in the dark: fires, flares, the headlamp, street lamps, NVG

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/src/game/systems/daynight.lua`, `game/src/app.lua`, `game/data/items.lua`, `game/src/game/prop.lua`

**Original:** The night layer has its own texture, and lights are sprites on it with blend 8 (destination out) that cut it away: a lit fire's 250 px light_sprite (8.10.3.24.2.1); a flare's, for 100 s (8.9.1.1); the headlamp's, pinned to it when 'Toggle torch' is used, its battery var#3 falling 0.1 every 0.5 s (12.10.17.39, 8.14); light_tower cones from street lamps (pillars, 3.5.7.1.4.2-4); a muzzle_flash on every night shot (8.6.2.x) and explosions. NVG puts light_nvg_color_tiled over the view and halves the night layer's opacity (12.10.17.40, 17.7.3.1). There is no light round the player himself.

**Remake:** One multiply rectangle with nothing cut out of it; only a lit campfire adds a glow (finding 11). The headlamp and NVG are wearables with warmth and no light (data/items.lua 593, 597); flares are inert (inventory.md G4); pillars carry no lamp light. Lights cut out of the dark need a canvas, which costs a phone something; the original does it every night.

### 21. At night the reticle is over the dark and the aim lines under it

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stages 20 and 22 asked for the cone and when it shows, not how it sits under the night)
- **Files:** `game/src/app.lua`, `game/src/game/systems/status.lua`

**Original:** gui_target lives on Sunset_Dawn (layer 48), above night (46), and is drawn at full strength; the aim lines are placed on Rain (44), under it (globals.txt layer list). At 02:03 the red ring stands out bright where the zombie itself is barely visible (S/orig/night_target.png).

**Remake:** The reticle is drawn under the night overlay and dims with it; the aim cone is drawn over it at full strength (app.lua 1101-1115; S/rm/r_night_target.png; S/cmp/m_night_target.png).

### 22. Day and night ambience

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ambience.lua`, `game/data/sounds.lua`

**Original:** At 00:00 (17.1.3.1) 'MD_Winter_night_amb' starts on levels 0-1 ('night_loop' on level 2 and up) and 'day_loop' stops; at 06:00 (17.1.9.1) 'ambient' starts ('MD_winter_amb' on level 3 and up) and the night loop stops. Both at 0 dB, switched outright.

**Remake:** 'ambient' by day and 'night_loop' by night, cross-faded along the darkness curve, each at most 0.5 (ambience.update). No winter beds.

### 23. Sleeping

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`, `game/src/game/systems/daynight.lua`, `game/data/ui/hud.lua`

**Original:** gui_sleep shows from midnight to 06:00 (17.2.5, 17.2.7; button covered by hud.md 13). Its pad (12.1.1) says "You can sleep in the night time. If you in good condition you can sleep until morning 06:00. You will wake up earlier if hunger or thirst will be too high." and "Current resources allows you to sleep until HH:MM", with 'Go to sleep'. It refuses (12.1.3, l_eng_new.xml) by day "I can't sleep in daytime.", outside a room "I need a roof at least for sleeping.", when Player_temperature < 0 "It's too cold here to sleep.", with food or water <= 10 "Currently you too hungry or thirsty to go to sleep.", when sick or bleeding, and with bandits within 300 px. Sleeping fades to black over 1 s, applies every 5 s tick of the skipped time at once (drain, warmth, regen, sickness, 12.1.4.2), advances the clock to 06:00 or until food or water would reach 10 (12.1.5), burns fires down, and runs at 0.33 timescale with the sound at -15 dB for 1 s. Measured on Regular in a city house: 00:02 to 03:26, food and water 71 to 9, heat 77.5 to 100 (S/orig/sleep_btn.png, sleep_pad.png, sleep_fade.png).

**Remake:** No sleeping.

### 24. How cold sickness is caught

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 23 "cold sickness from staying out too cold" (either rule fits these words)
- **Files:** `game/data/config.lua`, `game/src/game/systems/survival.lua`

**Original:** At heat <= 0, each 5 s tick, random(50) > 28 (44%) makes the player sick unless immune (6.2.2.14). Measured on Regular: not sick 12 s after heat was set to 0, sick 42 s after (S/orig/reg_freezing.png, reg_freezing2.png).

**Remake:** Heat under 20 (sick_cold_at 0.20) for 60 s on end (sick_cold_time), one warm tick restarting the count (survival.run_tick; Progress 23.8.1 replaced the 44% roll). Measured: not yet sick after 43 s at heat 0 (S/rm/reg_rates.log).

### 25. Cold sickness passes after 90 s at 75% food, water and heat

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 23 "cold sickness from staying out too cold and curable by keeping food water and heat above 75% for some time."
- **Files:** `game/data/config.lua`

**Original:** With food, water and heat >= 80 and not bleeding, choose(1,2) = 1 cures it each tick (6.2.2.15), so it passes in about 10 s once all three are at 80; tetracycline cures it at once (8.10.3.40). It costs 1 food and 1 water a tick and stops regeneration (6.2.2.12, 6.2.2.10), as in the remake.

**Remake:** All three >= 75 for 90 s on end (sick_cure_at, sick_cure_time).

### 26. Tetracycline does not cure cold sickness

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 23 "effects, like infection, which needs to be cured by tetracycline. ... cold sickness ... curable by keeping food water and heat above 75% for some time" (it does not say whether the pill also cures the cold's sickness)
- **Files:** `game/src/game/systems/survival.lua`, `game/data/items.lua`

**Original:** The original has one sickness, var#35, from the cold, raw meat and fish (8.10.5.2-3, 8.10.5.17-20), acid (6.9.1.1) and spitter rounds (15.2.1.1). Tetracycline (item 46) takes 3 s and clears it (8.10.3.40).

**Remake:** survival.cure clears only an infection; a pill with no infection is refused (Progress 23.7.3).

### 27. Immunity after tetracycline

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`, `game/data/ui/hud.lua`

**Original:** Tetracycline sets var#51 immune for 300 s (8.10.3.40, 8.10.10): no sickness from the cold, raw food, acid or spitter rounds meanwhile, and gui_sick plays its 'immune' frame (6.3.27).

**Remake:** No immunity (survival.cure).

### 28. Cold sickness wears a blue or red arrow

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 23 asked for the sickness, not a badge)
- **Files:** `game/data/ui/hud.lua`, `game/src/game/systems/survival.lua`

**Original:** gui_sick alone, its 2-frame 'sick' animation at 1 fps (6.3.25; hud.md 31). gui_heat_arrow is destroyed at layout start (2.11.4) and never shown.

**Remake:** The skull carries a half-size gui_heat_arrow badge: blue down while it holds, red up while the cure is counting (hud.lua icon_sick_chilled, icon_sick_mending; Progress 23.8.3; S/rm/r_freezing.png).

### 29. Infection from bites, on VETERAN

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 23 "effects, like infection, which needs to be cured by tetracycline"; Stage 26 "limit the infected status to only hard mode"
- **Files:** `game/src/game/systems/survival.lua`, `game/data/config.lua`, `game/data/ui/hud.lua`

**Original:** Bites never infect (Player_get_hit, 15.3); there is no infection.

**Remake:** 12% a landed bite through armour, VETERAN only, 1 hp a tick and no regeneration until tetracycline (config.lua survival; Progress 23.7). Its icon is the radiation trefoil as a stand-in (gui_radiation/default_2; art wanted, Progress 23.7.5). If Legend is added (menus.md 19) it is hard mode too.

### 30. Smoking stops hunger and thirst

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/systems/survival.lua`

**Original:** Cigarettes (item 120) need a fire in reach or matches or a lighter (Check_fire_cigarets, 8.10.1), take 2 s, play 'Smoking_1/2' and start 'Smoked' for 60 s (8.10.3.105.1). While var#52 is set the tick takes no food and no water (6.2.2.1/6.2.2.2), a puff rises every second (21.3), and gui_smoke shows on the water or food plate (2.11.2.1, 2.11.3.1).

**Remake:** Cigarettes are inert (inventory.md G4).

### 31. Adrenaline's crash to 25 hp

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/src/game/systems/survival.lua`

**Original:** The adrenaline syringe (item 45, the morphine sprite) sets hp to 100, gives +20 px/s for 10 s, adds 50 hp every second for those 10 s, and then caps hp at 25 (8.10.3.39.1, 6.18, 6.19): a burst of health that leaves you at a quarter.

**Remake:** Morphine heals 35 (inventory.md G3); no speed and no crash.

### 32. Blood drops: one a second, turned, 15 px down, fading after 50 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 20 "when bleeding, add blood drops that stay on the ground" (keeping them stays; combat.md 56)
- **Files:** `game/data/config.lua`, `game/src/game/systems/effects.lua`

**Original:** Each bleeding second (6.2.5) one blood_drop on items_on_ground, a random one of its 3 frames, at a random angle, 15 px below the player's centre; it waits 50 s and fades over 1 s (data.js Fade [1, 0, 50, 1, 1]). Bleeding costs 0.5 hp a second (identical).

**Remake:** drop_every 0.5 s, two a second, unrotated, at the feet with 3 px of scatter, kept up to 120 (config.lua effects).

### 33. An empty meter costs health only once the drain takes it below zero

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`

**Original:** food -= rate; if food < 0 then food = 0 and hp -= 1 (6.2.2.4); the same for water (6.2.2.6) and heat (6.2.2.8). A tick that lands exactly on 0 (75 in steps of 0.25 or 1 always does) costs nothing; the next one does.

**Remake:** run_tick clamps first and then charges at <= 0, so the tick that lands on 0 already costs 1 (survival.lua 765-779).

### 34. Low-health grey

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "Make screen gradually gray when health is low. 50% monkchrome when below 10% health. effect doesn't start until under 75% health."
- **Files:** `game/src/game/systems/render.lua`

**Original:** Update_Stats sets the layout's Grayscale to 50 - hp while 0 < hp < 50 (6.3.16), else 0. (combat.md 55 says the original has none; it has this.)

**Remake:** From 75% down to 50% grey at 10% (hud.md 45).

### 35. Bleeding lasts 60 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`

**Original:** A bleeding bite starts the 'Bleed' timer at 60 s (30 with the metabolism perk; 15.3.5.2); its end clears var#22 (6.10). Also combat.md 38 and zombies.md 34.

**Remake:** bleed_duration 30.

### 36. Other things that hurt: burning rooms and acid

- **Verdict:** make identical  **Effort:** L (follows molotovs, inventory.md G4, and spitters, zombies.md 4)  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`, `game/src/game/systems/combat.lua`

**Original:** Inside a room a molotov has set burning (warmzone var#4, 8.9.2.4.3): -5 hp every 1.5 s with the grey Hit_effect (6.2.17). In a spitter's zed_bio_splash: -5 hp every second with Hit_effect, 50% a second to fall sick, death reason "Acid" (6.9). Spitter rounds also make you sick (15.2.1.1). Death reasons tracked: Hunger, Thirst, Cold, Bleed, Acid, Bullet, Melee, Zed.

**Remake:** None of these: no molotov, no spitter, no acid; death reasons hunger, thirst, cold, bleed, zed, bullet, infection (survival.lua REASONS).

### 37. Radiation and the red zone

- **Verdict:** unclear  **Effort:** L  **User's ask:** none (Progress "Explicitly out of scope: PUBG / tower-defense / S.T.A.L.K.E.R. game modes" is the remake's own decision)
- **Files:** `Progress.md`

**Original:** Both are game modes picked from the New game list (Menu_Events 3.9) and run as seed groups. STALKER (2.2): rad_zone points 400-600 px across, strength random(1,2-3) + level; standing in one plays rad_sml/med/high clicks and adds 0.5-2 radiation a second by strength, less with a positive Player_temperature (2.2.2.1); every 5 s hp falls by radiation/25 and radiation by 0.5 (2.2.2.3); gui_radiation, the rad bar and '+N' sit on a fifth plate (2.11.5, 2.2.2.2). PUBG (2.8): a red_zone_core that bombs a random point every 2 s after 20 s (red_zone_explode, rounds from the blasts), red_zone_minimap, and a shrinking safe zone costing 1-4 hp a second outside it, with the grey flash and a shake (2.8.1.4.3.1).

**Remake:** No game modes, no radiation; gui_radiation's trefoil is reused for the infection icon (finding 29).

## Already identical

- Meters 0 to 100 for hp, food, water and heat, and hp starting at 100.
- The survival tick: every 5 s, all four meters on it; bleeding per second at 0.5 hp (6.2.5 = bleed_damage 0.5, bleed_tick 1).
- An empty food or water bar costs 1 hp a tick; heat at zero 0.25 hp a tick (6.2.2.4-8 = starve_damage, dehydrate_damage, freeze_damage), apart from finding 33's first tick.
- The regeneration gate: food, water and heat all >= 50, not bleeding, not sick (6.2.2.10 = regen_threshold 50 and run_tick's test); the remake also excludes its asked-for infection.
- Sickness costs 1 extra food and 1 extra water a tick and stops regeneration (6.2.2.12 = sick_drain 1).
- A bite opens a wound one time in four (15.3.5.2 = bleed_chance 0.25).
- Raw steak: +25 food, +10 water and sick one time in two (8.10.5.2, item 60 = data/items.lua raw_steak's food 25, water 10, sicken 0.5).
- The clock: one game minute per 1.5 s (17.2), a 1440-minute day of 36 real minutes, the day counter going up at midnight (17.2.5), night as hours 0-5 for anything that asks (Day_mode, daynight.is_night).
- No light round the player at night in either; the HUD is never darkened by the night; the night covers the trees, buildings and the infected's hp bars in both (layers 41-44 under 46; render.npc_bars before the overlay).
- No fog in either: neither the events nor the remake have any.
- The pause holds the meters and the clock (the original's Options sets timescale 0, 8.19.1 and 12.9.1.1.2; the remake holds them in the settings window, app.lua 802-823); the inventory and the map do not (the original changes no timescale for them; in the remake only app.open_settings sets the held "paused" mode, app.lua 580-587, and the map says so, app.lua 603).
- Heat is capped at 100 and floored at 0.
- A beach down the west edge with the spawn on it (S/orig/nov_start.png): the user's Stage 26 ask happens to match the original.

## Not checked

- The headlamp, NVG, flares and street-lamp light cones were read from the events and data.js only; none was switched on in a run.
- The rain-sound swap under a roof and the blizzard loop were read, not heard; headless runs here play no audio.
- The 5000 s turn to snow (23.4) and the snow levels' Day_cold 9 were read, not reached in a run (rains_level was set directly for the snow shots).
- The remake's room warming (+1.5 a tick to 80) was not re-measured live; it is taken from config.lua and Progress 24.3's captures.
- The original's in-car warmth (+4) and car explosions: vehicles are out of scope.
- Perks that change these numbers (camel, hamster, regen, snowborn, sweethome, metabolism, eye): perks are out of scope.
- The tutorial's refill at 5 hp (6.2.4) and the pause of the tick while a tutorial hint shows (6.2.2): no tutorial in the remake yet.
- The TD and casual modes' survival rules (2.9.3, 6.2.18), beside the STALKER and PUBG ones in finding 37.
- Phone sizes: everything here was at 1280x720 in both.
- The original's darkness was measured from screenshots in headless Edge with WebGL through SwiftShader; a real GPU may blend the destination-out holes with softer edges.
