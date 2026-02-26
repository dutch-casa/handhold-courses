---
title: What is React?
---

# Imperative vs declarative

{{split}} {{show: imperative-code typewriter 2s linear}} {{show: declarative-code reveal 0.5s spring}} Building UI with vanilla JavaScript is imperative. You tell the browser what to do, step by step. React is declarative. You describe what the UI should look like.

{{focus: imperative}} {{zoom: 1.2x}} {{pulse: imperative}} {{annotate: imperative "Step by step"}} Create element. Set text. Add class. Append to parent. Find old element. Remove it. Find another element. Update its text. Every state change requires manual DOM surgery. Miss a step and the UI is wrong.

{{zoom: 1x}} {{focus: declarative}} {{zoom: 1.2x}} {{pulse: declarative}} {{annotate: declarative "Describe the result"}} Return JSX. React figures out the DOM operations. When state changes, return new JSX. React compares old and new, and applies the minimum changes. You describe the destination. React drives.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:imperative-code lang=javascript
// Imperative: tell the browser what to do
function updateCounter(count) {
  const el = document.getElementById("count");
  el.textContent = count;
  if (count > 10) {
    el.classList.add("high");
  } else {
    el.classList.remove("high");
  }
  const items = document.querySelectorAll(".item");
  items.forEach((item) => item.remove());
  for (const name of getNames()) {
    const li = document.createElement("li");
    li.textContent = name;
    li.className = "item";
    document.getElementById("list").appendChild(li);
  }
}
// Manual. Fragile. Scales poorly.
---
imperative: 2-18
```

```code:declarative-code lang=tsx
// Declarative: describe what the UI looks like
function Counter({ count, names }) {
  return (
    <div>
      <span className={count > 10 ? "high" : ""}>
        {count}
      </span>
      <ul>
        {names.map((name) => (
          <li key={name} className="item">
            {name}
          </li>
        ))}
      </ul>
    </div>
  );
}
// React handles the DOM updates.
---
declarative: 2-17
```

# Components are functions

{{clear: slide}}

{{show: component-code typewriter 2s linear}} A React component is a function. It takes props (input data) and returns JSX (a description of UI).

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "Function component"}} `function Greeting({ name })` — a function that takes an object with a `name` property. Returns JSX. That's a component. Call it in JSX like an HTML tag: `<Greeting name="Alice" />`.

{{zoom: 1x}} {{focus: jsx}} {{zoom: 1.2x}} {{pulse: jsx}} {{annotate: jsx "JSX is function calls"}} JSX isn't HTML. `<div className="card">` compiles to `React.createElement("div", { className: "card" })`. `<Greeting name="Alice" />` compiles to `React.createElement(Greeting, { name: "Alice" })`. JSX is syntactic sugar for function calls.

{{zoom: 1x}} {{focus: compose}} {{zoom: 1.2x}} {{annotate: compose "Composition"}} Components nest. `<App>` renders `<Header>` and `<Main>`. `<Main>` renders a list of `<Card>` components. Each component is independent — it only knows about its own props and state. This is the component tree.

{{zoom: 1x}} {{focus: none}}

```code:component-code lang=tsx
// A component is a function
function Greeting({ name }: { name: string }) {
  return <h1>Hello, {name}!</h1>;
}

// JSX compiles to function calls
// <Greeting name="Alice" />
// becomes:
// React.createElement(Greeting, { name: "Alice" })

// Composition: components nest
function App() {
  return (
    <div>
      <Header />
      <Main>
        <Card title="First" />
        <Card title="Second" />
      </Main>
    </div>
  );
}

// UI = f(state)
// The entire UI is a function of data.
// Change the data, the UI updates.
---
basic: 2-4
jsx: 7-9
compose: 12-21
```

# The mental model

{{clear: slide}}

{{show: mental-diagram reveal 0.3s spring}} {{show: mental-code typewriter 2s linear}} React's mental model is one equation: `UI = f(state)`. The UI is a function of state. When state changes, React calls your function again.

{{focus: render}} {{zoom: 1.2x}} {{pulse: render}} {{annotate: render "Render cycle"}} State changes. React calls your component function. The function returns new JSX. React compares old JSX to new JSX. Only the differences get applied to the DOM. This is called reconciliation.

{{zoom: 1x}} {{focus: snapshot}} {{zoom: 1.2x}} {{annotate: snapshot "Snapshots"}} Each render is a snapshot. The component sees the state as it was when that render started. Event handlers from that render close over that snapshot's values. This is why React is predictable — there's no shared mutable state between renders.

{{zoom: 1x}} {{focus: none}}

```code:mental-code lang=tsx
import { useState } from "react";

// UI = f(state)
function Counter() {
  const [count, setCount] = useState(0);

  // Each render is a snapshot
  // count is 0, then 1, then 2...
  // Each render sees its own count

  return (
    <div>
      <p>Count: {count}</p>
      <button onClick={() => setCount(count + 1)}>
        Increment
      </button>
    </div>
  );
}

// State changes → re-render → new snapshot
// React diffs old vs new → updates only changes
---
render: 4-18
snapshot: 7-9
```

```diagram:mental-diagram kind=graph
State [label="State changes" shape=box]
Call [label="React calls component"]
JSX [label="New JSX returned"]
Diff [label="Compare old vs new"]
DOM [label="Update DOM" shape=box]
Event [label="User event"]

State -> Call
Call -> JSX
JSX -> Diff
Diff -> DOM
DOM -> Event
Event -> State
```
