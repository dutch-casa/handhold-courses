const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const src = fs.readFileSync("src/FilterableList.tsx", "utf-8");

  // Test 1: Item interface defined
  const hasInterface = /interface\s+Item/.test(src);
  const hasReadonlyId = /readonly\s+id\s*:\s*number/.test(src);
  check(hasInterface && hasReadonlyId, "Item interface defined with readonly id");

  // Test 2: Text filter narrows results
  const hasSearchState = /useState.*["']{2}/.test(src);
  const hasIncludes = /\.includes\s*\(/.test(src);
  check(hasSearchState && hasIncludes, "Text search filter with includes()");

  // Test 3: Category dropdown filter
  const hasCategoryState = /useState.*["']all["']/.test(src);
  const hasCategoryFilter = /category\s*===/.test(src) || /\.category\s*===/.test(src);
  check(hasCategoryState && hasCategoryFilter, "Category dropdown filter with state");

  // Test 4: Sort changes order
  const hasSortState = /useState.*["']name["']|useState.*["']date["']/.test(src);
  const hasToSorted = /\.toSorted\s*\(/.test(src);
  check(hasSortState && hasToSorted, "Sort toggle with toSorted()");

  // Test 5: Displayed list is derived, not stored in state
  const useStateCount = (src.match(/useState/g) || []).length;
  const hasFilterChain = /\.filter\([\s\S]*?\.filter\(/.test(src);
  check(useStateCount <= 3 && hasFilterChain, "Displayed list derived via filter chain, not stored in state");

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
