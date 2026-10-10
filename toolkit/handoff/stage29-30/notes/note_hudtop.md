Merged under you: 29.1-29.6 (the backlog's DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, the melee kept as it is, ZOMBIE-AI) and 30.1-30.7 (the user's asks). 30.2 HANDS ALREADY DID PART OF THIS BRIEF: the corner minimap is gone at every size, its code kept for the notebook's page, and the gear and the notepad stand at the right edge with the notepad **70** under the gear, as the running 1.0 measured, not GUI_layout's 75 (read Progress 30.2 and its note on 27.4.1). Keep 70; check the rest of what 30.2 did before you redo any of it.

Another track, 29.7 CAMERA, is building at the same time: the world at 1 px per CSS px and 2x in a room, through `src/core/viewport.lua`'s world scale (`world_view_height`, `world_scale`) and `src/core/camera.lua`. Add your GUI rule to `viewport.lua` as functions of their own beside `ui_scale`, and leave the world's scale and the camera alone, so the two merge cleanly.

Number your Progress entry 29.8.
