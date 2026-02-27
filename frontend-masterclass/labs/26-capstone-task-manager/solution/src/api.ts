// Task type — readonly, no mutable domain
export interface Task {
  readonly id: number;
  readonly title: string;
  readonly completed: boolean;
}

// Mock database
let nextId = 4;
const db: Task[] = [
  { id: 1, title: "Learn React", completed: true },
  { id: 2, title: "Build a task manager", completed: false },
  { id: 3, title: "Deploy to production", completed: false },
];

function delay(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// TODO: Implement fetchTasks
// - Simulate 300ms delay
// - Return a copy of db array
export async function fetchTasks(): Promise<readonly Task[]> {
  await delay(300);
  return [...db];
}

// TODO: Implement addTask
// - Simulate 200ms delay
// - Create a new task with nextId++, the given title, completed: false
// - Push to db
// - Return the new task
export async function addTask(title: string): Promise<Task> {
  await delay(200);
  const task: Task = { id: nextId++, title, completed: false };
  db.push(task);
  return task;
}

// TODO: Implement toggleTask
// - Simulate 200ms delay
// - Find the task by id, flip its completed flag
// - Return the updated task
export async function toggleTask(id: number): Promise<Task> {
  await delay(200);
  const index = db.findIndex((t) => t.id === id);
  if (index === -1) throw new Error("Task not found");
  const task = db[index];
  if (!task) throw new Error("Task not found");
  const updated: Task = { ...task, completed: !task.completed };
  db[index] = updated;
  return updated;
}

// TODO: Implement deleteTask
// - Simulate 200ms delay
// - Remove the task from db by id
export async function deleteTask(id: number): Promise<void> {
  await delay(200);
  const index = db.findIndex((t) => t.id === id);
  if (index === -1) throw new Error("Task not found");
  db.splice(index, 1);
}
