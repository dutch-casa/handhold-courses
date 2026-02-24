const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const tsx = fs.readFileSync("src/AnimatedTabs.tsx", "utf-8");

  // Test 1: Imports motion from framer-motion
  const hasMotionImport = /from\s+["']framer-motion["']/.test(tsx);
  const usesMotion = /motion\.div/.test(tsx);
  check(hasMotionImport && usesMotion, "Imports and uses motion from framer-motion");

  // Test 2: AnimatePresence wraps content
  const hasAnimatePresence = /AnimatePresence/.test(tsx);
  check(hasAnimatePresence, "Uses AnimatePresence for enter/exit animations");

  // Test 3: layoutId for tab indicator
  const hasLayoutId = /layoutId/.test(tsx);
  check(hasLayoutId, "Uses layoutId for shared element tab indicator");

  // Test 4: Direction state for directional animation
  const hasDirection = /direction/.test(tsx);
  const tracksDirection = /useState.*0\)/.test(tsx) || /setDirection/.test(tsx);
  check(hasDirection && tracksDirection, "Tracks direction state for directional content animation");

  // Test 5: Reduced motion support
  const hasReducedMotion = /useReducedMotion/.test(tsx);
  check(hasReducedMotion, "Uses useReducedMotion to respect motion preferences");

  // Test 6: Compound component pattern with context
  const hasContext = /createContext/.test(tsx);
  const hasCompound = /Object\.assign/.test(tsx) || /Tabs\.(List|Tab|Panel|Content)\s*=/.test(tsx);
  check(hasContext && hasCompound, "Uses compound component pattern with shared context");

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
