# hud: the remake against the official 1.0

*Area surveyed:* HUD and controls (meters, status icons, clock/day, portrait, minimap/map button, touch buttons and stick, PC keys, pickup prompt, over-head and other text, aim/target display, hit feedback) at 1280x720 and 844x390 touch

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

The original 1.0 was run at 1280x720 and 844x390 (touch), and every HUD element's on-screen rectangle was measured. Its event sheets were read: 1.2, 2.11, 6.2, 6.3, 8.5, 8.6.2, 8.7.1, 12.3, 12.6, 12.8, 12.9, 12.12, 12.13, 12.16, 12.18, 12.24, 12.26-12.29, 15.3-15.4, 17.2. The remake (Stage 26, b180a9d) was captured in the same states, and hud.lua, touch_controls.lua, key_hints.lua, status.lua, targeting.lua, widgets.lua, viewport.lua, input.lua and config.lua were read.

The skeleton differs throughout.
- Scale: the original draws its GUI 1:1 at 720p and 0.63 at 390p. The remake draws it 1.333 and 0.722, so its HUD is 33% and 15% bigger.
- Meters: the original has 4 plates in a row at top-centre, ordered heart, water, food, heat. The remake stacks them at top-left, ordered heart, food, water, heat.
- Buttons: the original's are in a right-edge column (interact, attack plus zoom, reload, switch in the corner) with the backpack bottom-left. The remake regroups them, puts the bag under a corner minimap and moves the gear.
- Missing from the remake:
  - the clock tab and the typed "DAY N" label;
  - the character portrait with gear and XP;
  - the notepad (pad) button with its new-game tip, and the night sleep button;
  - the green regeneration heart, and the heat "+N/-N" text (the remake shows its own red arrow instead);
  - hp bars over zombies, floating damage numbers ("-N", "Crit -N") and name labels on items in reach;
  - outline highlights (trunk outline), the full-screen 0.2 s grey hit flash and the camera shake;
  - the original's survival and state lines (red, yellow and green).
- Over-head lines: the original shows two outlined, colour-coded Arial lines, fading in 0.5 s, holding 5 s and fading out 1 s (measured). The remake shows one cream line for 2 s.
- Aim and target display: the remake hides nothing with melee (reticle and FOCUS show with fists), draws the reticle at 0.7 scale, and draws opaque short aim lines. The original's lines are 0.5 alpha and run 46 to 185 px.
- Phone movement: the original defaults to tap-to-move, with a fixed, player-placed stick and WASD as options. The remake uses a floating analog stick.

The user's asks cover these and they stay:
- status icons at the end of their bars;
- USE only with something in reach;
- the switch order melee, pistol, rifle;
- the punch face for melee;
- TARGET, FOCUS and the door button;
- the weapon picture bottom-left;
- RELOAD only with a gun;
- the PC keys, the TAB bag hint and the touch toggle;
- the low-health grey;
- the texts said over the head.

One open question needs the user: the remake's attack button face (a trigger) is the 1.2 fan mod's art. Assets/images is a 1.2 copy. 1.0's gui_btn_attack is a red crosshair, but the user asked for a "trigger sprite, gui_btn_attack". The 1.2 copy also changes two gui_pistol frames used by the weapon picture.

I could not write report.md: the harness refuses report files from subagents. All the content is in this structured output. The evidence is in S/survey/hud/orig (prefix v2_ for this pass; rect logs v2_run1280.log and v2_runphone.log), S/env/s2/caps (hud_*, hud2_*), S/survey/hud/cmp (side-by-side pair_*.png and zooms) and S/survey/hud/js2 (probes). S = /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad.

## Findings

Verdicts: make identical 33, keep (user asked) 9, unclear 3

### 1. HUD scale rule (GUI size on screen)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/core/viewport.lua`, `game/data/config.lua`, `game/data/ui/hud.lua`, `game/data/ui/touch_controls.lua`

**Original:** Game_events 1.2.1 Device_check: GUI layers scale 1.0 when viewport height >= 590, else gui_scale = 1-(590-h)*0.00185. Measured: 1280x720 scale 1.0 (panels 120x40, buttons 100x100; S/survey/hud/orig/v2_run1280.log); 844x390 scale 0.63 (panels 76x26, buttons 63x63; v2_runphone.log).

**Remake:** src/core/viewport.lua ui_scale = min(w/960,h/540): 1.333 at 1280x720 (plates 160x53, BAG 133x133, attack 133x106; S/env/s2/caps/hud_rects_1280.log), 0.722 at 844x390 (plates 86x28, BAG 72). HUD is 33% / 15% larger than the original.

### 2. Meter plates: one row at top-centre, order heart-water-food-heat

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`

**Original:** 12.13.2.1.1.1-.4: gui_panel frames 0..3 at user_device_X_mid -195/-65/+65/+195, top+30. 1280x720 rects x 385/515/645/775 y 10 (120x40); 844x390 x 261/343/425/507 y 6 (76x26). Order heart, water, food, thermometer. Shots v2_o_idle.png, v2_p_idle.png.

**Remake:** data/ui/hud.lua METERS: vertical stack top-left at (8,6), pitch 40, order hp, food, water, heat (hud_noon_1280.png, hud_phone_idle.png; cmp/pair_idle_1280.png, pair_idle_phone.png).

### 3. Bar fill geometry and low-value tint

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`, `game/src/ui/widgets.lua`

**Original:** Update_Stats 6.3 / 6.3.12-15: fill width = value*0.72 (max 72x10), at plate offset (43,14) (gui_blood_bar 428,24 72x10 on panel 385,10). Colour never changes (b6_lowhp.png).

**Remake:** hud.lua: track 70x10 at (44,14); low_at tints the bar: hp<25 (1,.35,.35), food/water<20 orange, heat<20 blue (hud_lowhp_1280.png).

### 4. Regeneration heart overlay missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`, `game/src/game/systems/survival.lua`

**Original:** gui_regeneration over the plate heart: frame 1 (green heart with up arrow) while regenerating (food/water/heat >=50, not bleeding/sick, 6.2.2.10), frame 2 when doubled (6.2.2.10.3), frame 0 grey otherwise (6.2.2.11, 6.2.5). t_hud_noon.png green vs a8_status.png grey; cmp/meters_1280.png.

**Remake:** No regeneration indicator in hud.lua; heart always the plate's grey/white pictogram.

### 5. Heat delta text '+N/-N' instead of the warming arrow

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 24 'Heat is acquired by being near a campfire or indoors' does not ask for an arrow)
- **Files:** `game/data/ui/hud.lua`, `game/src/game/systems/survival.lua`

**Original:** gui_heat_text sprite font (blue digit strip) centred on the heat bar: '+N' hue-shifted orange when Player_temperature>0 (6.3.18.2), '-N' blue when <0 (6.3.20.2), empty at 0 (6.3.19); 74x18 at 818,21 at 720p (cmp/heat_text_orig.png). gui_heat_arrow is destroyed at layout start (2.11.4) so 1.0 never shows an arrow (and the warming/cooling lines 6.3.18.1/6.3.20.1 never fire).

**Remake:** hud.lua icon_warming: gui_heat_arrow/default_1 red arrow beside the heat plate while warming (Stage 24.3); no number, no cooling indicator.

### 6. Clock tab top-right missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`

**Original:** gui_time_bg 118x48 (clock icon tab) at right edge, top+5 (1166,5 at 720p; 772,3 phone) with Text[t287] 'HH:MM' 18pt Arial black + rgb(150,140,131) copy 1 px off; updated each game minute (1.5 s) in 17.2.6. cmp/orig_corners.png.

**Remake:** No clock anywhere in a run (hud.lua has none).

### 7. 'DAY N' typed label missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`, `game/src/app.lua`

**Original:** Day_show_label 12.16.1: letters 'D','A','Y ',N every 0.3 s each with menu_click at -10 dB, bold 24pt Trebuchet MS white at GUI (200,500) (screen 328,476 at 720p), then Fade (wait 1, out 2). Shown 2 s after run start (3.6.3.7) and at 06:00 daily (17.2.7). orig/a1_daylabel.png.

**Remake:** None.

### 8. Character portrait with worn gear and XP number (top-left)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`

**Original:** gui_portrait_bg 78x106 at -2,0 holding gui_btn_perks (character face 58x64 at 8,6) with helmet/vest/outerwear/backpack portraits layered (66x66) and XP Text[t550] '0' 10pt white at 3,77 (updated 22.2); gui_btn_perks_notify flashes when a perk is available. v2_o_idle.png, cmp/orig_corners.png.

**Remake:** None.

### 9. Perks & stats screen behind the portrait

- **Verdict:** unclear  **Effort:** L  **User's ask:** none
- **Files:** `Progress.md`

**Original:** Tap gui_btn_perks opens the perks/stats menu (12.7.5, show_tab_perks/show_tab_stats).

**Remake:** Not present; Progress.md 'Explicitly out of scope' lists 'Perks (revisit after Stage 15)' -- the remake's own decision, not a user ask.

### 10. Gear (Options) button position and size

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`

**Original:** gui_help 100x66 at right edge, top+45 (12.13.2.1): 1180,45 at 720p, 781,28 phone.

**Remake:** btn_settings left of the minimap: 920,13 133x88 at 720p, 648,7 phone (hud.lua). Same art.

### 11. Notepad (pad) button and its new-game pulsing tip

- **Verdict:** make identical  **Effort:** M  **User's ask:** Stage 28: "As for the map button, use the notepad looking button" (Stage 27: "make the map into an openable menu, clicking a button opens and closed the map")
- **Files:** `game/data/ui/hud.lua`, `game/data/ui/touch_controls.lua`

**Original:** gui_pad_btn 100x66 right edge top+115 (1180,115 at 720p; 781,72 phone) opens the pad with pad_open sound (12.6.1-2); map is a pad tab. At a new game walk_marker[t689] (80x80, 6 frames 10 fps) pulses on it until first tapped (3.20/3.23); a1_daylabel.png, b6_lowhp.png.

**Remake:** Stage 26 has no pad/map button.

### 12. Always-on corner minimap

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 27/28 asks put the map behind a button)
- **Files:** `game/data/ui/hud.lua`, `game/src/ui/minimap.lua`

**Original:** No minimap on the HUD (minimap_tile instances parked off-screen at -1000; the map lives in the pad).

**Remake:** hud.lua minimap_paper + minimap 152x114 at top-right (1064,13 202x152 at 720p; 726,7 phone).

### 13. Night sleep button missing

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`

**Original:** gui_sleep 100x66 (bed, zZ) right edge top+185, visible from 00:00 (17.2.5) to 06:00 (17.2.7) and when a run starts before 06:00 (3.6.3.6); tap opens sleep menu (12.1.6). orig/b7_night.png.

**Remake:** No sleeping feature or button.

### 14. Touch button layout: right-edge column, bag bottom-left, switch bottom-right

- **Verdict:** make identical  **Effort:** M  **User's ask:** none for positions (Stages 20/22/23 asked the buttons' faces and visibility, Stage 18 the dead zone)
- **Files:** `game/data/ui/touch_controls.lua`, `game/data/ui/hud.lua`

**Original:** 12.13.2.1/1.2.10, 1280x720 // 844x390: interact 100x100 at 1180,288 // 781,134; attack 100x80 at 1180,413 // 781,213 (gun only); zoom 100x80 left of attack 1080,413 // 718,213 (target only); reload 100x100 1180,518 // 781,279 (gun only); switch 100x100 bottom-right 1180,620 // 781,327; backpack 100x100 bottom-left 0,620 // 0,327. v2_run1280.log, v2_runphone.log, b2_target.png.

**Remake:** touch_controls.lua / hud_rects_1280.log: BAG top-right under minimap 1122,168 133; USE 1128,304 106; attack 1112,448 133x106; reload 992,384 106; switch 853,384 106; target 1002,536 85; focus 853,536 106x85; same layout x0.722 on phone (cmp/pair_target_1280.png). Note: the user-asked weapon picture sits where the original's bag is (bottom-left); put it beside the bag.

### 15. USE/interact shown only when something is in reach

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 22: "oh also the pick up button should only appear when there's an item to pick up"
- **Files:** `game/data/ui/touch_controls.lua`

**Original:** gui_btn_interact always drawn: 50% opacity frame 6 when nothing in reach, 100% and frame by kind when something is (12.13.3.5.1.4/.5, every 0.5 s).

**Remake:** btn_interact visible_when player.item_in_reach (hidden otherwise).

### 16. Switch button hidden without a gun (original: 50% opacity, says 'I have no ranged weapon.')

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 20 asked one cycling switch button, not hiding)
- **Files:** `game/data/ui/touch_controls.lua`

**Original:** gui_btn_switch always shown; 50% when no rifle/pistol carried (12.24.2, 12.24.4), 100% with one (12.24.11/12); tap with none -> client_log 'I have no ranged weapon.' white (12.13.14.1.2.2); 0.1 s wait before switching. v2_o_idle.png (fist frame at 50%).

**Remake:** btn_switch visible_when player.can_switch (hidden with no gun).

### 17. Switch cycle order

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20: "switch button goes from Melee, Pistol then Rifle and back to melee"
- **Files:** `game/src/game/player.lua`

**Original:** 12.13.14: melee -> rifle (else pistol) -> pistol (else melee) -> melee.

**Remake:** melee -> pistol -> rifle -> melee.

### 18. Attack button face is the 1.2 fan-mod art (trigger), 1.0's is a red crosshair

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 20: "for the attack / fire button, show weapon trigger sprite, gui_btn_attack, when holding a pistol or rifle" -- describes the 1.2 art; 1.0's crosshair would also duplicate the asked TARGET face
- **Files:** `Assets/images/gui_btn_attack-sheet0.png`, `Assets/images/gui_pistol-sheet0.png`, `Assets/images/gui_pistol-sheet1.png`, `game/built/atlas/sprites_md.png`, `game/data/generated/atlas.lua`

**Original:** 1.0 images/gui_btn_attack-sheet0.png (md5 38f59fdf...) is a red crosshair on the torn tile; shown for pistol/rifle (v2_o_target.png, b2_target.png; cmp/attack_sheets.png).

**Remake:** Atlas gui_btn_attack/default_0 (sprites_md cell 273, cmp/remake_attack.png) is the trigger; Assets/images/gui_btn_attack-sheet0.png md5 1f2d374f... equals MiniDayZ+1.2's (cmp/MiniDayZ_1.2_attack.png). Assets/images is a 1.2 copy (62 sheets differ); other HUD frames differing from 1.0: gui_pistol items_4 (17 px) and items_13 (944 px) used by the held-weapon picture (tools/atlas_vs_10.py).

### 19. Attack button shown with melee (punch face)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20: "...show weapon trigger sprite... and a fist sprite if holding melee"
- **Files:** `game/data/ui/touch_controls.lua`

**Original:** Hidden with melee (8.7.1.6); melee swings automatically on contact (8.7.1.5).

**Remake:** btn_attack shows gui_btn_zoom/gl_shoot_0 punch with melee (hud2_fists_1280.png).

### 20. Reticle and FOCUS shown while holding fists/melee

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`, `game/data/ui/touch_controls.lua`

**Original:** With melee gui_target is set invisible (8.5.4.4.2.1.1) and gui_btn_zoom hidden (8.7.1.6); zoom only with a gun and a target (8.5.4.4.1.3.1).

**Remake:** status.draw draws gui_target whenever targeting.current exists; btn_focus visible with any target. hud2_fists_target_1280.png / cmp/remake_fists_target_crop.png show the green reticle and FOCUS with fists. (TARGET with melee stays: asked.)

### 21. Focus camera: instant halfway, uncapped

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 20 asked 'another button to focus on the locked target', not its motion)
- **Files:** `game/src/game/systems/targeting.lua`, `game/data/config.lua`

**Original:** 8.5.6.1 / 8.5.8.2.1: camera set to player + half the distance to the target each tick; no cap, no easing.

**Remake:** targeting.update_focus: eases at focus_ease 4/s, capped at focus_reach 0.5 of half the view (config targeting).

### 22. Target reticle size

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`

**Original:** gui_target 40x40 world px at scale 1 (40x40 on screen at 720p, v2_run1280.log).

**Remake:** config targeting.reticle_scale = 0.7 (28 world px).

### 23. Aim lines: alpha, length, angle and colour tiers

- **Verdict:** make identical  **Effort:** S  **User's ask:** none for alpha/length/tiers (Stage 20 'Use them like a cone to display dispersion' and Stage 22 'aiming lines should only appear when there's an actual target' stay)
- **Files:** `game/src/game/systems/status.lua`, `game/data/config.lua`

**Original:** Two aim_line_1 sprites 139x2 at opacity 0.5, from 46 to 185 px out (origin x -0.333), at +/- dispersion_angle (8.5.7.2); frame tiers: 4 at default dispersion, then thirds of dispersion_angle_max (8.5.2.3.x). b2_target.png, p3_target.png.

**Remake:** status.draw_aim/status.cone: opaque, from muzzle (11-24 px) to cone_reach 72 world px, at +/- spread half-angle, tiers in quarters with green only when settled (hud_target_1280.png). Cone shape and target-only display are asked.

### 24. HP bars over zombies/animals missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`

**Original:** npc_hp_bar_bg + npc_hp_bar 30x6, 25 px above every NPC from spawn (13.1.1.N), width 30*hp/max (13.2.1.N); rect 605,331 30x6 in v2_run1280.log; b2_target.png, p3_target.png.

**Remake:** None in Stage 26 (hud_target_1280.png, hud2_fists_target_1280.png).

### 25. Floating damage numbers missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/status.lua`

**Original:** dmg_hint 12.29: '-N' 12pt Arial rgb(255,196,68) outlined, flies away from the player at 40 px/s, gone after 1.5 s (Fade [1,0,1.5,0.001,1]); crit = 18pt red 'Crit -N'. orig/v2_o_dmg.png.

**Remake:** None.

### 26. Name label on the item in reach missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 20 'remove inventory hints' / 'any type of hint' concern the board)
- **Files:** `game/src/game/systems/status.lua`, `game/src/game/interaction.lua`

**Original:** Text[t513] 10pt Arial black outline at the nearest pickable within 100 px + LOS: rgb(200,255,200) items, white weapons, refreshed every 0.5 s (12.13.3.5.1.1-.2). a2_item_near.png, v2_p_item.png.

**Remake:** None (hud_item_1280.png, cmp/phone_item_pair.png).

### 27. Highlight of usable things (trunk outline, outlines on pumps/bushes/panels)

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/render.lua`, `game/src/game/container.lua`

**Original:** car_loot_point white trunk outline shown within 50 px (12.13.3.5.1.3.3; cmp/car_loot_point.png); Outline effect on waterpump/berry_bush/plants/panel_button in reach.

**Remake:** None; a trunk only changes the USE face.

### 28. Over-head lines: colour, outline, 2-line stack, 0.5/5/1 s fade, centre display while menus open

- **Verdict:** make identical  **Effort:** M  **User's ask:** none for styling/timing (texts 'It's empty', 'I don't have ammo for this' Stage 20 and 'My inventory is full' Stage 22 stay)
- **Files:** `game/src/game/systems/status.lua`, `game/src/game/systems/combat.lua`, `game/data/config.lua`

**Original:** client_log 12.18: Text[t474] 500x24 10pt Arial with black Outline, colour per call (red 219, white 102, yellow 69, green 15 uses), created 30 px over the player and pinned, older line pushed up 18 px, max 2 lines; measured fade in 0.5 s, hold 5 s, out 1 s (v2_runphone.log fade probe); with inventory/pad/perks open it is shown at screen centre at 2x size rising 48 px/s. v2_o_msg3.png, a8_status.png, b4_reload.png.

**Remake:** status.draw_say/combat.say: one line, cream (232,228,216) with 1 px shadow, 1.1x 12 px UI font, say_time 2 s with 0.6 s fade-out, no fade-in, not shown while the board is open (hud_say_1280.png). The asked texts stay; the originals there are yellow 'Mag empty.' and red 'No free slots.'.

### 29. Survival/state lines over the head missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/survival.lua`, `game/src/game/systems/combat.lua`

**Original:** Red every 10 s: 'I can feel blood dripping.' (6.2.9), 'I'm freezing.' (6.2.10), 'I'm dying of starvation.'/'dehydration.' (6.2.11/12); yellow: 'I want to eat something.' 30 s food<=25, 'I want to drink something.' 25 s water<=25, 'It's pretty cold.' 20 s heat<=25; green: 'I'm no longer bleeding.' (6.2.7), 'I feel healthy.' (6.2.8); red 'Weapon ruined!' (8.6.2.2.1.1).

**Remake:** Only the asked lines plus 'It's getting colder', 'Something's in the way', 'No campfire nearby'.

### 30. Being hit: 0.2 s full-screen grey flash and camera shake missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 20 asked blood and hit sound on player damage, which stay)
- **Files:** `game/src/game/systems/render.lua`, `game/src/core/camera.lua`, `game/src/game/systems/combat.lua`

**Original:** Player_get_hit 15.3.5 calls Hit_effect (15.4: layout Grayscale 100% incl. HUD for 0.2 s) and ScrollTo.Shake(3, 0.4). orig/v2_o_dmg.png fully grey.

**Remake:** Neither (no shake or flash code in src).

### 31. Status icon animations (sick 1 fps, boost 3 fps)

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`, `game/src/game/systems/survival.lua`

**Original:** gui_sick anim 'sick' 2 frames 1 fps loop, 'immune' when immune (6.3.25-27); gui_boost 'Boost'/'Adrenaline' 2 frames 3 fps loop the whole time (8.10.3.56.1/.39.1).

**Remake:** hud.lua: sick_0 static; boost steady with a 2 Hz dim pulse in its last 10 s.

### 32. Boost icon position

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 17's 'at the end of the appropriate bar' has no bar for boost)
- **Files:** `game/data/ui/hud.lua`

**Original:** gui_boost pinned to gui_regeneration, 83 px left of the heart (layout 200,35 vs 283,31), left of the armour shield.

**Remake:** Below the four plates (hud.lua BOOST_X/BOOST_Y).

### 33. Status icons at the end of their bars, armour shield on the HP row

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 17: "Fix the bleeding icon, starving icon and other icons to be at the end of the appropriate bar"
- **Files:** `game/data/ui/hud.lua`

**Original:** Bleed/sick on the HP bar's right end (gui_sick 471,14 over bar 428..500); starving/thirsty/freezing red overlays on the plate's own pictogram (657,14 / 531,14 / 791,14); armour shield left of the heart (345,14). a8_status.png, v2_o_status.png.

**Remake:** Past each plate's end (STATUS_X), armour at the end of the HP row (hud_status_1280.png).

### 34. Pressed-button darkening

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/ui/widgets.lua`

**Original:** 12.3.x Touch_fade: AdjustHSL lightness 80% while touched (interact, attack, inventory, perks, reload, switch, builder, help, pad).

**Remake:** widgets.button multiplies by 0.72 while held.

### 35. HUD hidden while the inventory board is open

- **Verdict:** unclear  **Effort:** M  **User's ask:** Stage 17: "Inventory is too disorganized, design inventory to be easier to understand"; Stage 22 one bag opens and shuts -- related but not about the HUD
- **Files:** `game/data/ui/hud.lua`, `game/data/ui/touch_controls.lua`, `game/data/ui/inventory.lua`

**Original:** Meters, portrait, clock, gear, pad, bag, interact and switch all stay visible around the inventory (orig/e_inv.png).

**Remake:** Every HUD element and touch control has visible_when_not = inventory_open; the board carries its own meters and bag.

### 36. Phone movement: tap-to-move default, fixed placeable stick option, full speed

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/ui/touch_controls.lua`, `game/src/ui/ui.lua`, `game/src/ui/widgets.lua`, `game/src/game/player.lua`, `game/data/ui/settings.lua`

**Original:** GUI_control_type default 0 = TAP (3.6.3.1): hold anywhere to walk toward the finger, tap drops walk_marker (6 frames 10 fps) and walks to it (12.13.5); option STICK = fixed gui_dpad_field 250x250 placed by dragging in the menu (Menu_Events 3.2.14, LocalStorage stick_pos) with full speed at any displacement (vector*100, 12.13.6; orig/f_stick.png); option WASD (12.13.7); Options cycles them (12.9.3.5).

**Remake:** touch_controls move_stick: floating stick in the left half, radius 60, deadzone 0.18, analog speed (player.heading); no tap-to-move, no fixed-stick option.

### 37. PC controls: keys and hidden touch buttons

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 17: "Keybinds, E to pickup ..., Tab for inventory backpack icon at the bottom with Tab written underneath it"; Stage 17: "Settings window that toggles touchscreen on/off"; Stage 18: "on pc, only space bar"
- **Files:** `game/data/input.lua`, `game/data/ui/key_hints.lua`

**Original:** No action keys at all; only WASD movement if chosen in Options (12.13.7); Keyboard.OnAnyKey (33) only appends to a debug text. A PC player clicks the same on-screen buttons and clicks to move by default.

**Remake:** data/input.lua: WASD + E/Tab/Space/R/Q/T/Z/F/Esc/Enter/G; with touch off the buttons hide and key_hints.lua shows the bag with 'TAB' at bottom centre (hud_noon_kb_1280.png).

### 38. Build-mode buttons position and size

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (Stage 24 asked for a build mode, not the button layout)
- **Files:** `game/data/ui/touch_controls.lua`, `game/data/ui/key_hints.lua`

**Original:** 12.8.2.1-.3: gui_btn_builder at 2x (80x80) centred: tick (frame 0) at mid-80, cross (frame 1) at mid+80, rotate (frame 2) at mid, all at mid_y+100.

**Remake:** Bottom-right row at 64 design units (85 px at 720p), cross left of tick (touch_controls btn_build_place/cancel); on PC either side of the TAB bag (key_hints).

### 39. Debug drawer tab drawn in the player's build

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/debug/debug.lua`

**Original:** Nothing at the bottom-right except the switch button.

**Remake:** '^ dbg' tab at bottom-right in every capture; Progress.md: 'The debug drawer's tab is drawn in the published web build (20.6, found)'.

### 40. Reload icon/spinner over the head drawn below native size

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`

**Original:** 8.3.1: reloading_icon 25x36 + reloading_spinner 47x47 at native size, pinned about 49 px above the origin.

**Remake:** config status.reload_scale = 0.6.

### 41. Starting HUD state: remake starts with a pistol in hand

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** A new run starts with fists: switch at 50% on the fist frame, no attack or reload button (v2_o_idle.png).

**Remake:** player.lua:263 'local held = e.weapon or "fnx_pistol"' equips a pistol, so attack, reload, switch and the held picture show from frame one (hud_noon_1280.png). Overlaps the start-loadout area.

### 42. Held-weapon picture bottom-left

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 22: "Instead of the name of the weapon on the bottom left, include the sprite of the weapon, no name, no ammo, no durability"
- **Files:** `game/data/ui/hud.lua`

**Original:** None (that corner is the backpack button).

**Remake:** hud.lua held_pic/held_icon: the board's picture of what is held, nothing with fists.

### 43. TARGET (switch target) button

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20: "the red circle sprite for target switching, gui_btn_car_shoot_sheet0, it should only show if there's an enemy on screen"
- **Files:** `game/data/ui/touch_controls.lua`

**Original:** No such button: always the nearest target (manual_target_on is never set).

**Remake:** btn_target gui_btn_car_shoot while an enemy is on screen.

### 44. Door button

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 23: "Door button with door sprite when near a door"
- **Files:** `game/data/ui/touch_controls.lua`

**Original:** Doors open by themselves when walked through (12.13.1 Doors_auto); no button.

**Remake:** btn_door with the door's own leaf when a door is in reach; F on PC.

### 45. Low-health grey

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20: "Make screen gradually gray when health is low. 50% monkchrome when below 10% health. effect doesn't start until under 75% health."
- **Files:** `game/src/game/systems/render.lua`

**Original:** 6.3.16: Grayscale = 50 - hp for 0 < hp < 50 (world goes 0..50% grey).

**Remake:** render.grey: starts under 75%, 50% at 10% and below.

## Already identical

- HUD art pixel-identical to 1.0 (475 of 510 compared frames): gui_panel plates, gui_blood/water/food/heat_bar strips, gui_bleeding, gui_starving, gui_thirsty, gui_heat, gui_sick, gui_armor + gui_armor_text digits, gui_help gear, gui_btn_inventory, gui_btn_switch (3 frames), gui_btn_zoom, gui_btn_car_shoot, gui_btn_reload, gui_btn_builder, gui_target, aim_line_1, reloading_icon/spinner, gui_wpn_cooldown_bg/fill, gui_dpad_field/stick (tools/atlas_vs_10.py)
- Switch button face follows what is held: frame 0 fist, 1 pistol, 2 rifle (12.26-12.28)
- Focus/zoom button frames: zoom_1 while on, zoom_0 off (8.5.6); camera target is the halfway point
- RELOAD shown only with a pistol or rifle in hand (12.26-12.28)
- Bleeding icon blinks at 1 Hz (6.2.5 Flash 0.5/0.5 every 1 s)
- Starving/thirsty/freezing icons come on at value <= 0 (6.3.21-24, 6.2.2.8)
- Armour number = sum of worn armour; shield shown only when non-zero (12.24.36/37)
- Fill = value% of the track; no number printed on meters
- No ammo counter on the HUD in either
- No visible action log in either (AddLog 12.12 is never called)
- Interact face: the same hand picture on a torn plate; trunk shows the car frame (gui_btn_interact 0 = 3)
- Headshot slow-down flourish exists (SetTimescale 0.3 for 0.2 s + headshoticon; combat.headshot_flourish) -- not compared visually

## Not checked

- report.md was not written: the harness refuses report files from subagents; the full content is in this structured output
- Real phone hardware (notch, DPR, fullscreen); phone results are headless Chromium 844x390 with touch emulation
- DAY label font: headless Chromium lacks Trebuchet MS and renders a serif fallback
- Original timings under load: C2 slows game time below 15 fps; the fade timing was sampled with performance.now on a machine at load average about 9 on 4 cores
- Contents of the Options, pad (map/notes) and perk screens (menus and map areas)
- Vehicle HUD (out of scope per Progress.md), grenade-launcher gui_btn_attack_2, the stalker-mode radiation plate, gui_smoke and the immune sick icon (cigarettes and vitamins are not modelled in the remake)
- Tutorial overlays/tips and achievement toasts (12.17, 11.x)
- HUD button sounds beyond what the events show (pad_open; menu_click on the DAY label)
- The remake's warming arrow and the remake's heat-text equivalent in a live capture: survival resets player.warming every tick, so the code was read instead
- World zoom: the remake shows 320 world px vertically (2.25x at 720p) vs the original's 1:1 (2x in warm zones, 20.1). This changes how the reticle, lines, labels and over-head text read against the screen; it belongs to the camera area
