const fs = require("fs");
const path = require("path");
const { JSDOM } = require("jsdom");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  // Load the HTML and execute the JS in a simulated DOM
  const htmlPath = path.resolve("index.html");
  const html = fs.readFileSync(htmlPath, "utf-8");
  const dom = new JSDOM(html, {
    runScripts: "dangerously",
    resources: "usable",
    url: "file://" + htmlPath,
  });
  const doc = dom.window.document;

  // Give external scripts time to load and execute
  await new Promise((resolve) => setTimeout(resolve, 500));

  // Test 1: titleText should contain the h1 text
  const appJs = fs.readFileSync("app.js", "utf-8");
  const hasTitleQuery = appJs.includes("getElementById") || appJs.includes("querySelector");
  check(hasTitleQuery, "titleText is read from the DOM (uses getElementById or querySelector)");

  // Test 2: paragraphCount should be the correct number
  const hasCountQuery = appJs.includes("querySelectorAll") || appJs.includes("getElementsByTagName");
  check(hasCountQuery, "paragraphCount uses querySelectorAll or getElementsByTagName");

  // Test 3: featured element should have highlight class
  const featured = doc.getElementById("featured");
  const hasHighlight = featured ? featured.classList.contains("highlight") : false;
  check(hasHighlight, "element with id='featured' has class 'highlight'");

  // Test 4: A new paragraph with "Added by JavaScript" should be in main
  const main = doc.querySelector("main");
  const allP = main ? main.querySelectorAll("p") : [];
  let foundAdded = false;
  for (const p of allP) {
    if (p.textContent.includes("Added by JavaScript")) {
      foundAdded = true;
      break;
    }
  }
  check(foundAdded, "main contains a new paragraph with text 'Added by JavaScript'");

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
