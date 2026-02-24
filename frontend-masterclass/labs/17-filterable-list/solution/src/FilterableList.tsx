import { useState } from "react";
import "./styles.css";

interface Item {
  readonly id: number;
  readonly name: string;
  readonly category: string;
  readonly date: string;
}

const ITEMS: readonly Item[] = [
  { id: 1, name: "React Guide", category: "books", date: "2024-03-15" },
  { id: 2, name: "TypeScript Handbook", category: "books", date: "2024-01-20" },
  { id: 3, name: "Mechanical Keyboard", category: "electronics", date: "2024-06-01" },
  { id: 4, name: "USB-C Hub", category: "electronics", date: "2024-02-10" },
  { id: 5, name: "Standing Desk", category: "furniture", date: "2024-04-22" },
  { id: 6, name: "Monitor Arm", category: "furniture", date: "2024-05-18" },
];

const CATEGORIES = [...new Set(ITEMS.map((i) => i.category))];

export function FilterableList() {
  const [filters, setFilters] = useState({ search: "", category: "all", sortBy: "name" as "name" | "date" });
  const { search, category, sortBy } = filters;

  const displayed = ITEMS
    .filter((item) => item.name.toLowerCase().includes(search.toLowerCase()))
    .filter((item) => category === "all" || item.category === category)
    .toSorted((a, b) =>
      sortBy === "name"
        ? a.name.localeCompare(b.name)
        : new Date(a.date).getTime() - new Date(b.date).getTime()
    );

  return (
    <div>
      <div>
        <input
          type="text"
          value={search}
          onChange={(e) => setFilters((prev) => ({ ...prev, search: e.target.value }))}
          placeholder="Search by name..."
        />

        <select
          value={category}
          onChange={(e) => setFilters((prev) => ({ ...prev, category: e.target.value }))}
        >
          <option value="all">All</option>
          {CATEGORIES.map((cat) => (
            <option key={cat} value={cat}>
              {cat.charAt(0).toUpperCase() + cat.slice(1)}
            </option>
          ))}
        </select>

        <select
          value={sortBy}
          onChange={(e) => setFilters((prev) => ({ ...prev, sortBy: e.target.value as "name" | "date" }))}
        >
          <option value="name">Sort by name</option>
          <option value="date">Sort by date</option>
        </select>
      </div>

      <ul>
        {displayed.map((item) => (
          <li key={item.id}>
            <div>
              <strong>{item.name}</strong>
              <span>{item.category}</span>
            </div>
            <time>{item.date}</time>
          </li>
        ))}
      </ul>

      <p>
        {displayed.length} of {ITEMS.length} items
      </p>
    </div>
  );
}
