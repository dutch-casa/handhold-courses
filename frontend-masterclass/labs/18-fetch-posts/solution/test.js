const fs = require("fs");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const app = fs.readFileSync("src/App.tsx", "utf-8");
  const list = fs.readFileSync("src/PostList.tsx", "utf-8");
  const detail = fs.readFileSync("src/PostDetail.tsx", "utf-8");

  // Test 1: QueryClientProvider set up in App
  const hasQueryClient = /new\s+QueryClient/.test(app);
  const hasProvider = /QueryClientProvider/.test(app);
  check(hasQueryClient && hasProvider, "App sets up QueryClient and QueryClientProvider");

  // Test 2: PostList uses useQuery with correct key
  const listUsesQuery = /useQuery/.test(list);
  const hasPostsKey = /["']posts["']/.test(list);
  check(listUsesQuery && hasPostsKey, "PostList uses useQuery with 'posts' key");

  // Test 3: PostList shows loading state
  const hasPending = /isPending/.test(list);
  const hasLoadingText = /Loading/.test(list);
  check(hasPending && hasLoadingText, "PostList shows loading state with isPending");

  // Test 4: PostDetail uses useQuery with parameterized key
  const detailUsesQuery = /useQuery/.test(detail);
  const hasPostKey = /["']post["']\s*,\s*postId/.test(detail);
  check(detailUsesQuery && hasPostKey, "PostDetail uses useQuery with ['post', postId] key");

  // Test 5: PostDetail has back button calling onBack
  const hasBackButton = /<button[^>]*onClick[^>]*onBack/.test(detail) || /onClick\s*=\s*\{[^}]*onBack/.test(detail);
  check(hasBackButton, "PostDetail has back button that calls onBack");

  // Test 6: Stale time configured for caching
  const hasStaleTime = /staleTime/.test(app);
  check(hasStaleTime, "QueryClient configured with staleTime for caching");

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
