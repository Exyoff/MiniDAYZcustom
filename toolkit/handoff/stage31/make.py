"""Side-by-side examples of the remake's player and Mini DayZ 2's survivor, as animated GIFs."""
import json, os
from PIL import Image

ROOT = 'A:/Documents/VS Code Projects'
NEW = ROOT + '/New Assets'
REM = ROOT + '/MiniOutbreak/Assets/Sprites'
DATA = ROOT + '/MiniOutbreak/OriginalData/mdz2/data'
OUT = os.path.dirname(os.path.abspath(__file__))
SCALE = 4
BG = (86, 104, 62, 255)          # a grass green, so outlines read as they do in game

# --- Mini DayZ 2 -----------------------------------------------------------
sprites = {}
for s in json.load(open(DATA + '/apk_sprites.json', encoding='utf-8')):
    sprites.setdefault(s['name'], []).append(s)
anims = {}
for e in json.load(open(DATA + '/animations.json', encoding='utf-8')):
    anims.setdefault(e['gameobject'].lower(), e)
sheets = {}

def sheet(tex):
    if tex not in sheets:
        sheets[tex] = Image.open(f'{NEW}/Texture2D/{tex}.png').convert('RGBA')
    return sheets[tex]

def sprite(name):
    """The frame's pixels and its pivot in them (from the top-left)."""
    s = sprites[name][0]
    im = sheet(s['texture'])
    x, y, w, h = [round(v) for v in s['rect']]
    top = im.height - (y + h)                     # Unity's rect counts from the bottom
    crop = im.crop((x, top, x + w, top + h))
    px, py = s['pivot']
    return crop, (px * w, (1 - py) * h)

def mdz2_frames(layer, action, direction):
    e = anims.get(layer.lower())
    if not e:
        return None, None
    for a in e['animations']:
        if a['action'] == action and a['direction'] in (direction, 'All'):
            return a['frames'], a['length_s']
    return None, None

def mdz2_anim(layers, action, direction, mirror=False, cell=(64, 64), foot=(32, 56)):
    """Each frame of `action` facing `direction`, the layers stacked on one pivot."""
    lists = [(l, *mdz2_frames(l, action, 'Horizontal' if direction in ('Left', 'Right') else direction)) for l in layers]
    lists = [(l, f, t) for l, f, t in lists if f]
    if not lists:
        return [], 0
    n = len(lists[0][1])
    length = lists[0][2]
    frames = []
    for i in range(n):
        canvas = Image.new('RGBA', cell, (0, 0, 0, 0))
        for _, f, _ in lists:
            img, (px, py) = sprite(f[i % len(f)])
            canvas.alpha_composite(img, (int(round(foot[0] - px)), int(round(foot[1] - py))))
        if mirror:
            canvas = canvas.transpose(Image.FLIP_LEFT_RIGHT)
        frames.append(canvas)
    return frames, length / n

# --- the remake ------------------------------------------------------------
def remake_anim(layers, pose, direction, n, per):
    frames = []
    for i in range(n):
        canvas = Image.new('RGBA', (30, 30), (0, 0, 0, 0))
        for l in layers:
            for name in (f'{pose}_{direction}_{i}.png', f'{pose}_{i}.png', f'h_{pose}_{direction}_{i}.png'):
                p = f'{REM}/{l}/{name}'
                if os.path.exists(p):
                    canvas.alpha_composite(Image.open(p).convert('RGBA'))
                    break
        frames.append(canvas)
    return frames, per

# --- output ----------------------------------------------------------------
def gif(frames, per, name, size):
    out = []
    for f in frames:
        bg = Image.new('RGBA', size, BG)
        bg.alpha_composite(f, ((size[0] - f.width) // 2, size[1] - f.height))
        out.append(bg.resize((size[0] * SCALE, size[1] * SCALE), Image.NEAREST).convert('RGB'))
    if not out:
        return None
    out[0].save(f'{OUT}/{name}.gif', save_all=True, append_images=out[1:], duration=int(per * 1000), loop=0)
    return name

made = []
# The remake: a T-shirt-and-jeans start is bare, so dress it as the Mini DayZ 2 survivor is dressed.
R = ['player_skin_def_1', 'hunter_pants', 'hunter_jacket', 'hunter_backpack', 'baseball_cap']
for d in ('down', 'left', 'right', 'up'):
    made.append(gif(*remake_anim(R + ['ak74_rifle'], 'idle', d, 1, 1.0), f'remake_idle_{d}', (40, 34)))
    made.append(gif(*remake_anim(R + ['ak74_rifle'], 'run', d, 4, 0.1), f'remake_run_{d}', (40, 34)))
    made.append(gif(*remake_anim(R + ['ak74_rifle'], 'pistol', d, 1, 1.0), f'remake_aim_{d}', (40, 34)))
    made.append(gif(*remake_anim(R + ['fireaxe'], 'axe', d, 3, 0.1), f'remake_melee_{d}', (40, 34)))
made.append(gif(*remake_anim(R, 'using_item', 'down', 4, 0.15), 'remake_use', (40, 34)))

# Mini DayZ 2: body under, then the outfit, the cap; the pack behind the body facing you and over it facing away.
BODY, OUTFIT, HEAD, PACK = 'human_Boris', 'Sprite_Item_Outfit_Hunter_Jacket', 'Sprite_Item_Head_Cap', 'Sprite_Item_Backpack_Hunter'
GUN, AXE = 'Sprite_Item_WpnR_akm', 'Sprite_Item_WpnM_Fireaxe'
def doll(direction, weapon):
    if direction == 'Up':
        return [weapon, BODY, OUTFIT, HEAD, PACK]
    return [PACK, BODY, OUTFIT, HEAD, weapon]
for d, mirror in (('Down', False), ('Left', True), ('Right', False), ('Up', False)):
    dd = 'Horizontal' if d in ('Left', 'Right') else d
    for action, weapon, tag in (('Idle', GUN, 'idle'), ('Run', GUN, 'run'), ('AttackRanged', GUN, 'aim'),
                                ('AttackMelee', AXE, 'melee'), ('StealthIdle', GUN, 'crouch_idle'),
                                ('StealthMove', GUN, 'crouch_walk'), ('StealthAttackRanged', GUN, 'crouch_aim')):
        frames, per = mdz2_anim(doll(dd, weapon), action, d if d != 'Left' else 'Right', mirror)
        made.append(gif(frames, per, f'mdz2_{tag}_{d.lower()}', (64, 64)))
frames, per = mdz2_anim([PACK, BODY, OUTFIT, HEAD], 'Interaction', 'Down')
made.append(gif(frames, per, 'mdz2_use', (64, 64)))

print([m for m in made if m])
print('missing:', [l for l in (BODY, OUTFIT, HEAD, PACK, GUN, AXE) if l.lower() not in anims])
