### 29.11 Being hit, and hitting: the grey over the screen and the HUD, the shake on the view, armour's rule, the shield, the blood at the round, the skull by kind, a drop a second, a wolf's five sounds

*"compare against official minidayz, use screenshots and source code to compare. Do not stop
working until it is identical except for the changes i had asked for specifically"* (the user,
2026-10-03). Backlog track 9, HITFX: hud#30 with character#13, zombies#33 and combat#36; zombies#28
with combat#24; combat#26, #37, #39, #40; survival#32 and audio#11. A landed hit on the player was
a burst and a thud and nothing else; armour held every armoured hit at 2 or more; a turned bite
showed nothing; every blow's blood was one a1 burst over the body's chest, restarted; the skull was
the rotten one, rising, mirrored, with eight invented chunks; a wound dripped twice a second at the
feet; a wolf's bite was one snarl. The asks that meet it stay: the low-health grey (20.5.4), which
the flash goes over at 100%; blood and a hit sound on every hit (20.5.3), so a bite on you and a
wolf's on an infected still bleed; drops that stay on the ground, the newest 120 (20.5.5); the
swing as it is (29.5) -- its wedge, its reach, its swoosh going forward like a wave. Checked
against the official 1.0: its events read (Player_get_hit 15.3 with Hit_effect 15.4, the rounds'
13.2.8.1-4, NPC_get_damage 13.2.1, the wolf's punch 13.2.4, the bleed 6.2.5, the swing's crescent
8.7.1.5.1.2), its data.js (the placed effects' Fade and Bullet behaviours, their frames' origins
and image points, the Map layout's layers and layout effects), its c2runtime.js (ScrollTo's
shake, the `grayscale` shader), and the running game at 1280x720 in headless Edge, shot and
measured beside the remake.

- [x] 29.11.1 **A landed hit greys the whole screen, the HUD with it, for 0.2 s.** Player_get_hit
      calls Hit_effect (15.3.5, 15.4): the layout effect "Hit_effect" -- the Map layout carries two
      Grayscale effects, `Grayscale` and `Hit_effect` -- to 100, `Wait(0.2)`, back to 0. A layout
      effect is laid over every layer once drawn, its GUI's among them; its shader is the remake's
      own grey to the letter, Rec. 601 luma mixed by the intensity. `effects.flash` starts
      `state.hit_flash` on every hit that lands, god mode's too, ticked on the world's clock;
      `render.flash` reads it. While it is on `render.begin_grey` sends 1 whatever the health's grey
      had it at, the shader stays on through `render.end_grey` over the cone, the outside dark,
      the ghost and the head's stack, and `render.begin_flash`/`end_flash` set it again round
      `ui.draw` -- on draws already being made, never a pass over the finished screen. The build
      ghost's own shader greys itself by the same luma and gives back the shader it found. A hit
      landing while it is on does not lengthen it: the first's Wait ends it on time. Twelve frames
      at 60, the hit's and eleven more. Shot in the running original with Player_get_hit called
      and the time scale held at 0: the world and every HUD plate grey; the remake's capture the
      same, its debug corner alone in colour.
- [x] 29.11.2 **And the view shakes, 3 px falling to nothing over 0.4 s, the ears held.**
      `ScrollTo.Shake(3, 0.4, reducing)` (15.3.5), and a wolf's punch shakes before it calls
      Player_get_hit (13.2.4.1.1), so a turned wolf's bite still shakes. Read off c2runtime.js's
      ScrollTo: the behaviour keeps one shake, a new one starts it again, and every tick while it
      runs the scroll is put off its point by a random angle and a random distance under
      `3 * min(timescale, 1) * (1 - elapsed / 0.4)`, on game time; the camera object, which is
      the listener, stays where it is pinned. `camera.shake` and `camera.update` do that, from a
      stream of the camera's own, never math.random, into `cam.shake_x`/`shake_y`, which only
      `viewport.view` reads -- the draw, the culling, a touch's world point -- so `cam.x`, the
      ears and the pin never move. Measured live in the original from the teleported player's
      middle: 0 to 2.5 px over the first 60 ms, under 1.7 by 174 ms, 0.1 at 306 ms, every reading
      under the falling line.
- [x] 29.11.3 **Armour's rule is the original's.** `Dealt_damage` is the damage less the four
      armour vars, set to 2 only if that is under 1 (15.3.2): the army helmet's 5 against a 6
      deals 1, against a 5 or a 3 deals 2. `combat.armoured`; it was `max(2, amount - armour)`
      whenever armour was worn and the hit was over 2, and a graze through armour dealt 1. As
      there, it holds with nothing worn: a hit under 1 deals 2 (no caller sends one).
- [x] 29.11.4 **A bite your weapon turns sends the green shield up.** Player_get_hit's else
      (15.3.6.2) spawns `shield_icon` on the collision base's origin on the Rain layer and sets its
      Bullet's angle of motion to 270; its placed instance's behaviours are Fade `[1, 0, 1, 1, 1]`
      and Bullet `[25, 0, 0, 0, 0, 1]`. `effects.shield`: 10x10 on your middle, up at 25 px/s,
      whole for a second and faded over the next, 50 px in all; no grey, no shake, no blood, no
      sound -- `impact.block` stays an empty list. The sheet frame was never packed:
      `shield_icon` joins `LATE_SHEETS` in `tools/build_atlas.py`, the last cell of the sm page,
      so nothing moved. Shot in the original with Math.random held at 0 for one Player_get_hit:
      the shield on the chest at 0.2 s, 30 px up and at 0.81 at 1.1 s; the remake's on its chest.
- [x] 29.11.5 **A blow's blood where it met the body, one a blow, turned one of four ways, gone
      in half a second.** Every round or swing crescent that meets a body spawns `test_bodyhit` at
      itself on `static_cars` (13.2.8.N): `SetAnim(choose("a1", "a2"))`,
      `SetAngle(choose(90, 180, 245, 0))`, Fade `[1, 0, 0.3, 0.2, 1]`, a1's 20x20 with its origin
      in the middle and a2's 10x10 with its origin at (1, 9), both looping at 10 fps. `effects.hit`
      makes one a call, never restarted -- a shotgun's five pellets are five -- from a stream of
      the state's own, turned about the frame's own origin, at its ground point and 14 px over it:
      a round's where its ray met the body, the height a round flies from the shooter's middle.
      **The swing's is on the swinger.** `melee_swing_general` is in `fam_m4_bullet`, spawned on
      the swinger's middle, turned to the reticle, with a Fade and no Bullet (8.7.1.5.1.2): it
      never moves, so the body it meets bleeds at it. Captured in the original: a punch's burst
      at 661 with the player at 660, on his chest; the remake's swing passes its anchor
      (`resolve_wedge`) and draws it there, and the F8 marker with it. A bite on you bleeds on
      your middle, the user's; a wolf's bite on an infected bleeds as before, the user's "every
      hit". `hit_anim`, `hit_height`'s comment and the one-a-body rule went.
- [x] 29.11.6 **The skull is its kind's, on the body's middle, still.** Each kind's handler
      spawns `headshoticon` at its origin on `Sunset_Dawn` and sets its frame
      (13.2.8.N.1.2.5.1.4): none for the infected, so 0, the human skull; 1 for a wolf; 3 for a
      deer. Fade `[1, 0, 0.4, 0.1, 1]`; no movement, no mirror. `headshot_particles` has no
      instance in the export and nothing creates one, so the eight invented chunks went with the
      rise and the mirror. Each frame stands on its own origin, 20 to 30 px under its bottom edge
      ((16, 46), (15, 47), (16, 56), (19, 58)), and the body's origin is `targeting.centre`'s, so
      the skull sits over the head on the health bar, as shot side by side with one stood over an
      infected in the original. Drawn over the night (`effects.draw_over_night`): Sunset_Dawn is
      the original's layer 48, over its night's 46. **A rabbit has no head to find:** its
      handler (13.2.8.2) rolls no headshot, so `combat.headshot` asks `effects.skull` first --
      no double damage, no slow, no skull on a rabbit.
- [x] 29.11.7 **A wound drips once a second, 17 px under the body's middle, turned.** 6.2.5:
      `Every(1)` while var#22, `Spawn(blood_drop, "items_on_ground", 2)` -- image point 2 of the
      30x30 collision base is `head`, (15, 17), two under its origin -- `choose(1, 2, 0)`,
      `SetAngle(random(360))`, `SetY(Y + 15)`. Measured in the running original: the player at
      y 2989, his drops at 3006, the first at once and one a second. **The brief had 15 px; it is
      17**, 3 px under the feet. `drop_every` 0.5 -> 1, `drop_below` 17, turned about each frame's
      own origin, no scatter, no mirror; they stay, the newest 120 (the user's; the original's
      fade after 50 s). The remake's, captured: at 643 under feet at 640.
- [x] 29.11.8 **A wolf's bite is the original's five sounds.** Its punch (13.2.4.1): the shake,
      Player_get_hit -- its thud on you at -5 dB -- then `wolf_attack` at 0 dB on the wolf and a
      thud on you, and on every punch within your hearing (13.2.4.1.11) another `wolf_attack` and
      a thud on the wolf. Five starts in one frame, as the survey's five within 2 ms (audio o5);
      four if your weapon turns it. On an infected the last pair alone, and no -15 dB thud on its
      body: NPC_get_damage plays none, so `apply_damage` with what dealt it logs the hit quietly
      (`record_hit`'s `quiet`). The thuds are -5 dB, `hit_player_volume`. 29.2.18's item, done.
- [x] 29.11.9 **The clothes' tear was built in 30.2, and is the original's.** Read against
      15.3.5.10: `choose(2..7)` on every hit that lands, 3 to 7 the player's var#8, #10, #18, #12
      and #20, `round(random(7, 13))` off the worn one -- `combat.wear_clothes`, from its own
      stream, after the block and the armour. Nothing changed; its scenario is 30.2's.
- [x] 29.11.10 **Scenarios, written and NOT run.**
      New:
      - 'combat: a bite that lands greys the whole screen, the HUD with it, for 0.2 s, and shakes the view 3 px falling to nothing over 0.4 s, the ears held' (combat)
      - 'combat: armour 5 against a 6 leaves 1 and against a 5 leaves 2, the original's rule' (combat)
      - 'combat: a bite the weapon turns sends the green shield up off your middle at 25 px/s for two seconds, and nothing else' (combat)
      - 'combat: a headshot's skull is its kind's on the body's middle, still and unmirrored, and a rabbit has no head to find' (combat)
      - 'animals: a wolf's bite on you starts the original's five sounds in its frame, and on an infected its own two' (combat)
      - 'survival: a wound drops blood once a second, 3 px under the feet, a random frame turned a random way' (hud)

      Changed:
      - 'combat: armour takes points off, and never adds any' is 'combat: armour takes points off, and a hit it stops whole deals 2, the original's': the graze through 23 reads 2, not 1
      - 'combat: a landed hit bursts blood over the body and sounds it, a zombie's and the player's' is 'combat: a landed hit spills the original's blood where it met the body, at the swing's crescent or on you, turned one of four ways, gone in half a second, and sounds it': the burst at the round 7 px short of a pinned zombie's middle, drawn 14 up from its origin, a swing's on the swinger, none half a second on, four hundred taking all eight turns and anims, the player's on his middle, the block's none
      - 'combat: a headshot slows the world for 0.2 s of its own time, and a pause holds it': F6's "fx 3" (two rounds' bursts and the skull, where one burst restarted made 2), and the westward skull unmirrored, frame 0 ("nil,0,0")
      - 'animals: a wolf finds you inside its sight, howls, runs at you and bites through the one door, snarling': two snarls a bite (`#snd == 1 + 2n`)
      - 'survival: a bleeding wound drops blood that stays on the ground, the newest 120' (hud): one or two drops in its 60 frames, not three or four

      `tools/scenarios/__init__.py`'s rows for effects.lua and camera.lua say what they hold now.
      The new and changed expectations were reasoned from the code and the captures; the wolf's
      five sounds alone were read off its script driven once straight through lovec.
- [ ] 29.11.11 **The user's calls, each one change to reverse.**
      - A swing's blood on the swinger, where the original's still crescent spills it. Passing
        `z.x, z.y` instead of `m.x, m.y` to `apply_damage` in `resolve_wedge` puts it on the body.
      - A wolf's bite on an infected still bleeds (20.5.3's "every hit"), where the original
        spills none. `if not by then effects.hit(...) end` in `apply_damage` takes it off.
      - The flash leaves the debug overlays in colour. Dropping `render.end_flash()` before
        `dbg.draw_world` and bracketing `dbg.draw_screen` greys them too.
      - A hit inside a flash does not lengthen it, as there; a later hit's own Wait cutting a third
        flash short is not kept. A list of pending ends on the state would keep it.
      - The shield packed late (`LATE_SHEETS`) rather than cut into `Assets/Sprites`, so no cell
        moved. `cut.py shield_icon` into `Assets/Sprites/shield_icon/` and out of `LATE_SHEETS`
        moves every later sm cell.
      - The skull over the night, the bursts and the shield under it, by the original's layers.
        Calling `effects.draw_over_night` before `daynight.draw_world_overlay` puts it back under.
      - No headshot on a rabbit, as there. Dropping `effects.skull`'s check from
        `combat.headshot` gives one back, skull-less.
      - Armour's rule with nothing worn too: a hit under 1 deals 2. `armour > 0 and` in
        `combat.armoured` keeps it to the armoured.
      - A wolf's two thuds at `hit_player_volume`, the same -5 dB, rather than a slider of their
        own. A `wolf_thud_volume` in `cfg.audio` and `audio.lua`'s BOUNDS would part them.
- [ ] 29.11.12 **Found, and left.**
      - The original's blood is on `static_cars` (40), under its facades (41) and trees (43);
        drawn over the world, the remake's is over them. And its reticle, `gui_target`, is on
        Sunset_Dawn over the night, where the remake's is under it (AIM's, or a draw-order
        track's with zombies#29).
      - a1's sixth frame lives in `test_bullet_hit-sheet0.png`, which the sheet catalogue files
        under `test_bullet_hit`; it is not packed, and the burst is gone before it would show.
      - A pause in the middle of a shake holds the view up to 3 px off its point until play
        resumes; whether the original's Options zeroes its time scale (and so the shake) is not
        read.
      - A swing in the original hits what its 10x21 crescent overlaps, on the swinger; the
        remake's wedge hits every body in it (the user's word, 29.5).
      - The original's bleed is 1 in 4 for 60 s, and every hit by `zed_fresh` (15.3.5.2-3):
        combat#38, zombies#34, another track's.
      - The original plays an infected's `attack_*` voice with every bite (13.2.5.1.1.1); the
        user's 28.1 keeps it silent.

*Verified 2026-10-09:* `python tools/verify.py --subject lint` 15/15, three times, the last after
every commit; `python tools/build.py --verify`: the generated files match. Generated:
`atlas.lua` one line (`shield_icon/default_0`, sm cell 6125) and `animations.lua` its entry;
`game/built` rebuilt. Captured and looked at, side by side with the running 1.0 at 1280x720: the
whole screen and HUD grey under a hit's flash; the shield on the player's chest; the skull over an
infected on its bar; a punch's blood on the puncher; a round's bursts on the body's near side at
mid-height; a wound's drops under the feet. Measured live in the original: the shake's offsets
for 306 ms, the drops at +17, the shield at 25 px/s and 0.81 at 1.1 s. Read in the remake through
the console: the drops at +3 under the feet, the swing's burst at the anchor 14 up. **Not run:**
every scenario through verify.py -- the six new and the five changed -- and the web build: the
flash's shader over the HUD and the ghost's new uniform are not seen in a browser's WebGL 1.

Waiting for the final check: the six new scenarios and the five changed; and since app.lua,
data/config.lua, viewport.lua, combat.lua and the generated atlas send a change to every subject,
all of them. What could move: any scenario a bite lands in reading a shader, `grey_sent` or the
HUD's colour within 0.2 s of it, or a world point, `world_bounds` or a click on the world within
0.4 s of it (the view off by up to 3 px); a wolf's bite asks `data/sounds.lua` for two more sounds,
each a `math.random` draw, so later draws move wherever a wolf bites; a 1-point hit through armour
deals 2 (`BITES_30`/`BITES_60` with armour worn, none found); a swing's F8 marker is at the
swinger; a rabbit's headshot is gone; `#state.effects` counts a burst a round. INTEGRATOR: after
merging, `python tools/build.py --only atlas` -- `game/built` is gitignored, and until it is
rebuilt the shield's cell is empty and draws nothing.

**Notes for earlier items (2026-10-09):**

- **6.13** (2026-10-09): "a 1 point graze still does 1 -- rounding it up to 2 because the victim
  is wearing a helmet would be armour making things worse" is not the original's: its
  Dealt_damage sets any hit armour leaves under 1 to 2 (15.3.2), so the graze deals 2, and 5
  against a 6 deals 1 where the floor held it at 2 (29.11.3).
- **16.2.13** (2026-10-09): the skull's frame is no literal lost with a sheet: the infected's
  handler sets none (0, the human skull), a wolf's 1, a deer's 3. It stands still on the body's
  origin, unmirrored, and the original throws no chunks -- nothing creates `headshot_particles`
  (29.11.6).
- **20.5.3** (2026-10-09): the burst is the original's: one a blow at the round or the swing's
  crescent -- on the swinger -- a1 or a2, turned 0, 90, 180 or 245, faded over its last 0.2 s
  (29.11.5). The blood and the sound on every hit stay.
- **20.5.5** (2026-10-09): a drop a second, not two, 17 px under the body's middle and turned a
  random way, without scatter or mirror (29.11.7). They still stay, the newest 120.
- **28.2** (2026-10-09): a wolf's bite snarls twice and thuds three times on you, and on an
  infected snarls and thuds at the wolf with no -15 dB thud on the body (29.11.8).
- **29.2.18** (2026-10-09): the wolf's bite left for HITFX is done (29.11.8).
