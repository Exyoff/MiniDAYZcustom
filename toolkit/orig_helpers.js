// Injected into every page orig.cjs opens, as window.ORIG. Everything is looked up lazily, so it
// works once the runtime exists (the Map layout of a run, the Menu...). The runtime is
// Closure-minified: rt.S = types_by_index, type.q = live instances, rt.wa = running layout,
// layout.ua = layers, rt.Yn(type, layer, x, y) = createInstance, rt.tD = global variables
// ({name, data}), inst.x/.y/.uid/.width/.height/.opacity/.visible keep their names, inst.u = angle
// (radians), inst.cc = instance variables (var#N = cc[N]), inst.P() = "bbox changed" after moving.
// NEVER assign a bare global in a js: step (`z = ...`): the runtime's own globals are 1-3 letter
// names and you will overwrite one (z is its clearArray) and break the game. Use ORIG.v.<name>.
(function () {
  const O = {};
  O.v = {};                                           // scratch space for js: steps
  O.rt = () => cr_getC2Runtime();
  O.tid = (n) => typeof n === 'number' ? n : (/^t?\d+$/.test(n) ? +n.replace('t', '') : (window.ORIG_NAMES || {})[n]);
  O.type = (n) => O.rt().S[O.tid(n)];
  O.insts = (n) => O.type(n).q;
  O.layout = () => O.rt().wa.name;
  O.layer = (name) => O.rt().wa.ua.find((l) => l.name === name);
  O.player = () => O.insts(181)[0];                   // player_collision_base (the player's position)
  O.create = (n, layer, x, y) => O.rt().Yn(O.type(n), typeof layer === 'string' ? O.layer(layer) : layer, x, y);
  O.call = (fname, ...params) => c2_callFunction(fname, params);   // run one of the game's own "Function"s
  O.global = (name, value) => { const v = O.rt().tD.find((g) => g.name === name); if (!v) return undefined; if (value !== undefined) v.data = value; return v.data; };
  O.globals = (re) => O.rt().tD.filter((g) => new RegExp(re || '.').test(g.name)).map((g) => g.name + '=' + JSON.stringify(g.data));
  O.move = (inst, x, y) => { inst.x = x; inst.y = y; inst.P(); return [x, y]; };
  // Teleport the player: the base (8-direction movement, t193) carries the pinned collision base and body
  O.teleport = (x, y) => { [193, 181].forEach((t) => O.insts(t).forEach((i) => O.move(i, x, y))); return [x, y]; };
  // Spawn an NPC the way the game does (Function "Spawn_NPC", events 13.1.1.N of Game_events):
  // kind 1 zed_normal, 3 zed_fast, 4 zed_shooter, 6 zed_screamer, 9 zed_army, 13 wolf, 14 deer, 15 rabbit
  O.spawnNPC = (kind, dx, dy) => {
    const p = O.player();
    const point = O.create('zed_resp_point', 'zed_resp', p.x + (dx === undefined ? 120 : dx), p.y + (dy || 0));
    O.call('Spawn_NPC', point.uid, 0, kind, 0);
    return [point.uid, Math.round(point.x), Math.round(point.y)];
  };
  O.where = (n) => O.insts(n).map((i) => [i.uid, Math.round(i.x), Math.round(i.y)]);
  window.ORIG = O;
})();
