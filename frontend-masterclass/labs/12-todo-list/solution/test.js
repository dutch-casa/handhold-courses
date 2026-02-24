const fs = require("fs");
const { JSDOM } = require("jsdom");

async function test() {
  let count = 0;
  const results = [];

  function check(condition, description) {
    count++;
    results.push({ ok: condition, index: count, description });
  }

  const html = fs.readFileSync("index.html", "utf-8");
  const dom = new JSDOM(html, { runScripts: "dangerously", resources: "usable" });
  const doc = dom.window.document;

  const appJs = fs.readFileSync("app.js", "utf-8");
  dom.window.eval(appJs);

  const form = doc.querySelector(".todo-form");
  const input = doc.querySelector(".todo-input");
  const list = doc.querySelector(".todo-list");
  const countEl = doc.querySelector(".count");

  // Helper: submit the form with a value
  function addTodo(text) {
    input.value = text;
    const event = new dom.window.Event("submit", { bubbles: true, cancelable: true });
    form.dispatchEvent(event);
  }

  // Test 1: Can add an item
  addTodo("Buy groceries");
  const items = list.querySelectorAll("li");
  check(items.length === 1, "adding a todo creates an <li> in the list");

  // Test 2: Item has correct text
  const firstItem = list.querySelector("li");
  const hasText = firstItem && firstItem.textContent.includes("Buy groceries");
  check(hasText, "todo item contains the entered text");

  // Test 3: Can toggle complete
  if (firstItem) {
    firstItem.click();
    check(firstItem.classList.contains("done"), "clicking a todo toggles the .done class");
  } else {
    check(false, "clicking a todo toggles the .done class");
  }

  // Test 4: Can delete an item
  addTodo("Walk the dog");
  const deleteBtn = list.querySelector(".delete-btn");
  if (deleteBtn) {
    deleteBtn.click();
    const remaining = list.querySelectorAll("li").length;
    check(remaining === 1, "clicking delete button removes the todo");
  } else {
    check(false, "clicking delete button removes the todo");
  }

  // Test 5: Count updates
  const countText = countEl ? countEl.textContent : "";
  check(/\d+\s*item/.test(countText), "count element shows remaining item count");

  // Test 6: Empty input doesn't add
  const beforeCount = list.querySelectorAll("li").length;
  addTodo("");
  addTodo("   ");
  const afterCount = list.querySelectorAll("li").length;
  check(afterCount === beforeCount, "empty or whitespace input does not add a todo");

  // TAP output
  console.log(`1..${count}`);
  for (const r of results) {
    console.log(`${r.ok ? "ok" : "not ok"} ${r.index} - ${r.description}`);
  }

  process.exit(results.some((r) => !r.ok) ? 1 : 0);
}

test().catch((err) => {
  console.error("Bail out!", err.message);
  process.exit(1);
});
