// TODO: Import useQuery, useMutation, useQueryClient from "@tanstack/react-query"
// TODO: Import fetchTasks, toggleTask, deleteTask, Task from "./api"

// TODO: Create TaskList component
// Props: { filter: "all" | "active" | "completed" }
//
// useQuery: queryKey: ["tasks"], queryFn: fetchTasks
//
// Derive filtered list from data + filter (not stored in state):
//   "all" → all tasks
//   "active" → tasks where !completed
//   "completed" → tasks where completed
//
// useMutation for toggle (optimistic update):
//   onMutate: cancel queries, snapshot, update cache
//   onError: rollback
//   onSettled: invalidate ["tasks"]
//
// useMutation for delete:
//   onSuccess: invalidate ["tasks"]
//
// Render:
//   - Loading → skeleton
//   - Error → error message
//   - Each task: checkbox (toggle), title, delete button with aria-label="Delete task"
//   - Tasks enter with CSS animation class "task-enter"

export function TaskList({ filter }: { readonly filter: "all" | "active" | "completed" }) {
  return <div>{/* Build task list here */}</div>;
}
