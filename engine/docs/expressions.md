# Expressions: why they are still unresolved

Conditions and actions were named by pulling their implementations out of the
live runtime and joining them to `data.js` by SID. The same approach does not
work for expressions, and this records exactly where it breaks so the next
attempt does not repeat it.

## What expressions are

Expressions are a third index space alongside conditions and actions. In
`data.js` they appear inside ACE parameters as nodes such as
`[19, exp_index, params]` (system), `[20, object_type, exp_index, ...]`
(object) and `[22, object_type, "Behavior", exp_index, ...]`. Naming them
requires mapping each `(kind, plugin, exp_index)` to an implementation.

`mdz_run` counts every unresolved expression rather than silently yielding
zero: 3559 of them in 30 ticks of `Game_events`. This is why `Loading_events`
can reach 100% ACE coverage and still change no variables — its logic gates on
expressions.

## Attempt 1: join by index. Does not work.

Expression nodes in the runtime carry no index. Sampling every numeric field on
them shows, for the two types that matter:

| Node type | Count | Numeric fields |
|-----------|-------|----------------|
| 19 system | 7714  | `ze` = -1, `Yy` = -1 |
| 20 object | 17703 | `ze` = -1, `Yy` = -1 |
| 21 instance var | 5739 | `Yy` = 0..50, 36 distinct — this IS the variable index |
| 22 behavior | 75 | `ze` = 0..6 |

Construct 2 resolves an expression at load time by binding its function
directly to the node, then discards the index. The `-1` placeholders are the
discarded slots. Type 21 keeps its index because an instance variable index is
data, not a function reference.

So there is no key to join on, unlike the SIDs that made the ACE join exact.

## Attempt 2: positional correlation. Wrong, and known to be wrong.

Without a key, the remaining option is to walk the `data.js` expression tree
and the runtime's node tree in the same order and pair them position by
position. `tools/extract_exprs.js` emits the runtime side in a fixed
depth-first order for exactly this.

Because every runtime node also reports its `type` — the same opcode namespace
`data.js` uses — the pairing can check itself. It does not hold:

```
data.js ACEs walked: 65733
runtime ACEs:        65733
nodes paired:        10343
type mismatches:      5235
ACEs with length mismatch: 53796   (82%)
distinct expression keys: 29, of which 11 resolve to more than one body
```

The two walks disagree on tree shape for most ACEs, so the pairing is invalid
and the resulting table was discarded rather than shipped. A table built from
it would have looked entirely plausible while being wrong — the same failure
mode that made the runtime's `index` field look like an ACE index earlier.

## What to try next

The traversal itself is the suspect, not the idea. Specifically:

- Whether `ea` and `sb` on an expression node are the call's parameters, or
  something else that should not be descended into at all.
- Whether `first`/`second` are populated for every operator type, or whether
  some operators keep operands elsewhere.
- Whether Construct 2 folds or rewrites expression trees at load, in which case
  the runtime tree is genuinely not isomorphic to the `data.js` tree and
  position can never line up.

Diffing one ACE's two trees node by node would settle which of these it is.
There are only 84 distinct expression bodies in the whole project, so once the
correlation is sound the naming itself is small.
