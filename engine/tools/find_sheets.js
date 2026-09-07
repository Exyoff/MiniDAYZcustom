// Locates the event-sheet collection inside the captured runtime and reports
// the shape of a sheet, an event, and a condition.
//
// The runtime's property names are minified, but event sheet NAMES are not, so
// an object whose keys include "Game_events" is an unambiguous anchor. From
// there the structure is discovered by inspection rather than guessed.

const http = require('http');
const fs = require('fs');
const path = require('path');
const { chromium } = require('playwright-core');

const gameDir = path.resolve(process.argv[2]);
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

  const result = await page.evaluate(() => {
    const rt = window.__c2_rt;
    if (!rt) return { error: 'runtime not captured' };

    // Breadth-first search for an object keyed by event sheet name.
    const seen = new Set();
    let anchor = null, anchorPath = null;
    const queue = [[rt, '__c2_rt', 0]];
    while (queue.length && !anchor) {
      const [obj, p, depth] = queue.shift();
      if (!obj || typeof obj !== 'object' || depth > 4 || seen.has(obj)) continue;
      seen.add(obj);
      let keys; try { keys = Object.keys(obj); } catch (e) { continue; }
      if (keys.includes('Game_events') && keys.includes('Menu_Events')) {
        anchor = obj; anchorPath = p; break;
      }
      for (const k of keys) {
        let v; try { v = obj[k]; } catch (e) { continue; }
        if (v && typeof v === 'object') queue.push([v, p + '.' + k, depth + 1]);
      }
    }
    if (!anchor) return { error: 'event sheet collection not found' };

    const describe = (o, limit = 40) => {
      const out = [];
      for (const k of Object.keys(o).slice(0, limit)) {
        let v; try { v = o[k]; } catch (e) { continue; }
        let d = typeof v;
        if (Array.isArray(v)) d = `array[${v.length}]`;
        else if (typeof v === 'function') d = `function(${v.length}) len=${String(v).length}`;
        else if (v && typeof v === 'object') d = `object{${Object.keys(v).length}}`;
        else if (typeof v === 'string') d = `"${v.slice(0, 24)}"`;
        out.push(`${k}: ${d}`);
      }
      return out;
    };

    const sheet = anchor['Game_events'];
    const report = { anchorPath, sheetNames: Object.keys(anchor), sheet: describe(sheet) };

    // The sheet should hold an array of top-level events; find the longest one.
    let bestKey = null, bestLen = -1;
    for (const k of Object.keys(sheet)) {
      let v; try { v = sheet[k]; } catch (e) { continue; }
      if (Array.isArray(v) && v.length > bestLen) { bestLen = v.length; bestKey = k; }
    }
    report.eventArrayKey = bestKey;
    report.eventArrayLen = bestLen;

    // Vg mixes sheet variables and events. An event is an element carrying
    // arrays whose members hold a function -- the condition and action lists.
    const members = sheet[bestKey] || [];
    let events = 0, vars = 0, sampleEvent = null;
    for (const m of members) {
      if (!m || typeof m !== 'object') continue;
      let isEvent = false;
      for (const k of Object.keys(m)) {
        let v; try { v = m[k]; } catch (e) { continue; }
        if (Array.isArray(v) && v.length && v[0] && typeof v[0] === 'object') {
          if (Object.keys(v[0]).some(kk => typeof v[0][kk] === 'function')) { isEvent = true; break; }
        }
      }
      if (isEvent) { events++; if (!sampleEvent) sampleEvent = m; } else { vars++; }
    }
    report.memberCounts = { total: members.length, events, nonEvents: vars };

    if (sampleEvent) {
      report.event = describe(sampleEvent);
      for (const k of Object.keys(sampleEvent)) {
        let v; try { v = sampleEvent[k]; } catch (e) { continue; }
        if (Array.isArray(v) && v.length && v[0] && typeof v[0] === 'object' &&
            Object.keys(v[0]).some(kk => typeof v[0][kk] === 'function')) {
          report['list_' + k] = { len: v.length, memberShape: describe(v[0]) };
        }
      }
    }
    return report;
  });

  console.log(JSON.stringify(result, null, 1).slice(0, 5000));
  await browser.close();
  server.close();
})().catch(e => { console.error('FATAL', e); process.exit(1); });
