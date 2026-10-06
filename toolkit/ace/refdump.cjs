// Load a Construct 2 c2runtime.js in headless Chromium and dump the source of every ACE method.
//   node refdump.cjs min <c2runtime.js> <out.json>   minified runtime: dumps the object-ref table
//   node refdump.cjs ref <c2runtime.js> <out.json>   unminified runtime: dumps cr.plugins_/cr.behaviors/system
const fs = require('fs');
const { chromium } = require(process.env.PW || 'playwright');
const [mode, file, out] = process.argv.slice(2);
(async () => {
  const browser = await chromium.launch({ executablePath: '/opt/pw-browsers/chromium', args: ['--autoplay-policy=no-user-gesture-required', '--enable-unsafe-swiftshader'] });
  const page = await browser.newPage();
  page.on('pageerror', e => console.error('pageerror', e.message));
  const src = fs.readFileSync(file, 'utf8').replace(/^\uFEFF/, '');
  if (mode === 'min') {
    // the real game page: common ACEs (CompareX, Destroy, SetPosition...) are only attached to the
    // plugins by cr.add_common_aces when the runtime loads the project, so let it boot a few seconds
    await page.goto(process.env.ORIG_URL || 'http://127.0.0.1:8731/index.html');
    await page.waitForTimeout(8000);
  } else {
    await page.setContent('<html><body></body></html>');
    // stub jQuery for runtimes that touch it at load
    await page.addScriptTag({ content: 'window.jQuery = window.$ = function(){ return { ready(){}, on(){}, bind(){} } };' });
    await page.addScriptTag({ content: src });
  }
  let res;
  if (mode === 'min') {
    const i = src.lastIndexOf('return[');
    const j = src.indexOf(']}', i);
    const table = src.slice(i + 7, j).replace(/\s+/g, '').split(',');
    res = await page.evaluate((table) => {
      const g = (e) => (0, eval)(e);
      const rows = table.map((e) => { let v; try { v = g(e); } catch (err) { return { expr: e, err: String(err) }; }
        return { expr: e, src: typeof v === 'function' ? v.toString() : String(v), len: typeof v === 'function' ? v.length : null }; });
      // every method of every ctor's cnds/acts/exps object (k/n/A), referenced or not
      const ctors = {};
      for (const e of table) { if (e.includes('.')) continue; const c = g(e); const d = { src: c.toString(), cats: {} };
        for (const cat of ['k', 'n', 'A']) { const o = c.prototype && c.prototype[cat]; if (!o) continue; d.cats[cat] = {};
          for (const k in o) if (typeof o[k] === 'function') d.cats[cat][k] = { src: o[k].toString(), len: o[k].length }; }
        // the rest of the prototype graph helps identify a plugin: Type/Instance prototypes are reachable only via instances; take ctor source + own keys
        d.protoKeys = Object.keys(c.prototype || {});
        ctors[e] = d; }
      return { table: rows, ctors };
    }, table);
  } else {
    const i = src.indexOf('cr.getObjectRefTable');
    const tsrc = src.slice(src.indexOf('[', i) + 1, src.indexOf(']', src.indexOf('[', i))).replace(/\s+/g, '');
    res = await page.evaluate((tsrc) => {
      const out = { plugins: {}, behaviors: {}, table: tsrc.split(',').filter(Boolean) };
      const dump = (c) => { const d = {};
        for (const [cat, nm] of [['cnds', 'cnds'], ['acts', 'acts'], ['exps', 'exps']]) { const o = c.prototype[nm]; if (!o) continue; d[cat] = {};
          for (const k in o) if (typeof o[k] === 'function') d[cat][k] = { src: o[k].toString(), len: o[k].length }; }
        return d; };
      for (const k in cr.plugins_) out.plugins[k] = dump(cr.plugins_[k]);
      for (const k in cr.behaviors) out.behaviors[k] = dump(cr.behaviors[k]);
      out.plugins.System = dump(cr.system_object);
      // the common ACEs every world plugin gets at project load (all flags on, not single-global)
      const Common = function () {}; Common.prototype = {};
      try { cr.add_common_aces([Common, false, true, true, true, true, true, true, true, true], Common.prototype); } catch (e) { out.commonErr = String(e); }
      out.plugins.Common = dump(Common);
      return out;
    }, tsrc);
  }
  fs.writeFileSync(out, JSON.stringify(res, null, 1));
  console.log('wrote', out);
  await browser.close();
})();
