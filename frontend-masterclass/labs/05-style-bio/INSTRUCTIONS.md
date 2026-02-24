# Style the bio page

Take the HTML bio page provided and make it look good with CSS. You'll use custom properties, pseudo-classes, pseudo-elements, and real selectors.

## Tasks

- [ ] Define a color palette using HSL custom properties on `:root` — at least `--color-primary`, `--color-text`, `--color-bg`, and `--color-muted`
- [ ] Style headings with the system font stack (`system-ui, -apple-system, sans-serif`) and use your `--color-text` variable
- [ ] Add a hover effect to all links — change color and remove the underline on hover
- [ ] Style the list with custom `::marker` colors using your `--color-primary` variable
- [ ] Add the universal `*, *::before, *::after { box-sizing: border-box; }` reset

## Hints

Custom properties go on `:root`:

```css
:root {
  --color-primary: hsl(220, 91%, 60%);
  --color-text: hsl(220, 80%, 15%);
}
```

Reference them with `var()`:

```css
h1 { color: var(--color-text); }
```

The system font stack:

```css
font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
```

`::marker` styles list bullets:

```css
li::marker { color: var(--color-primary); }
```

Use `Run Tests` in the testing panel to check your progress.
