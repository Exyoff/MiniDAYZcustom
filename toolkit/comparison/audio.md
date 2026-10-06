# audio: the remake against the official 1.0

*Area surveyed:* Audio: music and ambience (the menu's, day and night, rain, indoors), UI sounds (buttons, the bag, the notebook), footsteps, weapons, creatures, pick-ups, eating and drinking, doors, fire; for every sound event in the original's events, the file, the trigger, the level, whether it is placed in the world, its tag and falloff and how often it can repeat; against the remake's `data/sounds.lua`, `src/core/audio.lua` and every caller of them. Plus the engine around them: distance, panning, pitch, voice limits, the listener, what happens when the page is hidden.

Surveyed 2026-10-06 against the remake at 75d9a88 (Stages 27.1 to 28 merged), in the read-only worktree `wt/survey`, and the published web build.

Logs are under `S = C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/audio`:
- `S/orig_play_table.tsv`: all 985 Play actions of 1.0 (sheet, event path, action, folder, file or choose(), loop, dB, object or position with cone, tag); `S/orig_audio_actions.txt` all 1,069 audio actions; `S/file_sets.txt` which files each game plays.
- `S/orig/o1_menu_run.log` to `o6_hidden.log` and `S/orig/*.png`: the original in headless Edge, through `S/origA.cjs` (a copy of `../orig.cjs` that installs `S/audiohook.js` before the page loads). The hook records every file fetched and decoded and every `AudioBufferSourceNode.start`: the file, loop, the gain node's value, the panner's position and model, the listener's position and the playback rate. `S/dump.js` reads it back.
- `S/rm1`, `S/rm2`, `S/rm3` (`LOVE/minioutbreak/run.log`): the remake, `lovec.exe game --shot --res 1280x720 --touch off` (`--world 7` for rm1 and rm3), with `audio.play`, `audio.play_at`, `audio.ambience` and `audio.loop_at` wrapped from the console. Each call is logged with the frame and the volume the remake actually gives the source after master and distance (`SND`, `AMB`, `LOOP` lines). The commands are in `S/rm_cmds.json` and the scripts in `S/rm_script*.txt`.
- `S/web/w1_hidden.log`: the published build at exyoff.github.io/MiniOutbreak through `S/webA.cjs` with the same hook.
- `S/pan_measure.log` (`S/pan.cjs`): both games' panner set-ups rendered in an OfflineAudioContext in Edge.

## Summary

**What I read.** The original: all 1,069 audio actions in the six sheets (Game_events 1,053, Menu_Events 13, Login_events 3). In context, the groups behind them: menu 3.1/3.2, run start 2.11-2.13, footsteps 7.8, reloads 8.3, firing 8.6, melee 8.7, item use 8.10, pick-up and drop 8.11-8.13, the pad 12.6, doors 12.13.1 and 12.13.3.1.4, build mode 12.8, the DAY label 12.16, NPC damage 13.2, zombies 13.3, animals 13.5, noise 13.6, player damage 15, obstacle hits 16.1, day and night and rain 17.1/17.3, fires 18.2, the death screen 10. Also the Audio plugin's nine properties in `data.js` (t187: `[0,0,0,1,1,600,600,10000,10]`), what `c2runtime.js` does with them, and `media/`. `show.py` fails on Windows (`signal.SIGPIPE` does not exist there), so the event dumps were grepped directly. The remake: `data/sounds.lua`, `src/core/audio.lua`, `data/config.lua` audio, campfire and interiors, `systems/ambience.lua`, `ai.lua`, `fauna.lua`, `combat.lua`, `door.lua`, `player.lua`, `campfire.lua`, `crafting.lua`, `build.lua`, `app.lua`, `tools/web_template/index.html`, love.js's OpenAL (`dist/web/love.js`), Progress Stage 12, 20.6.1, 22.1.4, 28.1, 28.2.

**What I ran.** In the original, Novice at 1280x720 with touch, every sound recorded with its gain:
- the menu; the start of a run; a walk off the beach onto the grass;
- a zombie spawned beside the player, with its bites and the player's automatic punches;
- the notepad opened and shut; walking through a house door; midnight forced; a wolf spawned and biting;
- the bag opened; an FNX given with the game's own `Spawn_drop` and picked up; death and 8 s on the death screen;
- the page "hidden" and shown again.

In the remake:
- generated world 7 on Regular: footsteps on the beach, a zombie, the FNX fired, a wolf, midnight, a door toggled in the first house up the road;
- the test map: the map screen, fists and the AK-74 against a zombie, a lit campfire, death;
- the published web build: whether the page's audio stops when hidden.

**The biggest gaps.**
1. **Panning (new, measured).** In the web build every sound placed in the world comes out of one ear: a zombie 13 px to the right is heard only on the right. The remake meant a gentle pan: `pan_max` 0.85 in config. The original's HRTF panner, with its listener 600 px above the plane, separates a sound 300 px to the side by 2.2 dB.
2. **Levels.** The remake multiplies everything by a master 0.8 and gives each event its own volume. None of them follow the original's decibels:
   - footsteps 7 dB quieter, the day and night beds 8 dB quieter, a swing 6 dB quieter, the dry click 7 dB quieter;
   - a round or blow landing on a zombie 10 dB louder;
   - the campfire 6 dB quieter at the fire, 16 dB quieter at 120 px, and silent past 180 px.
3. **Engine behaviour.** Audio keeps playing when the page is hidden; the original suspends it. The ears are the player's, not the camera's. A sound placed at a moving zombie stays where it started. Every sound gets a random ±6% pitch the original never has.
4. **Wrong cues.** Doors play at -5 dB at the doorway with three extra doors counted as metal. Lighting a fire plays the cigarette's match strike. Placing a campfire plays a file 1.0 never plays. The death screen silences the ambience, which the original keeps. A wolf's bite is one snarl where the original plays two snarls and three thuds.
5. **Missing sounds.** Crows' caws and tree felling (chop, treefall) are found here. Already reported elsewhere and cited below: pick-up, drop and use sounds; the reload stages and bolt cycles; per-gun shots; rounds into walls; rain and snow; the winter night bed; the menu bed; the special infected kinds.

**What is kept and why.** All five audio asks are kept; the surveys that judged them are cited below.
- The volume slider in place of "Sounds: ON/OFF" (Stage 17; menus 24).
- A hit sound on every hit and on the player's damage (Stage 20). Its level on a zombie is a make-identical finding in zombies 23 and combat 25; on the player it already matches.
- Zombies groaning while wandering (Stage 22; zombies 24).
- A zombie's attack sounding like a hit (Stage 28; zombies 20).
- The wolf's sounds for animals (Stage 28): the remake's wolf uses the original's own wolf_spot and wolf_attack, so this ask matches the original.

Loading sounds ahead on the start menu came from Stage 20's "Game stutters on mobile browser". Folding world sounds to mono is OpenAL's need. Both are "platform".

**Covered in other surveys, cited not repeated.** These are listed in the table under "Reported in other areas", each with what this survey's captures add to it.

## Findings

Verdicts: make identical 13, unclear 1, platform 1 (the asks above are kept in the surveys cited; none is new here)

How the original's levels read: C2's volume is in dB, gain = 10^(dB/20). 0 dB is 1.000, -5 dB 0.562, -10 dB 0.316, -15 dB 0.178. Every gain below was read off the running game's gain nodes. The remake's figure is the source's volume after `master` (0.8), the event's volume and distance, as `audio.recent()` records it.

### 1. Every sound placed in the world is hard-panned in the web build

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/core/audio.lua` (`set_pan`, `attenuate`), `game/data/config.lua` (`audio.pan_width`, `audio.pan_max`)

**Original:** The Audio plugin's panner is HRTF with the inverse distance model (t187 properties 3 and 4). The listener sits 600 px above the ground plane at the camera: `listener.setPosition(x, y, -600)`, forward (0,0,1), up (0,-1,0), from property 5 in `c2runtime.js`. A sound's direction is therefore atan(dx/600): 26.6° at 300 px to the side. Rendered with exactly these settings, a tone 50, 130, 300 and 600 px to the right of the listener comes out 0.4, 0.9, 2.2 and 3.8 dB louder on the right than on the left (`S/pan_measure.log`). Nothing is ever in one ear only. The captures show every world sound going through that panner, e.g. `g1.000>P[1058,2873,0]Hi600/10/10000` for a zombie's spotted groan 159 px to the right (o2).

**Remake:** `set_pan` gives a mono source the relative position (pan, 0, 0), with pan = dx / 260 × 0.85 and OpenAL's rolloff at 0. OpenAL pans by direction, not by distance. love.js passes that position unchanged to a Web Audio PannerNode, which is equal-power because the context has no HRTF (`dist/web/love.js` `updateSourceSpace`, `updateSourceGlobal`). (pan, 0, 0) is 90° to the side for any pan that is not exactly 0. Rendered with love.js's own geometry, pan 0 gives left 0.500 and right 0.500, but pan 0.05, 0.2, 0.425 and 0.85 all give left 0.000 and right 0.707 (`S/pan_measure.log`). So every groan, howl, snarl, door and hit that is not dead centre is heard in one ear only. The config's "Short of 1: a sound entirely in one ear reads as a bug in headphones" never takes effect. Desktop LÖVE's OpenAL also places by direction (not measured).

### 2. The ears are the player's, not the camera's

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 20 asked for the focus button; where the ears are is not part of it. The camera's own motion is character 12)
- **Files:** `game/src/app.lua` (998), `game/src/game/player.lua` (317), `game/src/core/audio.lua`

**Original:** `Audio.SetListenerObject(camera)` at run start (2.13). The camera is pinned to the player. While the zoom button holds a target, it moves halfway to that target: `MoveAtAngle(..., distance / 2)` at 8.5.6.1 and 8.5.8.2.1. The plugin moves the listener to its object every tick (`Ra.ya`). Measured while walking: the listener's x and y equal the player's at every footstep (o2).

**Remake:** `audio.set_listener(player.x, player.y)` every frame, whatever the camera does. The focus key (Z) and button move only the view.

### 3. A sound at something that moves stays where it started

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/core/audio.lua` (`play_at`, `emit`), `game/src/game/systems/ai.lua`, `game/src/game/systems/fauna.lua`

**Original:** 577 of the 985 Play actions are PlayAtObject / PlayAtObjectByName. These attach the sound to the object, and the runtime re-places it every tick (the instance's tracker in `c2runtime.js`). A running zombie's groan, a wolf's howl and a thrown flare's burn move with their source.

**Remake:** `audio.play_at(name, x, y)` takes a point. Its gain and pan are set once, when the sound starts (`emit`). Only `audio.loop_at`, the campfire's crackle, is re-placed every frame.

### 4. Every sound's pitch is randomised by up to 6%

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua` (`audio.pitch_var`), `game/src/core/audio.lua` (`pitch_for`)

**Original:** No audio action changes the playback rate: there is no SetPlaybackRate among the 1,069. Every start in the five captures played at rate 1 (o1-o5, last column).

**Remake:** `pitch_var` 0.06. Every play is pitched at random between 0.94 and 1.06: footsteps, shots, groans, the notebook's rustle, the body thud.

### 5. The mix: a master of 0.8, and volumes that are not the original's decibels

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the player's own volume slider, Stage 17, is separate: it is the listener's volume and stays)
- **Files:** `game/data/config.lua` (`audio.*_volume`, `campfire.sound_volume`), `game/src/core/audio.lua`, the callers that pass a volume

**Original:** The master is 0 dB. Each Play action carries its own dB value. Measured gains at the source, before distance:

| Sound | Original | Remake (after master 0.8) | Difference |
|---|---|---|---|
| The player's gunshot (FNX, AK-74; other guns' offsets in combat 21) | 0 dB, 1.000 (8.6.2.3.4.1, 8.6.3.1.1.1.6.1.2) | `gunshot_volume` 0.9: 0.72 (rm1, rm2) | -2.9 dB |
| Dry click, empty or ruined gun | `empty_click` 0 dB, flat (8.6.2.2.1.1-2) | `reload_volume` 0.55: 0.44 | -7.1 dB |
| Reload | stages -5 dB, shotgun shells 0 dB (8.3) | `reload_volume`: 0.44 | -2.1 / -7.1 dB |
| A swing, fists or weapon | `Punch_Swipes_0/1` 0 dB at the player, measured 1.000 (o2, o4) | `melee_volume` 0.6: 0.48 (rm2) | -6.4 dB |
| A zombie spotting you | 0 dB at the zombie, measured 1.000 (o2) | `zombie_volume` 0.85: 0.68 at 11 px (rm1) | -3.3 dB |
| A wolf's howl and snarl | 0 dB, measured 1.000 (o2, o5) | `zombie_volume`: 0.68 (rm1) | -3.3 dB |
| The notebook's `pad_open`, `pad_page` | 0 dB flat, measured 1.000 (o3) | 0.80 (rm2) | -1.9 dB |
| Cooking, `meat_cook` | 0 dB flat (8.10.3.53.2.1) | 0.80 | -1.9 dB |
| Lighting a fire (file: finding 9) | 0 dB flat | 0.80 | -1.9 dB |
| A bite's thud on the player | -5 dB, measured 0.562 (o2, o4) | `impact_volume` 0.7: 0.56 (rm1, rm2) | equal |

The rest is reported elsewhere with these captures' numbers added below:
- footsteps -5 dB against 0.24 (character 14);
- a hit on a zombie or animal -15 dB against 0.56 (combat 25, zombies 23);
- the day and night beds 0 dB against 0.4 (survival 22);
- the campfire -5 dB against at most 0.28 (survival 11);
- the menu bed -10 dB against none (menus 16);
- doors (finding 8).

**Remake:** A source's volume is `master` 0.8 × the event's volume × the distance gain (`emit`). The per-event volumes are round numbers chosen for the remake's own balance (config.lua audio's comments). Making it identical means master 1.0 and each event at 10^(dB/20) of its original value.

### 6. Sound goes on when the page is hidden

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the original does this in a browser on a phone and on a PC alike, so it is not a platform matter)
- **Files:** `tools/web_template/index.html`, `game/main.lua` or `game/src/app.lua` (a `love.visible` / `love.focus` handler)

**Original:** "Play in background" is off (t187 property 2 = 0). When the page is suspended, the plugin pauses every playing sound and suspends its AudioContext; it resumes both on return (`Ra.uG` in `c2runtime.js`). Measured in a run: a hidden-page event took the context from running to suspended, and showing the page took it back to running (`S/orig/o6_hidden.log`). Switching apps or locking a phone silences the game.

**Remake:** The page listens to `visibilitychange` only to persist saves (index.html 643). The game has no `love.visible` or `love.focus` handler. Nothing pauses a source or suspends OpenAL's context. Measured on the published build: the context stayed running through the same hidden event (`S/web/w1_hidden.log`). A hidden tab stops requestAnimationFrame, so what goes on sounding is whatever is already queued or looping. That was not measured on a phone.

### 7. The ambience stops when the player dies

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ambience.lua`

**Original:** Dead_screen (10.3) stops nothing. The day or night bed keeps playing under the death screen until "Go to menu" (StopAll 2.5 s after the tap, 10.4.2) or a restart (10.2). Measured: 8 s on the death screen with the day bed still playing, no stop, and no new sound (o4; `S/orig/dead.png`).

**Remake:** `ambience.update` fades both beds out when the player is dead: "Nothing should be playing over a menu or a death screen". Measured: both beds to nil at the death (rm2). The death sound the remake adds, `tutor_death`, is character 23.

### 8. Doors: their level, where they sound, and which are metal

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 23 asked for doors that open and shut and a door button; no sound was named)
- **Files:** `game/src/game/door.lua` (`set_open`), `game/data/sounds.lua` (`door`, `door_metal`), `game/data/config.lua` (`audio.door_volume`)

**Original:** Doors sound in two places, Doors_auto (12.13.1) and the door tap (12.13.3.1.4). Every door sound plays flat with PlayByName, never at the door:
- `open_door` and `close_door` at 0 dB;
- `open_door_metal` and `close_door_metal` at -5 dB.

Metal means the `firestation`, `ga_brown` and `ga_blue` leaves. The wooden sound plays for every leaf that is not "firestation", so the two garage doors play both, the metal at -5 and the wooden at 0. `firestation2`, `barrack` and `shtab` are wooden. Measured walking through a village door: `open_door` then `close_door` at 1.000, flat (o3).

**Remake:** `door.set_open` plays at the doorway's middle at `door_volume` 0.7, through the distance model and the pan of finding 1. Measured on a brown2 house door: `open_door` 0.56 and `close_door` 0.56 (rm3). `door_metal` lists ga_blue, ga_brown, firestation, firestation2, barrack and shtab, and every toggle plays exactly one sound.

### 9. Lighting a campfire plays the cigarette's match strike

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stages 24 and 26 asked for the starter kit and lighting from the inventory ("lighting a campfire should only be done by standing near the campfire and using the campfire lighter kit from the inventory"); no sound was named
- **Files:** `game/data/sounds.lua` (`fire.light`), `game/src/game/campfire.lua` (`begin_light`)

**Original:** Lighting a fireplace with matches (item 24, 8.10.3.24.2.1) or the lighter (item 125, 8.10.3.109.2.1) plays `match_burn` at 0 dB, flat, at the start of its 3 s. `matchstrike_2` and `lighter_0` are for lighting a cigarette (Check_fire_cigarets 8.10.1.2.6 and 8.10.1.3.6). So items 31's "3 s, `matchstrike_2` or `lighter_0`" for lighting a fire is a misreading: both play `match_burn`. Near the STALKER radio, a lit fire also brings `radio_noise` and then `guitar` (-10 dB, tag-gated) from the radio: an easter egg the remake does not have.

**Remake:** The starter kit's LIGHT plays `fire.light` = `matchstrike_2` (`campfire.begin_light`) at 0.8.

### 10. Placing a campfire plays the crafting rustle

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 24 "To place the campfire, there needs to be a build mode." asks for the mode, not a sound
- **Files:** `game/src/game/build.lua` (`place`, line 161)

**Original:** Setting the campfire kit down (item 23, 8.10.3.23.2) takes 3 s and spawns the fireplace with no sound. Build mode's own placements (12.8.3.1.2-7) are silent too, except the tent's `tentpack`.

**Remake:** `build.place` plays `craft` = `crafting`. No 1.0 event plays that file (inventory 53 for the recipes that also use it).

### 11. A wolf's bite on the player: two snarls and three thuds

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 28 "add animals, wolf chasing player or deer makes the current zombie attack noise". Both games use the wolf's own sounds; how many play per bite is not the ask
- **Files:** `game/src/game/systems/fauna.lua` (`bite`), `game/src/game/systems/combat.lua` (`hit_player`)

**Original:** A wolf's "punch" timer fires every 1.5 s (13.2.4.1). On the player it plays:
- a camera shake;
- Player_get_hit, whose body thud is -5 dB at the player (15.3.5);
- `wolf_attack` 0 dB at the wolf and a body thud -5 dB at the player (13.2.4.1.1);
- because 13.2.4.1.11 plays on every punch within Player_Hear_radius, another `wolf_attack` 0 dB and body thud -5 dB, at the wolf.

Measured on one bite, within 2 ms: body_2 0.562 (player), wolf_attack_2 1.000 (wolf), body_2 0.562 (player), wolf_attack_2 1.000 (wolf), body_2 0.562 (wolf) (o5). A wolf biting an infected or a deer gets 13.2.4.1.11's pair alone.

**Remake:** `fauna.bite` plays one `wolf_attack` at 0.68, and `hit_player` one body thud at 0.56 (rm1). On an infected it plays one `wolf_attack` and `record_hit`'s body thud.

### 12. Crows, and their caws

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** new: a crow kind and its placement (`game/src/map/`), `game/data/sounds.lua`

**Original:** Crows (`crow_skin`) are placed one to three at a time at map locations (3.5.5.1.2.8, .3.7, .4.8, .5.8 and more). They peck about, and every 4 s any crow within 300 px of the player takes off (13.5.1.2.1.1). So does one that hears a noise (13.6.1.10.1). A crow taking off (crow_fly, 13.5.1.1) flies off, fades, and caws after random(0-2) s: `crow_1`..`crow_5` at 0 dB at the crow. This is the original's only bird sound outside the ambience loops.

**Remake:** No crows: the only `crow` in `src/` and `data/` is in the generated atlas. `crow_1`..`crow_5` are never played.

### 13. Felling a tree: chop on each blow, treefall when it falls

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, the tree props (`game/src/map/`), `game/data/sounds.lua`

**Original:** A swing of the split axe or the fire axe can chop trees: 8.7.1.5.1.2.2.2.1 and .5 set the swing's var#7, and so does the interact button's chop, 12.13.3.1.8. A chopping swing that hits a tree's obstacle (16.1.6) plays `chop`..`chop4` at 0 dB at the blow. On the third blow (the tree's var#0, 3), `treefall_02`/`treefall_03` plays at -5 dB, flat. The tree turns to its fallen frame and drops its items (Spawn_drop 38 twice and 50; 37 as well from `tree_leaves_2`).

**Remake:** Trees cannot be felled. `chop`..`chop4` play instead as the axe's swing (character 19, combat 33), and `treefall` never.

### 14. A voice cap, a pool per sound and a 30 ms retrigger guard

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `game/src/core/audio.lua` (`claim`, `acquire`, `emit`), `game/data/config.lua` (`max_voices`, `pool_per_sound`, `retrigger_guard`)

**Original:** No cap in the events or the plugin. Every play starts its own source: the captures show two snarls and three thuds starting within 2 ms (o5). The only gates are tags, and only where an event asks:
- one `hard_ground` at a time (16.1.1.1.1);
- one screamer (13.3.7.4.2);
- one `guitar`;
- one bot's reload.

**Remake:** At most 24 voices: when full, the quietest gives way, and only to a louder one (`claim`). At most 6 copies of a sample (`pool_per_sound`). The same sample is dropped if it started less than 30 ms ago (`retrigger_guard`, real time). The captures show two body thuds dropped (rm2 frames 1037 and 1130), but a capture runs faster than real time, so its 30 ms covers more frames.

Unclear because the cap and the guard exist for the phone. Whether either ever drops a sound in real play was not measured, and the original has neither.

### 15. World sounds folded to mono, and loaded ahead on the start menu

- **Verdict:** platform  **Effort:** S  **User's ask:** Stage 20 "Game stutters on mobile browser" (20.6.1 loads sounds ahead to stop a first play's hitch)
- **Files:** `game/src/core/audio.lua` (`new_mono_source`, `warm`), `game/data/sounds.lua` (`warm`)

**Original:** Each file is fetched and decoded on its first play: `menu_click` was requested only when the DAY label asked for it (o1). Its PannerNode takes the stereo files as they are.

**Remake:** OpenAL places mono sources only, so a sound played into the world is folded to one channel (193 of the 274 files are stereo). The warm list decodes and folds them a slice a frame from the start menu on, so no first play stalls a phone. The world sounds lose their stereo width; that is the engine's way to place them, and keeps no sound from playing. Nothing to change, once finding 1 places them gently.

## Reported in other areas (cited, not repeated)

| Where | What | What this survey adds |
|---|---|---|
| menus 16 | The original loops `ambient` at -10 dB on the menu; the remake's menu is silent | Measured: `ambient` at 0.316, flat, from 4.4 s after the page loads (o1) |
| menus 24 | "Sounds: ON/OFF" against the volume slider (keep, Stage 17) | — |
| menus (Not checked) | Menu taps silent in both: `menu_click` is not in `media/` | Measured: `media/menu_click.ogg` 404 and its decode fails (o1-o5). So the DAY label's "menu_click at -10 dB" in **hud 7** is silent as well |
| hud 11 | The notepad's `pad_open` | Now identical but for the level: `pad_open` on opening and shutting and `pad_page` 0.1 s after opening in both (original 104 ms, remake 6 frames; o3, rm2) |
| character 14 | Footsteps: every 0.25 s at -5 dB by surface, against one stride sound | Measured: steps every 250 ms at 0.562. `sand_run` on the beach, then `run_1`..`run_4` (tile 6) on the ordinary level-0 ground, not `forest_run` (o2). The remake: `forest_run` at 0.24 every 20 frames on the beach as everywhere (rm1, rm2) |
| character 19, combat 33 | Every swing plays Punch_Swipes; the remake's axe plays `chop` | `chop` is the tree-felling sound (finding 13) |
| character 23 | No death sound in the original; the remake plays `tutor_death` | Measured: 0.8 in the remake (rm2); nothing in 8 s in the original (o4); finding 7 for the beds |
| combat 21 | Per-gun shot samples and offsets | The remake also picks `ak74_2` and `izh_3`, which no 1.0 event plays (`S/file_sets.txt`) |
| combat 22, 41 | Bolt and pump cycles; the reload's three stages at -5 dB | — |
| combat 25, zombies 23 | A hit on a zombie: body at -15 dB within 2000 px, against `impact_volume` | Measured 0.178 against 0.56 (o2, rm2). The same -15 dB holds for wolves, deer and rabbits (13.2.8.1-3), which the remake also plays at 0.56 |
| combat 30, zombies 22 | Every zombie screamed on death | Fixed since: `zombie.death` is empty (28.1.2) and no kill played one (rm2). Now identical |
| combat 32 | Rounds into walls: `hard_ground_1/2` | Also: only within Player_Hear_radius, and never two at once (the `ground_hit` tag gate, 16.1.1.1.1) |
| zombies 4 | The tank's, jumper's, screamer's and spitter's sounds, and the buried one's `deerrun` | — |
| zombies 20 | The blow: `attack_N` replaced by the thud (keep, Stage 28) | — |
| zombies 21 | `jumper_spot` in the spotted list | Fixed since (28.1.2). Now identical |
| zombies 24 | Wander groans (keep, Stage 22) | — |
| zombies 25 | The distance model: heard to 2000 px through inverse rolloff, against silent past 640 px | Measured in the remake: a spotted groan past 640 px never starts (rm3, frame 1366). Findings 1-3 for the rest of the model |
| inventory 53 | `crafting` on recipes, which no 1.0 event plays | Finding 10: also on placing a campfire |
| inventory 54-56 | No pick-up, drop or use sounds | Measured: `pick_pistol` 1.000 flat on picking up the FNX (o4). Two `gearpick` at 1.000, flat, in the first frames of every run, by their timing the start kit being put on (o1-o5) |
| items 31 | Matches and the lighter do not light fires (keep, Stages 24 and 26) | Its "`matchstrike_2` or `lighter_0`" for lighting a fire is the cigarette's; the fire's is `match_burn` (finding 9) |
| items 32, 46 | The radio; containers, the trunk's `open`, `bushsearch`, attaching | — |
| survival 11 | A campfire's light and sound | Only the nearest fire sounds in the remake, silent past 180 px. Measured at 60 px: 0.233 × distance × master, about 0.18 (rm2). The original plays every lit fire's loop at -5 dB from where it stands: 0.535 at 60 px, about 0.39 at 180 px, 0.26 at 300 px |
| survival 12, 13 | Rain (`rain_loop`, `rainroof` under a roof), snow (`blizzard`) | — |
| survival 22 | Day and night beds: 0 dB, switched outright, a winter night bed on levels 0-1 | Measured on level 0 at 00:00: `ambient` stopped and `md_winter_night_amb` started at 1.000 in the same millisecond (o2, o3). The remake: `ambient` 0.5 → `night_loop` 0.4, eased at 0.8 a second (rm1) |
| survival 23 | Sleeping (master at -15 dB for 1 s) | — |

## The files each game plays

- **The remake plays, 1.0 never does:**
  - `ak74_2` and `izh_3` (combat 21);
  - `crafting` (finding 10, inventory 53);
  - `attack_9`, in the asked-for wander groans.
- **The remake plays in another role:**
  - `tutor_death`: the tutorial's in 1.0, the player's death here;
  - `chop`..`chop4`: tree blows in 1.0, the axe swing here;
  - `matchstrike_2`: a cigarette in 1.0, the campfire here;
  - `night_loop`: 1.0's night bed on levels 2 and up only;
  - `glock19_single_*`: every unmapped gun here.
- **The remake lists but never plays:** `sand_run`, `concrete_run`, `road_run` and `snow_run` (the footsteps always take `footstep.default`), `rain_loop`, and `bottle_break_0` (`impact.glass`, which no caller records).
- **1.0 plays and the remake never does: 184 files** (`S/file_sets.txt`, grouped by event). By where they belong:
  - Use, pick-up and drop (inventory 54-56): `eatingsoft_0`, `eatingcrunchy_0`, `drinksoda_0`, `whiskey`, `bandage`, `vitamins`, `adrenaline_use`, `md_canteen_fill`, `smoking_1/2`, `lighter_0`, `match_0`, `match_burn`, `torch_on`, `tentpack`, `tear_fabric`, `taping`, `eject`, `planting_seeds`, `plast`, `nuko`, `pick1/2`, `pick_pistol`, `pick_rifle`, `gearpick`, `drop1/2`, `geardrop1/2`, `bow_reload`, `action_refuel_0`, `action_repair_0`.
  - Containers (items 46): `open`, `bushsearch`.
  - Guns (combat 21, 22, 41): 53 shot samples (the grenade launcher's `gl_shot` among them), 19 reload stages and 4 cycle sounds.
  - Rounds into walls (combat 32): `hard_ground_1/2`. Felling trees (finding 13): `treefall_02/03`.
  - Footsteps (character 14): `run_1`..`_4`, `swamp_step_1`..`_4`, `wood_1`..`_4`.
  - Special infected (zombies 4): `zed_tank_1/2/3`, `zed_tank_scream`, `jumper_jump`, `jumper_spot`, `spitter_spit`, `deerrun`.
  - Crows (finding 12): `crow_1`..`_5`.
  - Weather and winter beds (survival 12, 13, 22): `rainroof`, `blizzard`, `md_winter_amb`, `md_winter_night_amb`.
  - Traps and explosives (items 32 and the inert items): `trap_bear_0`, `f1_explode`, `gl_explode`, `smokegrenade`, `flare_throw`, `flare_burn`, `flaregun_shot`, `gren pin`, `fence_destroy_1/2`. (The molotov's `bottle_break_0` is in the remake's list as `impact.glass` but never played.)
  - Out of scope in Progress:
    - vehicles: `car_*`, the `uaz`/`volga` engines;
    - the bandit bots: section 13.4;
    - perks and achievements: `md_perk_1/2`, `md_achievment unlocked`;
    - the STALKER and PUBG modes: `rad_*`, in inactive groups 2.2 and 2.8.
  - Not surveyed here (world events): the bunker (`bunker_ambient`, `bunker_door`, `card`), the helicopter (`heli_engine`), the final day, the radio (`radio_noise`, `guitar`), the `pred` easter egg.

## Already identical

- The infected's spotted groan: `spotted_0`..`spotted_6`, once on acquiring a target (the player or an animal), at the zombie (since 28.1.2).
- No death cry for the infected; a killing blow is the body thud like any other.
- A bite on the player is the body thud at -5 dB / 0.56 at the player, once a second. The blow's own voice is dropped by the user's ask.
- The wolf: `wolf_spot`/`wolf_spot_2` on finding prey, a bite every 1.5 s with `wolf_attack`.._3 (count and level: findings 11 and 5). Deer and rabbits are silent in both, and so is flaying a carcass.
- Cooking a steak: `meat_cook` flat at the start of a 5 s use (level: finding 5).
- The map notebook: `pad_open` on opening and on shutting, `pad_page` 0.1 s after opening (level: finding 5).
- Opening and shutting the bag: silent in both (o4).
- The pause: sounds and beds play on in both. The original sets the timescale to 0, and its audio ignores the timescale (t187 property 0).
- A new run or a rebuild stops everything first (StopAll at 2.11; `audio.stop_all`).
- The player's own sounds are centred and at full gain in both: footsteps, shots, swings, the thud on the player. The original places them at the player, where its listener is; the remake plays them flat.
- A round reaching the end of its range is silent in both (16.2).
- Menu taps and the DAY label are silent in both (the missing `menu_click`).
- The day bed is the same file, `ambient`.

## Not checked

- Nothing was listened to. Both games were compared by the gains, pans and files their engines set, read from the running page's Web Audio nodes and from the remake's own record of each source.
- The remake's panning was rendered for the web build's Web Audio geometry. Desktop LÖVE's OpenAL Soft was not measured.
- What a really hidden tab or a backgrounded phone does with the remake's streamed beds (its main loop stops): only the context's state was read.
- The original's guns were not fired in a run: the attack button punched, since the picked FNX was not switched into the hands. Shots, reloads and cycles are from the events, and from the combat survey's FNX and AK-74 sessions.
- Rain, snow and the roof swap, crows, tree felling, the tank and the other special kinds, traps and explosives: read from the events, not triggered.
- The animated infected on the original's menu (Menu_Events 45.3.5 plays spotted groans there): none groaned in the 22 s on the menu in five runs.
- How Chromium's HRTF panner treats a stereo file (downmix or per-channel) in the original.
- Phone sizes: no audio path depends on the screen in either game.
- The remake's door sound on the metal styles; only a wooden brown2 door was toggled.
