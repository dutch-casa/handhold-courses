const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const appJs = fs.readFileSync("app.js", "utf-8");

  // Test 1: Uses fetch
  const usesFetch = /fetch\s*\(/.test(appJs);
  check(usesFetch, "app.js uses fetch() to load data");

  // Test 2: Uses async/await
  const usesAsync = /async\s+function|async\s*\(/.test(appJs);
  const usesAwait = /await\s+/.test(appJs);
  check(usesAsync && usesAwait, "uses async/await pattern");

  // Test 3: Handles loading state
  const handlesLoading = /loading/.test(appJs) && /hidden/.test(appJs);
  check(handlesLoading, "manages loading state (shows/hides .loading)");

  // Test 4: Handles errors
  const handlesCatch = /catch\s*\(/.test(appJs) || /\.catch\s*\(/.test(appJs);
  const showsError = /error/.test(appJs);
  check(handlesCatch && showsError, "handles fetch errors with try/catch");

  // Test 5: Creates list items with name and email
  const createsItems = /createElement|innerHTML|textContent/.test(appJs);
  const usesUserData = /\.name|\.email/.test(appJs);
  check(createsItems && usesUserData, "renders user name and email in list items");

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
