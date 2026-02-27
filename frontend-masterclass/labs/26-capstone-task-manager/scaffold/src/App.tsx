import { useState } from "react";
// TODO: Import QueryClient, QueryClientProvider from "@tanstack/react-query"
// TODO: Import TaskList, AddTask

// TODO: Create QueryClient with staleTime: 60_000

// TODO: Wrap in QueryClientProvider
// Structure:
//   <QueryClientProvider client={queryClient}>
//     <div className={`app ${theme}`}>
//       <h1>Task Manager</h1>
//       <AddTask />
//       <FilterBar filter={filter} onFilterChange={setFilter} />
//       <TaskList filter={filter} />
//     </div>
//   </QueryClientProvider>

import "./styles.css";

export default function App() {
  const [filter, setFilter] = useState<"all" | "active" | "completed">("all");

  return (
    <main className="app">
      <h1>Task Manager</h1>
      {/* Add QueryClientProvider, AddTask, filters, TaskList */}
    </main>
  );
}
