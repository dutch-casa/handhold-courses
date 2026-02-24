const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const provider = fs.readFileSync("src/ThemeProvider.tsx", "utf-8");
  const app = fs.readFileSync("src/App.tsx", "utf-8");

  // Test 1: Context provides theme value
  const hasCreateContext = /createContext/.test(provider);
  const hasThemeType = /["']light["']\s*\|\s*["']dark["']/.test(provider);
  check(hasCreateContext && hasThemeType, "ThemeContext created with light | dark type");

  // Test 2: useTheme hook with use() and error guard
  const hasUseTheme = /function\s+useTheme|const\s+useTheme/.test(provider);
  const hasUseCall = /use\s*\(\s*ThemeCtx\s*\)/.test(provider);
  const hasThrow = /throw/.test(provider);
  check(hasUseTheme && hasUseCall && hasThrow, "useTheme reads context with use() and throws if missing");

  // Test 3: Toggle switches between light and dark
  const hasToggle = /toggleTheme/.test(provider);
  const hasSetter = /setTheme/.test(provider);
  check(hasToggle && hasSetter, "toggleTheme function toggles theme state");

  // Test 4: localStorage persistence
  const readsStorage = /localStorage\.getItem/.test(provider);
  const writesStorage = /localStorage\.setItem/.test(provider);
  check(readsStorage && writesStorage, "Theme persists via localStorage read and write");

  // Test 5: System preference detection
  const hasMatchMedia = /matchMedia.*prefers-color-scheme/.test(provider);
  check(hasMatchMedia, "Detects system preference with matchMedia");

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
