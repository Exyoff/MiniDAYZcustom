"""Write objects.txt (one line per object type) and globals.txt (global variables, layouts, layers,
what is placed on each, audio files) for the ORIGINAL Mini DayZ.

    python3 make_index.py
"""
import json
import os
import sys
import textwrap
from collections import Counter, defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import c2orig as C  # noqa: E402


def short(v, n=60):
    s = repr(v) if not isinstance(v, str) else C.q(v)
    return s if len(s) <= n else s[:n - 3] + '...'


def main():
    P = C.project()
    names = C.type_names(P)
    ace = C.ace_table()
    types = P[C.TYPES]
    fam_of = defaultdict(list)
    members = C.family_members(P)
    for f, ms in members.items():
        for m in ms:
            fam_of[m].append(f)

    # placed instances: counts per layout/layer, first instance's variable values, text samples
    placed = Counter()
    first_vars = {}
    text_sample = {}
    where = defaultdict(Counter)
    for L in P[C.LAYOUTS]:
        for ly in L[6]:
            for inst in ly[14]:
                ti = inst[1]
                placed[ti] += 1
                where[ti][L[0]] += 1
                first_vars.setdefault(ti, inst[3])
                props = inst[5] if len(inst) > 5 else []
                if ti not in text_sample and C.plugin_name(P, ti) in ('Text', 'Spritefont2', 'TextBox'):
                    s = next((p for p in props if isinstance(p, str) and p.strip()), None)
                    if s:
                        text_sample[ti] = s
        for inst in L[7]:
            ti = inst[1]
            placed[ti] += 1
            where[ti][L[0]] += 1
            first_vars.setdefault(ti, inst[3])

    lines = ['# objects of the ORIGINAL (MiniDayZ+1.0 data.js). One line per object type:',
             '# t-id | name (as the event dumps print it) | plugin | family/member | behaviours (instance name:type)',
             '# | instance vars: count, values of the first placed instance (the export keeps no var NAMES; events say var#N)',
             '# | placed: count per layout | effects | animations: name(frames, speed, loop) | sheet files',
             '']
    for ti, t in enumerate(types):
        plug = ace[t[1]]['name']
        parts = ['t%d' % ti, names[ti], plug]
        if t[2]:
            parts.append('FAMILY of %d: %s' % (len(members.get(ti, [])), ', '.join(names[m] for m in members.get(ti, [])[:12]) + (' ...' if len(members.get(ti, [])) > 12 else '')))
        elif fam_of.get(ti):
            parts.append('in ' + ', '.join(names[f] for f in fam_of[ti]))
        else:
            parts.append('-')
        behs = ['%s:%s' % (b[0], ace[b[1]]['name']) for b in (t[8] or [])]
        parts.append('behaviours ' + (', '.join(behs) if behs else '-'))
        nv = len(t[3] or [])
        fv = first_vars.get(ti)
        if fv:
            fv = [v[0] if isinstance(v, list) and len(v) == 1 else v for v in fv]
        parts.append('vars %d%s' % (nv, (' ' + short(fv, 140)) if fv else ''))
        if placed[ti]:
            parts.append('placed %d (%s)' % (placed[ti], ', '.join('%s %d' % kv for kv in where[ti].most_common())))
        else:
            parts.append('placed 0')
        if t[12]:
            parts.append('effects ' + ', '.join('%s(%s)' % (e[1], e[0]) for e in t[12]))
        if ti in text_sample:
            parts.append('text ' + short(text_sample[ti], 50))
        anims = []
        sheets = []
        for an in t[7] or []:
            anims.append('%s(%d, %g, %s)' % (an[0], len(an[7]), an[1], 'loop' if an[2] else 'once'))
            for fr in an[7]:
                if fr[0] and fr[0] not in sheets:
                    sheets.append(fr[0])
        if t[6]:
            sheets.append(t[6][0])
        parts.append('anims ' + (' '.join(anims) if anims else '-'))
        parts.append('sheets ' + (' '.join(s.replace('images/', '') for s in sheets) if sheets else '-'))
        lines.append(' | '.join(parts))
    (HERE / 'objects.txt').write_text('\n'.join(lines) + '\n')
    # name -> t-id for orig_helpers.js (ORIG.tid('wolf_eyes')); a bare sheet name maps to its first type
    nm = {}
    for i, n in names.items():
        nm[n] = i
    for i, b in sorted(C.base_names().items()):
        if b:
            nm.setdefault(b, i)
    (HERE / 'names.json').write_text(json.dumps(nm, sort_keys=True))

    # ---------------- globals.txt ----------------
    g = ['# global variables of the ORIGINAL, per event sheet (sheet-root variables are globals in C2)',
         '# name = initial value (type[, static, constant])', '']
    for sheet in P[C.SHEETS]:
        vs = [it for it in sheet[1] if it[0] == 1]
        inc = [it[1] for it in sheet[1] if it[0] == 2]
        g.append('## %s: %d globals%s' % (sheet[0], len(vs), ('; includes ' + ', '.join(inc)) if inc else ''))
        g += ['  ' + C.var_line(v) for v in vs]
        g.append('')
    g.append('# layouts (project[5]): name, size, event sheet; layers in draw order with their flags and what is placed on them')
    g.append('')
    for L in P[C.LAYOUTS]:
        g.append('## layout %s  %dx%d  sheet=%s  layers=%d  unbounded_scroll=%s' % (L[0], L[1], L[2], L[4], len(L[6]), L[3]))
        if L[8]:
            g.append('   layout effects: ' + ', '.join('%s(%s)' % (e[1], e[0]) for e in L[8]))
        if L[7]:
            c = Counter(names[i[1]] for i in L[7])
            g.append('   non-world objects: ' + ', '.join('%s x%d' % kv if kv[1] > 1 else kv[0] for kv in sorted(c.items())))
        for ly in L[6]:
            flags = []
            if not ly[3]:
                flags.append('HIDDEN')
            if not ly[5]:
                flags.append('opaque bg rgb%s' % (tuple(ly[4]),))
            if (ly[6], ly[7]) != (1, 1):
                flags.append('parallax %g,%g' % (ly[6], ly[7]))
            if ly[8] != 1:
                flags.append('opacity %g' % ly[8])
            if ly[11] != 1:
                flags.append('zoomrate %g' % ly[11])
            if ly[9]:
                flags.append('own texture')
            if ly[12]:
                flags.append('blend %d' % ly[12])
            if len(ly) > 15 and ly[15]:
                flags.append('effects ' + ','.join(e[1] for e in ly[15]))
            c = Counter(names[i[1]] for i in ly[14])
            g.append('   [%2d] %-24s %-40s %d placed%s' % (ly[1], ly[0], ' '.join(flags), len(ly[14]),
                     (': ' + ', '.join('%s x%d' % kv if kv[1] > 1 else kv[0] for kv in sorted(c.items()))) if c else ''))
        g.append('')
    g.append('# audio files (project[7]): name, bytes; media/ in the build')
    g += textwrap.wrap(', '.join('%s(%d)' % (a[0], a[1]) for a in P[C.AUDIO]), 150, initial_indent='  ', subsequent_indent='  ')
    g.append('')
    g.append('# containers (project[28]): types created/picked together')
    for ct in P[C.CONTAINERS]:
        g.append('  ' + ' + '.join(names[i] for i in ct))
    # The remake's game/data/generated/original_globals.lua was extracted from a DIFFERENT export: all
    # its values equal the MiniDayZ+1.2 fan mod's; these are the ones where the official 1.0 differs.
    lua = Path(os.environ.get('REMAKE', HERE.parent.parent / 'minioutbreak')) / 'game/data/generated/original_globals.lua'
    if lua.exists():
        import re
        rv = {}
        for m in re.finditer(r'^\s*([A-Za-z_]\w*)\s*=\s*("(?:[^"\\]|\\.)*"|[-\d.e]+)\s*,', lua.read_text(), re.M):
            v = m.group(2)
            rv[m.group(1)] = json.loads(v) if v.startswith('"') else float(v)
        g10 = {}
        for sheet in P[C.SHEETS]:
            for it in sheet[1]:
                if it[0] == 1:
                    g10.setdefault(it[1], it[3])
        diff = [(k, rv[k], g10.get(k)) for k in sorted(rv) if g10.get(k) != rv[k]]
        g.append('')
        g.append('# %d globals where the remake\'s game/data/generated/original_globals.lua does NOT hold the official 1.0 value' % len(diff))
        g.append('# (every one of its %d values equals the MiniDayZ+1.2 FAN MOD\'s data.js: it was extracted from a 1.2-like export).' % len(rv))
        g.append('# Several are overwritten at layout start by the difficulty events (Game_events 2.11.9-2.11.12); read those too.')
        for k, a, b in diff:
            g.append('  %-28s remake %-8s official 1.0 %s' % (k, a if not float(a).is_integer() else int(a), b))
    (HERE / 'globals.txt').write_text('\n'.join(g) + '\n')
    print('objects.txt %d types, globals.txt %d lines' % (len(types), len(g)))


if __name__ == '__main__':
    main()
