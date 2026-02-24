# Modern CSS refactor

Refactor a flat CSS file to use modern features: nesting, layers, :has(), and light-dark().

## Tasks

- [ ] Declare CSS layers in order: `@layer reset, base, components, utilities;`
- [ ] Move the reset styles into `@layer reset { ... }`
- [ ] Convert the `.card` styles to use native CSS nesting (nest `.title`, `:hover`, and the media query inside `.card`)
- [ ] Add a `:has()` selector: `.card:has(img)` should get `grid-template-columns: 150px 1fr`
- [ ] Use `light-dark()` for at least one color value (requires `color-scheme: light dark` on `:root`)

## Hints

Layer declaration goes at the top of the file:

```css
@layer reset, base, components, utilities;
```

CSS nesting puts child selectors inside the parent block:

```css
.card {
  padding: 16px;

  & .title { font-weight: 700; }
  &:hover { box-shadow: 0 2px 8px rgb(0 0 0 / 0.1); }

  @media (min-width: 768px) { padding: 24px; }
}
```

`light-dark()` needs `color-scheme` set:

```css
:root { color-scheme: light dark; }
body { color: light-dark(#1e293b, #e2e8f0); }
```

Use `Run Tests` in the testing panel to check your progress.
