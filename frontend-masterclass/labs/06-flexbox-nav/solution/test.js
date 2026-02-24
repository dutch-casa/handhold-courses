const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: .nav uses display: flex
  const navFlex = /\.nav\s*\{[^}]*display\s*:\s*flex/s.test(css);
  check(navFlex, ".nav uses display: flex");

  // Test 2: Items vertically centered
  const centered = /\.nav\s*\{[^}]*align-items\s*:\s*center/s.test(css);
  check(centered, ".nav items are vertically centered with align-items: center");

  // Test 3: Space-between for logo/CTA distribution
  const spaceBetween = /\.nav\s*\{[^}]*justify-content\s*:\s*space-between/s.test(css);
  check(spaceBetween, ".nav uses justify-content: space-between");

  // Test 4: Nav wraps on narrow screens
  const wraps = /\.nav\s*\{[^}]*flex-wrap\s*:\s*wrap/s.test(css);
  check(wraps, ".nav uses flex-wrap: wrap");

  // Test 5: Nav links use flex with gap
  const linksGap = /\.nav-links\s*\{[^}]*(display\s*:\s*flex|gap\s*:)/s.test(css);
  check(linksGap, ".nav-links uses flex display or gap for spacing");

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
