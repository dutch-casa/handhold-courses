---
title: Tailwind 4
---

# The config file is dead

{{show: config-gone typewriter 2s linear}} Tailwind 3 used `tailwind.config.js` — a JavaScript file where you defined colors, fonts, breakpoints, and plugins. Tailwind 4 killed it. Everything lives in CSS now.

{{focus: old-way}} {{zoom: 1.2x}} {{pulse: old-way}} {{annotate: old-way "Tailwind 3"}} A JavaScript config file. Import it, export an object. Define `theme.extend.colors`, `theme.extend.spacing`, content paths, plugins. A build tool reads this file and generates CSS. Config and output are different languages in different files.

{{zoom: 1x}} {{focus: new-way}} {{zoom: 1.2x}} {{pulse: new-way}} {{annotate: new-way "Tailwind 4"}} One CSS file. `@import "tailwindcss"` loads the framework. `@theme { }` defines your design tokens. Everything is CSS. No JavaScript config. No content paths — Tailwind auto-detects your source files.

{{zoom: 1x}} {{focus: import}} {{zoom: 1.2x}} {{annotate: import "The import"}} `@import "tailwindcss"` replaces the old `@tailwind base; @tailwind components; @tailwind utilities;` directives. One line. That's your entire setup.

{{zoom: 1x}} {{focus: none}} The config-to-CSS shift matters because your design system is now readable by any CSS tool, not locked inside a JavaScript object.

```code:config-gone lang=css
/* Tailwind 3: JavaScript config */
/* tailwind.config.js */
/* module.exports = { */
/*   content: ["./src/**/*.{html,jsx}"], */
/*   theme: { extend: { colors: { brand: "#3b82f6" }}}, */
/*   plugins: [], */
/* } */

/* Tailwind 4: CSS only */
@import "tailwindcss";

/* That's it. No config file needed. */
/* Source files are auto-detected. */
/* .gitignore paths are excluded. */
---
old-way: 2-7
new-way: 10
import: 10
```

# The @theme directive

{{clear: slide}}

{{split}} {{show: theme-code typewriter 2s linear}} {{show: theme-preview reveal 0.5s spring}} `@theme` is how you define your design system. Every variable you declare becomes utility classes.

{{focus: colors}} {{zoom: 1.2x}} {{pulse: colors}} {{annotate: colors "Custom colors"}} `--color-brand-500: oklch(0.62 0.19 250)` inside `@theme` generates `bg-brand-500`, `text-brand-500`, `border-brand-500`, and every other color utility. The namespace `--color-*` is the trigger. Tailwind maps the suffix to class names.

{{zoom: 1x}} {{focus: spacing}} {{zoom: 1.2x}} {{annotate: spacing "Spacing"}} `--spacing: 4px` sets the base unit. Every spacing utility multiplies by this value. `p-4` becomes `padding: calc(4 * 4px)` = 16px. Change the base and every spacing utility updates.

{{zoom: 1x}} {{focus: fonts}} {{zoom: 1.2x}} {{pulse: fonts}} {{annotate: fonts "Fonts and text"}} `--font-display: "Satoshi", sans-serif` generates the `font-display` utility. `--text-hero: 3rem` generates `text-hero`. Every namespace maps to a category of utilities.

{{zoom: 1x}} {{focus: reset}} {{zoom: 1.2x}} {{annotate: reset "Resetting defaults"}} `--color-*: initial` wipes all default colors. Start from scratch: `--*: initial` clears everything. Then define only what you need. No unused utility classes in the output.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:theme-code lang=css
@import "tailwindcss";

@theme {
  /* Custom colors → generates bg-*, text-*, border-* */
  --color-brand-50: oklch(0.97 0.02 250);
  --color-brand-500: oklch(0.62 0.19 250);
  --color-brand-900: oklch(0.28 0.10 250);

  /* Spacing base unit */
  --spacing: 4px;

  /* Custom font → generates font-display */
  --font-display: "Satoshi", sans-serif;

  /* Custom text size → generates text-hero */
  --text-hero: 3rem;

  /* Custom easing → generates ease-fluid */
  --ease-fluid: cubic-bezier(0.3, 0, 0, 1);

  /* Custom animation → generates animate-fade-in */
  --animate-fade-in: fade-in 0.3s ease-out;

  @keyframes fade-in {
    from { opacity: 0; transform: scale(0.95); }
    to   { opacity: 1; transform: scale(1); }
  }
}
---
colors: 4-7
spacing: 10
fonts: 13-16
reset: 4
```

```preview:theme-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .palette { display: flex; gap: 3px; margin-bottom: 6px; }
  .swatch { width: 36px; height: 36px; border-radius: 6px; display: flex; align-items: center; justify-content: center; font-size: 8px; color: white; font-weight: 600; }
  .namespace { display: grid; grid-template-columns: auto 1fr; gap: 4px 12px; font-size: 11px; }
  .ns-key { font-family: monospace; color: #3b82f6; font-weight: 600; }
  .ns-val { color: #475569; }
</style>
<div class="label">Custom brand palette from @theme:</div>
<div class="palette">
  <div class="swatch" style="background: oklch(0.97 0.02 250); color: #334155">50</div>
  <div class="swatch" style="background: oklch(0.85 0.08 250)">200</div>
  <div class="swatch" style="background: oklch(0.62 0.19 250)">500</div>
  <div class="swatch" style="background: oklch(0.45 0.15 250)">700</div>
  <div class="swatch" style="background: oklch(0.28 0.10 250)">900</div>
</div>
<div class="label">Theme namespace → utility mapping:</div>
<div class="namespace">
  <span class="ns-key">--color-*</span><span class="ns-val">bg-*, text-*, border-*</span>
  <span class="ns-key">--font-*</span><span class="ns-val">font-sans, font-display</span>
  <span class="ns-key">--spacing</span><span class="ns-val">p-*, m-*, gap-*, w-*</span>
  <span class="ns-key">--text-*</span><span class="ns-val">text-sm, text-hero</span>
  <span class="ns-key">--ease-*</span><span class="ns-val">ease-fluid, ease-snappy</span>
</div>
```

# Custom utilities and variants

{{clear: slide}}

{{show: custom-code typewriter 2s linear}} When Tailwind doesn't have a utility you need, create one. When it doesn't have a variant, create that too.

{{focus: utility}} {{zoom: 1.2x}} {{pulse: utility}} {{annotate: utility "@utility"}} `@utility tab-4 { tab-size: 4; }` creates the class `tab-4`. Use it in HTML like any built-in class. `@utility` replaces the `addUtilities()` plugin API from v3. No JavaScript plugin file — just CSS.

{{zoom: 1x}} {{focus: variant}} {{zoom: 1.2x}} {{pulse: variant}} {{annotate: variant "@custom-variant"}} `@custom-variant hocus (&:hover, &:focus)` creates a `hocus:` prefix. Now `hocus:text-blue-600` applies on both hover and focus. Common custom variants: `hocus` (hover + focus), `optional` (form fields), `inverted-colors` (accessibility).

{{zoom: 1x}} {{focus: source}} {{zoom: 1.2x}} {{annotate: source "Source detection"}} `@source "../node_modules/@my-company/ui"` tells Tailwind to scan additional directories. `@source not "./legacy"` excludes paths. Tailwind 4 auto-detects sources and respects `.gitignore`, so you rarely need this.

{{zoom: 1x}} {{focus: none}}

```code:custom-code lang=css
@import "tailwindcss";

/* Custom utility: generates .tab-4 class */
@utility tab-4 {
  tab-size: 4;
}

/* Custom utility with values */
@utility scrollbar-hidden {
  scrollbar-width: none;
  &::-webkit-scrollbar { display: none; }
}

/* Custom variants */
@custom-variant hocus (&:hover, &:focus);
@custom-variant optional (&:optional);
@custom-variant inverted-colors (@media (inverted-colors: inverted));

/* Source detection overrides */
@source "../packages/ui-lib";
@source not "./src/legacy";
---
utility: 4-12
variant: 15-17
source: 20-21
```

# Container queries in Tailwind 4

{{clear: slide}}

{{split}} {{show: cq-code typewriter 2s linear}} {{show: cq-preview reveal 0.5s spring}} Container queries are first-class in Tailwind 4. No plugin. No configuration.

{{focus: container}} {{zoom: 1.2x}} {{pulse: container}} {{annotate: container "@container class"}} Add `@container` to a parent element. Its children can now query its width with `@sm:`, `@md:`, `@lg:` prefixes. These are container breakpoints, not viewport breakpoints.

{{zoom: 1x}} {{focus: breakpoints}} {{zoom: 1.2x}} {{annotate: breakpoints "Container breakpoints"}} `@sm` is 20rem (320px). `@md` is 28rem (448px). `@lg` is 32rem (512px). You can define custom container breakpoints in `@theme` with `--container-*` namespace.

{{zoom: 1x}} {{focus: named}} {{zoom: 1.2x}} {{pulse: named}} {{annotate: named "Named containers"}} `@container/sidebar` creates a named container. `@sm/sidebar:text-lg` queries that specific container. Useful when you have nested containers and need to target the right one.

{{zoom: 1x}} {{focus: arbitrary}} {{zoom: 1.2x}} {{annotate: arbitrary "Arbitrary values"}} `@[500px]:flex` — arbitrary container breakpoint. No theme token needed. Square brackets escape to any width.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:cq-code lang=html
<!-- Container context -->
<div class="@container">
  <!-- Responds to this container's width -->
  <div class="grid grid-cols-1 @sm:grid-cols-2 @lg:grid-cols-3">
    <div>Card</div>
  </div>
</div>

<!-- Named container -->
<div class="@container/sidebar">
  <p class="text-sm @sm/sidebar:text-base @lg/sidebar:text-lg">
    Responds to sidebar width
  </p>
</div>

<!-- Arbitrary container breakpoint -->
<div class="@container">
  <div class="block @[500px]:flex @[800px]:grid">
    Precise breakpoints
  </div>
</div>

<!-- Custom container breakpoints in @theme -->
<!-- @theme { --container-card: 24rem; } -->
<!-- Then use: @card:grid-cols-2 -->
---
container: 2-7
named: 10-14
arbitrary: 17-21
breakpoints: 24-25
```

```preview:cq-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .wide-wrap { container-type: inline-size; width: 100%; }
  .narrow-wrap { container-type: inline-size; width: 200px; }
  .cq-grid { display: grid; gap: 4px; }
  @container (min-width: 320px) { .cq-grid { grid-template-columns: 1fr 1fr; } }
  @container (min-width: 500px) { .cq-grid { grid-template-columns: 1fr 1fr 1fr; } }
  .cq-item { background: #eff6ff; border: 1px solid #bfdbfe; padding: 8px; border-radius: 4px; font-size: 11px; text-align: center; }
</style>
<div class="label">Wide container → 3 columns:</div>
<div class="wide-wrap">
  <div class="cq-grid">
    <div class="cq-item">A</div><div class="cq-item">B</div><div class="cq-item">C</div>
  </div>
</div>
<div class="label">Narrow container → 1 column:</div>
<div class="narrow-wrap">
  <div class="cq-grid">
    <div class="cq-item">A</div><div class="cq-item">B</div><div class="cq-item">C</div>
  </div>
</div>
```

# Tailwind 4.1 features

{{clear: slide}}

{{show: v41-code typewriter 2s linear}} Tailwind 4.1 adds text shadows, masks, pointer variants, and safe alignment. Here's what matters most.

{{focus: text-shadow}} {{zoom: 1.2x}} {{pulse: text-shadow}} {{annotate: text-shadow "Text shadows"}} `text-shadow-sm` adds a subtle text shadow. `text-shadow-lg` for dramatic effect. Color them: `text-shadow-lg text-shadow-sky-300`. Opacity: `text-shadow-lg/50`. These replace the custom plugin that every project used to carry.

{{zoom: 1x}} {{focus: pointer}} {{zoom: 1.2x}} {{pulse: pointer}} {{annotate: pointer "Pointer variants"}} `pointer-coarse:p-6 pointer-fine:p-2` — larger touch targets on touchscreens, tighter spacing with a mouse. This is the CSS `@media (pointer: coarse)` query in utility form. Essential for accessible mobile design.

{{zoom: 1x}} {{focus: safe-align}} {{zoom: 1.2x}} {{annotate: safe-align "Safe alignment"}} `justify-center-safe` falls back to `start` when items would overflow the container. Regular `justify-center` can hide content off the left edge on narrow screens. The `safe` keyword prevents this.

{{zoom: 1x}} {{focus: not-variant}} {{zoom: 1.2x}} {{pulse: not-variant}} {{annotate: not-variant "Not variant"}} `not-hover:opacity-75` — dim when NOT hovered. `not-last:border-b` — border on every item except the last. The `not-*` prefix negates any variant. It replaces patterns like `last:border-0` with a more readable intent.

{{zoom: 1x}} {{focus: none}}

```code:v41-code lang=html
<!-- Text shadows (new in 4.1) -->
<h1 class="text-shadow-lg text-shadow-sky-300">
  Glowing heading
</h1>
<p class="text-shadow-sm">Subtle depth</p>

<!-- Pointer variants: touch vs mouse -->
<button class="pointer-coarse:p-6 pointer-coarse:text-lg
  pointer-fine:p-2 pointer-fine:text-sm">
  Adapts to input device
</button>

<!-- Safe alignment: prevents overflow -->
<div class="flex justify-center-safe gap-2 overflow-x-auto">
  <div>Item</div><div>Item</div><div>Item</div>
</div>

<!-- Not variant: negate any condition -->
<div class="not-hover:opacity-75 transition">
  Dims when not hovered
</div>
<ul>
  <li class="not-last:border-b py-2">Item 1</li>
  <li class="not-last:border-b py-2">Item 2</li>
  <li class="not-last:border-b py-2">Item 3</li>
</ul>

<!-- Descendant variant: style all children -->
<div class="*:text-center *:py-2">
  <p>Centered</p>
  <p>Also centered</p>
</div>
---
text-shadow: 2-5
pointer: 8-11
safe-align: 14-16
not-variant: 19-25
```

# Composing with @apply

{{clear: slide}}

{{show: apply-code typewriter 1.5s linear}} `@apply` extracts utility classes into a CSS rule. It's the bridge between utility-first and component classes. Use it for third-party component styles and very frequent patterns.

{{focus: usage}} {{zoom: 1.2x}} {{pulse: usage}} {{annotate: usage "Basic @apply"}} `@apply rounded-lg border p-4 shadow-sm` puts those utilities into a single class. The generated CSS contains the same declarations. Use it when you need a class name — for third-party libraries, CMS content, or markdown prose styling.

{{zoom: 1x}} {{focus: caution}} {{zoom: 1.2x}} {{pulse: caution}} {{annotate: caution "When NOT to use it"}} Don't `@apply` everything. That recreates the naming and dead-CSS problems Tailwind was built to solve. If a pattern appears in JSX, extract a component instead. `@apply` is for CSS-only contexts: global styles, prose, third-party overrides.

{{zoom: 1x}} {{focus: none}} Tailwind's philosophy: use utility classes in markup. Only reach for `@apply` when markup isn't an option.

```code:apply-code lang=css
@import "tailwindcss";

/* Use @apply for CSS-only contexts */
.prose h1 {
  @apply text-2xl font-bold tracking-tight text-gray-900;
}
.prose p {
  @apply text-base leading-relaxed text-gray-600;
}

/* Third-party component override */
.datepicker-input {
  @apply rounded-lg border border-gray-200 px-3 py-2
    text-sm focus:ring-2 focus:ring-blue-500;
}

/* DON'T: extracting everything defeats the purpose */
/* .btn-primary { @apply bg-blue-500 text-white ... } */
/* Instead, make a <Button> component in React */
---
usage: 4-10
caution: 18-19
```
