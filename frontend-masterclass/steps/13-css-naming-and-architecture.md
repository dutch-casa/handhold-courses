---
title: CSS naming and architecture
---

# The namespace problem

{{show: namespace-code typewriter 2s linear}} CSS has one global namespace. Every class name lives in the same flat space. At scale, this breaks.

{{focus: collision}} {{zoom: 1.2x}} {{pulse: collision}} {{annotate: collision "Name collision"}} Two developers write `.title`. One styles the page header. One styles a card heading. Both rules land in the same stylesheet. The second one wins by source order. Neither developer knows about the other's class. This is the fundamental CSS scaling problem.

{{zoom: 1x}} {{focus: side-effects}} {{zoom: 1.2x}} {{annotate: side-effects "Side effects"}} Change `.button` to fix a bug in the checkout flow. The login page breaks because it used the same class. Every CSS change is a global mutation. You can't predict what breaks.

{{zoom: 1x}} {{focus: specificity}} {{zoom: 1.2x}} {{pulse: specificity}} {{annotate: specificity "Specificity wars"}} Developer A writes `.card .title { color: blue; }`. Developer B needs it green. They write `.sidebar .card .title { color: green; }`. Developer A adds `!important`. Developer B adds `!important` with a higher-specificity selector. This is a specificity war. Nobody wins.

{{zoom: 1x}} {{focus: none}} Every CSS methodology is a response to this problem. Each one trades flexibility for predictability.

```code:namespace-code lang=css
/* Name collision — who wins? */
.title { font-size: 2rem; color: #0f172a; }
/* ... 500 lines later ... */
.title { font-size: 1rem; color: #64748b; }

/* Side effects — changing one breaks another */
.button { padding: 8px 16px; background: blue; }
/* "Let me just tweak the padding..." */
.button { padding: 12px 24px; background: blue; }
/* Now every button on every page changed. */

/* Specificity war */
.card .title { color: blue; }
.sidebar .card .title { color: green; }
.sidebar .card .title { color: red !important; }
---
collision: 2-4
side-effects: 7-10
specificity: 13-15
```

# BEM

{{clear: slide}}

{{show: bem-code typewriter 2s linear}} BEM stands for Block, Element, Modifier. It solves the naming problem with convention — no tools, just discipline.

{{focus: block}} {{zoom: 1.2x}} {{pulse: block}} {{annotate: block "Block"}} A block is a standalone component. `.card`, `.nav`, `.form`. It's the root name. It owns everything inside it.

{{zoom: 1x}} {{focus: element}} {{zoom: 1.2x}} {{pulse: element}} {{annotate: element "Element"}} An element is a part of a block. Double underscore separates block from element. `.card__title`, `.card__body`, `.card__image`. Elements don't exist outside their block.

{{zoom: 1x}} {{focus: modifier}} {{zoom: 1.2x}} {{pulse: modifier}} {{annotate: modifier "Modifier"}} A modifier is a variation. Double hyphen separates base from modifier. `.card--featured`, `.button--primary`, `.nav__link--active`. Modifiers change appearance or state.

{{zoom: 1x}} {{focus: flat}} {{zoom: 1.2x}} {{annotate: flat "Flat specificity"}} The beauty of BEM: every selector is a single class. `.card__title` has specificity 0-1-0. `.card--featured` has specificity 0-1-0. No nesting. No wars. Every rule has the same weight.

{{zoom: 1x}} {{focus: none}} BEM is verbose. `.search-form__submit-button--disabled` is a mouthful. But it's unambiguous. You know exactly what it styles, where it lives, and what state it represents.

```code:bem-code lang=css
/* Block: standalone component */
.card {
  border: 1px solid #e2e8f0;
  border-radius: 8px;
}

/* Element: part of a block (double underscore) */
.card__title {
  font-size: 1.25rem;
  font-weight: 700;
}
.card__body { padding: 16px; }
.card__image { border-radius: 8px 8px 0 0; }

/* Modifier: variation (double hyphen) */
.card--featured {
  border-color: #3b82f6;
  box-shadow: 0 4px 12px rgb(0 0 0 / 0.1);
}
.button--primary { background: #3b82f6; color: white; }
.button--secondary { background: #f1f5f9; color: #334155; }

/* Flat specificity — every selector is 0-1-0 */
/* No nesting. No !important. No wars. */
---
block: 2-5
element: 8-12
modifier: 15-20
flat: 22-23
```

# Utility-first

{{clear: slide}}

{{split}} {{show: utility-code typewriter 2s linear}} {{show: utility-preview reveal 0.5s spring}} The opposite of BEM. Instead of semantic names, every class does one thing. Compose them in HTML.

{{focus: single}} {{zoom: 1.2x}} {{pulse: single}} {{annotate: single "Single-purpose"}} `.text-lg` sets font size. `.font-bold` sets weight. `.p-4` sets padding. `.bg-blue-500` sets background. Each class is one declaration. You combine them to build up a design.

{{zoom: 1x}} {{focus: composition}} {{zoom: 1.2x}} {{pulse: composition}} {{annotate: composition "Compose in HTML"}} The styling moves to the HTML. `class="text-lg font-bold p-4 bg-blue-500 rounded-lg"` is a blue padded box with large bold text and rounded corners. No custom class name needed. No separate CSS file to maintain.

{{zoom: 1x}} {{focus: tradeoff}} {{zoom: 1.2x}} {{annotate: tradeoff "The tradeoff"}} HTML gets verbose. But you gain: no naming decisions, no specificity issues, no dead CSS, no side effects. Change one element's classes and nothing else changes. This is the philosophy behind Tailwind CSS — which you'll learn next.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:utility-code lang=css
/* Utility classes — single purpose */
.text-lg { font-size: 1.125rem; }
.font-bold { font-weight: 700; }
.p-4 { padding: 1rem; }
.rounded-lg { border-radius: 0.5rem; }
.bg-blue-500 { background: #3b82f6; }
.text-white { color: white; }

/* Compose in HTML: */
/* <div class="text-lg font-bold p-4 */
/*   bg-blue-500 text-white rounded-lg"> */
/*   Hello World */
/* </div> */

/* The tradeoff: verbose HTML, zero side effects */
---
single: 2-7
composition: 10-13
tradeoff: 15
```

```preview:utility-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 8px; }
  .text-lg { font-size: 1.125rem; }
  .font-bold { font-weight: 700; }
  .p-4 { padding: 1rem; }
  .rounded-lg { border-radius: 0.5rem; }
  .bg-blue { background: #3b82f6; }
  .bg-green { background: #22c55e; }
  .text-white { color: white; }
  .mb-2 { margin-bottom: 0.5rem; }
  .text-sm { font-size: 0.875rem; }
  .flex { display: flex; }
  .gap-2 { gap: 0.5rem; }
</style>
<div class="label">Composed from utility classes:</div>
<div class="p-4 bg-blue rounded-lg text-white font-bold text-lg mb-2">
  Primary Card
</div>
<div class="p-4 bg-green rounded-lg text-white mb-2">
  Success Alert
</div>
<div class="flex gap-2">
  <div class="p-4 bg-blue rounded-lg text-white text-sm flex" style="flex:1; justify-content:center">Button A</div>
  <div class="p-4 rounded-lg text-sm flex" style="flex:1; justify-content:center; border: 1px solid #e2e8f0">Button B</div>
</div>
```

# CSS Modules and scoping

{{clear: slide}}

{{show: modules-code typewriter 2s linear}} The tooling approach: let the build system solve naming.

{{focus: module}} {{zoom: 1.2x}} {{pulse: module}} {{annotate: module "CSS Modules"}} You write `.title` in a `.module.css` file. The build tool (Vite, webpack) rewrites it to `.Card_title_x7f2a` — a unique hash. Your code imports it as an object: `styles.title`. No collisions. The tool guarantees uniqueness.

{{zoom: 1x}} {{focus: import}} {{zoom: 1.2x}} {{annotate: import "Import as object"}} `import styles from './Card.module.css'`. Now `styles.title` is the hashed class name. You use it in JSX: `className={styles.title}`. The original name stays readable in your code. The browser sees the hash.

{{zoom: 1x}} {{focus: scoped}} {{zoom: 1.2x}} {{pulse: scoped}} {{annotate: scoped "Scoped by default"}} Every class is scoped to the component that imports it. Two components can both have `.title` — they get different hashes. You write natural, short class names. The build handles isolation.

{{zoom: 1x}} {{focus: none}}

```code:modules-code lang=css
/* Card.module.css — you write this */
.title {
  font-size: 1.25rem;
  font-weight: 700;
}
.body {
  padding: 16px;
}

/* Browser sees this: */
/* .Card_title_x7f2a { font-size: 1.25rem; ... } */
/* .Card_body_k3m9p { padding: 16px; } */

/* Import in component */
/* import styles from './Card.module.css' */
/* <h2 className={styles.title}>...</h2> */
/* <div className={styles.body}>...</div> */
---
module: 2-8
import: 14-16
scoped: 11-12
```

# The progression

{{clear: slide}}

{{show: progression-code typewriter 2s linear}} No methodology is universally right. Each one fits different project sizes and team dynamics.

{{focus: none-phase}} {{zoom: 1.2x}} {{pulse: none-phase}} {{annotate: none-phase "Solo project"}} No system. Write classes as you go. Works for personal projects and prototypes. Falls apart at 500 lines or with a second developer.

{{zoom: 1x}} {{focus: bem-phase}} {{zoom: 1.2x}} {{annotate: bem-phase "Team project"}} BEM. Convention-based naming. Works without build tools. Clear ownership of styles. Falls apart when you spend more time naming things than writing CSS.

{{zoom: 1x}} {{focus: utility-phase}} {{zoom: 1.2x}} {{pulse: utility-phase}} {{annotate: utility-phase "Product at scale"}} Utility-first (Tailwind). No naming decisions. No dead CSS. Consistent design tokens. Falls apart if you reject the verbose-HTML tradeoff.

{{zoom: 1x}} {{focus: module-phase}} {{zoom: 1.2x}} {{annotate: module-phase "Component library"}} CSS Modules or CSS-in-JS. Tooling-enforced isolation. Works naturally with component architectures (React, Vue). Falls apart without a build system.

{{zoom: 1x}} {{focus: none}} Pick the right tool for the job. You'll use Tailwind next — it's the most popular approach in modern frontend development.

```code:progression-code lang=css
/* Phase 1: No system (solo) */
.card { ... }
.title { ... }
.btn { ... }

/* Phase 2: BEM (team) */
.card__title { ... }
.card__body { ... }
.card--featured { ... }

/* Phase 3: Utilities (product) */
/* class="text-lg font-bold p-4 rounded-lg" */

/* Phase 4: CSS Modules (component library) */
/* .title in Card.module.css → Card_title_x7f2a */
---
none-phase: 2-4
bem-phase: 7-9
utility-phase: 12
module-phase: 15
```
