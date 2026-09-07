#!/usr/bin/env python3
"""Join runtime-extracted ACE implementations to data.js by SID.

tools/extract_aces.js pulls every condition/action object out of the live
runtime along with its implementation function. This pairs each one with the
(plugin, ace, behavior) key the decompiler prints, so an ACE can be named from
what its code actually does rather than from its parameter signature.

The join key is the Construct 2 SID, which both sides record. The runtime's own
`index` field is NOT the ACE index -- it is the ordinal within its event (0..227,
heavily skewed to 0, against data.js ACE indices of 38..468). Joining on it
produces plausible-looking nonsense, so SID is used instead.

    python3 tools/join_aces.py <data.js> <aces_sid.json> <out.json>
"""
import json
import sys
import collections


def load_meta(data_js):
    proj = json.load(open(data_js))["project"]
    plugin_of_name = {o[0]: o[1] for o in proj[3]}
    name_of_index = {i: o[0] for i, o in enumerate(proj[3])}

    meta = {}
    dupes = 0

    def walk(blocks):
        nonlocal dupes
        for b in blocks:
            if not (isinstance(b, list) and b and b[0] == 0):
                continue
            for c in b[5]:
                p = plugin_of_name[name_of_index[c[0]]] if c[0] >= 0 else -1
                if c[7] in meta:
                    dupes += 1
                meta[c[7]] = ("C", p, c[1], c[2] or "")
            for a in b[6]:
                p = plugin_of_name[name_of_index[a[0]]] if a[0] >= 0 else -1
                if a[3] in meta:
                    dupes += 1
                meta[a[3]] = ("A", p, a[1], a[2] or "")
            if len(b) > 7 and isinstance(b[7], list):
                walk(b[7])

    for sheet in proj[6]:
        walk(sheet[1])
    return meta, dupes


def main():
    data_js, aces_json, out_json = sys.argv[1], sys.argv[2], sys.argv[3]
    meta, dupes = load_meta(data_js)
    extracted = json.load(open(aces_json))
    rows, bodies = extracted["rows"], extracted["bodies"]

    # (kind, plugin, ace, behavior) -> {body_id: call_site_count}
    table = collections.defaultdict(collections.Counter)
    joined = 0
    for r in rows:
        key = meta.get(r["sid"])
        if key is None or "Ac" not in r["fns"]:
            continue
        joined += 1
        table[key][r["fns"]["Ac"]] += 1

    out = []
    conflicts = 0
    for (kind, plugin, ace, behavior), counter in sorted(table.items()):
        if len(counter) > 1:
            conflicts += 1
        body_id, _ = counter.most_common(1)[0]
        out.append({
            "kind": kind, "plugin": plugin, "ace": ace, "behavior": behavior,
            "call_sites": sum(counter.values()),
            "ambiguous": len(counter) > 1,
            "body": bodies[body_id],
        })
    out.sort(key=lambda e: -e["call_sites"])

    json.dump(out, open(out_json, "w"), indent=1)
    print(f"data.js call sites: {len(meta)} unique sids ({dupes} duplicate sids)")
    print(f"runtime rows: {len(rows)}, joined: {joined} ({100*joined/len(rows):.1f}%)")
    print(f"ACE keys: {len(out)}  (conditions {sum(1 for e in out if e['kind']=='C')}, "
          f"actions {sum(1 for e in out if e['kind']=='A')})")
    print(f"distinct implementation bodies: {len(set(e['body'] for e in out))}")
    print(f"keys with more than one body: {conflicts}")
    print(f"wrote {out_json}")


if __name__ == "__main__":
    main()
