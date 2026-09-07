// Extracts expression implementations from the live runtime.
//
// Expressions are a third index space alongside conditions and actions, and
// unlike those they carry no SID to join on. Instead each ACE's parameter
// expression trees are emitted in a fixed traversal order; the matching walk
// over data.js pairs them up positionally. Because every node also reports its
// `type` -- the same opcode namespace data.js uses -- the pairing validates
// itself: if the two walks ever disagree on a node type, the correlation is
// wrong and the extractor says so rather than emitting plausible nonsense.
//
//   node tools/extract_exprs.js <game_dir> <out.json>

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2]);
const outFile = path.resolve(process.argv[3] || 'exprs.json');
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
    const seen = new Set();
    let sheets = null;
    const q = [[rt, 0]];
    while (q.length && !sheets) {
      const [o, d] = q.shift();
      if (!o || typeof o !== 'object' || d > 4 || seen.has(o)) continue;
      seen.add(o);
      let ks; try { ks = Object.keys(o); } catch (e) { continue; }
      if (ks.includes('Game_events') && ks.includes('Menu_Events')) { sheets = o; break; }
      for (const k of ks) { let v; try { v = o[k]; } catch (e) { continue; } if (v && typeof v === 'object') q.push([v, d + 1]); }
    }
    if (!sheets) return { error: 'sheets not found' };

    const own = (o, k) => Object.prototype.hasOwnProperty.call(o, k);
    const isEvent = (o) => o && typeof o === 'object' && typeof o.group === 'boolean';
    const isCondition = (o) => o && typeof o === 'object' && own(o, 'trigger') && own(o, 'index');
    const isAction = (o) => o && typeof o === 'object' && own(o, 'index') && own(o, 'type') &&
                            !own(o, 'trigger') && !isEvent(o);

    const bodies = new Map();
    const bodyId = (src) => {
      let id = bodies.get(src);
      if (id === undefined) { id = bodies.size; bodies.set(src, id); }
      return id;
    };

    // Depth-first: the node, then first, then second, then its parameter
    // lists. The data.js side walks its argument list in the same order.
    const emit = (node, out) => {
      if (!node || typeof node !== 'object') return;
      let fnId = null;
      try { if (typeof node.Ac === 'function') fnId = bodyId(String(node.Ac)); } catch (e) {}
      out.push({ type: (typeof node.type === 'number' ? node.type : null), fn: fnId });
      const kids = [];
      if (node.first && typeof node.first === 'object') kids.push(node.first);
      if (node.second && typeof node.second === 'object') kids.push(node.second);
      for (const listKey of ['ea', 'sb']) {
        const list = node[listKey];
        if (Array.isArray(list)) for (const p of list) {
          if (p && typeof p === 'object') kids.push(p.kf && typeof p.kf === 'object' ? p.kf : p);
        }
      }
      for (const k of kids) emit(k, out);
    };

    const rows = [];
    const vis = new Set();
    const collect = (ace, kind) => {
      const nodes = [];
      if (Array.isArray(ace.ea)) {
        for (const p of ace.ea) {
          if (p && typeof p === 'object' && p.kf && typeof p.kf === 'object') emit(p.kf, nodes);
          else nodes.push({ type: null, fn: null });   // keep parameter alignment
        }
      }
      rows.push({ kind, sid: (typeof ace.Ba === 'number' ? ace.Ba : null), nodes });
    };

    const walk = (o, depth) => {
      if (!o || typeof o !== 'object' || depth > 60 || vis.has(o)) return;
      vis.add(o);
      if (isEvent(o)) {
        for (const k of Object.keys(o)) {
          if (k === 'sheet' || k === 'parent') continue;
          let arr; try { arr = o[k]; } catch (e) { continue; }
          if (!Array.isArray(arr) || !arr.length) continue;
          const m0 = arr[0];
          if (!m0 || typeof m0 !== 'object') continue;
          if (isCondition(m0)) for (const c of arr) collect(c, 'C');
          else if (isEvent(m0)) for (const s of arr) walk(s, depth + 1);
          else if (isAction(m0)) for (const a of arr) collect(a, 'A');
        }
      }
      for (const k of Object.keys(o)) {
        if (k === 'sheet' || k === 'parent' || k === 'type') continue;
        let v; try { v = o[k]; } catch (e) { continue; }
        if (v && typeof v === 'object') walk(v, depth + 1);
      }
    };
    for (const name of Object.keys(sheets)) walk(sheets[name], 0);

    const bodyList = new Array(bodies.size);
    for (const [src, id] of bodies) bodyList[id] = src;
    return { aceCount: rows.length, distinctBodies: bodies.size, bodies: bodyList, rows };
  });

  if (data.error) { console.error(data.error); process.exit(1); }
  fs.writeFileSync(outFile, JSON.stringify(data));
  console.log(`aces with params: ${data.aceCount}`);
  console.log(`distinct expression bodies: ${data.distinctBodies}`);
  console.log(`wrote ${outFile} (${(fs.statSync(outFile).size / 1048576).toFixed(1)} MB)`);

  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
