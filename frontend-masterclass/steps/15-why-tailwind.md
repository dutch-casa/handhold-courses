---
title: Why Tailwind?
---

# The scaling problem

{{show: scale-problem typewriter 2s linear}} You know how to write CSS. You know selectors, the cascade, flexbox, grid. So why do teams reach for a framework? Because CSS at scale has three unsolved problems.

{{focus: dead-css}} {{zoom: 1.2x}} {{pulse: dead-css}} {{annotate: dead-css "Dead CSS"}} You ship CSS nobody uses. A button style gets removed from HTML but stays in the stylesheet. After months, ten percent of your CSS matches nothing on any page. Tools exist to purge it, but they're fragile and require configuration.

{{zoom: 1x}} {{focus: naming}} {{zoom: 1.2x}} {{pulse: naming}} {{annotate: naming "Naming fatigue"}} Every component needs a class name. `.card-header-subtitle-small-muted`. You spend as much time naming things as writing the actual styles. Then a teammate picks a different convention. Then naming drifts. Then you're hunting through CSS files to find what styles what.

{{zoom: 1x}} {{focus: specificity}} {{zoom: 1.2x}} {{pulse: specificity}} {{annotate: specificity "Specificity creep"}} `.sidebar .card .title` needs to override `.card .title`. You add a selector. Someone else adds `!important`. Three months later, the only way to override anything is more `!important`. Specificity wars are technical debt that compounds daily.

{{zoom: 1x}} {{focus: none}} These problems scale with team size and project age. A solo project works fine with plain CSS. A team of ten, shipping for two years? The stylesheet becomes a liability.

```code:scale-problem lang=css
/* Dead CSS: nobody uses these anymore */
.old-banner { background: red; }
.promo-2024 { display: flex; }
.hero-v2 { height: 100vh; }

/* Naming fatigue: inventing names endlessly */
.card-header-subtitle-small-muted { ... }
.sidebar-nav-link-active-hover { ... }
.main-content-section-title-large { ... }

/* Specificity creep: overrides cascade */
.card .title { color: blue; }
.sidebar .card .title { color: green; }
#page .sidebar .card .title { color: red !important; }
---
dead-css: 2-4
naming: 7-9
specificity: 12-14
```

# The utility-first idea

{{clear: slide}}

{{split}} {{show: utility-code typewriter 2s linear}} {{show: utility-preview reveal 0.5s spring}} Tailwind inverts the CSS workflow. Instead of writing custom selectors, you compose predefined utility classes directly in your markup.

{{focus: traditional}} {{zoom: 1.2x}} {{pulse: traditional}} {{annotate: traditional "Traditional approach"}} Write HTML. Create a class name. Open the CSS file. Write declarations. Go back to HTML. Repeat. Two files stay in sync. The class name is the bridge between them — and the source of all naming problems.

{{zoom: 1x}} {{focus: utility}} {{zoom: 1.2x}} {{pulse: utility}} {{annotate: utility "Utility approach"}} Write HTML. Add classes. Done. `p-4` is `padding: 1rem`. `text-lg` is `font-size: 1.125rem`. `bg-blue-500` is a specific shade of blue. No naming. No separate file. No bridge to maintain.

{{zoom: 1x}} {{focus: design-system}} {{zoom: 1.2x}} {{pulse: design-system}} {{annotate: design-system "Built-in constraints"}} The classes enforce a design system. `p-4` is 1rem. `p-5` is 1.25rem. There is no `p-4.5`. The spacing scale (0, 1, 2, 3, 4, 5, 6, 8, 10, 12, 16...) prevents random values. The color palette prevents `#37a2eb` appearing next to `#3b82f6`. Constraints breed consistency.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:utility-code lang=html
<!-- Traditional: name everything, style separately -->
<div class="card">
  <h2 class="card-title">Hello</h2>
  <p class="card-body">World</p>
</div>

<!-- Utility-first: compose in markup -->
<div class="rounded-lg border border-gray-200 p-4">
  <h2 class="text-lg font-bold text-gray-900">Hello</h2>
  <p class="text-sm text-gray-600">World</p>
</div>

<!-- The design system is the class names:
     p-1 = 0.25rem, p-2 = 0.5rem, p-3 = 0.75rem
     p-4 = 1rem,    p-5 = 1.25rem, p-6 = 1.5rem
     No p-4.5. No random padding. -->
---
traditional: 2-5
utility: 8-11
design-system: 13-16
```

```preview:utility-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 6px; }
  .demo-card {
    border: 1px solid #e2e8f0; border-radius: 0.5rem; padding: 1rem;
    margin-bottom: 10px;
  }
  .demo-card h2 { font-size: 1.125rem; font-weight: 700; color: #111827; margin: 0 0 4px; }
  .demo-card p { font-size: 0.875rem; color: #4b5563; margin: 0; }
  .scale { display: flex; gap: 4px; margin-bottom: 6px; }
  .scale-item { height: 32px; border-radius: 4px; display: flex; align-items: center; justify-content: center; font-size: 9px; color: white; font-weight: 600; }
</style>
<div class="label">Utility-composed card:</div>
<div class="demo-card">
  <h2>Hello</h2>
  <p>World</p>
</div>
<div class="label">Spacing scale (enforced consistency):</div>
<div class="scale">
  <div class="scale-item" style="width: 16px; background: #3b82f6">1</div>
  <div class="scale-item" style="width: 24px; background: #3b82f6">2</div>
  <div class="scale-item" style="width: 32px; background: #3b82f6">3</div>
  <div class="scale-item" style="width: 40px; background: #3b82f6">4</div>
  <div class="scale-item" style="width: 48px; background: #3b82f6">5</div>
  <div class="scale-item" style="width: 56px; background: #2563eb">6</div>
  <div class="scale-item" style="width: 72px; background: #1d4ed8">8</div>
</div>
```

# How Tailwind works

{{clear: slide}}

{{show: pipeline-diagram reveal 0.3s spring}} {{show: pipeline-code typewriter 2s linear}} Tailwind is a build tool. It scans your source files, finds class names, and generates only the CSS you use.

{{focus: scan}} {{zoom: 1.2x}} {{pulse: scan}} {{annotate: scan "Step 1: Scan"}} Tailwind reads your HTML, JSX, and template files. It looks for class-like strings: `p-4`, `text-lg`, `bg-blue-500`. It doesn't understand your templates — it uses pattern matching. This is why dynamic class names like `bg-${color}-500` don't work. The full class string must exist in the source.

{{zoom: 1x}} {{focus: generate}} {{zoom: 1.2x}} {{pulse: generate}} {{annotate: generate "Step 2: Generate"}} For each detected class, Tailwind generates the CSS. `p-4` becomes `.p-4 { padding: 1rem; }`. `hover:bg-blue-600` becomes `.hover\:bg-blue-600:hover { background-color: #2563eb; }`. Only the classes you use get CSS. Nothing unused ships.

{{zoom: 1x}} {{focus: output}} {{zoom: 1.2x}} {{annotate: output "Step 3: Output"}} The result is a single CSS file with exactly the declarations you need. Typical Tailwind projects ship 10-20KB of CSS. Traditional projects often ship 100KB+ with most of it unused. The build is fast — the new Oxide engine compiles in under 100 milliseconds.

{{zoom: 1x}} {{focus: none}}

```code:pipeline-code lang=text
Source files (HTML, JSX, Vue, etc.)
  ↓  Scan for class names
  ↓  "p-4", "text-lg", "bg-blue-500"
  ↓
Tailwind Compiler (Oxide engine)
  ↓  Generate CSS for each class
  ↓  .p-4 { padding: 1rem; }
  ↓  .text-lg { font-size: 1.125rem; }
  ↓  .bg-blue-500 { background: #3b82f6; }
  ↓
Output CSS (only what you used)
  ↓  ~10-20KB gzipped
  ↓  Zero dead code
---
scan: 1-3
generate: 5-9
output: 11-13
```

```diagram:pipeline-diagram kind=graph
Source [label="Source files" shape=box]
Scanner [label="Class scanner"]
Compiler [label="Oxide engine"]
Output [label="CSS output" shape=box]
Dead [label="No dead code"]

Source -> Scanner
Scanner -> Compiler
Compiler -> Output
Output -> Dead
```

# Responsive and state variants

{{clear: slide}}

{{split}} {{show: variant-code typewriter 2s linear}} {{show: variant-preview reveal 0.5s spring}} Tailwind handles responsive design and interactive states with prefix modifiers. No media queries to write.

{{focus: responsive}} {{zoom: 1.2x}} {{pulse: responsive}} {{annotate: responsive "Responsive prefixes"}} `sm:`, `md:`, `lg:`, `xl:` — breakpoint prefixes. `grid-cols-1 md:grid-cols-2 lg:grid-cols-3` means one column on mobile, two on medium screens, three on large. Mobile-first by default — unprefixed styles are the base.

{{zoom: 1x}} {{focus: state}} {{zoom: 1.2x}} {{pulse: state}} {{annotate: state "State variants"}} `hover:bg-blue-600` applies on hover. `focus:ring-2` adds a ring on focus. `active:scale-95` scales down on click. Stack them: `hover:bg-blue-600 focus:ring-2 active:scale-95`. Each variant adds its condition.

{{zoom: 1x}} {{focus: dark}} {{zoom: 1.2x}} {{annotate: dark "Dark mode"}} `dark:bg-gray-900 dark:text-white` — the dark prefix targets dark mode. It uses `prefers-color-scheme` by default. You can switch it to class-based for manual toggle. One set of markup, two themes.

{{zoom: 1x}} {{focus: group}} {{zoom: 1.2x}} {{pulse: group}} {{annotate: group "Group and peer"}} `group-hover:` styles a child when the parent is hovered. `peer-invalid:` styles an element when a sibling input is invalid. These replace CSS combinators with readable class names.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:variant-code lang=html
<!-- Responsive: mobile-first breakpoints -->
<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
  <div>Card 1</div>
  <div>Card 2</div>
  <div>Card 3</div>
</div>

<!-- State: hover, focus, active -->
<button class="bg-blue-500 hover:bg-blue-600
  focus:ring-2 focus:ring-blue-300
  active:scale-95 transition">
  Click me
</button>

<!-- Dark mode -->
<div class="bg-white dark:bg-gray-900
  text-gray-900 dark:text-white">
  Adapts to theme
</div>

<!-- Group hover: parent triggers child style -->
<div class="group">
  <h2 class="group-hover:text-blue-600">Title</h2>
  <p class="group-hover:underline">Read more</p>
</div>
---
responsive: 2-6
state: 9-13
dark: 16-19
group: 22-25
```

```preview:variant-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .btn-demo {
    background: #3b82f6; color: white; border: none;
    padding: 8px 16px; border-radius: 6px; font-size: 13px;
    font-weight: 600; cursor: pointer; transition: all 0.15s ease-out;
  }
  .btn-demo:hover { background: #2563eb; }
  .btn-demo:active { transform: scale(0.95); }
  .btn-demo:focus { outline: none; box-shadow: 0 0 0 3px rgba(59,130,246,0.3); }
  .group-demo { padding: 12px; border: 1px solid #e2e8f0; border-radius: 6px; cursor: pointer; transition: background 0.15s; }
  .group-demo:hover { background: #f8fafc; }
  .group-demo:hover h3 { color: #2563eb; }
  .group-demo:hover .more { text-decoration: underline; }
  .group-demo h3 { margin: 0 0 4px; font-size: 13px; transition: color 0.15s; }
  .group-demo .more { font-size: 12px; color: #64748b; margin: 0; }
</style>
<div class="label">Hover, focus, and active states:</div>
<button class="btn-demo">Click me</button>
<div class="label">Group hover (hover the card):</div>
<div class="group-demo">
  <h3>Card Title</h3>
  <p class="more">Read more →</p>
</div>
```

# Is it just inline styles?

{{clear: slide}}

{{show: notinline-code typewriter 1.5s linear}} The first reaction is always "isn't this just inline styles?" No. Here's why.

{{focus: pseudo}} {{zoom: 1.2x}} {{pulse: pseudo}} {{annotate: pseudo "Pseudo-classes"}} Inline styles can't do `:hover`, `:focus`, `::before`, or any pseudo-class. Tailwind can. `hover:bg-blue-600` generates a real CSS rule with a `:hover` pseudo-class.

{{zoom: 1x}} {{focus: media}} {{zoom: 1.2x}} {{annotate: media "Media queries"}} Inline styles can't be responsive. Tailwind's `md:grid-cols-2` generates a `@media (min-width: 768px)` rule. Container queries work too: `@sm:flex`.

{{zoom: 1x}} {{focus: tokens}} {{zoom: 1.2x}} {{pulse: tokens}} {{annotate: tokens "Design tokens"}} Inline styles use arbitrary values — `style="padding: 13px"` is possible. Tailwind restricts to a scale: `p-3` (12px) or `p-4` (16px). No in-between. This constraint prevents visual inconsistency across a team.

{{zoom: 1x}} {{focus: devtools}} {{zoom: 1.2x}} {{annotate: devtools "Cascade and layers"}} Tailwind classes participate in the cascade. They can be overridden by specificity. They work with `@layer`. Inline styles override everything and can't be overridden without `!important`. Tailwind stays within the CSS system.

{{zoom: 1x}} {{focus: none}}

```code:notinline-code lang=html
<!-- Things inline styles CAN'T do: -->

<!-- 1. Pseudo-classes -->
<button class="hover:bg-blue-600 focus:ring-2">
  <!-- Inline: impossible -->
</button>

<!-- 2. Media queries / responsive -->
<div class="grid-cols-1 md:grid-cols-3">
  <!-- Inline: impossible -->
</div>

<!-- 3. Design tokens (constrained values) -->
<div class="p-4">   <!-- Always 1rem -->
<div style="padding: 13px"> <!-- Random value -->

<!-- 4. Cascade participation -->
<!-- Tailwind classes can be overridden in CSS -->
<!-- Inline styles can only be beaten by !important -->
---
pseudo: 4-6
media: 9-11
tokens: 14-15
devtools: 18-19
```
