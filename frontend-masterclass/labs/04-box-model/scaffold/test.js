const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: Universal box-sizing reset
  const hasReset = /\*[^{]*\{[^}]*box-sizing\s*:\s*border-box/s.test(css);
  check(hasReset, "universal box-sizing: border-box reset applied to *");

  // Test 2: .box-1 uses border-box (either via universal reset or direct)
  // If universal reset is applied, .box-1 inherits it — check that the reset exists
  check(hasReset, ".box-1 will be exactly 300px with border-box reset");

  // Test 3: Card stack gap fix (flex + gap, or padding/border trick)
  const hasGapFix = /\.card-stack[^{]*\{[^}]*(display\s*:\s*flex|gap\s*:)/s.test(css);
  check(hasGapFix, "margin collapse fixed between .card-a and .card-b");

  // Test 4: Container centered with margin auto
  const containerCentered = /\.container[^{]*\{[^}]*(margin\s*:[^;]*(auto|0\s+auto))/s.test(css);
  const hasMaxWidth = /\.container[^{]*\{[^}]*max-width\s*:/s.test(css);
  check(containerCentered && hasMaxWidth, ".container centered with max-width and margin auto");

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
