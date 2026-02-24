---
title: Flexbox
---

# The centering problem

{{show: center-problem reveal 0.3s spring}} Before flexbox, centering a div was a meme. Vertical centering required hacks — negative margins, table-cell display, absolute positioning with transforms. Every approach had tradeoffs and none felt right.

{{show: center-code typewriter 2s linear}}

{{focus: old-way}} {{zoom: 1.2x}} {{pulse: old-way}} {{annotate: old-way "The old way"}} Absolute positioning plus transform. You position the element at 50% from the top and left, then pull it back by half its own size with translate. It works, but you need to know the parent is positioned, and the element is removed from normal flow.

{{zoom: 1x}} {{focus: flex-way}} {{zoom: 1.2x}} {{pulse: flex-way}} {{annotate: flex-way "The flexbox way"}} Three properties. `display: flex` creates a flex container. `justify-content: center` centers horizontally. `align-items: center` centers vertically. Done. No hacks, no math, no caveats.

{{zoom: 1x}} {{focus: none}} Flexbox didn't just fix centering. It gave us a complete one-dimensional layout system. Let's break it apart.

```code:center-code lang=css
/* The old way: position + transform */
.parent {
  position: relative;
  height: 200px;
}
.child {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}

/* The flexbox way */
.parent {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 200px;
}
---
old-way: 2-10
flex-way: 13-18
```

```diagram:center-problem kind=graph
OldCenter [label="Old centering" shape=box]
Pos [label="position: absolute"]
Top50 [label="top: 50%"]
Transform [label="translate(-50%)"]
FlexCenter [label="Flexbox centering" shape=box]
Flex [label="display: flex"]
JC [label="justify-content: center"]
AI [label="align-items: center"]

OldCenter -> Pos
OldCenter -> Top50
OldCenter -> Transform
FlexCenter -> Flex
FlexCenter -> JC
FlexCenter -> AI
```

# Container and items

{{clear: slide}}

{{split}} {{show: flex-concepts typewriter 2s linear}} {{show: flex-preview reveal 0.5s spring}} Flexbox has two actors: the container and its items. The container controls the layout. The items respond.

{{focus: container}} {{zoom: 1.2x}} {{pulse: container}} {{annotate: container "Flex container"}} `display: flex` on the parent. That's it — the children become flex items. They stop behaving as block or inline elements. They're now in flex formatting context.

{{zoom: 1x}} {{focus: direction}} {{zoom: 1.2x}} {{pulse: direction}} {{annotate: direction "Flow direction"}} `flex-direction` sets the main axis. `row` flows left to right. `column` flows top to bottom. `row-reverse` and `column-reverse` flip the direction. Default is `row`.

{{zoom: 1x}} {{focus: wrap}} {{zoom: 1.2x}} {{annotate: wrap "Wrapping"}} `flex-wrap: wrap` lets items break to the next line when they run out of space. Without it, items shrink to fit — they never wrap. Most real layouts need wrap.

{{zoom: 1x}} {{focus: gap}} {{zoom: 1.2x}} {{pulse: gap}} {{annotate: gap "Gap"}} `gap` adds space between items. `gap: 16px` puts 16 pixels between every item, but not before the first or after the last. Before `gap`, people used margins and had to remove the margin from the last child. Gap is cleaner.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:flex-concepts lang=css
/* The flex container */
.container {
  display: flex;
}

/* Direction: which way items flow */
.row { flex-direction: row; }       /* → default */
.col { flex-direction: column; }    /* ↓ */
.row-rev { flex-direction: row-reverse; } /* ← */

/* Wrapping */
.container {
  flex-wrap: wrap;
}

/* Gap between items */
.container {
  gap: 16px;     /* equal row and column gap */
  row-gap: 24px; /* row gap only */
  column-gap: 8px; /* column gap only */
}
---
container: 2-4
direction: 7-9
wrap: 12-14
gap: 17-21
```

```preview:flex-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .demo { display: flex; gap: 8px; flex-wrap: wrap; margin-bottom: 12px; padding: 8px; background: #f8fafc; border-radius: 6px; border: 1px dashed #cbd5e1; }
  .demo.col { flex-direction: column; }
  .box { background: #3b82f6; color: white; padding: 8px 14px; border-radius: 4px; font-size: 12px; font-weight: 600; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
</style>
<div class="label">flex-direction: row (default)</div>
<div class="demo">
  <div class="box">A</div><div class="box">B</div><div class="box">C</div>
</div>
<div class="label">flex-direction: column</div>
<div class="demo col">
  <div class="box">A</div><div class="box">B</div><div class="box">C</div>
</div>
<div class="label">flex-wrap: wrap (resize to see)</div>
<div class="demo" style="max-width: 180px;">
  <div class="box">One</div><div class="box">Two</div><div class="box">Three</div><div class="box">Four</div>
</div>
```

# Justify and align

{{clear: slide}}

{{split}} {{show: justify-code typewriter 2s linear}} {{show: justify-preview reveal 0.5s spring}} Two properties control everything. `justify-content` distributes items along the main axis. `align-items` positions them along the cross axis.

{{focus: justify}} {{zoom: 1.2x}} {{pulse: justify}} {{annotate: justify "Main axis"}} `flex-start` packs items at the start. `flex-end` at the end. `center` in the middle. `space-between` puts the first item at the start, the last at the end, and distributes evenly between. `space-around` adds equal space around each item. `space-evenly` makes every gap identical.

{{zoom: 1x}} {{focus: align}} {{zoom: 1.2x}} {{pulse: align}} {{annotate: align "Cross axis"}} `stretch` is the default — items fill the container's height. `flex-start` packs to the top. `flex-end` to the bottom. `center` vertically centers. `baseline` aligns by text baseline — useful when items have different font sizes.

{{zoom: 1x}} {{focus: align-self}} {{zoom: 1.2x}} {{annotate: align-self "Per-item override"}} `align-self` overrides `align-items` for one specific item. The container says "everyone align to center." One item says "actually, I'll stretch." The item wins for itself.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:justify-code lang=css
/* Justify: distribute along main axis */
.between { justify-content: space-between; }
.center  { justify-content: center; }
.around  { justify-content: space-around; }
.evenly  { justify-content: space-evenly; }

/* Align: position along cross axis */
.stretch  { align-items: stretch; }   /* default */
.top      { align-items: flex-start; }
.middle   { align-items: center; }
.bottom   { align-items: flex-end; }
.baseline { align-items: baseline; }

/* Per-item override */
.container { align-items: center; }
.special { align-self: stretch; }
---
justify: 2-5
align: 8-12
align-self: 15-16
```

```preview:justify-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .demo {
    display: flex; gap: 6px; padding: 8px; margin-bottom: 8px;
    background: #f8fafc; border-radius: 6px; border: 1px dashed #cbd5e1;
    height: 60px;
  }
  .box { background: #3b82f6; color: white; padding: 6px 10px; border-radius: 4px; font-size: 11px; font-weight: 600; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 2px; }
</style>
<div class="label">justify-content: space-between</div>
<div class="demo" style="justify-content: space-between;">
  <div class="box">A</div><div class="box">B</div><div class="box">C</div>
</div>
<div class="label">justify-content: center + align-items: center</div>
<div class="demo" style="justify-content: center; align-items: center;">
  <div class="box">A</div><div class="box">B</div><div class="box">C</div>
</div>
<div class="label">justify-content: space-evenly + align-items: flex-end</div>
<div class="demo" style="justify-content: space-evenly; align-items: flex-end;">
  <div class="box">A</div><div class="box">B</div><div class="box">C</div>
</div>
```

# Flex grow, shrink, and basis

{{clear: slide}}

{{show: flex-props typewriter 2s linear}} Three properties control how items share available space: `flex-grow`, `flex-shrink`, and `flex-basis`. The shorthand `flex` combines all three.

{{focus: basis}} {{zoom: 1.2x}} {{pulse: basis}} {{annotate: basis "Starting size"}} `flex-basis` is the item's ideal size before growing or shrinking. Set it to `200px` and the item starts at 200 pixels. Set it to `0` and the item starts at zero — it only gets space from flex-grow. Set it to `auto` and it uses the item's content or width.

{{zoom: 1x}} {{focus: grow}} {{zoom: 1.2x}} {{pulse: grow}} {{annotate: grow "Taking extra space"}} `flex-grow` is a ratio. If one item has `flex-grow: 1` and another has `flex-grow: 2`, the second gets twice as much extra space. Default is 0 — items don't grow. Set it to 1 for even distribution.

{{zoom: 1x}} {{focus: shrink}} {{zoom: 1.2x}} {{annotate: shrink "Giving back space"}} `flex-shrink` is the opposite. When there's not enough room, items shrink. Default is 1 — all items shrink equally. Set it to 0 to prevent an item from shrinking below its basis. Set it to 2 to make it shrink faster.

{{zoom: 1x}} {{focus: shorthand}} {{zoom: 1.2x}} {{pulse: shorthand}} {{annotate: shorthand "The shorthand"}} `flex: 1` means `flex-grow: 1, flex-shrink: 1, flex-basis: 0`. The item starts at zero and takes one share of available space. `flex: 0 0 200px` means don't grow, don't shrink, stay at 200 pixels. Memorize `flex: 1` — you'll use it everywhere.

{{zoom: 1x}} {{focus: none}}

```code:flex-props lang=css
/* flex-basis: the starting size */
.fixed { flex-basis: 200px; }
.auto  { flex-basis: auto; }  /* use content width */
.zero  { flex-basis: 0; }     /* start from nothing */

/* flex-grow: how to take extra space */
.no-grow { flex-grow: 0; }    /* default — don't grow */
.grow-1  { flex-grow: 1; }    /* take 1 share */
.grow-2  { flex-grow: 2; }    /* take 2 shares */

/* flex-shrink: how to give back space */
.no-shrink { flex-shrink: 0; } /* never shrink */
.shrink-2  { flex-shrink: 2; } /* shrink faster */

/* Shorthand: flex: grow shrink basis */
.fill { flex: 1; }            /* = 1 1 0 */
.fixed-200 { flex: 0 0 200px; } /* = 0 0 200px */
.half { flex: 1 1 50%; }      /* = grow from 50% */
---
basis: 2-4
grow: 7-9
shrink: 12-13
shorthand: 16-18
```

# Common flexbox patterns

{{clear: slide}}

{{split}} {{show: patterns-code typewriter 2s linear}} {{show: patterns-preview reveal 0.5s spring}} Four patterns you'll build every week. Each one is a few flex properties.

{{focus: navbar}} {{zoom: 1.2x}} {{pulse: navbar}} {{annotate: navbar "Navigation bar"}} Logo on the left. Links in the center. Button on the right. `space-between` distributes them across the full width. `align-items: center` lines them up vertically. Wrap handles narrow screens.

{{zoom: 1x}} {{focus: card-row}} {{zoom: 1.2x}} {{pulse: card-row}} {{annotate: card-row "Card row"}} Equal-width cards that wrap. `flex: 1 1 250px` means each card wants to be 250 pixels, grows to fill space, and wraps when the row gets tight. Gap handles spacing.

{{zoom: 1x}} {{focus: sidebar}} {{zoom: 1.2x}} {{pulse: sidebar}} {{annotate: sidebar "Sidebar layout"}} Fixed sidebar, fluid content. The sidebar gets `flex: 0 0 250px` — don't grow, don't shrink, stay at 250px. The main content gets `flex: 1` — take everything else.

{{zoom: 1x}} {{focus: footer-push}} {{zoom: 1.2x}} {{pulse: footer-push}} {{annotate: footer-push "Sticky footer"}} Page content pushes the footer to the bottom even on short pages. The body is a column flex container with `min-height: 100vh`. The main content gets `flex: 1`. The footer stays at the bottom naturally.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:patterns-code lang=css
/* Navigation bar */
.nav {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  flex-wrap: wrap;
}

/* Equal card row */
.card-row {
  display: flex;
  flex-wrap: wrap;
  gap: 16px;
}
.card-row .card { flex: 1 1 250px; }

/* Sidebar + content */
.layout {
  display: flex;
  gap: 24px;
}
.sidebar { flex: 0 0 250px; }
.content { flex: 1; }

/* Sticky footer */
body {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}
main { flex: 1; }
---
navbar: 2-8
card-row: 11-16
sidebar: 19-23
footer-push: 26-31
```

```preview:patterns-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .nav-demo {
    display: flex; justify-content: space-between; align-items: center;
    padding: 8px 12px; background: #0f172a; border-radius: 6px; margin-bottom: 4px;
  }
  .nav-demo .logo { color: white; font-weight: 700; font-size: 13px; }
  .nav-demo .links { display: flex; gap: 12px; }
  .nav-demo .links a { color: #94a3b8; text-decoration: none; font-size: 12px; }
  .nav-demo .cta { background: #3b82f6; color: white; border: none; padding: 4px 10px; border-radius: 4px; font-size: 11px; }
  .card-demo { display: flex; flex-wrap: wrap; gap: 6px; margin-bottom: 4px; }
  .card-demo .card { flex: 1 1 80px; background: #eff6ff; padding: 8px; border-radius: 4px; font-size: 11px; text-align: center; border: 1px solid #bfdbfe; }
  .layout-demo { display: flex; gap: 6px; height: 60px; }
  .layout-demo .side { flex: 0 0 60px; background: #f0fdf4; border: 1px solid #86efac; border-radius: 4px; font-size: 10px; display: flex; align-items: center; justify-content: center; }
  .layout-demo .main { flex: 1; background: #eff6ff; border: 1px solid #93c5fd; border-radius: 4px; font-size: 10px; display: flex; align-items: center; justify-content: center; }
</style>
<div class="label">Navigation bar</div>
<div class="nav-demo">
  <span class="logo">Brand</span>
  <div class="links"><a>Home</a><a>About</a><a>Blog</a></div>
  <button class="cta">Sign up</button>
</div>
<div class="label">Card row (wraps)</div>
<div class="card-demo">
  <div class="card">Card 1</div><div class="card">Card 2</div><div class="card">Card 3</div>
</div>
<div class="label">Sidebar + content</div>
<div class="layout-demo">
  <div class="side">Sidebar</div>
  <div class="main">Main content (flex: 1)</div>
</div>
```

# Order and alignment tricks

{{clear: slide}}

{{show: tricks-code typewriter 1.5s linear}} A few more tools that round out your flexbox knowledge.

{{focus: order}} {{zoom: 1.2x}} {{pulse: order}} {{annotate: order "Visual reorder"}} `order` changes the visual position of an item without changing the HTML. Default is 0. Lower values come first. Use it for "mobile: logo first, desktop: logo second" layouts. But be careful — screen readers still follow DOM order.

{{zoom: 1x}} {{focus: margin-auto}} {{zoom: 1.2x}} {{pulse: margin-auto}} {{annotate: margin-auto "margin: auto trick"}} `margin-left: auto` on a flex item pushes it to the right. It eats all available space on that side. This is how you push one item to the far end without `space-between` — useful when you want most items on the left but one on the right.

{{zoom: 1x}} {{focus: centering}} {{zoom: 1.2x}} {{annotate: centering "The centering recipe"}} The universal centering pattern: `display: flex` plus `place-items: center`. `place-items` is shorthand for `align-items` and `justify-items`. Combine with `place-content: center` for centering in both directions. One line.

{{zoom: 1x}} {{focus: none}}

```code:tricks-code lang=css
/* Visual reorder */
.first-on-mobile { order: -1; }
.last { order: 99; }

/* margin auto trick — push item to far right */
.nav {
  display: flex;
  align-items: center;
  gap: 16px;
}
.logout { margin-left: auto; }

/* Universal centering */
.center-everything {
  display: flex;
  place-items: center;
  place-content: center;
  min-height: 100vh;
}
---
order: 2-3
margin-auto: 6-11
centering: 14-19
```
