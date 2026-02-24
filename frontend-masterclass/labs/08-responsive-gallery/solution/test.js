const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: Uses clamp() for font-size
  const hasClamp = /clamp\s*\(/s.test(css);
  check(hasClamp, "uses clamp() for fluid font-size");

  // Test 2: Uses @container query
  const hasContainer = /@container/s.test(css);
  check(hasContainer, "uses @container query");

  // Test 3: Has prefers-color-scheme: dark
  const hasDark = /prefers-color-scheme\s*:\s*dark/s.test(css);
  check(hasDark, "has prefers-color-scheme: dark media query");

  // Test 4: Has prefers-reduced-motion
  const hasMotion = /prefers-reduced-motion/s.test(css);
  check(hasMotion, "has prefers-reduced-motion media query");

  // Test 5: Container type set on gallery or card parent
  const hasContainerType = /container-type\s*:\s*inline-size/s.test(css);
  check(hasContainerType, "has container-type: inline-size for container queries");

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
