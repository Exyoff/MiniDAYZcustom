"""Write track_common31.md: the builders' template for Stage 31, from track_common.md."""
S = 'C:/Users/wasle/AppData/Local/Temp/claude/a--Documents-VS-Code-Projects-MiniOutbreak/ff9ce3be-9e99-4cd3-be7e-4c03e873f394/scratchpad'
t = open(S + '/track_common.md', encoding='utf-8').read()

a = t.index("The user's standing instruction:")
b = t.index('## Your worktree')
t = t[:a] + '''THIS IS STAGE 31, THE USER'S ART CHANGE (2026-10-09/10): Mini DayZ 2's art and content brought into the remake. Read Progress.md's Stage 31 first: the user's words, the decisions table and the plan. What the user decided stands over everything else, including the earlier rule that the remake be identical to the official Mini DayZ 1.0: **the remake's player, its movement, animations, clothing (jacket, trousers, vest, headwear, backpack) and its weapons stay exactly as they are; no crouch.** Mini DayZ 2's art comes in for buildings, unworn items, objects, its new infected, bandits and animals, its UI buttons and panels, and its ground and road tiles; same-named art of those kinds is replaced by Mini DayZ 2's. Numbers: the remake's for what it has, Mini DayZ 2's scaled to sit beside ours for what is new. Every ask is also in `A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/comparison/user_asks.md` ("Stage 31").

''' + t[b:]

a = t.index('## The original')
b = t.index('## The remake')
t = t[:a] + '''## Mini DayZ 2, the source
- `A:/Documents/VS Code Projects/New Assets`: its Unity export (AssetStudio): `Sprite/<name>/` sprites sliced from `Texture2D/<sheet>.png` (trimmed, no pivots), `AudioClip/`, `Font/`, `TextAsset/` (`World.bytes`, `en_us.json`, `SoundDb.bytes`). Read only.
- `A:/Documents/VS Code Projects/packagedUnityAPK`: its Android build (Unity 2019.4.5f1, il2cpp), where the sprites' rects and pivots and the prefabs were read from. Read only.
- `OriginalData/mdz2/` in your worktree (a junction to the main checkout's, gitignored, never committed): the two surveys.
  - `catalogue/`: `catalogue.csv` (every sheet by kind), `match.csv` (each against the remake's art: identical / changed / new), `items.csv`, `slice_rects.csv` (each slice's rect in its sheet), `remake_only.csv`, composites and contact sheets, `_work/` scripts.
  - `data/`: `format.md` (World.bytes's format), `decode_world.py`, `world_named.json`, `kinds/*.json` (items, characters, containers, loot_collections, area_presets, decorations, sources, recipes, base_buildings, biomes, levels, status_effects, noise, footsteps, surfaces, ...), `names.csv` (id, English name, sprite folders), `apk_sprites.json` (all 28,392 sprites: rect from the bottom-left, pivot, ppu 32, texture), `apk_prefabs.json` (1,606 prefab trees: SpriteRenderers with sprite, pivot, sorting order, local transforms in Unity units; pixels = (32X, -(16Z + 27.7Y)) on the world's tilted ground), `buildings_layout.json` (231 enterable buildings, 84 distinct: Exterior, BackWalls, FloorAndBounds with the walls' colliders, Entrance with doors, the interior trigger), `presets_layout.json` (446 map pieces with enemy, item and container spawn points), `animations.json` (per sheet: action, direction, frames, length), `new_vs_existing.csv` (each item, character and building against the remake's), `texture_overlap.json`, `_work/` (the APK and metadata readers). Units: stats are fixed-point /10,000, times in ms, distances in Unity units x32 = px.
- The first game, the official Mini DayZ 1.0, and its toolkit (`A:/Documents/VS Code Projects/MiniDAYZcustom/toolkit/`) stay the reference for what the remake already does and Stage 31 does not change.

''' + t[b:]

t = t.replace("Progress.md's Stages 29 and 30 (the backlog's tracks and the user's asks, built so far) are not committed yet: read them in `A:/Documents/VS Code Projects/MiniOutbreak/Progress.md` (read only; never edit it), not in your worktree's copy.",
              "Progress.md's Stages 29, 30 and 31 are in your worktree's copy.")
open(S + '/track_common31.md', 'w', encoding='utf-8').write(t)
print(len(t), 'RUN NO SCENARIOS' in t, 'Mini DayZ 2, the source' in t)
