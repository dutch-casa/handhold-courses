# Lab 20: Theme Provider

Build a theme system using the provider pattern. Components read and toggle the theme through context.

## Tasks

1. **Define the context shape** in `ThemeProvider.tsx` — `ThemeContext` with `readonly theme: "light" | "dark"` and `readonly toggleTheme: () => void`. Create the context with `createContext`.

2. **Create `useTheme` hook** — reads context with `use()`. Throws if used outside a provider.

3. **Build `ThemeProvider` component** — manages theme state. Toggle switches between "light" and "dark". On mount, reads `localStorage.getItem("theme")` for persistence. On toggle, saves to `localStorage`.

4. **System preference detection** — Use `window.matchMedia("(prefers-color-scheme: dark)")` to set the initial theme when no localStorage value exists.

5. **Use in App.tsx** — Wrap the app in `<ThemeProvider>`. Create a `ThemeToggle` button that uses `useTheme`. Create a `ThemedCard` component that reads `theme` and applies a className.

## Hints

- Initial theme priority: localStorage > system preference > "light".
- `window.matchMedia("(prefers-color-scheme: dark)").matches` returns a boolean.
- For persistence: `localStorage.setItem("theme", newTheme)` inside the toggle.
- `use(ThemeCtx)` is React 19's replacement for `useContext(ThemeCtx)`.
