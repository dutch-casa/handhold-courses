---
title: Modern CSS
---

# Native nesting

{{show: nesting-code typewriter 2s linear}} For years, nesting was the reason people used Sass. Write styles inside a parent selector and the preprocessor flattens them. Now CSS does it natively. No build step.

{{focus: sass-way}} {{zoom: 1.2x}} {{pulse: sass-way}} {{annotate: sass-way "The Sass way"}} Sass nesting compiles `.card .title` from `.card { .title { ... } }`. The output is regular CSS. The source is more readable — the hierarchy matches the HTML structure.

{{zoom: 1x}} {{focus: native}} {{zoom: 1.2x}} {{pulse: native}} {{annotate: native "Native CSS nesting"}} CSS nesting works the same way — with one rule. Nested selectors that start with a letter need `&`. `.card { & .title { } }`. Selectors that start with a symbol (`:`, `>`, `+`, `~`, `.`, `#`) don't need `&`. In practice, always use `&` and you'll never hit the edge case.

{{zoom: 1x}} {{focus: ampersand}} {{zoom: 1.2x}} {{annotate: ampersand "The & selector"}} `&` represents the parent selector. `&:hover` means `.card:hover`. `&.active` means `.card.active`. `& > .child` means `.card > .child`. You can put `&` anywhere — `.dark &` means `.dark .card`.

{{zoom: 1x}} {{focus: media-nest}} {{zoom: 1.2x}} {{pulse: media-nest}} {{annotate: media-nest "Nested media"}} Media queries nest inside selectors. The query stays next to the styles it modifies. No more jumping between a selector block and a media query block 200 lines away.

{{zoom: 1x}} {{focus: none}}

```code:nesting-code lang=css
/* Sass-style (now also valid CSS) */
.card {
  border: 1px solid #e2e8f0;
  border-radius: 8px;
}

/* Native CSS nesting */
.card {
  border: 1px solid #e2e8f0;

  & .title {
    font-size: 1.25rem;
    font-weight: 700;
  }

  &:hover {
    box-shadow: 0 4px 12px rgb(0 0 0 / 0.1);
  }

  &.featured {
    border-color: #3b82f6;
  }

  /* Nested media queries */
  @media (min-width: 768px) {
    padding: 24px;
  }
}
---
sass-way: 2-5
native: 8-22
ampersand: 16-21
media-nest: 24-26
```

# CSS Layers

{{clear: slide}}

{{show: layers-code typewriter 2s linear}} `@layer` lets you control the cascade without fighting specificity. You declare layer order once. Rules in later layers always beat rules in earlier layers.

{{focus: declare}} {{zoom: 1.2x}} {{pulse: declare}} {{annotate: declare "Layer order"}} `@layer reset, base, components, utilities;` declares four layers in order. Reset loses to base. Base loses to components. Components lose to utilities. This order is permanent — it doesn't matter where rules appear in the file.

{{zoom: 1x}} {{focus: assign}} {{zoom: 1.2x}} {{pulse: assign}} {{annotate: assign "Assign rules"}} `@layer components { .card { ... } }` puts the card styles in the components layer. `@layer utilities { .hidden { ... } }` puts hidden in utilities. A `.hidden` with specificity 0-1-0 beats a `.card` with specificity 0-1-0 because utilities comes after components.

{{zoom: 1x}} {{focus: third-party}} {{zoom: 1.2x}} {{pulse: third-party}} {{annotate: third-party "Third-party CSS"}} Put third-party CSS in an early layer: `@layer vendor { @import "some-library.css"; }`. Now your styles always win — they're in a later layer. No more `!important` battles with library CSS.

{{zoom: 1x}} {{focus: unlayered}} {{zoom: 1.2x}} {{annotate: unlayered "Unlayered wins"}} Styles not in any layer beat all layered styles. This is the escape hatch. But use layers consistently — mixing layered and unlayered code defeats the purpose.

{{zoom: 1x}} {{focus: none}}

```code:layers-code lang=css
/* Declare layer order — this is permanent */
@layer reset, base, components, utilities;

/* Assign rules to layers */
@layer reset {
  *, *::before, *::after { box-sizing: border-box; }
  body { margin: 0; }
}

@layer base {
  body { font-family: system-ui, sans-serif; }
  h1 { font-size: 2rem; }
}

@layer components {
  .card { border: 1px solid #e2e8f0; padding: 16px; }
  .button { padding: 8px 16px; border-radius: 6px; }
}

@layer utilities {
  .hidden { display: none; }
  .sr-only { position: absolute; width: 1px; height: 1px; }
}

/* Third-party in early layer */
@layer vendor {
  /* @import "some-library.css"; */
}
---
declare: 2
assign: 5-20
third-party: 23-25
unlayered: 2
```

# The :has() deep dive

{{clear: slide}}

{{split}} {{show: has-deep-code typewriter 2s linear}} {{show: has-deep-preview reveal 0.5s spring}} You met `:has()` in the selectors lesson. Now let's push it further — this selector replaces a surprising amount of JavaScript.

{{focus: quantity}} {{zoom: 1.2x}} {{pulse: quantity}} {{annotate: quantity "Quantity queries"}} `.grid:has(> :nth-child(4))` matches a grid that contains at least four children. Change the column count based on how many items exist. Three items? Two columns. Six items? Three columns. The content determines the layout.

{{zoom: 1x}} {{focus: state-machine}} {{zoom: 1.2x}} {{pulse: state-machine}} {{annotate: state-machine "State machine"}} A checkbox controls a sidebar. `:has(#toggle:checked) .sidebar` shows the sidebar when the checkbox is checked. The checkbox can be hidden. The label is the visible toggle. Entire toggle UI — no JavaScript.

{{zoom: 1x}} {{focus: validation}} {{zoom: 1.2x}} {{annotate: validation "Form validation"}} `form:has(:invalid) .submit` dims the submit button when any field is invalid. The form watches all its inputs. `fieldset:has(:focus) ` highlights the active section. The parent reacts to its children's state.

{{zoom: 1x}} {{focus: theming}} {{zoom: 1.2x}} {{pulse: theming}} {{annotate: theming "Conditional themes"}} `html:has([data-theme="dark"])` applies dark mode based on a data attribute. Set the attribute with one line of JavaScript. All the styling stays in CSS. Clean separation.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:has-deep-code lang=css
/* Quantity queries: layout based on item count */
.grid { display: grid; gap: 12px; }
.grid:has(> :nth-child(4)) {
  grid-template-columns: repeat(3, 1fr);
}
.grid:not(:has(> :nth-child(4))) {
  grid-template-columns: repeat(2, 1fr);
}

/* State machine: checkbox toggle */
.page:has(#sidebar-toggle:checked) .sidebar {
  transform: translateX(0);
}
.page:has(#sidebar-toggle:not(:checked)) .sidebar {
  transform: translateX(-100%);
}

/* Form validation feedback */
form:has(:invalid) .submit-btn {
  opacity: 0.5;
  pointer-events: none;
}
fieldset:has(:focus-within) {
  background: hsl(220, 90%, 97%);
}

/* Conditional theming */
html:has([data-theme="dark"]) {
  --color-bg: #0f172a;
  --color-text: #e2e8f0;
}
---
quantity: 2-8
state-machine: 11-16
validation: 19-24
theming: 27-30
```

```preview:has-deep-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .toggle-demo { padding: 8px; border: 1px solid #e2e8f0; border-radius: 6px; }
  .toggle-demo:has(#demo-check:checked) .panel { display: block; }
  .toggle-demo:has(#demo-check:not(:checked)) .panel { display: none; }
  .toggle-demo label { font-size: 12px; cursor: pointer; color: #3b82f6; font-weight: 600; }
  .toggle-demo #demo-check { display: none; }
  .panel { background: #eff6ff; padding: 8px; border-radius: 4px; margin-top: 6px; font-size: 12px; }
  .qty-demo { display: grid; gap: 4px; }
  .qty-demo:has(> :nth-child(4)) { grid-template-columns: repeat(3, 1fr); }
  .qty-demo:not(:has(> :nth-child(4))) { grid-template-columns: repeat(2, 1fr); }
  .qty-item { background: #3b82f6; color: white; padding: 8px; border-radius: 4px; font-size: 11px; text-align: center; }
</style>
<div class="label">Checkbox toggle (no JS):</div>
<div class="toggle-demo">
  <input type="checkbox" id="demo-check">
  <label for="demo-check">Toggle panel</label>
  <div class="panel">This panel shows/hides based on checkbox state. Pure CSS.</div>
</div>
<div class="label">Quantity query: 2 items → 2 cols</div>
<div class="qty-demo">
  <div class="qty-item">A</div><div class="qty-item">B</div><div class="qty-item">C</div>
</div>
<div class="label">Quantity query: 4+ items → 3 cols</div>
<div class="qty-demo">
  <div class="qty-item">A</div><div class="qty-item">B</div><div class="qty-item">C</div><div class="qty-item">D</div><div class="qty-item">E</div>
</div>
```

# Color functions

{{clear: slide}}

{{show: color-code typewriter 2s linear}} Modern CSS has color functions that make palette work simpler and more expressive.

{{focus: color-mix}} {{zoom: 1.2x}} {{pulse: color-mix}} {{annotate: color-mix "color-mix()"}} `color-mix(in srgb, #3b82f6, white 20%)` mixes blue with 20% white. You get a tint without manually calculating the lightness. Mix with black for shades. Mix with gray for desaturation. One function replaces a Sass mixin.

{{zoom: 1x}} {{focus: light-dark}} {{zoom: 1.2x}} {{pulse: light-dark}} {{annotate: light-dark "light-dark()"}} `light-dark(#1e293b, #e2e8f0)` returns the first value in light mode and the second in dark mode. No media query needed — it reads `color-scheme` from the root element. Define all your colors once with two values.

{{zoom: 1x}} {{focus: relative}} {{zoom: 1.2x}} {{annotate: relative "Relative colors"}} `hsl(from var(--brand) h s calc(l + 20%))` takes your brand color and creates a lighter version by adjusting just the lightness channel. Derive an entire palette from one base color, directly in CSS.

{{zoom: 1x}} {{focus: none}}

```code:color-code lang=css
/* color-mix(): blend two colors */
.tint { background: color-mix(in srgb, #3b82f6, white 20%); }
.shade { background: color-mix(in srgb, #3b82f6, black 30%); }
.muted { background: color-mix(in srgb, #3b82f6, gray 50%); }

/* light-dark(): automatic theme colors */
:root { color-scheme: light dark; }
body {
  background: light-dark(#ffffff, #0f172a);
  color: light-dark(#1e293b, #e2e8f0);
}
.border {
  border-color: light-dark(#e2e8f0, #334155);
}

/* Relative color syntax (derived palettes) */
:root {
  --brand: hsl(220, 91%, 60%);
  --brand-light: hsl(from var(--brand) h s calc(l + 25%));
  --brand-dark: hsl(from var(--brand) h s calc(l - 20%));
  --brand-muted: hsl(from var(--brand) h calc(s - 40%) l);
}
---
color-mix: 2-4
light-dark: 7-13
relative: 16-21
```

# View transitions and scroll animations

{{clear: slide}}

{{split}} {{show: transitions-code typewriter 2s linear}} {{show: transitions-preview reveal 0.5s spring}} Two features that used to require heavy JavaScript libraries are now native CSS.

{{focus: view-transition}} {{zoom: 1.2x}} {{pulse: view-transition}} {{annotate: view-transition "View transitions"}} `view-transition-name` on an element gives it a persistent identity across page navigations. The browser automatically animates the old state to the new state — cross-fade, morph, slide. Assign a name, and the browser handles the animation.

{{zoom: 1x}} {{focus: vt-css}} {{zoom: 1.2x}} {{annotate: vt-css "Customizing transitions"}} `::view-transition-old` and `::view-transition-new` pseudo-elements let you customize the animation. Slide old content left, new content from right. Fade, scale, rotate — any CSS animation works.

{{zoom: 1x}} {{focus: scroll-driven}} {{zoom: 1.2x}} {{pulse: scroll-driven}} {{annotate: scroll-driven "Scroll animations"}} `animation-timeline: scroll()` ties an animation to scroll position instead of time. An element fades in as you scroll it into view. A progress bar fills as you read the page. The animation plays forward as you scroll down and backward as you scroll up.

{{zoom: 1x}} {{focus: scroll-range}} {{zoom: 1.2x}} {{annotate: scroll-range "Animation range"}} `animation-range: entry 0% entry 100%` means the animation plays while the element enters the viewport. `cover 0% cover 100%` means it plays while the element is anywhere in the viewport. These range keywords replace intersection observer for most scroll-triggered effects.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:transitions-code lang=css
/* View transitions */
.card-image {
  view-transition-name: card-hero;
}

/* Customize the transition animation */
::view-transition-old(card-hero) {
  animation: fade-out 0.3s ease-out;
}
::view-transition-new(card-hero) {
  animation: fade-in 0.3s ease-out;
}

/* Scroll-driven animations */
@keyframes fade-in-up {
  from { opacity: 0; transform: translateY(20px); }
  to { opacity: 1; transform: translateY(0); }
}

.reveal-on-scroll {
  animation: fade-in-up linear both;
  animation-timeline: scroll();
  animation-range: entry 0% entry 100%;
}

/* Progress bar that fills with scroll */
.progress-bar {
  animation: grow-width linear;
  animation-timeline: scroll();
}
@keyframes grow-width {
  from { width: 0%; }
  to { width: 100%; }
}
---
view-transition: 2-4
vt-css: 7-12
scroll-driven: 15-23
scroll-range: 23
```

```preview:transitions-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
  .progress {
    height: 4px; background: #3b82f6; border-radius: 2px;
    width: 60%; margin-bottom: 12px; transition: width 0.3s ease-out;
  }
  .scroll-demo {
    height: 100px; overflow-y: auto; border: 1px solid #e2e8f0;
    border-radius: 6px; padding: 12px; font-size: 12px; color: #475569;
  }
  .scroll-demo p { margin-bottom: 8px; }
  .feature-list { list-style: none; padding: 0; margin: 12px 0 0; }
  .feature-list li {
    padding: 6px 10px; font-size: 12px; border-left: 3px solid #3b82f6;
    margin-bottom: 4px; background: #eff6ff; border-radius: 0 4px 4px 0;
  }
</style>
<div class="label">Scroll progress bar (60% read):</div>
<div class="progress"></div>
<div class="label">New CSS features covered:</div>
<ul class="feature-list">
  <li>Native nesting</li>
  <li>@layer cascade control</li>
  <li>:has() parent selector</li>
  <li>color-mix() and light-dark()</li>
  <li>View transitions</li>
  <li>Scroll-driven animations</li>
</ul>
```

# The popover API

{{clear: slide}}

{{show: popover-code typewriter 1.5s linear}} Popovers — tooltips, dropdowns, menus — used to require JavaScript for show/hide, positioning, focus trapping, and click-outside dismissal. The Popover API does all of it in HTML and CSS.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "Basic popover"}} Add `popover` to an element. Add `popovertarget` to a button pointing at the popover's ID. Click the button, the popover appears. Click outside, it dismisses. Click the button again, it toggles. Zero JavaScript.

{{zoom: 1x}} {{focus: styling}} {{zoom: 1.2x}} {{annotate: styling "Styling"}} `[popover]` targets popovers. `:popover-open` targets the open state. `::backdrop` targets the overlay behind the popover. `@starting-style` defines the initial state for entry animations.

{{zoom: 1x}} {{focus: anchor}} {{zoom: 1.2x}} {{pulse: anchor}} {{annotate: anchor "Anchor positioning"}} CSS anchor positioning (`anchor-name`, `position-anchor`, `position-area`) ties a popover to a trigger element. The popover follows its anchor and flips when it runs out of space. This replaces Floating UI and Popper.js for most use cases.

{{zoom: 1x}} {{focus: none}} These features are landing across all browsers. They represent a major shift: layout, interaction, and positioning that used to live in JavaScript moving into the platform.

```code:popover-code lang=html
<!-- Basic popover: HTML only -->
<button popovertarget="menu">Open Menu</button>
<div id="menu" popover>
  <a href="/settings">Settings</a>
  <a href="/logout">Log out</a>
</div>

<style>
  /* Styling the popover */
  [popover] {
    border: 1px solid #e2e8f0;
    border-radius: 8px;
    padding: 8px;
    box-shadow: 0 8px 24px rgb(0 0 0 / 0.12);
  }

  /* Open state animation */
  [popover]:popover-open {
    opacity: 1;
    transform: scale(1);
  }
  @starting-style {
    [popover]:popover-open {
      opacity: 0;
      transform: scale(0.95);
    }
  }

  /* Anchor positioning */
  .trigger { anchor-name: --menu-trigger; }
  [popover] {
    position-anchor: --menu-trigger;
    position-area: bottom span-right;
  }
</style>
---
basic: 2-6
styling: 9-16
anchor: 30-34
```
