const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const html = fs.readFileSync("index.html", "utf-8");
  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: @theme block with custom color
  const hasTheme = /@theme\s*\{[^}]*--color-brand/s.test(css);
  check(hasTheme, "styles.css has @theme block with custom brand color");

  // Test 2: Sidebar uses flex layout
  const sidebarFlex = /aside[^>]*class="[^"]*flex[^"]*"/.test(html)
    || /aside[^>]*class="[^"]*hidden\s+md:flex/.test(html);
  check(sidebarFlex, "sidebar uses flex layout");

  // Test 3: Main content uses responsive grid
  const hasGrid = /grid-cols-1/.test(html)
    && /(md|lg):grid-cols-[23]/.test(html);
  check(hasGrid, "card grid is responsive (1→2→3 columns)");

  // Test 4: Responsive sidebar collapse
  const sidebarCollapses = /hidden\s+(md|lg):flex/.test(html)
    || /hidden\s+(md|lg):block/.test(html);
  check(sidebarCollapses, "sidebar collapses on mobile (hidden md:flex)");

  // Test 5: Dark mode support
  const hasDark = /dark:bg-/.test(html);
  check(hasDark, "layout has dark mode support");

  // Test 6: Cards have hover state
  const hasHover = /hover:(shadow|scale|bg-)/.test(html);
  check(hasHover, "cards have hover state");

  // TAP output
  console.log(`1..${count}`);
  for (const r of results) {
    console.log(`${r.ok ? "ok" : "not ok"} ${r.index} - ${r.description}`);
  }

  process.exit(results.some((r) => !r.ok) ? 1 : 0);
}

test().catch((err) => {
  console.error("Bail out!", err.message);
  process.exit(1);
});
