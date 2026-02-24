---
title: Responsive design
---

# The viewport

{{show: viewport-code typewriter 2s linear}} Before responsive design, websites had a fixed width. If your screen was smaller, you scrolled horizontally. That broke with the iPhone in 2007 — screens were suddenly 320 pixels wide.

{{focus: meta-tag}} {{zoom: 1.2x}} {{pulse: meta-tag}} {{annotate: meta-tag "The viewport meta"}} This meta tag is mandatory. Without it, mobile browsers pretend to be 980 pixels wide and scale the page down. With it, the browser reports its actual width. Every responsive site starts here.

{{zoom: 1x}} {{focus: width-setting}} {{zoom: 1.2x}} {{annotate: width-setting "device-width"}} `width=device-width` sets the viewport to the screen's width. `initial-scale=1` prevents default zooming. These two values cover every case.

{{zoom: 1x}} {{focus: none}} The viewport meta tag doesn't make your site responsive. It just tells the truth about screen size. Your CSS needs to handle the rest.

```code:viewport-code lang=html
<!-- The viewport meta tag -->
<meta name="viewport"
  content="width=device-width, initial-scale=1">

<!-- Without this, mobile browsers fake 980px width -->
<!-- With this, a 375px phone reports 375px -->
---
meta-tag: 2-3
width-setting: 3
```

# Media queries

{{clear: slide}}

{{show: media-code typewriter 2s linear}} Media queries apply CSS rules conditionally. The browser evaluates the condition and applies the styles only if it's true.

{{focus: min-width}} {{zoom: 1.2x}} {{pulse: min-width}} {{annotate: min-width "Min-width: mobile-first"}} `@media (min-width: 768px)` means "when the viewport is at least 768 pixels wide." Start with mobile styles as defaults. Layer on wider layouts in min-width queries. This is mobile-first design.

{{zoom: 1x}} {{focus: breakpoints}} {{zoom: 1.2x}} {{annotate: breakpoints "Common breakpoints"}} 640 for small phones. 768 for tablets. 1024 for laptops. 1280 for desktops. These aren't magic — they're where common layouts tend to break. Design to your content, not to device sizes.

{{zoom: 1x}} {{focus: preference}} {{zoom: 1.2x}} {{pulse: preference}} {{annotate: preference "User preferences"}} `prefers-color-scheme: dark` detects the OS theme. `prefers-reduced-motion: reduce` means the user wants less animation. These aren't layout queries — they're accessibility features. Respect them.

{{zoom: 1x}} {{focus: hover-query}} {{zoom: 1.2x}} {{annotate: hover-query "Hover capability"}} `@media (hover: hover)` targets devices with a fine pointer — mice, trackpads. Touch screens fail this query. Wrap hover-only effects in this query so touch users never see broken states.

{{zoom: 1x}} {{focus: none}}

```code:media-code lang=css
/* Mobile-first: base styles for small screens */
.grid { display: grid; grid-template-columns: 1fr; }

/* Tablet and up */
@media (min-width: 768px) {
  .grid { grid-template-columns: repeat(2, 1fr); }
}

/* Desktop and up */
@media (min-width: 1024px) {
  .grid { grid-template-columns: repeat(3, 1fr); }
}

/* Common breakpoints */
/* 640px  — small (phones)     */
/* 768px  — medium (tablets)   */
/* 1024px — large (laptops)    */
/* 1280px — extra large (wide) */

/* User preferences */
@media (prefers-color-scheme: dark) {
  :root {
    --color-bg: hsl(220, 25%, 10%);
    --color-text: hsl(220, 15%, 90%);
  }
}

@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after {
    animation-duration: 0.01ms !important;
    transition-duration: 0.01ms !important;
  }
}

/* Hover capability */
@media (hover: hover) {
  .card:hover { transform: translateY(-2px); }
}
---
min-width: 2-11
breakpoints: 14-17
preference: 20-31
hover-query: 34-36
```

# Container queries

{{clear: slide}}

{{split}} {{show: container-code typewriter 2s linear}} {{show: container-preview reveal 0.5s spring}} Media queries ask "how wide is the viewport?" Container queries ask "how wide is my container?" That's a paradigm shift.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "The problem"}} A card component in a sidebar needs a vertical layout. The same card in the main content area needs a horizontal layout. With media queries, you'd need to know where the card lives. That breaks component encapsulation.

{{zoom: 1x}} {{focus: containment}} {{zoom: 1.2x}} {{pulse: containment}} {{annotate: containment "Container context"}} `container-type: inline-size` on a parent element makes it a containment context. Now its children can query its width — not the viewport's width.

{{zoom: 1x}} {{focus: query}} {{zoom: 1.2x}} {{pulse: query}} {{annotate: query "@container"}} `@container (min-width: 400px)` applies styles when the container is at least 400 pixels wide. The card doesn't care if it's in a sidebar or main area. It adapts to its own space.

{{zoom: 1x}} {{focus: named}} {{zoom: 1.2x}} {{annotate: named "Named containers"}} `container-name: card-wrapper` and `@container card-wrapper (min-width: 500px)` — name your containers when you have nested ones. The query matches the nearest ancestor with that name.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:container-code lang=css
/* The problem: same card, different contexts */
/* sidebar: 250px wide → vertical */
/* main area: 800px wide → horizontal */

/* Container context */
.card-wrapper {
  container-type: inline-size;
}

/* Container query */
.card {
  display: grid;
  gap: 12px;
}
@container (min-width: 400px) {
  .card {
    grid-template-columns: 200px 1fr;
  }
}

/* Named containers */
.sidebar {
  container-type: inline-size;
  container-name: sidebar;
}
@container sidebar (min-width: 300px) {
  .widget { padding: 24px; }
}
---
problem: 1-3
containment: 6-8
query: 11-18
named: 21-26
```

```preview:container-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .wide-ctx { container-type: inline-size; width: 100%; }
  .narrow-ctx { container-type: inline-size; width: 160px; }
  .cq-card { display: grid; gap: 6px; border: 1px solid #e2e8f0; border-radius: 6px; overflow: hidden; }
  .cq-card .img { background: #3b82f6; height: 60px; }
  .cq-card .body { padding: 8px; }
  .cq-card .body h3 { margin: 0 0 2px; font-size: 12px; }
  .cq-card .body p { margin: 0; font-size: 11px; color: #64748b; }
  @container (min-width: 250px) {
    .cq-card { grid-template-columns: 80px 1fr; }
    .cq-card .img { height: auto; min-height: 60px; }
  }
</style>
<div class="label">Wide container → horizontal card</div>
<div class="wide-ctx">
  <div class="cq-card">
    <div class="img"></div>
    <div class="body"><h3>Adaptive Card</h3><p>Horizontal in wide containers</p></div>
  </div>
</div>
<div class="label">Narrow container → vertical card</div>
<div class="narrow-ctx">
  <div class="cq-card">
    <div class="img"></div>
    <div class="body"><h3>Same Card</h3><p>Vertical when narrow</p></div>
  </div>
</div>
```

# Responsive images

{{clear: slide}}

{{show: img-code typewriter 2s linear}} Images are the biggest performance problem on the web. Responsive images let the browser choose the right file for each screen.

{{focus: max-width}} {{zoom: 1.2x}} {{pulse: max-width}} {{annotate: max-width "Fluid images"}} `max-width: 100%` prevents images from overflowing their containers. `height: auto` maintains the aspect ratio. Every image needs this as a baseline. Without it, large images break layouts on small screens.

{{zoom: 1x}} {{focus: srcset}} {{zoom: 1.2x}} {{pulse: srcset}} {{annotate: srcset "Resolution switching"}} `srcset` lists multiple image files at different widths. `sizes` tells the browser how wide the image will be rendered. The browser calculates which file to download — 400w on a phone, 1200w on a retina desktop. One `img` tag, multiple files, optimal loading.

{{zoom: 1x}} {{focus: picture}} {{zoom: 1.2x}} {{annotate: picture "Art direction"}} The `picture` element handles art direction — different crops for different screens. A wide landscape shot on desktop. A tight square crop on mobile. The `source` elements define breakpoints and image files. The `img` is the fallback.

{{zoom: 1x}} {{focus: aspect}} {{zoom: 1.2x}} {{pulse: aspect}} {{annotate: aspect "Preventing layout shift"}} `aspect-ratio: 16 / 9` reserves space before the image loads. Without it, the page jumps when images appear. This eliminates layout shift — one of the core web vitals.

{{zoom: 1x}} {{focus: none}}

```code:img-code lang=html
<!-- Fluid images: never overflow -->
<style>
  img { max-width: 100%; height: auto; }
</style>

<!-- srcset: resolution switching -->
<img
  src="photo-800.jpg"
  srcset="photo-400.jpg 400w,
          photo-800.jpg 800w,
          photo-1200.jpg 1200w"
  sizes="(min-width: 768px) 50vw, 100vw"
  alt="A mountain landscape">

<!-- picture: art direction -->
<picture>
  <source media="(min-width: 768px)"
          srcset="hero-wide.jpg">
  <source media="(min-width: 480px)"
          srcset="hero-medium.jpg">
  <img src="hero-narrow.jpg" alt="Hero image">
</picture>

<!-- Prevent layout shift -->
<style>
  .hero-img { aspect-ratio: 16 / 9; object-fit: cover; }
</style>
---
max-width: 2-4
srcset: 7-13
picture: 16-22
aspect: 25-27
```

# Logical properties

{{clear: slide}}

{{show: logical-code typewriter 1.5s linear}} Physical properties like `margin-left` assume left-to-right text. Logical properties use `inline` and `block` instead — they adapt to writing direction.

{{focus: mapping}} {{zoom: 1.2x}} {{pulse: mapping}} {{annotate: mapping "Physical → Logical"}} `margin-left` becomes `margin-inline-start`. `margin-right` becomes `margin-inline-end`. `margin-top` becomes `margin-block-start`. `margin-bottom` becomes `margin-block-end`. Inline is the text direction. Block is perpendicular to it.

{{zoom: 1x}} {{focus: shorthand}} {{zoom: 1.2x}} {{annotate: shorthand "Shorthands"}} `margin-inline: 16px` sets both inline-start and inline-end. `padding-block: 24px 16px` sets block-start to 24 and block-end to 16. `inset-inline: 0` replaces `left: 0; right: 0`. Fewer properties, clearer intent.

{{zoom: 1x}} {{focus: sizing}} {{zoom: 1.2x}} {{pulse: sizing}} {{annotate: sizing "Logical sizing"}} `inline-size` replaces `width`. `block-size` replaces `height`. `max-inline-size: 640px` works in any writing direction. For languages that write vertically, inline-size is the height.

{{zoom: 1x}} {{focus: none}} Logical properties aren't just for internationalization. They make your CSS more intentional. "Start of the inline axis" says what you mean. "Left" is a physical direction that might not match the layout direction.

```code:logical-code lang=css
/* Physical → Logical mapping */
.old { margin-left: 16px; }
.new { margin-inline-start: 16px; }

.old { padding-top: 24px; padding-bottom: 16px; }
.new { padding-block: 24px 16px; }

.old { border-right: 1px solid; }
.new { border-inline-end: 1px solid; }

/* Shorthand properties */
.centered {
  margin-inline: auto;   /* left + right auto */
  max-inline-size: 640px; /* max-width */
}

/* Logical sizing */
.container {
  inline-size: 100%;       /* width: 100% */
  max-inline-size: 1200px; /* max-width */
  block-size: auto;        /* height: auto */
}

/* Logical inset (for positioning) */
.overlay {
  position: absolute;
  inset: 0; /* top + right + bottom + left = 0 */
}
---
mapping: 2-9
shorthand: 12-15
sizing: 18-22
```
