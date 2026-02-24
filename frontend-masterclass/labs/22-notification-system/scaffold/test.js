const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const tsx = fs.readFileSync("src/Notification.tsx", "utf-8");
  const css = fs.readFileSync("src/notifications.css", "utf-8");

  // Test 1: Notification has role="alert" and aria-live
  const hasRole = /role\s*=\s*["']alert["']/.test(tsx);
  const hasAriaLive = /aria-live\s*=\s*["']polite["']/.test(tsx);
  check(hasRole && hasAriaLive, "Notification has role='alert' and aria-live='polite'");

  // Test 2: Enter animation with translateX and opacity
  const hasKeyframes = /@keyframes\s+slideIn/.test(css);
  const hasTranslateX = /translateX/.test(css);
  const hasOpacity = /opacity/.test(css);
  check(hasKeyframes && hasTranslateX && hasOpacity, "CSS defines slideIn keyframes with translateX and opacity");

  // Test 3: Auto-dismiss with setTimeout and cleanup
  const hasSetTimeout = /setTimeout/.test(tsx);
  const hasClearTimeout = /clearTimeout/.test(tsx);
  check(hasSetTimeout && hasClearTimeout, "Auto-dismiss uses setTimeout with clearTimeout cleanup");

  // Test 4: Manual dismiss button with aria-label
  const hasDismissLabel = /aria-label\s*=\s*["']Dismiss["']/.test(tsx);
  check(hasDismissLabel, "Close button has aria-label='Dismiss'");

  // Test 5: Stack renders multiple notifications
  const hasStack = /NotificationStack/.test(tsx);
  const hasArrayState = /useState.*\[\]/.test(tsx);
  check(hasStack && hasArrayState, "NotificationStack manages array of notifications");

  // Test 6: Reduced motion support
  const hasReducedMotion = /prefers-reduced-motion/.test(css);
  check(hasReducedMotion, "CSS includes prefers-reduced-motion media query");

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
