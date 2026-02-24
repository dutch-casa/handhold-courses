async function loadUsers() {
  const loading = document.querySelector(".loading");
  const error = document.querySelector(".error");
  const list = document.querySelector(".user-list");

  loading.classList.remove("hidden");
  error.classList.add("hidden");

  try {
    const response = await fetch("users.json");
    if (!response.ok) throw new Error(`Failed to fetch users (${response.status})`);

    const users = await response.json();

    for (const user of users) {
      const li = document.createElement("li");
      li.innerHTML = `<strong>${user.name}</strong> &mdash; ${user.email}`;
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
