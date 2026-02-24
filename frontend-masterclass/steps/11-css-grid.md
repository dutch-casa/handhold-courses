---
title: CSS Grid
---

# Two dimensions

{{show: grid-intro typewriter 2s linear}} Flexbox is one axis. Grid is two. You define rows and columns at the same time. The browser places items into cells.

{{focus: grid-def}} {{zoom: 1.2x}} {{pulse: grid-def}} {{annotate: grid-def "Grid container"}} `display: grid` on the parent. The children become grid items. Like flexbox, they leave normal flow and enter grid formatting context.

{{zoom: 1x}} {{focus: columns}} {{zoom: 1.2x}} {{pulse: columns}} {{annotate: columns "Defining columns"}} `grid-template-columns` defines the column tracks. `200px 1fr 1fr` creates three columns: one fixed at 200 pixels, two that split the remaining space equally. The `fr` unit is "fraction of available space."

{{zoom: 1x}} {{focus: rows}} {{zoom: 1.2x}} {{annotate: rows "Defining rows"}} `grid-template-rows` does the same for rows. But you rarely define rows explicitly — content creates them automatically. You define columns to control the layout shape. Rows grow as needed.

{{zoom: 1x}} {{focus: gap}} {{zoom: 1.2x}} {{pulse: gap}} {{annotate: gap "Grid gap"}} `gap` works exactly like in flexbox. Space between tracks, not at the edges. `row-gap` and `column-gap` for independent control.

{{zoom: 1x}} {{focus: none}}

```code:grid-intro lang=css
/* Grid container */
.grid {
  display: grid;
}

/* Defining columns */
.three-col {
  grid-template-columns: 200px 1fr 1fr;
}
.equal-thirds {
  grid-template-columns: 1fr 1fr 1fr;
}

/* Defining rows (usually optional) */
.explicit-rows {
  grid-template-rows: auto 1fr auto;
}

/* Gap */
.grid {
  gap: 16px;
  /* or separately: */
  row-gap: 24px;
  column-gap: 16px;
}
---
grid-def: 2-4
columns: 7-12
rows: 15-17
gap: 20-25
```

# Repeat and fr units

{{clear: slide}}

{{split}} {{show: repeat-code typewriter 2s linear}} {{show: repeat-preview reveal 0.5s spring}} Typing `1fr 1fr 1fr 1fr` is tedious. `repeat()` does it in fewer characters — and enables responsive patterns that are impossible without it.

{{focus: repeat-basic}} {{zoom: 1.2x}} {{pulse: repeat-basic}} {{annotate: repeat-basic "repeat()"}} `repeat(3, 1fr)` creates three equal columns. `repeat(4, 200px)` creates four fixed columns. The first argument is the count, the second is the track definition.

{{zoom: 1x}} {{focus: mixed}} {{zoom: 1.2x}} {{annotate: mixed "Mixed tracks"}} You can mix repeat with fixed tracks. `200px repeat(3, 1fr)` creates a fixed sidebar and three fluid columns. `repeat(2, 1fr 2fr)` alternates between narrow and wide.

{{zoom: 1x}} {{focus: auto-fill}} {{zoom: 1.2x}} {{pulse: auto-fill}} {{annotate: auto-fill "Auto-fill"}} `repeat(auto-fill, minmax(250px, 1fr))`. This is the responsive grid holy grail. `auto-fill` creates as many columns as fit. `minmax(250px, 1fr)` means each column is at least 250 pixels and grows to fill space. No media queries. The grid self-adapts.

{{zoom: 1x}} {{focus: auto-fit}} {{zoom: 1.2x}} {{annotate: auto-fit "Auto-fit vs auto-fill"}} `auto-fit` collapses empty tracks. `auto-fill` keeps them. If you have three items in a five-column grid: auto-fill leaves two empty columns visible. auto-fit stretches the three items to fill the row. Use auto-fit when you want items to grow into empty space.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:repeat-code lang=css
/* Basic repeat */
.grid { grid-template-columns: repeat(3, 1fr); }
.fixed { grid-template-columns: repeat(4, 200px); }

/* Mixed with repeat */
.sidebar-layout {
  grid-template-columns: 250px repeat(3, 1fr);
}

/* auto-fill: responsive grid, no media queries */
.responsive {
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
}

/* auto-fit: items stretch to fill empty space */
.stretch {
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
}
---
repeat-basic: 2-3
mixed: 6-8
auto-fill: 11-13
auto-fit: 16-18
```

```preview:repeat-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .grid-demo { display: grid; gap: 6px; margin-bottom: 4px; }
  .cell { background: #3b82f6; color: white; padding: 10px; border-radius: 4px; font-size: 11px; font-weight: 600; text-align: center; }
</style>
<div class="label">repeat(3, 1fr) — three equal columns</div>
<div class="grid-demo" style="grid-template-columns: repeat(3, 1fr);">
  <div class="cell">1</div><div class="cell">2</div><div class="cell">3</div>
</div>
<div class="label">250px repeat(2, 1fr) — sidebar + fluid</div>
<div class="grid-demo" style="grid-template-columns: 80px repeat(2, 1fr);">
  <div class="cell" style="background:#0f172a">Side</div><div class="cell">Main</div><div class="cell">Extra</div>
</div>
<div class="label">auto-fill, minmax(80px, 1fr) — responsive</div>
<div class="grid-demo" style="grid-template-columns: repeat(auto-fill, minmax(80px, 1fr));">
  <div class="cell">A</div><div class="cell">B</div><div class="cell">C</div><div class="cell">D</div><div class="cell">E</div>
</div>
```

# Grid template areas

{{clear: slide}}

{{split}} {{show: areas-code typewriter 2s linear}} {{show: areas-preview reveal 0.5s spring}} Grid template areas let you draw your layout in ASCII art. The code looks like the result.

{{focus: area-def}} {{zoom: 1.2x}} {{pulse: area-def}} {{annotate: area-def "The template"}} Each string is a row. Each word is a cell name. Repeat a name to span columns. The visual shape in the code is the visual shape on screen. `"header header header"` spans the header across all three columns.

{{zoom: 1x}} {{focus: area-assign}} {{zoom: 1.2x}} {{pulse: area-assign}} {{annotate: area-assign "Assigning items"}} Each item declares `grid-area` with the name from the template. The browser places it in the matching cells. No line numbers, no coordinates — just names.

{{zoom: 1x}} {{focus: area-dot}} {{zoom: 1.2x}} {{annotate: area-dot "Empty cells"}} A dot (`.`) in the template creates an empty cell. `". main main"` leaves the first column of that row empty.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:areas-code lang=css
/* Draw the layout */
.page {
  display: grid;
  grid-template-columns: 200px 1fr 1fr;
  grid-template-rows: auto 1fr auto;
  grid-template-areas:
    "header header header"
    "sidebar main   main"
    "footer  footer  footer";
  gap: 16px;
  min-height: 100vh;
}

/* Assign items to areas */
.page-header  { grid-area: header; }
.page-sidebar { grid-area: sidebar; }
.page-main    { grid-area: main; }
.page-footer  { grid-area: footer; }

/* Empty cell with dot */
.alt-layout {
  grid-template-areas:
    "header header"
    ".      main"
    "footer footer";
}
---
area-def: 2-10
area-assign: 13-16
area-dot: 19-23
```

```preview:areas-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .page-demo {
    display: grid;
    grid-template-columns: 70px 1fr 1fr;
    grid-template-rows: 30px 80px 30px;
    grid-template-areas:
      "header header header"
      "sidebar main main"
      "footer footer footer";
    gap: 4px; height: 150px;
  }
  .page-demo > div { border-radius: 4px; display: flex; align-items: center; justify-content: center; font-size: 11px; font-weight: 600; }
  .hdr { grid-area: header; background: #0f172a; color: white; }
  .side { grid-area: sidebar; background: #f0fdf4; border: 1px solid #86efac; color: #166534; }
  .mn { grid-area: main; background: #eff6ff; border: 1px solid #93c5fd; color: #1d4ed8; }
  .ftr { grid-area: footer; background: #fef3c7; border: 1px solid #fcd34d; color: #92400e; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
</style>
<div class="label">grid-template-areas layout:</div>
<div class="page-demo">
  <div class="hdr">header</div>
  <div class="side">sidebar</div>
  <div class="mn">main</div>
  <div class="ftr">footer</div>
</div>
```

# Placing items

{{clear: slide}}

{{show: placement-code typewriter 2s linear}} Sometimes you need manual placement — items that span multiple tracks or sit at exact positions.

{{focus: col-span}} {{zoom: 1.2x}} {{pulse: col-span}} {{annotate: col-span "Column span"}} `grid-column: 1 / 3` places an item from line 1 to line 3 — spanning two columns. Grid lines are numbered from 1, not 0. The end line is exclusive. `grid-column: 1 / -1` spans from start to end regardless of column count.

{{zoom: 1x}} {{focus: row-span}} {{zoom: 1.2x}} {{annotate: row-span "Row span"}} `grid-row` works the same way. `grid-row: 1 / 3` spans two rows. Combine column and row spans and you can make an item cover a rectangular block of cells.

{{zoom: 1x}} {{focus: span-keyword}} {{zoom: 1.2x}} {{pulse: span-keyword}} {{annotate: span-keyword "Span keyword"}} `grid-column: span 2` says "take up two columns from wherever I naturally land." No start line needed. This is simpler when you don't care about exact position — just size.

{{zoom: 1x}} {{focus: dense}} {{zoom: 1.2x}} {{annotate: dense "Dense packing"}} `grid-auto-flow: dense` fills holes left by large items. Without it, a big item leaves gaps. With dense, the browser moves smaller items earlier to fill them. Use it for galleries and masonry-like layouts.

{{zoom: 1x}} {{focus: none}}

```code:placement-code lang=css
/* Column span: line 1 to line 3 */
.full-width { grid-column: 1 / -1; }
.two-cols   { grid-column: 1 / 3; }

/* Row span */
.tall { grid-row: 1 / 3; }

/* Span keyword — relative sizing */
.wide { grid-column: span 2; }
.tall-span { grid-row: span 3; }

/* Dense packing — fill gaps */
.gallery {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
  grid-auto-flow: dense;
}
.gallery .featured {
  grid-column: span 2;
  grid-row: span 2;
}
---
col-span: 2-3
row-span: 6
span-keyword: 9-10
dense: 13-20
```

# Subgrid

{{clear: slide}}

{{split}} {{show: subgrid-code typewriter 2s linear}} {{show: subgrid-preview reveal 0.5s spring}} When you nest a grid inside a grid, the inner grid has its own tracks. The parent's and child's columns don't align. Subgrid fixes this.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "The alignment problem"}} A card grid where each card has a title, body, and button. Without subgrid, each card's internal layout is independent. Titles end up at different heights because content varies.

{{zoom: 1x}} {{focus: solution}} {{zoom: 1.2x}} {{pulse: solution}} {{annotate: solution "Subgrid"}} `grid-template-rows: subgrid` on a child element tells it to use the parent's row tracks instead of defining its own. Now all cards in a row share the same row lines. Titles align. Bodies align. Buttons align.

{{zoom: 1x}} {{focus: both}} {{zoom: 1.2x}} {{annotate: both "Both axes"}} You can subgrid columns, rows, or both. `grid-template-columns: subgrid` inherits the parent's column tracks. This is powerful for forms where labels and inputs need to align across rows.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:subgrid-code lang=css
/* The alignment problem */
.card-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 16px;
}
.card {
  /* Each card has its own independent rows */
  display: grid;
  grid-template-rows: auto 1fr auto;
}

/* Subgrid: inherit parent's row tracks */
.card-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  grid-template-rows: auto 1fr auto;
  gap: 16px;
}
.card {
  grid-row: span 3;
  display: grid;
  grid-template-rows: subgrid;
}

/* Subgrid on both axes */
.form-grid {
  display: grid;
  grid-template-columns: auto 1fr;
}
.form-row {
  grid-column: 1 / -1;
  display: grid;
  grid-template-columns: subgrid;
}
---
problem: 2-10
solution: 13-23
both: 26-33
```

```preview:subgrid-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
  .cards {
    display: grid; grid-template-columns: 1fr 1fr;
    grid-template-rows: auto 1fr auto; gap: 6px;
  }
  .card {
    grid-row: span 3; display: grid; grid-template-rows: subgrid;
    border: 1px solid #e2e8f0; border-radius: 6px; overflow: hidden; gap: 0;
  }
  .card-title { background: #0f172a; color: white; padding: 8px 10px; font-size: 12px; font-weight: 600; }
  .card-body { padding: 8px 10px; font-size: 11px; color: #475569; }
  .card-action { padding: 8px 10px; border-top: 1px solid #f1f5f9; }
  .card-action a { font-size: 11px; color: #3b82f6; text-decoration: none; }
</style>
<div class="label">Subgrid: titles and buttons align across cards</div>
<div class="cards">
  <div class="card">
    <div class="card-title">Short Title</div>
    <div class="card-body">Brief description.</div>
    <div class="card-action"><a href="#">Read more</a></div>
  </div>
  <div class="card">
    <div class="card-title">Longer Title With More Words</div>
    <div class="card-body">This card has more content that takes up more vertical space in the body area.</div>
    <div class="card-action"><a href="#">Read more</a></div>
  </div>
</div>
```

# Grid patterns

{{clear: slide}}

{{show: grid-patterns typewriter 2s linear}} Patterns that cover most layouts you'll build.

{{focus: dashboard}} {{zoom: 1.2x}} {{pulse: dashboard}} {{annotate: dashboard "Dashboard"}} Full viewport height. Header spans the top. Sidebar on the left. Content fills the rest. Template areas make the intent readable.

{{zoom: 1x}} {{focus: magazine}} {{zoom: 1.2x}} {{pulse: magazine}} {{annotate: magazine "Magazine layout"}} A featured article spans two columns and two rows. The rest auto-place around it. `grid-auto-flow: dense` fills the gaps. Mix fixed and responsive tracks for visual variety.

{{zoom: 1x}} {{focus: responsive}} {{zoom: 1.2x}} {{pulse: responsive}} {{annotate: responsive "Responsive gallery"}} `auto-fill` plus `minmax`. One line of CSS handles every screen width. Items are at least 200 pixels, grow to fill space, and wrap to new rows automatically. Zero media queries.

{{zoom: 1x}} {{focus: none}}

```code:grid-patterns lang=css
/* Dashboard layout */
.dashboard {
  display: grid;
  grid-template-columns: 240px 1fr;
  grid-template-rows: 60px 1fr;
  grid-template-areas:
    "header header"
    "sidebar main";
  min-height: 100vh;
}

/* Magazine: featured item spans */
.magazine {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  grid-auto-flow: dense;
  gap: 12px;
}
.magazine .featured {
  grid-column: span 2;
  grid-row: span 2;
}

/* Responsive gallery — zero media queries */
.gallery {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
  gap: 16px;
}
---
dashboard: 2-9
magazine: 12-20
responsive: 23-27
```
