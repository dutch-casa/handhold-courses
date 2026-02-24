import { useState } from "react";
import "./styles.css";

// TODO: Define Item interface
// - readonly id: number
// - readonly name: string
// - readonly category: string
// - readonly date: string

const ITEMS = [
  { id: 1, name: "React Guide", category: "books", date: "2024-03-15" },
  { id: 2, name: "TypeScript Handbook", category: "books", date: "2024-01-20" },
  { id: 3, name: "Mechanical Keyboard", category: "electronics", date: "2024-06-01" },
  { id: 4, name: "USB-C Hub", category: "electronics", date: "2024-02-10" },
  { id: 5, name: "Standing Desk", category: "furniture", date: "2024-04-22" },
  { id: 6, name: "Monitor Arm", category: "furniture", date: "2024-05-18" },
] as const satisfies readonly { id: number; name: string; category: string; date: string }[];

// TODO: Create FilterableList component
// State:
//   - search: string
//   - category: string ("all" initially)
//   - sortBy: "name" | "date"
//
// Derive displayed list:
//   ITEMS.filter(text).filter(category).toSorted(sortBy)
//
// Render:
//   - <input> for search
//   - <select> for category (with "All" option + unique categories)
//   - <select> for sort (name / date)
//   - <ul> with mapped <li key={item.id}> elements
//   - <p> showing "X of Y items"

export function FilterableList() {
  return <div>{/* Build filterable list here */}</div>;
}
