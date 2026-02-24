const form = document.querySelector(".todo-form");
const input = document.querySelector(".todo-input");
const list = document.querySelector(".todo-list");
const countEl = document.querySelector(".count");

function updateCount() {
  const remaining = list.querySelectorAll("li:not(.done)").length;
  countEl.textContent = `${remaining} item${remaining === 1 ? "" : "s"} left`;
}

form.addEventListener("submit", (event) => {
  event.preventDefault();
  const text = input.value.trim();
  if (!text) return;

  const li = document.createElement("li");
  li.textContent = text;

  const btn = document.createElement("button");
  btn.textContent = "\u00d7";
  btn.className = "delete-btn";
  li.appendChild(btn);

  list.appendChild(li);
  input.value = "";
  updateCount();
});

list.addEventListener("click", (event) => {
  if (event.target.matches(".delete-btn")) {
    event.target.closest("li").remove();
  } else if (event.target.matches("li")) {
    event.target.classList.toggle("done");
  }
  updateCount();
});
