const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const counter = fs.readFileSync("src/Counter.tsx", "utf-8");
  const form = fs.readFileSync("src/ContactForm.tsx", "utf-8");

  // Test 1: Counter uses functional updates
  const hasFunctionalUpdate = /setCount\s*\(\s*\(?\s*prev/.test(counter);
  check(hasFunctionalUpdate, "Counter uses functional updates (prev => ...)");

  // Test 2: Counter has increment, decrement, reset
  const hasIncrement = /prev\s*\+\s*1/.test(counter) || /prev\s*\+\s*1/.test(counter);
  const hasDecrement = /prev\s*-\s*1/.test(counter);
  const hasReset = /setCount\s*\(\s*0\s*\)/.test(counter);
  check(hasIncrement && hasDecrement && hasReset, "Counter has increment, decrement, and reset");

  // Test 3: Character count is derived (not a separate useState)
  const useStateCount = (counter.match(/useState/g) || []).length;
  const hasLengthDerivation = /\.length/.test(counter);
  check(useStateCount <= 2 && hasLengthDerivation, "Character count derived from text.length, not stored in state");

  // Test 4: Form has controlled name and email inputs
  const hasNameState = /useState.*["']{2}/.test(form);
  const hasControlledInput = /value\s*=\s*\{/.test(form) && /onChange/.test(form);
  check(hasNameState && hasControlledInput, "Form uses controlled inputs with value and onChange");

  // Test 5: Form validates empty fields
  const hasErrorClass = /className\s*=\s*["']error["']/.test(form);
  const hasErrorMessage = /All fields required/.test(form);
  check(hasErrorClass && hasErrorMessage, "Form shows error message for empty fields");

  // Test 6: Form shows success on valid submit
  const hasSuccessClass = /className\s*=\s*["']success["']/.test(form);
  const hasSuccessMessage = /Submitted!/.test(form);
  const hasPreventDefault = /preventDefault/.test(form);
  check(hasSuccessClass && hasSuccessMessage && hasPreventDefault, "Form shows success message and prevents default submit");

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
