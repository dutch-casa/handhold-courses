---
title: Components, props, and JSX
---

# Function components

{{show: func-comp typewriter 2s linear}} Every React component is a function. No class components. No `this`. A function that takes props and returns JSX.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "Basic component"}} Capital first letter — `Card`, not `card`. React uses the casing to distinguish components from HTML elements. The function returns JSX. JSX looks like HTML but it's JavaScript.

{{zoom: 1x}} {{focus: typed}} {{zoom: 1.2x}} {{pulse: typed}} {{annotate: typed "TypeScript props"}} Define a props interface. `interface CardProps { title: string; body: string; }`. Destructure in the parameter: `function Card({ title, body }: CardProps)`. The compiler checks every usage.

{{zoom: 1x}} {{focus: return}} {{zoom: 1.2x}} {{annotate: return "JSX return"}} Wrap multi-line JSX in parentheses. `className` instead of `class` (because `class` is a JavaScript keyword). `htmlFor` instead of `for`. Expressions in curly braces: `{title}`, `{count + 1}`, `{isActive ? "yes" : "no"}`.

{{zoom: 1x}} {{focus: none}}

```code:func-comp lang=tsx
// Props interface
interface CardProps {
  readonly title: string;
  readonly body: string;
  readonly variant?: "default" | "featured";
}

// Function component with typed props
function Card({ title, body, variant = "default" }: CardProps) {
  return (
    <div className={`card card-${variant}`}>
      <h2 className="card-title">{title}</h2>
      <p className="card-body">{body}</p>
    </div>
  );
}

// Usage
function App() {
  return (
    <div>
      <Card title="Hello" body="World" />
      <Card title="Featured" body="Content" variant="featured" />
    </div>
  );
}
---
basic: 9
typed: 2-6
return: 11-16
```

# Props flow down

{{clear: slide}}

{{show: props-code typewriter 2s linear}} Props are the data a parent passes to a child. They flow in one direction: down the tree.

{{focus: passing}} {{zoom: 1.2x}} {{pulse: passing}} {{annotate: passing "Passing props"}} `<UserCard name="Alice" age={30} />` passes two props. Strings use quotes. Everything else uses curly braces — numbers, booleans, objects, arrays, functions.

{{zoom: 1x}} {{focus: children}} {{zoom: 1.2x}} {{pulse: children}} {{annotate: children "Children prop"}} Content between opening and closing tags becomes the `children` prop. `<Card><p>Hello</p></Card>` — the paragraph is `children`. This enables composition. A `Card` doesn't need to know what's inside it.

{{zoom: 1x}} {{focus: lists}} {{zoom: 1.2x}} {{pulse: lists}} {{annotate: lists "Lists and keys"}} `.map()` renders arrays. Each item needs a `key` prop — a stable, unique identifier. React uses keys to track which items changed, were added, or removed. Use IDs from data, never array indices.

{{zoom: 1x}} {{focus: fragments}} {{zoom: 1.2x}} {{annotate: fragments "Fragments"}} `<>...</>` groups elements without adding a DOM node. `<Fragment key={id}>` when you need a key on a fragment. Components must return a single root — fragments are the invisible wrapper.

{{zoom: 1x}} {{focus: none}}

```code:props-code lang=tsx
// Passing props: strings, numbers, objects, functions
<UserCard
  name="Alice"
  age={30}
  skills={["React", "TypeScript"]}
  onEdit={() => console.log("edit")}
/>

// Children prop: composition
function Card({ children }: { children: React.ReactNode }) {
  return <div className="card">{children}</div>;
}
<Card>
  <h2>Title</h2>
  <p>Any content works here</p>
</Card>

// Lists with keys
function UserList({ users }: { users: readonly User[] }) {
  return (
    <ul>
      {users.map((user) => (
        <li key={user.id}>{user.name}</li>
      ))}
    </ul>
  );
}

// Fragments: invisible wrapper
function Columns() {
  return (
    <>
      <td>Name</td>
      <td>Age</td>
    </>
  );
}
---
passing: 2-7
children: 10-16
lists: 19-27
fragments: 30-36
```
