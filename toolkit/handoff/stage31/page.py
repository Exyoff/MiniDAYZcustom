"""Build the player-look comparison page, the GIFs embedded."""
import base64, os

HERE = os.path.dirname(os.path.abspath(__file__))

def img(name, alt):
    p = f'{HERE}/{name}.gif'
    if not os.path.exists(p):
        return '<div class="none">none</div>'
    b = base64.b64encode(open(p, 'rb').read()).decode()
    return f'<img src="data:image/gif;base64,{b}" alt="{alt}">'

DIRS = ['down', 'left', 'right', 'up']

def row(label, note, remake, mdz2):
    r = ''.join(f'<figure>{img(f"remake_{remake}_{d}", f"Remake {label} facing {d}") if remake else "<div class=none>no such animation</div>"}<figcaption>{d}</figcaption></figure>' for d in DIRS) if remake else '<p class="absent">The remake has no crouch.</p>'
    m = ''.join(f'<figure>{img(f"mdz2_{mdz2}_{d}", f"Mini DayZ 2 {label} facing {d}")}<figcaption>{d}{" (mirrored)" if d == "left" else ""}</figcaption></figure>' for d in DIRS)
    return f'''
<section class="anim">
  <header><h3>{label}</h3><p>{note}</p></header>
  <div class="pair">
    <div class="side remake"><h4>Remake now</h4><div class="strip">{r}</div></div>
    <div class="side mdz2"><h4>Mini DayZ 2</h4><div class="strip big">{m}</div></div>
  </div>
</section>'''

rows = [
    ('Standing', 'Remake: 1 frame. Mini DayZ 2: 2 frames, a slow breath.', 'idle', 'idle'),
    ('Running', 'Remake: 4 frames a step cycle. Mini DayZ 2: 6 frames.', 'run', 'run'),
    ('Aiming a rifle', 'One held frame in both.', 'aim', 'aim'),
    ('Melee swing (fire axe)', 'Remake: 3 frames. Mini DayZ 2: 6 frames, with the swing arc drawn in.', 'melee', 'melee'),
]
crouch = [
    ('Crouched, still', 'Mini DayZ 2 only: 2 frames, 24 px tall instead of 28.', None, 'crouch_idle'),
    ('Crouched, walking', 'Mini DayZ 2 only: 6 frames, at 60% speed.', None, 'crouch_walk'),
    ('Crouched, aiming', 'Mini DayZ 2 only: 1 frame.', None, 'crouch_aim'),
]

html = f'''<title>Two Survivors Compared</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Pixelify+Sans:wght@500;700&family=Source+Sans+3:wght@400;600&display=swap">
<style>
:root {{
  --ground: #E7EAE1; --panel: #F5F6F1; --ink: #1E241D; --muted: #596455; --line: #C9CFBF;
  --accent: #5F7330; --remake: #8A5A2B; --mdz2: #3F6A78;
}}
@media (prefers-color-scheme: dark) {{
  :root:not([data-theme="light"]) {{
    color-scheme: dark;
    --ground: #141913; --panel: #1C221A; --ink: #E3E8DC; --muted: #9AA592; --line: #2F382C;
    --accent: #A4BA66; --remake: #D69A5E; --mdz2: #7FB4C4;
  }}
}}
:root[data-theme="dark"] {{
  color-scheme: dark;
  --ground: #141913; --panel: #1C221A; --ink: #E3E8DC; --muted: #9AA592; --line: #2F382C;
  --accent: #A4BA66; --remake: #D69A5E; --mdz2: #7FB4C4;
}}
body {{ background: var(--ground); color: var(--ink); font: 16px/1.55 "Source Sans 3", "Segoe UI", system-ui, sans-serif; }}
.wrap {{ max-width: 1240px; margin: 0 auto; padding-inline: 20px; padding-block: 28px 56px; display: grid; gap: 28px; }}
h1, h2, h3, h4 {{ font-family: "Pixelify Sans", "Courier New", monospace; font-weight: 700; text-wrap: balance; margin: 0; }}
h1 {{ font-size: 34px; letter-spacing: .01em; }}
h2 {{ font-size: 22px; color: var(--accent); }}
h3 {{ font-size: 18px; }}
h4 {{ font-size: 14px; font-weight: 500; text-transform: uppercase; letter-spacing: .08em; }}
.lede {{ max-width: 68ch; margin: 8px 0 0; color: var(--muted); }}
.facts {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 16px; }}
.fact {{ background: var(--panel); border: 1px solid var(--line); padding: 14px 16px; display: grid; gap: 6px; }}
.fact h3 {{ font-size: 15px; }}
.fact.remake h3 {{ color: var(--remake); }} .fact.mdz2 h3 {{ color: var(--mdz2); }}
.fact ul {{ margin: 0; padding-left: 18px; color: var(--ink); }}
.anim {{ display: grid; gap: 10px; padding-top: 18px; border-top: 1px solid var(--line); }}
.anim header {{ display: flex; flex-wrap: wrap; align-items: baseline; gap: 4px 14px; }}
.anim header p {{ margin: 0; color: var(--muted); font-size: 15px; }}
.pair {{ display: grid; grid-template-columns: minmax(0, 2fr) minmax(0, 3fr); gap: 16px; align-items: end; }}
.side h4 {{ margin-bottom: 6px; }}
.side.remake h4 {{ color: var(--remake); }} .side.mdz2 h4 {{ color: var(--mdz2); }}
.strip {{ display: flex; flex-wrap: wrap; gap: 6px; align-items: flex-end; }}
figure {{ margin: 0; display: grid; justify-items: center; gap: 2px; }}
figure img {{ image-rendering: pixelated; display: block; }}
.strip:not(.big) img {{ width: 120px; height: auto; }}
.strip.big img {{ width: 192px; height: auto; }}
figcaption {{ font-size: 12px; color: var(--muted); letter-spacing: .04em; }}
.absent {{ margin: 0; padding: 18px 14px; color: var(--muted); border: 1px dashed var(--line); font-size: 15px; }}
.scale {{ font-size: 14px; color: var(--muted); margin: 0; }}
@media (max-width: 760px) {{
  .pair {{ grid-template-columns: 1fr; }}
  .strip:not(.big) img {{ width: 84px; }}
  .strip.big img {{ width: 134px; }}
}}
</style>
<div class="wrap">
  <header>
    <h1>Two survivors compared</h1>
    <p class="lede">The remake's player today beside Mini DayZ 2's survivor, dressed alike: hunter jacket and trousers, a cap, a hunter backpack, an AK, a fire axe for the swing. Every frame here is the games' own art, composited the way each game stacks its layers. Both are enlarged by the same amount, and each figure stands 28 pixels tall in its own game.</p>
  </header>

  <div class="facts">
    <div class="fact remake"><h3>Remake now</h3><ul>
      <li>4 directions, left drawn on its own</li>
      <li>Clothes in pieces: trousers, jacket, vest, backpack, hat, each its own item</li>
      <li>Idle 1 frame, run 4, swing 3, aim 1, use 4</li>
      <li>No crouch</li></ul></div>
    <div class="fact mdz2"><h3>Mini DayZ 2</h3><ul>
      <li>3 directions; left is right mirrored</li>
      <li>One whole outfit, plus headwear and a backpack</li>
      <li>Idle 2 frames, run 6, swing 6 with its arc, aim 1, use 4</li>
      <li>Crouch: still, walking and aiming, every direction</li></ul></div>
    <div class="fact"><h3>Choosing Mini DayZ 2 would mean</h3><ul>
      <li>Jackets and trousers become outfits: the remake's clothing list is reworked to Mini DayZ 2's</li>
      <li>Every held weapon uses Mini DayZ 2's weapon layers</li>
      <li>The crouch comes with all of it, dressed</li></ul></div>
  </div>

  <h2>Moving</h2>
  {''.join(row(*r) for r in rows)}

  <h2>Crouching</h2>
  {''.join(row(*r) for r in crouch)}

  <p class="scale">The Mini DayZ 2 survivor's pack is drawn behind the body when facing you, and over it when facing away. Mini DayZ 2 sets that order in its code, so here it is set by eye.</p>
</div>
'''
open(f'{HERE}/two_survivors.html', 'w', encoding='utf-8').write(html)
print(len(html))
