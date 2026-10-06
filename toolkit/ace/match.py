"""Name every entry of the original's object-reference table (c2runtime.js `function wc(){return[...]}`).

data.js refers to plugins, behaviours, conditions, actions and expressions by their INDEX in that
table, and the runtime is Closure-minified, so the table reads `W.prototype.n.Ek`. Two facts make it
recoverable:

  * Closure renamed each property name to ONE short name program-wide: `Ek` is SetPosition on every
    plugin, `od` is SetEnabled on every behaviour, `Hh` is SetSpeed on Bullet, Car, 8Direction and
    Rotate. So a name learnt in one place holds everywhere (names are decided by vote).
  * The minified bodies keep their shape, literals, strings and builtin calls (Math.floor, .length).
    Unminified C2 runtimes from other games (../ref/*, see REFS) give the real names with the real
    bodies, and a token-sequence similarity pairs them. Conditions that are triggers all read
    `return true`, so for those the code AROUND each `runtime.trigger(...cnds.X ...)` call is compared too.
    Identifier renames (set_float->F, runtime->b, inst->j...) are learnt from the confident pairs of a
    first pass and applied in a second.

The ACE categories are `k` = cnds, `n` = acts, `A` = exps (verified: Function.acts.CallFunction is
`zc.prototype.n.CallFunction`, Audio.acts.Play is `Cc.prototype.n.Play`).

Behaviours/plugins with no unminified reference we could fetch (Particles, Tilemap, Car, 8Direction,
Fade, LOS, Sin, Timer, Turret, ScrollTo) were named by hand from their minified code (MANUAL; each
entry says what in the code identifies it).

  python3 match.py [-v]     -> acenames.json  {"ctors": {...}, "table": [one row per index], "conflicts": [...]}
"""
import difflib
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
REFDIR = HERE.parent / 'ref'
MIN_RUNTIME = HERE.parent.parent / 'MiniDayZ+1.0' / 'c2runtime.js'
# reference dumps (refdump.cjs ref) and the runtime each came from (for trigger contexts)
REFS = {
    'ref_Meekuzo_Jotaro-Sans-Simulator.json': 'Meekuzo_Jotaro-Sans-Simulator/c2runtime.js',
    'ref_666proxy.json': '666proxy_666proxy.github.io/c2runtime.js',
    'ref_jgmy_jigsawpuzzle.json': 'jgmy_jigsawpuzzle/c2runtime.js',
    'ref_pandz3rd_petualangan-gatotkaca-game.json': 'pandz3rd_petualangan-gatotkaca-game/c2runtime.js',
    'ref_scblaster_src.json': 'scblaster/src/c2runtime.js',
}
CAT = {'k': 'cnds', 'n': 'acts', 'A': 'exps'}
AMBIG = {}
DEBUG = set(sys.argv[sys.argv.index('--debug') + 1].split(',')) if '--debug' in sys.argv else set()

# minified ctor -> (kind, C2 id). Identified from flags (project[2]), their own code (strings like
# "option", localforage, "auto pointer text...", MSXML2, nine-patch fillRect) and default behaviour
# names in the object types ("Fade", "Pin", "Bullet", "Solid", "NoSave", "LineOfSight", "Turret"...).
IDENT = {
    'H': ('plugin', 'System'),
    'Mc': ('plugin', 'NinePatch'), 'Ac': ('plugin', 'AJAX'), 'Bc': ('plugin', 'Arr'),
    'Cc': ('plugin', 'Audio'), 'Dc': ('plugin', 'Browser'), 'Ec': ('plugin', 'Dictionary'),
    'Nc': ('plugin', 'Particles'), 'Lc': ('plugin', 'Mouse'), 'Gc': ('plugin', 'List'),
    'Kc': ('plugin', 'LocalStorage'), 'zc': ('plugin', 'Function'), 'Fc': ('plugin', 'Keyboard'),
    'W': ('plugin', 'Sprite'), 'Pc': ('plugin', 'TextBox'), 'Qc': ('plugin', 'TiledBg'),
    'Oc': ('plugin', 'Spritefont2'), 'Rc': ('plugin', 'Tilemap'), 'Tc': ('plugin', 'XML'),
    'Y': ('plugin', 'Text'), 'Sc': ('plugin', 'Touch'),
    'xc': ('behavior', 'solid'), 'yc': ('behavior', 'NoSave'), 'Uc': ('behavior', 'Anchor'),
    'Vc': ('behavior', 'Bullet'), 'Wc': ('behavior', 'Car'), 'Xc': ('behavior', 'DragnDrop'),
    'Yc': ('behavior', 'EightDir'), 'Zc': ('behavior', 'Fade'), '$c': ('behavior', 'Flash'),
    'ad': ('behavior', 'LOS'), 'bd': ('behavior', 'Pin'), 'cd': ('behavior', 'Rotate'),
    'dd': ('behavior', 'Sin'), 'jd': ('behavior', 'Timer'), 'kd': ('behavior', 'Turret'),
    'ld': ('behavior', 'bound'), 'md': ('behavior', 'destroy'), 'od': ('behavior', 'scrollto'),
}

# Hand-named from the minified code (see min.json and c2runtime.js around each ctor).
MANUAL = {
    # Fade: onCreate reads mj=fadeInTime(B[1]) tk=waitTime(B[2]) $k=fadeOutTime(B[3]); tick
    # triggers Au after the fade-in, Uu after the wait, VA after the fade-out.
    ('Zc', 'k', 'Au'): 'OnFadeInEnd', ('Zc', 'k', 'Uu'): 'OnWaitEnd', ('Zc', 'k', 'VA'): 'OnFadeOutEnd',
    ('Zc', 'n', 'PC'): 'StartFade', ('Zc', 'n', 'IB'): 'RestartFade', ('Zc', 'n', 'XB'): 'SetFadeInTime',
    ('Zc', 'n', 'JC'): 'SetWaitTime', ('Zc', 'n', 'YB'): 'SetFadeOutTime',
    # Timer: Jb = timers by lower-cased tag {current,total,duration,regular}
    ('jd', 'k', 'iB'): 'OnTimer', ('jd', 'n', 'QC'): 'StartTimer', ('jd', 'n', 'UC'): 'StopTimer',
    ('jd', 'A', 'ju'): 'CurrentTime', ('jd', 'A', '$C'): 'TotalTime', ('jd', 'A', 'ku'): 'Duration',
    # Sin: kb=active(B[0]) wf=period(B[3]) Cb=phase i, Value = wave(i)*magnitude
    ('dd', 'n', 'NB'): 'SetActive', ('dd', 'n', 'sC'): 'SetPeriod', ('dd', 'n', 'tC'): 'SetPhase',
    ('dd', 'A', 'eD'): 'Value',
    # ScrollTo: LC(magnitude, duration, mode) stores the shake on the behaviour
    ('od', 'n', 'LC'): 'Shake',
    # Turret: props [range, rof, rotate, rotateSpeed, targetMode, predictiveAim(rm), projectileSpeed(Cp), enabled, useCells]
    ('kd', 'k', 'iA'): 'HasTarget', ('kd', 'k', 'gB'): 'OnShoot', ('kd', 'k', 'Lq'): 'OnTargetAcquired',
    ('kd', 'n', 'iz'): 'AddTarget', ('kd', 'n', 'vz'): 'ClearTargets', ('kd', 'n', 'vC'): 'SetPredictiveAim',
    ('kd', 'n', 'wC'): 'SetProjectileSpeed', ('kd', 'A', 'ZC'): 'TargetUID',
    # Line of sight: gA(obj) walks both instance lists; hA(x, y); TB stores a cone in radians; hz adds an obstacle type
    ('ad', 'k', 'gA'): 'HasLOSToObject', ('ad', 'k', 'hA'): 'HasLOSToPosition',
    ('ad', 'n', 'TB'): 'SetConeOfView', ('ad', 'n', 'hz'): 'AddObstacle',
    # Car: Mm=steerSpeed(B[3]) no=driftRecover(B[4]); ru=MovingAngle(rc)
    ('Wc', 'n', 'BC'): 'SetSteerSpeed', ('Wc', 'n', 'WB'): 'SetDriftRecover',
    # 8 direction: M/L are the velocity vector
    ('Yc', 'n', 'HC'): 'SetVectorX', ('Yc', 'n', 'IC'): 'SetVectorY',
    # Particles: props [rate(Am), cone, type, initSpeed(Lo), size, opacity, grow, xr, yr, speedRandom(Jm), ..., acc(za), gravity, ..., timeout]
    ('Nc', 'n', 'xC'): 'SetRate', ('Nc', 'n', 'cC'): 'SetInitSpeed', ('Nc', 'n', 'AC'): 'SetSpeedRandomiser',
    ('Nc', 'n', 'qC'): 'SetAcc', ('Nc', 'n', 'bv'): 'SetTimeout',
    # Tilemap: Gz(x,y,cmp,v) compares tileAt & mask; CC(x,y,tile,state); Uz(x,y,w,h) erases; DC(x,y,w,h,tile,state)
    ('Rc', 'k', 'Gz'): 'CompareTileAt', ('Rc', 'n', 'CC'): 'SetTile', ('Rc', 'n', 'Uz'): 'EraseTileRange',
    ('Rc', 'n', 'DC'): 'SetTileRange', ('Rc', 'A', 'yB'): 'PositionToTileX', ('Rc', 'A', 'zB'): 'PositionToTileY',
    # System: the event block constructor tests `conditions[0].func == H.prototype.k.lu` for is_else_block
    ('H', 'k', 'lu'): 'Else',
    # System: (a, cmp, b) -> do_cmp(a, cmp, b), params any/cmp/any = "Compare two values"
    ('H', 'k', 'xz'): 'Compare',
    # Sprite: one object param, collision-cell query at offset (0, 0), not a trigger
    ('W', 'k', 'wA'): 'IsOverlapping',
    # System: clamps at 0 and stores runtime.timescale
    ('H', 'n', 'EC'): 'SetTimescale',
    # System: one layout parameter (type 6), stored as the layout to change to
    ('H', 'n', 'cA'): 'GoToLayout',
    # Browser: `return false` (deprecated in C2)
    ('Dc', 'k', 'oA'): 'IsDownloadingUpdate',
    # Dictionary: dict[key] = value unconditionally (SetKey only if the key exists)
    ('Ec', 'n', 'gz'): 'AddKey',
    # Text: text += value
    ('Y', 'n', 'fu'): 'AppendText',
    # TextBox: elem.focus()
    ('Pc', 'n', 'Yu'): 'SetFocus',
    # --- near-ties the token match cannot split (bodies differ only in a mangled field); decided by
    # what the field is. Layer view edges, from the common IsOnScreen: right<Ca left, bottom<Da top,
    # left>Ha right, top>Ga bottom.
    ('H', 'A', 'yH'): 'viewportleft', ('H', 'A', 'xH'): 'viewportbottom',
    ('H', 'A', 'AH'): 'viewporttop', ('H', 'A', 'zH'): 'viewportright',
    # The runtime's save/load tick reads Np as the slot to SAVE to (IndexedDB put) and Kl as the slot to
    # LOAD from (continuous preview sets Kl="__c2_continuouspreview"): JA sets Kl, LB sets Np.
    ('H', 'n', 'JA'): 'LoadState', ('H', 'n', 'LB'): 'SaveState',
    # Audio: iy(x) sets Ej then lo(Ej||tl) which writes .muted -> setMuted, not setLooping (looping is rf)
    ('Cc', 'n', 'pC'): 'SetMuted',
    # Bullet: ok accumulates distance moved each tick (saved as "t") -> travelled; Hz compares it
    ('Vc', 'n', 'VB'): 'SetDistanceTravelled', ('Vc', 'k', 'Hz'): 'CompareTravelled',
    # Touch: Ri is the trigger touch id (Ri=e.identifier / pointerId), Ve the trigger index
    ('Sc', 'A', 'aD'): 'TouchID', ('Sc', 'A', 'bD'): 'TouchIndex',
    # Created/destroyed: the runtime triggers Fh after createInstance and wu in DestroyInstance
    ('W', 'k', 'Fh'): 'OnCreated', ('W', 'k', 'wu'): 'OnDestroyed',
}
# shared by every behaviour that has the method (Closure renamed one name to one name)
GLOBAL_MANUAL = {
    ('k', 'pu'): 'IsMoving', ('k', 'ln'): 'CompareSpeed', ('n', 'yn'): 'Stop', ('n', 'Hh'): 'SetSpeed',
    ('n', 'Pq'): 'SetMaxSpeed', ('n', 'Ck'): 'SetAcceleration', ('n', 'Wu'): 'SetDeceleration',
    ('n', 'Rq'): 'SimulateControl', ('n', 'od'): 'SetEnabled', ('n', 'Qq'): 'SetRange',
    ('A', 'Ik'): 'Speed', ('A', 'qu'): 'MaxSpeed', ('A', 'ru'): 'MovingAngle',
}

KEYWORDS = set('''break case catch continue default delete do else finally for function if in instanceof new
return switch this throw try typeof var void while with null true false undefined NaN Infinity'''.split())
TOK = re.compile(r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|0[xX][0-9a-fA-F]+|\d*\.?\d+(?:[eE][-+]?\d+)?|[A-Za-z_$][\w$]*|===|!==|==|!=|<=|>=|&&|\|\||\+\+|--|[-+*/%<>=!&|^~?:;,.(){}\[\]]''')


def raw_tokens(src):
    out = []
    for t in TOK.findall(src):
        if t == 'true':
            out += ['!', '0']
        elif t == 'false':
            out += ['!', '1']
        elif t in '{};':
            continue      # Closure reshapes blocks, ifs into &&, etc.
        else:
            out.append(t)
    return out


def is_ident(t):
    return t[0].isalpha() or t[0] in '_$'


def norm(toks, keep, rename=None):
    out = []
    for t in toks:
        if t[0] in '"\'':
            out.append('S' + t[1:-1])
        elif t[0].isdigit() or (t[0] == '.' and len(t) > 1):
            try:
                out.append('N%g' % float(t))
            except ValueError:
                out.append('N?')
        elif is_ident(t):
            if rename and t in rename:
                out.append(rename[t])
            elif t in KEYWORDS or t in keep:
                out.append(t)
            else:
                out.append('I')
        else:
            out.append(t)
    return out


def props(srcs):
    s = set()
    for x in srcs:
        s.update(re.findall(r'\.([A-Za-z_$][\w$]*)', x))
        s.update(t for t in re.findall(r'[A-Za-z_$][\w$]*', x) if len(t) >= 4)
    return s


def trigger_contexts(text, pat):
    out = defaultdict(list)
    for m in re.finditer(pat, text):
        key = tuple(g for g in m.groups() if g is not None)
        out[key].append(text[max(0, m.start() - 260):m.end() + 60])
    return out


def main():
    mn = json.load(open(HERE / 'min.json'))
    ref = defaultdict(lambda: defaultdict(list))     # (kind, id, cat) -> name -> [(src, len)]
    rseq = defaultdict(list)                         # C2 id -> [sequence of trigger names in source order, per reference]
    rctx = defaultdict(list)                         # (kind, id, name) -> [context]
    for jf, rt in REFS.items():
        if not (HERE / jf).exists():
            continue
        r = json.load(open(HERE / jf))
        for kind in ('plugins', 'behaviors'):
            for pid, d in r[kind].items():
                for cat, ms in d.items():
                    for name, f in ms.items():
                        ref[('behavior' if kind == 'behaviors' else 'plugin', pid, cat)][name].append((f['src'], f['len']))
        text = open(REFDIR / rt, encoding='utf-8-sig').read()
        pat = r'trigger\(\s*cr\.(?:plugins_\.(\w+)|behaviors\.(\w+)|(system_object))\.prototype\.cnds\.(\w+)'
        seqs = defaultdict(list)
        for m in re.finditer(pat, text):
            seqs['System' if m.group(3) else (m.group(1) or m.group(2))].append(m.group(4))
        for owner, seq in seqs.items():
            rseq[owner].append(seq)
        for key, ctxs in trigger_contexts(text, pat).items():
            owner, name = key[0], key[1]
            owner = 'System' if owner == 'system_object' else owner
            for kind in ('plugin', 'behavior'):
                rctx[(kind, owner, name)].extend(ctxs)
    mtext = MIN_RUNTIME.read_text()
    mtext = mtext[:mtext.rfind('return[')]
    mctx = defaultdict(list)                         # (ctor, mangled) -> [context]
    for key, ctxs in trigger_contexts(mtext, r'trigger\(([\w$]+)\.prototype\.k\.([\w$]+)').items():
        mctx[key].extend(ctxs)
    # The order in which a plugin fires its triggers is the strongest evidence for which is which
    # (OnTapGesture vs OnDoubleTapGesture all read `return true`): if the minified sequence has the
    # same shape as a reference's (each name replaced by the order of its first appearance), or is a
    # prefix of it, pair them position by position.
    mseq = defaultdict(list)
    for m in re.finditer(r'trigger\(([\w$]+)\.prototype\.k\.([\w$]+)', mtext):
        mseq[m.group(1)].append(m.group(2))

    def canon(seq):
        first = {}
        return [first.setdefault(x, len(first)) for x in seq]
    seq_names = {}                                   # (ctor, mangled) -> real
    for c, seq in mseq.items():
        if c not in IDENT:
            continue
        for rs in rseq.get(IDENT[c][1], []):
            if canon(rs)[:len(seq)] == canon(seq):
                for x, y in zip(seq, rs):
                    seq_names[(c, x)] = y
                break

    min_srcs = [f['src'] for d in mn['ctors'].values() for ms in d['cats'].values() for f in ms.values()]
    min_srcs += [r['src'] for r in mn['table']]
    ref_srcs = [s for v in ref.values() for lst in v.values() for s, _ in lst]
    min_props = props(min_srcs)
    keep = min_props & props(ref_srcs)

    # common ACEs (add_common_aces): same mangled name with the same body on 2+ plugins
    seen = defaultdict(set)
    for c, d in mn['ctors'].items():
        for cat, ms in d['cats'].items():
            for k, f in ms.items():
                seen[(cat, k, f['src'])].add(c)
    common = {}
    for (cat, k, src), cs in seen.items():
        if len(cs) >= 2 and all(IDENT[c][0] == 'plugin' for c in cs):
            common[(cat, k)] = src

    # what to match: (tag, kind, id, cat) -> {mangled: (src, len)}
    jobs = defaultdict(dict)
    for c, d in mn['ctors'].items():
        for cat, ms in d['cats'].items():
            for k, f in ms.items():
                if (cat, k) in common and common[(cat, k)] == f['src']:
                    jobs[('Common', 'plugin', 'Common', cat)][k] = (f['src'], f['len'])
                else:
                    kind, pid = IDENT[c]
                    jobs[(c, kind, pid, cat)][k] = (f['src'], f['len'])
    for row in mn['table']:
        m = re.match(r'^H\.prototype\.(\w)(?:\.([\w$]+)|\["([\w$]+)"\])$', row['expr'])
        if m:
            jobs[('H', 'plugin', 'System', m.group(1))][m.group(2) or m.group(3)] = (row['src'], row['len'])

    def run(rename):
        cache = {}

        def ntok(src, side):
            key = (src, side)
            if key not in cache:
                cache[key] = norm(raw_tokens(src), keep, rename if side == 'ref' else None)
            return cache[key]

        grams = {}

        def ng(t):
            key = id(t)
            if key not in grams:
                c = Counter()
                for n in (1, 2, 3):
                    c.update(tuple(t[i:i + n]) for i in range(len(t) - n + 1))
                grams[key] = (c, sum(c.values()))
            return grams[key]

        def sim(a, b):
            # SequenceMatcher alone is greedy (it will align 'I ) this . height' across the wrong
            # statement); n-gram overlap keeps 'this . height' telling SetHeight from SetWidth
            (ca, na), (cb, nb) = ng(a), ng(b)
            dice = 2 * sum((ca & cb).values()) / (na + nb) if na + nb else 0
            return 0.4 * difflib.SequenceMatcher(None, a, b, autojunk=False).ratio() + 0.6 * dice

        detail = {}
        for (tag, kind, pid, cat), ms in jobs.items():
            rc = dict(ref.get((kind, pid, CAT[cat]), {}))
            if kind == 'plugin' and pid not in ('System', 'Common'):
                # a plugin may override a common ACE (Text's SetWidth also flags the text dirty)
                for name, v in ref.get(('plugin', 'Common', CAT[cat]), {}).items():
                    rc.setdefault(name, v)
            if not rc:
                continue
            pairs = []
            fixed = {k for k in ms if k in rc}          # a name Closure kept (round, min, Play, CallFunction...)
            for k in fixed:
                detail[(tag, cat, k)] = (k, 'preserved')
            for k, (src, ln) in ms.items():
                if k in fixed:
                    continue
                a = ntok(src, 'min')
                mc = mctx.get((tag, k), []) if tag not in ('Common', 'H') else mctx.get((tag, k), [])
                for name, versions in rc.items():
                    if name in fixed or not versions:
                        continue
                    body = max(sim(a, ntok(rs, 'ref')) + (0.05 if rl == ln else 0) for rs, rl in versions)
                    rcx = rctx.get((kind, pid, name), [])
                    if cat == 'k' and (mc or rcx):
                        if mc and rcx:
                            cx = max(sim(ntok(x, 'min'), ntok(y, 'ref')) for x in mc[:4] for y in rcx[:4])
                        else:
                            cx = 0.0     # one side is a trigger and the other is not
                        sc = 0.35 * body + 0.65 * cx
                    else:
                        sc = body
                    pairs.append((sc, k, name))
            pairs.sort(key=lambda p: -p[0])
            # near-ties: the best and the runner-up for one minified method within 0.03 (see AMBIG)
            best = {}
            for sc, k, name in pairs:
                best.setdefault(k, []).append((sc, name))
            for k, lst in best.items():
                if len(lst) > 1 and lst[0][0] - lst[1][0] < 0.03:
                    AMBIG[(tag, cat, k)] = [(n, round(x, 3)) for x, n in lst[:4]]
            used_m, used_r = set(), set()
            for sc, k, name in pairs:
                if k in used_m or name in used_r:
                    continue
                used_m.add(k); used_r.add(name)
                detail[(tag, cat, k)] = (name, round(sc, 3))
            if DEBUG:
                for sc, k, name in pairs:
                    if k in DEBUG:
                        print('DBG', tag, cat, k, name, round(sc, 3))
        return detail

    # pass 1, then learn identifier renames from confident long pairs, then pass 2
    detail = run(None)
    learn = Counter()
    for (tag, cat, k), (name, sc) in detail.items():
        if not isinstance(sc, float) or sc < 0.8:
            continue
        kind, pid = ('plugin', 'Common') if tag == 'Common' else IDENT[tag]
        msrc = jobs[(tag, kind, pid, cat)][k][0]
        a = raw_tokens(msrc)
        versions = ref.get((kind, pid, CAT[cat]), {}).get(name) or ref.get(('plugin', 'Common', CAT[cat]), {}).get(name, [])
        for rs, _ in versions:
            b = raw_tokens(rs)
            if len(a) < 12:
                continue
            sm = difflib.SequenceMatcher(None, [t if not is_ident(t) else 'I' for t in a], [t if not is_ident(t) else 'I' for t in b], autojunk=False)
            for blk in sm.get_matching_blocks():
                for o in range(blk.size):
                    ia, ib = blk.a + o, blk.b + o
                    x, y = a[ia], b[ib]
                    prop = ib > 0 and b[ib - 1] == '.' and (b[ib - 2] == 'cr' or (ia > 0 and a[ia - 1] == '.'))
                    if prop and is_ident(x) and is_ident(y) and x != y and len(x) <= 3 and y not in KEYWORDS and y not in min_props:
                        learn[(y, x)] += 1
    by_ref = defaultdict(Counter)
    for (y, x), n in learn.items():
        by_ref[y][x] += n
    rename = {}
    for y, cnt in by_ref.items():
        (x, n), = cnt.most_common(1)
        if n >= 3 and n >= 0.75 * sum(cnt.values()):
            rename[y] = x
    keep |= set(rename.values())
    detail = run(rename)

    for (c, k), name in seq_names.items():
        detail[(c, 'k', k)] = (name, 'trigger-order')
    votes = defaultdict(Counter)
    for (tag, cat, k), (name, sc) in detail.items():
        votes[(cat, k)][name] += sc if isinstance(sc, float) else 50
    for (c, cat, k), name in MANUAL.items():
        votes[(cat, k)][name] += 100
        detail[(c, cat, k)] = (name, 'manual')
    for (cat, k), name in GLOBAL_MANUAL.items():
        votes[(cat, k)][name] += 10

    names, conflicts = {}, []
    for key, cnt in votes.items():
        top = cnt.most_common()
        names[key] = top[0][0]
        if len(top) > 1 and top[1][1] > 0.6 * top[0][1]:
            conflicts.append([list(key), top[:3]])

    table, ctor_names = [], {}
    for i, row in enumerate(mn['table']):
        e = row['expr']
        if '.' not in e:
            kind, pid = IDENT[e]
            ctor_names[e] = pid
            table.append({'i': i, 'expr': e, 'kind': kind, 'name': pid})
            continue
        m = re.match(r'^([\w$]+)\.prototype\.(\w)(?:\.([\w$]+)|\["([\w$]+)"\])$', e)
        c, cat, k1, k2 = m.groups()
        k = k1 or k2
        owner = IDENT[c][1]
        real = names.get((cat, k))
        how = None
        d = detail.get((c, cat, k)) or detail.get(('Common', cat, k)) or detail.get(('H', cat, k))
        if d and d[1] == 'preserved':
            real, how = k, 'preserved'   # an extern name Closure kept: round, random, abs, Play, CallFunction...
        elif real is None:
            real, how = k, 'UNNAMED'
        table.append({'i': i, 'expr': e, 'kind': CAT[cat], 'owner': owner, 'name': real,
                      'how': how or (d[1] if d else 'global-name'), 'len': row['len']})
    json.dump({'ctors': ctor_names, 'table': table, 'conflicts': conflicts, 'rename': rename},
              open(HERE / 'acenames.json', 'w'), indent=1)
    weak = [t for t in table if isinstance(t.get('how'), float) and t['how'] < 0.6]
    print('table', len(table), 'renames learnt', len(rename), 'conflicts', len(conflicts), 'weak', len(weak))
    for c in conflicts:
        print('CONFLICT', c)
    if '-a' in sys.argv:
        for key, lst in sorted(AMBIG.items()):
            fin = names.get((key[1], key[2]))
            if detail.get(key, ('', ''))[1] in ('manual', 'trigger-order', 'preserved'):
                continue
            print('AMBIG', key, '->', fin, lst)
    for t in weak:
        print('WEAK', t['expr'], t['owner'], t['kind'], t['name'], t['how'])
    if '-v' in sys.argv:
        for t in table:
            print(t['i'], t['expr'], t.get('owner', ''), t['kind'], t['name'], t.get('how', ''))


if __name__ == '__main__':
    main()
