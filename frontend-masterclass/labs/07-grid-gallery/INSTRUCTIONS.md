# Grid photo gallery

Build a responsive photo gallery using CSS Grid. No media queries.

## Tasks

- [ ] Make `.gallery` a grid container with `display: grid`
- [ ] Use `repeat(auto-fill, minmax(200px, 1fr))` for responsive columns
- [ ] Add `gap` between grid items
- [ ] Make `.featured` span 2 columns with `grid-column: span 2`
- [ ] Add `grid-auto-flow: dense` to fill gaps left by spanning items

## Hints

The responsive grid pattern in one line:

```css
grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
```

`auto-fill` creates as many columns as fit. `minmax(200px, 1fr)` sets each column to at least 200px, growing equally.

`grid-column: span 2` makes an item take two columns.

`grid-auto-flow: dense` tells the browser to backfill holes in the grid.

Use `Run Tests` in the testing panel to check your progress.
