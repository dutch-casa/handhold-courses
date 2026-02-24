---
title: What is the web?
---

# You type a URL

{{show: browser-bar typewriter 1s linear}} You open a browser. You type an address. You hit enter. Something happens.

{{focus: url-parts}} {{zoom: 1.2x}} {{pulse: url-parts}} That address has three parts. {{annotate: url-parts "Protocol + domain + path"}} The protocol says how to talk. The domain says who to talk to. The path says what to ask for.

{{zoom: 1x}} {{focus: none}} But what actually happens between pressing enter and seeing a page?

```code:browser-bar lang=text
https://example.com/about

 https  ://  example.com  /about
  │              │          │
protocol       domain      path
---
url-parts: 1, 3-6
```

# The request

{{clear: slide}}

{{show: request-flow grow 0.5s spring}} Your browser sends a message across the internet. That message is called a request.

{{focus: browser-node}} {{pulse: browser-node}} {{annotate: browser-node "You"}} It starts here. Your browser, on your machine.

{{focus: server-node}} {{pulse: server-node}} {{annotate: server-node "Them"}} It arrives here. A server, somewhere in the world. Just another computer, but one that's always listening.

{{trace: request-path}} {{annotate: request-path "Request"}} The browser sends a request. A small message: "Give me the page at /about."

{{trace: response-path}} {{annotate: response-path "Response"}} The server reads the request, finds the right file, and sends it back. That's the response. A text file, traveling back to you.

{{flow: full-trip}} The whole trip takes milliseconds. Request out, response back. That's the web. Two computers passing text files.

{{focus: none}} {{flow: none}} {{trace: none}} Every website you've ever visited works this way. Every single one.

```diagram:request-flow
browser [client]
server [service]
browser --request--> server
server --response--> browser
---
browser-node: browser
server-node: server
request-path: browser
response-path: server
full-trip: browser, server
```

# What comes back

{{clear: slide}}

{{show: raw-html typewriter 2s linear}} The response is a text file. Not an image. Not a program. A text file with a specific format.

{{focus: doctype}} {{zoom: 1.3x}} {{pulse: doctype}} {{annotate: doctype "Declaration"}} The first line tells the browser: this is HTML. It's not optional. It's been the same since 2014.

{{zoom: 1x}} {{focus: head-section}} {{zoom: 1.2x}} {{pan: head-section}} The head section is invisible. {{annotate: head-section "Metadata"}} It holds information about the page — the title in the browser tab, character encoding, links to stylesheets. The user never sees it.

{{zoom: 1x}} {{pan: body-section}} {{focus: body-section}} {{zoom: 1.2x}} {{annotate: body-section "The visible page"}} The body is what you see. Everything the user interacts with — text, images, buttons, forms — lives here.

{{zoom: 1x}} {{pan: none}} {{focus: none}} This is the entire skeleton of every webpage. A doctype, a head, a body. That's the structure.

```code:raw-html lang=html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>About Us</title>
</head>
<body>
  <h1>About Us</h1>
  <p>We build things for the web.</p>
</body>
</html>
---
doctype: 1
head-section: 3-6
body-section: 8-11
```

# The browser reads it

{{clear: slide}}

{{show: parse-pipeline grow 0.5s spring}} The browser doesn't display the text file directly. It reads it, interprets it, and builds something from it.

{{focus: parse-step}} {{pulse: parse-step}} {{annotate: parse-step "Step 1"}} First, the parser reads the HTML top to bottom. Every tag it encounters becomes a node.

{{trace: build-path}} {{focus: tree-step}} {{pulse: tree-step}} {{annotate: tree-step "Step 2"}} Those nodes form a tree. Parent tags contain child tags. This tree is the DOM — the Document Object Model. We'll spend an entire lesson on it later.

{{trace: render-path}} {{focus: render-step}} {{pulse: render-step}} {{annotate: render-step "Step 3"}} The browser walks the tree, calculates sizes and positions, and paints pixels to the screen.

{{flow: full-pipeline}} {{focus: none}} Three steps. Parse the text. Build the tree. Paint the pixels. That's what your browser does every time you load a page.

{{flow: none}} {{trace: none}}

```diagram:parse-pipeline
html-text [service]
parser [service]
dom-tree [service]
renderer [service]
pixels [service]
html-text --reads--> parser
parser --builds--> dom-tree
dom-tree --calculates--> renderer
renderer --paints--> pixels
{Pipeline: html-text, parser, dom-tree, renderer, pixels}
---
parse-step: parser
tree-step: dom-tree
render-step: renderer
build-path: html-text, parser, dom-tree
render-path: dom-tree, renderer, pixels
full-pipeline: html-text, parser, dom-tree, renderer, pixels
```

# Tags are instructions

{{clear: slide}}

{{show: tag-anatomy typewriter 1.5s linear}} HTML tags are instructions to the browser. Each one says: make a thing.

{{focus: opening}} {{zoom: 1.2x}} {{pulse: opening}} {{annotate: opening "Start here"}} The opening tag starts the element. The angle brackets and the tag name — that's it.

{{zoom: 1x}} {{focus: content}} {{pulse: content}} {{annotate: content "What's inside"}} Between the opening and closing tag is the content. Text, other elements, whatever the tag contains.

{{zoom: 1x}} {{focus: closing}} {{pulse: closing}} {{annotate: closing "End here"}} The closing tag ends the element. Same name, but with a forward slash.

{{zoom: 1x}} {{focus: attribute}} {{zoom: 1.2x}} {{pulse: attribute}} {{annotate: attribute "Configuration"}} Attributes go inside the opening tag. They configure the element. This href attribute tells the link where to go.

{{zoom: 1x}} {{focus: none}} Open, content, close. That's the anatomy of every HTML element. Some tags are self-closing — they have no content, so they don't need a closing tag. An image, for example, is just `<img>`. No closing tag, because there's nothing "inside" an image.

```code:tag-anatomy lang=html
<a href="https://example.com">Visit our site</a>

Opening tag:    <a href="https://example.com">
Content:        Visit our site
Closing tag:    </a>
Attribute:      href="https://example.com"
---
opening: 3
content: 4
closing: 5
attribute: 6
```

# Nesting builds structure

{{clear: slide}}

{{show: nesting-demo typewriter 1.5s linear}} Tags go inside other tags. This is nesting. It creates a hierarchy — a tree.

{{focus: outer}} {{zoom: 1.2x}} {{pulse: outer}} {{annotate: outer "Parent"}} The `<nav>` is the parent. It contains everything inside it.

{{zoom: 1x}} {{pan: children}} {{focus: children}} {{zoom: 1.2x}} {{pulse: children}} {{annotate: children "Children"}} The `<a>` tags are children of the nav. They're nested one level deep.

{{zoom: 1x}} {{pan: none}} {{focus: none}} This nesting is not decoration. It's how the browser knows that these links belong to the navigation. It's how screen readers announce "navigation with three links." Structure is meaning.

```code:nesting-demo lang=html
<nav>
  <a href="/">Home</a>
  <a href="/about">About</a>
  <a href="/contact">Contact</a>
</nav>
---
outer: 1, 5
children: 2-4
```

{{clear: slide}}

{{split}} {{show: nesting-code reveal 0.3s}} {{show: nesting-tree reveal 0.5s spring}} On the left, the HTML. On the right, the tree the browser builds from it.

{{focus: nav-node}} {{pulse: nav-node}} Every tag becomes a node. {{annotate: nav-node "Root of this subtree"}} The nav tag becomes the parent node.

{{focus: link-nodes}} {{pulse: link-nodes}} {{annotate: link-nodes "Leaf nodes"}} The links become child nodes. They're leaves — nothing nested inside them except text.

{{focus: none}} This tree structure is everything. CSS uses it to style elements. JavaScript uses it to find and change elements. Screen readers use it to announce page structure. The tree is the foundation.

{{unsplit}} {{clear: slide}}

```code:nesting-code lang=html
<nav>
  <a href="/">Home</a>
  <a href="/about">About</a>
  <a href="/contact">Contact</a>
</nav>
---
nav-node: 1, 5
link-nodes: 2-4
```

```data:nesting-tree type=binary-tree
[nav, a:Home, a:About, a:Contact]
---
nav-node: 0
link-nodes: 1, 2, 3
```

# Your first page

{{show: first-page typewriter 2s linear}} Here's a complete page. Twelve lines. Every line earns its place.

{{pan: doctype-line}} {{focus: doctype-line}} {{zoom: 1.3x}} {{pulse: doctype-line}} The doctype. One line. Same every time.

{{zoom: 1x}} {{pan: meta-lines}} {{focus: meta-lines}} {{zoom: 1.2x}} {{annotate: meta-lines "Setup"}} Two meta lines. Character encoding and viewport scaling. You'll include these in every page you write.

{{zoom: 1x}} {{pan: title-line}} {{focus: title-line}} {{pulse: title-line}} The title appears in the browser tab. Not on the page itself.

{{zoom: 1x}} {{pan: visible}} {{focus: visible}} {{zoom: 1.2x}} {{annotate: visible "What users see"}} The body. A heading, a paragraph, a link. Three elements that produce a readable page.

{{zoom: 1x}} {{pan: none}} {{focus: none}} That's it. Twelve lines from blank file to working webpage. The browser handles the rest — parsing, layout, painting. You write the blueprint. It builds the building.

```code:first-page lang=html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width">
  <title>My First Page</title>
</head>
<body>
  <h1>Hello, World</h1>
  <p>This is my first webpage.</p>
  <a href="https://example.com">Learn more</a>
</body>
</html>
---
doctype-line: 1
meta-lines: 4-5
title-line: 6
visible: 9-11
```
