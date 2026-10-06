// jsfile example: statements are fine here; `return` what you want printed
const rt = cr_getC2Runtime();
const p = ORIG.insts('player_collision_base')[0];
return {
  layout: ORIG.layout(),
  player: p ? [Math.round(p.x), Math.round(p.y)] : null,
  time: [ORIG.global('Timer_Hours'), ORIG.global('Timer_Minutes')],
  difficulty: ORIG.global('Difficulty'),
  zombies: ORIG.insts('fam_zed_army_skin1') ? ORIG.insts('fam_zed_army_skin1').length : 0,
  instances: rt.S.reduce((n, t) => n + t.q.length, 0),
};
