# Expressions: how they were resolved

Conditions and actions were named by pulling their implementations out of the
live runtime and joining them to `data.js` by SID. Expressions needed a
different route, and two attempts failed before one worked. All three are
recorded, because the failures are the useful part.

## What expressions are

A third index space alongside conditions and actions. In `data.js` they appear
inside ACE parameters as `[19, exp_index, params]` (system),
`[20, object_type, exp_index, ...]` (object) and
`[22, object_type, "Behavior", exp_index, ...]`.

## Attempt 1: join by index. Impossible.

Expression nodes carry no index. Sampling every numeric field:

| Node type | Count | Numeric fields |
|-----------|-------|----------------|
| 19 system | 7714  | `ze` = -1, `Yy` = -1 |
| 20 object | 17703 | `ze` = -1, `Yy` = -1 |
| 21 instance var | 5739 | `Yy` = 0..50, 36 distinct — this IS the variable index |
| 22 behavior | 75 | `ze` = 0..6 |

Construct 2 binds an expression's function to its node at load time and
discards the index; the `-1`s are the discarded slots. Type 21 keeps its index
because a variable index is data, not a function reference.

## Attempt 2: walk both trees in parallel. Wrong.

```
nodes paired 10343, type mismatches 5235, ACEs with length mismatch 53796 (82%)
```

Two real traversal faults were found by diffing a single ACE's two trees:

- `sb` is not a child list. It holds non-node data, and descending it invented
  nodes that do not exist in `data.js`.
- A conditional node keeps its third branch in `dq`, not in `second`. Walking
  only `first`/`second` silently dropped it.

Fixing both changed almost nothing (53803 length mismatches). The fixes were
correct — they were verified against one ACE by hand — but they were not the
cause.

## Attempt 3: match ACEs by SID first. Correct.

The real fault was one level up. Both sides were being walked in parallel and
ACE N zipped to ACE N, but the two walks do not visit ACEs in the same order.
The evidence was in the shape of the disagreement, not its size: the node-count
delta was symmetric (+1 on 10378 ACEs, -1 on 9484, +2 on 5855, -2 on 5214),
which is misalignment rather than a structural difference. A condition with
zero parameters pairing against a populated tree confirmed it.

SIDs already solve this — they are what made the ACE join exact — and were
simply not used here. Matching ACEs by SID first, then walking each matched
pair's trees:

```
runtime rows 65733, joined by sid 61620, skipped 4113 (duplicate or missing sid)
  length mismatches 4658, type mismatches 0
  nodes paired 106683
  expression keys 101, ambiguous 0
```

Zero type mismatches across 106683 paired nodes, and every key resolving to
exactly one implementation, is what makes this trustworthy. The 4658 remaining
length mismatches are skipped rather than forced.

## Result

101 expression keys mapped to implementations, drawn from 84 distinct bodies.

## The lesson worth keeping

All three attempts produced output. Attempts 1 and 2 produced *plausible*
output — 29 named keys, tidy tables — that was wrong. The only reason it was
caught is that each node reports its `type` from the same namespace both sides
use, so the pairing could check itself and be rejected. Any future correlation
here should carry the same kind of built-in check before its results are used.
