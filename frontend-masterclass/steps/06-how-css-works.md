---
title: How CSS works
---

# Rules

{{show: first-rule typewriter 1.5s linear}} CSS is a set of rules. Each rule says: "find these elements, give them these styles."

{{focus: selector-part}} {{zoom: 1.3x}} {{pulse: selector-part}} {{annotate: selector-part "Selector"}} The selector targets elements. This one targets every `<h1>`. Any CSS selector works — classes, IDs, attributes, combinations.

{{zoom: 1x}} {{pan: declaration}} {{focus: declaration}} {{zoom: 1.2x}} {{pulse: declaration}} {{annotate: declaration "Declaration block"}} The curly braces hold declarations. Each declaration is a property-value pair. `color` is the property. `#1e293b` is the value. The semicolon ends the declaration.

{{zoom: 1x}} {{pan: multiple-props}} {{focus: multiple-props}} {{zoom: 1.2x}} {{annotate: multiple-props "Multiple declarations"}} One rule, multiple declarations. This selector targets elements with class `card` and applies four styles at once.

{{zoom: 1x}} {{pan: none}} {{focus: none}} That's the whole syntax. Selector, braces, property-value pairs. Everything else in CSS is about which selectors match which elements, and what happens when multiple rules compete.

```code:first-rule lang=css
h1 {
  color: #1e293b;
}

.card {
  background: white;
  border-radius: 8px;
  padding: 24px;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
}
---
selector-part: 1
declaration: 1-3
multiple-props: 5-10
```

# The cascade

{{clear: slide}}

{{show: cascade-flow grow 0.5s spring}} When multiple rules target the same element with the same property, the browser needs to pick a winner. The cascade is that decision process.

{{focus: origin-step}} {{pulse: origin-step}} {{annotate: origin-step "1. Origin"}} First check: where does the rule come from? Browser defaults lose to your stylesheets. Your stylesheets lose to inline styles. `!important` overrides everything — and that's exactly why you should almost never use it.

{{trace: spec-path}} {{focus: specificity-step}} {{pulse: specificity-step}} {{annotate: specificity-step "2. Specificity"}} Same origin? Compare specificity. This is the scoring system that decides which selector wins. We'll break it down in a moment.

{{trace: order-path}} {{focus: order-step}} {{pulse: order-step}} {{annotate: order-step "3. Source order"}} Same specificity? Last rule wins. The one that appears later in the stylesheet overrides the earlier one. Simple. Predictable.

{{flow: full-cascade}} {{focus: none}} {{trace: none}} Origin, specificity, order. That's the cascade algorithm. Every property on every element goes through this. When your styles "don't work," one of these three steps is why.

{{flow: none}}

```diagram:cascade-flow
origin [service]
specificity [service]
order [service]
winner [service]
origin --same origin?--> specificity
specificity --same specificity?--> order
order --last wins--> winner
---
origin-step: origin
specificity-step: specificity
order-step: order
spec-path: origin, specificity
order-path: specificity, order
full-cascade: origin, specificity, order, winner
```

# Specificity

{{clear: slide}}

{{show: specificity-code typewriter 2s linear}} Specificity is a scoring system. Every selector gets a score. Higher score wins.

{{focus: type-sel}} {{zoom: 1.2x}} {{pulse: type-sel}} {{annotate: type-sel "0-0-1"}} Type selectors score lowest. `h1`, `p`, `div` — each tag name adds 1 to the rightmost column. This rule scores 0-0-1.

{{zoom: 1x}} {{pan: class-sel}} {{focus: class-sel}} {{zoom: 1.2x}} {{pulse: class-sel}} {{annotate: class-sel "0-1-0"}} Class selectors, attribute selectors, and pseudo-classes score in the middle column. `.card` scores 0-1-0. That beats any number of type selectors.

{{zoom: 1x}} {{pan: id-sel}} {{focus: id-sel}} {{zoom: 1.2x}} {{pulse: id-sel}} {{annotate: id-sel "1-0-0"}} ID selectors score highest. `#header` scores 1-0-0. One ID outweighs any combination of classes and types.

{{zoom: 1x}} {{pan: combined}} {{focus: combined}} {{zoom: 1.2x}} {{annotate: combined "0-2-1"}} Combined selectors add up. `nav.main .link` has one type (nav), two classes (.main, .link) = 0-2-1. Specificity is not a single number — each column is compared independently.

{{zoom: 1x}} {{pan: none}} {{focus: none}} The scoring rule: compare left to right. 1-0-0 beats 0-99-99. IDs always beat classes. Classes always beat types. This is why ID selectors make CSS hard to override — they win almost everything.

```code:specificity-code lang=css
/* 0-0-1: type selector */
h1 { color: gray; }

/* 0-1-0: class selector */
.title { color: blue; }

/* 1-0-0: ID selector */
#main-title { color: red; }

/* 0-2-1: combined (1 type + 2 classes) */
nav.main .link { color: green; }

/* 0-1-1: combined (1 type + 1 class) */
p.intro { font-size: 18px; }
---
type-sel: 2
class-sel: 5
id-sel: 8
combined: 11, 14
```

# Inheritance

{{clear: slide}}

{{split}} {{show: inherit-code reveal 0.3s}} {{show: inherit-tree reveal 0.5s spring}} Some CSS properties pass down from parent to child. This is inheritance.

{{focus: inherits}} {{pulse: inherits}} {{annotate: inherits "Inherited properties"}} `color`, `font-family`, `font-size`, `line-height`, `letter-spacing` — these inherit. Set `color: #1e293b` on the body, and every text element inside inherits that color unless overridden.

{{focus: no-inherit}} {{pulse: no-inherit}} {{annotate: no-inherit "Not inherited"}} `background`, `border`, `padding`, `margin`, `width`, `height` — these don't inherit. If they did, every child would have the parent's background and borders. That would be chaos.

{{focus: override}} {{annotate: override "Override"}} You can always override an inherited value. The `<em>` inside a paragraph inherits the paragraph's color, but applying `color: blue` to em overrides the inheritance.

{{focus: none}} {{unsplit}}

The rule of thumb: properties that affect text inherit. Properties that affect the box don't. When in doubt, check MDN — it lists whether each property inherits.

```code:inherit-code lang=css
/* Set on body — all children inherit */
body {
  color: #1e293b;
  font-family: system-ui, sans-serif;
  line-height: 1.6;
}

/* These do NOT inherit */
.card {
  background: white;
  border: 1px solid #e2e8f0;
  padding: 24px;
}

/* Override inherited color */
em { color: #3b82f6; }
---
inherits: 2-5
no-inherit: 9-12
override: 15
```

```data:inherit-tree type=graph layout=tree
body -> div -> p -> em
---
inherits: body, div, p, em
no-inherit: div
override: em
```

# Connecting CSS to HTML

{{clear: slide}}

{{show: connect-methods typewriter 2s linear}} Three ways to apply CSS to a page. One is right. Two are compromises.

{{focus: external}} {{zoom: 1.2x}} {{pulse: external}} {{annotate: external "Best: external"}} An external stylesheet in a separate `.css` file, linked with `<link>`. This is the standard. Separate concerns: HTML for structure, CSS for presentation. The browser caches the CSS file — subsequent page loads are faster.

{{zoom: 1x}} {{pan: internal}} {{focus: internal}} {{zoom: 1.2x}} {{annotate: internal "OK: internal"}} A `<style>` tag in the head. The CSS lives inside the HTML file. Useful for single-page demos, critical CSS, or email templates. Not ideal for sites with multiple pages — you'd duplicate styles across every file.

{{zoom: 1x}} {{pan: inline}} {{focus: inline}} {{zoom: 1.2x}} {{pulse: inline}} {{annotate: inline "Avoid: inline"}} The `style` attribute directly on an element. Highest specificity — hard to override. Impossible to reuse. Use only when dynamically setting styles in JavaScript, and even then prefer classList.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Use external stylesheets. Link them in the head. Everything else is a special case.

```code:connect-methods lang=html
<!-- External: the right way -->
<head>
  <link rel="stylesheet" href="styles.css">
</head>

<!-- Internal: lives in the HTML -->
<head>
  <style>
    h1 { color: #1e293b; }
    .card { padding: 24px; }
  </style>
</head>

<!-- Inline: on the element itself (avoid) -->
<h1 style="color: red; font-size: 32px;">
  Don't do this
</h1>
---
external: 2-4
internal: 7-12
inline: 15-17
```

# The algorithm in action

{{clear: slide}}

{{split}} {{show: conflict-code reveal 0.3s}} {{show: conflict-result reveal 0.5s spring}} Let's trace the cascade. Three rules target the same heading. What color wins?

{{focus: rule-one}} {{pulse: rule-one}} {{annotate: rule-one "0-0-1"}} First rule: `h1` with specificity 0-0-1. Sets color to gray.

{{focus: rule-two}} {{pulse: rule-two}} {{annotate: rule-two "0-1-0"}} Second rule: `.title` with specificity 0-1-0. Sets color to blue. 0-1-0 beats 0-0-1, so blue is winning.

{{focus: rule-three}} {{pulse: rule-three}} {{annotate: rule-three "0-0-1"}} Third rule: `h1` again. Same specificity as rule one: 0-0-1. It came later in the file. Between two rules with equal specificity, the later one wins.

{{focus: winner}} {{pulse: winner}} {{annotate: winner "Blue wins"}} But `.title` at 0-1-0 still beats both `h1` rules at 0-0-1. The heading is blue. The cascade chose: highest specificity first, source order as tiebreaker.

{{focus: none}} {{unsplit}}

```code:conflict-code lang=css
/* Rule 1: specificity 0-0-1 */
h1 {
  color: gray;
}

/* Rule 2: specificity 0-1-0 */
.title {
  color: blue;
}

/* Rule 3: specificity 0-0-1 */
h1 {
  color: green;
}
---
rule-one: 2-4
rule-two: 7-9
rule-three: 12-14
winner: 7-9
```

```preview:conflict-result
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; background: #f8fafc; }
  h1 { color: gray; }
  .title { color: blue; }
  h1 { color: green; }
  .result { margin-top: 16px; padding: 12px; background: white; border-radius: 8px; font-size: 14px; color: #64748b; border: 1px solid #e2e8f0; }
  code { background: #f1f5f9; padding: 2px 6px; border-radius: 4px; }
</style>
<h1 class="title">Hello World</h1>
<div class="result">
  <strong>Winner:</strong> <code>.title { color: blue }</code><br>
  Specificity 0-1-0 beats 0-0-1 regardless of source order
</div>
```
