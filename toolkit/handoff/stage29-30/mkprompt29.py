"""Write a backlog track's build prompt: python mkprompt29.py <track> <num> <N> <base> [note file]

<num> is the track's number in toolkit/comparison/backlog.md, <N> its Progress item (29.N)."""
import sys

S = 'C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad'
B = 'A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/comparison/backlog.md'
track, num, n, base = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
extra = open(sys.argv[5], encoding='utf-8').read().strip() if len(sys.argv) > 5 else ''
common = open(S + '/track_common.md', encoding='utf-8').read()
backlog = open(B, encoding='utf-8').read()
i = backlog.index(f'\n### {num}. ') + 1
j = backlog.find('\n### ', i + 5)
brief = '#' + backlog[i:j if j > 0 else None].strip()
key = brief.split('. ', 1)[1].split(' ', 1)[0]

note = ("they are made from the official 1.0's data since 29.1, and `python tools/build.py --verify` passes. "
        "If your change moves what an extractor reads or adds art, rebuild and commit what changes, and nothing else.")
c = (common.replace('branched from the integration head', f'branched from the integration head ({base})')
     .replace('WT', f'A:/Documents/VS Code Projects/wt/{track}')
     .replace('TRACK_DATA_NOTE', note).replace('TRACK', track))
prompt = c + f"""
## Your track: {key} (backlog track {num})
The planner's brief follows. Its findings' evidence is in the surveys it cites; check each against the original's events and data yourself before you change anything, and where the brief is wrong, follow the original and say so. The user's Stage 30 asks (Progress.md's Stage 30, `toolkit/comparison/user_asks.md`) stand over the brief where they meet it.

{brief}

Where the brief says to run a subject or `--changed`, the user's rules above win: write the scenarios without running them, and list them for the final test pass. Measuring and capturing the running original, and capturing the remake to look at what you build, are fine.

{extra}

## What to return
Build it, commit each piece as soon as it works (a usage limit can stop you at any time), and end with a report: a summary; the commits; the files changed; generated files changed; the scenarios added and changed (written, not run: say so); captures you took and looked at, the remake's and the original's; what of the original you read and what it says; choices (each with the one change that reverses it); what you found and left; every check that waits for the final check; and a Progress.md entry for the track in Progress's voice -- "### 29.{n} Title", a short paragraph quoting the user's standing instruction where it applies, "- [x] 29.{n}.K **Lead.** prose" items with the original's evidence, "- [ ]" for what is left, "*Verified ...*" lines saying honestly what was and was not run -- plus dated notes for earlier Progress items this overturns ("- **N.N.N** (2026-10-08): ..."), or none. Save the Progress entry to `{S}/progress_{track}.md` as well.
"""
open(f'{S}/prompt_{track}.md', 'w', encoding='utf-8').write(prompt)
print(f'{S}/prompt_{track}.md', len(prompt), key)
