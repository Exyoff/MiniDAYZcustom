"""python ins29.py <track> <first note's marker> <label> [29|30]: insert a saved entry and its notes into Progress."""
import sys
S = 'C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad'
P = 'A:/Documents/VS Code Projects/MiniOutbreak/Progress.md'
track, marker, label = sys.argv[1], sys.argv[2], sys.argv[3]
stage = sys.argv[4] if len(sys.argv) > 4 else '29'
e = open(f'{S}/progress_{track}.md', encoding='utf-8').read()
k = e.index('\n' + marker)
entry, notes = e[:k].rstrip(), e[k:].strip()
lines = entry.split('\n')
while lines and (lines[-1].strip() == '' or lines[-1].lstrip().startswith(('Dated notes', '**From', '### What'))):
    lines.pop()
entry = '\n'.join(lines).strip()
p = open(P, encoding='utf-8').read()
T, N = f'@@TRACKS{stage}@@', f'@@NOTES{stage}@@'
assert p.count(T) == 1 and p.count(N) == 1
p = p.replace(T, entry + '\n\n' + T).replace(N, f'**From {label}:**\n\n' + notes + '\n\n' + N)
open(P, 'w', encoding='utf-8', newline='\n').write(p)
print('ok', entry.split('\n')[0])
