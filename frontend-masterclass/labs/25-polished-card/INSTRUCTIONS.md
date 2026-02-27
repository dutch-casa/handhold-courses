# Lab 24: Polished Dashboard Card

Build a dashboard card that demonstrates every design engineering technique from this module.

## Tasks

1. **Layered shadows** — The card has at least two box-shadow layers. Not a single flat shadow. A tight contact shadow plus a wider ambient shadow.

2. **Hover lift** — On hover (inside `@media (hover: hover)`), the card lifts with `transform: translateY(-2px)` and the shadows increase. Transition both `transform` and `box-shadow` — specify exact properties, never `transition: all`.

3. **Dark mode** — Use `prefers-color-scheme: dark`. In dark mode, shadows become borders or subtle glows. Background changes to a dark surface color. Text uses off-white, not pure white.

4. **Loading skeleton** — A `CardSkeleton` component that matches the card layout. Pulse animation. Reduced motion disables the animation.

5. **Typography** — Use a type scale. Title in a larger size with `line-height: 1.2`. Body text with `line-height: 1.5`. Numbers use `font-variant-numeric: tabular-nums`.

6. **Press feedback** — The card gives subtle feedback on active/press: `transform: scale(0.98)` on `:active`.

## Hints

- Multiple shadows: `box-shadow: 0 1px 2px rgba(...), 0 4px 8px rgba(...)`.
- Hover lift: combine shadow increase with `translateY(-2px)`.
- Dark mode borders: `box-shadow: 0 0 0 1px rgba(255,255,255,0.08)`.
- Skeleton pulse: `@keyframes pulse { 0%, 100% { opacity: 0.4 } 50% { opacity: 0.7 } }`.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
