const { chromium } = require(process.env.PW);
(async () => {
  const b = await chromium.launch({ executablePath: '/opt/pw-browsers/chromium', args: ['--autoplay-policy=no-user-gesture-required'] });
  const ctx = await b.newContext({ viewport: { width: 1280, height: 720 } });
  const p = await ctx.newPage();
  const logs = [];
  p.on('console', m => logs.push(m.type() + ' ' + m.text()));
  p.on('pageerror', e => logs.push('pageerror ' + e.message));
  await p.goto('http://127.0.0.1:8731/index.html');
  for (const t of [5, 15, 30]) {
    await p.waitForTimeout(t === 5 ? 5000 : 10000);
    await p.screenshot({ path: `${process.env.OUT}/boot_${t}.png` });
  }
  console.log(logs.slice(0, 30).join('\n'));
  await b.close();
})();
