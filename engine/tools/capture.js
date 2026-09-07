// Runs the original browser build in Chromium and captures screenshots.
//
// This is the fidelity oracle: it shows what the real game does, which is what
// any C++ reimplementation has to be compared against. It also surfaces the
// console, which is where the runtime reports what it could not load.
//
//   node tools/capture.js <game_dir> <out_dir> [--shots 3,8,15,25] [--click]
//
// Needs playwright-core resolvable (set NODE_PATH) and a Chromium binary; the
// path is taken from PW_CHROME or the usual Playwright browsers location.

const http = require('http');
const fs = require('fs');
const path = require('path');

const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2] || '.');
const outDir = path.resolve(process.argv[3] || 'capture');
const shotArg = (process.argv.find(a => a.startsWith('--shots=')) || '').split('=')[1];
const shotTimes = (shotArg ? shotArg.split(',') : ['3', '8', '15', '25']).map(Number);
const doClick = process.argv.includes('--click');

const MIME = {
  '.html': 'text/html', '.js': 'application/javascript', '.json': 'application/json',
  '.png': 'image/png', '.jpg': 'image/jpeg', '.xml': 'text/xml', '.css': 'text/css',
  '.m4a': 'audio/mp4', '.ogg': 'audio/ogg', '.wav': 'audio/wav', '.webm': 'video/webm',
};

function serve(root) {
  const missing = [];
  const server = http.createServer((req, res) => {
    const rel = decodeURIComponent(req.url.split('?')[0]).replace(/^\/+/, '') || 'index.html';
    const file = path.join(root, rel);
    // Refuse to serve outside the game directory.
    if (!file.startsWith(root)) { res.writeHead(403).end(); return; }
    fs.readFile(file, (err, data) => {
      if (err) {
        missing.push(rel);
        res.writeHead(404).end();
        return;
      }
      res.writeHead(200, { 'Content-Type': MIME[path.extname(file).toLowerCase()] || 'application/octet-stream' });
      res.end(data);
    });
  });
  return { server, missing };
}

(async () => {
  fs.mkdirSync(outDir, { recursive: true });
  const { server, missing } = serve(gameDir);
  await new Promise(r => server.listen(0, '127.0.0.1', r));
  const port = server.address().port;
  const url = `http://127.0.0.1:${port}/index.html`;
  console.log(`serving ${gameDir} at ${url}`);

  const browser = await chromium.launch({
    executablePath: process.env.PW_CHROME ||
      '/opt/pw-browsers/chromium-1194/chrome-linux/chrome',
    args: [
      '--no-sandbox',
      '--use-gl=angle',
      '--use-angle=swiftshader',
      '--enable-unsafe-swiftshader',
      '--ignore-gpu-blocklist',
      '--autoplay-policy=no-user-gesture-required',
    ],
  });

  const page = await browser.newPage({ viewport: { width: 1024, height: 768 } });

  const console_lines = [];
  const errors = [];
  page.on('console', m => console_lines.push(`[${m.type()}] ${m.text()}`));
  page.on('pageerror', e => errors.push(String(e)));
  page.on('requestfailed', r => errors.push(`REQUEST FAILED ${r.url()} :: ${r.failure()?.errorText}`));
  page.on('dialog', async d => { console_lines.push(`[dialog] ${d.message()}`); await d.dismiss(); });

  await page.goto(url, { waitUntil: 'domcontentloaded', timeout: 60000 });

  let last = 0;
  for (const t of shotTimes) {
    await page.waitForTimeout(Math.max(0, (t - last) * 1000));
    last = t;
    const file = path.join(outDir, `t${String(t).padStart(3, '0')}s.png`);
    await page.screenshot({ path: file });
    console.log(`  shot ${file}`);
  }

  if (doClick) {
    // Many builds wait for a tap before leaving the title screen.
    await page.mouse.click(512, 384);
    await page.waitForTimeout(4000);
    await page.screenshot({ path: path.join(outDir, 'after_click.png') });
    console.log('  shot after_click.png');
  }

  // What the runtime actually built: canvas size tells us if it initialised.
  const info = await page.evaluate(() => {
    const c = document.querySelector('canvas');
    return {
      title: document.title,
      canvas: c ? { w: c.width, h: c.height, style: c.getAttribute('style') || '' } : null,
      webgl: (() => {
        try {
          const t = document.createElement('canvas');
          const gl = t.getContext('webgl') || t.getContext('experimental-webgl');
          return gl ? gl.getParameter(gl.VERSION) : 'none';
        } catch (e) { return 'error: ' + e.message; }
      })(),
      hasC2: typeof window.cr !== 'undefined',
    };
  }).catch(e => ({ error: String(e) }));

  fs.writeFileSync(path.join(outDir, 'report.json'), JSON.stringify({
    url, info, errors, missing: [...new Set(missing)], console: console_lines,
  }, null, 2));

  console.log('\n--- page info ---');
  console.log(JSON.stringify(info, null, 2));
  console.log(`\nconsole lines: ${console_lines.length}, errors: ${errors.length}, 404s: ${new Set(missing).size}`);
  for (const e of errors.slice(0, 15)) console.log('  ERR ' + e);
  for (const m of [...new Set(missing)].slice(0, 15)) console.log('  404 ' + m);

  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
