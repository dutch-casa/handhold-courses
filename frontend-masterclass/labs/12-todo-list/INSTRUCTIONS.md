# Interactive todo list

Build a working todo list using vanilla JavaScript DOM manipulation.

## Tasks

- [ ] Add a todo item when the form is submitted (listen for the `submit` event on the form)
- [ ] Each todo appears as an `<li>` in the `.todo-list` `<ul>`
- [ ] Clicking a todo toggles the `.done` class on the `<li>`
- [ ] Each todo has a delete button that removes it from the list
- [ ] The `.count` element shows the number of remaining (not done) items
- [ ] Empty input does not add a todo (check for blank string)

## Hints

Listen for form submit and prevent default:

```javascript
form.addEventListener("submit", (event) => {
  event.preventDefault();
  const text = input.value.trim();
  if (!text) return;
  // ... add the todo
  input.value = "";
});
```

Create a list item with a delete button:

```javascript
const li = document.createElement("li");
li.textContent = text;
const btn = document.createElement("button");
btn.textContent = "×";
btn.className = "delete-btn";
li.appendChild(btn);
```

Use event delegation on the list:

```javascript
list.addEventListener("click", (event) => {
  if (event.target.matches(".delete-btn")) {
    event.target.closest("li").remove();
  } else if (event.target.matches("li")) {
    event.target.classList.toggle("done");
  }
  updateCount();
});
```

Count remaining items:

```javascript
function updateCount() {
  const remaining = list.querySelectorAll("li:not(.done)").length;
  countEl.textContent = `${remaining} items left`;
}
```

Use `Run Tests` in the testing panel to check your progress.
