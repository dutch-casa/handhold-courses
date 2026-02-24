const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const tsx = fs.readFileSync("src/RegistrationForm.tsx", "utf-8");
  const css = fs.readFileSync("src/form.css", "utf-8");

  // Test 1: Inputs are 16px+ (prevents iOS zoom)
  const hasFontSize = /font-size\s*:\s*1rem/.test(css) || /font-size\s*:\s*16px/.test(css);
  check(hasFontSize, "Inputs have font-size: 1rem to prevent iOS zoom");

  // Test 2: Labels associated with htmlFor and id
  const hasHtmlFor = /htmlFor\s*=/.test(tsx);
  const hasId = /id\s*=\s*["']/.test(tsx);
  check(hasHtmlFor && hasId, "Labels associated via htmlFor and id");

  // Test 3: Errors use aria-invalid
  const hasAriaInvalid = /aria-invalid/.test(tsx);
  check(hasAriaInvalid, "Invalid inputs marked with aria-invalid");

  // Test 4: Error messages use aria-describedby
  const hasDescribedBy = /aria-describedby/.test(tsx);
  const hasRoleAlert = /role\s*=\s*["']alert["']/.test(tsx);
  check(hasDescribedBy && hasRoleAlert, "Error messages use aria-describedby and role='alert'");

  // Test 5: Form submits with fieldset disabled during loading
  const hasFieldset = /fieldset/.test(tsx);
  const hasDisabled = /disabled\s*=\s*\{/.test(tsx);
  check(hasFieldset && hasDisabled, "Form uses <fieldset disabled> for loading state");

  // Test 6: Password strength updates on change
  const hasStrength = /strength|strong|weak|medium/i.test(tsx);
  const hasLengthCheck = /\.length\s*>=?\s*8/.test(tsx);
  check(hasStrength && hasLengthCheck, "Password strength indicator checks length");

  // Test 7: Blur validation (onBlur handler)
  const hasOnBlur = /onBlur/.test(tsx);
  const hasTouched = /touched/.test(tsx);
  check(hasOnBlur && hasTouched, "Validates on blur with touched tracking");

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
