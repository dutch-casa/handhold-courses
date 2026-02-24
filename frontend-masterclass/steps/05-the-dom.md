---
title: The DOM
---

# Blueprint vs building

{{show: blueprint-diagram grow 0.5s spring}} You write HTML in a text file. The browser reads that file and builds something from it. Those two things are not the same.

{{focus: html-file}} {{pulse: html-file}} {{annotate: html-file "The blueprint"}} The HTML file is the blueprint. It's text. Characters in a file. It never changes unless you edit the file.

{{trace: build-path}} {{focus: dom-node}} {{pulse: dom-node}} {{annotate: dom-node "The building"}} The DOM is the building. It's a live data structure in the browser's memory. It can change — JavaScript can add rooms, remove walls, repaint everything.

{{focus: none}} {{trace: none}} When you change the DOM, you change the building. The blueprint stays the same. Reload the page, and the browser rebuilds the building from the blueprint again.

```diagram:blueprint-diagram
html-file [service]
parser [service]
dom-node [service]
screen [client]
html-file --parses--> parser
parser --builds--> dom-node
dom-node --renders--> screen
---
html-file: html-file
dom-node: dom-node
build-path: html-file, parser, dom-node
```

# The tree

{{clear: slide}}

{{split}} {{show: html-source reveal 0.3s}} {{show: dom-tree reveal 0.5s spring}} The DOM is a tree. Every element becomes a node. Nesting becomes parent-child relationships.

{{focus: document-root}} {{pulse: document-root}} {{annotate: document-root "Root"}} The root is the document itself. Everything hangs from it. `document` in JavaScript is your handle to this root.

{{focus: html-node}} {{pulse: html-node}} {{annotate: html-node "Root element"}} The `<html>` element is the root element. It has two children: `<head>` and `<body>`.

{{focus: body-children}} {{pulse: body-children}} {{annotate: body-children "What you see"}} The body's children are the visible elements. The heading, the list, the list items. Every tag in your HTML becomes a node in this tree.

{{focus: text-nodes}} {{annotate: text-nodes "Text is a node too"}} Even text is a node. "Hello" inside the h1 is a text node, a child of the h1 element node. The DOM tree is thorough — it accounts for everything.

{{focus: none}} {{unsplit}}

```code:html-source lang=html
<!DOCTYPE html>
<html>
<head>
  <title>My Page</title>
</head>
<body>
  <h1>Hello</h1>
  <ul>
    <li>First</li>
    <li>Second</li>
    <li>Third</li>
  </ul>
</body>
</html>
---
document-root: 1
html-node: 2
body-children: 7-12
text-nodes: 7, 9-11
```

```data:dom-tree type=graph layout=tree
document -> html
html -> head, body
head -> title
body -> h1, ul
ul -> li1, li2, li3
---
document-root: document
html-node: html
body-children: body, h1, ul
text-nodes: h1, li1, li2, li3
```

# Finding elements

{{clear: slide}}

{{show: query-methods typewriter 1.5s linear}} JavaScript talks to the DOM through the `document` object. You find elements, then you do things to them.

{{focus: query-single}} {{zoom: 1.3x}} {{pulse: query-single}} {{annotate: query-single "One element"}} `querySelector` finds the first element matching a CSS selector. Pass it any selector you'd use in a stylesheet — a class, an ID, a tag name, a complex selector. It returns one element or null.

{{zoom: 1x}} {{pan: query-all}} {{focus: query-all}} {{zoom: 1.2x}} {{pulse: query-all}} {{annotate: query-all "All matches"}} `querySelectorAll` finds every matching element. It returns a NodeList — like an array. You loop over it to work with each element.

{{zoom: 1x}} {{pan: by-id}} {{focus: by-id}} {{zoom: 1.2x}} {{annotate: by-id "Direct ID lookup"}} `getElementById` is the fast path when you know the ID. No hash needed — just the ID string. Returns the element or null.

{{zoom: 1x}} {{pan: none}} {{focus: none}} `querySelector` and `querySelectorAll` are the ones you'll use 90% of the time. They accept any CSS selector, so you already know the query language.

```code:query-methods lang=javascript
// querySelector: first match, returns one element
const heading = document.querySelector("h1");
const firstItem = document.querySelector(".item");
const nav = document.querySelector("nav > ul");

// querySelectorAll: all matches, returns NodeList
const items = document.querySelectorAll(".item");
const links = document.querySelectorAll("a[href]");

// getElementById: direct lookup by ID
const form = document.getElementById("login-form");
---
query-single: 2-4
query-all: 7-8
by-id: 11
```

# Reading and writing

{{clear: slide}}

{{show: dom-props typewriter 2s linear}} Once you have an element, you can read its content and change it.

{{pan: text-content}} {{focus: text-content}} {{zoom: 1.2x}} {{pulse: text-content}} {{annotate: text-content "Text access"}} `textContent` gets or sets the text inside an element. Reading it returns the raw text. Setting it replaces all content with plain text — no HTML parsing.

{{zoom: 1x}} {{pan: class-list}} {{focus: class-list}} {{zoom: 1.2x}} {{pulse: class-list}} {{annotate: class-list "CSS classes"}} `classList` manages CSS classes. `add` puts a class on. `remove` takes it off. `toggle` flips it. `contains` checks if it's there. These are the tools you use to change how an element looks.

{{zoom: 1x}} {{pan: attributes}} {{focus: attributes}} {{zoom: 1.2x}} {{annotate: attributes "Any attribute"}} `getAttribute` and `setAttribute` work with any HTML attribute. Read the href of a link. Change the src of an image. Set a custom data attribute.

{{zoom: 1x}} {{pan: dataset}} {{focus: dataset}} {{zoom: 1.2x}} {{pulse: dataset}} {{annotate: dataset "Custom data"}} `dataset` reads `data-*` attributes as a clean JavaScript object. `data-user-id="42"` becomes `element.dataset.userId`. Hyphens become camelCase automatically.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:dom-props lang=javascript
const el = document.querySelector(".card");

// textContent: read and write text
console.log(el.textContent);
el.textContent = "New text";

// classList: manage CSS classes
el.classList.add("active");
el.classList.remove("hidden");
el.classList.toggle("selected");
el.classList.contains("active"); // true

// getAttribute / setAttribute: any attribute
const link = document.querySelector("a");
const href = link.getAttribute("href");
link.setAttribute("target", "_blank");

// dataset: data-* attributes
// <div data-user-id="42" data-role="admin">
const userId = el.dataset.userId;    // "42"
const role = el.dataset.role;        // "admin"
---
text-content: 3-5
class-list: 7-11
attributes: 13-16
dataset: 18-21
```

# Creating and removing

{{clear: slide}}

{{show: dom-mutation typewriter 1.5s linear}} You can build new elements from scratch and add them to the page. You can remove elements that are already there.

{{focus: create-step}} {{zoom: 1.2x}} {{pulse: create-step}} {{annotate: create-step "Build it"}} `createElement` makes a new element. It exists in memory but isn't on the page yet. You configure it — set text, add classes, set attributes — before adding it.

{{zoom: 1x}} {{pan: append-step}} {{focus: append-step}} {{zoom: 1.2x}} {{pulse: append-step}} {{annotate: append-step "Add it"}} `appendChild` puts it on the page. The element becomes a child of whatever you append it to. Now it's visible. Now it's in the DOM tree.

{{zoom: 1x}} {{pan: remove-step}} {{focus: remove-step}} {{zoom: 1.2x}} {{annotate: remove-step "Remove it"}} `remove` takes an element off the page. Gone from the DOM tree. Gone from the screen.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Creating, configuring, appending. That's how every dynamic UI works. Every todo item added, every notification shown, every search result rendered — it's this three-step pattern.

```code:dom-mutation lang=javascript
// Create a new element
const card = document.createElement("div");
card.classList.add("card");
card.textContent = "New card content";

// Add it to the page
const container = document.querySelector(".cards");
container.appendChild(card);

// Remove an element
const oldCard = document.querySelector(".card.old");
oldCard.remove();

// Insert before another element
const list = document.querySelector("ul");
const newItem = document.createElement("li");
newItem.textContent = "Inserted item";
const secondItem = list.children[1];
list.insertBefore(newItem, secondItem);
---
create-step: 2-4
append-step: 7-8
remove-step: 11-12
```

# Events

{{clear: slide}}

{{show: event-flow grow 0.5s spring}} Something happens — a click, a keypress, a scroll. The browser creates an event object and walks the DOM tree looking for handlers.

{{focus: capture-phase}} {{pulse: capture-phase}} {{annotate: capture-phase "Phase 1: Capture"}} The event starts at the root and walks down toward the target element. This is the capture phase. Most of the time, you ignore it.

{{trace: target-walk}} {{focus: target-phase}} {{pulse: target-phase}} {{annotate: target-phase "Phase 2: Target"}} The event reaches the element that was clicked. Your event handler fires here.

{{trace: bubble-walk}} {{focus: bubble-phase}} {{pulse: bubble-phase}} {{annotate: bubble-phase "Phase 3: Bubble"}} The event walks back up the tree toward the root. This is bubbling. A click on a button also triggers click handlers on the button's parent, grandparent, all the way up.

{{flow: full-event}} {{focus: none}} {{trace: none}} This bubbling behavior is what makes event delegation possible. Instead of adding a click handler to every list item, add one handler to the list. When any item is clicked, the event bubbles up to the list handler.

{{flow: none}}

```diagram:event-flow
root [service]
body [service]
list [service]
item [service]
root --> body
body --> list
list --> item
---
capture-phase: root, body, list
target-phase: item
bubble-phase: item, list, body, root
target-walk: root, body, list, item
bubble-walk: item, list, body, root
full-event: root, body, list, item
```

# addEventListener

{{clear: slide}}

{{show: listener-code typewriter 2s linear}} `addEventListener` is how you connect code to events. The browser calls your function whenever that event happens on that element.

{{focus: basic-handler}} {{zoom: 1.2x}} {{pulse: basic-handler}} {{annotate: basic-handler "Click handler"}} First argument: the event type. Second argument: your callback function. When the button is clicked, the function runs.

{{zoom: 1x}} {{pan: event-object}} {{focus: event-object}} {{zoom: 1.2x}} {{annotate: event-object "Event details"}} The callback receives an event object. It has details about what happened — which element was clicked, the mouse position, which key was pressed, whether modifier keys were held.

{{zoom: 1x}} {{pan: delegation}} {{focus: delegation}} {{zoom: 1.2x}} {{pulse: delegation}} {{annotate: delegation "Event delegation"}} Event delegation in action. One handler on the list. When any item inside is clicked, the event bubbles up. `event.target` tells you which specific element was clicked. `closest` walks up the tree to find the matching ancestor.

{{zoom: 1x}} {{pan: none}} {{focus: none}} This is the bridge between HTML and interactivity. Without addEventListener, pages are static documents. With it, they respond to every user action.

```code:listener-code lang=javascript
// Basic: click handler on a button
const btn = document.querySelector("#submit");
btn.addEventListener("click", () => {
  console.log("Button clicked!");
});

// With event object: keyboard handler
document.addEventListener("keydown", (event) => {
  if (event.key === "Escape") {
    closeModal();
  }
});

// Event delegation: one handler, many items
const list = document.querySelector("ul");
list.addEventListener("click", (event) => {
  const item = event.target.closest("li");
  if (item) {
    item.classList.toggle("completed");
  }
});
---
basic-handler: 2-5
event-object: 8-12
delegation: 15-21
```

# DevTools

{{clear: slide}}

{{show: devtools-tip typewriter 1.5s linear}} Every browser has developer tools built in. Open them with F12 or Cmd+Option+I on Mac. This is where you see the live DOM.

{{focus: elements-panel}} {{zoom: 1.2x}} {{pulse: elements-panel}} {{annotate: elements-panel "Elements panel"}} The Elements panel shows the DOM tree in real time. Click any element to inspect it. Change text, add attributes, toggle classes — your changes appear on the page instantly. Refresh to reset.

{{zoom: 1x}} {{pan: console-panel}} {{focus: console-panel}} {{zoom: 1.2x}} {{pulse: console-panel}} {{annotate: console-panel "Console"}} The Console is where you run JavaScript live. Type `document.querySelector("h1")` and see the element. Type `document.title = "New Title"` and watch the browser tab change. This is your testing ground.

{{zoom: 1x}} {{pan: inspect-tool}} {{focus: inspect-tool}} {{zoom: 1.2x}} {{annotate: inspect-tool "Point and click"}} The inspect tool lets you hover over any element on the page and click to select it in the Elements panel. It highlights the box model — content, padding, border, margin — in real time.

{{zoom: 1x}} {{pan: none}} {{focus: none}} DevTools is your X-ray vision. Use it constantly. Inspect elements before writing CSS. Test queries before writing JavaScript. Debug events by watching them fire. It's the fastest way to understand what the browser is doing.

```code:devtools-tip lang=text
Open DevTools:
  macOS:    Cmd + Option + I
  Windows:  F12  or  Ctrl + Shift + I

Panels:
  Elements    — live DOM tree, edit HTML/CSS
  Console     — run JavaScript, see errors
  Network     — watch requests and responses
  Sources     — debug JavaScript with breakpoints
  Application — cookies, storage, service workers
---
elements-panel: 6
console-panel: 7
inspect-tool: 4-5
```
