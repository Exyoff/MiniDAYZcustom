Merged under you: 29.1-29.9 (DATA10, AUDIO, SURVIVAL, ZOMBIE-NUMBERS, 29.5 the melee kept as it is, ZOMBIE-AI with its bites on one beat, CAMERA with the camera pinned to the body: put the shake on the view, after the pin, HUD-TOP, TYPE) and 30.1-30.7 (the user's asks).

ALREADY DONE: **30.2 HANDS built the clothes' tear** (a landed hit wears one worn garment by `choose(2..7)` and round(random(7,13)), in `combat.lua` where the hit lands): read Progress 30.2, check it against 15.3.5.10, and do not build it again.

THE USER'S WORD: *"leave the melee as is"* (29.5): the swing's own mechanics -- its wedge hitting every body in it, its reach, its damage, its noise, its sound -- stay. The blood a hit leaves and the skull are a hit's effects, the same whatever landed it: make them the original's for a round and a swing alike. Stage 20's asks stay: the low-health grey, blood and a hit sound on every hit, drops that stay on the ground.

Another track, 29.10 HUD-TOUCH, is building at the same time over `data/ui/touch_controls.lua` and `widgets.button`; leave those alone. The flash greys the HUD too: do it in the draw (`render.begin_grey`/`end_grey` or a grey over the frame), not in the HUD's layout.

Number your Progress entry 29.11.
