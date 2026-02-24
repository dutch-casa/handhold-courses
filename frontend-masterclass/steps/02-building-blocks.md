---
title: Building blocks
---

# Text has hierarchy

{{show: heading-levels typewriter 1.5s linear}} A page without hierarchy is a wall of text. Headings break it up. They tell the reader — and the browser — what's important.

{{focus: h1}} {{zoom: 1.2x}} {{annotate: h1 "The title"}} h1 is the page title. One per page. It's the biggest, most important heading.

{{zoom: 1x}} {{focus: h2}} {{annotate: h2 "Sections"}} h2 marks major sections. A page might have three or four of these.

{{zoom: 1x}} {{focus: h3}} {{annotate: h3 "Subsections"}} h3 marks subsections within an h2. The hierarchy goes all the way to h6, but you'll rarely go past h3.

{{zoom: 1x}} {{focus: paragraph}} Paragraphs sit between headings. The `<p>` tag wraps body text. Browsers add vertical spacing between paragraphs automatically.

{{zoom: 1x}} {{focus: none}} The rule: never skip heading levels. Don't jump from h1 to h3. Screen readers use headings to build a table of contents. Skipping levels breaks that outline.

```code:heading-levels lang=html
<h1>The Complete Guide to Coffee</h1>

<h2>Brewing Methods</h2>
<p>There are dozens of ways to brew coffee,
   but three methods dominate home brewing.</p>

<h3>Pour Over</h3>
<p>Hot water, poured slowly over ground coffee.
   Simple, clean, and consistent.</p>

<h3>French Press</h3>
<p>Coarse grounds steeped in hot water for
   four minutes, then pressed through a filter.</p>

<h2>Choosing Beans</h2>
<p>The bean matters more than the method.</p>
---
h1: 1
h2: 3, 15
h3: 7, 11
paragraph: 4-5, 8-9, 12-13, 16
```

# Text formatting

{{clear: slide}}

{{split}} {{show: formatting-code slide 0.3s}} {{show: formatting-preview slide 0.5s spring}} Text inside paragraphs can be formatted. On the left, the HTML. On the right, how it renders.

{{focus: strong-tag}} {{annotate: strong-tag "Strong importance"}} `<strong>` means this text is important. Browsers render it bold, but the meaning matters more than the look. Screen readers emphasize it.

{{focus: em-tag}} {{annotate: em-tag "Stress emphasis"}} `<em>` adds emphasis. Like when you stress a word in speech. "I said do it now." Rendered italic by default.

{{focus: code-tag}} {{annotate: code-tag "Inline code"}} `<code>` marks inline code. It renders in a monospace font. Use it for variable names, function calls, terminal commands — anything that's code.

{{focus: br-tag}} {{annotate: br-tag "Line break"}} `<br>` forces a line break. No closing tag. Use it sparingly — for addresses or poetry, not for spacing. If you need spacing, use CSS.

{{focus: none}} These are inline elements. They flow within text, not as separate blocks. A `<strong>` inside a `<p>` stays inside the paragraph.

{{unsplit}} {{clear: slide}}

```code:formatting-code lang=html
<p>
  Coffee should be <strong>freshly ground</strong>
  right before brewing.
</p>
<p>
  The grind size <em>really</em> matters.
  Use <code>medium-coarse</code> for French press.
</p>
<p>
  123 Bean Street<br>
  Portland, OR 97201
</p>
---
strong-tag: 2
em-tag: 6
code-tag: 7
br-tag: 10
```

```preview:formatting-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; line-height: 1.6; color: #1a1a1a; }
  code { background: #f1f5f9; padding: 2px 6px; border-radius: 4px; font-size: 0.9em; }
</style>
<p>Coffee should be <strong>freshly ground</strong> right before brewing.</p>
<p>The grind size <em>really</em> matters. Use <code>medium-coarse</code> for French press.</p>
<p>123 Bean Street<br>Portland, OR 97201</p>
```

# Links connect the web

{{show: link-anatomy typewriter 1.5s linear}} The `<a>` tag is the most important element on the web. Without it, there is no web — just isolated documents.

{{focus: basic-link}} {{zoom: 1.2x}} {{annotate: basic-link "The anchor tag"}} The href attribute is the destination. The text between the tags is what the user clicks. This is a link to another page on the same site.

{{zoom: 1x}} {{focus: external-link}} {{annotate: external-link "External link"}} An absolute URL — starting with https — points to another website entirely.

{{zoom: 1x}} {{focus: section-link}} {{annotate: section-link "Same-page jump"}} A hash link jumps to an element with a matching ID on the same page. Click it, and the browser scrolls to that element.

{{zoom: 1x}} {{focus: none}} Links are how humans and search engines discover content. Every page that exists but has no links pointing to it is invisible. Links aren't just navigation — they're the connective tissue of the web.

```code:link-anatomy lang=html
<a href="/about">About us</a>

<a href="https://mozilla.org">Mozilla</a>

<a href="#brewing-methods">Jump to Brewing</a>
---
basic-link: 1
external-link: 3
section-link: 5
```

# Images

{{clear: slide}}

{{split}} {{show: image-code slide 0.3s}} {{show: image-preview slide 0.5s spring}} Images use the `<img>` tag. No closing tag — there's nothing "inside" an image.

{{focus: src-attr}} {{zoom: 1.2x}} {{annotate: src-attr "Source"}} The src attribute tells the browser where to find the image file. It can be a relative path or a full URL.

{{zoom: 1x}} {{focus: alt-attr}} {{zoom: 1.2x}} {{annotate: alt-attr "Accessibility"}} The alt attribute describes the image in words. This isn't optional decoration. When an image can't load, the alt text appears instead. When a screen reader encounters the image, it reads the alt text aloud. Without alt, the image is invisible to blind users.

{{zoom: 1x}} {{focus: dimensions}} {{annotate: dimensions "Prevents layout shift"}} Width and height attributes tell the browser the image dimensions before it loads. Without them, the page jumps around as images load. This is called layout shift, and it's one of the worst user experiences on the web.

{{zoom: 1x}} {{focus: none}} Every image needs three things: a source, alt text, and dimensions. No exceptions.

{{unsplit}} {{clear: slide}}

```code:image-code lang=html
<img
  src="coffee-beans.jpg"
  alt="Dark roasted coffee beans in a white bowl"
  width="600"
  height="400"
>
---
src-attr: 2
alt-attr: 3
dimensions: 4-5
```

```preview:image-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; background: #f8f9fa; }
  img { max-width: 100%; height: auto; border-radius: 8px; }
  .placeholder { background: #e2e8f0; width: 100%; aspect-ratio: 3/2; display: flex; align-items: center; justify-content: center; border-radius: 8px; color: #64748b; font-size: 14px; }
</style>
<div class="placeholder">coffee-beans.jpg (600x400)</div>
```

# Lists

{{show: list-types typewriter 1.5s linear}} Two kinds of lists. Unordered for collections where sequence doesn't matter. Ordered for steps or rankings.

{{focus: unordered}} {{zoom: 1.2x}} {{annotate: unordered "Bullet points"}} `<ul>` creates a bulleted list. Each `<li>` is one item. The order of beans doesn't matter, so bullets are right.

{{zoom: 1x}} {{focus: ordered}} {{zoom: 1.2x}} {{annotate: ordered "Numbered steps"}} `<ol>` creates a numbered list. Use it when sequence matters. These brewing steps must happen in order.

{{zoom: 1x}} {{focus: li-items}} {{annotate: li-items "List items"}} `<li>` goes inside either `<ul>` or `<ol>`. Never outside a list. A bare `<li>` is invalid HTML.

{{zoom: 1x}} {{focus: none}} Lists can nest. An `<li>` can contain another `<ul>` or `<ol>`. This creates sub-items. Navigation menus, file trees, outlines — all built from nested lists.

```code:list-types lang=html
<h3>Bean Types</h3>
<ul>
  <li>Arabica</li>
  <li>Robusta</li>
  <li>Liberica</li>
</ul>

<h3>Brewing Steps</h3>
<ol>
  <li>Boil water to 96 degrees</li>
  <li>Grind beans medium-coarse</li>
  <li>Add grounds to press</li>
  <li>Pour water, wait four minutes</li>
  <li>Press and serve</li>
</ol>
---
unordered: 2-6
ordered: 9-15
li-items: 3-5, 10-14
```

# Tables

{{clear: slide}}

{{split}} {{show: table-code slide 0.3s}} {{show: table-preview slide 0.5s spring}} Tables display data in rows and columns. Not for layout — for data.

{{focus: thead}} {{zoom: 1.2x}} {{annotate: thead "Column headers"}} `<thead>` wraps the header row. `<th>` cells are header cells — bold and centered by default. They tell the reader what each column means.

{{zoom: 1x}} {{focus: tbody}} {{zoom: 1.2x}} {{annotate: tbody "Data rows"}} `<tbody>` wraps the data rows. `<td>` cells hold the actual data. Each `<tr>` is one row.

{{zoom: 1x}} {{focus: none}} Tables are for tabular data. Comparison charts, pricing tiers, schedules, statistics. If the data makes sense in a spreadsheet, it belongs in a table. If it doesn't, use a list or a grid layout instead.

{{unsplit}} {{clear: slide}}

```code:table-code lang=html
<table>
  <thead>
    <tr>
      <th>Method</th>
      <th>Time</th>
      <th>Grind</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Pour Over</td>
      <td>3 min</td>
      <td>Medium</td>
    </tr>
    <tr>
      <td>French Press</td>
      <td>4 min</td>
      <td>Coarse</td>
    </tr>
    <tr>
      <td>Espresso</td>
      <td>25 sec</td>
      <td>Fine</td>
    </tr>
  </tbody>
</table>
---
thead: 2-8
tbody: 9-24
```

```preview:table-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; }
  table { border-collapse: collapse; width: 100%; }
  th, td { padding: 12px 16px; text-align: left; border-bottom: 1px solid #e2e8f0; }
  th { background: #f8fafc; font-weight: 600; color: #334155; }
  tr:hover td { background: #f1f5f9; }
</style>
<table>
  <thead><tr><th>Method</th><th>Time</th><th>Grind</th></tr></thead>
  <tbody>
    <tr><td>Pour Over</td><td>3 min</td><td>Medium</td></tr>
    <tr><td>French Press</td><td>4 min</td><td>Coarse</td></tr>
    <tr><td>Espresso</td><td>25 sec</td><td>Fine</td></tr>
  </tbody>
</table>
```

# Block vs inline

{{show: block-inline typewriter 1.5s linear}} Every HTML element is either block or inline. This distinction controls how elements flow on the page.

{{focus: block-els}} {{zoom: 1.2x}} {{annotate: block-els "Full width"}} Block elements take the full width available. They stack vertically — each one starts on a new line. Headings, paragraphs, divs, lists, tables — all block.

{{zoom: 1x}} {{focus: inline-els}} {{zoom: 1.2x}} {{annotate: inline-els "Flow with text"}} Inline elements flow with text. They sit side by side, wrapping naturally. Links, strong, em, code, span, img — all inline.

{{zoom: 1x}} {{focus: rule}} {{annotate: rule "The nesting rule"}} Here's the rule that matters: block elements can contain inline elements. But inline elements cannot contain block elements. A paragraph can contain a link. But a link cannot contain a paragraph.

{{zoom: 1x}} {{focus: none}} This isn't arbitrary. It maps to how content flows. A paragraph is a block of text. Bold text is emphasis within that block. The hierarchy is physical — blocks are containers, inlines are content.

```code:block-inline lang=html
<!-- Block: starts on new line, full width -->
<h1>Title</h1>
<p>A paragraph of text.</p>
<div>A generic container.</div>

<!-- Inline: flows with text -->
<p>
  Click <a href="/buy">here</a> to
  <strong>buy now</strong>.
</p>

<!-- Valid: block contains inline -->
<p>Text with <em>emphasis</em></p>

<!-- Invalid: inline contains block -->
<!-- <a><div>Don't do this</div></a> -->
---
block-els: 2-4
inline-els: 7-10
rule: 12-16
```

# The div and the span

{{clear: slide}}

{{show: div-span typewriter 1.5s linear}} Two generic elements. No meaning. No styling. Just containers.

{{focus: div-usage}} {{zoom: 1.2x}} {{annotate: div-usage "Generic block"}} `<div>` is a block container. It groups things together. By itself, it does nothing visible. But it's the hook you'll hang CSS on later — wrapping elements in a div to style them as a group.

{{zoom: 1x}} {{focus: span-usage}} {{zoom: 1.2x}} {{annotate: span-usage "Generic inline"}} `<span>` is an inline container. Same idea, but for text within a paragraph. Wrap a word in a span to style just that word.

{{zoom: 1x}} {{focus: none}} Div and span are the building blocks of layout. But don't reach for them first. If there's a semantic element that fits — `<nav>`, `<article>`, `<section>` — use it instead. Semantic elements carry meaning. Divs carry nothing.

We'll cover semantic elements in the next lesson.

```code:div-span lang=html
<!-- div: groups block content -->
<div>
  <h2>Featured Blend</h2>
  <p>A smooth, full-bodied roast.</p>
  <a href="/buy">Buy now</a>
</div>

<!-- span: marks inline content -->
<p>
  Price: <span>$14.99</span> per bag
</p>
---
div-usage: 2-6
span-usage: 9-11
```
