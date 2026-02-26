# Lab 19: Compound Accordion

Build a compound `Accordion` component. Parts share state through context. Only one item open at a time.

## Tasks

1. **Create context** — `AccordionContext` with `openId: string | null` and `toggle: (id: string) => void`. Create the context with `createContext`. Write a `useAccordion` hook that reads it.

2. **Accordion provider** — The `Accordion` component manages `openId` state. Toggle sets the `openId` or clears it (if already open). Wraps children in the context provider.

3. **Accordion.Trigger** — Reads context. On click, calls `toggle(id)`. Displays children plus an open/close indicator.

4. **Accordion.Content** — Reads context. If `openId !== id`, returns `null`. Otherwise renders children.

5. **Namespace attachment** — Attach `Item`, `Trigger`, and `Content` as properties of `Accordion`: `Accordion.Item = Item`.

6. **Use in App.tsx** — Render an `<Accordion>` with at least 3 items. Verify only one opens at a time.

## Hints

- `use(AccordionCtx)` in React 19 replaces `useContext(AccordionCtx)`.
- The toggle function: `setOpenId(prev => prev === id ? null : id)`.
- No boolean props on any component. The compound pattern replaces them.
- Each part needs the item `id` to know which item it belongs to.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
