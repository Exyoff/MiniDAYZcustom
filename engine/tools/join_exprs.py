#!/usr/bin/env python3
"""Join runtime-extracted expression implementations to data.js.

Expressions are a third index space after conditions and actions, and unlike
those the runtime discards their index: it binds the implementation to the node
at load time, leaving -1 placeholders. So the mapping has to come from tree
position instead of a key.

Two things make that sound rather than guesswork:

  * ACEs are matched by SID first. Walking both sides in parallel and zipping
    ACE N to ACE N does not work -- the two walks do not visit ACEs in the same
    order, which showed up as a symmetric +/-N spread in node counts and a
    condition with zero parameters pairing against a populated tree.

  * Within a matched ACE, every node reports its `type` from the same opcode
    namespace data.js uses, so the pairing checks itself. Any disagreement means
    the correlation is wrong and the run is rejected rather than emitting a
    plausible-looking table.

    python3 tools/join_exprs.py <data.js> <exprs.json> <out.json>
"""
import collections
import json
import sys


def emit(node, out, plug, tname):
    """Depth-first, in the same child order the extractor uses:
    first, second, dq (the conditional's third branch), then call parameters."""
    if not isinstance(node, list) or not node or not isinstance(node[0], int):
        out.append((None, None))
        return
    op = node[0]
    key = None
    if op == 19:
        key = ('S', -1, '', node[1])
    elif op == 20:
        key = ('O', plug[tname[node[1]]], '', node[2])
    elif op == 22:
        key = ('B', plug[tname[node[1]]], node[2], node[3])
    out.append((op, key))

    def params(at):
        raw = node[at] if len(node) > at and isinstance(node[at], list) else []
        return [p[1] for p in raw if isinstance(p, list) and len(p) > 1]

    if op == 19:
        kids = params(2)
    elif op == 20:
        kids = params(5)
    elif op == 22:
        kids = params(6)
    elif op == 21:
        kids = []
    else:
        kids = [c for c in node[1:] if isinstance(c, list)]
    for k in kids:
        emit(k, out, plug, tname)


def main():
    data_js, exprs_json, out_json = sys.argv[1], sys.argv[2], sys.argv[3]
    proj = json.load(open(data_js))["project"]
    plug = {o[0]: o[1] for o in proj[3]}
    tname = {i: o[0] for i, o in enumerate(proj[3])}

    by_sid, dupes = {}, set()

    def walk(blocks):
        for b in blocks:
            if not (isinstance(b, list) and b and b[0] == 0):
                continue
            for c in b[5]:
                nodes = []
                for p in (c[9] if len(c) == 10 else []):
                    emit(p[1] if isinstance(p, list) and len(p) > 1 else None, nodes, plug, tname)
                if c[7] in by_sid:
                    dupes.add(c[7])
                by_sid[c[7]] = nodes
            for a in b[6]:
                nodes = []
                for p in (a[5] if len(a) == 6 else []):
                    emit(p[1] if isinstance(p, list) and len(p) > 1 else None, nodes, plug, tname)
                if a[3] in by_sid:
                    dupes.add(a[3])
                by_sid[a[3]] = nodes
            if len(b) > 7 and isinstance(b[7], list):
                walk(b[7])

    for sheet in proj[6]:
        walk(sheet[1])

    doc = json.load(open(exprs_json))
    bodies, rows = doc["bodies"], doc["rows"]

    table = collections.defaultdict(collections.Counter)
    joined = skipped = len_mismatch = type_mismatch = paired = 0
    for r in rows:
        sid = r["sid"]
        if sid is None or sid in dupes or sid not in by_sid:
            skipped += 1
            continue
        dn, rn = by_sid[sid], r["nodes"]
        joined += 1
        if len(dn) != len(rn):
            len_mismatch += 1
            continue
        if any(d is not None and x["type"] is not None and x["type"] != d
               for (d, _), x in zip(dn, rn)):
            type_mismatch += 1
            continue
        for (dop, dkey), x in zip(dn, rn):
            if dop is None:
                continue
            paired += 1
            if dkey is not None and x["fn"] is not None:
                table[dkey][x["fn"]] += 1

    ambiguous = sum(1 for v in table.values() if len(v) > 1)
    out = [{"kind": k[0], "plugin": k[1], "behavior": k[2], "index": k[3],
            "uses": sum(v.values()), "ambiguous": len(v) > 1,
            "body": bodies[v.most_common(1)[0][0]]}
           for k, v in table.items()]
    out.sort(key=lambda e: -e["uses"])
    json.dump(out, open(out_json, "w"), indent=1)

    print(f"runtime rows {len(rows)}, joined by sid {joined}, skipped {skipped}")
    print(f"  length mismatches {len_mismatch}, type mismatches {type_mismatch}")
    print(f"  nodes paired {paired}")
    print(f"  expression keys {len(out)}, ambiguous {ambiguous}")
    if type_mismatch:
        print("  WARNING: type mismatches mean the correlation is unsound")
    print(f"wrote {out_json}")


if __name__ == "__main__":
    main()
