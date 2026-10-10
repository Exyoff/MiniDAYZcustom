Merged under you: 29.1-29.7 (the backlog's DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, the melee kept as it is, ZOMBIE-AI, CAMERA: the world 1:1, the UI at its own scale) and 30.1-30.7 (the user's asks, the editor among them: `tools/editor/` draws with LÖVE's own font as a tool, and stays that way). Progress Stage 29's Q7 default is "MINIOUTBREAK in the original's lettering" for the title: if the menu's title is text, it is MENU-LOOK's, not yours.

Another track, 29.8 HUD-TOP, is building at the same time: the HUD's plates, its GUI scale rule in `src/core/viewport.lua` and `data/ui/hud.lua`'s numbers. Leave `hud.lua`'s layout and the viewport alone; the obvious callers you move to a face are text-drawing helpers (`widgets.font_at` and what calls it), not the HUD's rects. If a HUD line's face must change, change only its font argument, so the two merge cleanly.

The web build: build it into your own scratch dir and check the faces load in love.js (PUC Lua, WebGL1) and what they add to the download; say the size.

Number your Progress entry 29.9.
