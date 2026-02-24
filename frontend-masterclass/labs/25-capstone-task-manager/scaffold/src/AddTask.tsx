import { useState } from "react";
// TODO: Import useMutation, useQueryClient from "@tanstack/react-query"
// TODO: Import addTask from "./api"

// TODO: Create AddTask component
// - useState for title (string)
// - useMutation with addTask, onSuccess: invalidate ["tasks"] and clear input
// - <form onSubmit={handleSubmit}>
//   - <input value={title} onChange={...} placeholder="Add a task..." />
//   - <button type="submit" disabled={mutation.isPending || !title.trim()}>
//       {mutation.isPending ? "Adding..." : "Add"}
//     </button>
// - Prevent empty submissions (check title.trim())

export function AddTask() {
  const [title, setTitle] = useState("");

  return (
    <form>
      <input
        value={title}
        onChange={(e) => setTitle(e.target.value)}
        placeholder="Add a task..."
      />
      <button type="submit">Add</button>
    </form>
  );
}
