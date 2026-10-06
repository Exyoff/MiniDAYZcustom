"""Cut every animation frame of one ORIGINAL object out of its packed sheets.

    python3 cut.py <object> <outdir>        object: zed_normal_skin1 | fnx_pistol[t139] | t139 | 139
    python3 cut.py --list <pattern>         names that contain <pattern> (and their t-ids)

Writes <outdir>/<animation>_<n>.png (animation name lower-cased, n from 0 -- the layout of the remake's
Assets/Sprites/<name>/ rip, e.g. wolf_skin/attack_down_0.png, gui_panel/default_0.png) and
<outdir>/frames.json:

    {"object", "tid", "plugin", "families", "anims": [{"name", "file_prefix", "speed", "loop",
     "repeat_count", "repeat_to", "pingpong", "frames": [{"file", "sheet", "rect": [x, y, w, h],
     "duration", "origin": [px, py], "points": {"name": [px, py]}, "points_rel_origin": {...},
     "poly_rel": [...]}]}]}

origin/points are pixels from the frame's top-left (C2 stores them as fractions of the frame and does
not clamp them, so they can fall outside it). poly_rel is the collision polygon exactly as exported
(fractions of the frame, relative to the origin). Objects without frames (Tilemap, TiledBg,
NinePatch, Particles, Spritefont2) get their one texture copied as texture.png. When several object
types share a sheet name (sprite reuse), the bare name picks the first and says so.
"""
import json
import sys
from pathlib import Path

from PIL import Image

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2orig as C  # noqa: E402


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == '--list':
        names = C.type_names()
        for i, n in sorted(names.items()):
            if sys.argv[2] in n:
                print('t%-5d %-36s %s' % (i, n, C.plugin_name(C.project(), i)))
        return
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    who, out = sys.argv[1], Path(sys.argv[2])
    P = C.project()
    hits = C.type_index_by_name(who)
    if not hits:
        sys.exit('no object named %r (try: cut.py --list %s)' % (who, who.split('[')[0]))
    if len(hits) > 1:
        print('note: %d object types match %r: %s -- cutting t%d' % (len(hits), who, ', '.join('t%d' % h for h in hits), hits[0]))
    ti = hits[0]
    t = P[C.TYPES][ti]
    names = C.type_names(P)
    fam = [names[f] for f, ms in C.family_members(P).items() if ti in ms]
    out.mkdir(parents=True, exist_ok=True)
    sheets = {}

    def sheet(path):
        if path not in sheets:
            sheets[path] = Image.open(C.GAME / path).convert('RGBA')
        return sheets[path]

    meta = {'object': names[ti], 'tid': ti, 'plugin': C.plugin_name(P, ti), 'families': fam, 'anims': []}
    used = set()
    n_files = 0
    for an in t[7] or []:
        prefix = an[0].lower()
        if prefix in used:                     # two animations differing only in case
            prefix = an[0]
        used.add(prefix)
        A = {'name': an[0], 'file_prefix': prefix, 'speed': an[1], 'loop': bool(an[2]), 'repeat_count': an[3],
             'repeat_to': an[4], 'pingpong': bool(an[5]), 'frames': []}
        for k, fr in enumerate(an[7]):
            x, y, w, h = fr[2], fr[3], fr[4], fr[5]
            fname = '%s_%d.png' % (prefix, k)
            if fr[0] and w and h:
                sheet(fr[0]).crop((x, y, x + w, y + h)).save(out / fname)
                n_files += 1
            ox, oy = fr[7] * w, fr[8] * h
            pts = {p[0]: [round(p[1] * w, 2), round(p[2] * h, 2)] for p in (fr[9] or [])}
            A['frames'].append({'file': fname if fr[0] else None, 'sheet': fr[0], 'rect': [x, y, w, h], 'duration': fr[6],
                                'origin': [round(ox, 2), round(oy, 2)], 'points': pts,
                                'points_rel_origin': {k2: [round(v[0] - ox, 2), round(v[1] - oy, 2)] for k2, v in pts.items()},
                                'poly_rel': fr[10] or []})
        meta['anims'].append(A)
    if t[6]:
        Image.open(C.GAME / t[6][0]).save(out / 'texture.png')
        meta['texture'] = t[6][0]
        n_files += 1
    (out / 'frames.json').write_text(json.dumps(meta, indent=1))
    print('%s (t%d, %s): %d animations, %d images -> %s' % (names[ti], ti, meta['plugin'], len(meta['anims']), n_files, out))


if __name__ == '__main__':
    main()
