---
title: Custom hooks
---

# Functions with superpowers

{{show: hook-code typewriter 2s linear}} A custom hook is a function whose name starts with `use`. That's it. No special API. No registration. React sees the `use` prefix and allows the function to call other hooks — `useState`, `useEffect`, `use`.

{{focus: before}} {{zoom: 1.2x}} {{pulse: before}} {{annotate: before "Duplicated logic"}} Two components need a toggle. Both create `useState(false)`. Both write the same toggle function. Copy-paste works for two. But ten components? Twenty? The logic belongs in one place.

{{zoom: 1x}} {{focus: hook}} {{zoom: 1.2x}} {{pulse: hook}} {{annotate: hook "Extracted hook"}} `useToggle` — a function that calls `useState` and returns `[value, toggle]`. Each component that calls `useToggle` gets its own independent state. The hook shares logic, not state.

{{zoom: 1x}} {{focus: returns}} {{zoom: 1.2x}} {{pan: returns}} {{annotate: returns "Returns data, not UI"}} Custom hooks return data — booleans, strings, objects, functions. Not JSX. Components render UI. Hooks manage stateful logic. The separation is clean: hooks supply data, components display it.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:hook-code lang=tsx
// BEFORE: duplicated toggle logic
function Modal() {
  const [isOpen, setIsOpen] = useState(false);
  const toggle = () => setIsOpen((prev) => !prev);
  // ...
}
function Sidebar() {
  const [isOpen, setIsOpen] = useState(false);
  const toggle = () => setIsOpen((prev) => !prev);
  // ...
}

// AFTER: extracted into a custom hook
function useToggle(initial = false): [boolean, () => void] {
  const [value, setValue] = useState(initial);
  const toggle = () => setValue((prev) => !prev);
  return [value, toggle];
}

// Each call creates independent state
function Modal() {
  const [isOpen, toggle] = useToggle();
  return <dialog open={isOpen}><button onClick={toggle}>Close</button></dialog>;
}
function Sidebar() {
  const [isOpen, toggle] = useToggle();
  return <aside className={isOpen ? "open" : ""}><button onClick={toggle}>Menu</button></aside>;
}
---
before: 2-10
hook: 13-18
returns: 13-18
```

# Rules of hooks

{{clear: slide}}

{{show: rules-code typewriter 2s linear}} Hooks have two rules. Both exist because React tracks hooks by call order — the first `useState` in a component is always the first, the second is always the second. Break the order and React loses track.

{{focus: rule1}} {{zoom: 1.2x}} {{pulse: rule1}} {{annotate: rule1 "Top level only"}} Call hooks at the top level of your component or hook. Not inside `if` blocks, `for` loops, or nested functions. An `if` that skips a hook changes the call order — React can't match hook 3 to the right state.

{{zoom: 1x}} {{focus: rule2}} {{zoom: 1.2x}} {{annotate: rule2 "Functions only"}} Only call hooks from React functions — components (capitalized) or custom hooks (prefixed with `use`). Not from regular functions, event handlers, or callbacks. The `use` prefix tells React's linter to check the rules.

{{zoom: 1x}} {{focus: why}} {{zoom: 1.2x}} {{pulse: why}} {{pan: why}} {{annotate: why "Order matters"}} React stores hook state as an array. On each render, it walks the array: hook 1 gets slot 0, hook 2 gets slot 1. Skip a hook with an `if`? Every hook after it reads the wrong slot. The rules prevent this.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:rules-code lang=tsx
// RULE 1: Only call hooks at the top level

// BAD — conditional hook
function BadComponent({ shouldFetch }: { readonly shouldFetch: boolean }) {
  if (shouldFetch) {
    const data = useQuery({ queryKey: ["x"], queryFn: fetch }); // WRONG
  }
  const [count, setCount] = useState(0);
}

// GOOD — hook always called, condition inside
function GoodComponent({ shouldFetch }: { readonly shouldFetch: boolean }) {
  const { data } = useQuery({
    queryKey: ["x"],
    queryFn: fetchX,
    enabled: shouldFetch, // conditionally fetch, not conditionally hook
  });
  const [count, setCount] = useState(0);
}

// RULE 2: Only call from React functions

// BAD — hook in regular function
function getUser() {
  const [user] = useState(null); // WRONG — not a component or hook
  return user;
}

// GOOD — hook in custom hook (use prefix)
function useUser() {
  const [user, setUser] = useState<User | null>(null);
  return { user, setUser };
}

// WHY: React tracks hooks by call order
// Render 1: useState(0) → slot 0, useState("") → slot 1
// Render 2: useState(0) → slot 0, useState("") → slot 1
// Skip one? Slot indices shift. State becomes corrupted.
---
rule1: 4-9
rule2: 24-28
why: 36-38
```

# useLocalStorage

{{clear: slide}}

{{show: storage-code typewriter 2s linear}} A practical custom hook: `useLocalStorage`. It works like `useState`, but the value persists in localStorage. Read on mount. Write on every update. The component doesn't know or care that persistence exists.

{{focus: signature}} {{zoom: 1.2x}} {{pulse: signature}} {{annotate: signature "Same API as useState"}} `useLocalStorage<T>(key, initialValue)` returns `[value, setValue]`. The consumer uses it exactly like `useState`. The persistence is invisible.

{{zoom: 1x}} {{focus: read}} {{zoom: 1.2x}} {{annotate: read "Lazy initializer"}} The `useState` initializer runs once. It reads localStorage, parses JSON, and returns the stored value or the fallback. Lazy initialization means no localStorage read on every render — only the first.

{{zoom: 1x}} {{focus: write}} {{zoom: 1.2x}} {{pulse: write}} {{pan: write}} {{annotate: write "Write on update"}} A custom `setValue` writes to localStorage before updating state. Both happen together. The hook coordinates two stores — React state for rendering, localStorage for persistence.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:storage-code lang=tsx
function useLocalStorage<T>(
  key: string,
  initialValue: T,
): [T, (value: T | ((prev: T) => T)) => void] {

  // Lazy initializer: read once on mount
  const [stored, setStored] = useState<T>(() => {
    const item = localStorage.getItem(key);
    if (item === null) return initialValue;
    try {
      return JSON.parse(item) as T;
    } catch {
      return initialValue;
    }
  });

  // Custom setter: persist then update state
  function setValue(value: T | ((prev: T) => T)) {
    setStored((prev) => {
      const next = value instanceof Function ? value(prev) : value;
      localStorage.setItem(key, JSON.stringify(next));
      return next;
    });
  }

  return [stored, setValue];
}

// Usage — identical to useState
function Settings() {
  const [fontSize, setFontSize] = useLocalStorage("fontSize", 16);

  return (
    <div style={{ fontSize }}>
      <button onClick={() => setFontSize((s) => s + 2)}>Bigger</button>
      <button onClick={() => setFontSize((s) => s - 2)}>Smaller</button>
    </div>
  );
}
---
signature: 1-4
read: 7-15
write: 18-25
```

# useMediaQuery

{{clear: slide}}

{{show: media-code typewriter 2s linear}} `useMediaQuery` wraps `window.matchMedia` in a reactive hook. Pass a CSS media query string. Get a boolean that updates when the match changes. Use it for responsive logic that CSS can't handle alone.

{{focus: impl}} {{zoom: 1.2x}} {{pulse: impl}} {{annotate: impl "Match + listener"}} Create the `MediaQueryList`. Read its `.matches` property. Add a `change` listener to update when the match flips. Clean up the listener on unmount. The hook turns a browser API into reactive state.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{pan: usage}} {{annotate: usage "Responsive logic"}} `useMediaQuery("(min-width: 768px)")` — true on desktop, false on mobile. `useMediaQuery("(prefers-color-scheme: dark)")` — true if the OS is in dark mode. Any valid CSS media query works.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:media-code lang=tsx
import { useState, useEffect } from "react";

function useMediaQuery(query: string): boolean {
  const [matches, setMatches] = useState(() =>
    window.matchMedia(query).matches,
  );

  useEffect(() => {
    const mql = window.matchMedia(query);
    const handler = (e: MediaQueryListEvent) => setMatches(e.matches);
    mql.addEventListener("change", handler);
    return () => mql.removeEventListener("change", handler);
  }, [query]);

  return matches;
}

// Usage: responsive component logic
function Navigation() {
  const isMobile = useMediaQuery("(max-width: 767px)");

  return isMobile ? <MobileNav /> : <DesktopNav />;
}

// Usage: dark mode detection
function usePrefersDark(): boolean {
  return useMediaQuery("(prefers-color-scheme: dark)");
}
---
impl: 3-16
usage: 19-28
```

# useDebounce

{{clear: slide}}

{{show: debounce-code typewriter 2s linear}} Debouncing delays a value until the user stops typing. Without it, a search input fires a query on every keystroke. With it, the query fires once — after the user pauses.

{{focus: hook-impl}} {{zoom: 1.2x}} {{pulse: hook-impl}} {{annotate: hook-impl "Delayed value"}} `useDebounce(value, delay)` — returns a delayed copy of the value. The value updates immediately in state (the input feels responsive). The debounced value updates after `delay` ms of silence. Use the debounced value for expensive operations.

{{zoom: 1x}} {{focus: effect}} {{zoom: 1.2x}} {{annotate: effect "Timer reset"}} Each time `value` changes, set a timeout. If `value` changes again before the timeout fires, clear the old timeout and start a new one. Only when the user stops changing the value does the timeout fire and update the debounced value.

{{zoom: 1x}} {{focus: search}} {{zoom: 1.2x}} {{pulse: search}} {{pan: search}} {{annotate: search "Debounced search"}} Type "react" — four keystrokes. Without debounce: four fetches. With 300ms debounce: one fetch, 300ms after the last keystroke. The input stays responsive because `search` updates immediately. The query waits for `debouncedSearch`.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:debounce-code lang=tsx
import { useState, useEffect } from "react";

function useDebounce<T>(value: T, delay: number): T {
  const [debounced, setDebounced] = useState(value);

  useEffect(() => {
    const timer = setTimeout(() => setDebounced(value), delay);
    return () => clearTimeout(timer);
  }, [value, delay]);

  return debounced;
}

// Usage: debounced search
function SearchPage() {
  const [search, setSearch] = useState("");
  const debouncedSearch = useDebounce(search, 300);

  // Query fires only when debouncedSearch changes
  const { data } = useQuery({
    queryKey: ["search", debouncedSearch],
    queryFn: () => fetchResults(debouncedSearch),
    enabled: debouncedSearch.length > 0,
  });

  return (
    <div>
      <input
        value={search}
        onChange={(e) => setSearch(e.target.value)}
        placeholder="Search..."
      />
      {data?.map((result) => (
        <div key={result.id}>{result.title}</div>
      ))}
    </div>
  );
}
---
hook-impl: 3-12
effect: 6-9
search: 15-36
```

# Composing hooks

{{clear: slide}}

{{show: compose-code typewriter 2s linear}} Hooks compose. A custom hook can call other custom hooks. Build small, focused hooks. Combine them into larger ones. The component at the top uses one hook that orchestrates many.

{{focus: small}} {{zoom: 1.2x}} {{pulse: small}} {{annotate: small "Small hooks"}} `useLocalStorage` handles persistence. `useMediaQuery` handles responsive detection. `useDebounce` handles timing. Each does one thing well. None know about each other.

{{zoom: 1x}} {{focus: composed}} {{zoom: 1.2x}} {{pulse: composed}} {{pan: composed}} {{annotate: composed "Composed hook"}} `useSearchPage` calls `useState` for the search term, `useDebounce` for the delayed value, `useQuery` for the fetch, and `useMediaQuery` for layout decisions. One hook that provides everything the component needs. The component is pure rendering.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:compose-code lang=tsx
// Small hooks compose into larger hooks

function useSearchPage() {
  const [search, setSearch] = useState("");
  const debouncedSearch = useDebounce(search, 300);
  const isMobile = useMediaQuery("(max-width: 767px)");
  const [viewMode, setViewMode] = useLocalStorage<"grid" | "list">(
    "search-view",
    "grid",
  );

  const { data, isPending } = useQuery({
    queryKey: ["search", debouncedSearch],
    queryFn: () => fetchResults(debouncedSearch),
    enabled: debouncedSearch.length > 0,
  });

  return {
    search,
    setSearch,
    results: data ?? [],
    isPending,
    isMobile,
    viewMode,
    setViewMode,
  } as const;
}

// The component is pure rendering
function SearchPage() {
  const {
    search, setSearch,
    results, isPending,
    isMobile,
    viewMode, setViewMode,
  } = useSearchPage();

  return (
    <div>
      <input value={search} onChange={(e) => setSearch(e.target.value)} />
      {isPending && <Spinner />}
      {isMobile
        ? <MobileResults results={results} />
        : <DesktopResults results={results} view={viewMode} />
      }
    </div>
  );
}
---
small: 4-6
composed: 3-26
```
