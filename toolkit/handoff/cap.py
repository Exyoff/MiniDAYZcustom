"""Run one capture in this track's worktree and copy its shot and log out.

usage: python3 cap.py NAME FRAMES BEAT RES SCRIPTFILE [extra love args, e.g. --touch off, --world 7]
  RES is WxH, e.g. 1280x720 or 844x390 (a phone held sideways).
  SCRIPTFILE is a python expression evaluating to a list of script steps, with
  everything from tools/scenarios/common.py in scope (START, GOD, lua(), OPEN_BOARD ...)
  plus PHONE_START, which starts a run from the console (START is a click at a
  1280x720 point and misses the menu at other sizes).
Writes caps/NAME.png (the LAST frame) and caps/NAME.log beside this file; prints the '= ' lines.
Source env.sh first.
"""
import os, sys, shutil, subprocess, glob
WT = "/home/user/wt/merge27"
S = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, WT + "/tools")
from scenarios.common import *  # noqa
PHONE_START = 'press:f11;type:lua require("src.app").start("regular") return "go";press:return;press:f11'
name, frames, beat, res, scriptfile = sys.argv[1:6]
extra = sys.argv[6:]
steps = eval(open(scriptfile).read())
script = ";".join(steps)
save = S + "/xdg/love/minioutbreak"
for f in glob.glob(save + "/shot_*.png"):
    os.remove(f)
cmd = ["timeout", "300", "love", "game", "--shot", "--frames", frames, "--beat", beat,
       "--res", res, "--script", script] + extra
r = subprocess.run(cmd, cwd=WT, capture_output=True, text=True)
print("exit", r.returncode)
shots = sorted(glob.glob(save + "/shot_*.png"))
if shots:
    shutil.copy(shots[-1], f"{S}/caps/{name}.png")
    print("shot", f"{S}/caps/{name}.png")
if os.path.exists(save + "/run.log"):
    shutil.copy(save + "/run.log", f"{S}/caps/{name}.log")
    for line in open(save + "/run.log").read().splitlines():
        if "= " in line or "error" in line.lower() or "warn" in line.lower():
            print(line[:300])
