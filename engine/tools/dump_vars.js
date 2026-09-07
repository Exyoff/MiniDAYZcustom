// Dumps the live runtime's event-sheet variables, for differential testing
// against the C++ engine.
//
// Sheet member entries carry an unminified `name` and a `data` field holding
// the current value, so variables can be read without knowing any other
// minified name. Snapshot time is a parameter because the values move as the
// game boots.
//
//   node tools/dump_vars.js <game_dir> <out.json> [--at-ms 1500]

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2]);
const outFile = path.resolve(process.argv[3] || 'vars.json');
const atArg = (process.argv.find(a => a.startsWith('--at-ms=')) || '').split('=')[1];
const AT_MS = Number(atArg || 1500);

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
  await page.waitForTimeout(AT_MS);

  const dump = await page.evaluate(() => {
    const rt = window.__c2_rt;
    if (!rt) return { error: 'runtime not captured' };
    const seen = new Set();
    let sheets = null;
    const q = [[rt, 0]];
    while (q.length && !sheets) {
      const [o, d] = q.shift();
      if (!o || typeof o !== 'object' || d > 4 || seen.has(o)) continue;
      seen.add(o);
      let ks; try { ks = Object.keys(o); } catch (e) { continue; }
      if (ks.includes('Game_events') && ks.includes('Menu_Events')) { sheets = o; break; }
      for (const k of ks) { let v; try { v = o[k]; } catch (e) { continue; }
        if (v && typeof v === 'object') q.push([v, d + 1]); }
    }
    if (!sheets) return { error: 'sheets not found' };

    const vars = {};
    let layoutName = null;
    try { layoutName = rt.wa && rt.wa.name ? rt.wa.name : null; } catch (e) {}
    for (const sheetName of Object.keys(sheets)) {
      const sheet = sheets[sheetName];
      for (const k of Object.keys(sheet)) {
        let arr; try { arr = sheet[k]; } catch (e) { continue; }
        if (!Array.isArray(arr)) continue;
        for (const m of arr) {
          if (!m || typeof m !== 'object') continue;
          if (typeof m.name !== 'string') continue;
          if (!Object.prototype.hasOwnProperty.call(m, 'data')) continue;
          const v = m.data;
          if (typeof v === 'number' || typeof v === 'string') vars[m.name] = v;
        }
      }
    }
    return { layout: layoutName, count: Object.keys(vars).length, vars };
  });

  if (dump.error) { console.error(dump.error); process.exit(1); }
  fs.writeFileSync(outFile, JSON.stringify(dump, null, 1));
  console.log(`layout at snapshot: ${dump.layout}`);
  console.log(`variables captured: ${dump.count}`);
  console.log(`wrote ${outFile}`);

  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
