# Fetch and display

Fetch data from a local JSON file and render a user list. Handle loading and error states.

## Tasks

- [ ] Fetch data from `users.json` when the page loads (use `async`/`await`)
- [ ] Render each user as an `<li>` in `.user-list` showing their name and email
- [ ] Show the `.loading` element while fetching (remove the `hidden` class)
- [ ] Hide the `.loading` element when done (add the `hidden` class)
- [ ] Handle fetch errors: show the `.error` element with a message

## Hints

The loading/error pattern:

```javascript
async function loadUsers() {
  const loading = document.querySelector(".loading");
  const error = document.querySelector(".error");
  const list = document.querySelector(".user-list");

  loading.classList.remove("hidden");
  error.classList.add("hidden");

  try {
    const response = await fetch("users.json");
    if (!response.ok) throw new Error("Failed to fetch");
    const users = await response.json();

    for (const user of users) {
      const li = document.createElement("li");
      li.innerHTML = `<strong>${user.name}</strong> — ${user.email}`;
      list.appendChild(li);
    }
  } catch (err) {
    error.textContent = err.message;
    error.classList.remove("hidden");
  } finally {
    loading.classList.add("hidden");
  }
}

loadUsers();
```

Use `Run Tests` in the testing panel to check your progress.
