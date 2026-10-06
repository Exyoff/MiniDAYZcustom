# menus: the remake against the official 1.0

*Area surveyed:* Menus and screens: boot/loading, main menu, New game difficulty screen, Options, Achievements, Unlocks, in-run pause, death screen, transitions, texts (l_eng_ui.xml / l_eng_new.xml), at 1280x720 and 844x390 --touch

Paths written `SP/...` were the cloud session's scratchpad (screenshots, side-by-sides); they did not come
along. Retake any of them with `../orig.cjs` and the remake's capture harness.

## Summary

I compared the original and the remake from the event sheets (Menu_Events ME, Game_events GE, Loading_events LE) and from screenshots of both games in the same states. SP below means /tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad.

Where the evidence is:
- Original screenshots: SP/survey/menus/orig/{a,b,c,d2,d3,e,f}/*.png. The e/ and f/ folders are 844x390; the rest are 1280x720. Each folder's log.txt holds the layout and font dumps.
- Remake captures: SP/env/s1/caps/m_*_1280.png and p_*_844.png.
- Side-by-side pairs, original on the left: SP/survey/menus/side/*.png.
- The harness refused to let this subagent write SP/survey/menus/report.md, so the full report is these fields.

The finding that explains most of the rest: the original runs Construct 2 in crop mode (data.js project[12]=1). One layout unit is one CSS pixel at every screen size, so the menu is the same at 1280x720 and on a phone: 160x39 px wood planks (menu_btn_wood), a white 16pt Times New Roman label, a 36 px pitch (32 px on screens shorter than 590 px).

The original menu itself is a static red-sky header strip with the MINIDAYZ+ logo, over a live world with a roaming camera. The full-screen red sky with the skyline scrolling left at 12/24 px/s is the original's loader, death and transition screen. Only the in-run GUI layers scale on a short screen (gui_scale = 1-(590-H)*0.00185, which is 0.63 at 390 px).

The remake differs on almost every screen:
- Every screen is a parchment panel, scaled by min(W/960, H/540), with dark all-caps text in LÖVE's default font.
- The difficulty screen offers three cards that start the run on one tap; the original has four plates (Legend included) that you select and then confirm with a green Start.
- The pause is dimmed and titled; the original's has neither.
- The death screen leaves the HUD visible and offers a RESTART that builds a new world, with the save kept. The original hides the HUD, offers "Go to menu" only, and wipes the save.
- Loading is one "GENERATING WORLD" plate; the original slides a sky curtain down and steps through six status lines.
- There is no "DAY N" label, no menu ambience, and no fades between screens.

What the user's asks cover:
- The full-screen scrolling red sky and skyline on the start menu.
- The three menu buttons, Continue, Start, Settings, with the user's words kept.
- The difficulty on a screen of its own.
- The contents of the settings and pause window: touch on/off, a volume slider, main menu, resume, dead zones.
- Infection only on Veteran.
Everything else in this area has no ask behind it.

Open for the user (verdict "unclear"):
- The logo vs the MINIOUTBREAK name.
- The Achievements, Unlocks, Facebook and "MINI DayZ 2" buttons. The user's menu ask names exactly three buttons, and Progress.md lists Achievements as out of scope.
- The Stick position editor, which depends on the controls survey's verdict on the floating stick.
- The "bites can infect" line on the Veteran card.
- The remake-only line under the title when CONTINUE is refused.

## Findings

Verdicts: make identical 31, unclear 5, platform 3, keep (user asked) 2

### 1. Boot screen: WELCOME TO, then the red-sky loader with logo and loading bar, sliding up into the menu

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `tools/web_template/index.html`, `game/src/app.lua`, `game/data/ui/menu.lua`

**Original:** Black page with the red 'WELCOME TO' C2 loader logo (MiniDayZ+1.0/loading-logo.png) while data downloads (SP/survey/menus/orig/a/boot_02.png). Then the Loading layout (a/boot_05, e/boot_06):
- a red sky over the top 3/4 of the screen (height 0.75*H, LE 3) with the skyline at its foot and black below;
- the MINIDAYZ+ logo centred;
- sky and city scrolling LEFT at 12 and 24 px/s (LE 3, 5, 6);
- a loading bar: grey tiledbg_loader track W-50 px wide and 10 px tall at H-30, filled with white ninepatch_loader.
On load the sky, skyline and logo slide UP at H/2 px/s into the menu's header strip (ME 1.1-1.3; a/boot_11).

**Remake:** The page shell's #loading div: 'LOADING n/m' in 14 px uppercase monospace (#8b9178) over a 240x2 px bar on #14160f (tools/web_template/index.html lines ~100-156, 680-701). Then the menu appears with no transition.

### 2. Menu fades in from black over 2 s

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`, `game/data/ui/menu.lua`

**Original:** At the menu's layout start a black fade_sprite over the world fades out over 2 s (ME 3.1.1; Fade props [0,0,0,2,1]). On boot the header also slides up (ME 1.1-1.3).

**Remake:** The menu is there on the first frame (app.show_menu, game/src/app.lua 464-478); there is no fade.

### 3. Loading a run: the sky curtain and six status lines vs a single GENERATING WORLD plate

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 25 asked for generated worlds on START, not for this screen's look)
- **Files:** `game/data/ui/generating.lua`, `game/src/app.lua`

**Original:** On Start or Continue (c/load_00..load_13, d3/cont_1, cont_4):
- the plates fade out and the sky curtain and logo slide DOWN over the menu while it fades to black over 1 s (ME 49.1 loader_pic_setup);
- after 2 s, the Map layout shows the full red sky and logo with a white 12pt Arial status line (Text[t521]) at 80% height, stepping 0.3 s apart through ui:16 'Generating world', ui:18 'Placing roads (step 1)', ui:19 'Placing roads (step 2)', ui:20 'Adding secret places', ui:21 'Tilemap drawings', ui:22 'Creating minimap' (GE 3.6.3 / 3.7.3);
- then the curtain slides up and the world fades in from black over 2 s.

**Remake:** An uppercase 'GENERATING WORLD' label in dark ink on a gui_card_cover parchment plate over a 61% amenu_bg dim (game/data/ui/generating.lua). It shows only for the frame(s) a world is being generated (app.defer / app.run_pending, src/app.lua 416-444), then the run appears at once. Never shown for a world already built. See SP/env/s1/caps/m_gen_1280.png and SP/survey/menus/side/loading_1280.png.

### 4. 'DAY N' typewriter label at the start of a run or day is missing

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/hud.lua`, `game/src/app.lua`

**Original:** 2 s after the curtain lifts, 'DAY 1' types in one letter every 0.3 s (bold 24pt Trebuchet MS, white), waits 1 s and fades out over 2 s (GE 3.6.3.7 -> Day_show_label, GE 12.16). Visible in c/run_17.png and d2/pause.png.

**Remake:** No such label (nothing in game/data/ui/hud.lua or src/ for it).

### 5. Start menu backdrop is a full-screen red sky with skyline (original menu: sky header strip over a live world)

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 22: "Also work on the start menu; Continue, Start, Settings. Slow red background, a little faster scrolling background of sky scrapper."
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`

**Original:** The original menu proper (a/menu.png, e/menu3.png):
- a static red-sky header ~95 px tall at 720p (~65 px at 390p) with the logo;
- below it, the Menu layout world (zombies, trucks, trees) under a camera roaming between random waypoints at up to 24 px/s (ME 3.1.1, ME 46);
- a black tiledbgmenu_city_down strip along the bottom.
The full-screen red sky with the scrolling skyline is the original's loader, death and transition screen.

**Remake:** The sky stretched over the whole screen with the skyline at the bottom edge, both scrolling (game/data/ui/menu.lua menu_bg, menu_city). The difficulty screen uses the same backdrop.

### 6. Sky and skyline scroll RIGHT; the original's scroll LEFT

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the ask says slow and a little faster, not a direction)
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`, `game/src/ui/widgets.lua`

**Original:** Bullet angle of motion 180, i.e. leftward (LE 3). LE 5 wraps the city by +711 px once it has travelled 711. Measured: the skyline moved left between a/boot_05 and a/boot_08.

**Remake:** scroll_x = 8 and 16, which drift right (game/data/ui/menu.lua, game/data/ui/difficulty.lua; Progress 22.3.4 says 'Both drift right').

### 7. Scroll speeds in screen pixels

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`

**Original:** Sky 12 px/s and city 24 px/s in CSS px at every screen size: crop mode, with Bullet speeds 12 and 24 on the Loading-layout instances t665 and t661.

**Remake:** 8 and 16 UI units/s. Progress 22.3.4 converted the speeds as if the original scaled a 1024x768 window. That gives 10.7/21.3 px/s at 1280x720 and 5.8/11.6 px/s at 844x390 (design 960x540, viewport.ui_scale).

### 8. Sky proportion and skyline position on the red screens

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the ask covers the red background and its scroll, not this composition)
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`

**Original:** On the loader, death and transition screens the sky is stretched to 0.75*H from the top (LE 3, GE 10.3, GE 29.2). The skyline is spawned at the sky's foot (~72% of H) with black ground below it (a/boot_05, d2/dead_3, f/dead).

**Remake:** The sky is stretched to the full height (ph=1) and the skyline is pushed to the bottom edge with a +137 offset. There is no black ground band (game/data/ui/menu.lua lines ~58-79; SP/survey/menus/side/menu_1280.png).

### 9. Parchment panel behind the menu buttons

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`

**Original:** No panel: the plates stand directly on the backdrop (a/menu, e/menu3). ninepatch_charactersbg is used only as the Character-select background (b/unl_character.png).

**Remake:** A 440x280-unit pinned-parchment panel (menu_panel, sprite tilesets/ninepatch_charactersbg) behind the buttons. The difficulty screen has a 440x330 one (diff_panel).

### 10. Logo sprite vs the 'MINIOUTBREAK' title text

- **Verdict:** unclear  **Effort:** S  **User's ask:** none, but the remake carries its own name, and the logo art is the DayZ mark
- **Files:** `game/data/ui/menu.lua`, `tools/build_web.py`

**Original:** The main_menu_logo sprite (the MINIDAYZ+ art, 213.5x54) at top centre over the header: layout y 42 at 720p, so its top 9 px are cut off; fully visible at 844x390. The original manifest name is 'Mini DAYZ'.

**Remake:** A dark-ink 'MINIOUTBREAK' text label inside the panel (menu.lua menu_title); the page title is 'MiniOutbreak' (tools/build_web.py line 260).

### 11. Button plate art, label colour and font

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`, `game/data/ui/settings.lua`, `game/data/ui/death.lua`, `game/src/ui/widgets.lua`, `tools/build_atlas.py`

**Original:** menu_btn_wood anim Default: a wood plank with a grey drop shadow, labelled in white 16pt Times New Roman (instance font of Text[t415]; ME 3.2.3.x). The selected state is Default_selected (green outline); Start is Green; Facebook is Blue; 'MINI DayZ 2' is Red_anim.

**Remake:** A gui_card_cover/default_5 torn parchment, 9-sliced, with a dark-brown {0.16,0.13,0.09} label in LÖVE's default sans (menu.lua LABEL; src/ui/widgets.lua font_at -> love.graphics.newFont(size)). The menu_btn_wood sheet is only in Assets/images, not in Assets/Sprites (SP/orig/missing.txt line 204).

### 12. Button size, pitch and column position

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/menu.lua`, `tools/scenarios/common.py`

**Original:** Every plate is 160x39 px at every screen size, with a 36 px pitch (32 px when H<590, ME 2.2.1). The column is anchored at Menu_hotspot = (X_mid, Y_mid-36) (ME 3.1.1). At 720p: Continue 324, New game 360, Achievements 396, Unlocks 432, Options 468. At 844x390: 159, 191, 223, 255, 287.

**Remake:** 260x40-unit plates with a 68-unit pitch: 347x53 px and 91 px at 1280x720; 188x29 px and 49 px at 844x390. Positions: CONTINUE -66, START 2, SETTINGS 70 units from centre (menu.lua BTN_W/H, Y_*). Evidence: SP/survey/menus/side/menu_1280.png and menu_844.png.

### 13. CONTINUE is always drawn (dark without a save); the original hides it

- **Verdict:** make identical  **Effort:** S  **User's ask:** none (the ask names Continue, not a dark placeholder)
- **Files:** `game/data/ui/menu.lua`, `game/src/app.lua`

**Original:** The Continue plate (ui:2) is created only when LocalStorage gamesav_v7 = '1', one slot ABOVE New game (ME 3.1.4.1.1). Compare d3/gm_9.png (with a save) and a/menu.png (without).

**Remake:** Always drawn: a darkened, inert copy without a save (menu_continue_off), so the column never moves (menu.lua; m_menu_1280 vs m_mainmenu_1280).

### 14. Label case: ALL CAPS vs the original's title case

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 22: "Continue, Start, Settings" covers the words of the three menu buttons (which stay), not their case.
- **Files:** `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`, `game/data/ui/settings.lua`, `game/data/ui/death.lua`, `game/data/ui/generating.lua`

**Original:** 'Continue' (ui:2), 'New game' (ui:1), 'Options' (ui:534), 'Back' (ui:10), 'Resume' (ui:11), 'Go to menu' (ui:37), 'Novice/Regular/Veteran' (ui:7-9), 'Legend' (new:178), 'Start' (new:621), 'You are dead' (ui:542).

**Remake:** CONTINUE, START, SETTINGS, DIFFICULTY, NOVICE, REGULAR, VETERAN, BACK, PAUSED, RESUME, MAIN MENU, YOU DIED, RESTART, GENERATING WORLD (menu.lua, difficulty.lua, settings.lua, death.lua, generating.lua).

### 15. Achievements, Unlocks, Facebook and 'MINI DayZ 2' are missing from the main menu

- **Verdict:** unclear  **Effort:** XL  **User's ask:** Stage 22: "Also work on the start menu; Continue, Start, Settings." Whether these three buttons are meant to be the whole menu is not said. Achievements is listed under Progress.md 'Explicitly out of scope', which is not a user ask.
- **Files:** `game/data/ui/menu.lua`, `game/src/app.lua`

**Original:** The main menu also has:
- Achievements (ui:3): a full-screen bg_rust panel with overall stats (ui:28-32) and 31 achievements with bronze/silver/gold ranks, a drag-scrolled list and an X close button (ME 3.2.2.15; b/achievements.png, e/achievements.png).
- Unlocks (ui:390): a submenu Character / Weapons / Items / Back (ME 3.2.9). Character select offers 20 survivors with their starting gear; weapon unlocks cover MAC-10, VSS, SV-98, Deagle, Saiga-12, AUG and Katana (b/unl_character, unl_weapons, unl_items).
- An orange 'new unlock' ping (char_update_ping).
- A blue 'Facebook' plate at the left edge (ME 3.2.3.7 -> facebook.com/MINIDAYZGAME).
- A flashing red 'MINI DayZ 2' plate at the right edge (ME 3.2.3.5 -> minidayz.com).

**Remake:** None of these exist.

### 16. Menu ambience: the original loops 'ambient' on the menu

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`, `game/src/game/systems/ambience.lua`

**Original:** Audio.Play(sound:'ambient', looping, -10 dB) at the menu's layout start (ME 3.1.1).

**Remake:** The menu is silent: app.update's menu branch returns before ambience.update (game/src/app.lua 728-758). The ambient bed only plays in a run.

### 17. Screen-to-screen transitions: fades and a 0.3-0.5 s switch vs an instant swap

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`, `game/src/ui/ui.lua`

**Original:** On a tap:
- switching=1, then a 0.1 s wait;
- menu_clear fades the old plates and labels out over 0.2 s (family Fade 'destroy' [0,0,0,0.2,1]);
- a 0.2 s wait, then the new plates fade in over 0.1 s ('create' [1,0.1,0,0,1]).
Taps are ignored while switching (ME 3.2.2.x, 3.2.13).

**Remake:** Screens swap on the same frame (app.choose_difficulty / leave_difficulty / open_settings / close_settings, game/src/app.lua 310-340, 566-592).

### 18. Difficulty screen layout and art

- **Verdict:** make identical  **Effort:** M  **User's ask:** Stage 26: "Clicking play should open a new screen and in there the difficulty should be" covers having the screen, which the original also has (ME 3.2.2.4 -> 3.2.6). It does not cover its look.
- **Files:** `game/data/ui/difficulty.lua`, `tools/scenarios/common.py`

**Original:** In a/difficulty.png and e/difficulty.png:
- four wood plates in a column at X_mid-100, starting at Menu_hotspot_Y: Novice 324, Regular 360, Veteran 396, Legend 432 at 720p; 159, 191, 223, 255 at 390p;
- a Back plate centred at hotspot+4*pitch (468 at 720p, 287 at 390p);
- a description to the right at (X_mid+10, hotspot-15), 281 px wide, in white 10pt Tahoma with an Outline effect (Text[t426]; ME 3.2.6);
- no title and no panel.

**Remake:** A parchment panel with a 'DIFFICULTY' title, three 380x56-unit parchment cards 14 apart (507x75 px at 720p, 274x40 at 390p) each holding a name and a numbers line, and a 160x40 BACK (game/data/ui/difficulty.lua; m_diff_1280, p_diff_844; side/difficulty_1280.png, difficulty_844.png).

### 19. Legend difficulty is missing

- **Verdict:** make identical  **Effort:** L  **User's ask:** none (Stage 26's "limit the infected status to only hard mode" says which mode infects, not that Legend goes)
- **Files:** `game/data/ui/difficulty.lua`, `game/src/core/settings.lua`, `game/data/config.lua`, `game/src/ui/minimap.lua`

**Original:** Four difficulties: Novice, Regular, Veteran and Legend (new:178; ME 3.2.6.4, 3.2.2.10). Legend plays with Veteran's numbers (GE 2.11.9) and hides unexplored areas on the minimap (new:620, the Legendary var). The death screen prints 'Legend' (GE 10.3.17.4).

**Remake:** NOVICE, REGULAR and VETERAN only (difficulty.lua DIFFS; src/core/settings.lua; data/config.lua *_infect).

### 20. Select, then Start (two taps) vs one tap starts the run

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (26.2.1 records this as the remake's own choice)
- **Files:** `game/data/ui/difficulty.lua`, `game/src/app.lua`, `game/data/config.lua`, `tools/scenarios/common.py`

**Original:** A tap selects:
- Difficulty is set and the plate turns Default_selected (green outline);
- the description becomes that difficulty's bullets;
- a green 'Start' plate (new:621) appears at (X_mid+100, hotspot+3*pitch), beside Legend (ME 3.2.2.7-3.2.2.10, 3.2.5).
Start (ME 3.2.2.11) wipes gamesav_v7 and loads the run (a/diff_novice.png, c/novice_selected.png, e/diff_novice.png).

**Remake:** A tap on a card starts the run at once (start_<name> -> app.new_run; Progress 26.2.1 'I chose to have a tap start the run'). A 0.35 s settle guard stops a double tap on START from starting REGULAR (26.2.2, data/config.lua ui.difficulty_settle); select-then-Start would make that guard unnecessary.

### 21. Difficulty descriptions: the original's bullets vs the remake's numbers (and those numbers are the 1.2 fan mod's)

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/difficulty.lua`

**Original:** Before a pick: 'Choose difficulty level.\nSome unlocks can be done only on higher difficulties.' (new:155). After a pick:
- Novice (new:617): '• Slow hunger and thirst rate • Warm weather • Lowered NPCs damage to player'.
- Regular (new:618): '• Medium hunger and thirst rate • Normal weather • Regular NPCs damage to player'.
- Veteran (new:619): '• Normal hunger and thirst rate • Cold weather • High NPCs damage to player • Little more zombies in some locations'.
- Legend (new:620): Veteran's lines plus '• Not explored areas is hidden on minimap'.

**Remake:** '0.75x hunger and thirst, 2.0 regen', '1.1x hunger and thirst, 1.75 regen', '1.4x hunger and thirst, 1.5 regen, bites can infect' (difficulty.lua DIFFS). These are the MiniDayZ+1.2 values. 1.0 has Novice 0.25/0.25/regen 8, Regular 1/1/6, Veteran 1/1/4 (SP/orig/globals.txt lines 847-877).

### 22. 'bites can infect' on the VETERAN card

- **Verdict:** unclear  **Effort:** S  **User's ask:** Stage 26: "limit the infected status to only hard mode" covers the rule, not whether the card should say it.
- **Files:** `game/data/ui/difficulty.lua`

**Original:** The original has no infection and no such line (new:619).

**Remake:** VETERAN's line ends ', bites can infect' (difficulty.lua).

### 23. The last difficulty chosen is remembered and highlighted

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/difficulty.lua`, `game/src/core/settings.lua`

**Original:** Nothing is highlighted when the screen opens; Difficulty is not stored across sessions (no LocalStorage key; a/difficulty.png).

**Remake:** The choice is saved to settings.lua and the last one wears the gui_perk_icon_select frame (diff_*_mark, visible_when settings.diff_<name>; m_diff_1280 shows REGULAR framed).

### 24. Options/Settings contents: sound toggle, stick, language vs touch toggle, volume and dead zones

- **Verdict:** keep (user asked)  **Effort:** S  **User's ask:** Stage 17: "Settings window that toggles touchscreen on/off, volume slider, return to main menu and resume"; Stage 18: "add dead zone to settings to push buttons closer to middle of screen ... Add vertical and horizontal dead zone."
- **Files:** `game/data/ui/settings.lua`

**Original:** Options from the main menu (ME 3.2.11; a/options.png, e/options.png): 'Sounds: ON/OFF' (ui:35/36), 'Stick position' (ui:69), 'Language' (ui:70), 'Back'. In a run the pause holds 'Go to menu', 'Sounds: ON/OFF', 'Movement: By tap/Stick/Keyboard(WASD)' (ui:33/34, new:661) and 'Resume' (GE 12.9.2).

**Remake:** A TOUCH CONTROLS checkbox, a VOLUME slider, HORIZONTAL and VERTICAL DEAD ZONE sliders, and BACK, or RESUME + MAIN MENU in a run (game/data/ui/settings.lua; m_menuset_1280, m_pause_1280).

### 25. Options over the main menu: plain plate column vs a titled window

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 17 asked for a settings window in a run; nothing was asked about how Options looks on the main menu.
- **Files:** `game/data/ui/settings.lua`, `game/data/ui/menu.lua`

**Original:** The menu's Options is a column of wood plates on the backdrop, with no window and no title (a/options.png).

**Remake:** The rust options_menu window with a 1.5x cream 'SETTINGS' title and a dimmed backdrop (set_panel_menu, set_title_menu, set_dim in game/data/ui/settings.lua; side/options_vs_settings_1280.png).

### 26. Stick position editor is missing

- **Verdict:** unclear  **Effort:** M  **User's ask:** none; it depends on the controls survey's verdict on the floating stick
- **Files:** `game/data/ui/settings.lua`, `game/data/ui/touch_controls.lua`

**Original:** 'Stick position' hides the sky and logo, dims the screen 75%, shows the HUD buttons, and offers a draggable thumbstick ring with 'Save' (ui:98) and 'Reset' (ui:97) plates. The position is stored in LocalStorage stick_pos_X/Y (ME 3.2.14, 3.2.2.32-33; a/stick_position.png).

**Remake:** No editor. The remake's stick floats wherever the thumb lands in the left half (game/data/ui/touch_controls.lua lines 43-49).

### 27. Language option is missing

- **Verdict:** make identical  **Effort:** XL  **User's ask:** none
- **Files:** `game/data/ui/settings.lua`

**Original:** Language opens two plates, 'English' (ui:500) and a second labelled 'Español' (ui:505) that actually loads the Russian files (var 55, ME 3.2.2.22; a/language.png). There is no Back plate on that screen. The build ships 8 language sets of xml files.

**Remake:** English only, with no language option.

### 28. Pause: a dim and a 'PAUSED' title the original does not have

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/settings.lua`

**Original:** No dim: the world and HUD stay fully lit and frozen (timescale 0, GE 12.9.1.1.2), with no title (d2/pause.png, f/pause.png).

**Remake:** A 61% black amenu_bg dim over everything and a 'PAUSED' title in cream at 1.5x (settings.lua set_dim, set_title; m_pause_1280, p_pause_844; side/pause_1280.png).

### 29. Pause panel and button art, size and font

- **Verdict:** make identical  **Effort:** M  **User's ask:** Stage 17 asked for the controls in the window, not for its art.
- **Files:** `game/data/ui/settings.lua`, `game/src/ui/widgets.lua`

**Original:** options_menu at 2x scale: 256x422 px at 720p, times gui_scale 0.63 at 390p (~160x265 px). Its plates are 2x wood planks (320x78) labelled in white 22pt Times New Roman (GE 12.9.2; GE 1.2.1 for gui_scale).

**Remake:** options_menu 9-sliced to 340x420 units (453x560 px at 720p, 245x303 px at 390p). Parchment plates 260x40 with dark labels. The sliders use a gui_wpn_cooldown_bg track and a gui_pickpile_scroll knob; TOUCH CONTROLS sits on a sub_menu plate with main_menu_checkbox (settings.lua).

### 30. Pause button order and labels: 'Go to menu' on top and 'Resume' at the bottom vs RESUME above MAIN MENU

- **Verdict:** make identical  **Effort:** S  **User's ask:** Stage 17's "return to main menu and resume" covers the two functions, not their labels or order.
- **Files:** `game/data/ui/settings.lua`, `tools/scenarios/common.py`

**Original:** 'Go to menu' (ui:37) at image point 1 (top), then Sounds and Movement, a gap, and 'Resume' (ui:11) at point 5 (bottom) (GE 12.9.2; d2/pause.png).

**Remake:** RESUME at y 292 and MAIN MENU at y 344, both at the foot of the window (settings.lua set_resume, set_main_menu).

### 31. Go to menu: sky-curtain transition vs an instant rebuild

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/app.lua`

**Original:** The game saves (SaveState 'mysave', gamesav_v7='1', GE 12.9.3.1.2). On OnSaveComplete the screen darkens and the sky curtain and logo slide down over the world (GE 29.2; d3/gm_1, gm_3). Then comes the menu with its header slide and Continue on top (d3/gm_9).

**Remake:** The game saves, rebuilds and shows the menu on the next frame (app.quit_to_menu, game/src/app.lua 636-658; m_mainmenu_1280).

### 32. Death screen backdrop: the original hides the HUD and world behind the red sky

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/death.lua`, `game/src/app.lua`

**Original:** Dead_screen (GE 10.3) destroys the HUD: fam_amenu_bg, portraits, bars, dpad, cards and more. It resets gui_scale to 1 and fades from black (fade_sprite fadeout 2 s) to the red sky and skyline at 0.75*H with black below. The world is not visible (d2/dead_0, dead_3, f/dead).

**Remake:** A 72% dark dim over the still-visible world and HUD (heart and bars, gear, minimap, weapon), with a bg_rust panel 420x260 (game/data/ui/death.lua; m_death_1280, p_death_844; side/death_1280.png).

### 33. Death screen texts

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/data/ui/death.lua`

**Original:** 'You are dead' (ui:542; white 16pt Times New Roman at Y_mid-100). Below it a two-column table in white 16pt Arial (Text t359/t360): 'Score / Minutes alive / infected killed / Bandits killed / Difficulty' (ui:533, 12-15, set in GE 2.11), with values = experience points, round(time lived/60), zeds killed, bandits killed, difficulty name (GE 10.3.17). The happy end shows new:655 'You successfully escaped.' instead. The cause of death (Death_log_reason) is never shown, only logged.

**Remake:** 'YOU DIED' in red, then the cause ('died of thirst'), 'survived N day(s)' and 'N zombies killed' (death.lua).

### 34. Death screen button: 'Go to menu' after 1 s vs RESTART into a new world

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/data/ui/death.lua`, `game/src/app.lua`

**Original:** A single 'Go to menu' wood plate (ui:37) appears 1 s after death at the btn_restart point (screen y ~501 at 720p, GE 10.3.11). Tapping it fades the texts out, fades the logo in, and 2.5 s later goes to the main menu (GE 10.4, 10.4.2 -> Restart_game GE 26; d2/gomenu_1, gomenu_4, menu_after_death).

**Remake:** 'RESTART' (a 200x40 parchment plate) is there at once and starts a NEW world on the same difficulty (app.restart, game/src/app.lua 341-348).

### 35. Permadeath: the original wipes the save on death; the remake keeps the last living autosave

- **Verdict:** make identical  **Effort:** S  **User's ask:** none
- **Files:** `game/src/game/save.lua`, `game/src/app.lua`

**Original:** On death gamesav_v7 is set to '0' (GE 6.2.3.1.2.1.1 and 6.2.3.1.2.2), so the menu afterwards has no Continue (d2/menu_after_death.png).

**Remake:** save.tick refuses to save while dead (game/src/game/save.lua 1042-1044) and nothing clears the slot. CONTINUE after a death resumes the run from before it. Progress.md 'Needs you' lists 'death and CONTINUE (permadeath or not)' as open.

### 36. Menu size on a phone: the original is 1:1 CSS px; the remake scales to 0.72

- **Verdict:** make identical  **Effort:** M  **User's ask:** none (Stage 18's dead zones move buttons inward; they do not size the menus)
- **Files:** `game/src/core/viewport.lua`, `game/data/ui/menu.lua`, `game/data/ui/difficulty.lua`, `game/data/ui/settings.lua`, `game/data/ui/death.lua`

**Original:** Crop mode (data.js project[12]=1): menus are 1:1 CSS px, with the same 160x39 plates and ~21 px labels at 844x390 as at 1280x720 (e/menu3, e/difficulty, e/options). The in-run pause scales by gui_scale = 1-(590-H)*0.00185 = 0.63 (GE 1.2.1). The death screen is 1:1 (GE 10.3).

**Remake:** Every screen scales by min(W/960, H/540) = 0.722 at 844x390: labels ~9 px tall, menu plates 188x29, difficulty cards 274x40, pause window 245x303 (game/src/core/viewport.lua ui_scale; p_menu_844, p_diff_844, p_pause_844; side/menu_844.png, difficulty_844.png).

### 37. Fonts across menus and screens

- **Verdict:** make identical  **Effort:** M  **User's ask:** none
- **Files:** `game/src/ui/widgets.lua`

**Original:** Times New Roman: plates, 'You are dead', the exit dialog. Arial: the death table, the loader status line, achievements. Tahoma with an Outline effect: the difficulty description. Bold Trebuchet MS: 'DAY N' (layout instance fonts in data.js).

**Remake:** LÖVE's built-in default font for all text (src/ui/widgets.lua font_at -> love.graphics.newFont(size), with no font file). Times New Roman cannot be shipped, but metric-compatible free faces can (Liberation Serif/Tinos, Liberation Sans/Arimo).

### 38. Exit confirmation dialogs on the back key

- **Verdict:** platform  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`

**Original:** Android back key:
- on the main menu: a rust panel (bg_rust 318x224) reading 'Are you sure you want to exit the game? Your game will be saved.' (ui:541, rgb 150,140,131), with Quit and Cancel plates (ui:537/539) (ME 2.1.2, 3.2.4; b/exit_dialog.png);
- in a run: the same question with No / Yes, where Yes saves and leaves (GE 1.3.2.1.2 -> Level_menu(8), GE 12.5.10.11).
In a browser C2's Browser.OnBackButton never fires, so neither dialog appears there.

**Remake:** Escape or the phone's back key quits from the menu on desktop and does nothing on the web; in a run it peels layers and opens the pause (app.back, game/src/app.lua 594-634).

### 39. Escape opens the pause over the death screen

- **Verdict:** platform  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`

**Original:** The original has no keyboard control on the death screen (no Escape handling anywhere; the only keyboard events are WASD).

**Remake:** app.back -> open_settings while dead brings up the pause over the death screen (game/src/app.lua 594-634, 558-578).

### 40. Portrait: rotate prompt vs crop

- **Verdict:** platform  **Effort:** S  **User's ask:** none
- **Files:** `game/src/app.lua`

**Original:** appmanifest.json sets orientation landscape. In a portrait browser tab C2 crop mode just shows the cropped game, with no prompt.

**Remake:** 'ROTATE YOUR DEVICE / MiniOutbreak is played in landscape' with a phone drawing (app.draw_rotate_prompt, game/src/app.lua 1032-1058).

### 41. CONTINUE refusal line under the title (remake only)

- **Verdict:** unclear  **Effort:** S  **User's ask:** It follows from Stage 25's "finish generated worlds and make it work when clicking start" (seeded saves); the line itself was not asked for.
- **Files:** `game/data/ui/menu.lua`, `game/src/app.lua`

**Original:** No equivalent: Continue either exists (gamesav_v7='1') or not.

**Remake:** A red line under the title when the save cannot be read or its seed builds another world, and CONTINUE goes dark (app.refuse, game/src/app.lua 260-279; menu.lua menu_refused).

## Already identical

- Backdrop art: the red sky is tiledbgmenu_sky_sprite frame 0 and the skyline is tilesets/tiledbgmenu_city_up, the original's own images. The skyline repeats at its 711 px width, the same wrap as LE 5, and the sky:city speed ratio is 1:2 in both (12:24 vs 8:16).
- The difficulty choice is a screen of its own reached from the start button, and its Back returns to the main menu (ME 3.2.2.4 -> 3.2.6, ME 2.1.3; remake app.choose_difficulty / leave_difficulty).
- The pause freezes the world and leaves the HUD in place under it (original timescale 0, GE 12.9.1.1.2; remake app.update's menu/paused branch).
- Going to the menu from the pause saves first, and Continue then appears and resumes the run (GE 12.9.3.1.2 + d3/gm_9; remake app.quit_to_menu + CONTINUE, m_mainmenu_1280).
- Starting a new game discards the previous run: the original wipes gamesav_v7 at Start (ME 3.2.2.11); the remake writes over the slot at the first save.
- Neither game reaches a tutorial from the menu. In 1.0, ME 35/36 wait on a CheckItemExists('gamesav_tutorial') that nothing issues, and menu_draw_new ('Survival / Special') is never called. Neither has a login screen: the Login layout is never gone to.
- Menu taps are silent in both: the original asks for 'menu_click', but the file is not among the 489 in media/.
- The in-run pause window uses the original's options_menu panel art.

## Not checked

- The report file SP/survey/menus/report.md was not written: the harness refused report files from this subagent, so the full report is in these fields.
- Sound was not heard: headless Chromium could not decode the original's audio ('Unable to decode audio data'). The menu ambience, Sounds ON/OFF and any transition sounds come from the events only.
- Timings were not measured from screenshots: the loaded 4-core machine slowed the C2 runtime (28 px of skyline in a nominal 3 s instead of 72), so durations and speeds are quoted from the events and data.js behaviour properties.
- The remake's web-build loading page was not screenshotted: there is no dist/ in the read-only checkout. It was read from tools/web_template/index.html.
- The exit dialogs were drawn by calling menu_draw_exit directly, not by an Android back key. The in-run No/Yes exit dialog (Level_menu 8) was not drawn at all.
- Not exercised: drag-scrolling the achievement list, scrolling and picking in character select, the Items unlock page text, switching the language to Russian, and the Stick position drag/Save/Reset.
- The happy-end death variant ('You successfully escaped.', teal sky frame 1, GE 10.3.16) was not reached.
- Neither game was looked at in portrait.
- No real phone: all phone shots are 844x390 at DPR 1 in emulation.
- The remake's debug 'dbg' tab shows in the captures; it is hidden in the web build and was not judged.
