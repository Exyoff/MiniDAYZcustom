Merged under you: 29.1-29.8 (DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, the melee kept as it is, ZOMBIE-AI, CAMERA, HUD-TOP) and 30.1-30.7 (the user's asks). Read Progress 29.8 first: HUD-TOP made the GUI scale rule (`viewport.gui_scale`, `units = "gui"`) and, as a stopgap, moved the touch BAG, the board's bag and the board's notepad to the foot of the right column because the touch controls were still in design units -- your track puts the touch controls on the GUI rule in the original's px and the bag back at the original's bottom-left (0,620; phone 0,327).

THE USER'S ASKS OVER THIS BRIEF: 30.2 (2026-10-07) made USE show at **every container in reach, empty or not** ("Make the button always show up when interacting with a container"), and searching one a timed use under the bar; only a drop on the ground keeps 22.6's "only with something to take". Keep that, not the brief's "USE only with something in reach". 29.5: the melee is kept as it is ("leave the melee as is"): the attack button with the punch face swings melee, so it is shown with a melee weapon or fists too, not only with a gun.

Another track, 29.9 TYPE, is building at the same time: the original's typefaces, `widgets.font_at` and an outline helper. If you touch `widgets.lua`, keep to `widgets.button` and its pressed look, so the two merge cleanly.

Number your Progress entry 29.10.
