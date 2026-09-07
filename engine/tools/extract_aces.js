// Extracts every ACE implementation the runtime holds, keyed by object type
// and ACE index.
//
// Runtime condition/action objects keep `index` (the ACE index), `type` (the
// object type, whose `name` is the minified tN that data.js also uses) and one
// or more function-valued properties. So each ACE's actual implementation can
// be read straight off the live runtime and paired with the numeric index the
// decompiler prints -- much stronger evidence than inferring identity from a
// parameter signature.
//
// A condition/action object is recognised structurally: a numeric `index`, a
// `type`, and at least one own function property. Everything reachable from the
// sheets is walked, so nesting depth and sub-event layout do not matter.
//
//   node tools/extract_aces.js <game_dir> <out.json>

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2]);
const outFile = path.resolve(process.argv[3] || 'aces.json');
const MIME = { '.html':'text/html','.js':'application/javascript','.json':'application/json',
  '.png':'image/png','.jpg':'image/jpeg','.xml':'text/xml','.css':'text/css',
  '.m4a':'audio/mp4','.ogg':'audio/ogg','.wav':'audio/wav' };
const PATCH = 'var __C2F=f;f=function(a){var o=new __C2F(a);try{window.__c2_rt=o;}catch(e){}return o};' +
              'f.prototype=__C2F.prototype;window.cr_createRuntime=ac;';

(async () => {
  const server = http.createServer((req, res) => {
    const rel = decodeURIComponent(req.url.split('?')[0]).replace(/^\/+/, '') || 'index.html';
    const file = path.join(gameDir, rel);
    if (!file.startsWith(gameDir)) { res.writeHead(403).end(); return; }
    fs.readFile(file, (err, data) => {
      if (err) { res.writeHead(404).end(); return; }
      let body = data;
      if (rel === 'c2runtime.js')
        body = Buffer.from(data.toString('utf8')
          .replace(/window\.cr_createRuntime\s*=\s*\n?\s*ac;/, PATCH), 'utf8');
      res.writeHead(200, { 'Content-Type': MIME[path.extname(file).toLowerCase()] || 'application/octet-stream' });
      res.end(body);
    });
  });
  await new Promise(r => server.listen(0, '127.0.0.1', r));

  const browser = await chromium.launch({
    executablePath: process.env.PW_CHROME || '/opt/pw-browsers/chromium-1194/chrome-linux/chrome',
    args: ['--no-sandbox','--use-gl=angle','--use-angle=swiftshader','--enable-unsafe-swiftshader'],
  });
  const page = await browser.newPage({ viewport: { width: 1024, height: 768 } });
  page.on('dialog', async d => { await d.dismiss(); });
  await page.goto(`http://127.0.0.1:${server.address().port}/index.html`, { waitUntil: 'domcontentloaded' });
  await page.waitForTimeout(20000);

  const data = await page.evaluate(() => {
    const rt = window.__c2_rt;
    if (!rt) return { error: 'runtime not captured' };

    // Find the sheet collection, keyed by unminified sheet names.
    const seen = new Set();
    let sheets = null;
    const q = [[rt, 0]];
    while (q.length && !sheets) {
      const [o, d] = q.shift();
      if (!o || typeof o !== 'object' || d > 4 || seen.has(o)) continue;
      seen.add(o);
      let keys; try { keys = Object.keys(o); } catch (e) { continue; }
      if (keys.includes('Game_events') && keys.includes('Menu_Events')) { sheets = o; break; }
      for (const k of keys) {
        let v; try { v = o[k]; } catch (e) { continue; }
        if (v && typeof v === 'object') q.push([v, d + 1]);
      }
    }
    if (!sheets) return { error: 'sheets not found' };

    // Event blocks are recognised by their boolean `group` flag. Scoping the
    // scan to an event's own arrays is what keeps actions honest: a purely
    // structural "has index + type + a function" test also matches expression
    // nodes and other runtime objects, which inflated the action count roughly
    // fivefold on the first attempt.
    const isEvent = (o) => o && typeof o === 'object' && typeof o.group === 'boolean';
    const own = (o, k) => Object.prototype.hasOwnProperty.call(o, k);

    // Own-property checks matter here. Several other per-event arrays inherit
    // an `index` from their prototype, so a plain `typeof o.index === 'number'`
    // test also swallows them -- that is what tripled the action count before
    // (44297 actions + 65347 + 21191 = the 130835 originally reported).
    const isCondition = (o) => o && typeof o === 'object' && own(o, 'trigger') && own(o, 'index');
    const isAction = (o) => o && typeof o === 'object' && own(o, 'index') &&
                            own(o, 'type') && !own(o, 'trigger') && !isEvent(o);

    let events = 0, conditions = 0, actions = 0;

    // Join key is the SID (`Ba`), which data.js also records for every event,
    // condition and action. The runtime's own `index` is NOT the ACE index --
    // it is the ordinal within its event (range 0..227, heavily skewed to 0,
    // whereas data.js ACE indices run 38..468). Joining by SID is exact and
    // needs no assumption about traversal order matching.
    const bodies = new Map();          // body source -> short id
    const bodyId = (src) => {
      let id = bodies.get(src);
      if (id === undefined) { id = bodies.size; bodies.set(src, id); }
      return id;
    };
    const rows = [];
    const record = (o, isCond) => {
      let typeName = null;
      try { typeName = (o.type && typeof o.type.name === 'string') ? o.type.name : null; } catch (e) {}
      const fns = {};
      for (const k of Object.keys(o)) {
        let v; try { v = o[k]; } catch (e) { continue; }
        if (typeof v === 'function') { try { fns[k] = bodyId(String(v)); } catch (e) {} }
      }
      rows.push({ kind: isCond ? 'C' : 'A', sid: (typeof o.Ba === 'number' ? o.Ba : null),
                  typeName, ordinal: o.index, fns });
    };

    const visited = new Set();
    const walk = (o, depth) => {
      if (!o || typeof o !== 'object' || depth > 60 || visited.has(o)) return;
      visited.add(o);

      if (isEvent(o)) {
        events++;
        for (const k of Object.keys(o)) {
          if (k === 'sheet' || k === 'parent') continue;
          let arr; try { arr = o[k]; } catch (e) { continue; }
          if (!Array.isArray(arr) || !arr.length) continue;
          const m0 = arr[0];
          if (!m0 || typeof m0 !== 'object') continue;
          if (isCondition(m0)) {
            for (const c of arr) { conditions++; record(c, true); }
          } else if (isEvent(m0)) {
            for (const sub of arr) walk(sub, depth + 1);
          } else if (isAction(m0)) {
            for (const a of arr) { actions++; record(a, false); }
          }
        }
      }

      let keys; try { keys = Object.keys(o); } catch (e) { return; }
      for (const k of keys) {
        if (k === 'sheet' || k === 'parent' || k === 'type') continue;
        let v; try { v = o[k]; } catch (e) { continue; }
        if (v && typeof v === 'object') walk(v, depth + 1);
      }
    };
    for (const name of Object.keys(sheets)) walk(sheets[name], 0);

    const bodyList = new Array(bodies.size);
    for (const [src, id] of bodies) bodyList[id] = src;
    return { totals: { events, conditions, actions, distinctBodies: bodies.size },
             bodies: bodyList, rows };
  });

  if (data.error) { console.error(data.error); process.exit(1); }
  fs.writeFileSync(outFile, JSON.stringify(data, null, 1));
  console.log(JSON.stringify(data.totals, null, 1));
  console.log(`wrote ${outFile} (${(fs.statSync(outFile).size / 1048576).toFixed(1)} MB)`);

  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
