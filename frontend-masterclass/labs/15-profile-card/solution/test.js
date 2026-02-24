const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const card = fs.readFileSync("src/ProfileCard.tsx", "utf-8");
  const app = fs.readFileSync("src/App.tsx", "utf-8");

  // Test 1: ProfileCardProps interface with readonly fields
  const hasInterface = /interface\s+ProfileCardProps/.test(card);
  const hasReadonlyName = /readonly\s+name\s*:\s*string/.test(card);
  check(hasInterface && hasReadonlyName, "ProfileCardProps interface defined with readonly name");

  // Test 2: Component renders name in an h2
  const hasH2Name = /<h2[^>]*>\s*\{?\s*name\s*\}?\s*<\/h2>/.test(card);
  check(hasH2Name, "Component renders name in an <h2>");

  // Test 3: Avatar img with alt text
  const hasImg = /<img[^>]*src\s*=\s*\{?\s*avatarUrl\s*\}?/.test(card);
  const hasAlt = /<img[^>]*alt\s*=\s*\{?\s*name\s*\}?/.test(card);
  check(hasImg && hasAlt, "Renders avatar <img> with src and alt={name}");

  // Test 4: Bio rendered in a <p>
  const hasBioP = /<p[^>]*>\s*\{?\s*bio\s*\}?\s*<\/p>/.test(card);
  check(hasBioP, "Renders bio in a <p>");

  // Test 5: Skills mapped to <li> with key
  const hasMap = /skills\.map/.test(card);
  const hasLiKey = /<li\s+key\s*=/.test(card);
  check(hasMap && hasLiKey, "Skills mapped to <li> elements with key prop");

  // Test 6: App.tsx uses ProfileCard with props
  const importsCard = /import.*ProfileCard.*from/.test(app);
  const rendersCard = /<ProfileCard[\s\S]*name\s*=/.test(app);
  check(importsCard && rendersCard, "App.tsx imports and renders ProfileCard with props");

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
