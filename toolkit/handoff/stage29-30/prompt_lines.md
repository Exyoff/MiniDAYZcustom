You are working on MiniOutbreak, a LÖVE 11.5 remake (Lua 5.1) of the browser game Mini DayZ. Its web build (love.js, WebGL1, PUC Lua 5.1) is published from `main` to exyoff.github.io/MiniOutbreak; the user plays it on a phone held sideways, and on PC.

The user's standing instruction: *"compare against official minidayz, use screenshots and source code to compare. Do not stop working until it is identical except for the changes i had asked for specifically."* Every deliberate difference the user asked for is in `A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/comparison/user_asks.md`; those stay. Where the remake differs from the original and no ask covers it, the original wins.

## Your worktree
`A:/Documents/VS Code Projects/wt/lines` on branch `work/lines`, branched from the integration head (95e649a). Work only there. Never touch `A:/Documents/VS Code Projects/MiniOutbreak` or any other worktree, never push, never switch branches. `OriginalData/` in it is a junction to the main checkout's (gitignored, never committed): `C2SourceData.js` is the official 1.0 `data.js` with its BOM stripped, `images/` the official sheets.

## The original
- `A:/Documents/VS Code Projects/MiniDAYZcustom/MiniDayZ+1.0` is the official build and the only reference (`data.js`, `images/`, `media/`, `l_eng_*.xml`). 1.2 and Reloaded are fan mods: never the reference.
- The toolkit, `A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/` (README.md first; run its Python with `PYTHONUTF8=1`): `events/` (the event sheets, readable, with event paths; `events/OUTLINE.txt`), `show.py` (`show.py 13.2.5`, `-f Function`, `-g REGEX`), `objects.txt`, `globals.txt`, `cut.py` (cuts an object's frames in the rip's layout).
- Run it: `bash "A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/serve.sh"`, then `node "A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/orig.cjs" --out DIR [--res 1280x720] [--touch] --steps "wait:22000;tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;wait:12000;shot:run;..."` (headless Edge; the menu answers `tap:` only, with `--touch`; New game, Novice, Start). `js:` evaluates in the page, `window.ORIG` has helpers; never assign a bare global in a `js:` step. Look at its screen beside the remake's.
- The surveys that found what your track fixes are in `toolkit/comparison/` (`<area>.md`, finding numbers as `area#N`), and the plan is `toolkit/comparison/backlog.md`.

## The remake
Read first: `docs/conventions.md`, `docs/architecture.md`, `docs/original_data.md`, Progress.md's "Ground rules" and "Where this stands", and the Progress items your brief names. Progress.md's Stages 29 and 30 (the backlog's tracks and the user's asks, built so far) are not committed yet: read them in `A:/Documents/VS Code Projects/MiniOutbreak/Progress.md` (read only; never edit it), not in your worktree's copy. Hard rules: Lua 5.1 only; the hot-reload conventions (require whole modules, read cfg at call time, no GPU objects at module scope, `src/core` never requires game or ui, `src/game/systems` never requires `src/ui`); a generator draws only from its own seeded stream, and nothing it makes follows a table's `pairs()` order (a seed must build one world natively and in the web build); nothing a frame walks may grow with the world; no new per-frame garbage in hot paths; only `combat.lua`'s one door may hurt the player (a lint).

Running it: `"$LOCALAPPDATA/Programs/love-11.5/lovec.exe" game --shot --frames N --beat B --res 1280x720 [--world 7] [--touch off] --script "step;step"` from your worktree's root, with `export APPDATA="$(cygpath -w "C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/agents/lines")"` -- your own save dir; others run the game at the same time. It writes `$APPDATA/LOVE/minioutbreak/run.log` and `shot_N.png` (the last frame); look at shots with the Read tool. The header of `game/src/core/headless.lua` lists the script steps; `tools/scenarios/common.py` the helpers (START, GOD, KILL, BUILD_ALL, lua(...)). A console `lua` line stays under 160 characters; read a value in the same step as what it measures (a frame between can change it); to count VM instructions, `jit.off() jit.flush()` first.

Scenarios: `tools/scenarios/<subject>.py` (a new file is a row in `tools/scenarios/__init__.py`'s CHANGES). THE USER'S RULES FOR THIS PHASE: *"can we skip checks until everything is implemented?"* (2026-10-07) and *"you are free to open windows. Just make sure to perform tests at the end of every agent's work, that way 1 test verifies them all"* (2026-10-08). So: write the scenarios your change deserves (a scenario asserts state, not strings) but RUN NO SCENARIOS -- not your own, no regression sets, never the full suite or `--changed`: one test pass after every track is in verifies them all, and your report lists what it should run. The lint subject is the exception: run `python tools/verify.py --subject lint` (static Python, seconds) before you finish. You MAY run the game and the original to see what you build: captures of the remake and the running original, looked at with the Read tool, side by side -- that is building, not testing, and for anything drawn it is how you match the original.

Art: cut what the rip lacks from `OriginalData/images` with `cut.py` into `Assets/Sprites/<object>/` in the rip's layout, then `python tools/build.py --only atlas` (and `--only icons`). GENERATED FILES: they are made from the official 1.0's data since 29.1, and `python tools/build.py --verify` passes. If your change moves what an extractor reads or adds art, rebuild and commit what changes, and nothing else.

## Commits
Commit early and often on `work/lines`; every commit message ends with exactly:

Co-Authored-By: Claude Opus 5.5 (1M context) <noreply@anthropic.com>

No model names anywhere else, in commits, code or docs. Update README.md and docs where they describe what you change. Do not edit Progress.md: return your progress entry. Match the surrounding code's comment density and Progress.md's voice. A choice that is the user's to make: make a sensible one and list it, with the one change that reverses it.

## Your track: LINES (backlog track 11)
The planner's brief follows. Its findings' evidence is in the surveys it cites; check each against the original's events and data yourself before you change anything, and where the brief is wrong, follow the original and say so. The user's Stage 30 asks (Progress.md's Stage 30, `toolkit/comparison/user_asks.md`) stand over the brief where they meet it.

#### 11. LINES — Lines over the head

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

Where the brief says to run a subject or `--changed`, the user's rules above win: write the scenarios without running them, and list them for the final test pass. Measuring and capturing the running original, and capturing the remake to look at what you build, are fine.

Merged under you: 29.1-29.10 (DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, 29.5 the melee kept as it is, ZOMBIE-AI, CAMERA at 1:1, HUD-TOP, TYPE with Arimo as the original's Arial and `widgets.outline` as its Outline effect, HUD-TOUCH, which already wires the switch's "I have no ranged weapon." through today's `combat.say`: keep the call, restyle the line) and 30.1-30.7 (the user's asks).

Progress Stage 29's question 3 (the original's "what happened" lines -- "I've eaten Canned beans.", "I found something in this trunk.") is unanswered; its default, written first, is **over the head only**: make the style and the survival and gun lines; the report lines after uses and searches stay out until the user answers, and nothing shows mid-screen while the board is open. While the pad or the perks are open, follow the original (the remake has the pad, 27.4).

Another track, 29.11 HITFX, is building at the same time over `combat.lua`'s hit on the player, `effects.lua` and the camera's shake: in `combat.lua` keep to `combat.say` and the gun lines' calls, so the two merge cleanly.

Number your Progress entry 29.12.

## What to return
Build it, commit each piece as soon as it works (a usage limit can stop you at any time), and end with a report: a summary; the commits; the files changed; generated files changed; the scenarios added and changed (written, not run: say so); captures you took and looked at, the remake's and the original's; what of the original you read and what it says; choices (each with the one change that reverses it); what you found and left; every check that waits for the final check; and a Progress.md entry for the track in Progress's voice -- "### 29.12 Title", a short paragraph quoting the user's standing instruction where it applies, "- [x] 29.12.K **Lead.** prose" items with the original's evidence, "- [ ]" for what is left, "*Verified ...*" lines saying honestly what was and was not run -- plus dated notes for earlier Progress items this overturns ("- **N.N.N** (2026-10-08): ..."), or none. Save the Progress entry to `C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad/progress_lines.md` as well.
