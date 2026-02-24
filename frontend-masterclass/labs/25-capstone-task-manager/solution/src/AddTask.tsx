import { useState, type FormEvent } from "react";
import { useMutation, useQueryClient } from "@tanstack/react-query";
import { addTask } from "./api";

export function AddTask() {
  const [title, setTitle] = useState("");
  const queryClient = useQueryClient();

  const mutation = useMutation({
    mutationFn: addTask,
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["tasks"] });
      setTitle("");
    },
  });

  function handleSubmit(e: FormEvent) {
    e.preventDefault();
    const trimmed = title.trim();
    if (!trimmed) return;
    mutation.mutate(trimmed);
  }

  return (
    <form className="add-task" onSubmit={handleSubmit}>
      <input
        className="add-task-input"
        type="text"
        value={title}
        onChange={(e) => setTitle(e.target.value)}
        placeholder="Add a task..."
        aria-label="Task title"
      />
      <button
        className="add-task-btn"
        type="submit"
        disabled={mutation.isPending || !title.trim()}
      >
        {mutation.isPending ? "Adding..." : "Add"}
      </button>
    </form>
  );
}
