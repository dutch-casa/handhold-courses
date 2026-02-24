---
title: Tailwind core
---

# Spacing and sizing

{{split}} {{show: spacing-code typewriter 2s linear}} {{show: spacing-preview reveal 0.5s spring}} You know the CSS. Now learn the shorthand. Tailwind's spacing scale maps directly to rem values.

{{focus: padding}} {{zoom: 1.2x}} {{pulse: padding}} {{annotate: padding "Padding"}} `p-4` is `padding: 1rem`. `px-4` is horizontal padding. `py-2` is vertical padding. `pt-8` is padding-top only. The number times 0.25rem gives the value. `p-4` = 4 times 0.25rem = 1rem. `p-8` = 8 times 0.25rem = 2rem.

{{zoom: 1x}} {{focus: margin}} {{zoom: 1.2x}} {{annotate: margin "Margin"}} Same pattern. `m-4`, `mx-auto`, `mt-8`, `mb-0`. Negative margins use a dash prefix: `-mt-4` is `margin-top: -1rem`. `mx-auto` centers a block with `margin-left: auto; margin-right: auto`.

{{zoom: 1x}} {{focus: sizing}} {{zoom: 1.2x}} {{pulse: sizing}} {{annotate: sizing "Width and height"}} `w-full` is 100%. `w-1/2` is 50%. `w-64` is 16rem (256px). `h-screen` is 100vh. `min-h-screen` is `min-height: 100vh`. `max-w-lg` caps at 32rem. The fractions and fixed sizes cover every common layout.

{{zoom: 1x}} {{focus: arbitrary}} {{zoom: 1.2x}} {{annotate: arbitrary "Escape hatch"}} When the scale doesn't have what you need: `w-[350px]`, `p-[13px]`, `mt-[2.5rem]`. Square brackets escape to any CSS value. Use sparingly — if you're using arbitrary values constantly, define a theme token instead.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:spacing-code lang=html
<!-- Padding: p, px, py, pt, pr, pb, pl -->
<div class="p-4">All sides: 1rem</div>
<div class="px-6 py-3">Horizontal: 1.5rem, Vertical: 0.75rem</div>
<div class="pt-8 pb-4">Top: 2rem, Bottom: 1rem</div>

<!-- Margin: m, mx, my, mt, mr, mb, ml -->
<div class="mx-auto max-w-lg">Centered block</div>
<div class="mt-8 mb-4">Top: 2rem, Bottom: 1rem</div>
<div class="-mt-4">Negative margin: -1rem</div>

<!-- Width and height -->
<div class="w-full">100% width</div>
<div class="w-1/2">50% width</div>
<div class="w-64">16rem (256px)</div>
<div class="h-screen">100vh</div>
<div class="max-w-lg">Max 32rem</div>

<!-- Arbitrary values (escape hatch) -->
<div class="w-[350px] p-[13px]">Custom values</div>
---
padding: 2-4
margin: 7-9
sizing: 12-16
arbitrary: 19
```

```preview:spacing-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .scale-demo { display: flex; gap: 4px; align-items: flex-end; margin-bottom: 8px; }
  .scale-item { background: #3b82f6; border-radius: 3px; display: flex; align-items: center; justify-content: center; color: white; font-size: 9px; font-weight: 600; }
  .box-demo { display: flex; gap: 6px; }
  .box { border: 1px dashed #3b82f6; background: #eff6ff; border-radius: 4px; font-size: 10px; text-align: center; }
</style>
<div class="label">Spacing scale (p-1 through p-8):</div>
<div class="scale-demo">
  <div class="scale-item" style="width: 20px; height: 20px">1</div>
  <div class="scale-item" style="width: 24px; height: 24px">2</div>
  <div class="scale-item" style="width: 28px; height: 28px">3</div>
  <div class="scale-item" style="width: 36px; height: 36px">4</div>
  <div class="scale-item" style="width: 40px; height: 40px">5</div>
  <div class="scale-item" style="width: 48px; height: 48px">6</div>
  <div class="scale-item" style="width: 64px; height: 64px">8</div>
</div>
<div class="label">Width: w-full, w-1/2, w-1/3:</div>
<div class="box-demo">
  <div class="box" style="flex: 1; padding: 8px;">w-full</div>
</div>
<div class="box-demo" style="margin-top: 4px">
  <div class="box" style="flex: 1; padding: 8px;">w-1/2</div>
  <div class="box" style="flex: 1; padding: 8px;">w-1/2</div>
</div>
```

# Colors

{{clear: slide}}

{{split}} {{show: color-code typewriter 2s linear}} {{show: color-preview reveal 0.5s spring}} Tailwind ships a curated palette. Each color has shades from 50 (lightest) to 950 (darkest).

{{focus: bg}} {{zoom: 1.2x}} {{pulse: bg}} {{annotate: bg "Background colors"}} `bg-blue-500` sets the background. `bg-slate-100` for light surfaces. `bg-red-600` for danger. The color name and shade number. That's the pattern.

{{zoom: 1x}} {{focus: text}} {{zoom: 1.2x}} {{annotate: text "Text colors"}} `text-gray-900` for headings. `text-gray-600` for body. `text-white` on dark backgrounds. Same color names, `text-` prefix instead of `bg-`.

{{zoom: 1x}} {{focus: border}} {{zoom: 1.2x}} {{pulse: border}} {{annotate: border "Border colors"}} `border border-gray-200` adds a border and colors it. The `border` class adds `border-width: 1px`. `border-gray-200` adds the color. Two classes, two properties.

{{zoom: 1x}} {{focus: opacity}} {{zoom: 1.2x}} {{annotate: opacity "Opacity modifier"}} `bg-blue-500/50` means blue at 50% opacity. The slash modifier works on any color utility. `text-white/75`, `border-red-500/20`. No separate opacity class needed.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:color-code lang=html
<!-- Background colors -->
<div class="bg-blue-500">Blue background</div>
<div class="bg-slate-100">Light surface</div>
<div class="bg-red-600">Danger</div>

<!-- Text colors -->
<h1 class="text-gray-900">Heading</h1>
<p class="text-gray-600">Body text</p>
<span class="text-blue-600">Link color</span>

<!-- Border colors -->
<div class="border border-gray-200 rounded-lg">
  Bordered card
</div>

<!-- Opacity modifier (slash syntax) -->
<div class="bg-blue-500/50">50% opacity blue</div>
<div class="text-white/75">75% opacity white</div>
<div class="bg-black/10">Subtle overlay</div>
---
bg: 2-4
text: 7-9
border: 12-14
opacity: 17-19
```

```preview:color-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .palette { display: flex; gap: 3px; margin-bottom: 6px; }
  .swatch { width: 36px; height: 36px; border-radius: 4px; display: flex; align-items: center; justify-content: center; font-size: 8px; font-weight: 600; }
</style>
<div class="label">Blue palette (50–900):</div>
<div class="palette">
  <div class="swatch" style="background:#eff6ff;color:#1e40af">50</div>
  <div class="swatch" style="background:#dbeafe;color:#1e40af">100</div>
  <div class="swatch" style="background:#93c5fd;color:#1e3a5f">300</div>
  <div class="swatch" style="background:#3b82f6;color:white">500</div>
  <div class="swatch" style="background:#2563eb;color:white">600</div>
  <div class="swatch" style="background:#1d4ed8;color:white">700</div>
  <div class="swatch" style="background:#1e3a8a;color:white">900</div>
</div>
<div class="label">Slate palette (for text and surfaces):</div>
<div class="palette">
  <div class="swatch" style="background:#f8fafc;color:#334155">50</div>
  <div class="swatch" style="background:#f1f5f9;color:#334155">100</div>
  <div class="swatch" style="background:#94a3b8;color:white">400</div>
  <div class="swatch" style="background:#475569;color:white">600</div>
  <div class="swatch" style="background:#1e293b;color:white">800</div>
  <div class="swatch" style="background:#0f172a;color:white">950</div>
</div>
```

# Typography

{{clear: slide}}

{{show: type-code typewriter 2s linear}} Font size, weight, line height, letter spacing — all one class each.

{{focus: font-size}} {{zoom: 1.2x}} {{pulse: font-size}} {{annotate: font-size "Font size"}} `text-xs` through `text-9xl`. `text-base` is 1rem (16px). `text-lg` is 1.125rem. `text-2xl` is 1.5rem. Each size class also sets a default line-height, so text looks good without extra configuration.

{{zoom: 1x}} {{focus: weight}} {{zoom: 1.2x}} {{annotate: weight "Font weight"}} `font-thin` (100) through `font-black` (900). `font-normal` is 400. `font-medium` is 500. `font-semibold` is 600. `font-bold` is 700. These map directly to CSS `font-weight` values.

{{zoom: 1x}} {{focus: leading}} {{zoom: 1.2x}} {{pulse: leading}} {{annotate: leading "Line height"}} `leading-none` is 1. `leading-tight` is 1.25. `leading-normal` is 1.5. `leading-relaxed` is 1.625. "Leading" is the typography term for line spacing — it comes from the lead strips typesetters placed between lines of metal type.

{{zoom: 1x}} {{focus: tracking}} {{zoom: 1.2x}} {{annotate: tracking "Letter spacing"}} `tracking-tighter` (-0.05em) through `tracking-widest` (0.1em). `tracking-tight` for headings. `tracking-wide` for all-caps labels. Same idea as CSS `letter-spacing`.

{{zoom: 1x}} {{focus: none}}

```code:type-code lang=html
<!-- Font size -->
<p class="text-sm">Small (0.875rem)</p>
<p class="text-base">Base (1rem)</p>
<p class="text-lg">Large (1.125rem)</p>
<p class="text-2xl">2XL (1.5rem)</p>

<!-- Font weight -->
<span class="font-normal">Normal (400)</span>
<span class="font-medium">Medium (500)</span>
<span class="font-semibold">Semibold (600)</span>
<span class="font-bold">Bold (700)</span>

<!-- Line height (leading) -->
<p class="leading-tight">Tight lines (1.25)</p>
<p class="leading-normal">Normal lines (1.5)</p>
<p class="leading-relaxed">Relaxed lines (1.625)</p>

<!-- Letter spacing (tracking) -->
<h1 class="tracking-tight">Tight heading</h1>
<span class="text-xs uppercase tracking-widest">Label</span>
---
font-size: 2-5
weight: 8-11
leading: 14-16
tracking: 19-20
```

# Layout

{{clear: slide}}

{{split}} {{show: layout-code typewriter 2s linear}} {{show: layout-preview reveal 0.5s spring}} Flexbox and grid in utility form. You already know the CSS — these are the shorthand class names.

{{focus: flex}} {{zoom: 1.2x}} {{pulse: flex}} {{annotate: flex "Flexbox"}} `flex` enables flexbox. `flex-col` sets column direction. `items-center` for vertical centering. `justify-between` for space-between. `gap-4` for spacing. These map one-to-one to the CSS properties you learned.

{{zoom: 1x}} {{focus: grid}} {{zoom: 1.2x}} {{pulse: grid}} {{annotate: grid "Grid"}} `grid` enables grid. `grid-cols-3` creates three equal columns. `grid-cols-[200px_1fr]` uses an arbitrary template. `col-span-2` makes an item span two columns. `gap-4` works the same as in flex.

{{zoom: 1x}} {{focus: responsive}} {{zoom: 1.2x}} {{pulse: responsive}} {{annotate: responsive "Responsive layout"}} `grid-cols-1 md:grid-cols-2 lg:grid-cols-3` — one column on mobile, two on tablet, three on desktop. Prefix any layout utility with a breakpoint. Mobile-first, layer up.

{{zoom: 1x}} {{focus: container}} {{zoom: 1.2x}} {{annotate: container "Container"}} `@container` makes a container query context. `@sm:grid-cols-2` responds to the container width, not the viewport. Built into Tailwind 4 — no plugin needed.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:layout-code lang=html
<!-- Flexbox -->
<div class="flex items-center justify-between gap-4">
  <span>Left</span>
  <span>Right</span>
</div>

<!-- Flex column -->
<div class="flex flex-col gap-2">
  <div>Top</div>
  <div>Bottom</div>
</div>

<!-- Grid -->
<div class="grid grid-cols-3 gap-4">
  <div>1</div><div>2</div><div>3</div>
</div>

<!-- Responsive grid -->
<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
  <div>Card</div><div>Card</div><div>Card</div>
</div>

<!-- Container queries (Tailwind 4) -->
<div class="@container">
  <div class="grid grid-cols-1 @sm:grid-cols-2 @lg:grid-cols-3">
    <div>Responds to container</div>
  </div>
</div>
---
flex: 2-5
grid: 13-15
responsive: 18-20
container: 23-27
```

```preview:layout-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .flex-demo { display: flex; align-items: center; justify-content: space-between; gap: 8px; padding: 8px; background: #f8fafc; border-radius: 6px; margin-bottom: 4px; }
  .flex-demo span { font-size: 12px; background: #3b82f6; color: white; padding: 4px 10px; border-radius: 4px; }
  .grid-demo { display: grid; gap: 4px; margin-bottom: 4px; }
  .grid-demo div { background: #eff6ff; border: 1px solid #bfdbfe; padding: 6px; border-radius: 4px; font-size: 11px; text-align: center; }
</style>
<div class="label">flex justify-between:</div>
<div class="flex-demo">
  <span>Left</span><span>Center</span><span>Right</span>
</div>
<div class="label">grid-cols-3:</div>
<div class="grid-demo" style="grid-template-columns: repeat(3, 1fr)">
  <div>1</div><div>2</div><div>3</div>
</div>
<div class="label">Responsive: 1→2→3 columns:</div>
<div class="grid-demo" style="grid-template-columns: repeat(3, 1fr)">
  <div>Card</div><div>Card</div><div>Card</div>
</div>
```

# Borders, shadows, and effects

{{clear: slide}}

{{show: effects-code typewriter 1.5s linear}} The visual polish layer. Border radius, shadows, transitions — all single classes.

{{focus: rounded}} {{zoom: 1.2x}} {{pulse: rounded}} {{annotate: rounded "Border radius"}} `rounded` is 0.25rem. `rounded-md` is 0.375rem. `rounded-lg` is 0.5rem. `rounded-full` is 9999px (pill shape). `rounded-t-lg` rounds only the top corners. The scale matches common design system values.

{{zoom: 1x}} {{focus: shadow}} {{zoom: 1.2x}} {{pulse: shadow}} {{annotate: shadow "Shadows"}} `shadow-sm` for subtle elevation. `shadow` for medium. `shadow-lg` for cards. `shadow-xl` for modals. `shadow-none` removes shadows. Each one is a carefully tuned multi-layer box-shadow.

{{zoom: 1x}} {{focus: transition}} {{zoom: 1.2x}} {{annotate: transition "Transitions"}} `transition` enables transitions on common properties. `duration-200` sets 200ms. `ease-in-out` sets the timing function. `transition-colors` narrows to color properties only. Combine: `transition-colors duration-150 ease-out`.

{{zoom: 1x}} {{focus: transform}} {{zoom: 1.2x}} {{annotate: transform "Transforms"}} `scale-95` shrinks to 95%. `translate-y-1` moves down. `rotate-3` rotates 3 degrees. Combine with hover: `hover:scale-105 hover:-translate-y-1 transition`. That's a card lift effect in three classes.

{{zoom: 1x}} {{focus: none}}

```code:effects-code lang=html
<!-- Border radius -->
<div class="rounded-lg">0.5rem corners</div>
<div class="rounded-full">Pill shape</div>
<div class="rounded-t-lg rounded-b-none">Top only</div>

<!-- Shadows -->
<div class="shadow-sm">Subtle</div>
<div class="shadow-lg">Card elevation</div>
<div class="shadow-xl">Modal elevation</div>

<!-- Transitions -->
<button class="transition-colors duration-150 ease-out
  bg-blue-500 hover:bg-blue-600">
  Smooth color change
</button>

<!-- Transforms on hover -->
<div class="hover:scale-105 hover:-translate-y-1
  transition duration-200 ease-out shadow-md hover:shadow-xl">
  Card with lift effect
</div>
---
rounded: 2-4
shadow: 7-9
transition: 12-14
transform: 17-19
```

# Dark mode and responsive

{{clear: slide}}

{{show: dark-code typewriter 2s linear}} Two patterns you'll use in every Tailwind project.

{{focus: dark-pattern}} {{zoom: 1.2x}} {{pulse: dark-pattern}} {{annotate: dark-pattern "Dark mode"}} Prefix any utility with `dark:` and it applies only in dark mode. `bg-white dark:bg-gray-900`. `text-gray-900 dark:text-gray-100`. `border-gray-200 dark:border-gray-800`. Write both themes inline. No separate dark stylesheet.

{{zoom: 1x}} {{focus: responsive-pattern}} {{zoom: 1.2x}} {{pulse: responsive-pattern}} {{annotate: responsive-pattern "Responsive"}} `text-sm md:text-base lg:text-lg` — font size grows with screen. `p-4 md:p-6 lg:p-8` — padding grows with screen. `hidden md:block` — hidden on mobile, visible on tablet. `flex md:hidden` — visible on mobile, hidden on tablet.

{{zoom: 1x}} {{focus: stacking}} {{zoom: 1.2x}} {{annotate: stacking "Stacking variants"}} Variants stack. `dark:md:bg-gray-800` means dark mode AND medium screen. `hover:dark:text-blue-400` means hovering AND dark mode. Read left to right: each prefix adds a condition.

{{zoom: 1x}} {{focus: none}}

```code:dark-code lang=html
<!-- Dark mode pattern -->
<div class="bg-white dark:bg-gray-900
  text-gray-900 dark:text-gray-100
  border border-gray-200 dark:border-gray-800
  rounded-lg p-6 shadow-sm dark:shadow-none">
  <h2 class="text-lg font-bold">Card</h2>
  <p class="text-gray-600 dark:text-gray-400">
    Adapts to light and dark themes
  </p>
</div>

<!-- Responsive patterns -->
<div class="text-sm md:text-base lg:text-lg">
  Font grows with screen
</div>
<div class="p-4 md:p-6 lg:p-8">
  Padding grows with screen
</div>
<nav class="hidden md:flex gap-4">
  Desktop only nav
</nav>
<button class="md:hidden">Mobile menu</button>

<!-- Stacking: dark + responsive -->
<div class="bg-white dark:bg-gray-900
  md:bg-gray-50 dark:md:bg-gray-800">
  Different at every combination
</div>
---
dark-pattern: 2-9
responsive-pattern: 12-20
stacking: 23-26
```
