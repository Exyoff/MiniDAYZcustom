import json,collections,sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import c2orig
p=c2orig.project()
sigs=collections.defaultdict(collections.Counter)
trig=collections.defaultdict(collections.Counter)
def walk(ev):
    if ev[0]!=0: return
    for c in ev[5]:
        sigs[c[1]][tuple(x[0] for x in c[9]) if len(c)>9 else ()]+=1
        trig[c[1]][c[3]]+=1
    for a in ev[6]:
        sigs[a[1]][tuple(x[0] for x in a[5]) if len(a)>5 else ()]+=1
    if len(ev)>7:
        for s in ev[7]: walk(s)
for sh in p[6]:
    for ev in sh[1]: walk(ev)
json.dump({str(k):{','.join(map(str,s)):n for s,n in v.items()} for k,v in sigs.items()}, open('sigs.json','w'))
json.dump({str(k):dict(v) for k,v in trig.items()}, open('trig.json','w'))
