# Lab 22: Notification System

Build an animated notification component that enters, stacks, auto-dismisses, and respects reduced motion.

## Tasks

1. **Notification component** — `Notification.tsx`. Takes `message`, `onDismiss`, and optional `duration` (default 5000ms). Renders in a `div` with `role="alert"` and `aria-live="polite"`.

2. **Enter animation** — CSS `@keyframes` that slides in from the right with a fade. Use `transform: translateX` and `opacity`. Apply with a `notification-enter` class.

3. **Auto-dismiss** — After `duration` ms, call `onDismiss`. Use `setTimeout` in a `useEffect` with cleanup.

4. **Manual dismiss** — A close button (`×`) that calls `onDismiss` on click. Must have `aria-label="Dismiss"`.

5. **Stack** — A `NotificationStack` component that holds an array of notifications. New ones appear at the top. Position fixed, top-right. `display: flex; flex-direction: column-reverse; gap: 8px`.

6. **Reduced motion** — `@media (prefers-reduced-motion: reduce)` sets animation-duration to 0.01ms.

## Hints

- `animation: slideIn 250ms ease-out forwards` — `forwards` keeps the final state.
- Auto-dismiss cleanup: `return () => clearTimeout(timer)`.
- Stack state: `useState<Array<{ id: number; message: string }>>([])`.
- Use `Date.now()` or a counter for unique notification IDs.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
