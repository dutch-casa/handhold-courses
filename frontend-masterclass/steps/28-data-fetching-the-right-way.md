---
title: Data fetching the right way
---

# The useEffect trap

{{show: effect-code typewriter 2s linear}} Every React tutorial teaches data fetching with `useEffect`. Fetch inside the effect. Set loading state. Set data state. Set error state. It works — until it doesn't.

{{focus: naive}} {{zoom: 1.2x}} {{pulse: naive}} {{annotate: naive "The naive approach"}} Three states to manage by hand. Loading starts true, flips to false when done. Data starts null. Error starts null. Every fetch needs this same boilerplate. Forget one transition and the UI lies to the user.

{{zoom: 1x}} {{focus: race}} {{zoom: 1.2x}} {{pan: race}} {{annotate: race "Race condition"}} Click user A. Fetch starts. Click user B before A finishes. Fetch for B starts. A's response arrives first — it sets data. B's response arrives second — it overwrites. But what if B's response arrives first? Now A's slower response overwrites B's data. The UI shows user A when user B is selected.

{{pan: none}} {{zoom: 1x}} {{focus: cleanup}} {{zoom: 1.2x}} {{pulse: cleanup}} {{annotate: cleanup "Cleanup band-aid"}} The "fix" is a cleanup flag: `let ignore = false`. Check it before setting state. The cleanup function sets `ignore = true` on unmount or dependency change. This works. But you wrote an entire state machine by hand — loading, error, data, race condition guard — for one fetch.

{{zoom: 1x}} {{focus: none}} And you haven't handled: caching, deduplication, background refetch, retry on failure, optimistic updates, prefetching, or garbage collection. That's not a component. That's a framework.

```code:effect-code lang=tsx
// The useEffect approach — DO NOT DO THIS
function UserProfile({ userId }: { readonly userId: number }) {
  const [user, setUser] = useState<User | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    let ignore = false; // race condition guard

    setIsLoading(true);
    setError(null);

    fetch(`/api/users/${userId}`)
      .then((res) => res.json())
      .then((data) => {
        if (!ignore) {
          setUser(data);
          setIsLoading(false);
        }
      })
      .catch((err) => {
        if (!ignore) {
          setError(err.message);
          setIsLoading(false);
        }
      });

    return () => { ignore = true; };
  }, [userId]);

  if (isLoading) return <Spinner />;
  if (error) return <ErrorMessage message={error} />;
  if (!user) return null;

  return <div>{user.name}</div>;
}
// 35 lines. No caching. No dedup. No retry. No prefetch.
---
naive: 2-5
race: 8-25
cleanup: 8-27
```

# React Query

{{clear: slide}}

{{show: rq-code typewriter 2s linear}} {{show: rq-diagram reveal 0.3s spring}} React Query (TanStack Query) handles data fetching properly. One hook: `useQuery`. It gives you loading, error, data, caching, deduplication, background refetch, retry, and stale-while-revalidate — all built in.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "One hook"}} `useQuery({ queryKey, queryFn })`. The query key identifies this data in the cache. The query function does the actual fetch. React Query handles everything else — loading state, error state, caching, deduplication.

{{zoom: 1x}} {{focus: result}} {{zoom: 1.2x}} {{annotate: result "Destructured result"}} `isPending` is true while the first fetch is in flight. `isError` is true if the fetch failed. `data` is the resolved value. No manual state management. No `useState` for loading. No `useEffect` at all.

{{zoom: 1x}} {{focus: key}} {{zoom: 1.2x}} {{pulse: key}} {{pan: key}} {{annotate: key "Query key = cache key"}} `["user", userId]` — the query key. Change `userId`, and React Query knows this is a different query. It checks the cache first. If the data exists and is fresh, it returns immediately. If stale, it returns the cached data and refetches in the background.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:rq-code lang=tsx
import { useQuery } from "@tanstack/react-query";

function UserProfile({ userId }: { readonly userId: number }) {
  const { data: user, isPending, isError, error } = useQuery({
    queryKey: ["user", userId],
    queryFn: () =>
      fetch(`/api/users/${userId}`).then((res) => res.json()),
  });

  if (isPending) return <Spinner />;
  if (isError) return <ErrorMessage message={error.message} />;

  return <div>{user.name}</div>;
}
// 12 lines. Caching. Dedup. Retry. Background refetch. Done.
---
basic: 4-8
result: 10-11
key: 5
```

```diagram:rq-diagram kind=graph
Component [label="Component renders" shape=box]
UseQuery [label="useQuery called"]
CacheCheck [label="Cache hit?" shape=diamond]
Fresh [label="Return cached\n(instant)"]
Stale [label="Return cached +\nrefetch background"]
Miss [label="Fetch from server"]
Update [label="Update cache\n+ re-render" shape=box]

Component -> UseQuery
UseQuery -> CacheCheck
CacheCheck -> Fresh [label="fresh"]
CacheCheck -> Stale [label="stale"]
CacheCheck -> Miss [label="miss"]
Stale -> Update
Miss -> Update
```

# Query keys and stale time

{{clear: slide}}

{{show: keys-code typewriter 2s linear}} Query keys are how React Query identifies and caches data. They're arrays. More specific keys mean more granular caching. The key structure determines when cached data is reused.

{{focus: simple}} {{zoom: 1.2x}} {{pulse: simple}} {{annotate: simple "Simple key"}} `["users"]` caches the user list. Every component that calls `useQuery` with `["users"]` gets the same cached data. No duplicate fetches. One request, shared result.

{{zoom: 1x}} {{focus: parameterized}} {{zoom: 1.2x}} {{annotate: parameterized "Parameterized key"}} `["user", userId]` caches per user. User 1 and user 5 are separate cache entries. Navigate to user 1, fetch. Navigate to user 5, fetch. Navigate back to user 1 — instant. Already cached.

{{zoom: 1x}} {{focus: stale}} {{zoom: 1.2x}} {{pulse: stale}} {{pan: stale}} {{annotate: stale "Stale time"}} `staleTime: 60_000` — data is fresh for 60 seconds. During that window, React Query returns the cached value without refetching. After 60 seconds, the next access triggers a background refetch while still showing the stale data. Stale-while-revalidate.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:keys-code lang=tsx
// Simple key: all users
const { data: users } = useQuery({
  queryKey: ["users"],
  queryFn: fetchUsers,
});

// Parameterized key: one user
const { data: user } = useQuery({
  queryKey: ["user", userId],
  queryFn: () => fetchUser(userId),
});

// Nested key: user's posts
const { data: posts } = useQuery({
  queryKey: ["user", userId, "posts"],
  queryFn: () => fetchUserPosts(userId),
});

// Stale time: fresh for 60 seconds
const { data } = useQuery({
  queryKey: ["dashboard"],
  queryFn: fetchDashboard,
  staleTime: 60_000,
});

// Infinite stale time: never refetch automatically
const { data: config } = useQuery({
  queryKey: ["config"],
  queryFn: fetchConfig,
  staleTime: Infinity,
});
---
simple: 2-5
parameterized: 8-11
stale: 20-24
```

# Mutations

{{clear: slide}}

{{show: mutation-code typewriter 2s linear}} Reading data is `useQuery`. Writing data is `useMutation`. A mutation sends data to the server and then invalidates the relevant cache so the UI updates.

{{focus: basic-mutation}} {{zoom: 1.2x}} {{pulse: basic-mutation}} {{annotate: basic-mutation "Mutate + invalidate"}} `useMutation` takes a `mutationFn` (the write operation) and `onSuccess` (what to do after). `queryClient.invalidateQueries` tells React Query: "the users data is stale, refetch it." The list updates automatically.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Call mutate"}} `mutation.mutate(newUser)` triggers the mutation. `mutation.isPending` is true while it runs — use it to disable the submit button or show a spinner. `mutation.isError` tells you if it failed.

{{zoom: 1x}} {{focus: optimistic}} {{zoom: 1.2x}} {{pulse: optimistic}} {{pan: optimistic}} {{annotate: optimistic "Optimistic update"}} Update the UI before the server confirms. If the server rejects, roll back to the old value. The user sees instant feedback. `onMutate` updates the cache optimistically. `onError` reverts. `onSettled` refetches to ensure consistency.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:mutation-code lang=tsx
import { useMutation, useQueryClient } from "@tanstack/react-query";

function AddUserForm() {
  const queryClient = useQueryClient();

  const mutation = useMutation({
    mutationFn: (newUser: { name: string; email: string }) =>
      fetch("/api/users", {
        method: "POST",
        body: JSON.stringify(newUser),
        headers: { "Content-Type": "application/json" },
      }).then((res) => res.json()),
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["users"] });
    },
  });

  function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    mutation.mutate({ name: "Alice", email: "alice@example.com" });
  }

  return (
    <form onSubmit={handleSubmit}>
      <button type="submit" disabled={mutation.isPending}>
        {mutation.isPending ? "Adding..." : "Add User"}
      </button>
      {mutation.isError && <p>Error: {mutation.error.message}</p>}
    </form>
  );
}

// Optimistic update
const mutation = useMutation({
  mutationFn: updateTodo,
  onMutate: async (newTodo) => {
    await queryClient.cancelQueries({ queryKey: ["todos"] });
    const previous = queryClient.getQueryData(["todos"]);
    queryClient.setQueryData(["todos"], (old: Todo[]) =>
      old.map((t) => (t.id === newTodo.id ? { ...t, ...newTodo } : t)),
    );
    return { previous };
  },
  onError: (_err, _newTodo, context) => {
    queryClient.setQueryData(["todos"], context?.previous);
  },
  onSettled: () => {
    queryClient.invalidateQueries({ queryKey: ["todos"] });
  },
});
---
basic-mutation: 6-16
usage: 19-21
optimistic: 34-48
```

# Setting up React Query

{{clear: slide}}

{{show: setup-code typewriter 2s linear}} React Query needs a `QueryClient` and a `QueryClientProvider` at the root of your app. Create the client once — outside the component — and wrap your app with the provider.

{{focus: client}} {{zoom: 1.2x}} {{pulse: client}} {{annotate: client "One client"}} `new QueryClient()` creates the cache and manages all queries. Define it outside any component so it's created once. Default options like `staleTime` can be set here for all queries.

{{zoom: 1x}} {{focus: provider}} {{zoom: 1.2x}} {{annotate: provider "Provider wraps app"}} `<QueryClientProvider client={queryClient}>` gives every component in the tree access to the cache. Any component can call `useQuery` or `useMutation` and share the same cache.

{{zoom: 1x}} {{focus: devtools}} {{zoom: 1.2x}} {{pan: devtools}} {{annotate: devtools "DevTools"}} `ReactQueryDevtools` shows every query in the cache — its key, status, data, and timing. Invaluable during development. Stripped from production builds automatically.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:setup-code lang=tsx
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { ReactQueryDevtools } from "@tanstack/react-query-devtools";

// Create once — outside components
const queryClient = new QueryClient({
  defaultOptions: {
    queries: {
      staleTime: 60_000,
      retry: 2,
    },
  },
});

// Wrap your app
function App() {
  return (
    <QueryClientProvider client={queryClient}>
      <Router />
      <ReactQueryDevtools initialIsOpen={false} />
    </QueryClientProvider>
  );
}

// Now any component can use useQuery
function Dashboard() {
  const { data, isPending } = useQuery({
    queryKey: ["dashboard"],
    queryFn: fetchDashboard,
  });

  if (isPending) return <Skeleton />;
  return <DashboardView data={data} />;
}
---
client: 5-12
provider: 16-22
devtools: 19
```
