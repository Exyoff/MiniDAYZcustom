// Captures the live Construct 2 runtime object so its ACE tables can be read.
//
// Why this is needed: the export's `cr_createRuntime` is the minified function
// `ac`, which does `new f(...)` where `f` is a closure-local constructor. From
// outside the bundle neither is reachable, and the runtime instance is never
// stored on a global. So the runtime script is rewritten in flight -- we serve
// a patched copy that reassigns `f` to a wrapper recording each constructed
// instance on `window.__c2_rt`. The patch runs inside the bundle's own scope,
// which is the only place those names exist.
//
//   node tools/hook.js <game_dir>

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2]);
const MIME = { '.html':'text/html','.js':'application/javascript','.json':'application/json',
  '.png':'image/png','.jpg':'image/jpeg','.xml':'text/xml','.css':'text/css',
  '.m4a':'audio/mp4','.ogg':'audio/ogg','.wav':'audio/wav' };

// Reassign the constructor before `ac` is ever called, keeping the prototype so
// instanceof and method lookup are unaffected.
const PATCH = 'var __C2F=f;f=function(a){var o=new __C2F(a);try{window.__c2_rt=o;' +
              'window.__c2_ctor=__C2F;}catch(e){}return o};' +
              'f.prototype=__C2F.prototype;window.cr_createRuntime=ac;';

function patchRuntime(src) {
  const re = /window\.cr_createRuntime\s*=\s*\n?\s*ac;/;
  if (!re.test(src)) return { src, patched: false };
  return { src: src.replace(re, PATCH), patched: true };
}

(async () => {
  let patched = false;
  const server = http.createServer((req, res) => {
    const rel = decodeURIComponent(req.url.split('?')[0]).replace(/^\/+/, '') || 'index.html';
    const file = path.join(gameDir, rel);
    if (!file.startsWith(gameDir)) { res.writeHead(403).end(); return; }
    fs.readFile(file, (err, data) => {
      if (err) { res.writeHead(404).end(); return; }
      let body = data;
      if (rel === 'c2runtime.js') {
        const r = patchRuntime(data.toString('utf8'));
        patched = r.patched;
        body = Buffer.from(r.src, 'utf8');
      }
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
  const errors = [];
  page.on('pageerror', e => errors.push(String(e)));
  page.on('dialog', async d => { await d.dismiss(); });
  await page.goto(`http://127.0.0.1:${server.address().port}/index.html`, { waitUntil: 'domcontentloaded' });
  await page.waitForTimeout(20000);

  // Describe the captured runtime: how many own properties, and which of them
  // look like the object-type and event-sheet collections.
  const found = await page.evaluate(() => {
    const rt = window.__c2_rt;
    if (!rt) return { captured: false };
    const out = { captured: true, ctor: typeof window.__c2_ctor, props: [] };
    for (const k of Object.keys(rt)) {
      let v; try { v = rt[k]; } catch (e) { continue; }
      let desc = typeof v;
      if (Array.isArray(v)) desc = `array[${v.length}]`;
      else if (v && typeof v === 'object') desc = `object{${Object.keys(v).length}}`;
      out.props.push(`${k}: ${desc}`);
    }
    return out;
  });

  console.log(`runtime script patched: ${patched}`);
  console.log(`page errors: ${errors.length}`);
  errors.slice(0, 5).forEach(e => console.log('  ERR ' + e));
  console.log(JSON.stringify(found, null, 1).slice(0, 3500));

  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
