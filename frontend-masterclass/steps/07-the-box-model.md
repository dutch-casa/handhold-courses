---
title: The box model
---

# Everything is a box

{{show: box-diagram grow 0.5s spring}} Every element on a webpage is a rectangular box. Headings, paragraphs, images, buttons — all rectangles. The browser doesn't know about circles or organic shapes. It knows about rectangles.

{{focus: content-layer}} {{pulse: content-layer}} {{annotate: content-layer "Content"}} The innermost layer is content. Your text, your image, your child elements. This is the actual stuff.

{{focus: padding-layer}} {{pulse: padding-layer}} {{annotate: padding-layer "Padding"}} Padding is space between the content and the border. It's inside the box. Background color fills the padding area. Click on padding and you're still clicking on the element.

{{focus: border-layer}} {{pulse: border-layer}} {{annotate: border-layer "Border"}} The border wraps the padding. It has a width, a style, and a color. It's part of the element.

{{focus: margin-layer}} {{pulse: margin-layer}} {{annotate: margin-layer "Margin"}} Margin is space outside the border. It pushes other elements away. It's transparent — the parent's background shows through. Margin is not part of the element.

{{focus: none}} Content, padding, border, margin. Inside to outside. That's the box model.

```data:box-diagram type=array
["Margin", "Border", "Padding", "Content"]
---
margin-layer: 0
border-layer: 1
padding-layer: 2
content-layer: 3
```

# The sizing problem

{{clear: slide}}

{{split}} {{show: sizing-code reveal 0.3s}} {{show: sizing-preview reveal 0.5s spring}} You set `width: 200px`. How wide is the element? It depends.

{{focus: content-box}} {{zoom: 1.2x}} {{pulse: content-box}} {{annotate: content-box "content-box"}} By default, `width` applies to the content only. Add 20px padding on each side plus a 1px border on each side and the element is actually 242px wide. The math: 200 + 20 + 20 + 1 + 1 = 242.

{{zoom: 1x}} {{focus: border-box}} {{zoom: 1.2x}} {{pulse: border-box}} {{annotate: border-box "border-box"}} `box-sizing: border-box` changes the math. Now `width: 200px` means 200px total, including padding and border. The content shrinks to fit. The element is exactly 200px wide, as you intended.

{{zoom: 1x}} {{focus: universal-fix}} {{zoom: 1.3x}} {{pulse: universal-fix}} {{annotate: universal-fix "The fix"}} This rule goes at the top of every stylesheet. Apply border-box to everything. Now width means what you think it means. Every professional project does this.

{{focus: none}} {{unsplit}}

```code:sizing-code lang=css
/* Default: content-box (the confusing one) */
.box-content {
  box-sizing: content-box; /* default */
  width: 200px;
  padding: 20px;
  border: 1px solid gray;
  /* Actual width: 200 + 40 + 2 = 242px! */
}

/* Better: border-box */
.box-border {
  box-sizing: border-box;
  width: 200px;
  padding: 20px;
  border: 1px solid gray;
  /* Actual width: 200px. Content shrinks. */
}

/* The universal fix: apply to everything */
*, *::before, *::after {
  box-sizing: border-box;
}
---
content-box: 2-8
border-box: 11-17
universal-fix: 20-22
```

```preview:sizing-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 16px; }
  .demo { margin-bottom: 16px; }
  .demo-label { font-size: 12px; color: #64748b; margin-bottom: 4px; }
  .box { padding: 20px; border: 2px solid #3b82f6; background: #eff6ff; font-size: 14px; text-align: center; }
  .content-box { box-sizing: content-box; width: 200px; }
  .border-box { box-sizing: border-box; width: 200px; }
  .ruler { width: 200px; height: 2px; background: #ef4444; margin-top: 4px; }
  .ruler-label { font-size: 11px; color: #ef4444; }
</style>
<div class="demo">
  <div class="demo-label">content-box: width 200px (actual: 244px)</div>
  <div class="box content-box">Content</div>
  <div class="ruler"></div>
  <div class="ruler-label">200px reference</div>
</div>
<div class="demo">
  <div class="demo-label">border-box: width 200px (actual: 200px)</div>
  <div class="box border-box">Content</div>
  <div class="ruler"></div>
  <div class="ruler-label">200px reference — matches!</div>
</div>
```

# Padding

{{clear: slide}}

{{show: padding-code typewriter 1.5s linear}} Padding creates breathing room inside the box. Between the content and the border.

{{focus: individual}} {{zoom: 1.2x}} {{pulse: individual}} {{annotate: individual "Individual sides"}} You can set each side independently. `padding-top`, `padding-right`, `padding-bottom`, `padding-left`. Four separate properties.

{{zoom: 1x}} {{pan: shorthand}} {{focus: shorthand}} {{zoom: 1.2x}} {{pulse: shorthand}} {{annotate: shorthand "Shorthand"}} The shorthand `padding` accepts one to four values. One value: all sides. Two values: top-bottom, left-right. Four values: clockwise from top — top, right, bottom, left. The mnemonic: TRouBLe.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Use consistent spacing values. Don't use random numbers like 13px or 37px. Pick a scale — 4, 8, 12, 16, 24, 32, 48 — and stick with it. Tailwind's spacing scale is this exact idea baked into a framework.

```code:padding-code lang=css
/* Individual sides */
.card {
  padding-top: 16px;
  padding-right: 24px;
  padding-bottom: 16px;
  padding-left: 24px;
}

/* Shorthand: all sides */
.box { padding: 16px; }

/* Shorthand: vertical | horizontal */
.box { padding: 16px 24px; }

/* Shorthand: top | right | bottom | left */
.box { padding: 16px 24px 16px 24px; }
---
individual: 2-7
shorthand: 10-16
```

# Margin and collapse

{{clear: slide}}

{{show: margin-code typewriter 1.5s linear}} Margin pushes elements apart. Same syntax as padding. But margin has a quirk.

{{focus: basic-margin}} {{zoom: 1.2x}} {{annotate: basic-margin "Space outside"}} Margin creates space outside the border. Two elements with `margin: 16px` will have 16px of space between them — not 32px.

{{zoom: 1x}} {{pan: collapse}} {{focus: collapse}} {{zoom: 1.3x}} {{pulse: collapse}} {{annotate: collapse "Margin collapse"}} That's margin collapse. Vertical margins of adjacent block elements merge into one. The larger margin wins. The first paragraph has 24px bottom margin. The second has 16px top margin. The gap between them is 24px — the larger of the two — not 40px.

{{zoom: 1x}} {{pan: no-collapse}} {{focus: no-collapse}} {{zoom: 1.2x}} {{annotate: no-collapse "Horizontal: no collapse"}} Horizontal margins never collapse. Left and right margins always add up. This is why flex and grid layouts are easier to reason about — they disable margin collapse.

{{zoom: 1x}} {{pan: auto-trick}} {{focus: auto-trick}} {{zoom: 1.2x}} {{pulse: auto-trick}} {{annotate: auto-trick "The centering trick"}} `margin: 0 auto` centers a block element. `auto` distributes available space equally on left and right. The element must have a width — without one, there's no "available space" to distribute.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:margin-code lang=css
/* Basic margin */
.card {
  margin: 24px;
}

/* Margin collapse: vertical margins merge */
.paragraph-1 {
  margin-bottom: 24px; /* This wins */
}
.paragraph-2 {
  margin-top: 16px;    /* This is swallowed */
}
/* Gap between them: 24px, not 40px */

/* Horizontal margins don't collapse */
.side-by-side {
  margin-left: 16px;
  margin-right: 16px;
  /* Total horizontal: 32px */
}

/* Center a block element */
.container {
  width: 800px;
  margin: 0 auto;
}
---
basic-margin: 2-4
collapse: 7-14
no-collapse: 17-21
auto-trick: 24-27
```

# Display types

{{clear: slide}}

{{split}} {{show: display-code reveal 0.3s}} {{show: display-preview reveal 0.5s spring}} The `display` property controls how an element participates in layout. Three core values.

{{focus: block-display}} {{zoom: 1.2x}} {{pulse: block-display}} {{annotate: block-display "Block"}} Block elements take the full width. They stack vertically. Width, height, margin, and padding all work as expected. Divs, headings, paragraphs — block by default.

{{zoom: 1x}} {{focus: inline-display}} {{zoom: 1.2x}} {{pulse: inline-display}} {{annotate: inline-display "Inline"}} Inline elements flow with text. They sit side by side, wrapping at the end of the line. Width and height are ignored. Vertical margin and padding don't push other elements. Links, spans, strong — inline by default.

{{zoom: 1x}} {{focus: inline-block}} {{zoom: 1.2x}} {{pulse: inline-block}} {{annotate: inline-block "Inline-block"}} Inline-block combines both. It flows with text like inline, but it respects width, height, and vertical margin like block. Useful for buttons, badges, and chips that should sit in a row but need box control.

{{focus: none}} {{unsplit}}

```code:display-code lang=css
/* Block: full width, stacks vertically */
.block-demo {
  display: block;
  width: 200px;
  padding: 16px;
  margin: 8px 0;
}

/* Inline: flows with text */
.inline-demo {
  display: inline;
  /* width and height are IGNORED */
  padding: 4px 8px;
}

/* Inline-block: flows inline but respects box */
.chip {
  display: inline-block;
  padding: 4px 12px;
  border-radius: 16px;
  background: #e2e8f0;
}
---
block-display: 2-7
inline-display: 10-14
inline-block: 17-22
```

```preview:display-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 16px; font-size: 14px; }
  .label { font-size: 12px; color: #64748b; margin-bottom: 4px; margin-top: 12px; }
  .block { display: block; background: #dbeafe; border: 1px solid #3b82f6; padding: 8px; margin: 4px 0; border-radius: 4px; }
  .inline { display: inline; background: #dcfce7; border: 1px solid #22c55e; padding: 4px 8px; border-radius: 4px; }
  .iblock { display: inline-block; background: #fef3c7; border: 1px solid #f59e0b; padding: 8px 16px; margin: 4px; border-radius: 16px; }
</style>
<div class="label">Block (full width, stacks):</div>
<div class="block">Block A</div>
<div class="block">Block B</div>
<div class="label">Inline (flows with text):</div>
<p>Text with <span class="inline">inline A</span> and <span class="inline">inline B</span> elements.</p>
<div class="label">Inline-block (flows inline, respects box):</div>
<span class="iblock">Chip A</span>
<span class="iblock">Chip B</span>
<span class="iblock">Chip C</span>
```

# Putting it together

{{clear: slide}}

{{show: complete-box typewriter 1.5s linear}} Here's a card component with every box model property in play.

{{pan: reset}} {{focus: reset}} {{zoom: 1.2x}} {{pulse: reset}} {{annotate: reset "Global reset"}} Border-box on everything. No surprises with width calculations.

{{zoom: 1x}} {{pan: card-box}} {{focus: card-box}} {{zoom: 1.2x}} {{annotate: card-box "The card"}} 24px padding for internal breathing room. 1px border for definition. 16px margin to push siblings away. `max-width` with margin auto centers it in the container.

{{zoom: 1x}} {{pan: inner-spacing}} {{focus: inner-spacing}} {{zoom: 1.2x}} {{annotate: inner-spacing "Internal rhythm"}} Inside the card, headings and paragraphs use margin for vertical rhythm. The paragraph has smaller margin than the heading — establishing a hierarchy of space that mirrors the content hierarchy.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Every layout in CSS is boxes controlling their size, internal spacing, borders, and external spacing. Master the box model and layout becomes predictable.

```code:complete-box lang=css
/* Reset */
*, *::before, *::after {
  box-sizing: border-box;
  margin: 0;
}

/* The card */
.card {
  max-width: 480px;
  margin: 16px auto;
  padding: 24px;
  border: 1px solid #e2e8f0;
  border-radius: 8px;
  background: white;
}

/* Internal rhythm */
.card h2 {
  margin-bottom: 12px;
}

.card p {
  margin-bottom: 8px;
  line-height: 1.6;
}
---
reset: 2-5
card-box: 8-15
inner-spacing: 18-25
```
