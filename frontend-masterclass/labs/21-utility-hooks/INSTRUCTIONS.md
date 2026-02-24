# Lab 21: Utility Hooks

Build three custom hooks: `useLocalStorage`, `useMediaQuery`, and `useDebounce`.

## Tasks

1. **`useLocalStorage<T>(key, initialValue)`** in `useLocalStorage.ts`
   - Returns `[T, (value: T | ((prev: T) => T)) => void]` — same API as `useState`.
   - Reads from localStorage on mount (lazy initializer). Parses JSON.
   - Writes to localStorage on every setter call. Stringifies to JSON.
   - Falls back to `initialValue` if key doesn't exist or JSON parse fails.

2. **`useMediaQuery(query)`** in `useMediaQuery.ts`
   - Returns `boolean`.
   - Reads `window.matchMedia(query).matches` on mount.
   - Subscribes to the `change` event. Unsubscribes on cleanup.

3. **`useDebounce<T>(value, delay)`** in `useDebounce.ts`
   - Returns `T` — the debounced value.
   - Sets a timeout on value change. Clears the timeout on cleanup (or next change).
   - The debounced value updates after `delay` ms of no changes.

4. **Type safety** — All three hooks should be generic where applicable. `useLocalStorage<T>` and `useDebounce<T>` accept any serializable type.

5. **Export** — Each hook should be a named export from its file.

## Hints

- `useState(() => ...)` is the lazy initializer pattern — the function runs only once.
- `useEffect` cleanup runs before the next effect and on unmount.
- `JSON.parse` can throw — wrap in try/catch.
- `matchMedia` returns a `MediaQueryList` with `.matches` and `addEventListener("change", ...)`.
