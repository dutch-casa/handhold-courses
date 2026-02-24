# Flexbox navigation

Build a responsive navigation bar using flexbox.

## Tasks

- [ ] Make `.nav` a flex container with items vertically centered
- [ ] Push the `.logo` to the left and the `.cta` button to the right using `space-between`
- [ ] Center the `.nav-links` using `gap` for spacing between links (no margins)
- [ ] Make the nav wrap on narrow screens with `flex-wrap: wrap`
- [ ] Vertically center all items with `align-items: center`

## Hints

The nav layout:

```
[Logo]     [Home] [About] [Blog]     [Sign Up]
```

`justify-content: space-between` pushes the first and last items to opposite edges, with the rest distributed between.

`gap: 16px` on the nav links container adds even spacing without margins.

`flex-wrap: wrap` lets the nav reflow when the screen gets narrow.

`align-items: center` on the nav makes everything line up vertically.

Use `Run Tests` in the testing panel to check your progress.
