# Responsive gallery

Enhance the grid gallery with responsive features: fluid typography, container queries, dark mode, and reduced motion support.

## Tasks

- [ ] Use `clamp()` for the `h1` font-size — minimum 1.25rem, maximum 2rem
- [ ] Add `container-type: inline-size` to `.gallery` and use `@container` to change card layout at 300px
- [ ] Add a `prefers-color-scheme: dark` media query that changes background and text colors
- [ ] Add a `prefers-reduced-motion: reduce` media query that disables transitions
- [ ] Make the card layout adapt to its container width (vertical below 300px, horizontal above)

## Hints

Fluid typography with clamp:

```css
h1 { font-size: clamp(1.25rem, 1rem + 1.5vw, 2rem); }
```

Container queries need a container context on the parent:

```css
.gallery { container-type: inline-size; }

@container (min-width: 300px) {
  .card { grid-template-columns: 120px 1fr; }
}
```

Dark mode:

```css
@media (prefers-color-scheme: dark) {
  :root { --color-bg: #0f172a; --color-text: #e2e8f0; }
}
```

Reduced motion:

```css
@media (prefers-reduced-motion: reduce) {
  * { transition-duration: 0.01ms !important; }
}
```

Use `Run Tests` in the testing panel to check your progress.
