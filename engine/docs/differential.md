# Differential testing against the original runtime

Coverage says whether a call was recognised. It cannot say whether the answer
was right. This compares state directly: the browser runtime's own event-sheet
variables are read out of the live page and diffed against the C++ engine's.

`tools/dump_vars.js` reads the browser side; `mdz_run --dump-vars` reads the
engine side. Sheet members carry an unminified `name` next to a `data` field
holding the current value, so variables come out without needing any other
minified name.

## Results so far

| Comparison | Match |
|------------|-------|
| Initial state, all 401 variables | 401/401 |
| Browser on Menu vs engine on Menu only | 374/401 (93.3%) |
| Browser on Menu vs engine booted Loading -> Menu | 384/401 (95.8%) |
| After fixing Find and Language | 386/401 (96.3%) |

The first row is the useful one to keep in mind: two independent
implementations of the export format agree exactly on parsing. Everything
before the differential existed was self-consistency.

## What it has found that coverage could not

**Find returning zero.** `Find` was unimplemented, and unimplemented
expressions return 0. The game tests `Find(locale, "en") >= 0`, so "not
implemented" read as "found at index 0", every language branch matched, and the
last one won. The engine reported RU where the original reported EN, with the
sheet at 100% coverage throughout.

The general hazard: zero is only a safe default where zero is not itself a
legitimate answer. Anything feeding a `>= 0`, an index, or a count has the same
trap.

## Open: the Loader_ variables

Four `Loader_*` variables still disagree. An earlier guess blamed the viewport,
reasoning backwards from `Menu_hotspot_Y`. That was wrong, and measuring said
so: the runtime's layer rects are left=676 top=228 right=1700 bottom=996, which
is 1024x768 -- exactly what this engine already assumes.

The real evidence points elsewhere. `Loader_city_Height` reads `t661.Height`.
In the export, t661 is a plugin-14 object in the **Loading** layout, on layer
"test", sized 6953x304 at position (2073, -1058). The browser reports 304,
which is precisely that instance's height. This engine reports 0, and reports
512 and 288 for its X and Y -- neither of which is anywhere near (2073, -1058).

So the fault is in resolving object expressions for that instance, not in the
viewport. Height returning 0 while X and Y return non-zero values is the thread
to pull: they should all resolve against the same instance.

## Known-unmatchable

Session and user identifiers are randomly generated per run and can never
agree, so the realistic ceiling on this metric is 399/401, not 401/401.
