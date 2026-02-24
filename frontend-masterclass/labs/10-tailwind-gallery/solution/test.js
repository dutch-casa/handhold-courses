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

  // Test 1: No custom CSS (only Tailwind import)
  const cssLines = css.trim().split("\n").filter((l) => l.trim() && !l.trim().startsWith("/*") && !l.trim().startsWith("//"));
  const onlyImport = cssLines.length <= 1 && /@import\s+["']tailwindcss["']/.test(css);
  check(onlyImport, "styles.css contains only Tailwind import (no custom CSS)");

  // Test 2: Responsive grid classes
  const hasResponsiveGrid = /grid-cols-1/.test(html)
    && /md:grid-cols-2/.test(html)
    && /lg:grid-cols-3/.test(html);
  check(hasResponsiveGrid, "gallery uses responsive grid: 1→2→3 columns");

  // Test 3: Hover effects
  const hasHover = /hover:scale|hover:shadow/.test(html);
  check(hasHover, "cards have hover effects (scale or shadow)");

  // Test 4: Dark mode classes
  const hasDark = /dark:bg-|dark:text-/.test(html);
  check(hasDark, "has dark mode classes (dark:bg- or dark:text-)");

  // Test 5: Gap between items
  const hasGap = /gap-\d/.test(html);
  check(hasGap, "grid has gap spacing between items");

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
