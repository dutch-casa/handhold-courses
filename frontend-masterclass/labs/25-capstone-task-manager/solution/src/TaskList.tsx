import { useQuery, useMutation, useQueryClient } from "@tanstack/react-query";
import { fetchTasks, toggleTask, deleteTask, type Task } from "./api";

function TaskSkeleton() {
  return (
    <div className="task-list" aria-busy="true">
      {[1, 2, 3].map((i) => (
        <div key={i} className="task-item task-item--skeleton" aria-hidden="true">
          <div className="skeleton skeleton-checkbox" />
          <div className="skeleton skeleton-text" />
          <div className="skeleton skeleton-button" />
        </div>
      ))}
    </div>
  );
}

export function TaskList({ filter }: { readonly filter: "all" | "active" | "completed" }) {
  const queryClient = useQueryClient();

  const { data: tasks, isLoading, isError, error } = useQuery({
    queryKey: ["tasks"],
    queryFn: fetchTasks,
  });

  const toggleMutation = useMutation({
    mutationFn: toggleTask,
    onMutate: async (taskId) => {
      await queryClient.cancelQueries({ queryKey: ["tasks"] });

      const previousTasks = queryClient.getQueryData<readonly Task[]>(["tasks"]);

      queryClient.setQueryData<readonly Task[]>(["tasks"], (old) => {
        if (!old) return old;
        return old.map((t) =>
          t.id === taskId ? { ...t, completed: !t.completed } : t
        );
      });

      return { previousTasks };
    },
    onError: (_err, _taskId, context) => {
      if (context?.previousTasks) {
        queryClient.setQueryData(["tasks"], context.previousTasks);
      }
    },
    onSettled: () => {
      queryClient.invalidateQueries({ queryKey: ["tasks"] });
    },
  });

  const deleteMutation = useMutation({
    mutationFn: deleteTask,
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["tasks"] });
    },
  });

  if (isLoading) return <TaskSkeleton />;

  if (isError) {
    return (
      <div className="error-state" role="alert">
        Failed to load tasks: {error.message}
      </div>
    );
  }

  const displayed = (tasks ?? []).filter((task) => {
    if (filter === "all") return true;
    if (filter === "active") return !task.completed;
    return task.completed;
  });

  if (displayed.length === 0) {
    return (
      <div className="empty-state">
        {filter === "all" ? "No tasks yet. Add one above!" : `No ${filter} tasks.`}
      </div>
    );
  }

  return (
    <ul className="task-list" role="list">
      {displayed.map((task) => (
        <li key={task.id} className="task-item task-enter">
          <label className="task-toggle">
            <input
              type="checkbox"
              checked={task.completed}
              onChange={() => toggleMutation.mutate(task.id)}
              className="task-checkbox"
              aria-label={`Mark "${task.title}" as ${task.completed ? "incomplete" : "complete"}`}
            />
            <span className="task-checkmark" />
          </label>

          <span className={`task-title ${task.completed ? "task-title--done" : ""}`}>
            {task.title}
          </span>

          <button
            className="task-delete"
            onClick={() => deleteMutation.mutate(task.id)}
            aria-label="Delete task"
          >
            <svg width="16" height="16" viewBox="0 0 16 16" fill="none" aria-hidden="true">
              <path d="M4 4l8 8M12 4l-8 8" stroke="currentColor" strokeWidth="1.5" strokeLinecap="round" />
            </svg>
          </button>
        </li>
      ))}
    </ul>
  );
}
