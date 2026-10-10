# Handoff: MiniOutbreak (updated by the desktop session, 2026-10-09)

Read this first, then `toolkit/README.md`, `toolkit/comparison/backlog.md` and the remake's `Progress.md`
("Where this stands", Stages 29 and 30).

**PAUSED 2026-10-09.** The user pivoted to an art change: *"Lets pivot, document what you were working on and
pause it. Lets start on an art change"* (new textures, buildings, items, vehicles, zombies, animals and a
crouch, from `A:/Documents/VS Code Projects/New Assets`). The comparison work below is paused, not dropped:
pick it up from "Next, in order" when the user says so.

## The two repositories

- **Exyoff/minioutbreak** (private): the remake, a LÖVE 11.5 game (Lua 5.1) with a love.js web build.
  `.github/workflows/publish-web.yml` builds and publishes it to exyoff.github.io/MiniOutbreak on every push
  to `main` (the public Exyoff/Exyoff.github.io repo gets a commit "MiniOutbreak: publish <sha>").
  `Progress.md` is the project's record.
- **Exyoff/MiniDAYZcustom** (this one): the original. `MiniDayZ+1.0` is the **official** build and the only
  reference; 1.2 and Reloaded are fan mods. `toolkit/` reads and drives it; `toolkit/comparison/` holds the
  eleven area surveys, `backlog.md` and `user_asks.md` (every deliberate difference the user asked for).

On the user's Windows machine both are cloned side by side under `A:/Documents/VS Code Projects/`
(`MiniOutbreak`, `MiniDAYZcustom`), with worktrees in `wt/`. The remake's `OriginalData/` (gitignored) holds
the official `data.js` as `C2SourceData.js` (BOM stripped), the 1.2 export kept as `C2SourceData.fanmod-1.2.js`,
and `images/` as a junction to `MiniDayZ+1.0/images`.

## Where things stand

| branch (minioutbreak) | commit | what |
|---|---|---|
| `main` | `cba0ea8` | **Published** 2026-10-06: Stage 27 whole and its review. Nothing since is live. |
| integration `claude/minioutbreak-exploration-jhs27h` | `1dc2ca0` | **Merged, not published, not tested**: backlog tracks 29.1 DATA10, 29.2 AUDIO, 29.3 SURVIVAL, 29.4 ZOMBIE-NUMBERS, 29.5 (the melee kept as it is, by the user's word), 29.6 ZOMBIE-AI, 29.7 CAMERA, 29.8 HUD-TOP, 29.9 TYPE, 29.10 HUD-TOUCH; the user's asks 30.1 FRONTS, 30.2 HANDS, 30.3 LOOT-AREAS, 30.4-30.6 the editor (`editor.bat`, `tools/editor/`), 30.7 TILES. Each has its entry in Progress Stage 29 or 30. |
| `work/hitfx` (`wt/hitfx`) | `2c00e35` | **Built, not merged**: 29.11 HITFX (the grey flash, the shake, armour's rule, the shield, the blood, the skull, the drops, a wolf's five sounds). Its Progress entry is `toolkit/handoff/stage29-30/entries/progress_hitfx.md`. After merging, run `python tools/build.py --only atlas` (the shield icon is new art). |
| `work/depth` (`wt/depth`) | `95e649a` | **Briefed, not started**: 30.8 DEPTH, the user's ask of 2026-10-09 (no dark outside a room; from behind a building its front alone, see-through; things in front drawn over you). Brief: `toolkit/handoff/stage29-30/prompt_depth.md`. The art change may answer part of it (the new buildings' wall sprites hide when you run behind them): re-read the ask against the new art before building it. |
| `work/lines` (`wt/lines`) | `95e649a` | **Briefed, not started**: 29.12 LINES (backlog track 11). Brief: `prompt_lines.md`. |

The other `wt/*` worktrees are merged tracks' and can be removed (`git worktree remove`).

**Testing is deferred, by the user's rule**: *"can we skip checks until everything is implemented?"* (2026-10-07)
and *"Just make sure to perform tests at the end of every agent's work, that way 1 test verifies them all"*
(2026-10-08). Builders wrote their scenarios and ran none (only the lint subject, static Python, 15/15 at
`95e649a`). **One test pass is owed before anything is published**: the whole suite (every subject: several
merged files map to `*`), every scenario written since 27.3 (each Progress entry lists its own), the web build
(`tools/web_layout.cjs`, `web_perf.cjs`, `web_world.cjs`, the forty seeds' hash in both runtimes), and reviews
of the riskiest merges (CAMERA, ZOMBIE-AI, FRONTS, LOOT-AREAS, the editor's writer). Then publish `main`.

## Next, in order (when the user resumes this work)

1. Merge `work/hitfx`, rebuild the atlas, insert its Progress entry (`ins29.py hitfx "- **6.13**" "29.11, being hit"`).
2. Build 30.8 DEPTH (re-read against the art change first), then 29.12 LINES.
3. The backlog's tracks from track 12 (CLOCK) on, in `backlog.md`'s order, two at a time, each with the
   user's asks over it (`user_asks.md`): the melee stays the remake's (no one-target swing, crits, damage
   numbers); USE shows at every container; military only at the army's places; and the rest.
4. The questions at the top of Progress Stage 29: each goes the way written first until the user answers.
5. The one test pass, fixes, then publish.

## The user's standing instructions

- *"continue, compare against official minidayz, use screenshots and source code to compare. Do not stop
  working until it is identical except for the changes i had asked for specifically"*.
- *"continue, less agents"*: at most two agents at once.
- No pull requests. Push only to the integration branch and `main`.
- Testing deferred to one pass at the end (above); windows (the game, the original in headless Edge) may be
  opened again since 2026-10-08.
- No model names in commits, code or docs. End commit messages with the attribution lines your own
  session is given.

## How the work was run (what to reuse)

- **Build prompts**: `toolkit/handoff/stage29-30/`. `track_common.md` is the template every builder got
  (worktree, `OriginalData` junction, own `APPDATA`, the testing rule, the commit line); `mkprompt29.py
  <track> <backlog no.> <N> <base> [note]` makes a backlog track's prompt from `backlog.md`, `mkprompt30.py
  <track> <n> <KEY> <base>` a user ask's from `brief30.md`; `notes/` holds each track's extra note (what was
  merged under it, the asks over it, what runs beside it). `ins29.py` inserts a builder's saved entry into
  Progress. All of them name the session's scratchpad as `S`: change it before reuse.
- **Tracks in worktrees**: `git worktree add -b work/<track> ../wt/<track> <integration head>`, copy
  `game/built`, and make `OriginalData` a junction to the main checkout's
  (`New-Item -ItemType Junction`). Merge with `--no-ff` into the integration branch, run the lint subject, push.
- **Per-agent save dirs** so captures never collide: each agent its own `APPDATA`.
- **Lua with no window**: LÖVE's own `lua51.dll` loaded from Python through ctypes compiles and runs pure Lua
  (`luaL_loadfile`), used to check merges and data files.
- **Scenarios by name**: `toolkit/handoff/run_named.py NAMESFILE [--subjects lint]` (`REMAKE=<checkout>`).
- **One capture**: `lovec game --shot --frames N --beat B --res WxH --script "step;step" [--world SEED]`
  from the remake's root.
- **The original, running**: `toolkit/serve.sh`, then `node toolkit/orig.cjs --out DIR --touch --steps
  "wait:22000;tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;..."` (headless Edge; the menu answers
  taps only). Never assign a bare global in a `js:` step.
