---
title: The polished UI
---

# Layered shadows

{{show: shadow-code typewriter 2s linear}} {{split}} {{show: shadow-preview reveal 0.3s spring}} A single `box-shadow` looks flat. Real objects cast multiple shadows at different distances and blurs. Layered shadows create depth that the eye accepts as real.

{{focus: single}} {{zoom: 1.2x}} {{pulse: single}} {{annotate: single "Single shadow"}} `box-shadow: 0 4px 12px rgba(0,0,0,0.15)` — one shadow, one layer of depth. It looks like a sticker floating over the page. Better than no shadow. But flat.

{{zoom: 1x}} {{focus: layered}} {{zoom: 1.2x}} {{pulse: layered}} {{annotate: layered "Layered shadows"}} Three shadows. The first is tight and dark — the contact shadow. The second is medium and softer — the main shadow. The third is wide and faint — the ambient shadow. Together they simulate light hitting a real object from above.

{{zoom: 1x}} {{focus: dark-shadow}} {{zoom: 1.2x}} {{pan: dark-shadow}} {{annotate: dark-shadow "Dark mode shadows"}} In dark mode, shadows become invisible against dark backgrounds. Replace shadows with subtle borders or glows. `box-shadow: 0 0 0 1px rgba(255,255,255,0.1)` gives a soft border effect. Or use `inset 0 1px 0 rgba(255,255,255,0.05)` for a top highlight.

{{pan: none}} {{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:shadow-code lang=css
/* Single shadow: flat */
.card-flat {
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
}

/* Layered shadows: depth */
.card-depth {
  box-shadow:
    0 1px 2px rgba(0, 0, 0, 0.07),
    0 4px 8px rgba(0, 0, 0, 0.07),
    0 12px 24px rgba(0, 0, 0, 0.05);
}

/* Hover: increase depth */
@media (hover: hover) {
  .card-depth:hover {
    box-shadow:
      0 2px 4px rgba(0, 0, 0, 0.07),
      0 8px 16px rgba(0, 0, 0, 0.1),
      0 24px 48px rgba(0, 0, 0, 0.08);
    transform: translateY(-2px);
  }
}

/* Dark mode: shadows → borders/glows */
@media (prefers-color-scheme: dark) {
  .card-depth {
    box-shadow:
      0 0 0 1px rgba(255, 255, 255, 0.08),
      inset 0 1px 0 rgba(255, 255, 255, 0.04);
  }
}
---
single: 2-4
layered: 7-12
dark-shadow: 25-31
```

```preview:shadow-preview
<style>
  .shadow-demo { display: flex; gap: 2rem; padding: 2rem; align-items: center; }
  .shadow-card { width: 160px; height: 100px; border-radius: 12px; background: white; display: flex; align-items: center; justify-content: center; font-size: 0.875rem; color: #64748b; }
  .s-flat { box-shadow: 0 4px 12px rgba(0,0,0,0.15); }
  .s-layered { box-shadow: 0 1px 2px rgba(0,0,0,0.07), 0 4px 8px rgba(0,0,0,0.07), 0 12px 24px rgba(0,0,0,0.05); }
</style>
<div class="shadow-demo">
  <div class="shadow-card s-flat">Single</div>
  <div class="shadow-card s-layered">Layered</div>
</div>
```

# Micro-interactions

{{clear: slide}}

{{show: micro-code typewriter 2s linear}} {{split}} {{show: micro-preview reveal 0.3s spring}} Micro-interactions are tiny animations that confirm actions. A button that presses in 2px on click. An input that glows on focus. A toggle that bounces into position. Each one says: "I heard you." Without them, the UI feels dead.

{{focus: press}} {{zoom: 1.2x}} {{pulse: press}} {{annotate: press "Button press"}} `transform: scale(0.97)` on `:active` — the button physically depresses. The scale is small enough to feel like a press, not a shrink. Combine with a shadow reduction for a tactile effect.

{{zoom: 1x}} {{focus: focus-glow}} {{zoom: 1.2x}} {{annotate: focus-glow "Input focus"}} A blue glow around the focused input: `box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.2)`. Not an outline — a soft glow. The transition is 150ms ease-out. The user's eye is drawn to where they're typing.

{{zoom: 1x}} {{focus: toggle}} {{zoom: 1.2x}} {{pulse: toggle}} {{annotate: toggle "Toggle switch"}} The thumb slides from left to right with a spring easing. The track color transitions. The thumb slightly grows on hover (scale 1.05). Each detail says: "this is a real object responding to you."

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:micro-code lang=css
/* Button press: physical feedback */
.button {
  transition: transform 100ms ease-out, box-shadow 100ms ease-out;
}
.button:active {
  transform: scale(0.97);
  box-shadow: 0 1px 2px rgba(0, 0, 0, 0.1); /* shadow shrinks too */
}

/* Input focus: glow ring */
.input {
  transition: box-shadow 150ms ease-out, border-color 150ms ease-out;
  border: 1px solid #d1d5db;
}
.input:focus {
  border-color: #2563eb;
  box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.2);
  outline: none;
}

/* Toggle switch */
.toggle-track {
  width: 48px;
  height: 28px;
  border-radius: 14px;
  background: #d1d5db;
  transition: background 200ms ease-out;
  position: relative;
  cursor: pointer;
}
.toggle-track[data-active="true"] {
  background: #2563eb;
}
.toggle-thumb {
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: white;
  position: absolute;
  top: 2px;
  left: 2px;
  transition: transform 200ms cubic-bezier(0.34, 1.56, 0.64, 1);
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.2);
}
.toggle-track[data-active="true"] .toggle-thumb {
  transform: translateX(20px);
}
---
press: 2-8
focus-glow: 11-19
toggle: 22-44
```

```preview:micro-preview
<style>
  .micro-demo { display: flex; gap: 2rem; padding: 1.5rem; align-items: center; flex-wrap: wrap; }
  .demo-btn { padding: 0.75rem 1.5rem; background: #2563eb; color: white; border: none; border-radius: 8px; font-size: 1rem; cursor: pointer; transition: transform 100ms ease-out, box-shadow 100ms ease-out; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
  .demo-btn:active { transform: scale(0.97); box-shadow: 0 1px 2px rgba(0,0,0,0.1); }
  .demo-input { padding: 0.75rem 1rem; border: 1px solid #d1d5db; border-radius: 8px; font-size: 1rem; transition: box-shadow 150ms ease-out, border-color 150ms ease-out; outline: none; }
  .demo-input:focus { border-color: #2563eb; box-shadow: 0 0 0 3px rgba(37,99,235,0.2); }
</style>
<div class="micro-demo">
  <button class="demo-btn">Press me</button>
  <input class="demo-input" placeholder="Focus me" />
</div>
```

# Dark mode

{{clear: slide}}

{{show: dark-code typewriter 2s linear}} Dark mode isn't color inversion. It's a separate design layer with different rules. Shadows become invisible. Borders become more important. White text on black is harsh — use off-white on near-black. The craft is in the adjustments.

{{focus: colors}} {{zoom: 1.2x}} {{pulse: colors}} {{annotate: colors "Color adjustments"}} Pure black (#000) is harsh. Use a dark gray (#0f172a or #1e293b). Pure white (#fff) text vibrates against dark backgrounds. Use off-white (#f1f5f9 or #e2e8f0). Reduce contrast slightly — dark mode is for comfort, not high-contrast testing.

{{zoom: 1x}} {{focus: surfaces}} {{zoom: 1.2x}} {{annotate: surfaces "Surface hierarchy"}} In light mode, depth comes from shadows. In dark mode, depth comes from lightness. The background is darkest. Cards are slightly lighter. Elevated elements are lighter still. Each layer adds a few points of lightness.

{{zoom: 1x}} {{focus: borders}} {{zoom: 1.2x}} {{pulse: borders}} {{pan: borders}} {{annotate: borders "Borders replace shadows"}} Shadows are invisible on dark backgrounds. Replace them with subtle borders: `border: 1px solid rgba(255, 255, 255, 0.08)`. Or a top highlight: `box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.05)`. These create separation without invisible shadows.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:dark-code lang=css
/* Dark mode: CSS custom properties */
:root {
  --bg: #ffffff;
  --bg-card: #ffffff;
  --text: #0f172a;
  --text-muted: #64748b;
  --border: #e2e8f0;
  --shadow: 0 1px 2px rgba(0, 0, 0, 0.07),
            0 4px 8px rgba(0, 0, 0, 0.07);
}

@media (prefers-color-scheme: dark) {
  :root {
    --bg: #0f172a;
    --bg-card: #1e293b;
    --text: #e2e8f0;
    --text-muted: #94a3b8;
    --border: rgba(255, 255, 255, 0.08);
    --shadow: none; /* shadows invisible in dark mode */
  }
}

/* Surface hierarchy via lightness */
.page { background: var(--bg); }
.card {
  background: var(--bg-card);
  border: 1px solid var(--border);
  box-shadow: var(--shadow);
}

/* Dark mode hover: glow instead of shadow */
@media (prefers-color-scheme: dark) {
  @media (hover: hover) {
    .card:hover {
      border-color: rgba(255, 255, 255, 0.15);
      box-shadow: 0 0 20px rgba(59, 130, 246, 0.1);
    }
  }
}
---
colors: 2-10
surfaces: 23-28
borders: 31-37
```

# Typography

{{clear: slide}}

{{show: type-code typewriter 2s linear}} Good typography is invisible. Bad typography is distracting. The details: antialiased rendering, optical sizing, a consistent type scale, and proper line heights. These take five minutes to set up and affect every pixel on the page.

{{focus: rendering}} {{zoom: 1.2x}} {{pulse: rendering}} {{annotate: rendering "Font rendering"}} `-webkit-font-smoothing: antialiased` and `-moz-osx-font-smoothing: grayscale` — subpixel rendering makes light text on dark backgrounds look bold and blurry. Antialiased rendering fixes this. Apply it globally.

{{zoom: 1x}} {{focus: scale}} {{zoom: 1.2x}} {{annotate: scale "Type scale"}} A consistent scale: 0.75rem, 0.875rem, 1rem, 1.125rem, 1.25rem, 1.5rem, 2rem, 3rem. Every text on the page uses one of these sizes. No random 13px or 17px. The scale creates visual rhythm.

{{zoom: 1x}} {{focus: line-height}} {{zoom: 1.2x}} {{pulse: line-height}} {{annotate: line-height "Line height"}} Body text: `line-height: 1.5`. Headings: `line-height: 1.2`. Tight line height on headings keeps them compact and weighty. Generous line height on body text keeps it readable. Never unitless values below 1.2.

{{zoom: 1x}} {{focus: none}}

```code:type-code lang=css
/* Global font rendering */
body {
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-rendering: optimizeLegibility;
  font-optical-sizing: auto;
}

/* Type scale */
:root {
  --text-xs: 0.75rem;    /* 12px */
  --text-sm: 0.875rem;   /* 14px */
  --text-base: 1rem;     /* 16px */
  --text-lg: 1.125rem;   /* 18px */
  --text-xl: 1.25rem;    /* 20px */
  --text-2xl: 1.5rem;    /* 24px */
  --text-3xl: 2rem;      /* 32px */
  --text-4xl: 3rem;      /* 48px */
}

/* Line heights */
body {
  font-size: var(--text-base);
  line-height: 1.5; /* body text: generous */
}

h1, h2, h3, h4 {
  line-height: 1.2; /* headings: tight */
  letter-spacing: -0.02em; /* optical tightening */
}

/* Tabular numbers for data */
.data, .price, .counter {
  font-variant-numeric: tabular-nums;
}
---
rendering: 2-7
scale: 10-19
line-height: 22-30
```

# Letter spacing and text wrapping

{{clear: slide}}

{{show: letter-code typewriter 2s linear}} Two details that separate polished typography from default typography. Larger text needs tighter letter spacing. Headings need balanced wrapping. These are small CSS additions with outsized visual impact.

{{focus: spacing}} {{zoom: 1.2x}} {{pulse: spacing}} {{annotate: spacing "Size-dependent spacing"}} Bigger text needs tighter tracking. At body size, the default spacing is fine. At heading size, characters look spread apart — tightening by `-0.02em` pulls them into a cohesive unit. At small label sizes, loosening by `0.01em` improves readability. Pair each font size with its optimal letter spacing inside a Text component or utility classes.

{{zoom: 1x}} {{focus: wrapping}} {{zoom: 1.2x}} {{annotate: wrapping "Balanced wrapping"}} `text-wrap: balance` distributes text evenly across lines instead of filling the first line and leaving a short orphan at the end. Apply it to headings. Body text stays at the default — balanced wrapping on long paragraphs can hurt readability. This is a one-line CSS addition that makes headlines look typeset instead of dumped.

{{zoom: 1x}} {{focus: chars}} {{zoom: 1.2x}} {{pulse: chars}} {{annotate: chars "Proper characters"}} Use real typographic characters. An ellipsis is `…`, not three dots. Apostrophes are `'`, not `'`. Quotes are `""`, not `""`. These feel pedantic until you see them at 48px heading size — then the difference is obvious.

{{zoom: 1x}} {{focus: none}}

```code:letter-code lang=css
/* Letter spacing: tighter for large, looser for small */
h1 {
  font-size: 2rem;
  letter-spacing: -0.02em;
  line-height: 1.2;
}

.label {
  font-size: 0.75rem;
  letter-spacing: 0.01em;
  text-transform: uppercase;
}

/* Balanced text wrapping on headings */
h1, h2, h3 {
  text-wrap: balance;
}

/* Proper typographic characters */
/* "Hello" not "Hello"  */
/* It's    not It's      */
/* Wait…   not Wait...   */
---
spacing: 2-10
wrapping: 13-15
chars: 18-20
```

# Loading skeletons

{{clear: slide}}

{{show: skeleton-code typewriter 2s linear}} {{split}} {{show: skeleton-preview reveal 0.3s spring}} A loading spinner says "wait." A skeleton says "here's what's coming." Skeletons match the layout of the content they replace. When the real content loads, it swaps in without layout shift — because the skeleton already occupied the right space.

{{focus: skeleton}} {{zoom: 1.2x}} {{pulse: skeleton}} {{annotate: skeleton "Shape of content"}} The skeleton mimics the layout: a circle for the avatar, a wide bar for the title, narrow bars for text lines. The proportions match the real content. When data arrives, the swap is seamless.

{{zoom: 1x}} {{focus: pulse}} {{zoom: 1.2x}} {{annotate: pulse "Pulse animation"}} A subtle pulse animation (`opacity: 0.4 → 0.7`) tells the user "loading, not broken." The pulse is slow — 1.5-2 seconds per cycle. Fast pulses feel frantic. Slow pulses feel calm and controlled.

{{zoom: 1x}} {{focus: reduced}} {{zoom: 1.2x}} {{pan: reduced}} {{annotate: reduced "Reduced motion"}} `prefers-reduced-motion: reduce` — disable the pulse. Show a static skeleton. Still better than a spinner because it reserves the layout space.

{{pan: none}} {{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:skeleton-code lang=css
/* Skeleton base */
.skeleton {
  background: #e2e8f0;
  border-radius: 4px;
  animation: pulse 2s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% { opacity: 0.4; }
  50% { opacity: 0.7; }
}

/* Skeleton shapes */
.skeleton-avatar {
  width: 48px;
  height: 48px;
  border-radius: 50%;
}

.skeleton-title {
  height: 1.5rem;
  width: 60%;
}

.skeleton-text {
  height: 1rem;
  width: 100%;
}

.skeleton-text-short {
  height: 1rem;
  width: 40%;
}

/* Reduced motion: static skeleton */
@media (prefers-reduced-motion: reduce) {
  .skeleton {
    animation: none;
    opacity: 0.5;
  }
}

/* Dark mode skeleton */
@media (prefers-color-scheme: dark) {
  .skeleton {
    background: #334155;
  }
}
---
skeleton: 2-6
pulse: 8-11
reduced: 35-39
```

```preview:skeleton-preview
<style>
  .skel-card { padding: 1rem; background: white; border-radius: 12px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); display: flex; gap: 1rem; width: 280px; }
  .skel { background: #e2e8f0; border-radius: 4px; animation: skel-pulse 2s ease-in-out infinite; }
  @keyframes skel-pulse { 0%, 100% { opacity: 0.4; } 50% { opacity: 0.7; } }
  .skel-avatar { width: 48px; height: 48px; border-radius: 50%; flex-shrink: 0; }
  .skel-body { flex: 1; display: flex; flex-direction: column; gap: 0.5rem; }
  .skel-title { height: 1rem; width: 70%; }
  .skel-line { height: 0.75rem; width: 100%; }
  .skel-line-short { height: 0.75rem; width: 50%; }
</style>
<div class="skel-card">
  <div class="skel skel-avatar"></div>
  <div class="skel-body">
    <div class="skel skel-title"></div>
    <div class="skel skel-line"></div>
    <div class="skel skel-line-short"></div>
  </div>
</div>
```

# Animation timing

{{clear: slide}}

{{show: timing-code typewriter 2s linear}} Every animation in your UI needs two decisions: how long and what easing. Get them wrong and the interface feels sluggish, jittery, or cheap. The rules are simple — but most developers guess instead of following them.

{{focus: easing}} {{zoom: 1.2x}} {{pulse: easing}} {{annotate: easing "Pick the right curve"}} Three easings cover every situation. `ease-out` for elements entering or exiting — the fast start feels responsive, the gentle stop feels controlled. `ease-in-out` for elements already on screen that need to move. `ease` for hover states and color changes. That's it. `linear` feels robotic — avoid it except for constant-speed things like progress bars. `ease-in` feels sluggish — the slow start delays visual feedback. Almost never use it.

{{zoom: 1x}} {{focus: duration}} {{zoom: 1.2x}} {{annotate: duration "Duration by purpose"}} Micro-interactions like button presses and checkbox toggles: 100 to 150 milliseconds. Standard UI like tooltips and dropdowns: 150 to 250 milliseconds. Modals and drawers: 200 to 300 milliseconds. Page transitions: 300 to 400 milliseconds max. UI animations should stay under 300 milliseconds. Exit animations can be faster than entrances. Larger elements animate slower than smaller ones.

{{zoom: 1x}} {{focus: frequency}} {{zoom: 1.2x}} {{pulse: frequency}} {{annotate: frequency "The frequency rule"}} Ask: how often will the user see this animation? If the answer is a hundred times a day, skip the animation entirely. Raycast never animates its menu toggle because users open it hundreds of times daily. Speed over delight. Product UI should be fast and purposeful. Marketing pages can be more elaborate — users see them once.

{{zoom: 1x}} {{focus: paired}} {{zoom: 1.2x}} {{annotate: paired "Paired elements"}} Elements that animate together must use the same easing and duration. Modal plus overlay. Tooltip plus arrow. Drawer plus backdrop. If they move as a unit, they should feel like a unit. Mismatched timing between paired elements looks broken.

{{zoom: 1x}} {{focus: none}}

```code:timing-code lang=css
/* Animation timing: easing + duration as custom properties */
:root {
  /* Easings */
  --ease-out: cubic-bezier(0.23, 1, 0.32, 1);
  --ease-in-out: cubic-bezier(0.645, 0.045, 0.355, 1);

  /* Duration by purpose */
  --duration-micro: 100ms;    /* button press, toggle */
  --duration-fast: 150ms;     /* hover, focus ring */
  --duration-normal: 200ms;   /* tooltip, dropdown */
  --duration-slow: 300ms;     /* modal, drawer */
}

/* Micro: button press feedback */
.button:active {
  transform: scale(0.97);
  transition: transform var(--duration-micro) var(--ease-out);
}

/* Standard: dropdown entering */
.dropdown {
  transition: opacity var(--duration-normal) var(--ease-out),
              transform var(--duration-normal) var(--ease-out);
}

/* Paired: modal + overlay share timing */
.modal   { transition: transform var(--duration-slow) var(--ease-out); }
.overlay { transition: opacity var(--duration-slow) var(--ease-out); }

/* NEVER transition: all — specify exact properties */
/* NEVER ease-in for UI — feels sluggish */
/* NEVER exceed 300ms for product UI */
---
easing: 3-5
duration: 8-12
frequency: 15-18
paired: 27-28
```
