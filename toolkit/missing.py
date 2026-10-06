"""missing.txt: the ORIGINAL's object types whose art has no folder in the remake's Assets/Sprites
(compared by name: the sheet name of the object's frames, or its texture for tilemaps/tiled
backgrounds), grouped by what they are.

    python3 missing.py [remake checkout, default $REMAKE or ../../minioutbreak]

Each line: name, t-ids, frames, how often the event sheets mention it, and WHERE ELSE in the remake's
Assets the same art already is (so "missing from Sprites" is not read as "missing"):
  tilesets/ ui/   a loose png in Assets/Sprites/tilesets or ui
  icon            Assets/Icons/**/<name>.png or <id>_<name>.png (item pictures, not the object's frames)
  sheet           the packed sheet Assets/images/<name>-sheetN.png (cut.py-able; c2data.load_sheet_frames)
  NOWHERE         none of the above
"""
import os
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import c2orig as C  # noqa: E402

GROUPS = [  # first match wins
    ('logic / invisible helpers (spawners, markers, collision proxies, cameras, controllers)',
     r'(_base$|collision|_point$|_marker$|spawner|controller|waypoint|checker|_zone$|^script_zone|^z_spawner|^camera|'
     r'^mapgen|^map_element|^stepper|^spinning|^noice|^fade_sprite|_area$|^target_for|^td_zone|^alife|^test_|^tent_checker|'
     r'^obst|^build_ghost|^levelchanger|_levelchanger|^timers_sprite|^char_update_ping|^updater_icon|^walk_marker|^loaderrunner|_eyes$|^hammercrash_point|^helicrash_point|^polygon_target_base|^bot_respawn)'),
    ('achievements', r'^(ach_|achieve)'),
    ('GUI: HUD, buttons, touch controls, menus, panels',
     r'^(gui_|menu_|main_menu|btn_|pad_|options_menu|sub_menu|lang_selector|char_select|dead_screen|logo_|amenu_bg|'
     r'friend_hp|minimap|map_notes|panel_button|seed_btn|seed_icon|shield_icon|headshoticon|crosshair_|ammo_count_inv_bg|'
     r'pubg_.*_gui|red_zone_minimap|ninepatch_|bg_|tiledbg_loader|tiledbgmenu_|buddy_marker|dog_marker|marker_secret)'),
    ('zombies', r'^zed_'),
    ('animals', r'^(wolf_|deer_|rabbit_(eyes|dead|skin)|dog_(skin|dead)|crow_|fish_sprite|fish_nest|chicken|boar_|bear_skin)'),
    ('player / NPC bodies', r'^(player_|bot_|bandit_|pilot_)'),
    ('vehicles / aircraft', r'^(car_|heli|helicopter|airdrop|bridge_piece)'),
    ('buildings / world structures', r'^(b_|bunker_|bridge|helipad|stash|bed$|fueltank|wood_piles|mines$|quest_case|barbedwire|barberwire|'
                                      r'heli_crash|hammer_crash|gl_safezone|rad_zone|red_zone|safe_zone|exp_hole)'),
    ('ground / terrain textures', r'(tilemap|^snow_ground|^fake_water|^cloud_shadow|^dawnsunset|^night_overlay|^rain_|^light_)'),
    ('effects / particles', r'^(eff_|effect_|explolsion|muzzle|flame|molotov_(flame|visual)|blood_drop|headshot_particles|grenadeparticles|'
                            r'particles|smoke_grenade_visual|f1_grenade_visual|zed_bio|campfire_off|fire_place|flare_active|flare_gun_rocket|'
                            r'ee_pubg_drop_smoke|player_breathe|lg_grenade_active|f1_grenade_active|landmine_deployed|claymore_deployed|beartrap_deployed)'),
]
ITEMISH = 'items (weapons, ammo, attachments, food, medicine, tools, clothing: world/held sprites)'


def group_of(name):
    for label, rx in GROUPS:
        if re.search(rx, name):
            return label
    return ITEMISH


def main():
    remake = Path(sys.argv[1] if len(sys.argv) > 1 else os.environ.get('REMAKE', Path(__file__).resolve().parent.parent.parent / 'minioutbreak'))
    assets = remake / 'Assets'
    folders = {p.name for p in (assets / 'Sprites').iterdir() if p.is_dir()}
    loose = {}
    for sub in ('tilesets', 'ui'):
        for p in (assets / 'Sprites' / sub).glob('*.png'):
            loose[p.stem] = sub + '/'
    icons = set()
    for p in (assets / 'Icons').rglob('*.png'):
        icons.add(p.stem)
        icons.add(re.sub(r'^\d+_', '', p.stem))
    sheets = {re.sub(r'-sheet\d+$', '', p.stem) for p in (assets / 'images').glob('*.png')}

    P = C.project()
    base = C.base_names()
    events = (HERE / 'events' / 'ALL.txt').read_text() if (HERE / 'events' / 'ALL.txt').exists() else ''
    mention = Counter()                       # lines naming each object (fnx_pistol[t139] counts for fnx_pistol)
    for line in events.split('\n'):
        for w in set(re.findall(r'[A-Za-z_][\w]*', line)):
            mention[w] += 1
    names = C.type_names(P)
    by_name = defaultdict(list)
    for i, t in enumerate(P[C.TYPES]):
        if base[i] and not t[2]:
            by_name[base[i]].append(i)
    groups = defaultdict(list)
    nowhere = 0
    for nm, tids in sorted(by_name.items()):
        if nm in folders:
            continue
        frames = sum(len(an[7]) for i in tids[:1] for an in (P[C.TYPES][i][7] or []))
        where = []
        if nm in loose:
            where.append(loose[nm])
        if nm in icons:
            where.append('icon')
        if nm in sheets:
            where.append('sheet')
        if not where:
            where.append('NOWHERE')
            nowhere += 1
        mentions = mention[nm]
        groups[group_of(nm)].append('  %-34s %-22s %3d frames  %5d event mentions  in: %s' % (
            nm, ','.join('t%d' % i for i in tids), frames, mentions, ' '.join(where)))
    total = sum(len(v) for v in groups.values())
    out = ['# Object art of the ORIGINAL with no folder in %s/Assets/Sprites (by name)' % remake,
           '# %d names; %d of them have the art nowhere in Assets (no loose png, icon or packed sheet).' % (total, nowhere),
           '# Cut any of them with: python3 cut.py <name> <outdir>  (reads the ORIGINAL build\'s images/).',
           '# "event mentions" counts lines in events/ALL.txt that name it: 0 usually means a template/unused type.', '']
    order = [g[0] for g in GROUPS] + [ITEMISH]
    for label in sorted(groups, key=order.index):
        out.append('## %s (%d)' % (label, len(groups[label])))
        out += groups[label]
        out.append('')
    (HERE / 'missing.txt').write_text('\n'.join(out))
    print('missing.txt: %d names (%d nowhere in Assets)' % (total, nowhere), Counter({k: len(v) for k, v in groups.items()}))


if __name__ == '__main__':
    main()
