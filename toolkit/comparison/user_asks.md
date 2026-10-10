# What the user asked for, in their words (from Progress.md), oldest stage first

Everything here is a DELIBERATE difference from the original Mini DayZ and must be KEPT.
Anything the remake does differently from the original that is NOT traceable to one of these asks
(or to the 'Asked for' tables below, or to a ground rule in Progress.md) is a candidate to make identical to the original.


## Stage 6 — Items and pickup

> **Inventory design is deliberately deferred.** This stage builds everything *around* the storage
> model and leaves the model itself swappable behind a five-function interface. The real design —
> grid, slots, weight, nesting, or something else — gets decided here, with a working game to try it against.
> *"Let's go with a simple slot inventory, like the original game. I want every item in the world
> to be permanent, and items that have pockets remember what was in them when they got dropped."*

## Stage 17 — A pause, the keys, and a board you can read

> *"Settings window that toggles touchscreen on/off, volume slider, return to main menu and
> resume*
>
> *Keybinds, E to pickup with the hand icon showing up above the players head, Tab for
> inventory backpack icon at the bottom with Tab written underneath it*
>
> *Fix the bleeding icon, starving icon and other icons to be at the end of the appropriate bar*
>
> *Inventory is too disorganized, design inventory to be easier to understand"*

## Stage 18 — Four bugs from a phone

> *"1. tapping or clicking the left side of the screen fires the weapon. Here's a suggested fix.
> Only the fire button should fire the weapon, and on pc, only space bar. If there is no
> target, the weapon will fire toward whatever direction the player is facing.*
> *2. melee has no cool down*
> *3. changing the screen size or orientation on mobile breaks the screen dimension*
> *4. add dead zone to settings to push buttons closer to middle of screen due to notches or
> punch out cameras on mobile devices. Add vertical and horizontal dead zone."*

## Stage 19 — Right-click, the swoosh, and two exploits

> *"publish to main first, remove right click as melee and make it so that using your melee
> displays a white swoosh sprite from the asset. It should go forward like a wave. Then fix
> the exploits."*

## Stage 20 — Thirty requests from a phone

> *"a few things, no melee while holding a weapon, weapon switch button with appropriate
> sprites. switch button goes from Melee, Pistol then Rifle and back to melee. One button to
> switch with each cycle showing a different sprite gui_btn_switch. Also for the attack / fire
> button, show weapon trigger sprite, gui_btn_attack, when holding a pistol or rifle; and a fist
> sprite if holding melee. the red circle sprite for target switching, gui_btn_car_shoot_sheet0,
> it should only show if there's an enemy on screen. Finally another button to focus on the
> locked target, should be in gui_btn_zoom-sheet0."*
>
> *"double check the sprites for weapons and gear. When there's no target, the weapon is
> lowered; when there's a target, the weapon is put up and aimed. use
> gui_wpn_cooldown_bg-sheet0.png for any interaction, such as eating etc. Performing an
> interaction should play the player crouched down animation. using an item inside the
> inventory shouldn't close the inventory; just shade it dark and not allow the player to
> perform another interaction until they are done with their current interaction. Grabbing
> something from the ground shouldn't open the inventory. Remove inventory hints, it's
> unnecessary. remove ammo counter, instead, have a text say above the player "It's empty", and
> if there's no ammo "I don't have ammo for this". Don't auto reload when firing an empty
> weapon. There's 3 sprites that look like lines, red yellow and green. Use them like a cone to
> display dispersion. Reduce dispersion for pistols. Fix "on ground" section of inventory, it's
> too far right. I don't like how items hover slightly above where they're held, feels laggy
> and clumsy. Just make it so that they're where the touch input began, same with dripping
> items. Clicking empty gear slots shouldn't prompt anything, just items. Also remove the
> "can't wear that" text or any type of hint, just the red tint is enough. Game stutters on
> mobile browser. grabbing item from ground or container should auto equip if slot is empty.
> Pistols, Rifles, Melee should only go on backpack slots, and they should be 1x2 for pistols,
> 1x3 for rifles and 1x2 for melee. Clothing shouldn't go on other clothing. Also, don't have
> every agent run a full test, just make them all do their own work, then you run a test for
> everything all at once."*
>
> *"few more issues, sprites don't match their icons. seems like some animation mismatch
> happens when standing still, the weapon shows up on their back even tho it should be in
> their hand. when reloading a weapon, lock the ammo so it can't be moved. add a reloading
> icon over it. whenever a hit connects to a zombie, add blood effects and a hit sound effect.
> same with when a player takes damage. Make screen gradually gray when health is low. 50%
> monkchrome when below 10% health. effect doesn't start until under 75% health. when
> bleeding, add blood drops that stay on the ground."*

## Stage 21 — Subjects, a wider well, and a world of roads

> *"for items that take more than 1 slot, have the slot they're occupying just be 1 wide slot so
> the background is cohesive. when using an item, don't freeze the inventory, just the item that
> is being used. Remove the carrying x out of x items. Don't run the full suite. Also separate
> the tests into different subjects so that the entire suite isn't ran when making changes
> unrelated to other tests. After it's complete, begin working on randomly generated maps. start
> with roads, then interest points, then buildings. Interest points range from basic towns to
> cities to military bases."*

## Stage 22 — Tabs, one bag, and a fight you can read

> *"the aiming lines should only appear when there's an actual target. reload button should
> only appear when holding a weapon. durability should show up even when 100%, ammo count for
> items in inventory should also show up. The pick up in the inventory is too messy, just
> include tabs on the left hand side with the icons of the container the player is picking up.
> On ground always being first and anything else below it based on range. Tapping the icons
> opens the containers. Moving weapons in the backpack is a little difficult, when trying to
> put weapons side by side on the same row, hovering over the slot in front of another weapon
> attempts to replace it. Instead of the name of the weapon on the bottom left, include the
> sprite of the weapon, no name, no ammo, no durability. Less range on melee. Shooting while
> running does not play shooting animation. Remove close inventory button and instead just
> leave the open inventory button always on, so tapping it opens and closes the inventory.
> Move scroll bar from side to bottom of left column, horizontal buttons, same as scroll
> buttons but left and right, back and forward. No page number, just darken the back button
> when in the first page, darken the forward button when at the last page, darken both when
> there's only 1 page. Zombies should randomly make noises when wandering, hitting a zombie
> should slow it down for a second. Also work on the start menu; Continue, Start, Settings.
> Slow red background, a little faster scrolling background of sky scrapper."*
>
> *"oh also the pick up button should only appear when there's an item to pick up, and get rid
> of the pick up icon above the player"*

## Stage 23 — Walls, doors, trunks, attachments and what ails you

> *"for no room, have the player say "My inventory is full" no need to hint a pc player as
> there will be a tutorial later. First commit changes. Then work on: Zombies phasing through
> walls. doors not where their top sprite has them. Door button with door sprite when near a
> door. Lootable vehicle trunks. appropriate container scales; boxes, wardrobe etc can just be
> copied from interior sprites. let me know if you'd like me to create the art. weapon
> attachments, not all attachments go on every gun. effects, like infection, which needs to be
> cured by tetracycline. Speed boost from energy drinks. cold sickness from staying out too
> cold and curable by keeping food water and heat above 75% for some time."*

## Stage 24 — Fire, and somewhere warm

> *"publish to main. Heat is acquired by being near a campfire or indoors. To build a campfire
> the player needs 1 wood, 1 stick. That makes a campfire kit. Then with 1 stick and 1
> newspaper, the player can make a campfire starter kit. Acting the campfire starter near the
> campfire turns it on. To place the campfire, there needs to be a build mode."*

## Stage 25 — A world of its own on START, and a use you can watch

> *"is the randomly generated world built?"* -- answered: built in Stage 21, reached only
> through the console, START still on the test map (21.5.8).
>
> *"just finish generated worlds and make it work when clicking start, also place the
> difficulty after the start button"*
>
> *"also the crouch down animation doesn't seem to be working. Closing the inventory stops
> the player from eating or using an item, i want the character to be crouched down with a
> bar above his head, matching the one that's under an item. Also, center the items on
> their slot."*

## Stage 26 — Rooms seen from the street, a screen for the difficulty, a beach, the cold east, and a phone's frames

> *"interior of buildings should always be visible, with entering the building hiding the
> outside. limit the infected status to only hard mode. Clicking play should open a new screen
> and in there the difficulty should be. opening the game on mobile browser while in landscape
> causes the top to be pushed down. Dropping items inside of buildings does not show. Reduce
> the range for opening containers and increase the size of the containers tabs. lighting a
> campfire should only be done by standing near the campfire and using the campfire lighter
> kit from the inventory so it doesn't need a button. On the left side of the map, there should
> be a beach are. Players spawns on the beach. Traveling right slowly gets colder and
> eventually the tiles change, art is in sprites. Also fix the name of the sprites folder.
> There's also a lot of choppiness in the browser, but only on mobile, can you investigate?"*


# 'Asked for -> What it became' tables (Where this stands)

### Latest: Stage 26
| Interiors always visible; entering hides the outside | Every building with a room shows its cutaway from the street -- floor, furniture, whoever is in it; step in and everything outside its walls goes black, eased, and nothing out there is drawn or aimed at. `interiors.always_cutaway = false` puts the house fronts back | 26.1 |
| Dropped items inside buildings do not show | The floor was drawn over them; it is drawn with the ground now, and a drop never lands inside a wall | 26.1.6 |
| START opens a new screen with the difficulty | NOVICE, REGULAR and VETERAN as big cards with their numbers; a tap starts the run and is remembered; BACK, Escape and a phone's back key return; a double tap on START starts nothing | 26.2.1, 26.2.2 |
| Infection on hard mode only | A bite infects on VETERAN alone; a save carrying one onto another difficulty loads clean | 26.2.3 |
| Containers opened from closer; bigger tabs | A piece or a trunk opens from a step off its footprint (16 px), never through a wall; NEARBY's tabs 50x62, six of them | 26.2.4, 26.2.5 |
| A campfire lit only from the kit in the inventory, no button | The starter kit's LIGHT near a cold fire; E and USE do nothing to a fire; the button is gone; "No campfire nearby" otherwise | 26.2.6 |
| Fix the sprites folder's name | `Assets/Sprites`, with every tool that reads it; the build output byte-identical | 26.3.1 |
| A beach on the left; players spawn on it | The sea down the west edge, solid to feet, and a sand beach in the rip's own pieces; the spawn on the sand facing inland, the makings of a fire at the beach's edge and a farmhouse up the road | 26.3 |
| Colder going right, and the tiles change | Heat drains by how far east, from nothing on the beach to a full bar in eight minutes by day in the snow, unclothed; the ground turns green, brown, frosted, dry, snow in ragged bands, the roads with it; "It's getting colder" | 26.4 |
| The top pushed down in landscape on a phone | Not reproduced in emulation; the page is pinned to the visible viewport, goes fullscreen on the first touch and can be added to a home screen, fullscreen and sideways -- **a screenshot and the browser's name are wanted** | 26.5.1-26.5.3 |
| Choppiness in the browser, only on mobile | Measured: physics moved the world in whole sixtieths, so a phone's uneven or 90/120 Hz frames moved it by none or two; now every frame moves it by its own time. And a shot could crash the published build on a phone | 26.5.4-26.5.7 |

#### What Stage 25 was
| Finish generated worlds; make them work when clicking START | START builds a new world from a fresh seed behind a "Generating world" frame; the seed is saved, and CONTINUE rebuilds the same world before the save applies (a fingerprint refuses one a later generator would change); RESTART a new world at the ended run's difficulty; an older save loads the test map | 25.1 |
| The difficulty after the start button | NOVICE / REGULAR / VETERAN chips directly under START, SETTINGS below; the settings window back as it was | 25.1 |
| Generated worlds, finished | Measured in the web build at a phone's pace: 34-41 ms of Lua a frame down to 14-17, generation about halved; the spawn never in or by a city over 100 seeds; the church placed; sticks, logs and newspapers near every spawn | 25.2 |
| The crouch "doesn't seem to be working" | It played, under the board that hides the player; now a use goes on behind the shut board, so it is seen -- and bags are drawn in it | 25.3 |
| Keep eating or using after shutting the board, crouched with the item's bar over the head | Every timed use runs to its end behind the shut board; the bar over the head is the board's bar, drawn by the same function; a step or an attack cancels with nothing spent | 25.3 |
| Centre the items on their slots | Every icon in the middle of its well, labels outlined in the corners | 25.3 |

#### What Stage 24 was
| 1 wood + 1 stick make a campfire kit; 1 stick + 1 newspaper a starter kit | Two recipes, made by dragging one ingredient onto the other or with MAKE on its tooltip, a timed use with the crafting sound; the ingredients found in houses, kitchens, gas stations and car trunks, and under the test map's trees | 24.1 |
| A build mode to place the campfire | The kit's BUILD shows a ghost ahead of you, green where it may stand and red on walls, furniture, bodies, indoors or in a doorway; the original's tick places it and the cross cancels | 24.2 |
| The starter kit near the campfire lights it | LIGHT on the kit, or E / the fire's USE with one carried; the fire burns ten minutes with its own crackle and glows at night, then can be lit again; saved with the run | 24.2 |
| Heat near a campfire or indoors | By a lit fire heat climbs 5 a tick (20 to 75 in under a minute); in a room 1.5 a tick up to 80%; walls and shut doors keep a fire's warmth in; a red arrow on the heat bar while you warm | 24.3 |

#### What Stage 23 was
| For no room, "My inventory is full"; no hint for PC | Said over the head by E or USE; USE shows there | 22.6.3 |
| Zombies phasing through walls | One world: a room's walls are solid to everyone, as in the original, and a building with no room stands on its footprint; no zombie is made inside a wall | 23.1 |
| Doors where their sprite has them | Every leaf, doorway gap and door point moved to where the original's own data and the facade's art put it; a lint holds all three together | 23.1 |
| A door button with the door's sprite near a door | Doors open and shut, shut ones stop players, zombies, rounds and sight; a button with that door's own leaf, and F (or E with nothing else to do) on a keyboard | 23.2 |
| Containers at the right scale, copied from the interior sprites | Each searchable piece cut out of a room that paints it, at the room's scale; open frames are stand-ins (art wanted) | 23.3 |
| Lootable vehicle trunks | Every parked car has a trunk at its back with its own loot; USE wears the original's car-with-its-lid-up face there | 23.4 |
| Weapon attachments, not every one on every gun | Eleven attachments on the original's four mounts, by gun family; scopes, silencers, grips, a choke, each changing the numbers the trigger reads; drag on, tap off | 23.5, 23.6 |
| Infection, cured by tetracycline | A bite can infect; it drains and stops regen; only tetracycline cures it | 23.7 |
| An energy drink's speed | A minute at x1.25, refreshed not stacked | 23.8 |
| Cold sickness from staying cold, cured above 75% for a while | Caught after 60 s under 20% heat; passes after 90 s with food, water and heat at 75% or more | 23.8 |

#### What Stage 22 was
| Aim lines only with an actual target | The cone asks the target the reticle and the pose ask, at the draw, and never follows the facing fallback | 22.1.1 |
| Less range on melee | Reach 34 to 22 in the one place it lives: a swing lands at contact and half a body past a bite | 22.1.2 |
| The shooting animation while running | The rip has no running-and-shooting frames, so a shot on the move draws the raised pose above the waist over the running legs; the pack, vest and hat ride whole | 22.1.3 |
| Zombies groan now and then while wandering | Idle zombies near you, each on its own 6–14 s wait, never two within 2.5 s, through the positional audio | 22.1.4 |
| A hit slows a zombie for a second | Half speed for 1.0 s, a new hit restarts it, the walk cycle slowed to match | 22.1.5 |
| RELOAD only when holding a weapon | Shown only with a pistol or a rifle in hand | 22.2.1 |
| The weapon's picture bottom left, nothing else | The board's own picture of what is in hand, left-aligned; nothing with fists | 22.2.2 |
| Start menu: Continue, Start, Settings; a slow red background and a faster skyline | Three buttons in one column; the original's red sky at 8 units/s and a repeating skyline at 16 — its own speeds; difficulty is a setting | 22.3 |
| NEARBY as tabs: the ground first, then by range; a tap opens | A tab per source down the left, the list shows one source; tapping a container's tab opens it | 22.4.1–22.4.3 |
| The pager at the column's foot, darkened, no page number | Two turned scroll arrows where CLOSE was; a dark one does nothing | 22.4.4 |
| No CLOSE; the bag opens and shuts | One bag in one place over the board and off it; the board slides clear of it at 16:9 | 22.4.6 |
| Durability at 100%; ammo counts in the inventory | Every well and row shows condition, 100% included, and a gun its rounds | 22.5 |
| Weapons side by side without the neighbour being taken | A weapon lands where its ghost is; it trades places only with the thing under the finger | 22.5 |
| USE only with something to pick up; no icon over the player | USE shows for a drop in reach, or a container with something in it; the hand and its E badge are gone | 22.6 |
| For no room, the player says "My inventory is full"; no hint for a PC player, a tutorial will come | E or USE over a drop there is no room for says it over the head, and USE shows there again | 22.6.3 |

#### What Stage 21 was
| Separate the tests into subjects; don't run the full suite | Fourteen subjects in `tools/scenarios/`, one table mapping every file to what it can break, and a proof — not a run — that all 188 scenarios moved unchanged | 21.1 |
| A weapon's slots as one wide slot | One well drawn across the span, on whole pixels, one target to press and drop on | 21.2.1 |
| A use freezes only its item | The board-wide dim is gone; the thing used is locked and dimmed under its bar, and a second timed use waits | 21.2.2, 21.2.3 |
| No "carrying x out of x" | Gone; each card keeps its own count | 21.2.4 |
| Random maps: roads | A seeded network in the rip's own 60 px terrain pieces — highways, main roads, branches, dirt tracks, straight and meeting at right angles because that is what the art has — drawn in two draw calls, on the minimap in the original's symbols | 21.3 |
| Interest points: towns, cities, military bases | Placed on the roads from the seed: cities with their own street grid, a walled base with one gated road, towns along lanes, a roadside gas station and cafe; the hospital and fire station inside cities | 21.4 |
| Buildings | Every place filled with its kind's buildings, each fronting a road — the rip draws every facade facing south — with its loot, its zombies, and one farmhouse per town you can enter; the spawn on a road short of a town | 21.5 |


## Stage 27 (2026-09-28), not yet built when this list was made
> "Some building interiors are still missing, Increase the height, make the map grid uniform. More interest points with roads connecting them. make the biomes wider, like 100 across, just for testing."
  (answers: the height = the generated world's; the uniform grid = roads connect every interest point to each other; 100 = 100 ground tiles, 6000 px, per biome)
> "also make the map into an openable menu, clicking a button opens and closed the map"

## Stage 28 (2026-10-03)
> "the zombie attacking sound should just sound like a hit sound. The current sound is for wolfs. Speaking of which, add animals, wolf chasing player or deer makes the current zombie attack noise. As for the map button, use the notepad looking button. And some bugs: Building exteriors get hidden when rotating device, i think. Either way, the current way it works is incorrect. Make it hidden only when the player is inside"
> "continue, compare against official minidayz, use screenshots and source code to compare. Do not stop working until it is identical except for the changes i had asked for specifically"

## Stage 30 (2026-10-07)
> "Remove minimap. Getting hit should damage clothing items. items with better stats should spawn only on harder areas, pistols in house, simpler guns in other houses. AR in military bases and what not. clothing with better slots and armor spawn in houses and other items in military bases. military bases spawn later in the world. Make the button always show up when interacting with a container, and when looting a container, have the interaction bar show up. The more loot a container has, the more items in a container, the longer it takes.
> Make a new program modifying areas, building hitboxes and loot spawns. container spawns and also sprites for houses, vehicles, mobs and containers.
> Buildings should display their interiors even when not near it. Doors should show their open sprite when a door is open instead of darkening the sprite"
  (answers, asked: the split is bases = armour, gear and assault rifles, houses = everyday clothing, bags and pistols, other houses simpler guns; "harder" = further east, quality rising from the beach to the snow, military bases only in the later seasons; the program = a desktop LÖVE app beside the game)
> "right now, building that don't have doors show grass, and walking inside changes to an interior sprite. Buildings that have windows also have the same issues. So a solution for every building. Just display the interior always with the outside sprite layered on top. Also, zombies should also spawn in accordance with their surroundings. Military zombies only spawn in bases and civilian zombies in civilian areas. Another thing, the editor app should allow for tile editing and zombie spawn type editing as well. When editing interest points or areas . So main editor functions, Edit interest points, edit buildings, edit objects, edit mobs, edit loot table"
> "leave the melee as is but change the way the animation plays, i dont want the stabbing animation while the player is running. I'll allow the multi target when meleeing. Then continue"
  (said of the backlog's MELEE track, which was to make the swing the original's: one body a swing, crits, damage numbers, a ruined weapon's 15, Punch_Swipes on every swing. Read as: none of it; the remake's swing stays, the wedge hitting every body in it; only the pose changes: on the run the body keeps running, built in 29.5)

## Stage 30, 2026-10-09
> "remove the black covering the outside when the player is inside a building, also when the player goes behind a building, dont show the inside of the building, just the outside but transparent. Also sort sprites to show on top of the player if the player goes behind something, like a car or a building. The world has a slight tilt so its not completely top down"
  (overturns 26.1's dark outside a room and 30.1's half-dark room under a faded front; 30.8)

## Stage 31 -- the art change (2026-10-09, 2026-10-10)
> "Lets pivot, document what you were working on and pause it. Lets start on an art change, look at A:\Documents\VS Code Projects\New Assets. Textures are sorted into sepperate parts, single textures for buttons or UI, Buildings have a floor sprite, which is they foundation size, and a wall texture which the player does not go on top of, and if running behind the building, it does not get shown. Theres also new items, new buildings, vehicles. objects, zombies, animals etc. Add all those to the current project, and replace existing textures if they already exist. Also theres a new player mechanic, crouching, with apropriate textures to follow"
  (New Assets is Mini DayZ 2's export; its data and placement were decoded from it and from its Android build, kept in the remake's gitignored `OriginalData/mdz2/`.)

Answers of 2026-10-10:
- **Numbers**: the remake's numbers stay for what it already has; new items, zombies and animals take Mini DayZ 2's, scaled to sit beside ours.
- **Mini DayZ 2's systems that come in**: its bandits, its one hunger meter and morale (in place of food and water), its base building and its crafting recipes. *"We will have to rethink the gameplay loop for base building. I want death to feel impactful but i also want loot to be collectable. So maybe, a starter island. Starter island being 20x20 or something and only the easy biome, player is able to respawn at starter island. Then after fixing key locations in the island, like a radio station or something, unlocks the ability to locate other islands. Then traveling all the way to the right and fixing the boat on shore allows the player to travel to a new island. traveling back to the left shore in the new island sends player back to starter island or traveling to right shore sends player to starter island as well. Dying respawns player at shore of starter island. Zombies at starter island don't respawn. Just a plan for now."* -- a plan, not built yet; it answers Progress Stage 29's question 6 (the islands) in its own way.
- **The new buildings**: mixed into today's kinds of place and new kinds of place, *"Depends on island difficulty"*.
- **The player's look**: the user asked to see examples of the two first.
- **The player's look, after seeing both** (2026-10-10): *"let's keep the character movement and clothing the same, only use new buildings, non equitable items and objects"*. The remake's player, its animations, its clothing (jacket, trousers, vest, headwear, backpack) and its weapons stay. Mini DayZ 2's art comes in for: buildings, items that are not worn or held (food, drink, medicine, materials, ammunition, tools), objects (props, decorations, trees, fences, vehicles and wrecks, containers), its new infected, bandits and animals (boar, bear, chicken, rooster), its UI buttons and panels, and its ground and road tiles. Same-named art in those kinds is replaced by Mini DayZ 2's. **The crouch is left out** (no frames for the remake's player).
- **2026-10-10**: *"slow the zombie animations down"* -- every infected's and bandit's frames at 0.75 of their sheet's pace (an F4 slider), Mini DayZ 2's 24 fps walk held to the original's 0.8 s step (31.7.15).
- **2026-10-10, after the UI preview**: *"Keep new in game buttons, keep old inventory, keep old map, keep new pause and paper buttons. Revert thirst bar, bleeding icon and armor icon"*; asked which "thirst bar", answered **"Bring thirst back"**: the separate water meter returns beside food (morale kept), undoing the single hunger meter's merge of food and water (31.2). The UI keeps Mini DayZ 2's in-game buttons, pause menu and paper buttons; the inventory board and the map screen keep the remake's art; the bleeding and armour marks are the remake's (31.9).
