"""Dump the ORIGINAL's six event sheets as readable text, one line per condition / action.

    python3 dump_events.py            -> events/<sheet>.txt for every sheet (and events/ALL.txt)

Each line starts with the event's number path (12.3.1: 3rd sub-event's 1st sub-event of top-level
event 12; only event blocks and groups are numbered, as in Construct 2's editor). Then:

    group  "Name" (active|inactive)   a group header (groups are event blocks, numbered too)
    event  [or] sid=...                an event block header; [or] = an OR block, whose 2nd.. conditions read `or`
    on     ...   a trigger condition         if   ...   a normal condition
    loop   ...   a looping condition (For each, Repeat, While...)
    else   System.Else()
    do     ...   an action
    var    name = value (number|text[, static, constant])   a local (or, at sheet root, global) variable
    include "Sheet"

Conditions/actions read Object[.Behaviour].Name(params): Sprite.cnds.X etc. are named by
ace/acenames.json, objects by their art (c2orig.type_names). Parameter conventions: `var#N` is
an instance variable by index (the export keeps no names for them), `opt:N` a combo choice,
`key:87(W)` a key code, comparison operators as = <> < <= > >=, `sound:"x"` an audio file,
`layout:"Map"`. Expressions are printed as expressions (operators: + - * / % ^ & | = <> < <= > >=,
& is Construct's string-concatenation / logical and; a ? b : c). Comments are not in the export.
Every group's own `System.IsGroupActive("<group>")` condition (an exporter artefact on all 219
groups) is left out.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2orig as C  # noqa: E402

OUT = Path(__file__).resolve().parent / 'events'
COL = 12


def dump_sheet(pr, sheet):
    name, items = sheet[0], sheet[1]
    lines = ['# event sheet %s  (%d top-level items)' % (name, len(items))]

    def emit(path, depth, kind, text):
        lines.append('%-*s %s%-5s %s' % (COL, path, '  ' * depth, kind, text))

    def walk(items, prefix, depth):
        n = 0
        for it in items:
            if it[0] == 1:
                emit(prefix or '-', depth, 'var', C.var_line(it))
                continue
            if it[0] == 2:
                emit(prefix or '-', depth, 'incl', C.q(it[1]))
                continue
            n += 1
            path = ('%s.%d' % (prefix, n)) if prefix else str(n)
            group, orblock, sid = it[1], it[2], it[4]
            if group:
                emit(path, depth, 'group', '%s (%s)  sid=%s' % (C.q(group[1]), 'active' if group[0] else 'INACTIVE', sid))
            else:
                emit(path, depth, 'event', ('[or] ' if orblock else '') + 'sid=%s' % sid)
            conds = it[5]
            if group and len(conds) == 1 and conds[0][0] == -1 and pr.acename(conds[0][1]) == 'IsGroupActive':
                conds = []        # the exporter gives every group an IsGroupActive(<itself>) condition
            for ci, c in enumerate(conds):
                kind, s = pr.cond(c)
                emit(path, depth + 1, 'or' if orblock and ci and kind == 'if' else kind, s)
            for a in it[6]:
                emit(path, depth + 1, 'do', pr.act(a))
            if len(it) > 7:
                walk(it[7], path, depth + 1)

    walk(items, '', 0)
    return lines


def main():
    pr = C.Printer()
    OUT.mkdir(exist_ok=True)
    allx = []
    for sheet in pr.P[C.SHEETS]:
        lines = dump_sheet(pr, sheet)
        (OUT / ('%s.txt' % sheet[0])).write_text('\n'.join(lines) + '\n')
        allx += lines + ['']
        print('%-16s %6d lines' % (sheet[0], len(lines)))
    (OUT / 'ALL.txt').write_text('\n'.join(allx))
    # OUTLINE.txt: every group and every Function ("on Function.OnFunction") with its path, to navigate by
    outline = []
    for line in allx:
        if line.startswith('# event sheet'):
            outline.append(line)
        elif ' group ' in line or 'Function.OnFunction(' in line:
            outline.append(line.split('  sid=')[0].replace('on    Function.OnFunction', 'function'))
    (OUT / 'OUTLINE.txt').write_text('\n'.join(outline) + '\n')


if __name__ == '__main__':
    main()
