# zombies: the remake against the official 1.0

*Area surveyed:* Zombies (kinds, stats, AI/senses, sounds, animations, death/corpses, spawning/population, day/night, difficulty)

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

I compared the official 1.0 build (read and run) against the remake as published (Stage 26, read and run) and found 36 differences: 30 to make identical, 3 to keep because the user asked, 3 unclear. The harness refused to write S/survey/zombies/report.md ("subagents should return findings as text"), so this structured output is the full report. S = /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad.

**What I read.** Original event groups 13.1, 13.2.x, 13.3.1-13.3.9, 13.6, 13.7, 2.11, 3.3/3.5.1/3.16, 7.3.1, 8.6, 12.29, 15.3/15.4, 17.1, 17.8, 20.6 and 20.7. I also read the behaviour properties of the placed instances straight from data.js: MainLook [1,335,180,1], ShortLook [1,70,360,1], Turret [335,0,1,600,...], corpse Fade [1,0,60,5,1] and the Audio plugin settings.

**What I ran in the original.** Headless at 1280x720; shots and logs are in S/survey/zombies/orig/ (o1, o2, o5, o6 .log/.png and zoom_*.png). Timings use the runtime's own game clock, because headless the game runs at about 0.52x real time.
- Bite damage read live: Regular 9 / 13 / 16 (normal / army / runner), Novice 5 / 8 / 16.
- Chase speeds sampled live: 92 / 94 / 130 px/s.
- Sight read live: a 180-degree cone; at night a zombie sees you only within 175 px.
- A zombie that loses sight stops where it is within a second.
- Zombies stack on top of each other and on the player.
- Corpses fade from about 59 s and are gone by about 71 s.
- Idle zombies wander about 60 px in 3 s.

**What I ran in the remake.** S/env/s5/caps/q1_idle, q2_contact, q3_corpses, q4_night and q5_lost (.png and .log), plus the earlier z_r2_chase_measure.log and z_r6_world.log. Side-by-side images are in S/survey/zombies/cmp/.

**The biggest gaps, to make identical:**
- **Numbers:** chase speeds are 90-100 / 90-100 / 120-140, not 80-90 / 90-100 / 100-110 (those are the original's warn speeds). Runners hit for 16 on every difficulty. Bite damage depends on difficulty.
- **Sight:** the cone is 180 degrees and is checked once a second. Night limits sight to 175 px unless you stand in light. Losing sight stops the zombie at once. A zombie that spots you alerts every zombie within 400 px.
- **Movement and hearing:** idle zombies random-walk instead of staying near their spawn point. Zombies and the player do not collide with each other. Footsteps make noise; melee does not.
- **Looks:** every zombie has a health bar, melee hits show damage numbers, corpses last 60 s and half of them are mirrored, and zombies drop loot.
- **Sounds:** no death cry; the spotted list should not include jumper_spot.
- **Population:** zombies come in groups of about 5 per spawn point and are only ever created 1500-2000 px from the player.
- **Missing kinds:** buried, screamer, shooter/spitter, tank, jumper and skins 7-11 are absent (the largest piece of work).

**Kept because the user asked:** the blow sound (Stage 28), the slow on hit, and the wander groans. Infection on VETERAN is also asked for; it is listed under "already identical" for that reason.

## Findings

Verdicts: make identical 33, keep (user asked) 3, unclear 2

### 1. Chase speeds: normal 90-100, army 90-100, runner 120-140

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/data/zombies.lua`, `game/src/game/systems/ai.lua`, `Progress.md`

**Original:** Spawn_NPC sets the chase speed with attack.SetSpeed: random(90,100) for normal (13.1.1.1), random(90,100) for army (13.1.1.9), random(120,140) for runners (13.1.1.3). Velocities sampled live (o1.log, m_behs.js): 92.0 / 94.3 / 129.7 px/s. The 80-90 / 90-100 / 100-110 bands are the warn speeds (b_warn in warn_zed, 13.3.1.1), not the chase.

**Remake:** data/config.lua zombie.chase_speed_min/max is 80/90 and data/zombies.lua adds speed_offset 0/10/20, giving 80-90 / 90-100 / 100-110. Measured ~85 / 90 / 107 px/s in caps/z_r2_chase_measure.log. The Progress.md reference table repeats these numbers.

### 2. Runner bite is 16 on every difficulty

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/zombies.lua`, `game/data/generated/original_globals.lua`

**Original:** The global Zed_fast_dmg is 16 and no difficulty block changes it; 2.11.9-2.11.12 set only Zed_dmg and Zed_army_dmg. Read live: Regular [9,13,16], Novice [5,8,16]. In o1.log the runner's first bite took the player from 100 to 84.

**Remake:** 6: data/zombies.lua fast.damage_scale = 6/9 of 9. data/generated/original_globals.lua holds Zed_fast_dmg = 6, which is the 1.2 fan mod's value.

### 3. Bite damage depends on difficulty

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`, `game/data/config.lua`, `game/data/zombies.lua`

**Original:** 2.11.12 Novice: Zed_dmg 5, Zed_army_dmg 8. 2.11.11 Regular: 9 / 13. 2.11.10 Veteran: 10 / 15. 2.11.9 Legend: 10 / 15. The runner is 16 on all of them. Confirmed live on Novice and Regular.

**Remake:** 9 for normal and 13 for army on every difficulty: ai.lua uses ai.zombie_tuned("damage") * damage_scale, and the difficulty keys in data/config.lua cover only hunger, thirst, regen and infection.

### 4. The other zombie kinds are missing

- **Verdict:** make identical  **Effort:** XL  **User's ask:** none
- **Files:** `game/data/zombies.lua`, `game/src/game/zombie.lua`, `game/src/game/systems/ai.lua`, `game/src/game/systems/population.lua`, `game/data/sounds.lua`

**Original:** Ordinary spawn points and z_spawners also produce these kinds:
- buried (kind 2): zed_buried_hidden lies in the ground and jumps out on any noise with the deerrun sound (13.1.1.2, 13.3.3, 13.6.1.11).
- screamer (6): 50 hp, LOS 400 px / 180 deg. It plays attack_8 every 2.5 s and calls noice(player, 1700) every 0.5 s (13.3.7).
- shooter/spitter (4, 10, 11, 18): 100-175 hp, fires zed_bullet (13.3.6, 13.2.9).
- tank (12): 800 hp, hits for Zed_dmg x2, sounds zed_tank_1-3 and zed_tank_scream (13.3.8, 13.2.6).
- jumper (16): 150 hp, sounds jumper_spot and jumper_jump (13.3.9).
- skin7 (7): 80 hp, leaves a bio splash on death.
- skin8 (8): 120 hp, runner speed; a bio explosion and splashes on death, and a trail every 0.5 s (13.3.5.6, 13.3.1.2.5.4).
- skin9 (5): 200 hp, always drops item 268.
- skins 10/11 'fresh' (17, 19): 110-120 px/s, dodge sideways at 300 px/s (Zed_dodge 13.3.2.6), and their bites always bleed (15.3.5.3).

**Remake:** Only normal, army and fast exist (data/zombies.lua). Its comment keeps skins 7-11 'for LATER'. None of the special kinds or their art behaviour are in src/.

### 5. Sight cone is 180 degrees, not 60

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/systems/ai.lua`, `Progress.md`

**Original:** Every eyes object's MainLook has range 335 and cone 180 (data.js instance properties [1,335,180,1]; read live as Vn = 3.142 rad). ShortLook has range 70 and cone 360.

**Remake:** data/config.lua sets ai.cone_degrees = 60 (30 each side). The ranges, 335 and 70, are the same.

### 6. Sight is checked once a second

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`

**Original:** Aggro is only gained or dropped inside System.Every(1) (13.3.1.3.1-5), so a zombie reacts 0-1 s after you come into view. In o1.log the army aggroed by 1.3 s and the normal by 2.5 s; in o2.log the delay was 0.8-1.6 s.

**Remake:** ai.update calls can_see every frame, so the reaction is instant.

### 7. At night a zombie sees you only within 175 px or when you are in light

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`, `game/src/game/systems/daynight.lua`, `game/src/game/campfire.lua`

**Original:** With Day_mode = 0 (hours 0-5, 17.1.3), aggro needs distance <= 175 px or the target overlapping fam_car_light (13.3.1.3.x.2.1.1 and .3.1.1.1). That family is car_light, light_sprite (a fireplace's light), light_tower and muzzle_flash; every shot fired at night spawns a muzzle_flash (8.6.2.2.4.1). Live (o2.log, o2_04_night250.png, o2_05_night150.png): at 01:00 a zombie facing the player at 250 px stayed idle; at 150 px it chased.

**Remake:** ai.lua has no day/night rule. In caps/q4_night.log at 01:00, the army zombie and the runner chase from 250 px (cmp/night_orig_vs_remake.png). The remake has campfires that glow at night, which could serve as the light.

### 8. What blocks a zombie's sight

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/physics/world.lua`, `game/data/colliders.lua`, `game/src/physics/categories.lua`

**Original:** Only fam_b_bar_int (room walls), big_obstacle_base and grenade smoke are sight obstacles (13.1.1.1 AddObstacle).
- big_obstacle_base: tree_block (forests), vans, trucks, the BTR, fence_horizontal/vertical, hesco, the heli and hammer crashes, shut doors, and some building parts.
- obst_base blocks walking but not sight: single trees (tree_pine, tree_leaves*), regular cars, obst_fence, benches, trash containers, pillars, sandbags.

**Remake:** physics.sight_blocked (src/physics/world.lua, sight_ray) stops at any WALL or WALL_INSIDE fixture. That includes every tree, car and fence collider (data/colliders.lua), so single trees and cars hide you from zombies.

### 9. Losing sight stops the zombie where it is

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`, `game/data/config.lua`

**Original:** At the next 1 s check with no LOS: var#0 = 0, the attack movement and the Turret are disabled, and the zombie stands idle where it is (13.3.1.3.x.1). There is no memory of where it last saw you. Live (o2.log, o2_03_lost.png): after the player was teleported 390 px away, the zombie ran ~0.6 s more, then idled at 598 px.

**Remake:** The zombie keeps running to the last seen point for lose_sight_time (3 s), then 'investigating' for 2 s, then wanders (data/config.lua ai). In caps/q5_lost.log it runs to the old spot (-396,0), stands 'chasing:idle', then 'investigating', then 'idle:walk' (cmp/lost_orig_vs_remake.png).

### 10. A zombie that spots you alerts the zombies around you

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`

**Original:** On Turret.OnTargetAcquired the zombie plays spotted_N and calls noice(target.X, target.Y, 400) (13.3.2.4.1, 13.3.4.4.1, 13.3.5.4.1, 13.3.3.4.1). Every non-aggro zombie within 400 px of you is then warned toward you (13.6.1.4-7). In o1.log all three placed zombies carried the warned flag (var#3) in the first second.

**Remake:** Only the sound plays (ai.lua emit "zombie.spotted"). No other zombie is alerted.

### 11. How a zombie responds to a noise

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`, `game/data/config.lua`

**Original:** warn_zed mode 0 (13.3.1.1) acts only on a zombie that is not aggro, not already warned and not biting. It heads for the noise point +/-50 px (random) at the warn speed (80-90 normal, 90-100 army, 100-110 runner) for 2 s, then stops wherever it is. While warned it plays the RUN animation (13.3.2.2: run when warned and speed > 51; army and runner always run, 13.3.4.2 and 13.3.5.2).

**Remake:** ai.noise sets 'investigating': the zombie walks to the exact point at investigate_speed_fraction 0.6 x chase (48-66 px/s), forgets it after 2 s, and a new noise retargets it. The animation is 'walk' (ai.lua plays 'run' only while chasing).

### 12. Noise sources: footsteps 65 px, melee silent

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/config.lua`, `game/src/game/player.lua`

**Original:** While the player moves, noice(player, 65) fires every 0.5 s (7.3.1). Melee makes no noise at all: no noice call in 8.7. Gunfire is per weapon, 100-600 px (8.6.2.x and 8.6.3.x; combat survey).

**Remake:** Melee makes noise: combat.lua calls ai.noise(..., melee_noise_radius = 150), an invented value. Footsteps make none.

### 13. A hit makes an unaware zombie lurch for a second

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/ai.lua`

**Original:** Every round or melee 'bullet' that hits calls warn_zed(uid, 0, 0, 1, kind) (13.2.8.4). If the zombie is not yet aggro, it turns to a random angle and moves at warn speed for 1 s (13.3.1.1.x.1.2).

**Remake:** The only AI reaction to a hit is ai.hit, the asked-for slow; the zombie does not lurch.

### 14. Slow on hit (the original has none)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Zombies should randomly make noises when wandering, hitting a zombie should slow it down for a second." (Stage 22)
- **Files:** `game/src/game/systems/ai.lua`, `game/data/config.lua`

**Original:** No slow: the z_walker 'Slowdown' handler exists (13.3.1.9), but nothing ever starts that timer.

**Remake:** Half speed, walk cycle included, for 1 s; a new hit restarts it (data/config.lua zombie.hit_slow 0.5 / hit_slow_time 1.0, ai.hit).

### 15. Idle wander: 3 s at 20 px/s in a random direction every 5-9 s, half the time

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`, `game/src/game/zombie.lua`, `game/data/config.lua`

**Original:** The z_walker 'Walk' timer repeats every ceil(random(4,9)) s (StartTimer in 13.1.1.1). On each tick, with 50% chance, the zombie picks a random angle and moves at 20 px/s for 3 s (~60 px) with the walk animation, then stands (13.3.2.5, 13.3.4.5, 13.3.5.5). The walk is not tied to the spawn point. Live (o6.log): bouts of ~59 px in 3 game-seconds, starting 7.6 s and then 20 s apart.

**Remake:** The zombie walks to random points within 90 px (35-100%) of its spawn 'home' at 0.34 x chase speed (27-37 px/s), pauses 0.8-2.6 s and gives up after 4 s (data/config.lua ai.wander_*). It is moving most of the time (caps/q1_idle.log 'idle:walk').

### 16. Walls: a 10x10 box at the body's centre that bounces

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (the Stage 23 ask only wants walls solid, which both are)
- **Files:** `game/src/game/systems/ai.lua`, `game/src/game/zombie.lua`, `game/data/config.lua`

**Original:** The moving object is the 10x10 'eyes' at the centre of the 30x30 body sprite; the body is pinned to it. Its Bullet movements bounce off fam_b_bar_int, tilemap_forest, tilemap_water, obst_base and big_obstacle_base (13.1.1.1; Bullet 'bounce off solids' = 1). The Turret turns it back toward its target at 600 deg/s, so a zombie blocked by an obstacle jitters against it. In o1.log the normal zombie stayed stuck at (-151,-259) for 10 s, flipping between run_up and run_down.

**Remake:** A radius-7 circle at the feet that slides along walls (data/config.lua zombie.radius 7; ai.lua 'wall sliding is the collider's job'; Progress 3.4 says the original slides). The feet anchor is the remake's projection convention, so the collision point is ~12 px lower than the original's.

### 17. Zombies collide with neither each other nor the player

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/physics/categories.lua`

**Original:** Zombie bodies have no Solid behaviour, and the eyes bounce only off scenery. Zombies therefore pile onto one spot and onto the player, and the player walks through them. In o5.log a normal at (18,0), an army at (19,0) and a runner at (18,0) occupied the same spot (zoom_o5_02_contact.png).

**Remake:** The ZOMBIE collision mask includes PLAYER and ZOMBIE (src/physics/categories.lua). Zombies form a ring 21 px out and box the player in; one runner was left idle outside the ring (caps/q2_contact.png/.log, cmp/contact_orig_vs_remake.png).

### 18. Three zombies in four lead a moving player

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`

**Original:** With chance 3 in 4, Spawn_NPC turns on Turret predictive aim with a projectile speed of random(0,100) (13.1.1.1.1, 13.1.1.3.1, 13.1.1.9.1). Those zombies aim ahead of a moving player and cut corners.

**Remake:** Every zombie steers straight at the player's current position (ai.lua).

### 19. Bite timing: one global tick for all biters; a 1 s pause after you break contact

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/ai.lua`

**Original:** The first bite lands on first contact: OnCollision in 13.3.1.4-7, with the sound playing 50% of the time. After that, one global System.Every(1) (13.2.5.1) bites with every zombie that overlaps the player and that the player's interactor LOS (70 px, through solids) can see; all biters land on the same tick, each with a sound. The zombie stops moving on contact and only moves again at the next 1 s check after contact ends (13.3.1.3.x.2.1).

**Remake:** Each zombie has its own 1.0 s cooldown from its own first swing (ai.lua attack_cooldown), so bites are staggered. A zombie resumes chasing the frame the player leaves its range.

### 20. Zombie blow sound

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "the zombie attacking sound should just sound like a hit sound. The current sound is for wolfs." (Stage 28)
- **Files:** `game/data/sounds.lua`, `game/src/game/systems/ai.lua`

**Original:** Each bite plays one of attack_0, attack_1, attack_10, attack_11, attack_12, attack_13 at the zombie at 0 dB (13.2.5.1.1.1, 13.3.1.4.1; also the tank and the jumper, 13.2.6, 13.2.7). Player_get_hit also plays body_1 or body_2 at -5 dB on the player (15.3.5). attack_8 is the screamer's scream; attack_9 is never used.

**Remake:** Stage 26 plays zed_tank_1, zed_tank_2, zed_tank_3 or wolf_attack (data/sounds.lua zombie.attack).

### 21. Spotted sounds: spotted_0 to spotted_6 only

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/sounds.lua`

**Original:** spotted_0 to spotted_6 play at 0 dB when the player is within Player_Hear_radius (2000) (13.3.2.4, 13.3.4.4, 13.3.5.4). jumper_spot belongs to the jumper only (13.3.9).

**Remake:** data/sounds.lua zombie.spotted adds jumper_spot to the seven. Volume is zombie_volume 0.85.

### 22. No death cry

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/sounds.lua`

**Original:** A normal, army or runner death plays nothing (13.3.1.2). You hear only the round's body_1 or body_2 (13.2.8.4.3). zed_tank_scream belongs to the tank (13.3.1.1.7).

**Remake:** combat.kill plays emit_at("zombie.death") = zed_tank_scream, a 2.4 s scream, on every kill (data/sounds.lua zombie.death).

### 23. A hit on a zombie is quieter than a hit on the player

- **Verdict:** make identical  **Effort:** S  **User's ask:** "whenever a hit connects to a zombie, add blood effects and a hit sound effect. same with when a player takes damage" (Stage 20) asks for the sound to exist, not for its volume
- **Files:** `game/data/config.lua`, `game/src/game/systems/combat.lua`, `game/data/sounds.lua`

**Original:** A hit on a zombie plays body_1 or body_2 at the zombie at -15 dB, only within Player_Hear_radius (13.2.8.4.3). A hit on the player plays it at -5 dB (15.3.5).

**Remake:** Both play at impact_volume 0.7 (data/config.lua audio, data/sounds.lua impact.hit).

### 24. Wander groans (the original has none)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** "Zombies should randomly make noises when wandering" (Stage 22)
- **Files:** `game/src/game/systems/ai.lua`, `game/data/sounds.lua`

**Original:** Idle zombies make no sound.

**Remake:** An idle zombie within 300 px groans every 6-14 s, never two within 2.5 s (ai.lua wander_groan). It uses attack_0, 1, 8, 9, 10-13, which is the original's blow set plus the screamer's attack_8. If the blow sound becomes attack_N again, groans and blows will sound alike.

### 25. How far zombie sounds carry

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/core/audio.lua`, `game/data/config.lua`

**Original:** The Audio plugin uses an inverse distance model: reference 600, rolloff 10, listener Z 600, max 10000 (data.js t187 [0,0,0,1,1,600,600,10000,10]). That gives ~46% volume at 300 px, ~19% at 600, ~10% at 1000 and ~4% at 2000.

**Remake:** Full volume inside 48 px and silent past 640 px (data/config.lua audio min_distance, max_distance, falloff 1.4).

### 26. Health bar over every zombie

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/render.lua`, `game/src/game/zombie.lua`

**Original:** npc_hp_bar_bg (30x6) sits centred 25 px above the body; npc_hp_bar (30x6) is left-anchored at x-15 with width 30 / max_hp * hp. Both are pinned and drawn on layer Rain, above trees and roofs (13.1.1.x.N.1, 13.2.1.4.1.1). The bar is always shown, green. Live (o2.log): 25 damage on a 60 hp zombie gave a 17.5 px bar. Visible in zoom_o5_00_ring.png and zoom_A1_idle.png.

**Remake:** No health bar. The art is in the atlas (npc_hp_bar/default_0, npc_hp_bar_bg/default_0), but nothing in src/ draws it (cmp/idle_orig_vs_remake.png).

### 27. Damage numbers on melee hits

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/effects.lua`, `game/src/game/systems/combat.lua`

**Original:** dmg_hint (12.29, called for melee 'bullets' in 13.2.8.4.1.1) spawns the text '-N' in rgb(255,196,68) on layer Rain. The text moves away from the player. A crit is 18 px, red, with a prefix. Seen in zoom_o1_03_contact.png and zoom_o5_02_contact.png.

**Remake:** No damage numbers. combat.record_hit only feeds the F8 debug overlay.

### 28. Blood burst details

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 20 asked for blood on hits ("add blood effects"); the frames, rotation and fade are not covered
- **Files:** `game/src/game/systems/effects.lua`, `game/data/config.lua`

**Original:** test_bodyhit plays a1 (6 frames) or a2 (5 frames) at random, rotated by choose(0, 90, 180, 245). It is spawned at the round's position on layer static_cars and fades: wait 0.3 s, then out over 0.2 s (13.2.8.4; Fade [1,0,0.3,0.2,1]).

**Remake:** effects.hit always plays a1, unrotated, at the body's chest height, once through over 0.5 s (data/config.lua effects.hit_anim "a1").

### 29. Draw order: zombies under the player, corpses at the bottom

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/render.lua`

**Original:** Drawing goes by layer, not by y:
- corpses: items_on_ground [18] with MoveToBottom, under dropped items too;
- zombies: Zeds [29], moved to Buildings_shadows [9] when behind a building (13.3.1.8);
- player [32], blood [40], facades [41], trees [43], hp bars [44].
A zombie standing 10 px south of the player is drawn under him (orig/zoom_E1.png).

**Remake:** Zombies and corpses are y-sorted with everything else (render.lua by_ground_y). A zombie or corpse south of the player is drawn over him.

### 30. Corpses stay 60 s and fade over 5 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/zombie.lua`

**Original:** Every *_dead sprite has Fade [active 1, in 0, wait 60, out 5, destroy 1]. Bot_corpse_stay_time (90) is never read for zombies. Live (o5.log, game clock): opacity 1.0 until ~58 s, 0.04 at 64.9 s, gone by 71.5 s (o5_c9.png, o5_c10.png).

**Remake:** Corpses stay 90 s and fade over 2.5 s (data/config.lua corpse.life 90, fade 2.5, citing Bot_corpse_stay_time).

### 31. Half the corpses are mirrored

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/zombie.lua`, `game/src/game/systems/render.lua`

**Original:** Every corpse spawn runs 'if choose(1,2) = 1 then SetMirrored' (13.3.1.2.2.x.1 and the others). o5.log shows corpse widths of -30 and 30.

**Remake:** Corpses are never mirrored (zombie.corpse_spawn), so they all lie the same way (cmp/corpses_orig_vs_remake.png).

### 32. Zombies drop loot when they die near the player

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/game/systems/combat.lua`, `game/data/loot_tables.lua`

**Original:** Drops happen only if the player is within 500 px:
- normal, 2 in 7: Spawn_drop(choose(22,116,35,88,39,110,20,102,36,33,18,39,119,26,20,13,58,16,17));
- army, 1 in 7: choose(116,11,12,14,15,69,21,102,43,36,58,100);
- runner, 2 in 7: choose(97,116,87,18,39,110,26,119,22,113,53,110,39,35,18,102,13,58,16,17).
The item lands at the corpse's y+15 (13.3.1.2.2.13, .4.5, .5.6). In o5_04_corpses.png one of the eight dropped a 'Molotov'.

**Remake:** combat.kill spawns only the corpse; no loot.

### 33. A landed bite shakes the camera and flashes the screen grey

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 20 asked for blood and a hit sound on player damage; the shake and flash are not covered
- **Files:** `game/src/game/systems/combat.lua`, `game/src/game/systems/effects.lua`

**Original:** Player_get_hit (15.3.5) runs ScrollTo.Shake(3, 0.4) and Hit_effect (15.4), which sets the layout's grayscale 'Hit_effect' to 100 for 0.2 s, on every bite that is not blocked.

**Remake:** A bite shows only the blood burst and the body sound; nothing in src/ shakes the camera, and there is no grey flash.

### 34. Bleeding from a bite lasts 60 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `Progress.md`

**Original:** A landed bite starts bleeding 1 time in 4, with timer 'Bleed' = perk_metabolism = 2 ? 30 : 60 (15.3.5.2). Without the perk that is 60 s.

**Remake:** data/config.lua survival.bleed_duration = 30. Progress' reference table read the perk branch.

### 35. Zombies come in groups of about 5 per spawn point

- **Verdict:** make identical  **Effort:** L  **User's ask:** none
- **Files:** `game/data/places.lua`, `game/src/map/fill.lua`, `game/src/game/systems/population.lua`, `game/data/config.lua`, `game/data/zombies.lua`

**Original:** There is one zed_resp_point per 1000x1000 location, at hotspot+500 (3.16.1.N), with an animation frame set by the location type. Zed_check fills it from a table (13.7.1.21):
- frame 0 (location 24 = village houses, also 7 and 9): 4 normal at +/-350 px, then 1 from choose(1,2,3,4,6,2,5,7,16,17) at +/-250; one time in 6 it is 4 buried + 1 screamer instead;
- frame 1: 5 normal-or-runner at +/-400 + 1 screamer, or 3 of skin7/skin8/spitter;
- frame 2: 5 normal + 2 mixed;
- frame 3 (type 14): 2 army at +/-400 + 1 shooter;
- frame 4 (5, 22): 5 army + 1 heavy;
- frame 8 (26, 40, 41): 2 army + 3 shooters;
- frame 16 (70): 5 army + 10 shooters;
- frames 5-7: wolves, deer, rabbits.
Runners appear only as the 1-in-10 extra or at frame 1.

**Remake:** One zombie per point (population.per_point 1, scatter 40). Points come per 1200 px lot: city 5, base 5, town 3 (data/places.lua spawns). Each point's kind is rolled from the place's weights: city normal 80 / fast 20, base army 70 / normal 30, town 90 / 10, gas station and cafe normal. caps/z_r6_world.log: 109 zombies (84 normal, 20 army, 5 fast), strewn singly.

### 36. Zombies are only ever created 1500-2000 px from the player and removed past 2000

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/map/loader.lua`, `game/src/game/systems/population.lua`, `game/data/config.lua`

**Original:** A point is filled only while the player is more than 1500 and at most 2000 px from it (13.7.1.21), at layout start (2.11) and at each Zed_check (once a second after the camera has moved 400 px, 3.5.1). Zombies further than respawn_radius_npc (2000) are destroyed and their point's count freed (13.7.1.9-14). So nothing is ever created within 1500 px of you, a fresh run starts with no zombies near you (o1/o2: m_clear found 0 NPCs within 3000 px), and leaving and coming back re-rolls a point.

**Remake:** src/map/loader.lua (lines 97-110) creates every point's zombie at load; only the 600 px around the spawn is kept clear (data/places.lua spawn.clear). Points refill in their band. Distant zombies are kept and frozen past 2000 px (ai.sleep_range) rather than removed.

### 37. Respawn band size: 1500/2000 vs 760/1020

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `game/data/config.lua`, `game/src/game/systems/population.lua`

**Original:** The respawn band is 1500 to 2000 px from the player (13.7.1.21; global respawn_radius_npc = 2000).

**Remake:** 760 to 1020 px (data/config.lua population.spawn_inner/spawn_outer). The remake scaled the band down for its 2.25x camera zoom (population.lua header).

### 38. Far zombies: blinded (original) vs frozen (remake)

- **Verdict:** unclear  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/systems/population.lua`, `game/src/game/systems/ai.lua`

**Original:** npc_blinder (20.7) sets sight and Turret range to 0, every 3 s, for zombies more than 1500 px away; they still wander. Ranges are restored when they come back within 1500.

**Remake:** Zombies past 2000 px sleep: stopped, with no clocks running (ai.set_asleep, population.sleep).

## Already identical

- Art: every zed_* sheet, 30x30 frames, and every animation's frame count, speed and loop flag (generated/anim_speeds.lua matches objects.txt). Idle is 5 fps once; run is 8 fps loop (skin8: 10); walk is 5 fps loop. The attack is 4 frames at 8 fps for normal and army skin2, 3 frames at 2 fps for army skins 1/3 and the runners. Deaths die1/die2 play once at 8 fps. The four facings split at 45 degrees (13.3.2.1). Sprites match at the remake's zoom (cmp/idle_orig_vs_remake.png).
- Skin rolls: normal skins 1-6 uniform (13.1.1.1 choose(1..6)), army 1-3, runner 1-3. Army corpses are zed_army_dead_skinN. A death sheet with one pose uses that pose (army skin2 die1, normal skin3 die2).
- Health per kind: normal 60, army 100, runner 80 (13.1.1.x var#0; data/generated/zombie_stats.lua ratios).
- Sight ranges: 335 px in front (MainLook) and 70 px all round (ShortLook).
- Contact/bite distance: original 18-22 px centre to centre (o5.log, o1.log); remake 21-22 (attack_range 22, caps/q2_contact.log).
- Regular bite damage: normal 9, army 13. The first bite lands on contact, then about one a second. Armour subtracts, with a floor of 2 (15.3.2.1).
- Room walls are solid to zombies and block their sight (also asked in Stage 23). Shut doors block zombies and sight; in the original a shut door spawns a big_obstacle_base. Zombies cannot open doors.
- No zombie is made inside a wall: the original destroys one whose eyes overlap walls, water or forest (13.1.1.1.9.1); the remake tries four clear spots (population.clear).
- Random facing at spawn. Each zombie rolls its own speed inside its band.
- Respawn sweep cadence: once a second, and only after the camera has moved 400 px (3.5.1 vs population.sweep_due). Occupancy is counted per spawn point.
- No population scaling by difficulty: Zed_per_base is set per level and difficulty (3.8.x) but never read.
- A hit on a zombie plays body_1/body_2 and shows test_bodyhit in both (asked in Stage 20; volume and details differ, see C4 and D3).
- Infection from bites: the original has none; the remake's infection, on VETERAN only, was asked for in Stage 23 and Stage 26 and stays.

## Not checked

- The report file: writing S/survey/zombies/report.md was refused by the harness ('Subagents should return findings as text'). This structured output is the full report.
- Listening: headless runs have no audio. Sounds were compared by file name, duration (attack_0/1 1.0 s, attack_10-13 0.5-0.6 s, zed_tank_1-3 1.4-1.6 s, zed_tank_scream 2.4 s) and the dB values in the events.
- The special kinds (buried, screamer, shooter/spitter, tank, jumper, skins 7-11) were read from the events but not run live.
- In the original, the draw order of zombies against building facades and trees: only player-vs-zombie was seen live (zoom_E1.png); the rest comes from the layer list.
- Which mapgen location types are towns, cities or bases, apart from type 24 (village houses 1-10). The point frame for each type is listed in E1.
- Final-day hordes and tent-sleep attack waves (8.2.1, 17.5.1.1.1, 17.8), and bunker z_spawners (Polygon layout, 3.3): the remake has no such systems.
- Zombies vs wolves and deer: zombies target wolves and hurt overlapping wolves and deer for 15 a second (2.11, 13.2.5.1.2-3). There are no animals at Stage 26; that belongs to the Stage 28 fauna work.
- Barbed wire (zombies slowed to 1/4 for 2 s) and barricades broken by zombies (20.6); grenade smoke as a sight blocker; landmines. The remake keeps these items inert.
- The original player's automatic melee when a zombie touches him (8.7.1 Melee_2_auto) belongs to the combat survey. It is why zombies in the original captures die in contact and why damage numbers appear there.
- XP and Stats_Zeds_killed counting rules, and the death screen's 'infected killed'.
- Gunfire noise radius per weapon (100-600 px, 8.6.x), for the combat survey.
- Zombies inside the remake's Stage 26 cutaway rooms vs the original's interiors were not captured.
- Phone: no zombie behaviour differs by platform in the original, so there is no platform verdict here.
