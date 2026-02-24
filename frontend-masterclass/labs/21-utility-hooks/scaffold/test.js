const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const storage = fs.readFileSync("src/useLocalStorage.ts", "utf-8");
  const media = fs.readFileSync("src/useMediaQuery.ts", "utf-8");
  const debounce = fs.readFileSync("src/useDebounce.ts", "utf-8");

  // Test 1: useLocalStorage reads from localStorage
  const hasGetItem = /localStorage\.getItem/.test(storage);
  const hasLazyInit = /useState\s*\(\s*\(\s*\)\s*=>/.test(storage);
  check(hasGetItem && hasLazyInit, "useLocalStorage reads localStorage with lazy initializer");

  // Test 2: useLocalStorage writes to localStorage
  const hasSetItem = /localStorage\.setItem/.test(storage);
  const hasStringify = /JSON\.stringify/.test(storage);
  check(hasSetItem && hasStringify, "useLocalStorage writes to localStorage with JSON.stringify");

  // Test 3: useMediaQuery subscribes to matchMedia changes
  const hasMatchMedia = /matchMedia/.test(media);
  const hasAddListener = /addEventListener.*change/.test(media);
  const hasRemoveListener = /removeEventListener/.test(media);
  check(hasMatchMedia && hasAddListener && hasRemoveListener, "useMediaQuery subscribes and cleans up matchMedia listener");

  // Test 4: useDebounce uses setTimeout with cleanup
  const hasSetTimeout = /setTimeout/.test(debounce);
  const hasClearTimeout = /clearTimeout/.test(debounce);
  check(hasSetTimeout && hasClearTimeout, "useDebounce uses setTimeout with clearTimeout cleanup");

  // Test 5: All hooks are generic where applicable
  const storageGeneric = /useLocalStorage\s*<\s*T\s*>/.test(storage);
  const debounceGeneric = /useDebounce\s*<\s*T\s*>/.test(debounce);
  check(storageGeneric && debounceGeneric, "useLocalStorage and useDebounce are generic <T>");

  // Test 6: All hooks exported as named exports
  const storageExport = /export\s+function\s+useLocalStorage/.test(storage);
  const mediaExport = /export\s+function\s+useMediaQuery/.test(media);
  const debounceExport = /export\s+function\s+useDebounce/.test(debounce);
  check(storageExport && mediaExport && debounceExport, "All hooks are named exports");

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
