"""Shared reader for the ORIGINAL Mini DayZ export (Construct 2 data.js), used by dump_events.py,
make_index.py, cut.py and missing.py. Read-only; nothing here touches a git checkout.

    import c2orig as C
    P = C.project()                 # the top-level `project` array
    names = C.type_names(P)         # t-index -> readable unique name
    ace = C.ace_table()             # ref-table index -> {'owner','kind','name'} (see ace/match.py)
    C.expr(P, names, ace, node)     # print an expression node
"""
import json
import re
from collections import Counter
from functools import lru_cache
from pathlib import Path

HERE = Path(__file__).resolve().parent
import os
# The official build, beside this folder in the MiniDAYZcustom checkout (ORIG_GAME overrides).
GAME = Path(os.environ.get('ORIG_GAME', HERE.parent / 'MiniDayZ+1.0'))
DATA = GAME / 'data.js'                                          # read with utf-8-sig: it has a BOM
IMAGES = GAME / 'images'
SHEET_RE = re.compile(r'^images/(?P<name>.+)-sheet\d+\.png$')

# project indices
PLUGINS, TYPES, FAMILIES, LAYOUTS, SHEETS, AUDIO, CONTAINERS = 2, 3, 4, 5, 6, 7, 28
# object type record: [name, plugin(ref index), is_family, instvar_sids, behs_count, fx_count,
#                      texture_file, animations, behaviors, global, on_loader_layout, sid, effects, tilepoly]
# animation: [name, speed, loop, repeat_count, repeat_to, pingpong, sid, frames]
# frame: [image, filesize, sheet_x, sheet_y, w, h, duration, origin_x(rel), origin_y(rel), [[pt, rx, ry]...], poly, pixfmt]
CMP = ['=', '<>', '<', '<=', '>', '>=']
# expression node types 4..17 (verified against the minified ExpNode: 7 is divide, 10 is & (concat/and))
BINOP = {4: '+', 5: '-', 6: '*', 7: '/', 8: '%', 9: '^', 10: '&', 11: '|', 12: '=', 13: '<>',
         14: '<', 15: '<=', 16: '>', 17: '>='}
PREC = {11: 1, 10: 2, 12: 3, 13: 3, 14: 3, 15: 3, 16: 3, 17: 3, 4: 4, 5: 4, 6: 5, 7: 5, 8: 5, 9: 6}
KEYS = {8: 'Backspace', 9: 'Tab', 13: 'Enter', 16: 'Shift', 17: 'Ctrl', 18: 'Alt', 27: 'Esc', 32: 'Space',
        37: 'Left', 38: 'Up', 39: 'Right', 40: 'Down', 46: 'Delete'}


@lru_cache(None)
def project():
    with DATA.open(encoding='utf-8-sig') as fh:
        return json.load(fh)['project']


@lru_cache(None)
def ace_table():
    """Object-reference-table index -> row of ace/acenames.json (plugin/behaviour ctors and every
    condition/action/expression with its recovered C2 name)."""
    rows = json.load(open(HERE / 'ace' / 'acenames.json'))['table']
    return {r['i']: r for r in rows}


def asset_name(frame):
    m = SHEET_RE.match(frame[0] or '')
    return m.group('name') if m else None


def plugin_name(P, ti):
    return ace_table()[P[TYPES][ti][1]]['name']


@lru_cache(None)
def _names():
    P = project()
    types = P[TYPES]
    base = {}
    for i, t in enumerate(types):
        nm = None
        for an in t[7] or []:
            for fr in an[7]:
                nm = asset_name(fr)
                if nm:
                    break
            if nm:
                break
        if not nm and t[6]:
            # Tilemap / TiledBg / NinePatch / Particles / Spritefont2 carry one texture instead of frames
            nm = re.sub(r'(-sheet\d+)?\.png$', '', t[6][0].replace('images/', ''))
        base[i] = nm
    # families are named after their first member, as the old dumps did (fam_ak74_rifle)
    for fam in P[FAMILIES]:
        fi, members = fam[0], fam[1:]
        first = base.get(members[0]) if members else None
        base[fi] = 'fam_' + (first or 't%d' % members[0]) if members else 'fam_t%d' % fi
    count = Counter(v for v in base.values() if v)
    per_plugin = Counter(plugin_name(P, i) for i in range(len(types)) if not base[i])
    out = {}
    for i, t in enumerate(types):
        nm = base[i]
        if not nm:
            pn = plugin_name(P, i)
            # Keyboard, Mouse, Touch, Audio, Function...: the only object of its plugin
            out[i] = pn if per_plugin[pn] == 1 else '%s[t%d]' % (pn, i)   # Text[t5], Arr[t235]
        elif count[nm] > 1:
            out[i] = '%s[t%d]' % (nm, i)                     # sprite reuse: three types draw fnx_pistol
        else:
            out[i] = nm
    return out, base


def type_names(P=None):
    return _names()[0]


def base_names():
    """t-index -> the bare sheet name (None for objects without art); not unique."""
    return _names()[1]


def family_members(P):
    return {fam[0]: fam[1:] for fam in P[FAMILIES]}


def type_index_by_name(name):
    """Accept 'zed_base', 'fnx_pistol[t139]', 't139' or '139'."""
    names, base = _names()
    m = re.match(r'^(?:.*\[)?t?(\d+)\]?$', name)
    if m and int(m.group(1)) in names and (name.startswith('t') or name.isdigit() or '[' in name):
        return [int(m.group(1))]
    hits = [i for i, n in names.items() if n == name]
    return hits or [i for i, b in base.items() if b == name]


def q(s):
    return '"' + str(s).replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '') + '"'


class Printer:
    def __init__(self, P=None):
        self.P = P or project()
        self.names = type_names(self.P)
        self.ace = ace_table()
        self.layouts = [L[0] for L in self.P[LAYOUTS]]

    def tname(self, ti):
        return self.names.get(ti, 't%d' % ti)

    def acename(self, idx):
        r = self.ace.get(idx)
        return r['name'] if r else '#%d' % idx

    # ---- expressions -------------------------------------------------------
    def expr(self, e, parent_prec=0):
        t = e[0]
        if t == 0:
            return str(e[1])
        if t == 1:
            v = e[1]
            return ('%d.0' % v) if float(v).is_integer() and abs(v) < 1e15 else repr(v)
        if t == 2:
            return q(e[1])
        if t == 3:
            return '-' + self.expr(e[1], 7)
        if t in BINOP:
            p = PREC[t]
            s = '%s %s %s' % (self.expr(e[1], p), BINOP[t], self.expr(e[2], p + 1))
            return '(%s)' % s if p < parent_prec else s
        if t == 18:
            s = '%s ? %s : %s' % (self.expr(e[1], 1), self.expr(e[2], 1), self.expr(e[3], 1))
            return '(%s)' % s if parent_prec > 0 else s
        if t == 19:
            nm = self.acename(e[1])
            args = e[2] if len(e) == 3 else None
            return '%s(%s)' % (nm, ', '.join(self.expr(a) for a in args)) if args is not None else nm
        if t == 20:   # object expression [20, type, func, returns_string, instance_expr, params]
            obj = self.tname(e[1]) + ('(%s)' % self.expr(e[4]) if e[4] else '')
            nm = self.acename(e[2])
            args = e[5] if len(e) == 6 else None
            return '%s.%s%s' % (obj, nm, '(%s)' % ', '.join(self.expr(a) for a in args) if args is not None else '')
        if t == 21:   # instance variable [21, type, returns_string, instance_expr, index]
            obj = self.tname(e[1]) + ('(%s)' % self.expr(e[3]) if e[3] else '')
            return '%s.var#%d' % (obj, e[4])
        if t == 22:   # behaviour expression [22, type, behaviour name, func, returns_string, instance_expr, params]
            obj = self.tname(e[1]) + ('(%s)' % self.expr(e[5]) if e[5] else '')
            nm = self.acename(e[3])
            args = e[6] if len(e) == 7 else None
            return '%s.%s.%s%s' % (obj, e[2], nm, '(%s)' % ', '.join(self.expr(a) for a in args) if args is not None else '')
        if t == 23:
            return e[1]
        return '?expr%r' % (e,)

    # ---- parameters --------------------------------------------------------
    def param(self, p):
        t = p[0]
        if t in (0, 1, 5, 7):
            return self.expr(p[1])
        if t == 3:
            return 'opt:%d' % p[1]                      # combo box index (e.g. 0 = not looping)
        if t == 8:
            return CMP[p[1]] if 0 <= p[1] < len(CMP) else 'cmp%d' % p[1]
        if t == 6:
            return 'layout:' + q(p[1] if isinstance(p[1], str) else self.layouts[p[1]])
        if t == 9:
            k = p[1]
            return 'key:%d(%s)' % (k, KEYS.get(k, chr(k) if 48 <= k <= 90 else '?'))
        if t == 4:
            return self.tname(p[1])
        if t == 10:
            return 'var#%d' % p[1]
        if t == 11:
            return p[1]
        if t == 2:
            return 'sound:%s%s' % (q(p[1][0]), ' (music)' if p[1][1] else '')
        if t == 12:
            return 'file:' + q(p[1])
        if t == 13:
            return ', '.join(self.param(x) for x in p[1:]) if len(p) > 1 else ''
        return '?param%r' % (p,)

    def params(self, ps):
        return ', '.join(x for x in (self.param(p) for p in ps) if x != '')

    # ---- conditions / actions ---------------------------------------------
    def cond(self, c):
        # [type, func, behaviour, trigger(0/1/2 fast), looping, inverted, static, sid, ?, params]
        obj = 'System' if c[0] == -1 else self.tname(c[0])
        beh = '.' + c[2] if c[2] else ''
        nm = self.acename(c[1])
        ps = c[9] if len(c) == 10 else []
        s = '%s%s%s.%s(%s)' % ('NOT ' if c[5] else '', obj, beh, nm, self.params(ps))
        kind = 'on' if c[3] else ('loop' if c[4] else 'if')
        if c[0] == -1 and nm == 'Else':
            kind = 'else'
        return kind, s

    def act(self, a):
        # [type, func, behaviour, sid, ?, params]
        obj = 'System' if a[0] == -1 else self.tname(a[0])
        beh = '.' + a[2] if a[2] else ''
        ps = a[5] if len(a) == 6 else []
        return '%s%s.%s(%s)' % (obj, beh, self.acename(a[1]), self.params(ps))


def var_line(v):
    # [1, name, type(0 number / 1 text), initial, static, constant, sid, ?]
    typ = 'text' if v[2] == 1 else 'number'
    init = q(v[3]) if v[2] == 1 else v[3]
    flags = [f for f, on in (('static', v[4]), ('constant', v[5])) if on]
    return '%s = %s (%s%s)' % (v[1], init, typ, (', ' + ', '.join(flags)) if flags else '')
