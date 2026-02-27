const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const app = fs.readFileSync("src/App.tsx", "utf-8");
  const taskList = fs.readFileSync("src/TaskList.tsx", "utf-8");
  const addTask = fs.readFileSync("src/AddTask.tsx", "utf-8");
  const api = fs.readFileSync("src/api.ts", "utf-8");
  const css = fs.readFileSync("src/styles.css", "utf-8");

  // Test 1: React Query setup
  const hasQueryClient = /new\s+QueryClient/.test(app);
  const hasProvider = /QueryClientProvider/.test(app);
  check(hasQueryClient && hasProvider, "App sets up QueryClient and QueryClientProvider");

  // Test 2: Task list uses useQuery
  const hasUseQuery = /useQuery/.test(taskList);
  const hasTasksKey = /["']tasks["']/.test(taskList);
  check(hasUseQuery && hasTasksKey, "TaskList fetches tasks with useQuery");

  // Test 3: Add task uses useMutation
  const hasMutation = /useMutation/.test(addTask);
  const hasInvalidate = /invalidateQueries/.test(addTask);
  check(hasMutation && hasInvalidate, "AddTask uses useMutation with cache invalidation");

  // Test 4: Toggle with optimistic update
  const hasOnMutate = /onMutate/.test(taskList);
  const hasOnError = /onError/.test(taskList);
  const hasSetQueryData = /setQueryData/.test(taskList);
  check(hasOnMutate && hasOnError && hasSetQueryData, "Toggle uses optimistic update (onMutate + onError + setQueryData)");

  // Test 5: Filter derives displayed list (not stored in state)
  const hasFilter = /filter\s*===\s*["']all["']/.test(taskList) || /filter.*completed/.test(taskList);
  const filterNotState = !/useState.*filtered/.test(taskList);
  check(hasFilter && filterNotState, "Filter derives displayed list during render");

  // Test 6: CSS enter animation with transform and opacity
  const hasKeyframes = /@keyframes/.test(css);
  const hasTransform = /translateY/.test(css);
  const hasOpacity = /opacity/.test(css);
  check(hasKeyframes && hasTransform && hasOpacity, "CSS enter animation uses transform and opacity");

  // Test 7: Dark mode support
  const hasDarkMode = /prefers-color-scheme\s*:\s*dark/.test(css);
  check(hasDarkMode, "CSS supports dark mode with prefers-color-scheme");

  // Test 8: Keyboard accessible (aria-label on delete)
  const hasDeleteLabel = /aria-label/.test(taskList);
  check(hasDeleteLabel, "Delete button has aria-label for accessibility");

  // Test 9: Reduced motion support
  const hasReducedMotion = /prefers-reduced-motion/.test(css);
  check(hasReducedMotion, "CSS respects prefers-reduced-motion");

  // Test 10: Hover effects behind @media (hover: hover)
  const hasHoverMedia = /hover:\s*hover/.test(css);
  check(hasHoverMedia, "Hover effects guarded by @media (hover: hover)");

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
