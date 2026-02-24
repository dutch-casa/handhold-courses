---
title: Async JavaScript
---

# Why async?

{{show: async-diagram reveal 0.3s spring}} {{show: async-code typewriter 2s linear}} Network requests take time. A fetch to a server might take 200 milliseconds. If JavaScript waited — blocked — nothing else would run. No clicks, no scrolling, no animation. The page would freeze.

{{focus: sync-problem}} {{zoom: 1.2x}} {{pulse: sync-problem}} {{annotate: sync-problem "Blocking"}} Synchronous code runs line by line. If line 3 takes 2 seconds, lines 4 through 100 wait. The user sees a frozen page. This is why we need async — to start a long operation and keep running other code while we wait.

{{zoom: 1x}} {{focus: async-solution}} {{zoom: 1.2x}} {{annotate: async-solution "Non-blocking"}} Async code starts an operation, sets up a callback for when it finishes, and moves on. The fetch starts. JavaScript keeps running. When the response arrives, the callback fires. The UI stays responsive the entire time.

{{zoom: 1x}} {{focus: none}} Three patterns evolved for writing async code: callbacks, promises, and async/await. Each one builds on the last.

```code:async-code lang=javascript
// Synchronous: blocks until done
// const data = fetchSync("/api/users"); // FROZEN for 200ms
// console.log(data); // only runs after fetch completes

// Asynchronous: non-blocking
fetch("/api/users")           // starts the request
  .then((res) => res.json())  // runs when response arrives
  .then((data) => {
    console.log(data);        // runs when JSON is parsed
  });
console.log("This runs immediately!"); // doesn't wait
---
sync-problem: 2-3
async-solution: 6-11
```

```diagram:async-diagram kind=graph
Sync [label="Synchronous" shape=box]
S1 [label="Line 1"]
Fetch [label="fetch() — 200ms ⏳"]
S3 [label="Line 3 (waits)"]

Async [label="Asynchronous" shape=box]
A1 [label="Line 1"]
AFetch [label="fetch() starts →"]
A3 [label="Line 3 (runs now!)"]
Callback [label="Response arrives"]

Sync -> S1 -> Fetch -> S3
Async -> A1 -> AFetch -> A3
AFetch -> Callback
```

# Promises

{{clear: slide}}

{{show: promise-code typewriter 2s linear}} A promise is a receipt for a future value. You don't have the data yet, but you have an object that represents it.

{{focus: states}} {{zoom: 1.2x}} {{pulse: states}} {{annotate: states "Three states"}} A promise is in one of three states. Pending: the operation is still running. Fulfilled: it succeeded and has a value. Rejected: it failed and has an error. Once settled (fulfilled or rejected), a promise never changes state again.

{{zoom: 1x}} {{focus: then-catch}} {{zoom: 1.2x}} {{pulse: then-catch}} {{annotate: then-catch "then / catch"}} `.then(callback)` runs when the promise fulfills. `.catch(callback)` runs when it rejects. `.finally(callback)` runs either way — cleanup code goes here.

{{zoom: 1x}} {{focus: chaining}} {{zoom: 1.2x}} {{annotate: chaining "Chaining"}} `.then()` returns a new promise. Chain them: `fetch().then(parse).then(process).catch(handleError)`. Each step receives the return value of the previous step. The chain flows like a pipeline.

{{zoom: 1x}} {{focus: create}} {{zoom: 1.2x}} {{annotate: create "Creating promises"}} `new Promise((resolve, reject) => { ... })` wraps callback-based code. Call `resolve(value)` to fulfill. Call `reject(error)` to reject. You rarely create promises manually — most async APIs return them.

{{zoom: 1x}} {{focus: none}}

```code:promise-code lang=javascript
// Promise states: pending → fulfilled OR rejected
const promise = fetch("/api/users");
// promise is Pending...
// ...then Fulfilled (with response) or Rejected (with error)

// .then() for success, .catch() for errors
fetch("/api/users")
  .then((response) => response.json())
  .then((users) => {
    console.log(users);
  })
  .catch((error) => {
    console.error("Failed:", error.message);
  })
  .finally(() => {
    console.log("Done (success or failure)");
  });

// Chaining: each .then() returns a new promise
fetch("/api/users")
  .then((res) => res.json())
  .then((users) => users.filter((u) => u.active))
  .then((active) => console.log(active));

// Creating a promise (wrapping setTimeout)
const delay = (ms) => new Promise((resolve) => {
  setTimeout(resolve, ms);
});
---
states: 2-4
then-catch: 7-17
chaining: 20-23
create: 26-28
```

# Async / await

{{clear: slide}}

{{split}} {{show: await-code typewriter 2s linear}} {{show: await-preview reveal 0.5s spring}} `async`/`await` is syntactic sugar over promises. The code looks synchronous but runs asynchronously.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "The pattern"}} `async function` marks a function as asynchronous. `await` pauses execution until the promise settles. The result is the fulfilled value. No `.then()` chains. The code reads top to bottom.

{{zoom: 1x}} {{focus: error}} {{zoom: 1.2x}} {{pulse: error}} {{annotate: error "Error handling"}} `try`/`catch` replaces `.catch()`. Wrap the await in a try block. If the promise rejects, execution jumps to the catch block. Same error handling you'd use for synchronous code.

{{zoom: 1x}} {{focus: parallel}} {{zoom: 1.2x}} {{pulse: parallel}} {{annotate: parallel "Parallel requests"}} `await Promise.all([fetch(a), fetch(b)])` runs both requests simultaneously. Without `Promise.all`, `await fetch(a); await fetch(b)` runs them sequentially — the second waits for the first. For independent requests, always use `Promise.all`.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:await-code lang=javascript
// Basic async/await
async function getUsers() {
  const response = await fetch("/api/users");
  const users = await response.json();
  return users;
}

// Error handling with try/catch
async function getUsersSafe() {
  try {
    const response = await fetch("/api/users");
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }
    return await response.json();
  } catch (error) {
    console.error("Failed to fetch users:", error.message);
    return [];
  }
}

// Parallel requests with Promise.all
async function getDashboard() {
  const [users, posts, stats] = await Promise.all([
    fetch("/api/users").then((r) => r.json()),
    fetch("/api/posts").then((r) => r.json()),
    fetch("/api/stats").then((r) => r.json()),
  ]);
  return { users, posts, stats };
}
---
basic: 2-6
error: 9-20
parallel: 23-30
```

```preview:await-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 12px; }
  .label { font-size: 11px; color: #64748b; margin-bottom: 6px; }
  .timeline { display: flex; flex-direction: column; gap: 4px; }
  .row { display: flex; align-items: center; gap: 8px; }
  .bar { height: 24px; border-radius: 4px; display: flex; align-items: center; padding: 0 8px; font-size: 10px; color: white; font-weight: 600; }
  .seq { background: #ef4444; }
  .par { background: #22c55e; }
  .time { font-size: 10px; color: #94a3b8; min-width: 40px; }
</style>
<div class="label">Sequential (await one, then the other):</div>
<div class="timeline">
  <div class="row"><span class="time">0ms</span><div class="bar seq" style="width: 100px">fetch A</div></div>
  <div class="row"><span class="time">200ms</span><div class="bar seq" style="width: 100px">fetch B</div></div>
  <div class="row"><span class="time">400ms</span><span style="font-size:11px; color:#475569">Total: 400ms</span></div>
</div>
<div class="label" style="margin-top: 12px">Parallel (Promise.all):</div>
<div class="timeline">
  <div class="row"><span class="time">0ms</span><div class="bar par" style="width: 100px">fetch A</div></div>
  <div class="row"><span class="time">0ms</span><div class="bar par" style="width: 100px">fetch B</div></div>
  <div class="row"><span class="time">200ms</span><span style="font-size:11px; color:#475569">Total: 200ms (2x faster)</span></div>
</div>
```

# The fetch API

{{clear: slide}}

{{show: fetch-code typewriter 2s linear}} `fetch()` is the built-in way to make HTTP requests. It returns a promise that resolves to a Response object.

{{focus: get}} {{zoom: 1.2x}} {{pulse: get}} {{annotate: get "GET request"}} `fetch(url)` defaults to GET. The response object has `.ok` (true if status is 200-299), `.status` (the HTTP code), and `.json()` (parse the body as JSON). Always check `.ok` — fetch doesn't reject on 404 or 500.

{{zoom: 1x}} {{focus: post}} {{zoom: 1.2x}} {{pulse: post}} {{annotate: post "POST request"}} Pass an options object: `{ method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(data) }`. The body must be a string — `JSON.stringify` converts an object. The Content-Type header tells the server how to parse it.

{{zoom: 1x}} {{focus: pattern}} {{zoom: 1.2x}} {{annotate: pattern "Complete pattern"}} The full pattern: fetch, check ok, parse JSON, catch errors. This is the function you'll write in every project. Extract it once, use it everywhere.

{{zoom: 1x}} {{focus: none}}

```code:fetch-code lang=javascript
// GET request
async function getUsers() {
  const response = await fetch("/api/users");
  if (!response.ok) {
    throw new Error(`HTTP error: ${response.status}`);
  }
  return response.json();
}

// POST request
async function createUser(user) {
  const response = await fetch("/api/users", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(user),
  });
  if (!response.ok) {
    throw new Error(`HTTP error: ${response.status}`);
  }
  return response.json();
}

// Complete fetch pattern (reusable)
async function apiFetch(url, options = {}) {
  try {
    const response = await fetch(url, {
      headers: { "Content-Type": "application/json" },
      ...options,
    });
    if (!response.ok) {
      throw new Error(`${response.status}: ${response.statusText}`);
    }
    return await response.json();
  } catch (error) {
    console.error(`Fetch failed: ${url}`, error.message);
    throw error;
  }
}
---
get: 2-8
post: 11-21
pattern: 24-37
```

# Putting it together

{{clear: slide}}

{{show: together-code typewriter 2s linear}} A real pattern: fetch data, show loading state, handle errors, render results.

{{focus: loading}} {{zoom: 1.2x}} {{pulse: loading}} {{annotate: loading "Loading state"}} Show a loading indicator before the fetch. Hide it when done (in `finally`). The user needs feedback that something is happening — otherwise they'll click again, creating duplicate requests.

{{zoom: 1x}} {{focus: render}} {{zoom: 1.2x}} {{annotate: render "Rendering"}} Build HTML from data. Map over the array, create elements, insert them. `innerHTML` with template literals is quick for simple cases. For production code, create elements individually to avoid XSS.

{{zoom: 1x}} {{focus: error-state}} {{zoom: 1.2x}} {{pulse: error-state}} {{annotate: error-state "Error state"}} Show the user what went wrong. "Failed to load users" with a retry button. Never show raw error objects to users — translate them to human language.

{{zoom: 1x}} {{focus: none}}

```code:together-code lang=javascript
async function loadUsers() {
  const list = document.querySelector(".user-list");
  const loading = document.querySelector(".loading");
  const error = document.querySelector(".error");

  // Show loading, hide error
  loading.classList.remove("hidden");
  error.classList.add("hidden");
  list.innerHTML = "";

  try {
    const users = await apiFetch("/api/users");

    // Render the list
    const fragment = document.createDocumentFragment();
    for (const user of users) {
      const li = document.createElement("li");
      li.textContent = `${user.name} (${user.email})`;
      fragment.appendChild(li);
    }
    list.appendChild(fragment);
  } catch (err) {
    // Show error state
    error.textContent = "Failed to load users. Try again.";
    error.classList.remove("hidden");
  } finally {
    // Always hide loading
    loading.classList.add("hidden");
  }
}

// Load on page ready
loadUsers();
---
loading: 7-9
render: 14-21
error-state: 23-26
```
