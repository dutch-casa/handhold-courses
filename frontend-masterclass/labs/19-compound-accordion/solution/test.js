const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const accordion = fs.readFileSync("src/Accordion.tsx", "utf-8");
  const app = fs.readFileSync("src/App.tsx", "utf-8");

  // Test 1: Context created with createContext
  const hasCreateContext = /createContext/.test(accordion);
  const hasContextType = /AccordionContext/.test(accordion);
  check(hasCreateContext && hasContextType, "AccordionContext created with createContext");

  // Test 2: useAccordion hook reads context
  const hasUseAccordion = /function\s+useAccordion|const\s+useAccordion/.test(accordion);
  const readsContext = /use\s*\(\s*AccordionCtx\s*\)/.test(accordion);
  check(hasUseAccordion && readsContext, "useAccordion hook reads context with use()");

  // Test 3: Toggle function implements single-open behavior
  const hasToggle = /toggle/.test(accordion);
  const hasTernary = /prev\s*===\s*id\s*\?\s*null\s*:\s*id/.test(accordion);
  check(hasToggle && hasTernary, "Toggle implements single-open (prev === id ? null : id)");

  // Test 4: Compound parts attached to namespace
  const hasItemAttach = /Accordion\.Item\s*=/.test(accordion);
  const hasTriggerAttach = /Accordion\.Trigger\s*=/.test(accordion);
  const hasContentAttach = /Accordion\.Content\s*=/.test(accordion);
  check(hasItemAttach && hasTriggerAttach && hasContentAttach, "Item, Trigger, Content attached to Accordion namespace");

  // Test 5: Content conditionally renders based on openId
  const checksOpenId = /openId\s*!==\s*id/.test(accordion) || /openId\s*===\s*id/.test(accordion);
  const returnsNull = /return\s+null/.test(accordion);
  check(checksOpenId && returnsNull, "Content conditionally renders based on openId match");

  // Test 6: App uses compound pattern with at least 3 items
  const usesAccordion = /Accordion\.Item/.test(app);
  const itemCount = (app.match(/Accordion\.Item/g) || []).length;
  check(usesAccordion && itemCount >= 3, "App renders Accordion with 3+ compound items");

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
