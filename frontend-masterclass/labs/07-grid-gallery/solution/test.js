const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("styles.css", "utf-8");

  // Test 1: Gallery uses display: grid
  const hasGrid = /\.gallery\s*\{[^}]*display\s*:\s*grid/s.test(css);
  check(hasGrid, ".gallery uses display: grid");

  // Test 2: Uses auto-fill or auto-fit
  const hasAutoFill = /auto-fill|auto-fit/s.test(css);
  check(hasAutoFill, "uses auto-fill or auto-fit for responsive columns");

  // Test 3: Has gap defined
  const hasGap = /\.gallery\s*\{[^}]*gap\s*:/s.test(css);
  check(hasGap, ".gallery has gap defined");

  // Test 4: Featured item spans 2 columns
  const hasSpan = /\.featured\s*\{[^}]*grid-column\s*:\s*span\s+2/s.test(css);
  check(hasSpan, ".featured spans 2 columns");

  // Test 5: Dense auto-flow
  const hasDense = /\.gallery\s*\{[^}]*grid-auto-flow\s*:[^;]*dense/s.test(css);
  check(hasDense, ".gallery uses grid-auto-flow: dense");

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
