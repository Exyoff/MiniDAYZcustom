"""Run a chosen set of scenarios through verify.py's own machinery.

usage: run_named.py NAMESFILE [--subjects a,b] [--list]
NAMESFILE: one scenario name per line; a line may carry ' -- was ...' after the
name, or be a prefix of the name. Partners (pairs_with) are pulled in by main().
"""
import sys, os
ROOT = os.environ.get("REMAKE", os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "minioutbreak"))
sys.path.insert(0, os.path.join(ROOT, "tools"))
os.chdir(ROOT)
import verify
names_file = sys.argv[1]
subjects = set()
if "--subjects" in sys.argv:
    subjects = set(sys.argv[sys.argv.index("--subjects") + 1].split(","))
wanted = []
for line in open(names_file, encoding="utf-8"):
    line = line.strip()
    if not line:
        continue
    for sep in (" -- was ", " — was ", " (was "):
        if sep in line:
            line = line.split(sep)[0].strip()
    wanted.append(line)
all_names = [s["name"] for _, s in verify.LOADED]
chosen, missing = set(), []
for w in wanted:
    hit = [n for n in all_names if n == w] or [n for n in all_names if n.startswith(w)] or [n for n in all_names if w.startswith(n)]
    if hit:
        chosen.update(hit)
    else:
        missing.append(w)
for subject, s in verify.LOADED:
    if subject in subjects:
        chosen.add(s["name"])
verify.LOADED = [(sub, s) for sub, s in verify.LOADED if s["name"] in chosen]
verify.SCENARIOS = [s for _, s in verify.LOADED]
print(f"[run_named] {len(verify.SCENARIOS)} chosen; {len(missing)} names not found:")
for m in missing:
    print("   missing:", m)
if "--list" in sys.argv:
    from collections import Counter
    print(Counter(sub for sub, _ in verify.LOADED))
    raise SystemExit(0)
sys.argv = ["verify.py", "--timeout", "900"]
raise SystemExit(verify.main())
