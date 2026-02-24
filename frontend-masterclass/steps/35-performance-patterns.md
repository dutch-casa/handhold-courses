---
title: Performance patterns
---

# Why performance matters

{{show: perf-diagram reveal 0.3s spring}} {{show: perf-code typewriter 2s linear}} Users notice delays above 100ms. Above 300ms, they feel the app is slow. Above 1 second, they lose focus. Performance isn't about benchmarks — it's about human perception thresholds.

{{focus: thresholds}} {{zoom: 1.2x}} {{pulse: thresholds}} {{annotate: thresholds "Perception thresholds"}} 0-100ms: feels instant. 100-300ms: noticeable delay. 300-1000ms: feels slow but tolerable. 1000ms+: user leaves or gets frustrated. Every operation in your app should target the first two buckets.

{{zoom: 1x}} {{focus: principle}} {{zoom: 1.2x}} {{annotate: principle "Do less work"}} The fastest code is code that doesn't run. Virtualization renders 20 items instead of 10,000. Lazy loading loads the code when needed, not at startup. Memoization skips re-rendering when props haven't changed. Every performance pattern is a way to do less.

{{zoom: 1x}} {{focus: none}}

```code:perf-code lang=tsx
// Performance is about perception thresholds
//
// 0-100ms    → Instant (clicks, hovers)
// 100-300ms  → Slight delay (transitions)
// 300-1000ms → Slow (page loads, searches)
// 1000ms+    → Broken (user leaves)
//
// The goal: keep every interaction under 100ms.
// If it takes longer, show a loading state.
// Never leave the user staring at a frozen screen.

const THRESHOLDS = {
  instant: 100,
  noticeable: 300,
  slow: 1000,
  broken: 3000,
} as const;
---
thresholds: 3-9
principle: 12-16
```

```diagram:perf-diagram kind=graph
Action [label="User action" shape=box]
Under100 [label="< 100ms\nInstant" shape=box]
Under300 [label="100-300ms\nShow result" shape=box]
Under1000 [label="300ms-1s\nShow spinner" shape=box]
Over1000 [label="> 1s\nShow skeleton" shape=box]

Action -> Under100
Action -> Under300
Action -> Under1000
Action -> Over1000
```

# Memoization

{{clear: slide}}

{{show: memo-code typewriter 2s linear}} `React.memo` skips re-rendering when props haven't changed. `useMemo` caches a computed value. `useCallback` caches a function reference. All three prevent unnecessary work — but only when the work is actually expensive.

{{focus: memo}} {{zoom: 1.2x}} {{pulse: memo}} {{annotate: memo "React.memo"}} Wrap a component in `React.memo` and it only re-renders when its props change (shallow comparison). A parent re-renders? The memoized child skips if its props are identical. Use this for components that receive the same props most of the time.

{{zoom: 1x}} {{focus: usememo}} {{zoom: 1.2x}} {{annotate: usememo "useMemo"}} `useMemo(() => expensiveComputation(data), [data])` — only recompute when `data` changes. Use for expensive array transformations, complex calculations, or derived objects that would otherwise trigger child re-renders.

{{zoom: 1x}} {{focus: when-not}} {{zoom: 1.2x}} {{pulse: when-not}} {{pan: when-not}} {{annotate: when-not "When NOT to memo"}} Don't memoize everything. Memoization has overhead — React checks if deps changed on every render. For simple primitives like `count + 1`, the check costs more than the computation. Only memoize when you've measured a performance problem.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:memo-code lang=tsx
import { memo, useMemo, useCallback } from "react";

// React.memo: skip re-render when props haven't changed
const ExpensiveList = memo(function ExpensiveList({
  items,
}: {
  readonly items: readonly Item[];
}) {
  return (
    <ul>
      {items.map((item) => (
        <li key={item.id}>{item.name}</li>
      ))}
    </ul>
  );
});

// useMemo: cache expensive computation
function Dashboard({ transactions }: { readonly transactions: readonly Transaction[] }) {
  const stats = useMemo(() => ({
    total: transactions.reduce((sum, t) => sum + t.amount, 0),
    average: transactions.reduce((sum, t) => sum + t.amount, 0) / transactions.length,
    max: Math.max(...transactions.map((t) => t.amount)),
  }), [transactions]);

  return <StatsDisplay stats={stats} />;
}

// DON'T: memoizing cheap operations
function Bad() {
  const doubled = useMemo(() => count * 2, [count]); // WRONG: cheaper than the memo
  return <p>{doubled}</p>;
}

// DO: just compute it
function Good({ count }: { readonly count: number }) {
  const doubled = count * 2; // simple math, no memo needed
  return <p>{doubled}</p>;
}
---
memo: 4-15
usememo: 19-25
when-not: 29-37
```

# Lazy loading

{{clear: slide}}

{{show: lazy-code typewriter 2s linear}} `React.lazy` + `Suspense` split your app into chunks. The initial bundle only contains the code for the first screen. Other screens load on demand — when the user navigates to them.

{{focus: lazy}} {{zoom: 1.2x}} {{pulse: lazy}} {{annotate: lazy "Dynamic import"}} `React.lazy(() => import("./Settings"))` — React loads the Settings module only when the component renders for the first time. The bundler (Vite, webpack) creates a separate chunk file. The browser downloads it on demand.

{{zoom: 1x}} {{focus: suspense}} {{zoom: 1.2x}} {{annotate: suspense "Suspense boundary"}} `<Suspense fallback={<Spinner />}>` — while the lazy component loads, React shows the fallback. Once loaded, it swaps in the component. The loading state is declarative, not imperative.

{{zoom: 1x}} {{focus: routes}} {{zoom: 1.2x}} {{pulse: routes}} {{pan: routes}} {{annotate: routes "Route-level splitting"}} The best split point is routes. Each page is a separate chunk. The homepage loads instantly. The settings page loads when the user clicks "Settings." The admin dashboard loads when an admin logs in. Users only download the code they use.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:lazy-code lang=tsx
import { lazy, Suspense } from "react";

// Lazy-load route components
const Home = lazy(() => import("./pages/Home"));
const Settings = lazy(() => import("./pages/Settings"));
const Admin = lazy(() => import("./pages/Admin"));

function App() {
  return (
    <Suspense fallback={<PageSkeleton />}>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/settings" element={<Settings />} />
        <Route path="/admin" element={<Admin />} />
      </Routes>
    </Suspense>
  );
}

// Lazy-load heavy components
const Chart = lazy(() => import("./Chart"));
const CodeEditor = lazy(() => import("./CodeEditor"));

function Dashboard() {
  return (
    <div>
      <h1>Dashboard</h1>
      <Suspense fallback={<ChartSkeleton />}>
        <Chart data={chartData} />
      </Suspense>
      <Suspense fallback={<EditorSkeleton />}>
        <CodeEditor />
      </Suspense>
    </div>
  );
}
---
lazy: 4-6
suspense: 10-17
routes: 4-17
```

# Virtualization

{{clear: slide}}

{{show: virtual-code typewriter 2s linear}} {{show: virtual-diagram reveal 0.3s spring}} A list of 10,000 items means 10,000 DOM nodes. Each one consumes memory, participates in layout, and slows every scroll frame. Virtualization renders only what's visible — typically 20-30 items — and swaps them as the user scrolls.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "DOM bloat"}} 10,000 `<li>` elements. Each is ~1KB of DOM. That's 10MB of DOM nodes. The browser has to layout all of them. Scroll handlers fire on all of them. Painting touches all of them. The page crawls.

{{zoom: 1x}} {{focus: virtual}} {{zoom: 1.2x}} {{annotate: virtual "Window rendering"}} Virtualization keeps a "window" of visible items. Scroll down? The items that scroll out of view are recycled for items scrolling in. The DOM always has ~30 elements. 10,000 items or 1,000,000 items — the performance is the same.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{pulse: usage}} {{pan: usage}} {{annotate: usage "TanStack Virtual"}} `@tanstack/react-virtual` is the go-to library. Give it the total count and row height. It tells you which items are visible. You render only those. The container has a spacer element to maintain the correct scroll height.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:virtual-code lang=tsx
import { useVirtualizer } from "@tanstack/react-virtual";

function VirtualList({ items }: { readonly items: readonly Item[] }) {
  const parentRef = useRef<HTMLDivElement>(null);

  const virtualizer = useVirtualizer({
    count: items.length,
    getScrollElement: () => parentRef.current,
    estimateSize: () => 48,
  });

  return (
    <div ref={parentRef} style={{ height: "400px", overflow: "auto" }}>
      <div style={{ height: `${virtualizer.getTotalSize()}px`, position: "relative" }}>
        {virtualizer.getVirtualItems().map((virtualRow) => {
          const item = items[virtualRow.index];
          if (!item) return null;
          return (
            <div
              key={virtualRow.key}
              style={{
                position: "absolute",
                top: 0,
                left: 0,
                width: "100%",
                height: `${virtualRow.size}px`,
                transform: `translateY(${virtualRow.start}px)`,
              }}
            >
              {item.name}
            </div>
          );
        })}
      </div>
    </div>
  );
}

// Without virtualization: 10,000 DOM nodes
// With virtualization: ~30 DOM nodes (only visible items)
---
problem: 3-9
virtual: 15-32
usage: 6-10
```

```diagram:virtual-diagram kind=graph
AllItems [label="10,000 items\nin data" shape=box]
Window [label="Virtualization\nwindow" shape=diamond]
Visible [label="~30 visible\nDOM nodes" shape=box]
Recycled [label="Scroll → recycle\nnodes" shape=box]

AllItems -> Window
Window -> Visible
Visible -> Recycled
Recycled -> Visible
```

# Optimistic updates

{{clear: slide}}

{{show: optimistic-code typewriter 2s linear}} The server takes 200-500ms to respond. The user clicked "complete" on a task. Do they wait? No. Update the UI immediately. When the server confirms, the UI is already correct. If the server rejects, roll back.

{{focus: instant}} {{zoom: 1.2x}} {{pulse: instant}} {{annotate: instant "Instant feedback"}} The user clicks. The checkbox fills. The item moves to "completed." This happened in 0ms from the user's perspective. The server request is in flight, but the user doesn't know or care. The UI feels instant.

{{zoom: 1x}} {{focus: rollback}} {{zoom: 1.2x}} {{annotate: rollback "Rollback on error"}} If the server rejects (network error, permission denied), revert the UI to the previous state. The user sees a brief flash — the checkbox unchecks, the item moves back. Show an error toast. This is rare, and the user understands what happened.

{{zoom: 1x}} {{focus: pattern}} {{zoom: 1.2x}} {{pulse: pattern}} {{pan: pattern}} {{annotate: pattern "The pattern"}} `onMutate`: save the old cache, update the cache optimistically. `onError`: restore the old cache. `onSettled`: refetch to ensure server and client agree. Three callbacks, instant UI, automatic recovery.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:optimistic-code lang=tsx
const toggleTodo = useMutation({
  mutationFn: (id: number) =>
    fetch(`/api/todos/${id}/toggle`, { method: "PATCH" })
      .then((res) => res.json()),

  onMutate: async (id) => {
    // Cancel in-flight queries
    await queryClient.cancelQueries({ queryKey: ["todos"] });

    // Snapshot the old value
    const previous = queryClient.getQueryData<Todo[]>(["todos"]);

    // Optimistically update
    queryClient.setQueryData<Todo[]>(["todos"], (old) =>
      (old ?? []).map((t) =>
        t.id === id ? { ...t, completed: !t.completed } : t,
      ),
    );

    return { previous };
  },

  onError: (_err, _id, context) => {
    // Roll back on failure
    if (context?.previous) {
      queryClient.setQueryData(["todos"], context.previous);
    }
  },

  onSettled: () => {
    // Refetch to ensure consistency
    queryClient.invalidateQueries({ queryKey: ["todos"] });
  },
});

// Usage: feels instant
<button onClick={() => toggleTodo.mutate(todo.id)}>
  {todo.completed ? "✓" : "○"}
</button>
---
instant: 6-18
rollback: 22-28
pattern: 6-33
```
