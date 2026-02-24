---
title: Selectors and combinators
---

# Pattern matching the DOM

{{show: selector-basics typewriter 2s linear}} CSS selectors are pattern matching. You describe a shape, and the browser finds every element that matches.

{{focus: type-sel}} {{zoom: 1.2x}} {{pulse: type-sel}} {{annotate: type-sel "Type selector"}} A bare tag name matches every element of that type. `p` matches every paragraph. `h1` matches every level-one heading. Simple and broad.

{{zoom: 1x}} {{focus: class-sel}} {{zoom: 1.2x}} {{pulse: class-sel}} {{annotate: class-sel "Class selector"}} A dot prefix matches elements with that class. `.card` matches anything with `class="card"`. Classes are reusable — one element can have many, many elements can share one.

{{zoom: 1x}} {{focus: id-sel}} {{zoom: 1.2x}} {{pulse: id-sel}} {{annotate: id-sel "ID selector"}} A hash prefix matches a single element by ID. `#nav` matches `id="nav"`. IDs are unique per page. High specificity — use sparingly.

{{zoom: 1x}} {{focus: universal-sel}} {{pulse: universal-sel}} {{annotate: universal-sel "Universal selector"}} The asterisk matches everything. You saw it in the box-sizing reset: `* { box-sizing: border-box }`. Powerful, but zero specificity.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:selector-basics lang=css
/* Type: every <p> element */
p { color: #334155; }

/* Class: elements with class="card" */
.card { border-radius: 8px; }

/* ID: the one element with id="nav" */
#nav { position: sticky; top: 0; }

/* Universal: every element */
* { box-sizing: border-box; }

/* Combining: element + class */
a.active { font-weight: 700; }
---
type-sel: 2
class-sel: 5
id-sel: 8
universal-sel: 11
```

# Attribute selectors

{{clear: slide}}

{{show: attr-code typewriter 2s linear}} Sometimes class and ID aren't enough. Attribute selectors match elements by any HTML attribute and its value.

{{focus: exact-match}} {{zoom: 1.2x}} {{pulse: exact-match}} {{annotate: exact-match "Exact match"}} Square brackets with an equals sign. `[type="email"]` matches inputs whose type attribute is exactly "email". Nothing more, nothing less.

{{zoom: 1x}} {{focus: starts-with}} {{zoom: 1.2x}} {{pulse: starts-with}} {{annotate: starts-with "Starts with"}} The caret-equals operator: `[href^="https"]` matches links that begin with "https". Useful for styling external links differently from internal ones.

{{zoom: 1x}} {{focus: ends-with}} {{zoom: 1.2x}} {{pulse: ends-with}} {{annotate: ends-with "Ends with"}} The dollar-equals operator: `[href$=".pdf"]` matches links ending in dot-pdf. Add a PDF icon to every document link — no extra class needed.

{{zoom: 1x}} {{focus: contains}} {{zoom: 1.2x}} {{pulse: contains}} {{annotate: contains "Contains"}} The star-equals operator: `[class*="btn"]` matches any element whose class contains "btn" anywhere. Matches `btn-primary`, `submit-btn`, `btn`. Broad — use carefully.

{{zoom: 1x}} {{focus: presence}} {{zoom: 1.2x}} {{annotate: presence "Has attribute"}} Just the brackets with an attribute name. `[disabled]` matches any element that has a disabled attribute, regardless of its value. Boolean attributes like disabled, required, and readonly are common targets.

{{zoom: 1x}} {{focus: none}}

```code:attr-code lang=css
/* Exact match */
input[type="email"] { border-color: #3b82f6; }

/* Starts with */
a[href^="https"] { padding-right: 16px; }

/* Ends with */
a[href$=".pdf"]::after { content: " (PDF)"; }

/* Contains */
[class*="btn"] { cursor: pointer; }

/* Has attribute (presence) */
[disabled] {
  opacity: 0.5;
  pointer-events: none;
}
---
exact-match: 2
starts-with: 5
ends-with: 8
contains: 11
presence: 14-17
```

# Pseudo-classes: state and position

{{clear: slide}}

{{split}} {{show: pseudo-code typewriter 2s linear}} {{show: pseudo-preview reveal 0.5s spring}} Pseudo-classes select elements based on state or position. They start with a single colon.

{{focus: hover-focus}} {{zoom: 1.2x}} {{pulse: hover-focus}} {{annotate: hover-focus "User interaction"}} `:hover` fires when the cursor is over an element. `:focus` fires when an element has keyboard focus. `:active` fires during a click. These are the interaction triad — hover, focus, active. Style all three for accessible, responsive controls.

{{zoom: 1x}} {{focus: child-pos}} {{zoom: 1.2x}} {{pulse: child-pos}} {{annotate: child-pos "Positional"}} `:first-child` matches the first element inside a parent. `:last-child` matches the last. `:nth-child()` takes a formula — `2n` for every even element, `3n+1` for every third starting at one. The formula is `An+B` where A is the step size and B is the offset.

{{zoom: 1x}} {{focus: nth-examples}} {{zoom: 1.2x}} {{annotate: nth-examples "nth-child formulas"}} `:nth-child(odd)` and `:nth-child(even)` are built-in keywords. The formula `n+4` means "fourth element and every one after." Negative steps work too: `-n+3` means "the first three only."

{{zoom: 1x}} {{focus: negation}} {{zoom: 1.2x}} {{pulse: negation}} {{annotate: negation "Negation"}} `:not()` excludes elements from a match. `.card:not(:last-child)` matches every card except the last. You can put any simple selector inside `:not()`.

{{zoom: 1x}} {{focus: form-states}} {{zoom: 1.2x}} {{annotate: form-states "Form states"}} `:checked`, `:valid`, `:invalid`, `:required`, `:placeholder-shown` — forms are full of states. CSS can react to all of them without JavaScript. A red border on `:invalid` gives instant feedback.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:pseudo-code lang=css
/* Interaction states */
a:hover { color: #2563eb; }
a:focus { outline: 2px solid #3b82f6; }
button:active { transform: scale(0.97); }

/* Positional — child selectors */
li:first-child { font-weight: 700; }
li:last-child { border-bottom: none; }
tr:nth-child(even) { background: #f8fafc; }

/* nth-child formulas */
li:nth-child(3n+1) { color: #dc2626; }
li:nth-child(-n+3) { font-size: 1.1rem; }

/* Negation */
.card:not(:last-child) { margin-bottom: 16px; }
input:not([type="submit"]) { border: 1px solid #cbd5e1; }

/* Form states */
input:valid { border-color: #22c55e; }
input:invalid { border-color: #ef4444; }
input:required { border-left: 3px solid #3b82f6; }
---
hover-focus: 2-4
child-pos: 7-9
nth-examples: 12-13
negation: 16-17
form-states: 20-22
```

```preview:pseudo-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .demo-list { list-style: none; padding: 0; margin: 0 0 12px; }
  .demo-list li {
    padding: 8px 12px; border: 1px solid #e2e8f0;
    border-bottom: none; font-size: 13px;
  }
  .demo-list li:last-child { border-bottom: 1px solid #e2e8f0; }
  .demo-list li:first-child { font-weight: 700; }
  .demo-list li:nth-child(even) { background: #f8fafc; }
  .demo-list li:hover { background: #eff6ff; }
  .demo-input {
    padding: 6px 10px; border: 1px solid #cbd5e1;
    border-radius: 4px; font-size: 13px; margin: 4px 0;
    display: block; width: 200px;
  }
  .demo-input:focus { outline: 2px solid #3b82f6; border-color: #3b82f6; }
  .demo-input:valid { border-color: #22c55e; }
  .demo-input:invalid { border-color: #ef4444; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; }
</style>
<div class="label">Hover the list. First item is bold. Even rows are shaded.</div>
<ul class="demo-list">
  <li>First item (bold)</li>
  <li>Second (even bg)</li>
  <li>Third</li>
  <li>Fourth (even bg)</li>
  <li>Fifth</li>
</ul>
<div class="label">Type in these inputs to see :valid / :invalid</div>
<input class="demo-input" type="email" placeholder="Email (requires @)" required>
<input class="demo-input" type="text" placeholder="Name (any text)" required>
```

# Pseudo-elements

{{clear: slide}}

{{show: pseudo-el-code typewriter 1.5s linear}} Pseudo-elements create virtual elements that don't exist in the HTML. They start with a double colon.

{{focus: before-after}} {{zoom: 1.2x}} {{pulse: before-after}} {{annotate: before-after "Generated content"}} `::before` and `::after` insert content at the start and end of an element. They require a `content` property — even if it's empty. These are workhorses: decorative icons, quotation marks, required-field asterisks, clearfixes.

{{zoom: 1x}} {{focus: first-line}} {{zoom: 1.2x}} {{annotate: first-line "First line, first letter"}} `::first-line` styles only the first rendered line of text. `::first-letter` targets the first character. Both respond to line wrapping — they select based on what the browser renders, not what's in the HTML.

{{zoom: 1x}} {{focus: placeholder}} {{zoom: 1.2x}} {{pulse: placeholder}} {{annotate: placeholder "Input placeholder"}} `::placeholder` styles placeholder text inside inputs. Keep the contrast lower than real input text — the user needs to tell them apart instantly.

{{zoom: 1x}} {{focus: marker}} {{zoom: 1.2x}} {{annotate: marker "List markers"}} `::marker` styles list bullets and numbers. Change their color, size, or content. Before `::marker` existed, people reset list-style and faked bullets with `::before`. No more.

{{zoom: 1x}} {{focus: none}}

```code:pseudo-el-code lang=css
/* Generated content */
.required::after {
  content: " *";
  color: #ef4444;
}
blockquote::before {
  content: "\201C"; /* left double quote */
  font-size: 3rem;
  color: #94a3b8;
}

/* First line and first letter */
p::first-letter {
  font-size: 2em;
  float: left;
  line-height: 1;
  margin-right: 4px;
}

/* Placeholder text */
input::placeholder {
  color: #94a3b8;
  font-style: italic;
}

/* List markers */
li::marker {
  color: #3b82f6;
  font-weight: 700;
}
---
before-after: 2-9
first-line: 12-17
placeholder: 20-23
marker: 26-29
```

# Combinators

{{clear: slide}}

{{show: comb-tree reveal 0.3s spring}} {{show: comb-code typewriter 2s linear}} Combinators express relationships between elements. They look at the DOM tree structure to narrow matches.

{{focus: descendant}} {{zoom: 1.2x}} {{pulse: descendant}} {{annotate: descendant "Descendant (space)"}} A space between selectors matches any descendant. `nav a` matches every link inside a nav — children, grandchildren, any depth. The most common combinator. Also the loosest.

{{zoom: 1x}} {{focus: child}} {{zoom: 1.2x}} {{pulse: child}} {{annotate: child "Child (>)"}} The greater-than sign matches only direct children. `nav > a` matches links that are immediate children of nav, but not links nested deeper inside a list or div. Tighter control.

{{zoom: 1x}} {{pan: adjacent}} {{focus: adjacent}} {{zoom: 1.2x}} {{pulse: adjacent}} {{annotate: adjacent "Adjacent (+)"}} The plus sign matches the next sibling. `h2 + p` matches a paragraph that immediately follows an h2. Only the first one after. Use it for "the intro paragraph after a heading" styling.

{{zoom: 1x}} {{pan: general-sib}} {{focus: general-sib}} {{zoom: 1.2x}} {{pulse: general-sib}} {{annotate: general-sib "General sibling (~)"}} The tilde matches all following siblings. `h2 ~ p` matches every paragraph that comes after an h2 within the same parent. Not just the first — all of them.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:comb-code lang=css
/* Descendant: any <a> inside <nav> */
nav a { text-decoration: none; }

/* Child: only direct <a> children of <nav> */
nav > a { font-weight: 600; }

/* Adjacent sibling: <p> right after <h2> */
h2 + p { font-size: 1.1rem; color: #475569; }

/* General sibling: all <p> after <h2> */
h2 ~ p { line-height: 1.7; }

/* Combining multiple combinators */
.sidebar > ul > li:first-child a {
  color: #2563eb;
  font-weight: 700;
}
---
descendant: 2
child: 5
adjacent: 8
general-sib: 11
```

```data:comb-tree kind=tree
nav
  a "Home"
  ul
    li
      a "About"
    li
      a "Blog"
---
root: nav
```

# The :has() revolution

{{clear: slide}}

{{split}} {{show: has-code typewriter 2s linear}} {{show: has-preview reveal 0.5s spring}} For decades, CSS could only select downward. You could style a child based on its parent, but never a parent based on its child. `:has()` changes that.

{{focus: parent-sel}} {{zoom: 1.2x}} {{pulse: parent-sel}} {{annotate: parent-sel "Parent selector"}} `.card:has(img)` selects any card that contains an image. The selector looks inside the element to decide whether to match. This is the most requested CSS feature in history — and it shipped in 2023.

{{zoom: 1x}} {{focus: sibling-check}} {{zoom: 1.2x}} {{pulse: sibling-check}} {{annotate: sibling-check "Sibling awareness"}} `:has()` isn't limited to children. `label:has(+ input:invalid)` selects a label whose next sibling is an invalid input. Labels turn red when the input fails validation. No JavaScript.

{{zoom: 1x}} {{focus: logical}} {{zoom: 1.2x}} {{annotate: logical "Conditional layouts"}} Combine `:has()` with layout properties and you get conditional layouts. A card with an image gets a grid layout. A card without gets a simple stack. The content determines the layout, not a modifier class.

{{zoom: 1x}} {{focus: form-ux}} {{zoom: 1.2x}} {{pulse: form-ux}} {{annotate: form-ux "Form UX without JS"}} A form group that highlights when its input has focus. A fieldset that shows a success state when all required fields are valid. A submit button that changes appearance when the form is complete. All CSS. Zero JavaScript.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:has-code lang=css
/* Parent selector: card that contains an image */
.card:has(img) {
  display: grid;
  grid-template-columns: 200px 1fr;
}

/* Sibling-aware: label before invalid input */
label:has(+ input:invalid) {
  color: #ef4444;
}

/* Conditional layout */
.card:has(img):has(h2) {
  border: 2px solid #3b82f6;
}
.card:not(:has(img)) {
  padding: 24px;
}

/* Form UX without JavaScript */
.field:has(input:focus) {
  background: #eff6ff;
  border-radius: 8px;
}
.form:has(input:invalid) .submit-btn {
  opacity: 0.5;
  pointer-events: none;
}
---
parent-sel: 2-5
sibling-check: 8-10
logical: 13-18
form-ux: 21-27
```

```preview:has-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .cards { display: flex; flex-direction: column; gap: 12px; }
  .card {
    border: 1px solid #e2e8f0; border-radius: 8px;
    padding: 16px; background: white;
  }
  .card:has(img) {
    display: grid; grid-template-columns: 80px 1fr;
    gap: 12px; align-items: center; padding: 12px;
  }
  .card:not(:has(img)) { padding: 16px; }
  .card img { width: 80px; height: 80px; object-fit: cover; border-radius: 6px; }
  .card h3 { margin: 0 0 4px; font-size: 14px; }
  .card p { margin: 0; font-size: 12px; color: #64748b; }
  .field { padding: 6px 8px; border-radius: 6px; margin-bottom: 8px; transition: background 0.15s ease-out; }
  .field:has(input:focus) { background: #eff6ff; }
  .field label { display: block; font-size: 11px; color: #64748b; margin-bottom: 2px; }
  .field input { padding: 4px 8px; border: 1px solid #cbd5e1; border-radius: 4px; font-size: 13px; width: 100%; }
  .label { font-size: 11px; color: #94a3b8; margin-bottom: 8px; }
</style>
<div class="label">Cards with images get a grid layout:</div>
<div class="cards">
  <div class="card">
    <img src="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='80' height='80'%3E%3Crect fill='%233b82f6' width='80' height='80' rx='6'/%3E%3C/svg%3E">
    <div><h3>With image</h3><p>Grid layout applied</p></div>
  </div>
  <div class="card">
    <h3>Without image</h3>
    <p>Simple stack layout</p>
  </div>
</div>
<div class="label" style="margin-top: 12px">Click inputs — the field highlights:</div>
<div class="field">
  <label>Name</label>
  <input type="text" placeholder="Focus me">
</div>
<div class="field">
  <label>Email</label>
  <input type="email" placeholder="Focus me too">
</div>
```

# Grouping and specificity

{{clear: slide}}

{{show: grouping-code typewriter 1.5s linear}} Two final pieces. Grouping keeps your CSS DRY. Specificity determines which rule wins when selectors conflict.

{{focus: grouping}} {{zoom: 1.2x}} {{pulse: grouping}} {{annotate: grouping "Comma-separated"}} Comma-separate selectors to apply the same styles to multiple targets. `h1, h2, h3` targets all three heading levels. Each selector in the list is independent — if one is invalid, the others still work.

{{zoom: 1x}} {{focus: spec-calc}} {{zoom: 1.2x}} {{pulse: spec-calc}} {{annotate: spec-calc "Specificity scoring"}} Specificity is a three-part score: IDs, classes, and types. `#nav .link a` scores 1-1-1. `.card:hover` scores 0-2-0 (class plus pseudo-class). `div p` scores 0-0-2. Higher numbers win, read left to right.

{{zoom: 1x}} {{focus: spec-rules}} {{zoom: 1.2x}} {{annotate: spec-rules "The rules"}} IDs beat any number of classes. Classes beat any number of types. `:not()` itself adds nothing, but its argument does — `:not(.active)` has the specificity of `.active`. `:has()` takes the specificity of its most specific argument. `!important` overrides everything but creates maintenance nightmares.

{{zoom: 1x}} {{focus: spec-tips}} {{zoom: 1.2x}} {{pulse: spec-tips}} {{annotate: spec-tips "Keep it flat"}} The best specificity strategy: keep it flat. Use single classes. Avoid IDs in selectors. Avoid nesting more than two levels. When specificity battles start, the architecture is wrong — not the selector.

{{zoom: 1x}} {{focus: none}}

```code:grouping-code lang=css
/* Grouping: same styles, multiple selectors */
h1, h2, h3 {
  font-family: system-ui, sans-serif;
  line-height: 1.2;
}

/* Specificity: 0-0-1 (one type) */
p { color: #334155; }

/* Specificity: 0-1-0 (one class) */
.text { color: #1e293b; }

/* Specificity: 0-2-0 (two classes) */
.card .text { color: #0f172a; }

/* Specificity: 1-0-0 (one ID) — wins */
#main { color: #000; }

/* :not() and :has() specificity */
.card:not(.hidden) { display: block; }  /* 0-2-0 */
.list:has(.active) { border: 1px solid; } /* 0-2-0 */

/* Avoid: specificity war */
/* #sidebar .nav ul li a.active { } */
/* Use instead: */
.nav-link.active { font-weight: 700; }
---
grouping: 2-5
spec-calc: 8-17
spec-rules: 20-21
spec-tips: 24-25
```

# Selector patterns in the wild

{{clear: slide}}

{{split}} {{show: patterns-code typewriter 2s linear}} {{show: patterns-preview reveal 0.5s spring}} Selectors get powerful when you combine them. Here are patterns you'll use constantly.

{{focus: zebra}} {{zoom: 1.2x}} {{pulse: zebra}} {{annotate: zebra "Zebra striping"}} Alternating row colors. One selector. No alternating classes. `nth-child(even)` handles it. This pattern scales to any number of rows.

{{zoom: 1x}} {{focus: lobotomized}} {{zoom: 1.2x}} {{pulse: lobotomized}} {{annotate: lobotomized "Owl selector"}} The "lobotomized owl" — `* + *` — matches every element that has a previous sibling. It adds margin-top to everything except the first child. One line replaces margin-bottom on every element.

{{zoom: 1x}} {{focus: input-suite}} {{zoom: 1.2x}} {{annotate: input-suite "Input styling"}} A complete input style in four selectors. Default state, focus ring, valid state, invalid state. Layer them carefully — focus should override valid/invalid so the user always knows where they are.

{{zoom: 1x}} {{focus: sr-only}} {{zoom: 1.2x}} {{pulse: sr-only}} {{annotate: sr-only "Screen reader only"}} The `.sr-only` pattern hides content visually but keeps it accessible to screen readers. This is one of the most important utility classes in web development. Every project needs it.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:patterns-code lang=css
/* Zebra-striped table */
tbody tr:nth-child(even) {
  background: hsl(220, 15%, 97%);
}

/* Lobotomized owl (spacing) */
.stack > * + * {
  margin-top: 16px;
}

/* Complete input styling */
.input {
  border: 1px solid #cbd5e1;
  border-radius: 6px;
  padding: 8px 12px;
}
.input:focus {
  outline: 2px solid #3b82f6;
  outline-offset: 2px;
}
.input:valid { border-color: #22c55e; }
.input:invalid:not(:focus) { border-color: #ef4444; }

/* Screen reader only — visually hidden */
.sr-only {
  position: absolute;
  width: 1px;
  height: 1px;
  padding: 0;
  margin: -1px;
  overflow: hidden;
  clip: rect(0, 0, 0, 0);
  white-space: nowrap;
  border: 0;
}
---
zebra: 2-4
lobotomized: 7-9
input-suite: 12-21
sr-only: 24-33
```

```preview:patterns-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; font-size: 13px; }
  table { width: 100%; border-collapse: collapse; margin-bottom: 12px; }
  th { text-align: left; padding: 6px 8px; border-bottom: 2px solid #e2e8f0; font-size: 11px; color: #64748b; }
  td { padding: 6px 8px; border-bottom: 1px solid #f1f5f9; }
  tbody tr:nth-child(even) { background: hsl(220, 15%, 97%); }
  .stack > * + * { margin-top: 12px; }
  .stack { margin-bottom: 12px; }
  .stack-item { background: #eff6ff; padding: 8px 12px; border-radius: 4px; font-size: 12px; }
  .label { font-size: 11px; color: #94a3b8; margin-bottom: 4px; }
</style>
<div class="label">Zebra striping with nth-child:</div>
<table>
  <thead><tr><th>Name</th><th>Role</th></tr></thead>
  <tbody>
    <tr><td>Alice</td><td>Engineer</td></tr>
    <tr><td>Bob</td><td>Designer</td></tr>
    <tr><td>Carol</td><td>PM</td></tr>
    <tr><td>Dave</td><td>Engineer</td></tr>
  </tbody>
</table>
<div class="label">Owl selector spacing (no margin on first):</div>
<div class="stack">
  <div class="stack-item">First (no top margin)</div>
  <div class="stack-item">Second</div>
  <div class="stack-item">Third</div>
</div>
```
