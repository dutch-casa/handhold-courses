---
title: State and events
---

# The missing piece

{{show: static-code typewriter 2s linear}} Components with only props are static. A parent passes data down, the child renders it. Nothing changes on screen unless the parent re-renders with different props. But most UI needs to respond to the user. A counter counts. A toggle toggles. An input accepts text.

{{focus: static}} {{zoom: 1.2x}} {{pulse: static}} {{annotate: static "Static only"}} This component renders whatever props it receives. Click a button? Nothing happens. Type in a field? Nothing updates. Props alone give you a one-way mirror — data in, pixels out.

{{zoom: 1x}} {{focus: problem}} {{zoom: 1.2x}} {{annotate: problem "No interactivity"}} The `onClick` fires, but there's no way to change what's displayed. You'd need to re-render with new props. But who re-renders the component? The parent. And how does the parent know to re-render? You need a mechanism for a component to say: "my data changed, draw me again."

{{zoom: 1x}} {{focus: none}} That mechanism is state.

```code:static-code lang=tsx
function Counter({ count }: { readonly count: number }) {
  return (
    <div>
      <p>Count: {count}</p>
      <button onClick={() => {
        // count is a prop — read-only
        // how do we update the display?
      }}>
        Increment
      </button>
    </div>
  );
}

// The parent controls the value.
// The child can't change it.
// We need something the child owns.
---
static: 1-8
problem: 5-8
```

# useState

{{clear: slide}}

{{show: usestate-code typewriter 2s linear}} `useState` gives a component its own data that persists between renders. Import it from React. Call it at the top of your component. It returns a pair: the current value, and a function to update it.

{{focus: declare}} {{zoom: 1.2x}} {{pulse: declare}} {{annotate: declare "Declare state"}} `const [count, setCount] = useState(0)`. The argument is the initial value — only used on the first render. `count` is the current value. `setCount` is the setter.

{{zoom: 1x}} {{focus: update}} {{zoom: 1.2x}} {{pulse: update}} {{annotate: update "Update triggers render"}} Call `setCount(count + 1)` inside an event handler. React schedules a re-render. When the component function runs again, `count` is the new value. The old render is gone. This render sees `1`.

{{zoom: 1x}} {{focus: multiple}} {{zoom: 1.2x}} {{pan: multiple}} {{annotate: multiple "Multiple states"}} A component can call `useState` multiple times. Each call creates an independent piece of state. `name` and `age` are separate. Updating one doesn't affect the other.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:usestate-code lang=tsx
import { useState } from "react";

function Counter() {
  // Declare state with initial value
  const [count, setCount] = useState(0);

  return (
    <div>
      <p>Count: {count}</p>
      <button onClick={() => setCount(count + 1)}>
        Increment
      </button>
    </div>
  );
}

// Multiple independent states
function UserForm() {
  const [name, setName] = useState("");
  const [age, setAge] = useState(0);

  return (
    <form>
      <input value={name} onChange={(e) => setName(e.target.value)} />
      <input
        type="number"
        value={age}
        onChange={(e) => setAge(Number(e.target.value))}
      />
    </form>
  );
}
---
declare: 4-5
update: 10-12
multiple: 19-20
```

# State is a snapshot

{{clear: slide}}

{{show: snapshot-diagram reveal 0.3s spring}} {{show: snapshot-code typewriter 2s linear}} Each render is a snapshot. When React calls your component function, `count` is a constant — not a variable that changes. It's the value from this specific render.

{{focus: snapshot-render}} {{zoom: 1.2x}} {{pulse: snapshot-render}} {{annotate: snapshot-render "Frozen value"}} Inside `handleClick`, `count` is `0`. Calling `setCount` doesn't change `count` in this render. It requests a new render where `count` will be `1`. The current render still sees `0` everywhere.

{{zoom: 1x}} {{focus: triple}} {{zoom: 1.2x}} {{pan: triple}} {{annotate: triple "Same snapshot"}} Three `setCount(count + 1)` calls in a row? `count` is `0` in all three. All three set state to `0 + 1 = 1`. Not 3. Each call reads the same snapshot.

{{pan: none}} {{zoom: 1x}} {{focus: functional}} {{zoom: 1.2x}} {{pulse: functional}} {{annotate: functional "Functional update"}} `setCount(prev => prev + 1)` — the functional form. Each update reads the previous pending value, not the render snapshot. Three functional updates: `0 → 1 → 2 → 3`. Use this form when the new value depends on the old value.

{{zoom: 1x}} {{focus: none}}

```code:snapshot-code lang=tsx
import { useState } from "react";

function Counter() {
  const [count, setCount] = useState(0);

  function handleClick() {
    // count is 0 in this render — a snapshot
    setCount(count + 1); // schedules: set to 1
    setCount(count + 1); // schedules: set to 1 (not 2!)
    setCount(count + 1); // schedules: set to 1 (not 3!)
    console.log(count);  // still 0 — snapshot
  }

  function handleClickCorrect() {
    // Functional updates read pending state
    setCount((prev) => prev + 1); // 0 → 1
    setCount((prev) => prev + 1); // 1 → 2
    setCount((prev) => prev + 1); // 2 → 3
  }

  return (
    <div>
      <p>{count}</p>
      <button onClick={handleClick}>Broken +3</button>
      <button onClick={handleClickCorrect}>Real +3</button>
    </div>
  );
}
---
snapshot-render: 6-12
triple: 8-10
functional: 15-19
```

```diagram:snapshot-diagram kind=graph
Render0 [label="Render 0\ncount = 0" shape=box]
Click [label="onClick fires"]
SetState [label="setCount(1)"]
Render1 [label="Render 1\ncount = 1" shape=box]
Click2 [label="onClick fires"]
SetState2 [label="setCount(2)"]
Render2 [label="Render 2\ncount = 2" shape=box]

Render0 -> Click
Click -> SetState
SetState -> Render1
Render1 -> Click2
Click2 -> SetState2
SetState2 -> Render2
```

# Event handlers

{{clear: slide}}

{{show: events-code typewriter 2s linear}} React event handlers look like DOM event handlers but they're synthetic. React wraps native events in a consistent cross-browser interface. The naming convention: `onClick`, `onChange`, `onSubmit` — camelCase, not lowercase.

{{focus: click}} {{zoom: 1.2x}} {{pulse: click}} {{annotate: click "Click handler"}} Pass a function reference, not a function call. `onClick={handleClick}` — no parentheses. `onClick={handleClick()}` would call the function during render, which is wrong.

{{zoom: 1x}} {{focus: inline}} {{zoom: 1.2x}} {{annotate: inline "Inline handler"}} Arrow functions inline: `onClick={() => setCount(count + 1)}`. Fine for short handlers. Extract to a named function when the logic grows past one line.

{{zoom: 1x}} {{focus: event-obj}} {{zoom: 1.2x}} {{pulse: event-obj}} {{annotate: event-obj "Event object"}} The handler receives an event object. `e.target` is the element that fired the event. `e.preventDefault()` stops default behavior — essential for form submission. `e.currentTarget` is the element the handler is attached to.

{{zoom: 1x}} {{focus: submit}} {{zoom: 1.2x}} {{pan: submit}} {{annotate: submit "Form submit"}} `onSubmit` on the `<form>` element. Call `e.preventDefault()` first — without it, the browser reloads the page. Read values from state, not from the DOM.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:events-code lang=tsx
import { useState } from "react";

function EventExamples() {
  const [count, setCount] = useState(0);

  // Named handler — function reference, no parens
  function handleClick() {
    setCount((prev) => prev + 1);
  }

  return (
    <div>
      <button onClick={handleClick}>Named</button>
      <button onClick={() => setCount(0)}>Inline reset</button>
    </div>
  );
}

// Event object
function Input() {
  const [value, setValue] = useState("");

  function handleChange(e: React.ChangeEvent<HTMLInputElement>) {
    setValue(e.target.value);
  }

  return <input value={value} onChange={handleChange} />;
}

// Form submission
function LoginForm() {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    console.log("Submit:", email, password);
  }

  return (
    <form onSubmit={handleSubmit}>
      <input value={email} onChange={(e) => setEmail(e.target.value)} />
      <input
        type="password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />
      <button type="submit">Log in</button>
    </form>
  );
}
---
click: 7-9
inline: 14
event-obj: 23-25
submit: 35-38
```

# Controlled inputs

{{clear: slide}}

{{show: controlled-code typewriter 2s linear}} A controlled input is one where React owns the value. You set `value` from state and update state in `onChange`. The DOM input never stores its own value — React is the single source of truth.

{{focus: controlled}} {{zoom: 1.2x}} {{pulse: controlled}} {{annotate: controlled "Single source of truth"}} `value={text}` makes the input display whatever's in state. `onChange` updates state on every keystroke. React re-renders. The input shows the new value. The loop is: state → render → user types → onChange → setState → re-render.

{{zoom: 1x}} {{focus: uncontrolled}} {{zoom: 1.2x}} {{annotate: uncontrolled "Uncontrolled"}} Without `value`, the input manages itself. You read it with a ref when you need it. This is "uncontrolled." Controlled is the default in React — it gives you full power over validation, formatting, and derived state.

{{zoom: 1x}} {{focus: textarea}} {{zoom: 1.2x}} {{pan: textarea}} {{annotate: textarea "Same pattern"}} Same pattern for `<textarea>`, `<select>`, checkboxes. The shape is always: state variable, `value` or `checked` prop, `onChange` handler. Once you know the pattern, every input works the same way.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:controlled-code lang=tsx
import { useRef, useState } from "react";

// Controlled: React owns the value
function SearchBar() {
  const [text, setText] = useState("");

  return (
    <div>
      <input
        value={text}
        onChange={(e) => setText(e.target.value)}
        placeholder="Search..."
      />
      <p>{text.length} characters</p>
    </div>
  );
}

// Uncontrolled: the DOM owns the value
function FileUpload() {
  const fileRef = useRef<HTMLInputElement>(null);

  return <input type="file" ref={fileRef} />;
}

// Controlled textarea and select
function NoteEditor() {
  const [content, setContent] = useState("");
  const [category, setCategory] = useState("general");

  return (
    <form>
      <textarea
        value={content}
        onChange={(e) => setContent(e.target.value)}
        rows={5}
      />
      <select
        value={category}
        onChange={(e) => setCategory(e.target.value)}
      >
        <option value="general">General</option>
        <option value="work">Work</option>
        <option value="personal">Personal</option>
      </select>
    </form>
  );
}
---
controlled: 4-16
uncontrolled: 20-24
textarea: 27-44
```

# Derived state

{{clear: slide}}

{{show: derived-code typewriter 2s linear}} Not everything needs to be state. If a value can be computed from existing state, compute it during render. Don't store it in a separate `useState` — that creates a synchronization problem.

{{focus: bad}} {{zoom: 1.2x}} {{pulse: bad}} {{annotate: bad "Duplicated state"}} Two states that must always agree. Update `items` but forget to update `count`? They're out of sync. This is a bug waiting to happen. Every time you see "I need to keep these in sync," you're looking at derived state stored as real state.

{{zoom: 1x}} {{focus: good}} {{zoom: 1.2x}} {{pulse: good}} {{annotate: good "Derived at render"}} One state: `items`. The count is `items.length` — computed fresh on every render. The filter is `items.filter(...)` — also computed. No sync bugs because there's nothing to sync. One source of truth, everything else derived.

{{zoom: 1x}} {{focus: complex}} {{zoom: 1.2x}} {{pan: complex}} {{annotate: complex "Complex derivation"}} Even complex derivations belong in render, not in state. `totalPrice` derived from a cart array. `isValid` derived from form fields. `filteredUsers` derived from a search query and user list. If it can be computed, compute it.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:derived-code lang=tsx
import { useState } from "react";

// BAD: duplicated state
function TodoListBad() {
  const [items, setItems] = useState<string[]>([]);
  const [count, setCount] = useState(0); // sync bug!

  function addItem(text: string) {
    setItems([...items, text]);
    setCount(count + 1); // must remember this every time
  }

  return <p>{count} items</p>;
}

// GOOD: derived at render time
function TodoListGood() {
  const [items, setItems] = useState<string[]>([]);

  // Derived — always correct, zero sync bugs
  const count = items.length;
  const hasItems = items.length > 0;

  return (
    <div>
      <p>{count} items</p>
      {hasItems && <ul>{items.map((i) => <li key={i}>{i}</li>)}</ul>}
    </div>
  );
}

// Complex derivation — still not state
function Cart({ items }: { readonly items: readonly CartItem[] }) {
  const totalPrice = items.reduce((sum, i) => sum + i.price * i.qty, 0);
  const itemCount = items.reduce((sum, i) => sum + i.qty, 0);
  const freeShipping = totalPrice > 50;

  return (
    <div>
      <p>{itemCount} items — ${totalPrice.toFixed(2)}</p>
      {freeShipping && <p>Free shipping!</p>}
    </div>
  );
}
---
bad: 4-12
good: 16-29
complex: 32-42
```

# Batched updates

{{clear: slide}}

{{show: batch-code typewriter 2s linear}} React batches state updates. Multiple `setState` calls in the same event handler produce one re-render, not many. This is a performance feature — React waits until all state updates in a handler are collected, then re-renders once with all new values.

{{focus: batched}} {{zoom: 1.2x}} {{pulse: batched}} {{annotate: batched "One re-render"}} Three `setState` calls. One re-render. React collects all three, applies them together, and calls your component function once with all new values. The intermediate states never appear on screen.

{{zoom: 1x}} {{focus: object}} {{zoom: 1.2x}} {{pan: object}} {{annotate: object "Object state"}} When state is an object, spread the previous state and override what changed. `{ ...prev, name: "Alice" }` creates a new object with the updated field. Never mutate state directly — `state.name = "Alice"` doesn't trigger a re-render.

{{pan: none}} {{zoom: 1x}} {{focus: immutable}} {{zoom: 1.2x}} {{pulse: immutable}} {{annotate: immutable "Always new reference"}} Arrays too: `[...items, newItem]` to add, `items.filter(...)` to remove, `items.map(...)` to update. Each operation creates a new array. React compares the reference — same object means "nothing changed," new object means "re-render."

{{zoom: 1x}} {{focus: none}}

```code:batch-code lang=tsx
import { useState } from "react";

function Profile() {
  const [name, setName] = useState("");
  const [email, setEmail] = useState("");
  const [age, setAge] = useState(0);

  function handleReset() {
    // All three batched — one re-render
    setName("");
    setEmail("");
    setAge(0);
  }

  return <button onClick={handleReset}>Reset</button>;
}

// Object state — spread to update
function UserEditor() {
  const [user, setUser] = useState({
    name: "Alice",
    email: "alice@example.com",
    role: "editor" as const,
  });

  function updateName(name: string) {
    setUser((prev) => ({ ...prev, name }));
  }

  return (
    <input value={user.name} onChange={(e) => updateName(e.target.value)} />
  );
}

// Array state — always new arrays
function TodoApp() {
  const [todos, setTodos] = useState<readonly string[]>([]);

  function add(text: string) {
    setTodos((prev) => [...prev, text]);
  }
  function remove(index: number) {
    setTodos((prev) => prev.filter((_, i) => i !== index));
  }
  function update(index: number, text: string) {
    setTodos((prev) => prev.map((t, i) => (i === index ? text : t)));
  }

  return <p>{todos.length} todos</p>;
}
---
batched: 9-13
object: 20-29
immutable: 37-46
```
