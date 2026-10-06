// Drive the ORIGINAL Mini DayZ (the official HTML5 build, Construct 2) in headless Chromium.
//
//   node orig.cjs --out DIR [--res 1280x720] [--touch] [--build 1.0|1.2|Reloaded] --steps "STEP;STEP;..."
//
// Steps (separated by ';'):
//   wait:MS            wait MS milliseconds of real time
//   shot:NAME          screenshot to DIR/NAME.png
//   click:X,Y          mouse click at page pixel X,Y   (tap:X,Y with --touch)
//   down:X,Y / up:X,Y  mouse button down / up at X,Y; move:X,Y moves the mouse
//   key:CODE[:MS]      press a key (KeyboardEvent.code, e.g. KeyW, Space, Tab, Escape) and hold it MS ms (default 80)
//   hold:CODE / release:CODE
//   js:EXPR            evaluate EXPR in the page and print its result (cr_getC2Runtime() is the C2 runtime;
//                      window.ORIG has helpers, see orig_helpers.js). EXPR cannot contain ';' (the step
//                      separator) and must not assign bare globals (they clobber the minified runtime's).
//   jsfile:PATH        evaluate the JavaScript in PATH (statements allowed; `return` a value to print it)
// The page is served from http://127.0.0.1:8731 (start: cd <build dir> && python3 -m http.server 8731 --bind 127.0.0.1).
// Loading the game to its menu takes about 15-25 s here: start with wait:20000.
const path = require('path');
// playwright, or playwright-core beside this file (`npm install --no-save playwright-core`),
// or PW=<path to either>.
const pick = () => { try { require.resolve('playwright'); return 'playwright'; } catch (e) { return 'playwright-core'; } };
const { chromium } = require(process.env.PW || pick());
const args = process.argv.slice(2);
const opt = (k, d) => { const i = args.indexOf(k); return i >= 0 ? args[i + 1] : d; };
const flag = (k) => args.includes(k);
const OUT = path.resolve(opt('--out', '.'));
const [W, H] = opt('--res', '1280x720').split('x').map(Number);
const PORT = Number(opt('--port', '8731'));
const steps = (opt('--steps', 'wait:20000;shot:menu')).split(';').filter(Boolean);
(async () => {
  const browser = await chromium.launch({ executablePath: process.env.ORIG_BROWSER || (process.platform === 'win32'
    ? 'C:/Program Files (x86)/Microsoft/Edge/Application/msedge.exe' : '/opt/pw-browsers/chromium'),
    args: ['--autoplay-policy=no-user-gesture-required', '--enable-unsafe-swiftshader'] });
  const touch = flag('--touch');
  const ctx = await browser.newContext({ viewport: { width: W, height: H },
    ...(touch ? { isMobile: true, hasTouch: true, deviceScaleFactor: 1 } : {}) });
  const page = await ctx.newPage();
  const seen = {};
  page.on('pageerror', e => { const m = e.message; seen[m] = (seen[m] || 0) + 1; if (seen[m] === 1) console.log('pageerror', m); else if (seen[m] === 2) console.log('pageerror (repeating, further copies not printed)', m); });
  // ORIG helpers + the name -> t-id table written by make_index.py
  const fs = require('fs');
  const names = fs.existsSync(path.join(__dirname, 'names.json')) ? fs.readFileSync(path.join(__dirname, 'names.json'), 'utf8') : '{}';
  await page.addInitScript({ content: 'window.ORIG_NAMES = ' + names + ';\n' + fs.readFileSync(path.join(__dirname, 'orig_helpers.js'), 'utf8') });
  await page.goto(`http://127.0.0.1:${PORT}/index.html`);
  for (const s of steps) {
    const i = s.indexOf(':'); const op = s.slice(0, i); const a = s.slice(i + 1);
    if (op === 'wait') await page.waitForTimeout(Number(a));
    else if (op === 'shot') { await page.screenshot({ path: path.join(OUT, a + '.png') }); console.log('shot', path.join(OUT, a + '.png')); }
    else if (op === 'click') { const [x, y] = a.split(',').map(Number); await page.mouse.click(x, y); }
    else if (op === 'tap') { const [x, y] = a.split(',').map(Number); await page.touchscreen.tap(x, y); }
    else if (op === 'down') { const [x, y] = a.split(',').map(Number); await page.mouse.move(x, y); await page.mouse.down(); }
    else if (op === 'up') { const [x, y] = a.split(',').map(Number); await page.mouse.move(x, y); await page.mouse.up(); }
    else if (op === 'move') { const [x, y] = a.split(',').map(Number); await page.mouse.move(x, y, { steps: 5 }); }
    else if (op === 'key') { const [k, ms] = a.split(':'); await page.keyboard.down(k); await page.waitForTimeout(Number(ms || 80)); await page.keyboard.up(k); }
    else if (op === 'hold') await page.keyboard.down(a);
    else if (op === 'release') await page.keyboard.up(a);
    else if (op === 'js') { try { console.log('js', JSON.stringify(await page.evaluate(a))); } catch (e) { console.log('js error', e.message.split('\n')[0]); } }
    else if (op === 'jsfile') { try { console.log('jsfile', JSON.stringify(await page.evaluate('(() => {\n' + require('fs').readFileSync(a, 'utf8') + '\n})()'))); } catch (e) { console.log('jsfile error', e.message.split('\n')[0]); } }
    else console.log('unknown step', s);
  }
  await browser.close();
})();
