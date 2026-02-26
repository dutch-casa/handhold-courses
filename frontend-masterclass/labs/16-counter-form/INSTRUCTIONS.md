# Lab 16: Counter and Form

Build two components that use `useState` and event handlers.

## Tasks

1. **Counter component** — `Counter.tsx`. Three buttons: increment (+1), decrement (-1), reset (back to 0). Display the current count in a `<p>`.

2. **Character-counted input** — Inside `Counter.tsx`, add a `<input>` that displays a live character count below it, like `"12 / 100 characters"`.

3. **Contact form** — `ContactForm.tsx`. Controlled inputs for `name` and `email`. A `<button type="submit">` that validates: both fields must be non-empty on submit.

4. **Validation feedback** — If the user submits with an empty field, display `"All fields required"` in a `<p className="error">`. If both are filled, display `"Submitted!"` in a `<p className="success">`.

5. **Use functional updates** for the counter — `setCount(prev => prev + 1)`, not `setCount(count + 1)`.

6. **Derive, don't store** — the character count should be derived from state (`value.length`), not stored in a separate `useState`.

## Hints

- `e.preventDefault()` in `onSubmit` stops the page from reloading.
- For validation, use a `status` state: `"idle" | "error" | "success"`.
- The character count is `text.length` — no `useState` needed for it.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
