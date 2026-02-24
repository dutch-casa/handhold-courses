# Lab 23: Animated Tabs

Build a compound tab component with Framer Motion. The tab indicator glides between tabs using `layoutId`. Content slides in from the direction you clicked using `AnimatePresence` and direction-aware variants. Reduced motion disables all animation.

## Tasks

1. **Context and provider** — Create a `TabsContext` with `activeTab`, `direction`, `selectTab`, and `registerTab`. The root `Tabs` component holds state, wraps children in the provider and a `MotionConfig` that sets the default spring transition.

2. **Tab registration** — Each `Tab` calls `registerTab(id)` on mount via `useEffect`. The parent stores IDs in a ref to track order. When `selectTab` is called, compare the old and new index to set `direction` (1 for forward, -1 for backward).

3. **Tab indicator with layoutId** — When a tab is active, render a `motion.div` with `className="tab-indicator"` and `layoutId="tab-indicator"` inside the button. Framer Motion animates the indicator between tabs automatically.

4. **AnimatePresence for content** — `TabContent` wraps children in `AnimatePresence` with `mode="popLayout"` and `custom={direction}`. It finds the active child panel and renders only that one. The key on each panel triggers enter/exit.

5. **Direction-aware panel animation** — `TabPanel` uses variants: `enter` slides in from `direction * 80` px with 0 opacity. `center` is the resting state. `exit` slides out to `direction * -80` px. Use the `custom` prop to pass direction to variant functions.

6. **Reduced motion** — Call `useReducedMotion()` in the root. When true, set `MotionConfig` transition to `{ duration: 0 }`.

## Hints

- `Object.assign(TabsRoot, { List, Tab, Content, Panel })` creates the compound API.
- Use `useRef` for the tab ID registry and active tab mirror — keeps `selectTab` stable.
- `Children.toArray(children).find(...)` + `isValidElement` to locate the active panel.
- Variant functions receive the `custom` value: `(d: number) => ({ x: d * 80 })`.
- `AnimatePresence initial={false}` skips the enter animation on first render.
