---
title: Colors and typography
---

# How computers see color

{{show: color-systems typewriter 2s linear}} Three ways to describe a color in CSS. Each gives you the same result but makes you think differently.

{{pan: hex-section}} {{focus: hex-section}} {{zoom: 1.2x}} {{pulse: hex-section}} {{annotate: hex-section "Hex"}} Hex codes: six characters after a hash. Two for red, two for green, two for blue. `#3b82f6` means 3b red, 82 green, f6 blue. Compact, but the numbers mean nothing to your eyes.

{{zoom: 1x}} {{pan: rgb-section}} {{focus: rgb-section}} {{zoom: 1.2x}} {{pulse: rgb-section}} {{annotate: rgb-section "RGB"}} `rgb()` spells it out: red 59, green 130, blue 246. Same color as the hex above. Slightly more readable, but you still can't predict what "59, 130, 246" looks like.

{{zoom: 1x}} {{pan: hsl-section}} {{focus: hsl-section}} {{zoom: 1.3x}} {{pulse: hsl-section}} {{annotate: hsl-section "HSL: the human way"}} `hsl()` uses hue, saturation, lightness. Hue is the color angle on a wheel — 0 is red, 120 is green, 240 is blue. Saturation is vividness, 0% is gray, 100% is full color. Lightness is brightness, 0% is black, 100% is white.

{{zoom: 1x}} {{pan: none}} {{focus: none}} HSL is the one you should use. You can reason about it: "I want a medium blue, very saturated, slightly light." That's `hsl(220, 91%, 60%)`. Try describing that in hex.

```code:color-systems lang=css
/* Hex: compact but opaque */
.hex { color: #3b82f6; }
.hex-short { color: #38f; } /* 3-char shorthand */
.hex-alpha { color: #3b82f680; } /* with 50% opacity */

/* RGB: explicit channels */
.rgb { color: rgb(59, 130, 246); }
.rgba { color: rgb(59 130 246 / 0.5); } /* modern syntax */

/* HSL: human-readable */
.hsl { color: hsl(220, 91%, 60%); }
.hsla { color: hsl(220 91% 60% / 0.5); }
---
hex-section: 2-4
rgb-section: 7-8
hsl-section: 11-12
```

# Building a palette with HSL

{{clear: slide}}

{{split}} {{show: palette-code reveal 0.3s}} {{show: palette-preview reveal 0.5s spring}} Pick one hue. Vary saturation and lightness. You get a complete color palette.

{{focus: base-hue}} {{zoom: 1.2x}} {{pulse: base-hue}} {{annotate: base-hue "Pick a hue"}} 220 is blue. That's our brand hue. One number. Everything derives from it.

{{zoom: 1x}} {{focus: light-shades}} {{pulse: light-shades}} {{annotate: light-shades "Lighter variants"}} Higher lightness for backgrounds and subtle tints. 95% is almost white with a blue hint. 85% is a light wash.

{{focus: dark-shades}} {{pulse: dark-shades}} {{annotate: dark-shades "Darker variants"}} Lower lightness for text and emphasis. 40% for body text. 15% for headings. These are the same hue — they just live at different points on the lightness axis.

{{focus: gray-scale}} {{pulse: gray-scale}} {{annotate: gray-scale "Desaturated grays"}} Drop saturation to 10-20% and you get tinted grays. They're not pure gray — they carry a subtle warmth from the hue. Much more refined than `#666`.

{{focus: none}} {{unsplit}}

```code:palette-code lang=css
:root {
  /* Base hue */
  --hue: 220;

  /* Light shades (backgrounds, surfaces) */
  --color-50: hsl(var(--hue), 90%, 95%);
  --color-100: hsl(var(--hue), 85%, 90%);
  --color-200: hsl(var(--hue), 80%, 85%);

  /* Mid shades (borders, secondary text) */
  --color-400: hsl(var(--hue), 70%, 55%);
  --color-500: hsl(var(--hue), 91%, 60%);

  /* Dark shades (text, headings) */
  --color-700: hsl(var(--hue), 75%, 40%);
  --color-900: hsl(var(--hue), 80%, 15%);

  /* Tinted grays */
  --gray-100: hsl(var(--hue), 15%, 95%);
  --gray-500: hsl(var(--hue), 10%, 45%);
  --gray-900: hsl(var(--hue), 15%, 10%);
}
---
base-hue: 3
light-shades: 6-8
dark-shades: 14-15
gray-scale: 18-20
```

```preview:palette-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 16px; }
  .swatch-row { display: flex; gap: 4px; margin-bottom: 8px; }
  .swatch { width: 48px; height: 48px; border-radius: 6px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
</style>
<div class="label">Blue palette (hue: 220)</div>
<div class="swatch-row">
  <div class="swatch" style="background: hsl(220, 90%, 95%)"></div>
  <div class="swatch" style="background: hsl(220, 85%, 90%)"></div>
  <div class="swatch" style="background: hsl(220, 80%, 85%)"></div>
  <div class="swatch" style="background: hsl(220, 70%, 55%)"></div>
  <div class="swatch" style="background: hsl(220, 91%, 60%)"></div>
  <div class="swatch" style="background: hsl(220, 75%, 40%)"></div>
  <div class="swatch" style="background: hsl(220, 80%, 15%)"></div>
</div>
<div class="label">Tinted grays</div>
<div class="swatch-row">
  <div class="swatch" style="background: hsl(220, 15%, 95%)"></div>
  <div class="swatch" style="background: hsl(220, 10%, 75%)"></div>
  <div class="swatch" style="background: hsl(220, 10%, 45%)"></div>
  <div class="swatch" style="background: hsl(220, 15%, 10%)"></div>
</div>
```

# CSS custom properties

{{clear: slide}}

{{show: custom-props typewriter 1.5s linear}} Custom properties — also called CSS variables — let you define a value once and reuse it everywhere.

{{focus: definition}} {{zoom: 1.2x}} {{pulse: definition}} {{annotate: definition "Define once"}} Declare custom properties on `:root` (the html element). The `--` prefix is required. The value can be anything — a color, a length, a font stack, even another custom property.

{{zoom: 1x}} {{pan: usage}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Use everywhere"}} Reference them with `var()`. The browser substitutes the value at render time. Change the variable, and every element using it updates.

{{zoom: 1x}} {{pan: override}} {{focus: override}} {{zoom: 1.2x}} {{pulse: override}} {{annotate: override "Scoped override"}} Custom properties cascade and inherit. Override one on a specific element and all its children see the new value. This is how you do per-component theming — override `--bg` on a dark card and everything inside adapts.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Custom properties replace the need for Sass variables in most cases. They're live — they work with media queries, JavaScript, and inheritance. Sass variables are compile-time only.

```code:custom-props lang=css
/* Define on :root */
:root {
  --color-primary: hsl(220, 91%, 60%);
  --color-text: hsl(220, 80%, 15%);
  --spacing-md: 16px;
  --font-body: system-ui, -apple-system, sans-serif;
}

/* Use with var() */
body {
  color: var(--color-text);
  font-family: var(--font-body);
  padding: var(--spacing-md);
}

/* Scoped override */
.dark-card {
  --color-text: hsl(220, 15%, 95%);
  --color-primary: hsl(220, 80%, 70%);
  background: hsl(220, 25%, 12%);
  color: var(--color-text);
}
---
definition: 2-7
usage: 10-14
override: 17-22
```

# Typography

{{clear: slide}}

{{show: type-code typewriter 2s linear}} Good typography makes content readable. Bad typography makes it invisible. The difference is a few CSS properties.

{{pan: font-stack}} {{focus: font-stack}} {{zoom: 1.2x}} {{pulse: font-stack}} {{annotate: font-stack "System font stack"}} The system font stack uses the operating system's built-in fonts. San Francisco on Mac. Segoe UI on Windows. Fast — no font file to download. Familiar — matches the OS aesthetic.

{{zoom: 1x}} {{pan: font-size}} {{focus: font-size}} {{zoom: 1.2x}} {{annotate: font-size "Sizing with rem"}} `rem` is relative to the root font size, usually 16px. `1rem` is 16px. `1.25rem` is 20px. Always use rem for font sizes — it respects the user's browser font settings. Hard-coded pixel sizes override accessibility preferences.

{{zoom: 1x}} {{pan: line-height}} {{focus: line-height}} {{zoom: 1.2x}} {{pulse: line-height}} {{annotate: line-height "Breathing room"}} `line-height` controls the space between lines of text. 1.6 is the sweet spot for body copy. Unitless values multiply by the font size — `line-height: 1.6` on a 16px font means 25.6px line height.

{{zoom: 1x}} {{pan: letter-spacing}} {{focus: letter-spacing}} {{zoom: 1.2x}} {{annotate: letter-spacing "Tracking"}} `letter-spacing` adjusts the space between characters. Negative for tight headings. Positive for small all-caps labels. A little goes a long way — `0.05em` is usually enough.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:type-code lang=css
/* System font stack */
body {
  font-family:
    system-ui,
    -apple-system,
    BlinkMacSystemFont,
    "Segoe UI",
    Roboto,
    sans-serif;
}

/* Font sizing with rem */
h1 { font-size: 2.25rem; }   /* 36px */
h2 { font-size: 1.5rem; }    /* 24px */
h3 { font-size: 1.25rem; }   /* 20px */
body { font-size: 1rem; }    /* 16px base */
small { font-size: 0.875rem; } /* 14px */

/* Line height: unitless multiplier */
body {
  line-height: 1.6;
}
h1, h2, h3 {
  line-height: 1.2; /* Tighter for headings */
}

/* Letter spacing */
h1 { letter-spacing: -0.02em; } /* Tight headings */
.label {
  text-transform: uppercase;
  letter-spacing: 0.05em; /* Wider for small caps */
  font-size: 0.75rem;
}
---
font-stack: 2-9
font-size: 12-16
line-height: 19-24
letter-spacing: 27-32
```

# Fluid typography

{{clear: slide}}

{{split}} {{show: fluid-code reveal 0.3s}} {{show: fluid-preview reveal 0.5s spring}} Fixed font sizes look good at one screen width and wrong at others. `clamp()` makes text fluid — it scales between a minimum and maximum based on viewport width.

{{focus: clamp-syntax}} {{zoom: 1.2x}} {{pulse: clamp-syntax}} {{annotate: clamp-syntax "clamp(min, preferred, max)"}} Three values. The minimum size the text will ever be. The preferred size — a viewport-relative calculation that scales smoothly. The maximum size it caps at.

{{zoom: 1x}} {{focus: heading-fluid}} {{pulse: heading-fluid}} {{annotate: heading-fluid "Fluid heading"}} This heading is 1.5rem on a small phone, grows fluidly, and caps at 3rem on a wide desktop. One line of CSS. No media queries.

{{focus: body-fluid}} {{annotate: body-fluid "Fluid body"}} Body text scales between 1rem and 1.25rem. Subtle but it makes reading comfortable on both small and large screens.

{{focus: none}} {{unsplit}}

```code:fluid-code lang=css
/* clamp(min, preferred, max) */
h1 {
  font-size: clamp(1.5rem, 1rem + 2vw, 3rem);
}

/* Fluid body text */
body {
  font-size: clamp(1rem, 0.9rem + 0.5vw, 1.25rem);
}

/* Fluid spacing too */
.section {
  padding: clamp(16px, 2vw + 8px, 48px);
}
---
clamp-syntax: 2-4
heading-fluid: 3
body-fluid: 8
```

```preview:fluid-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 16px; color: #1e293b; }
  h1 { font-size: clamp(1.25rem, 1rem + 2vw, 2.5rem); line-height: 1.2; margin-bottom: 8px; }
  p { font-size: clamp(0.875rem, 0.8rem + 0.3vw, 1.1rem); line-height: 1.6; color: #475569; }
  .note { font-size: 11px; color: #94a3b8; margin-top: 12px; border-top: 1px solid #e2e8f0; padding-top: 8px; }
</style>
<h1>Fluid Typography</h1>
<p>This text scales smoothly between screen sizes. No media queries, no breakpoints. The heading grows from 1.25rem to 2.5rem. The body text adjusts subtly between 0.875rem and 1.1rem. Resize the window to see it.</p>
<div class="note">Resize your browser to see the text scale fluidly</div>
```
