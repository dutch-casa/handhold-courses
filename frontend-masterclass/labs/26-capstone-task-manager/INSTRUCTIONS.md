# Lab 25: Capstone — Task Manager

Build a complete task manager that brings together everything from the course. React Query for data, compound components for structure, design engineering for polish.

## Tasks

1. **API layer** — `api.ts`. Define a `Task` interface: `readonly id: number`, `readonly title: string`, `readonly completed: boolean`. Create functions: `fetchTasks`, `addTask`, `toggleTask`, `deleteTask`. Use the provided `db` array as a mock backend with simulated delays.

2. **QueryClient setup** — `App.tsx`. Create a `QueryClient` with `staleTime: 60_000`. Wrap everything in `<QueryClientProvider>`.

3. **Task list** — `TaskList.tsx`. Use `useQuery` with key `["tasks"]` and `fetchTasks`. Show loading skeleton while fetching. Show error state on failure. Render each task with a checkbox (toggle), title, and delete button.

4. **Add task** — `AddTask.tsx`. Controlled input + submit button. Use `useMutation` with `addTask`. On success, invalidate `["tasks"]`. Prevent empty submissions.

5. **Toggle and delete** — `useMutation` for toggle (optimistic update). `useMutation` for delete with invalidation. Toggle should update the UI immediately (optimistic), rollback on error.

6. **Filter by status** — Filter buttons: "All", "Active", "Completed". Store filter state. Derive the filtered list during render (not in state).

7. **Animated transitions** — CSS enter animation on new tasks. Use `transform` and `opacity` only. Respect `prefers-reduced-motion`.

8. **Dark mode** — `prefers-color-scheme: dark` support. Layered shadows in light mode, border treatment in dark mode.

9. **Keyboard accessible** — Tab through tasks. Enter/Space toggles. Delete button has `aria-label`. Form submits with Enter.

10. **Mobile responsive** — Works on narrow viewports. Touch targets 44px+. `@media (hover: hover)` for hover effects.

## Data

The mock API is already scaffolded in `api.ts`. It uses an in-memory array with `setTimeout` to simulate network delay.

## Hints

- Optimistic toggle: `onMutate` updates cache, `onError` rolls back, `onSettled` invalidates.
- Filter is derived: `tasks.filter(t => filter === "all" || ...)`.
- Dark mode: use CSS custom properties with `@media (prefers-color-scheme: dark)`.
- Skeleton: match the task item layout (checkbox placeholder + text bar + button placeholder).
- For 10 tests to pass, every piece matters. This lab validates the full stack.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
