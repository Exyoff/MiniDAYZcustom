Merged under you: Stages 29-30 as built (not tested), 31.1 IMPORT (read `docs/mdz2.md` first: the art, the sidecars, the lazy pages, the per-frame anchors, how a building's parts are named) and 31.2 HUNGER. Another track, 31.5 ITEMS, is building at the same time over `data/items.lua`, `item_art.lua` and the loot tables: leave those alone, and add your buildings' loot spots on tables that exist (30.3's).

THE USER IS EDITING `data/buildings.lua`, `data/colliders.lua`, `data/places.lua` and `data/trunks.lua` by hand in the main checkout (uncommitted, not in your base). So in `buildings.lua` and `colliders.lua` add the new buildings as new entries and change existing entries only where the drawing rule needs it, entry by entry, never reformatting or reordering a file -- the merge has to lay your entries beside the user's. Leave `places.lua` and `trunks.lua` alone (placing the new buildings in the world is 31.4's).

Number your Progress entry 31.3.
