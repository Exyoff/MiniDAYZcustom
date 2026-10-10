"""Write a Stage 30 track's build prompt: python mkprompt30.py <track> <n> <KEY> <base>"""
import sys

S = 'C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad'
track, n, key, base = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
common = open(S + '/track_common.md', encoding='utf-8').read()
briefs = open(S + '/brief30.md', encoding='utf-8').read()
i = briefs.index(f'## 30.{n} {key}')
j = briefs.find('\n## 30.', i + 5)
brief = briefs[i:j if j > 0 else None].strip()

note = ("they are made from the official 1.0's data since 29.1, and `python tools/build.py --verify` passes. "
        "If your change moves what an extractor reads or adds art, rebuild and commit what changes, and nothing else.")
c = (common.replace('branched from the integration head', f'branched from the integration head ({base})')
     .replace('WT', f'A:/Documents/VS Code Projects/wt/{track}')
     .replace('TRACK_DATA_NOTE', note).replace('TRACK', track))
prompt = c + f"""
## Your track: 30.{n} {key} -- the user's own ask
This track is not from the comparison's backlog: it is what the user asked for on 2026-10-07, and it stands over the original where the two differ (the user's words are at the top of Progress.md's Stage 30). Where the original does the same thing, copy the original's numbers and ways, and check them in it. Read Progress.md's Stage 30 and Stage 29 ("Needs a word from you") first.

{brief}

The user's rules for this phase win over anything above about testing: build, look at what you build in captures, run the lint subject, write the scenarios without running them -- one test pass at the end of every track's work verifies them all, and your report lists what it should run.

## What to return
Build it, commit each piece as soon as it works (a usage limit can stop you at any time), and end with a report: a summary; the commits; the files changed; generated files changed; the scenarios added and changed (written, not run: say so); captures you took and looked at; what of the original you compared; choices (each with the one change that reverses it); what you found and left; and a Progress.md entry for the track in Progress's voice -- "### 30.{n} Title", a short paragraph quoting the user's words it answers, "- [x] 30.{n}.K **Lead.** prose" items with what was measured, "- [ ]" for what is left, "*Verified ...*" lines -- plus dated notes for earlier Progress items this overturns ("- **N.N.N** (2026-10-08): ..."), or none. Save the Progress entry to `{S}/progress_{track}.md` as well.
"""
open(f'{S}/prompt_{track}.md', 'w', encoding='utf-8').write(prompt)
print(f'{S}/prompt_{track}.md', len(prompt))
