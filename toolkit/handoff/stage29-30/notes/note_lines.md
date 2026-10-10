Merged under you: 29.1-29.10 (DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, 29.5 the melee kept as it is, ZOMBIE-AI, CAMERA at 1:1, HUD-TOP, TYPE with Arimo as the original's Arial and `widgets.outline` as its Outline effect, HUD-TOUCH, which already wires the switch's "I have no ranged weapon." through today's `combat.say`: keep the call, restyle the line) and 30.1-30.7 (the user's asks).

Progress Stage 29's question 3 (the original's "what happened" lines -- "I've eaten Canned beans.", "I found something in this trunk.") is unanswered; its default, written first, is **over the head only**: make the style and the survival and gun lines; the report lines after uses and searches stay out until the user answers, and nothing shows mid-screen while the board is open. While the pad or the perks are open, follow the original (the remake has the pad, 27.4).

Another track, 29.11 HITFX, is building at the same time over `combat.lua`'s hit on the player, `effects.lua` and the camera's shake: in `combat.lua` keep to `combat.say` and the gun lines' calls, so the two merge cleanly.

Number your Progress entry 29.12.
