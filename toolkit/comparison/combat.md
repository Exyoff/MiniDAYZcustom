# combat: the remake against the official 1.0

*Area surveyed:* combat

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

I compared COMBAT in the official Mini DayZ 1.0 against the MiniOutbreak survey checkout (/home/user/wt/survey @ b180a9d, Stage 26, read only, nothing edited). The harness refused to let this subagent write report.md, so /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad/survey/combat/report.md does NOT exist. This output is the full report.

PATH LEGEND (all under /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad):
- O/ = survey/combat/orig/ (original screenshots at 1280x720, plus logs b_log.txt, d_log.txt, d2_log.txt, f_log.txt, f2_log.txt holding state/rect dumps).
- R/ = env/s6/caps/ (remake captures).
- CMP/ = survey/combat/cmp/. Side-by-side images at the SAME world scale. The original draws 1 screen px per world px at 1280x720, shown here x2.25. The remake shows 320 world px of height, which is 2.25 px per world px at 720p.
- EV x.y = an event path in orig/events, readable with `python3 orig/show.py x.y`. Combat excerpts are copied to survey/combat/ev/.
- Capture scripts are in survey/combat/rs/ (remake) and survey/combat/js/ (original page helpers).

WHAT WAS RUN
- Original session b: FNX against a pinned zombie (aim, fire, headshot, reload, empty, ruined, range end, night).
- Original sessions d/d2: fists, then the red axe, with automatic melee, crits and blocks.
- Original session f: an AK-74 held on the attack button through a synthetic pointerdown.
- Original session g: Player_get_hit called directly.
- Remake: aim, fire, headshot, reload, empty with and without ammo, ruined, fist and axe swings, bite, night, AK held.

MAIN RESULT
- data/generated/weapon_stats.lua was generated from the 1.2 FAN MOD, not 1.0. This is the same fault orig/globals.txt found for original_globals.lua. Every melee cooldown, the wear column, FN FAL/RPK 0.4 and Remington/Chigur 0.9 are 1.2 values. Re-running tools/extract_combat_stats.py on /home/user/OriginalData/C2SourceData.js fixes findings 1-3.
- The remake also uses one global dispersion model (0..25, +5/shot, -1/0.1s, +10/0.1s moving). Those are the original's start/melee values, which every gun overwrites on switch (EV 12.30/12.31).
- Most of the original's combat feedback is missing in the remake:
  - muzzle sprite, dust and night flash
  - shells
  - the cooldown bar after single shots
  - per-gun gunshot and bolt-cycle sounds
  - zombie hp bars
  - melee damage numbers and crits
  - the block shield icon
  - the grey hit flash and camera shake
  - ground and obstacle hit puffs and sounds
  - range-tiered headshot power
  - per-weapon noise
- Reticle, aim lines, reload icon, headshot skull and body-hit blood all differ in size, anchor, tiering or behaviour.
- Asked-for differences are listed with verdict keep.
- Bandits, vehicles and perks are 'Explicitly out of scope' in Progress.md and are not listed.
- Ground rule 3 ('Approximate, don't replicate') is overridden by the user's Stage 28 instruction ('identical except for the changes i had asked for specifically').

## Findings

Verdicts: make identical 43, keep (user asked) 11, unclear 3

### 1. Melee swing cooldowns (and fists) are the 1.2 fan mod's numbers

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 18 "melee has no cool down" (asked FOR a cooldown; the original's own values satisfy it)
- **Files:** `game/data/generated/weapon_stats.lua`, `tools/extract_combat_stats.py`, `game/src/game/systems/combat.lua`

**Original:** Swing lock = melee item var#4 (EV 8.7.1.5.1.2.2.2.24: Reloading_time_melee=1, Wpn_cooldown(var#4), Wait(var#4)). 1.0 Map values: Split Axe 0.5, Shovel 0.6, Pipe Wrench 0.3, Bat 0.4, Fireaxe 0.6, Crowbar 0.2, Pickaxe 0.3, Pitchfork 0.4, Sledgehammer 0.8, Sword 0.5, Pan 0.3, Katana 0.5. Fists Wait(0.3) (EV 8.7.1.5.1.2.1.7). The 1.2 mod's Map has 1.5/1.2/1.3/1/1.6/1.2/1.3/1.2/1.8/1.5/0.9/1.2. Seen: O/d1_axe_a..c, O/d2_crit_a..d (axe bar refills in about 0.5 s).

**Remake:** game/data/generated/weapon_stats.lua fire_delay: axe_red 1.5, shovel 1.2, pipe_wrench 1.3, baseball_bat 1, fireaxe 1.6, crowbar 1.2, pickaxe 1.3, pitchfork 1.2, sledgehammer 1.8, sword 1.5, pan 0.9, katana 1.2, fists 0.5. combat.swing waits max(fire_delay, melee_recover_time 0.25). Captures: R/r_melee_8, R/r_melee_30.

### 2. Gun cadence: automatic rates, bursts, tap-rate guns, two 1.2 locks

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/weapons.lua`, `game/data/generated/weapon_stats.lua`, `game/src/game/systems/combat.lua`, `tools/extract_combat_stats.py`

**Original:** Automatics fire while the finger is held at their own Every (EV 8.6.3): M4 0.1, L85 0.12, AK-74 0.1, AKS-74U 0.08, MP5 0.07, UMP 0.09, RPK 0.1, FN FAL 0.11, AKM 0.13, AUG 0.09, Bizon 0.11, Groza 0.1, VSS 0.1, Madsen 0.12, Vector 0.05, MAC-10 0.08. Saiga is automatic at 0.3 (5 pellets, dispersion added twice). AN-94 fires a 2-round burst and M16 a 3-round burst, 0.05 s apart. BM-16, sawed IZH and both bows set no Reloading_time lock (fire as fast as you tap). Remington's lock is 0.3 and Chigur's 0.4 (1.0 var#4). Measured: O/f2_log shows the AK firing about 10 rounds a second.

**Remake:** game/data/weapons.lua + weapon_stats.lua: M4 0.09, L85 0.1, AKS-74U 0.09, AKM 0.11, AUG 0.1, Bizon 0.07, Groza 0.11, VSS 0.15, Madsen 0.09, MAC-10 0.07. FN FAL and RPK are 0.4 (1.2 values, effectively semi). Saiga is semi-automatic at 0.35. R670 and Chigur are 0.9; BM-16 and sawed IZH 0.1; crossbow 1.5; bow 1.6. No bursts. Measured: R/r_ak2 shows 5 rounds in 30 frames.

### 3. Wear per shot is the 1.2 mod's

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/generated/weapon_stats.lua`, `tools/extract_combat_stats.py`

**Original:** 1.0 firing branches (EV 8.6.2.2.x / 8.6.3.x SubInstanceVar(var#6,n)):
- Pistols and sawed-offs: FNX 0.2, Glock 0.3, Colt 0.5, rare Colt 0, Magnum 0.5, Amphibia 0.1, sawed IZH 0.5, sawed Mosin 0.6, sawed Repeater 0.6, PM 0.2, PB 0.3, Deagle 0.2, MAC-10 0.1.
- Rifles, shotguns and bows: SVD 0.6, Mosin 0.8, crossbow 0.8, bow 1, BM-16 1, Remington 0.5, SKS 0.5, Sporter 0.4, Repeater 0.4, SV-98 0.6, Saiga/Chigur 0.5.
- Automatics: AN-94/M16/M4/L85/AUG/Groza/VSS 0.2; AK-74/AKS-74U/MP5/UMP/RPK/FN FAL/AKM/Bizon/Madsen/Vector 0.1.
Measured: O/f2_log shows the AK-74 going from 100 to 99.3 after 7 rounds.

**Remake:** weapon_stats.lua wear: FNX, Glock, rare Colt, Magnum, Deagle, Amphibia, sawed IZH and sawed Mosin are 1. MAC-10 0.4, SVD 0.8. M4/L85/AUG/Groza/VSS 0.4. AK-74/AKS-74U/AKM/RPK/FN FAL/Bizon/MP5 0.3. Madsen 1.25.

### 4. Damage, pellets, bullet speeds and the invented spread_bonus

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/weapons.lua`, `game/data/generated/weapon_stats.lua`, `game/src/game/systems/combat.lua`

**Original:** Bullet templates:
- Damage: Amphibia 20. Deagle 40 (override in its branch). Arrows: handmade 50 at 500 px/s, composite 100 at 550, either arrow in either bow. Chigur fires 5 pellets.
- Speeds: pistol round t186 600; Colt round t259 700; Glock/MP5/UMP/Bizon/M4 family/AKM/Magnum/Deagle/Repeater/Groza/AN-94 800; 12-gauge 700; Sporter 700; VSS 700; Madsen 750; SVD/Mosin/SKS/SV-98 1000; swing 100.
- No per-gun spread bonus. A shotgun's width is its dispersion_default (13-17).

**Remake:** game/data/weapons.lua:
- Damage: Amphibia 15, Deagle 45. Crossbow 50 with composite arrows only, at 450. Bow 50 with handmade arrows only, at 380. Chigur fires 6 pellets.
- Speeds (invented): FNX/Glock 620, rare Colt 640, Magnum/Deagle 700, MAC-10 560, Amphibia 520, sawed IZH 500, sawed Mosin 750, AKM 880, AKS-74U 800, M4 910, AUG/L85/FN FAL 900, Groza 720, VSS 700, RPK 880, Madsen 870, Bizon 560, MP5 600, SKS 870, SVD 930, Mosin 900, SV-98 950, Repeater 800, Sporter 700, shotguns 500-520.
- spread_bonus 0-12 on most guns, added to the cone.

### 5. Magazine size, calibre and reload time

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/weapons.lua`, `game/data/attachments.lua`, `game/data/items.lua`

**Original:** EV 8.10.3 load branches with Mag_reload(time,type), plus the calibre search in 12.13.4 (quickdraw time in brackets):
- Pistols and sawed-offs: FNX 15 x .45 ACP, 2 s. Colt 7. Rare Colt 12. Magnum 6 x .357, 3 s. Deagle 9 x .357, 2 s. Sawed Repeater 7. Sawed Mosin 5, 3 s. Sawed IZH 2, 3 s. Amphibia 10 x .22 LR, 2 s. Glock 18 x 9 mm, 2 s. MAC-10 30 x 9 mm. PM/PB 8 x 9x18.
- Automatics: M4 30, 2.5 (1.75). L85 30, 3 (2). FN FAL 20, 2.5 (1.75). AUG 30, 3 (2). UMP/Vector 30 x .45, 2. Madsen 60, 3. AKM 30, 2.5. RPK 75, 3 (2.25). AK-74/AKS-74U/AN-94 30, 2.5. MP5 30 x 9 mm, 2. Bizon 40 x 9x18, 3.5. VSS 20 x 9x39, 3 (2). Groza 30, 2.
- Bolt and semi rifles: SV-98 10 x 7.62, 3 (2). Mosin 5, 4 (2.8). SVD 10, 3. SKS 10, 3 (2). Repeater 7, 3 (2). Sporter 30 x .22, 2.5.
- Shotguns: BM-16 2, 3 (2). Remington 8, 3. Saiga 15, 3. Chigur 6, 3.
- Bows: 1 arrow of either kind, 2 s.

**Remake:** game/data/weapons.lua (mag / ammo / reload_time):
- Pistols and sawed-offs: FNX 12 / 9x18 / 1.6. Glock 17 / 1.5. Rare Colt 7 / .45. Magnum 2.4. Deagle 7 / .45 / 2.3. MAC-10 .45. Amphibia 6 / 9x39. Sawed IZH 2.4. Sawed Mosin 1 / 2.6.
- Automatics: AKM 2.6, AKS-74U 2.4, AK-74 2.6, AUG 2.6, L85 2.7. FN FAL 30. Groza 20. VSS 10 / 2.4. RPK 45 / 4.0. Madsen 40 / 4.2. Bizon 53 / 2.4. MP5 2.2.
- Bolt and semi rifles: SKS 2.8. Mosin 3.4. SV-98 .308 / 3.2. Repeater 8 / .357. Sporter 10 / 2.2.
- Shotguns: Saiga 8. R670 5. Chigur 2. BM-16 2.2.
- Bows: one arrow type each.
- Quickdraw is a flat x0.7 (data/attachments.lua), not the original's per-gun times.

### 6. Dispersion is one global model instead of each gun's five numbers

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/data/weapons.lua`, `game/src/game/systems/combat.lua`

**Original:** On switching to a gun, EV 12.30 (rifles, var#1) and 12.31 (pistols, var#44) set default/max/pershot/cooldown/run. EV 8.5.2 then ticks every 0.1 s: standing still subtracts cooldown down to default; moving adds run up to max; each shot adds pershot.
- Pistols: FNX 7/20/9/4/2, Colt 6/20/10/4/2, Magnum 6/25/15/2/3, Glock 5/20/10/4/2, Amphibia 4/20/9/4/3, sawed IZH 13/30/20/3/2, sawed Mosin 12/25/25/3/1, MAC-10 10/20/5/2.5/1, Deagle 6/20/13/3/2.
- Rifles and shotguns: Mosin 3/25/20/1/5, M4 4/25/3/1.5/2, BM-16 13/30/20/1/2, AK-74 5/25/4/1.5/2, AKS-74U 8/25/4/2/1.5, SKS 4/25/10/1.5/4, Remington 15/30/20/1/2, SVD 3/25/13/1/5, MP5 10/20/4/2/1, Saiga 17/30/19/1/2, VSS 2/25/4/1.5/2, SV-98 3/25/9/1.5/5.
- The full table is in the original's 12.30/12.31.
- Live readouts: O/b_log disp [7,7,20,9,4,2] (FNX); O/f2_log AK [5,5,25,4,1.5,2] rising to 27.5 while held; O/d2_log melee/start [0,0,25,5,1,10].
- A settled (headshot) round is dispersion == the gun's default.

**Remake:** game/data/config.lua combat: dispersion 0..25, +5 per shot, -1 per 0.1 s still, +10 per 0.1 s moving, for every gun. These are the original's start/melee values; Progress.md 'Reference numbers' presents them as the recovered model. Each gun also adds an invented spread_bonus. Settled = dispersion 0.

### 7. Pistols' dispersion halved

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "Reduce dispersion for pistols."
- **Files:** `game/data/weapons.lua`, `game/src/game/systems/combat.lua`

**Original:** Each pistol uses its own row of EV 12.31 (FNX 7..20, Glock 5..20, Deagle 6..20 ...).

**Remake:** dispersion_scale = 0.5 on every pistol (game/data/weapons.lua, combat.spread).

### 8. Attachment effects are invented multipliers

- **Verdict:** make identical  **Effort:** M  **User's ask:** related: Stage 23 "weapon attachments, not all attachments go on every gun." -- covers which gun takes which mount (keep that table), not the effect numbers
- **Files:** `game/data/attachments.lua`, `game/src/game/attachments.lua`, `game/src/game/systems/combat.lua`

**Original:** Scopes (EV 12.30.1.30-36, by var#17): 1 -> dispersion_default -1; 2/3/4 -> -2; 5 -> -3. A scope also ADDS +5/+15/+25 to the headshot roll (bullet var#9, EV 13.2.8.4.1.2.5.1).
Grips (var#20): 3/4 -> dispersion max -10; 5/6 -> cooldown step x1.3.
Silencer: the shot calls no noice at all (EV 13.6) and plays the silenced sample.
Quickdraw belt: the per-gun shorter reload times (e.g. Mosin 4 -> 2.8, RPK 3 -> 2.25, L85 3 -> 2).

**Remake:** game/data/attachments.lua:
- Scopes multiply spread by 0.8/0.65/0.7/0.55 and headshot chance by 1.5 or 2.
- Silencer multiplies noise by 0.25.
- choke_bor x0.6 has no original equivalent.
- Grips: kick 0.7/0.75, settle 1.5.
- Quickdraw: reload x0.7.

### 9. Bullet range 360 and the puff where a round dies

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/bullet.lua`, `game/src/game/systems/effects.lua`

**Original:** EV 16.2.1: CompareTravelled >= 360. The round is destroyed with a test_groundhit puff (a1, 6 frames at 10 fps, fade) and muzzle_dust at that point. O/b7_range_a shows test_groundhit at +380 px. O/f2_log shows three test_groundhit + muzzle_dust at about +350..370.

**Remake:** cfg.combat.bullet_range = 400. bullet.lua removes the round silently at range: no sprite, no sound.

### 10. Range tiers: headshot power and rounds passing fences/bushes

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/bullet.lua`, `game/src/game/systems/combat.lua`, `game/src/physics/categories.lua`, `game/data/config.lua`

**Original:** EV 16.3 Range_less_power: a round's tier (var#2) starts at 3, drops to 2 after 100 px and to 1 after 220 px. HS_power is 15/10/5 by tier (EV 13.2.8.4.1.2.1-3). An obst_base obstacle stops a round only if its tier <= choose(1,2), so close shots pass fences and bushes.

**Remake:** Flat headshot_chance 15 at any range (data/config.lua). Every WALL fixture stops every round (game/src/physics/categories.lua, bullet.lua).

### 11. Auto-aim candidates: 400 px radius, all enemy kinds, not screen-bound

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/targeting.lua`, `game/data/config.lua`

**Original:** Player_check_targets runs every 0.2 s (EV 8.5.1, 8.5.4.4.1.3). It picks the NEAREST member of the target family (zombies, wolves, deer, rabbits, bandits) with PlayerAim LOS within 400 px, regardless of the screen. The world view is 1 layout px per CSS px (data.js project fullscreen mode 1 = crop; only GUI layers are scaled, EV 1.2.1.1): 1280x720 world px on this desktop, the phone's CSS size on a phone.

**Remake:** game/src/game/systems/targeting.lua candidates: zombies only, on screen inside edge_margin 40, sight-tested. With a 320-world-px-tall view that is about +-244 x +-120 px at 16:9.

### 12. Reticle size and anchor

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`, `game/data/config.lua`

**Original:** gui_target 40x40 at native size, pinned to the target's origin, i.e. the body centre (EV 8.5.4.4.1.3.1 SetPosToObject(target,0)). Rect: Sunset_Dawn gui_target 688,340 40x40, with the zombie's feet at about y 372. Seen in CMP/cmp_aim, CMP/cmp_fire, CMP/cmp_auto.

**Remake:** status.draw draws gui_target at reticle_scale 0.7 (28x28), centred on the target's feet. In CMP/cmp_aim the ring sits on the ground under the zombie. Also R/r_aim.

### 13. Reticle / aim-line colour tiers

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`

**Original:** EV 8.5.2.3, every 0.1 s, relative to the gun's own default and max:
- <= default: frame 4 green
- default .. max/3-1: frame 3 yellow-green
- max/3 .. 2max/3: frame 2 yellow
- 2max/3 .. max-1: frame 1 orange
- >= max: frame 0 red
The reticle and both lines always share the frame.

**Remake:** status.reticle_tier = round((1 - total spread/25) * 4), where the spread includes spread_bonus and the pistol halving. status.cone_tier = green only when settled, otherwise quarters of dispersion/25. The two disagree: R/r_head_4 and R/r_fire_1/3/8 show a yellow cone under a green reticle. At the same moment the original showed a red/orange reticle with yellow lines (CMP/cmp_head, CMP/cmp_fire).

### 14. Original reticle intermittently resets to red

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`

**Original:** Player_check_targets restarts gui_target's animation from frame 0 (red) every 0.2 s (EV 8.5.4.4.1.3.1 SetAnim("firearm", opt:1)). EV 8.5.5 also sets frame 0 once when the shot lock ends. EV 8.5.2 re-tiers it on its next 0.1 s tick. Captured red in O/b1_fire_t0 and O/b3_head (FNX dispersion 16 of 20 should read orange), orange in O/f1_auto_a..c, green in O/b0_aim.

**Remake:** No reset; the tier is computed every draw.

### 15. Aim lines: length, start, height and opacity

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 20 "There's 3 sprites that look like lines, red yellow and green. Use them like a cone to display dispersion." and Stage 22 "the aiming lines should only appear when there's an actual target." -- the original's own lines are that cone; only the geometry differs
- **Files:** `game/src/game/systems/status.lua`, `game/data/config.lua`

**Original:** Two aim_line_1 sprites (3x2 frames stretched to 139 px), opacity 0.5. Rects: aim_line_1[t758] 646,343 139x14 op 0.50 and [t759] 646,363. They are pinned at the player's body, turned to the target +- dispersion, start about 46 px from the player's centre and end about 185 px out, past the target. Visible only with a target (EV 8.5.4.4.1.3.1 / 1.4). Seen in CMP/cmp_aim (faint lines beyond the zombie) and CMP/cmp_auto (wide translucent orange).

**Remake:** status.cone / status.draw_aim: two lines from the body box edge (min muzzle_offset 11) to cone_reach 72 px from the FEET, at cone_height 4, opacity 1, never past 72 px. At 0 dispersion both coincide as one solid bar: CMP/cmp_aim, R/r_aim.

### 16. Melee in hand still shows the firearm reticle

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`

**Original:** With melee held, gui_target is hidden (EV 8.5.4.4.2.1.1 SetVisible(0), anim "melee"; O/d2_log target vis:false) and the aim lines are hidden (8.5.4.4.2). The red gui_meleefight_ring_ramka (46x46) is pinned to gui_target (EV 2.11). Switch_to_melee shows it and immediately hides it again (EV 12.28 / 12.28.4), so the ring is seen only from run start until the first switch (O/d0_fist_a/b: ring round the zombie; O/d1_axe_a: none).

**Remake:** The green firearm reticle is drawn at the target's feet whenever a target exists, melee or not: R/r_fist_4, R/r_melee_3/8/30, CMP/cmp_melee.

### 17. Focus (zoom) camera: exact midpoint, no cap, no easing

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 20 "another button to focus on the locked target, should be in gui_btn_zoom-sheet0." -- the button is the original's own; its behaviour was not asked to differ
- **Files:** `game/src/game/systems/targeting.lua`, `game/data/config.lua`

**Original:** EV 8.5.6 / 8.5.8 (isAim = 1 via gui_btn_zoom): the camera is placed at the exact midpoint of player and target every tick.

**Remake:** targeting.update_focus leads by half the offset, capped at focus_reach 0.5 of the half-screen per axis, and eases at focus_ease 4/s.

### 18. No muzzle sprite, muzzle dust or night muzzle flash

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`, `game/data/config.lua`

**Original:** Every shot (EV 8.6.2.2.x): muzzle_sprite (3 frames at 24 fps, Fade) at the muzzle (O/f2_log muzzle_sprite 615,351 10x10 x3; O/b3_head) and muzzle_dust particles. muzzle_flash (night layer, Overlay effect, fade 0.2) shows at night, and always for SVD, Mosin, SV-98, sawed Mosin and sawed Repeater.

**Remake:** None at any hour: R/r_fire_1, R/r_ak_8/30, R/r_night_1/3, CMP/cmp_auto, CMP/cmp_night.

### 19. No spent shells

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`

**Original:** test_shell (test_shell_12cal for 12-gauge) is spawned at the player +-2 px per shot: 7 frames at 10 fps, stays 10 s, fades 2 s. None for Magnum, BM-16, sawed IZH and bows. Bolt and pump guns eject at the end of the cycle. O/f2_log lists 14 shells after 0.65 s of AK fire; yellow specks in O/f1_auto_a..c and O/b3_head.

**Remake:** None: R/r_ak_30, R/r_fire_8.

### 20. No cooldown bar after a single-shot gun's shot

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 20 "use gui_wpn_cooldown_bg-sheet0.png for any interaction, such as eating etc." -- extends the bar to uses; it did not ask to drop it from guns
- **Files:** `game/src/game/systems/status.lua`, `game/src/game/systems/combat.lua`

**Original:** Wpn_cooldown(var#4) (EV 8.6.4/8.6.5) runs after every single-shot gun's shot (not automatics) and after every swing. gui_wpn_cooldown_bg 30x3 sits at the player's centre -19; the fill follows a Sine of period 4 x cooldown and hides when full. Seen: O/b8_night1-2 (FNX), O/d1_axe_a..c.

**Remake:** status.draw_head shows the bar only for melee recovery and timed uses, never after a shot.

### 21. Gunshot sounds per weapon (and levels)

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/systems/combat.lua`

**Original:** Each gun has its own sample and dB offset:
- Pistols: FNX_1/2, glock19_single_1/2, colt_1/2, magnum_single_1, amph_1/2 (+5), pm_shot, pb_shot_1/2, deagel_1/2 (+10), mac10_1/2.
- Shotguns: izh_1/2, remington_1/2, saiga12 (-5), chigur_shot.
- Bolt/semi: mosin_1/2/3, repeater, svd_single_0/1 (-10), SKS_close_0/1, sporter_22, sv98_shot_1, Bow_1.
- Automatics: an_shot_1/an_silence_1, m4_single_1/2, M16_Shot, m4_silent_1/2, l85, Ak74_1/Ak74_3, AK_silent_1/2, mp5k_single_0/1 (-5), ump45_single_1/2 (-5), akm_3, fn_fal_single_1/2 (-5), akm_5, steyraug_shot_0/1, bizon_shot_1, groza_shot_1/2, vss_shot_3, madsen_shot (-5), vector_1/2/3 (-5).
- Dry: empty_click.

**Remake:** game/data/sounds.lua + combat.weapon_class use four classes by id:
- every *pistol* plays the FNX sound
- every *rifle*/ak* plays ak74_1/2/3
- every *shotgun* plays izh
- everything else (MP5, Bizon, Madsen, Mosin, Amphibia, sawed-offs, crossbow, bow) plays glock19
Plus silenced east/nato. All 279 original samples are already in the build.

### 22. No bolt / pump cycle sounds

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/systems/combat.lua`

**Original:** After the shot, mid-cooldown: mosin_cycle at T/5, shotgun_pump at T/3, repeater_cycle at T/4, sniper_cycle at T/4.

**Remake:** None.

### 23. Noise radius per gun; melee and silenced shots are silent

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/data/weapons.lua`, `game/src/game/systems/combat.lua`, `game/data/attachments.lua`

**Original:** noice (EV 13.6) radius by weapon:
- 600: SVD, Mosin, BM-16, Remington, SV-98, Magnum, sawed IZH, sawed Mosin
- 500: most automatics and pistols, SKS, AN-94, M16
- 400: Repeater, PM, Deagle
- 100: Sporter
- None: bows, Amphibia, PB, VSS, any silenced shot, and melee (no noice call in EV 8.7)
Zombies walk to the noise +-50 px for 2 s.

**Remake:** cfg.combat gunshot_noise_radius 600 for every gun. Silencer x0.25 (150). melee_noise_radius 150 on every swing (combat.swing).

### 24. Body-hit blood: where, which anim, how many

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 20 "whenever a hit connects to a zombie, add blood effects and a hit sound effect." -- the original's own test_bodyhit + body_1/2 are that; the ask names no different look
- **Files:** `game/src/game/systems/effects.lua`, `game/data/config.lua`

**Original:** EV 13.2.8.4: every round or swing that hits spawns test_bodyhit[t528] AT THE ROUND (layer static_cars). It picks anim a1 (6 frames) or a2 (5, 10x10) at random and an angle of 0/90/180/245, with Fade wait 0.3 / fade 0.2. One per round, so each shotgun pellet makes one. O/f2_log shows bursts at +79 and +120 px; O/b3_head2 shows the scatter beside the body.

**Remake:** effects.hit: one per BODY (restarted), always a1, never rotated, centred hit_height 14 above the feet, played once at 10 fps, no fade. Seen: R/r_melee_8, R/r_fire_8, CMP/cmp_melee.

### 25. Hit sound level and hearing gate

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/sounds.lua`

**Original:** body_1/body_2 at -15 dB, played at the zombie, only if it is within Player_Hear_radius (EV 13.2.8.4.3).

**Remake:** impact.hit/kill/headshot = body_1/2 at impact_volume, at the impact point, no hearing gate (combat.record_hit).

### 26. Headshot skull: frame, motion, mirroring, blood spray

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/effects.lua`, `game/data/config.lua`

**Original:** EV 13.2.8.4.1.2.5.1.4 spawns headshoticon at the zombie's origin on Sunset_Dawn at its default frame 0, the HUMAN skull (frame 1 for wolves, 3 for deer). Fade wait 0.4 / fade 0.1. No movement, never mirrored, no particles: headshot_particles has no instance and is never created. Seen: O/b1_fire_t0, O/b3_head, O/b3_head2, CMP/cmp_head.

**Remake:** effects.headshot + cfg.effects: frame 1 (the rotten skull with diamond eyes) for zombies. Mirrored for a westward round. Rises 6 px over 0.6 s with a 0.2 s fade. An invented spray of 8 headshot_particles chunks. Seen: R/r_head_4, R/r_head_14.

### 27. Headshot chance with a scope is additive, not multiplied

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/attachments.lua`

**Original:** round(random(100)) < HS_power + var#9, where var#9 is +5/+15/+25 by scope; +10 with the perk (out of scope).

**Remake:** headshot_chance x head (1.5 or 2) in combat.headshot, with head from data/attachments.lua.

### 28. No zombie health bars

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/status.lua`, `game/src/game/zombie.lua`

**Original:** Every zombie gets npc_hp_bar_bg + npc_hp_bar (30x6), pinned 25 px above its centre from spawn (EV 13.1.1.1.x). Width = hp share, updated in NPC_get_damage. Visible in O/b0_aim, O/d1_axe_a, O/f1_auto_a and every other zombie shot.

**Remake:** None on zombies: R/r_aim .. R/r_melee_30.

### 29. No damage numbers on melee hits

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/effects.lua`, `game/src/game/systems/combat.lua`

**Original:** dmg_hint (EV 12.29), called for every MELEE hit (EV 13.2.8.4.1.1). Text[t536] (12pt Arial with Outline effect) spawns at the zombie on layer Rain as "-N" in rgb(255,196,68), flying away from the player (Bullet about 40 px/s) and fading. A crit is 18pt red "Crit -N" (Arr[t543].At(336)). Seen: O/d0_fist_* "-10", O/d1_axe_a "-59", O/d2_crit_b "Crit -75" x3.

**Remake:** Nothing: R/r_melee_8, CMP/cmp_melee.

### 30. Every zombie screams on death

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`, `game/src/game/systems/combat.lua`

**Original:** A normal zombie killed by a round or swing makes no death sound. zed_tank_scream is used only by the tank (EV 13.3.1.1.7.1.1 at -10 dB, 13.3.8.4) and the shooter (13.3.6.1.1.13.1.1).

**Remake:** game/data/sounds.lua zombie.death = { "zed_tank_scream" } for every zombie.

### 31. Hit on an unaware zombie: lurch vs asked slow

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 22 "hitting a zombie should slow it down for a second." (covers the slow; whether the lurch should also stay is not said)
- **Files:** `game/src/game/systems/ai.lua`

**Original:** warn_zed(uid,0,0,1,archetype) on every hit (EV 13.2.8.4): an unaware zombie lurches in a random direction for 1 s.

**Remake:** A hit slows the zombie to half speed for 1.0 s (asked, Stage 22). No lurch.

### 32. Rounds hitting walls/obstacles: no puff, no sound, no arrow recovery

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/bullet.lua`, `game/data/sounds.lua`, `game/src/game/systems/effects.lua`

**Original:** EV 16.1.1: a round stopped by an obstacle spawns test_bullet_hit (4 frames at 15 fps, fade 0.2) and plays hard_ground_1/2 (tag ground_hit). Arrows that hit drop back as an item: 1 in 4 for the bow, 1 in 2 for the crossbow (EV 16.1.7).

**Remake:** impact.wall = {} (silent), no sprite, arrows are never recovered (bullet.lua, data/sounds.lua).

### 33. The swing: one target, the original's swoosh motion, Punch_Swipes for every weapon

- **Verdict:** make identical  **Effort:** S  **User's ask:** related: Stage 19 "make it so that using your melee displays a white swoosh sprite from the asset. It should go forward like a wave." (the original's own swing sprite does exactly that) and Stage 22 "Less range on melee." (keeps the 22 px reach)
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`, `game/data/config.lua`, `game/data/sounds.lua`

**Original:** A swing spawns melee_swing_general (10x21 white translucent crescent) at the player's collision origin on player_weapons, aimed at gui_target. It moves at 100 px/s (Bullet) with Fade wait 0.1 / fade 0.1. It is a fam_m4_bullet destroyed on its first hit (EV 13.2.8.4.5), so ONE zombie per swing. It plays Punch_Swipes_0/1 for fists and every weapon (EV 8.7.1.5.1.2.1, .2.2.2).

**Remake:** combat.swing hits EVERY zombie in a 90-degree wedge of melee_range 22 once. effects.swoosh runs from 8 to 22 px (70 px/s) at swoosh_height 12 over 0.2 s, fade 0.1. The axe plays chop/chop2-4 (data/sounds.lua melee.axe). Seen: R/r_melee_3, R/r_fist_4.

### 34. No melee crits

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/generated/weapon_stats.lua`, `tools/extract_combat_stats.py`

**Original:** EV 8.7.1.5.1.2.2.2.17-22 crit chance by weapon, damage doubled:
- 10%: knife 9
- 15%: knife 8, shovel, pickaxe, pitchfork, sledgehammer
- 20%: knife 7
- 25%: Split Axe, Pipe Wrench, Bat, Fireaxe, Crowbar
- 35%: Sword, Katana
- 40%: Pan
Shown red "Crit -N": O/d2_crit_a..d "Crit -81/-110/-90/-75".

**Remake:** No crits (combat.roll_damage only).

### 35. A ruined melee weapon still swings (for 15) and still blocks

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`

**Original:** Condition <= 0 still swings, for 15 damage (EV 8.7.1.5.1.2.2.2.23). The block roll reads only the weapon kind (EV 15.3.1), so a ruined weapon still blocks.

**Remake:** combat.swing refuses with a dry click when not intact(stack). combat.block_chance gives 0 for a ruined weapon (scenario 'bare fists and a ruined melee weapon turn nothing while held').

### 36. No grey hit flash and no camera shake when the player is hit

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (separate from the asked low-health grey, which stays)
- **Files:** `game/src/game/systems/combat.lua`, `game/src/core/camera.lua`

**Original:** EV 15.3.5 / 15.4: a landed hit calls Hit_effect (layout effect 'Hit_effect', a Grayscale, set to 100 for 0.2 s), ScrollTo.Shake(3, 0.4), and body_1/2 at -5 dB on the player.

**Remake:** combat.hit_player: blood burst + body sound only. No flash, no shake: R/r_bite_62/70/90.

### 37. No block feedback

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`

**Original:** EV 15.3.6.2: a blocked melee hit spawns the green shield_icon (10x10) on the player, moving up (Bullet angle 270, about 25 px/s) and fading. O/d2_crit_b..d show two shields rising over the player.

**Remake:** combat.record_hit(..,"block") with impact.block = {}: nothing seen, nothing heard.

### 38. Bleeding from a hit lasts 60 s; a fresh zombie always opens one

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/systems/survival.lua`

**Original:** EV 15.3.5.2-3: 1 hit in 4 (choose(1,2,3,4)=1) starts the 'Bleed' timer for 60 s (30 with the metabolism perk). A zed_fresh hit always bleeds.

**Remake:** cfg.survival bleed_duration 30, bleed_chance 0.25 for every bite.

### 39. A landed hit tears a worn garment

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/inventory.lua`

**Original:** EV 15.3.5.10: dmg_randomizer = choose(2..7). Values 3..7 pick one worn garment (player vars #8, #10, #18, #12, #20) and take round(random(7,13)) off its condition, so 5 hits in 6 damage something.

**Remake:** Nothing worn loses condition when hit (combat.hit_player / survival.damage).

### 40. Armour's floor rule

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`

**Original:** EV 15.3.2: Dealt = damage - the four armour values; only if Dealt < 1 is it set to 2 (so 1.5 stays 1.5).

**Remake:** combat.hit_player: with any armour and a hit above 2, max(2, amount - armour), never under 2.

### 41. Reload: when rounds load, what is locked, three-stage sounds

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/sounds.lua`, `game/src/game/player.lua`

**Original:** EV 8.10.3 + 8.3: rounds are ADDED at the start of a reload. The stack is taken from hands -> pants -> outerwear -> vest -> backpack (12.13.4.1.2-6). Mag_reload then locks firing (Reloading_mag) for the time and plays three stages at -5 dB:
- type 0: pistol_magout/magin/charge
- type 1: ak_magout/magin/chamber
- type 2: m4_*
- type 3: shotgun_reload x6 + pump
- smg_*, sniper_*, mosin_*, magnum_reload
- bows silent
Switching is refused while Reloading_mag = 1.

**Remake:** combat.reload / tick_reload count and load the rounds at the END. One sound at the start (reload_pistol, reload_rifle or shotgun_reload_1/2). combat.equip (every switch) cancels a reload in progress and zeroes the shot lock (e.fire_cooldown = 0). Seen: R/r_reload_20/60.

### 42. Reload icon over the head: size and height

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the Stage 20 'add a reloading icon over it' is about the locked ammo on the board)
- **Files:** `game/src/game/systems/status.lua`, `game/data/config.lua`

**Original:** EV 8.3.1: reloading_icon (25x36, two magazines) at native size, pinned to the player, with reloading_spinner (47x47, 19 frames at 19/T fps) round it. The ring's centre is about 80 px above the feet: O/b4_reload0/1, CMP/cmp_reload.

**Remake:** status.draw_head draws both at reload_scale 0.6 (15x22 and 28x28), ring centre about 46 px above the feet (lift 32): R/r_reload_20/60, CMP/cmp_reload.

### 43. Missing lines: 'Weapon ruined!', 'Weapon fully loaded.', 'I have no ranged weapon.'

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/player.lua`

**Original:** A ruined gun's trigger says "Weapon ruined!" in red (Arr[t544].At(11), EV 8.6.2.2.1.1; O/b6_ruined, O/b7_range_a). Reloading a full gun says "Weapon fully loaded." in white (At(32), EV 8.10.3.11.1.1). Switching with no gun says "I have no ranged weapon." (At(73), EV 12.13.14.1.2.2).

**Remake:** A ruined gun only clicks (combat.fire; R/r_ruined, CMP/cmp_ruined). combat.reload returns false silently on a full magazine. The switch says nothing.

### 44. Colour and stacking of the lines over the head

- **Verdict:** unclear  **Effort:** S  **User's ask:** related: Stage 20 "have a text say above the player \"It's empty\", and if there's no ammo \"I don't have ammo for this\"" -- the words are dictated, colour and stacking are not
- **Files:** `game/src/game/systems/status.lua`, `game/src/game/systems/combat.lua`

**Original:** client_log draws each line in its colour (yellow "Mag empty.", red "No more ammo."/"Weapon ruined!", white "Weapon fully loaded."), stacked over the player with the newest lowest; a dark outline appears as it settles. Seen: O/b5_empty, O/b6_ruined; table in survey/hud/client_log_calls.txt.

**Remake:** One white line with a shadow (status.draw_say): R/r_empty, R/r_empty2.

### 45. Switching: 0.1 s delay, refused mid-reload, does not clear the shot lock

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the cycle ORDER is asked, see the separate keep finding)
- **Files:** `game/src/game/player.lua`, `game/src/game/systems/combat.lua`

**Original:** EV 12.13.14 + 12.26-12.28: the switch takes 0.1 s and is refused while reloading. With no gun it says "I have no ranged weapon." and the button sits at 50% opacity. The gun lock (Reloading_time) is global and survives a switch.

**Remake:** player.hold / combat.equip: immediate, allowed mid-reload. It cancels the reload and sets fire_cooldown = 0, so Q-Q skips a slow gun's lock.

### 46. The run starts with an FNX in hand

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/player.lua`

**Original:** A new survivor has no weapon: fists, mode 0 (EV 2.13.1.1; O/d_log before any give: v2 -1, v43 0). Weapons come from loot.

**Remake:** game/src/game/player.lua (about lines 258-268) equips a loaded fnx_pistol on every new player: every R/ capture starts with it up.

### 47. Guns, knives, launchers and throwables the remake does not have

- **Verdict:** make identical  **Effort:** XL  **User's ask:** none
- **Files:** `game/data/weapons.lua`, `game/data/generated/weapon_stats.lua`, `game/data/items.lua`, `game/data/attachments.lua`, `game/src/game/systems/combat.lua`

**Original:** objects.txt family fam_ak74_rifle includes, beyond the remake's set:
- Colt (pistol enum 2), PM (11), PB (12), sawed Repeater (10), UMP (rifle enum 21), AN-94 (31), M16 (33), Vector (34). These are drawn with other guns' art (fnx_pistol[t71/t139/t140], sawed_mosin[t126], mp5_smg[t117/t149], ak74_rifle[t143], m4_rifle[t148]); the enum-to-type pairing was not traced one by one.
- Knives as melee weapons ('Knife-melee-1/2/3' -> var#4 8/9/7 = 20/19/21 damage, EV 12.10.17.41-43).
- The GP-25 / M203 launchers (gui_btn_attack_2, Mag_reload type 4).
- Grenades, molotovs, claymores, landmines, beartraps.

**Remake:** game/data/weapons.lua has 47 entries and none of these. data/items.lua 'NOT DESCRIBED' lists the knives, launchers and throwables as waiting on systems. Overlaps the items area.

### 48. Melee reach is a 22 px wedge

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 22 "Less range on melee."
- **Files:** `game/data/config.lua`

**Original:** The swing is a bullet from the body that hits on contact (see the swing finding); auto-melee triggers on body contact (EV 8.7.1.1).

**Remake:** cfg.combat.melee_range 22 (was 34).

### 49. Switch order Melee -> Pistol -> Rifle

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "switch button goes from Melee, Pistol then Rifle and back to melee."
- **Files:** `game/src/game/player.lua`

**Original:** melee -> rifle -> pistol -> melee (EV 12.13.14).

**Remake:** player.lua CYCLE {melee, pistol, rifle}.

### 50. Melee by button (fist sprite) instead of automatic on contact

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "for the attack / fire button, show weapon trigger sprite, gui_btn_attack, when holding a pistol or rifle; and a fist sprite if holding melee."; Stage 19 "remove right click as melee"
- **Files:** `game/src/game/systems/combat.lua`, `game/data/input.lua`

**Original:** Melee is automatic while melee is held and an enemy touches the player with LOS (EV 8.7.1.1-8.7.1.5). The attack button is hidden in melee mode (EV 8.7.1.6).

**Remake:** The attack button / Space swings with melee held (combat.swing). There is no auto-melee and no mouse button.

### 51. Only the fire button / Space fires

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 18 "Only the fire button should fire the weapon, and on pc, only space bar. If there is no target, the weapon will fire toward whatever direction the player is facing."
- **Files:** `game/data/input.lua`, `game/src/game/systems/combat.lua`

**Original:** On-screen attack button only (touch, held for automatics; on desktop the mouse on that button); no keyboard fire at all (only WASD exist). With no target the shot already goes along the facing (gui_target 1500 px ahead, EV 8.5.4.4.1.4). On a phone the original fires from the same button.

**Remake:** Space on PC + the touch button; with no target, along the facing.

### 52. 'It's empty' / 'I don't have ammo for this' instead of 'Mag empty.' / 'No more ammo.'

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "remove ammo counter, instead, have a text say above the player \"It's empty\", and if there's no ammo \"I don't have ammo for this\". Don't auto reload when firing an empty weapon."
- **Files:** `game/src/game/systems/combat.lua`

**Original:** "Mag empty." yellow on an empty trigger (EV 8.6.2.2.1.2). "No more ammo." red when the reload search finds nothing (EV 12.13.4.1.6.1). No auto reload either. Seen: O/b5_empty, CMP/cmp_empty.

**Remake:** "It's empty" (rounds in the bag) / "I don't have ammo for this" (none) over the head; no auto reload. Seen: R/r_empty2, R/r_empty.

### 53. Target-switch button (and the sticky target it needs)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "the red circle sprite for target switching, gui_btn_car_shoot_sheet0, it should only show if there's an enemy on screen."
- **Files:** `game/src/game/systems/targeting.lua`

**Original:** 1.0 has no manual target switching: manual_target_on is never set, and the nearest target is re-picked every 0.2 s (EV 8.5.4.4.1.3.1).

**Remake:** gui_btn_car_shoot cycles targeting.next. targeting.update keeps the held target while it is a candidate (sticky), which the switch needs.

### 54. Gun lowered without a target, raised at one

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "When there's no target, the weapon is lowered; when there's a target, the weapon is put up and aimed."
- **Files:** `game/src/game/player.lua`

**Original:** The aim pose (pistol_* anims) is set only while standing with a target on screen (EV 8.5.4.4.1.1.1).

**Remake:** Raised only at a target (player.pose).

### 55. Low-health grey screen

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "Make screen gradually gray when health is low. 50% monkchrome when below 10% health. effect doesn't start until under 75% health."
- **Files:** `game/data/config.lua`

**Original:** None. The only grey is the 0.2 s Hit_effect flash on a hit (see the hit-flash finding).

**Remake:** The world greys from 75% hp down to 50% grey at 10% (cfg.health_grey).

### 56. Bleeding drops stay on the ground

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "when bleeding, add blood drops that stay on the ground."
- **Files:** `game/src/game/systems/effects.lua`, `game/data/config.lua`

**Original:** blood_drop has a Fade behaviour (objects.txt t251).

**Remake:** Drops kept, newest 120 (cfg.effects drop_cap), never fade.

### 57. Cooldown bar also for timed uses

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 20 "use gui_wpn_cooldown_bg-sheet0.png for any interaction, such as eating etc."
- **Files:** `game/src/game/systems/status.lua`

**Original:** gui_wpn_cooldown_bg/fill only for weapon cooldowns (EV 8.6.4).

**Remake:** The same bar over the head for eating, bandaging, making, lighting.

## Already identical

- Damage roll: round(dmg + random(-dmg/4, dmg/4)) (EV 13.2.8.4) = cfg.combat damage_variance 0.25 (combat.roll_damage).
- Headshot rules: only a round fired settled can roll (bullet var#8 vs b.settled); x2 damage; SetTimescale(0.3) + Wait(0.2) game time = headshot_time_scale 0.3 for headshot_slow_time 0.2 of world time.
- Base bullet damage for every gun except Amphibia, Deagle and the arrows.
- Single-shot locks (item var#4) for every pistol and bolt/semi rifle except Remington and Chigur.
- Melee damage per weapon: Split Axe 50, Shovel 25, Pipe Wrench 30, Bat 35, Fireaxe 60, Crowbar 25, Pickaxe 44, Pitchfork 44, Sledgehammer 40, Sword 75, Pan 33, Katana 65, fists 10 (EV 8.7.1.5.1.2.2.2.1-16) = weapon_stats.lua.
- Block chance by melee weapon while melee is held, never against bullets or tanks: Wrench/Bat/Crowbar 15, Split Axe/Fireaxe/Sword 25, Shovel/Pickaxe/Pitchfork/Sledgehammer/Pan 35, Katana 0, fists 0 (EV 15.3.1, 15.3.3, 15.3.4) = weapon_stats.lua block (except the ruined-weapon case).
- No melee while a gun is held: the original's auto-melee also runs only with melee held (var#3 = 0, EV 8.7.1.5.1).
- Aim lines exist only while there is a target (EV 8.5.4.4.1.3.1 / 8.5.4.4.1.4).
- No auto reload on an empty trigger (original only says 'Mag empty.', EV 8.6.2.2.1.2).
- With no target a shot goes along the facing (EV 8.5.4.4.1.4 puts gui_target 1500 px ahead).
- Melee wear per HIT, not per swing; Split Axe 0.5 and Pan 0.2 per hit match (EV 13.2.8.4.1.1.1.1-4).
- Swing cooldown bar position: original 30x3 bar at player centre -19, remake bar bottom 32 px above the feet -- same spot within 2 px (CMP/cmp_melee).
- Headshot skull size (native 37x26) and height over the head within about 6 px (CMP/cmp_head).
- empty_click on an empty or ruined trigger.
- Target needs line of sight (PlayerAim LOS vs physics.sight_blocked).

## Not checked

- report.md was not written: the harness refused report files from this subagent, so this output is the report.
- The reticle's red reset (EV 8.5.4.4.1.3.1 / 8.5.5) was not sampled frame by frame; whether it visibly blinks in play is unmeasured.
- Hit_effect grey flash and camera shake were read from events only. O/g1_hit_0..2 (Player_get_hit called directly) show no grey frame, probably because the WebGL layout effect does not render in the headless runner.
- Only the FNX (session b) and AK-74 (session f) were fired in the original. Other guns' muzzle points, shell kinds, cycle sounds and bullet sprites come from events and objects.txt only. Per-gun bullet sprites (pistol_bullet / akm_bullet ...) were not compared.
- Shotgun pellet spread, the Saiga's double dispersion and the AN-94/M16 bursts: events only, not run.
- Sounds were compared by sample name and dB offset, not by listening. Positional audio falloff (PlayAtObjectByName 360/360/5) vs the remake's distance model belongs to the audio area.
- Remake bullet range end and wall hits were checked in code (bullet.lua), not captured.
- Player auto-turn to the target while standing (EV 8.5.4.4.1.1.1 / 8.7.1.5.1.1) and the aim/shoot/swing animations belong to the character area.
- Attack/switch/reload/zoom button art, visibility and placement belong to the HUD area (e.g. 1.0's attack button is a red crosshair, hidden in melee mode, EV 8.7.1.6).
- Zombie attack timing, per-archetype damage and bite sounds belong to the zombies area.
- Loot belongs to the loot area: half of found guns carry round(random(15)) rounds, and ammo stacks such as .45 ACP 10-20.
- World view size belongs to the camera area. The original draws 1 world px per CSS px (1280x720 on this desktop, the phone's CSS size on a phone); the remake shows 320 world px tall at every size. This changes how far auto-aim reaches.
- Wolves, deer and rabbits as targets, and their headshot frames: not in this Stage 26 checkout (Stage 28 fauna is later).
- Bandits, vehicles (in-car aim EV 8.5.3 / 8.5.4.3) and perks (Crit_bonus, perk_header, perk_meleedef, perk_metabolism) are Explicitly out of scope in Progress.md and were not listed.
