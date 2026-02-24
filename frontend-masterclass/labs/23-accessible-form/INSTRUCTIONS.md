# Lab 23: Accessible Form

Build a registration form that prevents iOS zoom, announces errors, validates on blur, and handles loading state.

## Tasks

1. **Input sizing** — All inputs must have `font-size: 1rem` (16px minimum) in `form.css` to prevent iOS auto-zoom.

2. **Label association** — Every input has an associated `<label>` using `htmlFor` and `id`. No placeholder-only fields.

3. **Error states** — Invalid inputs get `aria-invalid={true}`. Error messages use `aria-describedby` linking to the error `<p>` by id.

4. **Error announcements** — Error messages use `role="alert"` so screen readers announce them.

5. **Blur validation** — Validate on blur, not on keystroke. Show errors only for fields the user has touched. On submit, validate all fields.

6. **Loading state** — Wrap fields in `<fieldset disabled={isPending}>`. Change submit button text during submission.

7. **Password strength** — Show a strength indicator that updates as the user types (length, uppercase, number checks). This is the one field that validates on change.

## Hints

- `aria-invalid` accepts a boolean: `aria-invalid={!!error}`.
- `aria-describedby` links to the error message's `id`.
- `fieldset[disabled]` disables all children — no need to disable each input.
- Password strength: `value.length >= 8`, `/[A-Z]/.test(value)`, `/\d/.test(value)`.
