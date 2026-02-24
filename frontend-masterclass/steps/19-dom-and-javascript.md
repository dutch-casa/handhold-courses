---
title: The DOM and JavaScript
---

# Finding elements

{{show: query-code typewriter 2s linear}} JavaScript interacts with the page through the DOM. The first step is always finding the element you want.

{{focus: query-one}} {{zoom: 1.2x}} {{pulse: query-one}} {{annotate: query-one "querySelector"}} `document.querySelector(".card")` finds the first element matching a CSS selector. Any valid CSS selector works: `.card`, `#nav`, `[data-active]`, `.sidebar > ul > li:first-child`. Returns the element or `null` if nothing matches.

{{zoom: 1x}} {{focus: query-all}} {{zoom: 1.2x}} {{annotate: query-all "querySelectorAll"}} `document.querySelectorAll("p")` finds all matching elements. Returns a NodeList — array-like but not an array. Convert it: `[...document.querySelectorAll("p")]` or use `.forEach()` directly. Each element in the list is a full DOM node.

{{zoom: 1x}} {{focus: by-id}} {{zoom: 1.2x}} {{pulse: by-id}} {{annotate: by-id "getElementById"}} `document.getElementById("nav")` is the fast path for IDs. No hash prefix. Returns the element or `null`. For everything else, use `querySelector`.

{{zoom: 1x}} {{focus: none}} These are your doorway into the DOM. Find an element, then read it, change it, or listen to it.

```code:query-code lang=javascript
// querySelector: first match (any CSS selector)
const card = document.querySelector(".card");
const nav = document.querySelector("#main-nav");
const active = document.querySelector("[data-active]");

// querySelectorAll: all matches
const paragraphs = document.querySelectorAll("p");
const items = document.querySelectorAll(".list-item");

// Convert NodeList to array for full array methods
const itemArray = [...document.querySelectorAll(".item")];
const names = itemArray.map((el) => el.textContent);

// getElementById: fast path for IDs
const header = document.getElementById("header");
---
query-one: 2-4
query-all: 7-8
by-id: 14
```

# Reading and writing

{{clear: slide}}

{{show: readwrite-code typewriter 2s linear}} Once you have an element, read its content, change its styles, and toggle its classes.

{{focus: text}} {{zoom: 1.2x}} {{pulse: text}} {{annotate: text "Text content"}} `.textContent` reads or sets the text inside an element. `.innerHTML` reads or sets the HTML. Use `textContent` for plain text — it's faster and safer (no XSS risk). Use `innerHTML` only when you need to insert HTML structure.

{{zoom: 1x}} {{focus: classes}} {{zoom: 1.2x}} {{pulse: classes}} {{annotate: classes "Class list"}} `.classList` is the interface for CSS classes. `.add("active")`, `.remove("active")`, `.toggle("active")`, `.contains("active")`. These replace the old `className` string manipulation. Toggle is especially useful — add if missing, remove if present.

{{zoom: 1x}} {{focus: style}} {{zoom: 1.2x}} {{annotate: style "Inline styles"}} `.style.color = "blue"` sets an inline style. CSS property names use camelCase: `backgroundColor`, `fontSize`, `marginTop`. Prefer class toggling over inline styles — it separates concerns and uses your existing CSS.

{{zoom: 1x}} {{focus: dataset}} {{zoom: 1.2x}} {{pulse: dataset}} {{annotate: dataset "Data attributes"}} `.dataset` reads and writes `data-*` attributes. `data-id="42"` becomes `element.dataset.id`. Store metadata on elements — IDs, states, configuration — without polluting class names.

{{zoom: 1x}} {{focus: none}}

```code:readwrite-code lang=javascript
const card = document.querySelector(".card");

// Text content (safe, no HTML parsing)
card.textContent = "Updated text";
const text = card.textContent;

// innerHTML (parses HTML — use carefully)
card.innerHTML = "<strong>Bold</strong> text";

// Class list: add, remove, toggle
card.classList.add("active");
card.classList.remove("hidden");
card.classList.toggle("expanded");
const isActive = card.classList.contains("active");

// Inline styles (prefer class toggling)
card.style.backgroundColor = "#eff6ff";
card.style.borderRadius = "8px";

// Data attributes
// <div class="card" data-id="42" data-status="open">
card.dataset.id;     // "42"
card.dataset.status;  // "open"
card.dataset.status = "closed"; // updates the attribute
---
text: 4-5
classes: 11-14
style: 17-18
dataset: 21-24
```

# Creating and removing

{{clear: slide}}

{{split}} {{show: create-code typewriter 2s linear}} {{show: create-preview reveal 0.5s spring}} JavaScript can build elements from scratch and insert them into the page.

{{focus: create}} {{zoom: 1.2x}} {{pulse: create}} {{annotate: create "createElement"}} `document.createElement("div")` creates a new element in memory. It doesn't appear on the page yet — you need to attach it. Set its content, classes, and attributes before inserting.

{{zoom: 1x}} {{focus: append}} {{zoom: 1.2x}} {{pulse: append}} {{annotate: append "Inserting"}} `.appendChild(element)` adds a child at the end. `.prepend(element)` at the beginning. `.before(element)` and `.after(element)` place siblings. `.insertAdjacentHTML("beforeend", html)` is the fast path for HTML strings.

{{zoom: 1x}} {{focus: remove}} {{zoom: 1.2x}} {{annotate: remove "Removing"}} `.remove()` takes an element out of the DOM. `element.remove()`. Gone. The object still exists in memory if you keep a reference — you can re-insert it later.

{{zoom: 1x}} {{focus: fragment}} {{zoom: 1.2x}} {{pulse: fragment}} {{annotate: fragment "Document fragment"}} `document.createDocumentFragment()` is a lightweight container. Build multiple elements into a fragment, then insert the fragment once. One DOM write instead of many. The browser reflows once, not per element.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:create-code lang=javascript
// Create an element
const card = document.createElement("div");
card.className = "card";
card.textContent = "New card";

// Insert into the page
const container = document.querySelector(".cards");
container.appendChild(card);     // at the end
container.prepend(card);         // at the beginning
card.after(anotherElement);      // as a sibling

// Remove from the page
card.remove();

// Document fragment: batch insert
const fragment = document.createDocumentFragment();
for (const name of ["Alice", "Bob", "Carol"]) {
  const li = document.createElement("li");
  li.textContent = name;
  fragment.appendChild(li);
}
document.querySelector("ul").appendChild(fragment);
---
create: 2-4
append: 7-10
remove: 13
fragment: 16-22
```

```preview:create-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 4px; margin-top: 10px; }
  .demo-list { list-style: none; padding: 0; }
  .demo-list li { padding: 6px 10px; background: #eff6ff; border: 1px solid #bfdbfe; margin-bottom: 4px; border-radius: 4px; font-size: 12px; }
  .demo-list li.new { background: #f0fdf4; border-color: #86efac; }
</style>
<div class="label">Original list:</div>
<ul class="demo-list">
  <li>Alice</li>
  <li>Bob</li>
</ul>
<div class="label">After appendChild("Carol"):</div>
<ul class="demo-list">
  <li>Alice</li>
  <li>Bob</li>
  <li class="new">Carol (new)</li>
</ul>
```

# Events

{{clear: slide}}

{{show: event-diagram reveal 0.3s spring}} {{show: event-code typewriter 2s linear}} Events are the bridge between the user and your code. When someone clicks, types, or scrolls, the browser fires an event.

{{focus: add-listener}} {{zoom: 1.2x}} {{pulse: add-listener}} {{annotate: add-listener "addEventListener"}} `element.addEventListener("click", handler)` registers a function to run when the event fires. The event name is a string. The handler receives an event object with details about what happened.

{{zoom: 1x}} {{focus: event-obj}} {{zoom: 1.2x}} {{annotate: event-obj "Event object"}} The event object has `target` (the element clicked), `type` (the event name), `preventDefault()` (stop default behavior), `stopPropagation()` (stop bubbling). `event.target` is the most used property — it tells you what was actually clicked.

{{zoom: 1x}} {{focus: delegation}} {{zoom: 1.2x}} {{pulse: delegation}} {{annotate: delegation "Event delegation"}} Instead of adding a listener to every list item, add one to the parent. Check `event.target` to find which child was clicked. This is event delegation — one listener handles all children, including ones added dynamically. Less memory, works with future elements.

{{zoom: 1x}} {{focus: bubbling}} {{zoom: 1.2x}} {{annotate: bubbling "Bubbling"}} When you click a button inside a card inside the page, the click event fires on the button, then the card, then the page. Events bubble up the DOM tree. This is why delegation works — the parent catches events from its children.

{{zoom: 1x}} {{focus: none}}

```code:event-code lang=javascript
// Basic event listener
const button = document.querySelector(".btn");
button.addEventListener("click", (event) => {
  console.log("Clicked!", event.target);
});

// Event object: prevent default behavior
const form = document.querySelector("form");
form.addEventListener("submit", (event) => {
  event.preventDefault(); // stop page reload
  const data = new FormData(form);
  console.log(data.get("name"));
});

// Event delegation: one listener on parent
const list = document.querySelector(".todo-list");
list.addEventListener("click", (event) => {
  if (event.target.matches(".delete-btn")) {
    event.target.closest("li").remove();
  }
  if (event.target.matches(".toggle")) {
    event.target.closest("li").classList.toggle("done");
  }
});
---
add-listener: 2-5
event-obj: 8-13
delegation: 16-24
bubbling: 16
```

```diagram:event-diagram kind=graph
Click [label="Click event" shape=box]
Capture [label="Capture phase ↓"]
Target [label="Target phase"]
Bubble [label="Bubble phase ↑"]
Doc [label="document"]
Body [label="body"]
Card [label=".card"]
Btn [label="button (target)" shape=box]

Doc -> Body [label="capture"]
Body -> Card [label="capture"]
Card -> Btn [label="capture"]
Btn -> Card [label="bubble"]
Card -> Body [label="bubble"]
Body -> Doc [label="bubble"]
```

# The event loop

{{clear: slide}}

{{show: loop-code typewriter 2s linear}} JavaScript is single-threaded. It runs one thing at a time. The event loop is how it handles asynchronous work without blocking.

{{focus: sync}} {{zoom: 1.2x}} {{pulse: sync}} {{annotate: sync "Synchronous"}} Code runs top to bottom, one line at a time. `console.log(1); console.log(2); console.log(3);` — always 1, 2, 3. If a function takes 5 seconds, everything waits.

{{zoom: 1x}} {{focus: timeout}} {{zoom: 1.2x}} {{annotate: timeout "Scheduled work"}} `setTimeout(callback, 1000)` schedules a function to run after 1000 milliseconds. The callback goes into a queue. The event loop checks the queue after the current code finishes. `setTimeout(fn, 0)` doesn't run immediately — it runs after the current synchronous code completes.

{{zoom: 1x}} {{focus: microtask}} {{zoom: 1.2x}} {{pulse: microtask}} {{annotate: microtask "Microtasks"}} Promises create microtasks. Microtasks run before the next macrotask (like setTimeout). `Promise.resolve().then(() => console.log("micro"))` runs before `setTimeout(() => console.log("macro"), 0)`. The priority: synchronous code first, then microtasks, then macrotasks.

{{zoom: 1x}} {{focus: none}} Understanding the event loop prevents a class of confusing bugs. When code runs "later," it's the event loop scheduling it.

```code:loop-code lang=javascript
// Synchronous: runs in order
console.log("1: sync");
console.log("2: sync");

// setTimeout: macrotask queue
setTimeout(() => {
  console.log("4: timeout (macrotask)");
}, 0);

// Promise: microtask queue (runs before setTimeout)
Promise.resolve().then(() => {
  console.log("3: promise (microtask)");
});

// Output order:
// 1: sync
// 2: sync
// 3: promise (microtask)
// 4: timeout (macrotask)

// The event loop priority:
// 1. Synchronous code (call stack)
// 2. Microtasks (promises, queueMicrotask)
// 3. Macrotasks (setTimeout, setInterval, I/O)
---
sync: 2-3
timeout: 6-8
microtask: 11-13
```
