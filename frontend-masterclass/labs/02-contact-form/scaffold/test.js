const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const html = fs.readFileSync("index.html", "utf-8");

  // Test 1: Form with action and method
  const formMatch = html.match(/<form[^>]*action=["']\/contact["'][^>]*method=["']POST["'][^>]*>/i)
    || html.match(/<form[^>]*method=["']POST["'][^>]*action=["']\/contact["'][^>]*>/i);
  check(!!formMatch, "form has action='/contact' and method='POST'");

  // Test 2: Labeled name input (required)
  const hasNameInput = /<input[^>]*type=["']text["'][^>]*name=["']name["'][^>]*>/i.test(html)
    || /<input[^>]*name=["']name["'][^>]*type=["']text["'][^>]*>/i.test(html)
    || /<input[^>]*name=["']name["'][^>]*>/i.test(html);
  const hasNameLabel = /<label[^>]*for=["'][^"']*["'][^>]*>[^<]*name[^<]*<\/label>/i.test(html)
    || /<label[^>]*>[^<]*name[^<]*<input/i.test(html);
  check(hasNameInput && hasNameLabel, "has a labeled text input for name");

  // Test 3: Email input with type="email"
  const hasEmailInput = /<input[^>]*type=["']email["'][^>]*>/i.test(html);
  check(hasEmailInput, "has an input with type='email'");

  // Test 4: Textarea exists
  const hasTextarea = /<textarea[^>]*>/i.test(html);
  const hasTextareaLabel = /<label[^>]*>[^<]*(message|comment|body)[^<]*<\/label>/i.test(html)
    || /<label[^>]*for=["'][^"']*["'][^>]*>[^<]*(message|comment|body)[^<]*<\/label>/i.test(html);
  check(hasTextarea, "has a textarea for the message");

  // Test 5: Select with at least 3 options
  const selectMatch = html.match(/<select[^>]*>([\s\S]*?)<\/select>/i);
  const optionCount = selectMatch
    ? (selectMatch[1].match(/<option[^>]*>/gi) || []).length
    : 0;
  check(optionCount >= 3, "has a select dropdown with at least 3 options");

  // Test 6: Submit button
  const hasSubmitBtn = /<button[^>]*type=["']submit["'][^>]*>[\s\S]*?send\s*message[\s\S]*?<\/button>/i.test(html)
    || /<button[^>]*>[\s\S]*?send\s*message[\s\S]*?<\/button>/i.test(html)
    || /<input[^>]*type=["']submit["'][^>]*value=["'][^"']*send\s*message[^"']*["']/i.test(html);
  check(hasSubmitBtn, "has a submit button with text 'Send Message'");

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
