const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: Custom properties defined on :root
  const hasRoot = /:root\s*\{[^}]*--color-primary\s*:/s.test(css);
  const hasTextVar = /:root\s*\{[^}]*--color-text\s*:/s.test(css);
  check(hasRoot && hasTextVar, "custom properties --color-primary and --color-text defined on :root");

  // Test 2: Headings use system font stack
  const hasSystemFont = /system-ui/.test(css);
  const headingRule = /(h1|h2|h3)[^{]*\{[^}]*font-family/s.test(css)
    || /body[^{]*\{[^}]*font-family[^}]*system-ui/s.test(css);
  check(hasSystemFont && headingRule, "headings use system font stack");

  // Test 3: Link hover effect
  const hasHover = /a\s*:hover|a:hover/s.test(css);
  check(hasHover, "links have a :hover style");

  // Test 4: List markers styled with custom property
  const hasMarker = /::marker/s.test(css);
  const usesVar = /::marker[^}]*var\s*\(\s*--color-primary\s*\)/s.test(css)
    || /li\s*::marker[^}]*var\s*\(\s*--color-primary\s*\)/s.test(css);
  check(hasMarker && usesVar, "list markers styled with --color-primary");

  // Test 5: Universal box-sizing reset
  const hasReset = /\*[^{]*\{[^}]*box-sizing\s*:\s*border-box/s.test(css);
  check(hasReset, "universal box-sizing: border-box reset");

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
