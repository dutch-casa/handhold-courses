const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: Layer declaration exists
  const hasLayers = /@layer\s+reset\s*,\s*base\s*,\s*components\s*,\s*utilities\s*;/s.test(css);
  check(hasLayers, "CSS layers declared in order: reset, base, components, utilities");

  // Test 2: Reset is inside a layer
  const resetInLayer = /@layer\s+reset\s*\{[^}]*box-sizing/s.test(css);
  check(resetInLayer, "reset styles are inside @layer reset");

  // Test 3: Uses native CSS nesting (& inside .card block)
  const hasNesting = /\.card\s*\{[^}]*&/s.test(css);
  check(hasNesting, "uses native CSS nesting with & inside .card");

  // Test 4: :has(img) selector for card layout
  const hasHas = /\.card:has\(\s*img\s*\)/s.test(css);
  check(hasHas, ".card:has(img) selector exists for image card layout");

  // Test 5: light-dark() function used
  const hasLightDark = /light-dark\s*\(/s.test(css);
  check(hasLightDark, "uses light-dark() for at least one color value");

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
