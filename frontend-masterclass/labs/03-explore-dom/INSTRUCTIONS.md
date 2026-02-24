# Explore the DOM

Write JavaScript to find and modify elements on a page.

## Tasks

- [ ] Find the element with `id="title"` and store its `textContent` in the variable `titleText`
- [ ] Count all `<p>` elements on the page and store the count in the variable `paragraphCount`
- [ ] Add the CSS class `"highlight"` to the element with `id="featured"`
- [ ] Create a new `<p>` element with the text "Added by JavaScript" and append it to the `<main>` element

## Hints

Use `document.getElementById("title")` or `document.querySelector("#title")` to find by ID.

Use `document.querySelectorAll("p")` to find all paragraphs. The `.length` property gives you the count.

Use `element.classList.add("highlight")` to add a CSS class.

Use `document.createElement("p")` to make a new paragraph, then set its `textContent`, then use `appendChild` to add it.

The variables are already declared at the top of `app.js` — fill in the values.

Use `Run Tests` in the testing panel to check your progress.
