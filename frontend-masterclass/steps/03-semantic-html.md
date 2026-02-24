---
title: Semantic HTML
---

# Div soup

{{show: div-soup typewriter 2s linear}} Here's a page. It works. It renders. But something is wrong.

{{focus: outer-divs}} {{zoom: 1.2x}} {{pulse: outer-divs}} {{annotate: outer-divs "All divs"}} Every wrapper is a div. The navigation, the main content, the sidebar, the footer — all divs with class names hinting at their purpose.

{{zoom: 1x}} {{pan: inner-divs}} {{focus: inner-divs}} {{zoom: 1.2x}} {{pulse: inner-divs}} {{annotate: inner-divs "More divs"}} Inside those divs, more divs. The structure is there, hidden in class names that only humans reading the source code can interpret.

{{zoom: 1x}} {{pan: none}} {{focus: none}} A screen reader encounters this page and sees... boxes. Unlabeled, unstructured boxes. It can't tell the user "you're in the navigation" or "this is the main content." The meaning is invisible.

```code:div-soup lang=html
<div class="header">
  <div class="logo">My Site</div>
  <div class="nav">
    <div class="nav-item"><a href="/">Home</a></div>
    <div class="nav-item"><a href="/about">About</a></div>
    <div class="nav-item"><a href="/blog">Blog</a></div>
  </div>
</div>
<div class="main">
  <div class="article">
    <div class="article-title">How CSS Works</div>
    <div class="article-body">
      <div class="paragraph">The cascade is...</div>
    </div>
  </div>
  <div class="sidebar">
    <div class="widget">Recent Posts</div>
  </div>
</div>
<div class="footer">
  <div class="copyright">2024 My Site</div>
</div>
---
outer-divs: 1, 8, 9, 18, 20
inner-divs: 2-7, 10-17, 19, 21
```

# Tags that mean something

{{clear: slide}}

{{split}} {{show: semantic-code reveal 0.3s}} {{show: semantic-tree reveal 0.5s spring}} Same page. Same layout. But now the tags carry meaning.

{{focus: header-tag}} {{pulse: header-tag}} {{annotate: header-tag "Site header"}} `<header>` says "this is the top of the page." A screen reader announces it. Search engines understand it.

{{focus: nav-tag}} {{pulse: nav-tag}} {{annotate: nav-tag "Navigation"}} `<nav>` says "these are navigation links." Screen readers let users jump directly to the nav. Keyboard users can skip it.

{{focus: main-tag}} {{pulse: main-tag}} {{annotate: main-tag "Primary content"}} `<main>` says "this is the primary content." There should be exactly one per page. Screen readers have a shortcut to jump here.

{{focus: article-tag}} {{pulse: article-tag}} {{annotate: article-tag "Self-contained"}} `<article>` says "this content makes sense on its own." If you pulled it out of the page and put it somewhere else, it would still be meaningful.

{{focus: aside-tag}} {{pulse: aside-tag}} {{annotate: aside-tag "Related but separate"}} `<aside>` says "this is related but not essential." Sidebars, pull quotes, related links — content that supports but isn't the main point.

{{focus: footer-tag}} {{pulse: footer-tag}} {{annotate: footer-tag "Page footer"}} `<footer>` says "this is the bottom." Copyright, contact info, secondary navigation.

{{focus: none}} {{unsplit}} {{clear: slide}}

```code:semantic-code lang=html
<header>
  <h1>My Site</h1>
  <nav>
    <a href="/">Home</a>
    <a href="/about">About</a>
    <a href="/blog">Blog</a>
  </nav>
</header>
<main>
  <article>
    <h2>How CSS Works</h2>
    <p>The cascade is...</p>
  </article>
  <aside>
    <h3>Recent Posts</h3>
  </aside>
</main>
<footer>
  <p>2024 My Site</p>
</footer>
---
header-tag: 1-7
nav-tag: 3-7
main-tag: 9-16
article-tag: 10-13
aside-tag: 14-16
footer-tag: 18-20
```

```diagram:semantic-tree
page [service]
header [service]
nav [service]
main [service]
article [service]
aside [service]
footer [service]
page --> header
page --> main
page --> footer
header --> nav
main --> article
main --> aside
---
header-tag: header
nav-tag: nav
main-tag: main
article-tag: article
aside-tag: aside
footer-tag: footer
```

# Section vs article vs div

{{show: section-article typewriter 1.5s linear}} Three tags that group content. Each means something different.

{{focus: article-block}} {{zoom: 1.2x}} {{pulse: article-block}} {{annotate: article-block "Standalone"}} `<article>` is self-contained content. A blog post, a comment, a product card. The test: would this make sense in an RSS feed? If yes, it's an article.

{{zoom: 1x}} {{focus: section-block}} {{zoom: 1.2x}} {{pulse: section-block}} {{annotate: section-block "Thematic group"}} `<section>` groups related content with a heading. "About Us," "Features," "Pricing" — each is a section. The rule: every section should have a heading.

{{zoom: 1x}} {{focus: div-block}} {{zoom: 1.2x}} {{annotate: div-block "No meaning"}} `<div>` is the fallback. When nothing semantic fits, use a div. It means nothing, and that's the point — it's pure structure without semantics.

{{zoom: 1x}} {{focus: none}} The decision tree is simple. Is it self-contained? Article. Is it a thematic group with a heading? Section. Neither? Div.

```code:section-article lang=html
<!-- article: self-contained, could stand alone -->
<article>
  <h2>Understanding the Cascade</h2>
  <p>CSS rules compete for control...</p>
  <footer>Published Jan 2024</footer>
</article>

<!-- section: thematic group with heading -->
<section>
  <h2>Our Services</h2>
  <p>We offer three tiers...</p>
</section>

<!-- div: no semantic meaning, just a wrapper -->
<div class="card-grid">
  <div class="card">...</div>
  <div class="card">...</div>
</div>
---
article-block: 2-6
section-block: 9-12
div-block: 15-18
```

# The outline

{{clear: slide}}

{{show: outline-demo typewriter 1.5s linear}} Semantic tags create an implicit outline. This outline is what screen readers, search engines, and browser accessibility tools use to understand your page.

{{focus: page-level}} {{zoom: 1.2x}} {{pulse: page-level}} {{annotate: page-level "Level 1"}} The h1 is the page title. One per page. It tells search engines and assistive tech what this page is about.

{{zoom: 1x}} {{pan: section-levels}} {{focus: section-levels}} {{zoom: 1.2x}} {{annotate: section-levels "Level 2"}} h2 tags are section headings. They form the main branches of the outline tree.

{{zoom: 1x}} {{pan: sub-levels}} {{focus: sub-levels}} {{zoom: 1.2x}} {{annotate: sub-levels "Level 3"}} h3 tags are sub-sections within an h2. The hierarchy is strict — never skip from h2 to h4.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Read the headings alone, top to bottom. If they tell a coherent story — "Brewing Guide, Equipment, Beans, Grinder, Techniques, Pour Over, French Press" — the outline is good. If they don't make sense without the body text, restructure.

```code:outline-demo lang=html
<h1>Coffee Brewing Guide</h1>

<section>
  <h2>Equipment</h2>

  <section>
    <h3>Beans</h3>
    <p>Start with freshly roasted...</p>
  </section>

  <section>
    <h3>Grinder</h3>
    <p>Burr grinders produce consistent...</p>
  </section>
</section>

<section>
  <h2>Techniques</h2>

  <section>
    <h3>Pour Over</h3>
    <p>Heat water to 96 degrees...</p>
  </section>

  <section>
    <h3>French Press</h3>
    <p>Coarse grounds, four minutes...</p>
  </section>
</section>
---
page-level: 1
section-levels: 4, 18
sub-levels: 7, 12, 21, 26
```

# Modern HTML elements

{{clear: slide}}

{{show: modern-els typewriter 2s linear}} HTML keeps evolving. These elements were added in recent years and they solve real problems.

{{focus: dialog-el}} {{zoom: 1.3x}} {{pulse: dialog-el}} {{annotate: dialog-el "Built-in modal"}} `<dialog>` is a native modal. No JavaScript library needed for the overlay, the backdrop, the focus trap, or the escape-to-close behavior. The browser handles all of it. Call `.showModal()` in JavaScript and it just works.

{{zoom: 1x}} {{pan: details-el}} {{focus: details-el}} {{zoom: 1.2x}} {{pulse: details-el}} {{annotate: details-el "Toggle disclosure"}} `<details>` and `<summary>` create an expandable section. Click the summary, the content toggles. No JavaScript. Accessible by default. Keyboard operable.

{{zoom: 1x}} {{pan: search-el}} {{focus: search-el}} {{zoom: 1.2x}} {{pulse: search-el}} {{annotate: search-el "Search landmark"}} `<search>` marks a search section. It's like `<nav>` but for search functionality. Screen readers announce it as a search landmark.

{{zoom: 1x}} {{pan: none}} {{focus: none}} These elements work in every modern browser. They replace hundreds of lines of JavaScript with a single tag. When a native element exists for what you're building, use it.

```code:modern-els lang=html
<!-- dialog: native modal -->
<dialog id="confirm">
  <h2>Are you sure?</h2>
  <p>This action cannot be undone.</p>
  <button onclick="this.closest('dialog').close()">
    Cancel
  </button>
  <button onclick="deleteItem()">Delete</button>
</dialog>

<!-- details/summary: toggle disclosure -->
<details>
  <summary>Brewing instructions</summary>
  <ol>
    <li>Heat water to 96 degrees</li>
    <li>Grind beans medium-coarse</li>
    <li>Pour and wait four minutes</li>
  </ol>
</details>

<!-- search: search landmark -->
<search>
  <form action="/search">
    <label for="q">Search articles</label>
    <input type="search" id="q" name="q">
    <button type="submit">Search</button>
  </form>
</search>
---
dialog-el: 2-9
details-el: 12-18
search-el: 21-27
```

# Why it matters

{{clear: slide}}

{{show: a11y-stats grow 0.5s spring}} One in four adults in the US has a disability. 8.1 million people have a vision impairment. They use screen readers that interpret your HTML structure.

{{focus: screen-reader}} {{pulse: screen-reader}} {{annotate: screen-reader "What they hear"}} A screen reader on a page with semantic HTML announces: "Banner. Navigation with 3 items. Main content. Article: How CSS Works. Aside. Footer." The user knows exactly where they are.

{{focus: no-semantics}} {{annotate: no-semantics "What they hear"}} A screen reader on div soup announces: "Group. Group. Link. Link. Link. Group. Group. Group." The user is lost.

{{focus: none}} Semantic HTML isn't extra work for accessibility. It's the baseline. Anything less is a broken page.

```data:a11y-stats type=array
["Semantic HTML", "Div Soup"]
---
screen-reader: 0
no-semantics: 1
```
