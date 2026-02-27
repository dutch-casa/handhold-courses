import { useState } from "react";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { TaskList } from "./TaskList";
import { AddTask } from "./AddTask";
import "./styles.css";

type FilterValue = "all" | "active" | "completed";

const queryClient = new QueryClient({
  defaultOptions: {
    queries: {
      staleTime: 60_000,
    },
  },
});

const FILTERS: readonly { readonly label: string; readonly value: FilterValue }[] = [
  { label: "All", value: "all" },
  { label: "Active", value: "active" },
  { label: "Completed", value: "completed" },
] as const;

function FilterBar({ filter, onFilterChange }: {
  readonly filter: FilterValue;
  readonly onFilterChange: (value: FilterValue) => void;
}) {
  return (
    <div className="filter-bar" role="group" aria-label="Filter tasks">
      {FILTERS.map((f) => (
        <button
          key={f.value}
          className={`filter-btn ${filter === f.value ? "filter-btn--active" : ""}`}
          onClick={() => onFilterChange(f.value)}
          aria-pressed={filter === f.value}
        >
          {f.label}
        </button>
      ))}
    </div>
  );
}

export default function App() {
  const [filter, setFilter] = useState<FilterValue>("all");

  return (
    <QueryClientProvider client={queryClient}>
      <main className="app">
        <h1 className="app-title">Task Manager</h1>
        <AddTask />
        <FilterBar filter={filter} onFilterChange={setFilter} />
        <TaskList filter={filter} />
      </main>
    </QueryClientProvider>
  );
}
