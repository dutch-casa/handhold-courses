# Build a contact form

Create a contact form with proper labels, input types, and validation.

## Tasks

- [ ] Add a `<form>` with `action="/contact"` and `method="POST"`
- [ ] Add a labeled text input for the user's name (required)
- [ ] Add a labeled email input with `type="email"` (required)
- [ ] Add a labeled `<textarea>` for the message
- [ ] Add a `<select>` dropdown for the subject with at least 3 options
- [ ] Add a submit button with the text "Send Message"

## Hints

Every `<input>` and `<textarea>` needs a `<label>`. Link them with `for` and `id` attributes, or wrap the input inside the label.

Use `type="email"` for the email field — it gives you free validation and the right mobile keyboard.

The `required` attribute prevents empty submissions. Add it to any field that must have a value.

Use `Run Tests` in the testing panel to check your progress.
