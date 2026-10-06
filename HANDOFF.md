# Handoff: MiniOutbreak (updated by the desktop session, 2026-10-06)

Read this first, then `toolkit/README.md`, `toolkit/comparison/backlog.md` and the remake's `Progress.md`
("Where this stands", Stage 27.2/27.3 and Stage 29).

## The two repositories

- **Exyoff/minioutbreak** (private): the remake, a LÖVE 11.5 game (Lua 5.1) with a love.js web build.
  `.github/workflows/publish-web.yml` builds and publishes it to exyoff.github.io/MiniOutbreak on every push
  to `main` (the public Exyoff/Exyoff.github.io repo gets a commit "MiniOutbreak: publish <sha>").
  `Progress.md` is the project's record.
- **Exyoff/MiniDAYZcustom** (this one): the original. `MiniDayZ+1.0` is the **official** build and the only
  reference; 1.2 and Reloaded are fan mods. `toolkit/` reads and drives it; `toolkit/comparison/` holds the
  eleven area surveys and `backlog.md`.

On the user's Windows machine both are cloned side by side under `A:/Documents/VS Code Projects/`
(`MiniOutbreak`, `MiniDAYZcustom`), with worktrees in `wt/`. The remake's `OriginalData/` (gitignored) holds
the official `data.js` as `C2SourceData.js` (BOM stripped), the 1.2 export kept as `C2SourceData.fanmod-1.2.js`,
and `images/` as a junction to `MiniDayZ+1.0/images`.

## Where things stand

| branch (minioutbreak) | commit | what |
|---|---|---|
| `main` = integration `claude/minioutbreak-exploration-jhs27h` | `cba0ea8` | **Published** 2026-10-06 07:18 UTC: all of Stage 27 (27.2 and 27.3 joined 27.1, 27.4 and 28) and the merge's review in four lenses, twelve findings fixed (Progress 27.3.14-27.3.31). The whole suite ran over it: 375/376, the one a stale scenario since fixed. The live build was played in headless Edge: seeds 1-10 hash the same as natively, START works. |
| `work/data10` (worktree `wt/data10`) | from cba0ea8 | **Building**: backlog track 1, every generated file from the official 1.0 data |
| `work/audio` (worktree `wt/audio`) | from cba0ea8 | **Building**: backlog track 2, the web build's one-ear panning, falloff and mix |

The comparison is finished: eleven surveys, 437 findings, 299 differences to make identical in 96 ordered
tracks (`toolkit/comparison/backlog.md`), 43 kept because the user asked, and 7 questions only the user can
answer (also at the top of the remake's Progress Stage 29).

## Next, in order

1. When DATA10 and AUDIO finish: an adversarial review of each (an agent whose job is to break it, against
   the original), a fix pass, merge into the integration branch, the test set the merge reaches (the whole
   suite when a change touches what everything runs through), Progress Stage 29.1/29.2 from the builders'
   entries, publish (fast-forward `main`, push, check the pages repo and the live build).
2. Then the backlog's tracks in order, two at a time, each the same way. Tracks that need
   `tools/build.py` must wait for DATA10's merge: until then a build over the official data changes four
   generated files the commit made from 1.2's.
3. The questions in Stage 29: each goes the way written there until the user answers; the four tracks that
   wait on one (BOARD-LOOK, ISLANDS, BUNKER, ENDGAME) wait.

## The user's standing instructions

- *"continue, compare against official minidayz, use screenshots and source code to compare. Do not stop
  working until it is identical except for the changes i had asked for specifically"* (run as a loop).
- *"continue, less agents"*: at most two agents at once.
- Publish to `main` when a stage is done (Progress.md ground rule 8); *"publish it too"*.
- No pull requests. Push only to the integration branch and `main`.
- Agents run only their own scenarios plus lint: never the full suite, never `--changed`, never a whole
  subject but lint. The orchestrator runs a test set at each merge.
- No model names in commits, code or docs. End commit messages with the attribution lines your own
  session is given.

## How the work was run (what to reuse)

- **On Windows**: prompts and the track template used on 2026-10-06 are summarised in the remake's Progress 29; the build agents get a template naming the worktree, `OriginalData` as a junction, their own `APPDATA` for every game run, the testing rule and the commit line, then the backlog's brief for their track, and return a Progress entry (they never edit Progress.md).
- **Tracks in worktrees**: `git worktree add -b work/<track> ../wt/<track> <integration head>`, copy
  `game/built`, and give it `OriginalData/` (gitignored in the remake; `tools/c2data.py`, the build's metadata
  step and `tools/extract_rooms.py` read `OriginalData/C2SourceData.js`, the official `data.js` with its
  BOM stripped, and `OriginalData/images` = `MiniDayZ+1.0/images`). Paths in the findings below that start
  `/tmp/claude-0/...` were the cloud session's and did not come along.
- **Per-agent save dirs** so captures never collide: each agent its own `APPDATA` (with `APPDATA/LOVE` ->
  `$XDG_DATA_HOME/love`, which verify.py's reload scenario needs) and `XDG_DATA_HOME`; on Linux headless,
  `Xvfb :99`, `ALSOFT_DRIVERS=null`, `SDL_AUDIODRIVER=dummy`.
- **Scenarios by name**: `toolkit/handoff/run_named.py NAMESFILE [--subjects lint]` (one name or prefix
  per line; `REMAKE=<checkout>` picks the tree).
- **One capture**: `love game --shot --frames N --beat B --res WxH --script "step;step" [--world SEED]`
  from the remake's root; `toolkit/handoff/cap.py` wraps it (set its `WT`).
- **The original, running**: `toolkit/serve.sh`, then `node toolkit/orig.cjs --out DIR --touch --steps
  "wait:22000;tap:640,353;wait:2500;tap:540,317;wait:1500;tap:740,428;..."` (headless, the menu answers
  taps only: New game 640,353, Novice 540,317, Start 740,428). Never assign a bare global in a `js:` step.
- **Workflows**: `toolkit/handoff/track.js` (build, review, fix for one track) and `survey.js` (the
  area-by-area comparison) are the scripts the cloud session ran; their `SP` constant points at its
  scratchpad -- change it, and the paths in their prompts, before reuse.

