export const meta = {
  name: 'minioutbreak-vs-original-survey',
  description: 'Build a toolkit for reading and running the original Mini DayZ, then compare the remake to it area by area (screenshots and its event sheets), classify every difference against the user\'s asks, and synthesize a backlog',
  phases: [
    { title: 'Toolkit', detail: 'readable event-sheet dump, object index, sprite cutter' },
    { title: 'Survey', detail: 'one agent per area, original vs remake' },
    { title: 'Backlog', detail: 'dedup, verify the doubtful, order into fix tracks' },
  ],
}

const SP = '/tmp/claude-0/-home-user-MiniDAYZcustom/0ff86e18-6019-5d27-9ca6-6fb5467d12e7/scratchpad'

const CONTEXT = `
MiniOutbreak is a LÖVE 11.5 remake (Lua 5.1; web build via love.js) of the web game Mini DayZ (Bohemia Interactive, Construct 2). The user plays the remake's web build on a phone held sideways, and on PC. Their standing instruction (2026-10-03): "compare against official minidayz, use screenshots and source code to compare. Do not stop working until it is identical except for the changes i had asked for specifically."

THE ORIGINAL: /home/user/MiniDAYZcustom/MiniDayZ+1.0 is the official browser build (only its page title changed): data.js (the whole C2 project: object types, animations with frame rects, event sheets = all the game's logic, layouts, audio, globals), images/*.png (every sprite sheet), media/ (sounds), l_eng_*.xml (texts). MiniDayZ+1.2 and MiniDayZ+Reloaded are fan mods -- never the reference. /home/user/OriginalData/C2SourceData.js is that data.js with its BOM stripped (json.load works); the remake's tools/c2data.py reads it (object names come from frames' image paths). docs/original_data.md in the remake says what is where and what surprised us before.
RUN THE ORIGINAL: \`${SP}/orig/serve.sh\` (serves it on 127.0.0.1:8731 if not already), then \`node ${SP}/orig/orig.cjs --out DIR --res 1280x720 [--touch] --steps "wait:20000;shot:menu;click:640,353;wait:4000;shot:difficulty;..."\` -- read the header of orig.cjs for the steps (wait, shot, click, tap, down/up/move, key, hold/release, js). \`js:\` evaluates in the page: cr_getC2Runtime() is the C2 runtime (its types, instances, global variables -- read and set them: teleport the player, give items, change the time, spawn objects). Boot to the menu takes ~20 s. "New game" -> Novice/Regular/Veteran/Legend -> a run. Run several browsers at once only if you must; the machine has 4 cores shared with other agents.

THE REMAKE (as published, Stage 26): a read-only checkout at /home/user/wt/survey -- NEVER edit or commit there. Read Progress.md there (the stages, "Where this stands", "Ground rules", "Explicitly out of scope", "Reference numbers recovered from the original"), docs/, game/. Run it: \`source ${SP}/env/AGENT/env.sh\` then \`python3 ${SP}/env/AGENT/cap.py NAME FRAMES BEAT RES SCRIPTFILE [--touch off] [--world 7]\` (SCRIPTFILE: a Python expression -> a list of script steps, with tools/scenarios/common.py's START, GOD, KILL, lua(...), OPEN_BOARD... in scope, plus PHONE_START; console lua lines < 160 chars, no ';'). Writes ${SP}/env/AGENT/caps/NAME.png (the last frame) and prints the log's "= " lines. The remake's START opens its difficulty screen and a capture's START builds the hand-made test map; --world 7 a generated world.

THE USER'S ASKS: ${SP}/user_asks.md compiles every request the user made, in their words, and what each became. Each is a DELIBERATE difference from the original and stays. A difference with no ask behind it is a candidate to make identical to the original. Some differences are platform necessities (a phone's touch controls the original lays out differently, a keyboard on PC) -- call those "platform" and say what the original does on a phone.
`

const TOOLKIT = `${CONTEXT}
YOUR JOB: build the toolkit every other agent will use to read the original. Write into ${SP}/orig/ (never into any git checkout):
1. dump_events.py -> ${SP}/orig/events/<sheet>.txt for each of the 6 event sheets: every event block, nested, one line per condition and action, READABLE: object types by name (from their frames' image paths, as tools/c2data.py load_sprites resolves them; fall back to t-ids with the plugin name), the condition/action as plugin.method (e.g. Sprite.cnds.IsOverlapping, System.acts.SetVar), its parameters with literals, variable and global names, expressions printed as expressions as far as the export allows, groups and their names, 'else', 'or' blocks, loops (for each, repeat), triggers, comments if any. Keep the event number path (e.g. 12.3.1) on each line so others can cite it. Verify by reading a few known pieces against what docs/original_data.md says (the building/door spawn events; Zed_dmg; the autosave loop in "Everywere").
2. ${SP}/orig/objects.txt: one line per object type: t-id, readable name, plugin, behaviours, instance variables, animations (name, frame count, speed, loop), and its sheet files.
3. ${SP}/orig/globals.txt: every global variable with its initial value, and the layouts (project[5]) with their layers and the objects placed on each (names, counts).
4. cut.py: \`python3 ${SP}/orig/cut.py <object name> <outdir>\` cuts every animation frame of an object from its sheets into <outdir>/<animation>_<n>.png (and a frames.json with origins and image points), the layout the remake's Assets/Sprites rip uses (look at a few rip folders and match them).
5. missing.txt: the original's object types whose art is NOT in the remake's Assets/Sprites (compare by name with /home/user/wt/survey/Assets/Sprites), grouped by what they are (animals, GUI, buildings, items, effects...).
6. A one-page README.md in ${SP}/orig/ saying how to use all of it and orig.cjs, with three worked examples (read the zombie attack events; find what sound plays when the player is hit; spawn a wolf near the player in a run via js: and screenshot it).
Work fast but correctly; test each tool. Return a short summary of what you made and anything surprising you learned about the original.`

const AREAS = [
  { key: 'menus', title: 'Menus and screens', what: 'the main menu, New game and its difficulty screen (Novice/Regular/Veteran/Legend), Options, Achievements, Unlocks, the pause, the death screen, loading, the tutorial if any, every text (l_eng_ui.xml) -- layout, art, fonts, buttons, what each does; at 1280x720 and with --touch at 844x390.' },
  { key: 'hud', title: 'The HUD and controls', what: 'every bar, icon, number and button on screen in a run: health/food/water/heat (and armour, radiation), status icons, the clock or day count, the minimap and the map button, the touch controls on a phone (stick, buttons, their art and places), key/mouse controls on PC, the item pickup prompt, messages over the head and in a log, the aim/target display; at 1280x720 and --touch 844x390.' },
  { key: 'inventory', title: 'The inventory and crafting', what: 'the original inventory screen: its layout, slots, equipment, backpack/clothing capacity, containers and the ground, item tooltips and their actions (use, drop, equip, eat, drink, combine, craft, repair, attach), drag and drop, how timed actions show, crafting recipes (all of them in its events), what each item does when used.' },
  { key: 'character', title: 'The player character', what: 'sprites and animations (idle, walk, run, crouch, use, aim, shoot, reload, melee, hit, death), speeds (walk, run, crouch, while aiming, while hurt, while carrying), clothing and backpack layering and how each looks worn, held weapons and their positions, sizes and scale on screen, the camera (zoom, follow, lead), stamina if any.' },
  { key: 'zombies', title: 'Zombies', what: 'every zombie kind (sprites, size, health, speed, damage, armour), their AI (wander, idle, hearing, sight, spotting and alert, chase, attack range and rate, losing the player, doors and walls, groups), sounds (groans, spot, attack, hit, death), animations, death and corpses, spawning and population (where, how many, respawn), day/night differences, difficulty scalars.' },
  { key: 'combat', title: 'Combat', what: 'every weapon (melee and firearm): damage, rate, range, spread, recoil, reload time and how reload works, magazines and ammo types, durability and jamming, sounds; aiming and targeting (auto-target? how the target is chosen and switched), headshots, knockback, stagger/slow on hit, bleeding caused, blocking, noise drawing zombies, blood and hit effects, muzzle flash, shell casings, screen shake, kill feedback.' },
  { key: 'items', title: 'Items and loot', what: 'every item the original has (from data.js and l_eng_items.xml) against the remake\'s data/items.lua: stats, stacks, durability/condition, weights or sizes, icons; the loot tables per building and container type and their rates; vehicles\' loot; rare items; attachments and which weapon takes which; food and drink values; medical items and what they cure.' },
  { key: 'survival', title: 'Survival, time and weather', what: 'health, food, water, heat and their rates by difficulty (all four), regen and its conditions, bleeding, sickness/infection/cold and their cures, temperature and clothing warmth, day/night cycle length and how night looks (darkness, flashlight?), weather (rain? fog? snow?) and its effects, radiation and the red zone (red_zone_* art exists), anything that kills the player besides zombies.' },
  { key: 'world', title: 'The world', what: 'how the original builds its map (layouts, lots/tiles, procedural or fixed, size), its places (towns, cities, bases, farms, camps, castle, church, bunker, helipad, crash sites, red zone...), buildings and their facades/interiors (which are enterable; how entering looks: the facade hidden, the outside darkened?), props, trees, fences, vehicles, roads, ground tiles and seasons, water, the minimap and map screen, the camera\'s scale on screen, lighting and shadows.' },
  { key: 'audio', title: 'Audio', what: 'music (menu, in game, events), ambience (day, night, rain, wind, birds), UI sounds, footsteps, every sound event in the events (which sound, on what, at what volume, positional or not), compared to the remake\'s data/sounds.lua and what the remake plays.' },
  { key: 'events', title: 'Events, progression and everything else', what: 'world events (airdrops -- ee_pubg_drop_smoke exists --, helicopter crashes, the bunker -- bunker_json --, the red zone, traders, NPCs/bandits?), achievements and unlocks and what they unlock (characters? perks? starting kits?), the character select or skins, saving and continuing, score/stats shown at death, tutorials and hints, anything in the events no other area covers -- walk the event sheet groups list and name each group the remake has nothing for.' },
]

const FINDINGS_SCHEMA = {
  type: 'object',
  properties: {
    area: { type: 'string' },
    summary: { type: 'string' },
    findings: { type: 'array', items: { type: 'object', properties: {
      title: { type: 'string' },
      original: { type: 'string', description: 'what the original does, with evidence: screenshot paths, event numbers (sheet path), object/variable names, numbers' },
      remake: { type: 'string', description: 'what the remake does, with evidence: capture paths, files and functions, Progress items' },
      ask: { type: 'string', description: 'the user ask (quoted, from user_asks.md) that makes this difference deliberate -- or "none"' },
      verdict: { type: 'string', enum: ['make identical', 'keep (user asked)', 'platform', 'unclear'] },
      effort: { type: 'string', enum: ['S', 'M', 'L', 'XL'] },
      files: { type: 'array', items: { type: 'string' } },
    }, required: ['title', 'original', 'remake', 'ask', 'verdict', 'effort', 'files'] } },
    already_identical: { type: 'array', items: { type: 'string' } },
    not_checked: { type: 'array', items: { type: 'string' } },
  },
  required: ['area', 'summary', 'findings', 'already_identical', 'not_checked'],
}

const BACKLOG_SCHEMA = {
  type: 'object',
  properties: {
    summary: { type: 'string' },
    tracks: { type: 'array', items: { type: 'object', properties: {
      key: { type: 'string' }, title: { type: 'string' },
      findings: { type: 'array', items: { type: 'string' } },
      brief: { type: 'string', description: 'a build brief for one agent: what to make identical, the original\'s evidence, the remake\'s files, scenarios to write, what NOT to touch (user asks)' },
      files: { type: 'array', items: { type: 'string' } },
      effort: { type: 'string' }, order: { type: 'number' },
    }, required: ['key', 'title', 'findings', 'brief', 'files', 'effort', 'order'] } },
    kept: { type: 'array', items: { type: 'string' }, description: 'differences kept because the user asked, one line each with the ask' },
    questions_for_user: { type: 'array', items: { type: 'string' } },
  },
  required: ['summary', 'tracks', 'kept', 'questions_for_user'],
}

phase('Toolkit')
const tk = await agent(TOOLKIT.replace(/AGENT/g, 'tk'), { label: 'toolkit', phase: 'Toolkit' })

phase('Survey')
// One area at a time: the user asked for fewer agents at once (2026-10-05).
const results = []
for (let i = 0; i < AREAS.length; i++) { const a = AREAS[i]; results.push(await agent(`${CONTEXT.replace(/AGENT/g, 's' + (i + 1))}
THE TOOLKIT is ready in ${SP}/orig/ (README.md first): readable event sheets in ${SP}/orig/events/, objects.txt, globals.txt, cut.py, missing.txt. Its builder said: ${String(tk || '').slice(0, 3000)}

YOUR AREA: ${a.title} -- ${a.what}
Compare the original and the remake in this area, thoroughly: read the original's events and data for it (cite event numbers), run the original and screenshot every screen/state of it you can reach (save the shots under ${SP}/survey/${a.key}/orig/), capture the remake in the same states (${SP}/env/s${i + 1}/caps/), LOOK at both side by side, and read the remake's code and Progress.md for why it is as it is. List EVERY difference, small ones included (sizes, colours, positions, numbers, timings, sounds, texts), each with evidence on both sides and its verdict: "make identical" (no user ask covers it), "keep (user asked)" (quote the ask from ${SP}/user_asks.md), "platform" (a phone or PC necessity -- say what the original does there), "unclear". Note what is already identical, and what you could not check. Do NOT edit any code. Write your full report also to ${SP}/survey/${a.key}/report.md.`,
  { label: `survey:${a.key}`, phase: 'Survey', schema: FINDINGS_SCHEMA })) }

phase('Backlog')
const ok = results.filter(Boolean)
const backlog = await agent(`${CONTEXT.replace(/AGENT/g, 's12')}
YOU ARE THE PLANNER. ${ok.length} area surveys compared the remake to the original; their findings follow (full reports in ${SP}/survey/<area>/report.md). Dedup them; re-check any "unclear" verdict and any "make identical" that might collide with a user ask (read ${SP}/user_asks.md and Progress.md); then order the "make identical" work into FIX TRACKS for build agents: each track a coherent set of findings touching a coherent set of files (so tracks can run in parallel worktrees with few merge conflicts), with a brief an agent can build from (the original's evidence, the remake's files and functions, the scenarios to write, and what NOT to change because the user asked for it), effort, and order (first: what the player sees most and what is cheapest; the in-flight tracks rooms (27.1 interiors), map (27.2/27.3 a bigger world with more places joined by roads), mapscreen (27.4 the map screen and its notepad button) and fauna (28.1/28.2 the zombie's hit sound, exteriors hidden only inside, animals) already cover their areas -- do not duplicate them, but list what in their areas they will need to match). Also: the differences kept because the user asked (one line each with the ask), and the questions only the user can answer. Write it all to ${SP}/backlog.md as well.
THE FINDINGS:
${JSON.stringify(ok, null, 1).slice(0, 120000)}`, { label: 'backlog', phase: 'Backlog', schema: BACKLOG_SCHEMA })

return { toolkit: tk, surveys: ok.map(r => ({ area: r.area, n: r.findings.length, summary: r.summary })), backlog }
