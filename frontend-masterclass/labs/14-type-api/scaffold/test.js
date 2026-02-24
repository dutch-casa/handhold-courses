const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const ts = fs.readFileSync("types.ts", "utf-8");

  // Test 1: User type with readonly id
  const hasUser = /type\s+User\s*=|interface\s+User/.test(ts);
  const hasReadonlyId = /readonly\s+id\s*:\s*number/.test(ts);
  check(hasUser && hasReadonlyId, "User type defined with readonly id");

  // Test 2: RequestState discriminated union with generic
  const hasRequestState = /type\s+RequestState\s*</.test(ts);
  const hasKinds = /kind.*idle/.test(ts) && /kind.*loading/.test(ts)
    && /kind.*success/.test(ts) && /kind.*error/.test(ts);
  check(hasRequestState && hasKinds, "RequestState<T> discriminated union with all four kinds");

  // Test 3: PaginatedResponse generic
  const hasPaginated = /type\s+PaginatedResponse\s*<|interface\s+PaginatedResponse\s*</.test(ts);
  const hasItems = /readonly\s+items\s*:\s*readonly/.test(ts);
  check(hasPaginated && hasItems, "PaginatedResponse<T> with readonly items array");

  // Test 4: UserUpdate with Partial and Omit
  const hasUserUpdate = /type\s+UserUpdate/.test(ts);
  const usesPartialOmit = /Partial.*Omit|Omit.*Partial/.test(ts);
  check(hasUserUpdate && usesPartialOmit, "UserUpdate type uses Partial and Omit");

  // Test 5: Readonly used throughout
  const readonlyCount = (ts.match(/readonly/g) || []).length;
  check(readonlyCount >= 5, "domain types use readonly extensively (5+ usages)");

  // Test 6: Exhaustive handler function
  const hasRenderState = /function\s+renderState/.test(ts);
  const hasSwitch = /switch\s*\(\s*state\.kind\s*\)/.test(ts);
  check(hasRenderState && hasSwitch, "renderState function handles RequestState exhaustively");

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
