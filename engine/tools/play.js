// Drives the original build past the menu and into gameplay, capturing each
// step. Used to produce reference frames for comparing a reimplementation
// against, and to confirm the build is actually playable rather than merely
// booting.
//
//   node tools/play.js <game_dir> <out_dir>

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2] || '.');
const outDir = path.resolve(process.argv[3] || 'play');

const MIME = {
  '.html': 'text/html', '.js': 'application/javascript', '.json': 'application/json',
  '.png': 'image/png', '.jpg': 'image/jpeg', '.xml': 'text/xml', '.css': 'text/css',
  '.m4a': 'audio/mp4', '.ogg': 'audio/ogg', '.wav': 'audio/wav',
};

(async () => {
  fs.mkdirSync(outDir, { recursive: true });
  const server = http.createServer((req, res) => {
    const rel = decodeURIComponent(req.url.split('?')[0]).replace(/^\/+/, '') || 'index.html';
    const file = path.join(gameDir, rel);
    if (!file.startsWith(gameDir)) { res.writeHead(403).end(); return; }
    fs.readFile(file, (err, data) => {
      if (err) { res.writeHead(404).end(); return; }
      res.writeHead(200, { 'Content-Type': MIME[path.extname(file).toLowerCase()] || 'application/octet-stream' });
      res.end(data);
    });
  });
  await new Promise(r => server.listen(0, '127.0.0.1', r));
  const port = server.address().port;

  const browser = await chromium.launch({
    executablePath: process.env.PW_CHROME || '/opt/pw-browsers/chromium-1194/chrome-linux/chrome',
    args: ['--no-sandbox', '--use-gl=angle', '--use-angle=swiftshader',
           '--enable-unsafe-swiftshader', '--autoplay-policy=no-user-gesture-required'],
  });
  const page = await browser.newPage({ viewport: { width: 1024, height: 768 } });
  const errors = [];
  page.on('pageerror', e => errors.push(String(e)));
  page.on('dialog', async d => { await d.dismiss(); });

  await page.goto(`http://127.0.0.1:${port}/index.html`, { waitUntil: 'domcontentloaded' });
  await page.waitForTimeout(24000);          // splash + load + menu settle
  await page.screenshot({ path: path.join(outDir, '00_menu.png') });

  // Menu buttons are not all in the same column: the main menu column sits
  // around x=512, the difficulty column shifts left to about x=411. Clicking a
  // fixed centre point silently misses, so each step names its own target.
  // Only two deliberate clicks. Blind extra clicks land on "Back" and bounce
  // the menu around, which makes it impossible to tell whether a step worked.
  // The flow is: New game -> pick a difficulty -> a Start button appears to the
  // right of the column -> gameplay. Start does not exist until a difficulty is
  // selected, which is why clicking blindly never gets past this screen.
  const steps = [
    { name: 'new_game',   x: 512, y: 377, wait: 5000 },
    { name: 'difficulty', x: 411, y: 377, wait: 6000 },
    { name: 'start',      x: 612, y: 450, wait: 12000 },
    { name: 'ingame_a',   x: null, y: null, wait: 10000 },
    { name: 'ingame_b',   x: null, y: null, wait: 12000 },
  ];
  let n = 1;
  for (const s of steps) {
    if (s.x !== null) await page.mouse.click(s.x, s.y);
    await page.waitForTimeout(s.wait);
    await page.screenshot({ path: path.join(outDir, `${String(n).padStart(2, '0')}_${s.name}.png`) });
    console.log(`  step ${s.name} -> ${s.x === null ? 'wait only' : `click(${s.x},${s.y})`}`);
    n++;
  }

  const state = await page.evaluate(() => {
    const c = document.querySelector('canvas');
    return { canvas: c ? [c.width, c.height] : null, title: document.title };
  }).catch(e => ({ error: String(e) }));

  fs.writeFileSync(path.join(outDir, 'report.json'),
                   JSON.stringify({ state, errors }, null, 2));
  console.log(JSON.stringify({ state, errors: errors.slice(0, 10) }, null, 2));
  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
