# Box model exercises

Fix four common box model problems in the provided stylesheet.

## Tasks

- [ ] Add the universal `box-sizing: border-box` reset to all elements (including `::before` and `::after`)
- [ ] Fix `.box-1` so its total width is exactly 300px (it's currently wider due to padding)
- [ ] Fix the margin collapse between `.card-a` and `.card-b` — they should have 32px gap, not a collapsed 24px
- [ ] Center `.container` horizontally using margin auto (it needs a `max-width` set)

## Hints

The universal box-sizing reset looks like: `*, *::before, *::after { box-sizing: border-box; }`

`.box-1` has `width: 300px` with `padding: 20px` and a `2px` border. With `content-box` sizing, the total is 300 + 40 + 4 = 344px. Switch to `border-box` to make it exactly 300px.

Margin collapse happens between adjacent block siblings. To prevent it, you can use `display: flex; flex-direction: column; gap: 32px` on the parent, or add a border/padding to break the collapse.

`margin: 0 auto` centers a block element, but only if it has a defined width or `max-width`.

Use `Run Tests` in the testing panel to check your progress.
