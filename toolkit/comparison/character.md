# character: the remake against the official 1.0

*Area surveyed:* The player character: sprites and animations, held weapons, clothing layers, speeds, scale, camera

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

I compared the original (MiniDayZ+1.0 events and data, plus about 80 screenshots driven through JS state dumps) with the remake (Stage 26 code, 44 mid-run snapshots, montages side by side). The harness refused to write report.md ("subagents should return findings as text"), so the full report is this output. Evidence lives under the scratchpad (S = /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad). Original screenshots are in S/survey/character/orig. Remake snapshots are in S/survey/character/remake and are copied to S/env/s4/caps/char_*.png. Montages are S/survey/character/m_*.png. 1.0 frames cut for every wearable, weapon and skin are in S/survey/character/cut2. The sheet md5 diff is S/survey/character/sheet_diff.txt.

The four biggest differences:
1. Fourteen sheets the character wears or holds are the MiniDayZ+1.2 fan mod's art, not 1.0's. The helmets and gas mask look completely different and one backpack is missing frames. Across Assets/images, 62 sheets in all equal 1.2.
2. Weapons not in hand are never drawn. The original shows the slung rifle, the pistol at the hip and the axe on the back in every pose.
3. With a gun lowered, the original's body plays twohand; the remake's plays idle.
4. The camera differs:
   - the world is 1.22x larger on the phone and 2.25x at 1280x720, where the original is 1 world px per CSS px;
   - there is no 2x zoom inside buildings;
   - a 4 px follow lag;
   - the camera centres the feet instead of the body;
   - the focus button eases and caps where the original snaps to the midpoint.

Smaller differences:
- The remake shows the backpack in the crouch, and a step cancels a use. The original hides the backpack and locks movement until the use ends.
- The remake starts in underwear with a pistol. The original starts in a t-shirt and jeans with nothing in hand.
- The original greys the screen and shakes the camera on a hit; the remake does neither.
- Footsteps differ in rate, loudness and surface.
- The energy drink gives x1.25 in the remake, +10 px/s in the original.
- A half-pushed stick walks slower in the remake.
- Melee cooldowns and the axe swing sound come from 1.2.
- The original has a white outline behind buildings, breath puffs, and no death pose or sound.

Already identical:
- Base speed 100, and no stamina or sprint.
- Every animation frame rate.
- How a diagonal picks its facing.
- The aim pose.
- The def_1 body art.
- The clothing order, in effect.

## Findings

Verdicts: make identical 21, keep (user asked) 3, unclear 3

### 1. Fourteen worn/held sheets are the 1.2 fan mod's art, not 1.0

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `Assets/images/`, `Assets/Sprites/helmet_army`, `Assets/Sprites/helmet_hard`, `Assets/Sprites/helmet_nvg`, `Assets/Sprites/gasmask`, `Assets/Sprites/gorka_helmet`, `Assets/Sprites/pilot_helmet`, `Assets/Sprites/vest_bulletproof`, `Assets/Sprites/r670_shotgun`, `Assets/Sprites/madsen_mg`, `Assets/Sprites/crossbow`, `Assets/Sprites/hunter_backpack`, `tools/build_atlas.py`

**Original:** In 1.0 images/, 62 sheets differ from the remake's Assets/images (md5), and all 62 equal MiniDayZ+1.2 (S/survey/character/sheet_diff.txt).

The character ones, compared frame by frame on the body (m_art_diff1.png, m_art_diff2.png):
- helmet_army: 1.0 has a black mask face (26 of 34 frames differ)
- helmet_hard: a dark helmet, not the remake's yellow hard hat (38/38)
- helmet_nvg: a dark-green full NVG helmet (32/32)
- gasmask: a white full mask (32/32)
- gorka_helmet: a closed visor (25/32)
- pilot_helmet: black (25/32)
- vest_bulletproof: a different black (42/43)
- r670_shotgun: black stock, not wooden (21/54)
- madsen_mg (30/50) and crossbow (32/54)
- hunter_backpack[t457]: 25 frames differ, and 1.0's 4 big-pack twohand_* frames are missing in the rip
- player_skin_dead, player_skin_guard, player_skin_jager

In a run, the a_kit_d dump shows helmet_army t40 with the mask (m_zoom_kit.png).

**Remake:** Draws the 1.2 art: ra_kit_d shows a plain dome helmet with the face showing. The other 1.2 sheets fall outside this area:
- gui_item_helmet, gui_item_vest, gui_portrait_helmet, gui_portrait_vest, gui_pistol, gui_firearm
- gui_btn_attack, gui_btn_perks, gui_inventory, main_menu_wpn_icon, pad_controls
- b_car_bus, b_gunshop, ee_bpla, ee_kv2, ee_ruins
- zed_tank_skin, ammo_5x45, ammo_7x62, ground_enviroment_tilemap, shroom

### 2. Weapons not in hand are never drawn (slung rifle, holstered pistol, axe on back)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 20's 'the weapon shows up on their back even tho it should be in their hand' was about the held weapon)
- **Files:** `game/src/game/systems/render.lua`

**Original:** Every equipped weapon is on layer player_weapons. The ones not in hand (weapon var#2 = 1) play 'h_' + the pose:
- standing: h_twohand_* (7.6.x.3.2, 7.7.2.x.3.2)
- running: h_run_* (7.5.1.x.2)
- aiming: h_twohand_* (8.5.4.4.1.1.1.2)
- swinging: h_axe_* (8.7.1.5.1.2.3)
- crouching: using_item (7.1.1.2-4)

State dumps agree:
- a_rpa_d: 'firearm t45 h_twohand_down, pistol t39 h_twohand_down, melee t33 idle_down'
- a_fir_run_r: 'pistol h_run_right, melee h_run_right'
- c_mel_5: 'firearm h_axe_right'

Visible on the character: a_rpa_*, a_pis_*, a_kitmel_u (m_zoom_held.png).

**Remake:** render.held_key draws only e.weapon. The h_ frames are used only for a swing still finishing after Q (render.lua 109-146 and 217-231). ra_rpa_* and ra_pis_* show only the axe or only the pistol.

### 3. Gun in hand with no target: original body is twohand, remake's is idle

- **Verdict:** make identical  **Effort:** S  **User's ask:** none for the body. 'When there's no target, the weapon is lowered' (Stage 20) is met either way.
- **Files:** `game/src/game/player.lua`, `game/src/game/systems/render.lua`

**Original:** Switch_to_firearm and Switch_to_pistol (12.26.1, 12.27.1) call two_handed_in_hands (7.2.1), which sets Default_idle_anim_* to twohand_*. Standing (7.6.x), the parts then play:
- body and hands: twohand_<dir>
- outerwear (var#18): twohand_<dir>
- pants, vest, helmet and backpack: idle_<dir>
- the weapon in hand: idle_<dir>, lowered

The a_rifle_d dump says 'skin twohand_down, firearm idle_down, outer twohand_down, pants idle_down'. Both hands are on the gun in a_pis_d, a_pis_r and a_rifle_r. Melee goes back to idle_* (12.28 and 7.2.2).

**Remake:** player.pose plays 'idle' for the body whenever a gun is lowered; player.lua 601-659 says twohand was dropped. The arms hang apart and the pistol dangles at the side (ra_pis_d, ra_pis_r, ra_rifle_r; m_zoom_held.png). The original pairs the body's twohand with the weapon's own idle frame, so no art is missing.

### 4. Crouch (using_item): the original hides the backpack

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 25 asks only 'crouched down with a bar above his head')
- **Files:** `game/src/game/systems/render.lua`, `game/data/config.lua`

**Original:** 7.1.1.9: the var#10 backpack is made invisible while using_item plays. 7.1.2.5 shows it again in idle_down. The a_use1 dump says 'pack t43 ... HIDDEN'.

**Remake:** Draws the bag's idle frame facing down, lowered by crouch_drop (7 px): render.lua 199-203 and config player.crouch_drop. ra_use1 shows the backpack.

### 5. Speed while using an item: the original locks movement, the remake cancels the use

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 25 is about closing the board, not moving)
- **Files:** `game/src/game/player.lua`, `game/src/game/interaction.lua`

**Original:** A use sets player_collision_base var#21 = 1 and calls AltMove.Stop() (7.1.1). Every move path needs var#21 = 0: WASD 12.13.7.1, stick 12.13.6.4.1. The use runs its own wait (energy drink 2 s, 8.10.3.56.1), and nothing cancels it. Taken from the events; not tested in a run.

**Remake:** A step cancels the use with nothing spent, and so does an attack (player.lua ~397: 'if moving and e.action then interaction.cancel(e)').

### 6. Starting look: the original wears a t-shirt and jeans with nothing in hand

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** 2.13.1.1 (Player_skin_type 1) does Spawn_drop 350 (tshirt) and 300 (jeans), picky twice, condition 100. There is no weapon; the hands are melee or fists. run_idle.png shows the green t-shirt and jeans; the dump says 'outer t84, pants t83'.

**Remake:** player.spawn puts fnx_pistol in the pistol slot and holds it, and nothing is worn (player.lua 257-269). c_rd_world.png (--world 7) shows grey underwear and a pistol.

### 7. Scale on screen: the original is 1 world px per CSS px

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (a remake design choice; config.lua line 9)
- **Files:** `game/data/config.lua`, `game/src/core/viewport.lua`

**Original:** The C2 project is in fullscreen mode 'crop' (project[12] = 1), and the world layers stay at scale 1 (Current_zoom_lvl = 1).
- 844x390 at DPR 3: 844x390 world px. Measured layer 0 = 1 and GUI = 0.63 (gui_scale from 1.2.1); p_run.png.
- 1280x720: 1280x720 world px, with the 30 px character about 30 px tall.

**Remake:** world_view_height is 320, so scale = window height / 320 (viewport.lua 136-139).
- 844x390: measured 1.219, so the character is about 22% larger.
- 1280x720: 2.25, character about 67 px tall.
See m_phone_scale.png.

### 8. No 2x zoom inside buildings

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (the Stage 26/28 interior asks are about visibility and darkness, not zoom)
- **Files:** `game/src/core/viewport.lua`, `game/src/core/camera.lua`, `game/src/game/systems/interior.lua`

**Original:** Autozoom (20.1). Every 0.5 s, if player_base overlaps a warmzone (a room's floor), group zoomin adds 0.05 per tick to layers 0-50 up to Current_zoom_lvl + 1 = 2. That is about 20 ticks, ≈0.33 s at 60 fps. The Markers layer goes to 2. Leaving ramps back to 1. Measured in a church: l0 went 1, 1.5, 2.0, then back to 1 after walking out (b_inside2.png, b_outside.png). The GUI stays at 1.

**Remake:** No zoom state at all: world_scale depends only on window height, and a grep for zoom in src finds nothing.

### 9. The camera lags the player (exponential follow)

- **Verdict:** make identical  **Effort:** S  **User's ask:** Progress 16.1.2 (Stage 16 ask #17, not in user_asks.md): 'The player is locked to the centre of the screen'. This supports zero lag.
- **Files:** `game/src/core/camera.lua`, `game/data/config.lua`

**Original:** The camera sprite carries ScrollTo and is pinned (position only) to player_collision_base (2.13). It follows exactly every tick, with no easing.

**Remake:** Exponential follow with follow_speed 20 (core/camera.lua; config camera). Measured 4.2 world px behind during a run, about 9.5 px on screen at 1280x720 (rb_lagrun; logged as lag=4.21).

### 10. The camera centres the feet, not the body

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/core/camera.lua`

**Original:** The followed point is player_collision_base, the centre of the 30x30 frame (origin 15,15). The body is vertically centred: at 1280x720 it spans y 345-375 around 360 (a_* crops).

**Remake:** The camera follows the ground point, so the feet are at screen centre and the body is about 15 world px (≈34 px) higher, spanning y 293-360 (ra_* crops).

### 11. View clamped at map edges in the original

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Progress 16.1.2, Stage 16 #17: 'The player is locked to the centre of the screen'
- **Files:** `game/data/config.lua`

**Original:** The Map layout is bounded (unbounded_scroll False), so the view stops at the layout edge. At spawn x = 600 on a 1280-wide window the player stands at screen x 600, not 640 (run_idle.png). On the 844-wide phone this has no effect at spawn.

**Remake:** clamp_to_map = false (config camera), so the player is always centred.

### 12. The focus (zoom) button's camera eases and caps; the original snaps to the midpoint

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 20: 'another button to focus on the locked target, should be in gui_btn_zoom-sheet0'. The button was asked for; its motion and its melee visibility were not.
- **Files:** `game/src/game/systems/targeting.lua`, `game/data/config.lua`, `game/src/game/player.lua`

**Original:** isAim (8.5.6.1, 8.5.8.2.1) puts the camera every tick on the exact midpoint between player and gui_target, with no cap and no easing. It lets go on a second tap or when the target is no longer visible. gui_btn_zoom shows only in rifle or pistol mode with a target (8.5.4.4.1.3.1); melee hides it (8.7.1.6).

**Remake:** targeting.update_focus eases at focus_ease 4/s and caps at focus_reach 0.5 of half the view. Progress 20.1.6 calls both numbers invented. The button shows whenever a target is locked, whatever is in hand (player.lua e.focus_face).

### 13. Being hit: no full-screen grey flash and no camera shake

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 20's 'add blood effects and a hit sound effect. same with when a player takes damage' covers the blood and sound, which stay. The flash and the shake are not covered.
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`, `game/src/core/camera.lua`

**Original:** Player_get_hit 15.3.5 calls Hit_effect (15.4), which sets the layout's grayscale effect to 100 for 0.2 s. It also does ScrollTo.Shake(3, 0.4, decaying) and plays body_1/body_2 at -5 dB on the player. Wolf bites shake too (13.2.4). c_aim_rifle.png caught the grey frame at a bite; shake.js measured a scroll offset for about 0.4 s.

**Remake:** combat.hit_player calls effects.hit (a blood splat) and plays the impact.hit body sound. There is no grey flash and no shake; a grep for shake finds nothing.

### 14. Footsteps: timing, loudness and surface

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/player.lua`, `game/data/sounds.lua`, `game/data/config.lua`

**Original:** Event 7.8. While moving, a step plays every 0.25 s at -5 dB at the player, chosen by surface:
- sand (tilemap_sand, outdoors): sand_run
- indoors, by warmzone var#1: wood (1) or concrete_run (2)
- swamp tiles 13/14: swamp_step
- ground tile 6: run_1..4
- anything else: forest_run
That table is for CurrentLevel < 4; levels 4 and up use snow_run and road_run.

**Remake:** One step per 32 px walked (0.32 s at 100 px/s) at volume 0.3, always footstep.default (forest_run). player.lua footsteps() and data/sounds.lua still say only one ground type exists, though the remake now has a beach, snow bands and floors.

### 15. Ground and wire slowing missing

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** Max speed is 100 - var#6 - var#7 + var#4 + var#3 + var#5 + var#8 (7.3).
- Swamp tiles 13/14: var#7 = 45 on levels below 4 (55 px/s), 25 on levels 4 and up (7.8.1.x.3.1.1).
- Barbed wire: var#6 = 45 for 1 s (20.6.2).

**Remake:** speed = (100 + speed_modifier) * boost (player.lua 380-383), and nothing ever sets speed_modifier.

### 16. The energy drink's boost size, and the run cycle speeding up with it

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 23: 'Speed boost from energy drinks.' The boost exists in both; its size was not asked.
- **Files:** `game/data/config.lua`, `game/data/items.lua`, `game/src/game/player.lua`

**Original:** Energy drink (item 63, 8.10.3.56.1): +10 px/s (player_base var#4 = 10) for 60 s, +20 water, 2 s use. Adrenaline (item 45, 8.10.3.39.1): +20 px/s for 10 s. The run animation stays at 10 fps whatever the speed.

**Remake:** x1.25 (cfg.survival.boost_speed) for 60 s. The edrink is a 1.4 s use giving water 20 and food 4 (items.lua 305). The run cycle is sped up by the same factor (player.lua e.anim.rate).

### 17. Stick: any push walks at full speed; the original also accelerates

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** The stick sets AltMove's vector to (knob - centre) * 100, which EightDir caps at max 100 (12.13.6.4.1), so any deflection is full speed. EightDir accelerates at 600 px/s² (player_base props [100, 600, 500, 3, 3, 0, 1]), 0 to 100 in 0.17 s. Letting go sets the vector to 0 at once (12.13.7.2, 12.13.6.5). The original's default phone control is tap-to-move (GUI_control_type 0); the stick is option 1.

**Remake:** player.heading lets a half-tilted stick walk at half speed, and velocity is set directly with no ramp.

### 18. Melee is swung on a button (the original swings automatically on contact)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20: 'for the attack / fire button ... a fist sprite if holding melee'; Stage 18: 'melee has no cool down'; Stage 19: 'using your melee displays a white swoosh'
- **Files:** `game/src/game/player.lua`

**Original:** Melee_2_auto (8.7.1): when touching an enemy, with line of sight and in melee mode, the player turns to the target if standing and swings axe_<dir>. The attack button is hidden in melee (8.7.1.6). c_mel_* shows the automatic swing.

**Remake:** Swings on the attack button (Space), with a fist face and a swoosh.

### 19. Melee cooldowns and swing sound come from 1.2

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 18 asked for a cooldown, not these numbers)
- **Files:** `game/data/generated/weapon_stats.lua`, `game/data/sounds.lua`

**Original:** Fists: 0.3 s (Wpn_cooldown 0.3, Wait 0.3; 8.7.1.5.1.2.1.7). A weapon uses its own var#4: axe_red is 0.5 on the 1.0 Map (1.5 only in the Tutorial layout, and 1.5 in 1.2). Every swing, fist or weapon, plays Punch_Swipes_0/1. In 1.0, chop is the sound of a round hitting an obstacle (16.1.6).

**Remake:** Fists 0.5 and split axe 1.5, both from the 1.2-derived data/generated/weapon_stats.lua. The axe swing plays chop..chop4 (sounds.lua melee.axe).

### 20. A melee weapon is put up at a target

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 20: 'When there's no target, the weapon is lowered; when there's a target, the weapon is put up and aimed.' It does not say whether melee is meant.
- **Files:** `game/src/game/player.lua`

**Original:** In melee mode a target changes no pose: 8.5.4.4.2 only moves gui_target. The player turns only for the contact swing (8.7.1.5.1.1).

**Remake:** Holds the swing's first frame facing the target (player.pose, sprites.still); see rb_guard_melee.

### 21. No white outline on the player behind a building

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (the interior asks are about cutaways; facades without a room still fade)
- **Files:** `game/src/game/systems/render.lua`

**Original:** When the player overlaps a facade north of its image point 1, var#42 is set (20.2.1.1.2.1) and the facade fades to 25%. The skin then gets a white 1 px Outline effect (20.2.6, 20.2.7): d_behind_25.png, d_behind_60.png, m_behind_zoom.png.

**Remake:** No outline on the player.

### 22. No breath puffs

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/effects.lua`

**Original:** Every 3 s (21.2), a 6x6 player_breathe puff appears at (x, y-10) on player_weapons. It drifts 15 px/s to the right, or along the moving angle, and fades out. While smoking (var#52), one every 1 s at scale 1 to 2.5 (21.3).

**Remake:** None; a grep for breathe finds nothing.

### 23. Death: the original has no body pose and no sound

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`, `game/data/sounds.lua`

**Original:** No dying animation and no death sound: 10.3 and 6.2.3 play nothing, and tutor_death is tutorial-only (11.18). player_collision_base and player_base are destroyed and Dead_screen replaces the world (c_dead_a.png, c_dead_b.png). player_skin_dead is used only in casual mode (6.2.3.1.1).

**Remake:** The body stays in idle under a dimmed world with the YOU DIED panel (player.lua 327-345; rb_dead_b.png), and plays 'tutor_death'. The death screen itself belongs to the menus area.

### 24. Draw order against zombies, trees and buildings

- **Verdict:** unclear  **Effort:** L  **User's ask:** none (it rests on a Progress ground rule, not a user ask)
- **Files:** `game/src/game/systems/render.lua`

**Original:** Fixed layers: Zeds (29) below player (32-39) below Buildings (41) below Trees (43). Zombies are always drawn under the player. Tree canopies always cover him (a_fir_run_d, a_fir_run_u). Buildings change layer and fade by y (20.2, 20.3).

**Remake:** Everything sorts by ground y (the Progress ground rule 'Draw order sorts by ground-y').

### 25. A held knife and a lit flare are never drawn

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/items.lua`, `game/data/weapons.lua`, `Assets/Sprites`

**Original:** knife_any and flare_hand (Equip_flare, function 30) are drawn on player_weapons.

**Remake:** Neither has an Assets/Sprites folder, and the items are not built (data/items.lua 580-607).

### 26. Only one character

- **Verdict:** unclear  **Effort:** L  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** 20 skins with three hand colours, picked on the menu and unlocked by achievements (unlock_char_*, 4.x).

**Remake:** player_skin_def_1 only. Achievements are listed under 'Explicitly out of scope' in Progress.

### 27. Shooting on the move: raised upper body over running legs

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 22: 'Shooting while running does not play shooting animation.'
- **Files:** `game/src/game/player.lua`, `game/src/game/systems/render.lua`

**Original:** There are no running-and-shooting frames. A shot while moving keeps the run animation (aiming in 8.5.4.4.1.1 needs NOT IsMoving).

**Remake:** The upper rows come from the pistol pose and the legs from the run (player.pose anim.upper; render draw_layers).

## Already identical

- Base speed 100 px/s; no sprint and no stamina. EightDir max 100, formula 7.3; measured 205 px in about 2 s. Diagonals capped at the same speed.
- No speed change while aiming, hurt or carrying. The original's hurt and runner bonuses are perks (6.3.28, 12.7.11), which are out of scope.
- Animation rates and loops (objects.txt t742 matches anim_speeds.lua line 613): run 4 frames at 10 fps looping; idle 1 frame; axe 3 frames at 10 fps once; using_item 4 frames at 8 fps looping; pistol and twohand 1 frame each.
- player_skin_def_1 art: all 44 frames pixel-identical. Every other wearable and weapon is pixel-identical too, apart from the 1.2 sheets listed in the first finding.
- Facing chosen from movement, diagonals included: the original run gave run_down at 135 and 45 degrees and run_up at -45 and -135; the remake's direction_from also resolves ties vertically.
- Facing right at spawn.
- Aim pose: pistol_<dir> for both rifle and pistol, turning to face the target while standing (8.5.4.4.1.1.1). Clothing follows in its own pistol frames.
- Weapon in hand: idle frame when lowered, run frames while moving, using_item while crouched.
- Clothing order: swapping helmet and backpack changes 0 of 1,710 composites (19 headwear x 9 packs x 10 poses). Leaving out the original's player_hands_white layer changes 0 of 308 composites for def_1.
- No hit-reaction pose in either.
- A cooldown bar over the head on a swing or use (original: gui_wpn_cooldown_bg pinned over the player, 2.13; c_mel_0).

## Not checked

- Tap-to-move, the original's default phone control (Movement_tap 12.13.5, walk_marker); belongs to the controls area.
- Vehicles: driving pose and camera (out of scope).
- Headlamp, NVG and flare light on the character at night; belongs to the lighting area.
- Actual loudness of remake sounds against the original's dB values (not listened to).
- The other 19 character skins and the black and asian hand layers, in use.
- Casual-mode death animation (player_skin_dead id_N).
- The remake at a real phone's device pixel ratio: its harness renders 844x390 at DPR 1. The original was checked at DPR 3.
- Exact timings at 60 fps in the original: headless C2 ran slowly, so the zoom ramp and swing cadence are taken from the events, not measured.
- Moving during a use in the original: read from the events, not tried in a run.
- Low-HP grayscale (user-asked variant); belongs to the effects area.
- Firing effects (muzzle flash, shells), dispersion and recoil; belongs to the combat area.
- Whether the original fires or reloads during a use (var#21 checks in 8.6 were not read).
