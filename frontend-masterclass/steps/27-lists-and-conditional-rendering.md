---
title: Lists and conditional rendering
---

# Rendering arrays

{{show: lists-code typewriter 2s linear}} Arrays in JSX render each element in sequence. `[<li>A</li>, <li>B</li>]` renders two list items. The `.map()` method transforms data arrays into JSX arrays. This is how you render dynamic lists.

{{focus: basic-map}} {{zoom: 1.2x}} {{pulse: basic-map}} {{annotate: basic-map "Map to JSX"}} `users.map(user => <li>{user.name}</li>)` — transform each user object into a `<li>`. The result is an array of JSX elements. React renders each one. No loops, no imperative DOM creation.

{{zoom: 1x}} {{focus: key-prop}} {{zoom: 1.2x}} {{pulse: key-prop}} {{annotate: key-prop "Key identity"}} Every mapped element needs a `key` prop. `key={user.id}` tells React: "this element represents this specific user." Between renders, React matches elements by key to determine what changed, what was added, and what was removed.

{{zoom: 1x}} {{focus: no-key}} {{zoom: 1.2x}} {{pan: no-key}} {{annotate: no-key "Index is not identity"}} Without keys (or with array indices as keys), React matches by position. Insert an item at the top? Every element "changes" because its position shifted. React destroys and recreates all of them. With stable keys, React knows the new item is new and the rest are unchanged.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:lists-code lang=tsx
interface User {
  readonly id: number;
  readonly name: string;
  readonly email: string;
}

// Map data to JSX
function UserList({ users }: { readonly users: readonly User[] }) {
  return (
    <ul>
      {users.map((user) => (
        <li key={user.id}>
          {user.name} — {user.email}
        </li>
      ))}
    </ul>
  );
}

// BAD: index as key — breaks on reorder
function BadList({ items }: { readonly items: readonly string[] }) {
  return (
    <ul>
      {items.map((item, index) => (
        <li key={index}>{item}</li>
      ))}
    </ul>
  );
}
// Insert at top → every key shifts → React rebuilds all
---
basic-map: 8-17
key-prop: 12
no-key: 21-28
```

# Why keys matter

{{clear: slide}}

{{show: keys-diagram reveal 0.3s spring}} {{show: keys-code typewriter 2s linear}} Keys are identity. Without them, React sees position. With them, React sees entities. The difference determines whether React reuses DOM nodes or destroys them.

{{focus: without}} {{zoom: 1.2x}} {{pulse: without}} {{annotate: without "Positional matching"}} Three items: Alice, Bob, Charlie. Insert Diana at the top. Without stable keys, position 0 was "Alice," now it's "Diana." React thinks Alice changed to Diana, Bob changed to Alice, Charlie changed to Bob, and a new Charlie appeared. Four DOM updates.

{{zoom: 1x}} {{focus: with}} {{zoom: 1.2x}} {{pulse: with}} {{annotate: with "Identity matching"}} Same scenario, stable keys. React matches by key: Alice is still Alice (moved from 0 to 1). Bob is still Bob. Charlie is still Charlie. Diana is new. One DOM insertion. Three nodes untouched. Faster, and input state is preserved.

{{zoom: 1x}} {{focus: none}}

```code:keys-code lang=tsx
// Before: [Alice, Bob, Charlie]
// After:  [Diana, Alice, Bob, Charlie]

// Without stable keys (index):
// key=0: Alice → Diana    (update text)
// key=1: Bob   → Alice    (update text)
// key=2: Charlie → Bob    (update text)
// key=3: (new)  → Charlie (create)
// Result: 4 DOM operations

// With stable keys (id):
// key="diana": (new) → create Diana
// key="alice": unchanged
// key="bob":   unchanged
// key="charlie": unchanged
// Result: 1 DOM operation
---
without: 4-9
with: 11-15
```

```diagram:keys-diagram kind=graph
Before [label="[Alice, Bob, Charlie]" shape=box]
Insert [label="Insert Diana at top"]
After [label="[Diana, Alice, Bob, Charlie]" shape=box]

IndexMatch [label="Index keys:\n4 DOM updates" shape=box]
IdMatch [label="ID keys:\n1 DOM insert" shape=box]

Before -> Insert
Insert -> After
After -> IndexMatch
After -> IdMatch
```

# Conditional rendering

{{clear: slide}}

{{show: cond-code typewriter 2s linear}} Sometimes a component should render something, and sometimes it shouldn't. React gives you JavaScript's own conditional tools — no special template syntax.

{{focus: ternary}} {{zoom: 1.2x}} {{pulse: ternary}} {{annotate: ternary "Ternary"}} `isLoggedIn ? <Dashboard /> : <Login />` — render one or the other. The ternary operator is the workhorse of conditional rendering. One condition, two branches.

{{zoom: 1x}} {{focus: and-op}} {{zoom: 1.2x}} {{annotate: and-op "Logical AND"}} `hasNotification && <Badge />` — render or render nothing. If `hasNotification` is `false`, React renders nothing. If `true`, React renders the `<Badge>`. Watch out for `0` — `0 && <Badge />` renders the string `"0"`, not nothing. Use booleans.

{{zoom: 1x}} {{focus: early-return}} {{zoom: 1.2x}} {{pulse: early-return}} {{annotate: early-return "Early return"}} Guard clause at the top. If loading, return the spinner. If error, return the error. The "happy path" renders at the bottom. This reads top-to-bottom like prose — handle the edge cases, then the main case.

{{zoom: 1x}} {{focus: null-render}} {{zoom: 1.2x}} {{pan: null-render}} {{annotate: null-render "Return null"}} `return null` renders nothing. The component is in the tree, React calls it, but it produces no DOM nodes. Use this for components that conditionally contribute nothing.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:cond-code lang=tsx
// Ternary: one or the other
function App({ isLoggedIn }: { readonly isLoggedIn: boolean }) {
  return isLoggedIn ? <Dashboard /> : <Login />;
}

// Logical AND: render or nothing
function Navbar({ unreadCount }: { readonly unreadCount: number }) {
  return (
    <nav>
      <span>Home</span>
      {unreadCount > 0 && <Badge count={unreadCount} />}
    </nav>
  );
}

// Early return: guard clauses
function UserProfile({ user, isLoading, error }: ProfileProps) {
  if (isLoading) return <Spinner />;
  if (error) return <ErrorMessage message={error} />;
  if (!user) return null;

  return (
    <div>
      <h1>{user.name}</h1>
      <p>{user.email}</p>
    </div>
  );
}

// Conditional styles
function Tag({ active }: { readonly active: boolean }) {
  return (
    <span className={`tag ${active ? "tag-active" : ""}`}>
      Label
    </span>
  );
}
---
ternary: 2-4
and-op: 8-14
early-return: 17-22
null-render: 20
```

# Filtering and sorting

{{clear: slide}}

{{show: filter-code typewriter 2s linear}} Derived data lives in the render, not in state. Filtering and sorting are the most common derivations. Compute the displayed list from the source data and the current filter/sort criteria.

{{focus: filter}} {{zoom: 1.2x}} {{pulse: filter}} {{annotate: filter "Filter in render"}} `users.filter(u => u.role === selectedRole)` — derive the filtered list from the full list and the selected role. When either changes, React re-renders, and the filter runs again. One source of truth: the full list. One control: the role selector.

{{zoom: 1x}} {{focus: sort}} {{zoom: 1.2x}} {{annotate: sort "Sort in render"}} `toSorted()` creates a new sorted array without mutating the original. Chain filter and sort: filter first (reduce the set), then sort (order the remainder). The chain reads like a pipeline.

{{zoom: 1x}} {{focus: combined}} {{zoom: 1.2x}} {{pan: combined}} {{pulse: combined}} {{annotate: combined "Pipeline"}} Search text filters by name. Category dropdown filters by category. Sort selector changes the order. All three applied in sequence. The displayed list is derived from three pieces of state applied to the source data.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:filter-code lang=tsx
interface Product {
  readonly id: number;
  readonly name: string;
  readonly category: string;
  readonly price: number;
}

function ProductList({ products }: { readonly products: readonly Product[] }) {
  const [search, setSearch] = useState("");
  const [category, setCategory] = useState("all");
  const [sortBy, setSortBy] = useState<"name" | "price">("name");

  // Derived: filter then sort
  const displayed = products
    .filter((p) => p.name.toLowerCase().includes(search.toLowerCase()))
    .filter((p) => category === "all" || p.category === category)
    .toSorted((a, b) =>
      sortBy === "name"
        ? a.name.localeCompare(b.name)
        : a.price - b.price
    );

  return (
    <div>
      <input
        value={search}
        onChange={(e) => setSearch(e.target.value)}
        placeholder="Search..."
      />
      <select value={category} onChange={(e) => setCategory(e.target.value)}>
        <option value="all">All</option>
        <option value="electronics">Electronics</option>
        <option value="books">Books</option>
      </select>
      <select value={sortBy} onChange={(e) => setSortBy(e.target.value as "name" | "price")}>
        <option value="name">Name</option>
        <option value="price">Price</option>
      </select>
      <ul>
        {displayed.map((p) => (
          <li key={p.id}>
            {p.name} — ${p.price}
          </li>
        ))}
      </ul>
      <p>{displayed.length} of {products.length} products</p>
    </div>
  );
}
---
filter: 15-16
sort: 17-21
combined: 14-21
```

# Lifting state up

{{clear: slide}}

{{show: lift-diagram reveal 0.3s spring}} {{show: lift-code typewriter 2s linear}} When two sibling components need to share state, neither can own it. The solution: lift the state up to their closest common parent. The parent owns the state and passes it down as props.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "Siblings can't share"}} `SearchInput` has the search text. `ResultCount` needs it to display "X results." They're siblings — neither can reach into the other. Props flow down, not sideways.

{{zoom: 1x}} {{focus: lifted}} {{zoom: 1.2x}} {{pulse: lifted}} {{pan: lifted}} {{annotate: lifted "State lifted to parent"}} Move `search` state to the parent. Pass `search` and `setSearch` down to `SearchInput`. Pass `search` to `ResultCount`. Both siblings read from the same source. The parent is the single owner. This is lifting state up.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:lift-code lang=tsx
// BEFORE: search state trapped in SearchInput
function SearchInput() {
  const [search, setSearch] = useState("");
  return <input value={search} onChange={(e) => setSearch(e.target.value)} />;
}
function ResultCount() {
  // Can't access search — it's in a sibling!
  return <p>? results</p>;
}

// AFTER: state lifted to parent
function SearchPage({ items }: { readonly items: readonly string[] }) {
  const [search, setSearch] = useState("");

  const filtered = items.filter((item) =>
    item.toLowerCase().includes(search.toLowerCase())
  );

  return (
    <div>
      <SearchBar value={search} onChange={setSearch} />
      <ResultCount count={filtered.length} />
      <ItemList items={filtered} />
    </div>
  );
}

function SearchBar({
  value,
  onChange,
}: {
  readonly value: string;
  readonly onChange: (value: string) => void;
}) {
  return (
    <input value={value} onChange={(e) => onChange(e.target.value)} />
  );
}

function ResultCount({ count }: { readonly count: number }) {
  return <p>{count} results</p>;
}

function ItemList({ items }: { readonly items: readonly string[] }) {
  return (
    <ul>
      {items.map((item) => (
        <li key={item}>{item}</li>
      ))}
    </ul>
  );
}
---
problem: 2-9
lifted: 12-25
```

```diagram:lift-diagram kind=graph
Parent [label="SearchPage\nowns search state" shape=box]
Search [label="SearchBar\nreads + writes"]
Count [label="ResultCount\nreads"]
List [label="ItemList\nreads filtered"]

Parent -> Search [label="value, onChange"]
Parent -> Count [label="count"]
Parent -> List [label="items"]
```
