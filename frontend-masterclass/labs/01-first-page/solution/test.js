const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  // Read the HTML file
  const html = fs.readFileSync("index.html", "utf-8");

  // Test 1: Page has an h1
  check(/<h1[^>]*>.+<\/h1>/is.test(html), "page has an h1 with content");

  // Test 2: Page has an img with alt attribute
  const hasImg = /<img[^>]+src=["'][^"']+["'][^>]*>/i.test(html);
  const hasAlt = /<img[^>]+alt=["'][^"']+["'][^>]*>/i.test(html);
  check(hasImg && hasAlt, "page has an img with both src and alt attributes");

  // Test 3: Page has a ul with at least 3 li items
  const ulMatch = html.match(/<ul[^>]*>([\s\S]*?)<\/ul>/i);
  const liCount = ulMatch
    ? (ulMatch[1].match(/<li[^>]*>/gi) || []).length
    : 0;
  check(liCount >= 3, "page has a ul with at least 3 li items");

  // Test 4: Page has a link (a tag with href)
  check(/<a[^>]+href=["'][^"']+["'][^>]*>.+<\/a>/is.test(html), "page has a link with href and text");

  // Test 5: Page has a paragraph
  check(/<p[^>]*>.+<\/p>/is.test(html), "page has a paragraph with content");

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
