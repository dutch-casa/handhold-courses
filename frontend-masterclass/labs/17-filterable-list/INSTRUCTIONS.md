# Lab 17: Filterable List

Build a searchable, filterable, sortable list. All filters work together.

## Tasks

1. **Define an `Item` interface** — `readonly id: number`, `readonly name: string`, `readonly category: string`, `readonly date: string` (ISO format).

2. **Text search filter** — An `<input>` that filters items by name (case-insensitive). Store the search string in state.

3. **Category dropdown filter** — A `<select>` with "All" plus each unique category. Store the selected category in state.

4. **Sort toggle** — A `<select>` with options "name" and "date". Store the sort field in state. Sort alphabetically by name, or chronologically by date.

5. **Derive the displayed list** — Chain `.filter().filter().toSorted()` during render. Do NOT store the filtered list in state.

6. **Render with keys** — Map the displayed items to `<li key={item.id}>` elements. Show `"X of Y items"` below the list.

## Data

Use this hardcoded array as the source data:

```tsx
const ITEMS: readonly Item[] = [
  { id: 1, name: "React Guide", category: "books", date: "2024-03-15" },
  { id: 2, name: "TypeScript Handbook", category: "books", date: "2024-01-20" },
  { id: 3, name: "Mechanical Keyboard", category: "electronics", date: "2024-06-01" },
  { id: 4, name: "USB-C Hub", category: "electronics", date: "2024-02-10" },
  { id: 5, name: "Standing Desk", category: "furniture", date: "2024-04-22" },
  { id: 6, name: "Monitor Arm", category: "furniture", date: "2024-05-18" },
];
```

## Hints

- `toSorted()` creates a new sorted array without mutating the original.
- For date sorting: `new Date(a.date).getTime() - new Date(b.date).getTime()`.
- For unique categories: `[...new Set(ITEMS.map(i => i.category))]`.
- The filter chain is: text filter → category filter → sort.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
