const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const css = fs.readFileSync("src/card.css", "utf-8");
  const tsx = fs.readFileSync("src/Card.tsx", "utf-8");

  // Test 1: Multiple box-shadow layers
  const shadowMatches = css.match(/box-shadow\s*:[^;]*/g) || [];
  const hasLayered = shadowMatches.some((s) => (s.match(/,/g) || []).length >= 1);
  check(hasLayered, "Card has layered box-shadow (2+ shadow layers)");

  // Test 2: Hover with translateY (not transition: all)
  const hasHoverHover = /hover:\s*hover/.test(css);
  const hasTranslateY = /translateY/.test(css);
  const noTransitionAll = !/transition\s*:\s*all/.test(css);
  check(hasHoverHover && hasTranslateY && noTransitionAll, "Hover lift with translateY inside @media (hover: hover), no transition: all");

  // Test 3: Dark mode with prefers-color-scheme
  const hasDarkMode = /prefers-color-scheme\s*:\s*dark/.test(css);
  check(hasDarkMode, "Dark mode styles with prefers-color-scheme: dark");

  // Test 4: Loading skeleton with pulse animation
  const hasSkeleton = /CardSkeleton|skeleton/.test(tsx);
  const hasPulse = /@keyframes\s+pulse/.test(css);
  check(hasSkeleton && hasPulse, "CardSkeleton component with pulse animation");

  // Test 5: Tabular nums for data values
  const hasTabular = /tabular-nums/.test(css);
  check(hasTabular, "Typography uses font-variant-numeric: tabular-nums");

  // Test 6: Press feedback with scale on :active
  const hasActive = /:active/.test(css);
  const hasScale = /scale\s*\(\s*0\.9/.test(css);
  check(hasActive && hasScale, "Press feedback with scale on :active");

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
