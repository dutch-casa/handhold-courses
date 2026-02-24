---
title: Forms that work
---

# Input sizing

{{show: input-code typewriter 2s linear}} {{split}} {{show: input-preview reveal 0.3s spring}} Inputs smaller than 16px trigger auto-zoom on iOS Safari. The browser zooms in so the user can read the tiny text. It doesn't zoom back out. The page is stuck zoomed in until the user manually pinches. One CSS rule prevents this entirely.

{{focus: size}} {{zoom: 1.2x}} {{pulse: size}} {{annotate: size "16px minimum"}} `font-size: 1rem` on inputs. That's 16px at default browser settings. iOS Safari only auto-zooms when the input font is less than 16px. This one line eliminates the most common mobile form frustration.

{{zoom: 1x}} {{focus: padding}} {{zoom: 1.2x}} {{annotate: padding "Generous padding"}} `padding: 0.75rem 1rem` — large enough to tap comfortably on touch. The input should feel like a target, not a wire. Combined with the 16px font, the total height gives a 44px+ tap area.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:input-code lang=css
/* Prevents iOS auto-zoom on focus */
input, textarea, select {
  font-size: 1rem; /* 16px — the magic number */
}

/* Generous padding for touch targets */
.input {
  font-size: 1rem;
  padding: 0.75rem 1rem;
  border: 1px solid #d1d5db;
  border-radius: 8px;
  width: 100%;
}

/* The input itself should be a comfortable tap target */
/* font-size: 1rem (16px) + padding: 0.75rem top/bottom = ~44px height */
---
size: 2-4
padding: 7-13
```

```preview:input-preview
<style>
  .form-demo { display: flex; flex-direction: column; gap: 1rem; padding: 1rem; max-width: 300px; }
  .bad-input { font-size: 12px; padding: 4px 8px; border: 1px solid #d1d5db; border-radius: 4px; width: 100%; }
  .good-input { font-size: 1rem; padding: 0.75rem 1rem; border: 1px solid #d1d5db; border-radius: 8px; width: 100%; }
  .label { font-size: 0.75rem; color: #64748b; }
</style>
<div class="form-demo">
  <div>
    <div class="label">12px — triggers iOS zoom:</div>
    <input class="bad-input" placeholder="Tiny text, big problem" />
  </div>
  <div>
    <div class="label">16px — no zoom, comfortable:</div>
    <input class="good-input" placeholder="Proper size" />
  </div>
</div>
```

# Labels and association

{{clear: slide}}

{{show: label-code typewriter 2s linear}} Every input needs a label. Not a placeholder — a label. Placeholders disappear when you type. A user looking at a filled form has no idea what each field is. Labels persist. And screen readers announce them.

{{focus: explicit}} {{zoom: 1.2x}} {{pulse: explicit}} {{annotate: explicit "Explicit: for + id"}} `<label htmlFor="email">` links to `<input id="email">`. Click the label? The input focuses. Screen reader enters the field? It reads "Email." The `for`/`id` link is the accessibility bridge.

{{zoom: 1x}} {{focus: implicit}} {{zoom: 1.2x}} {{annotate: implicit "Implicit: wrapping"}} Wrap the input inside the label: `<label>Email <input /></label>`. No `for`/`id` needed — the nesting creates the association. Both methods work. Pick one and be consistent.

{{zoom: 1x}} {{focus: placeholder}} {{zoom: 1.2x}} {{pulse: placeholder}} {{pan: placeholder}} {{annotate: placeholder "Placeholder is not a label"}} Placeholders are hints, not labels. `placeholder="Enter your email"` disappears the moment you type. If a user pauses and looks at the form, they see filled inputs with no context. Placeholders supplement labels — they never replace them.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:label-code lang=tsx
// Explicit association: htmlFor + id
<div>
  <label htmlFor="email">Email</label>
  <input id="email" type="email" placeholder="you@example.com" />
</div>

// Implicit association: wrapping
<label>
  Email
  <input type="email" placeholder="you@example.com" />
</label>

// BAD: placeholder as label
<input placeholder="Email" /> {/* Screen reader says: "edit text" */}

// GOOD: label + placeholder as hint
<label htmlFor="email">Email</label>
<input id="email" placeholder="you@example.com" />
{/* Screen reader says: "Email, edit text" */}
---
explicit: 2-5
implicit: 8-11
placeholder: 14-19
```

# Error states

{{clear: slide}}

{{show: error-code typewriter 2s linear}} Validation errors must be visible, associated with the right input, and announced to screen readers. A red border alone doesn't cut it. Color-blind users miss it. Screen readers ignore it.

{{focus: aria-invalid}} {{zoom: 1.2x}} {{pulse: aria-invalid}} {{annotate: aria-invalid "aria-invalid"}} `aria-invalid="true"` on the input tells assistive tech this field has an error. Combine with a visual indicator — red border, error icon. The visual and semantic signals must match.

{{zoom: 1x}} {{focus: described}} {{zoom: 1.2x}} {{annotate: described "aria-describedby"}} `aria-describedby="email-error"` links the input to its error message. When the screen reader focuses the input, it reads the label, then the error. The user knows what's wrong without hunting.

{{zoom: 1x}} {{focus: timing}} {{zoom: 1.2x}} {{pulse: timing}} {{pan: timing}} {{annotate: timing "Validate on blur"}} Validate on blur (when the user leaves the field), not on every keystroke. Keystroke validation is hostile — showing "invalid email" after typing "j". On blur, the user has finished their attempt. On submit, validate everything. Never mid-keystroke.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:error-code lang=tsx
interface FieldProps {
  readonly label: string;
  readonly error: string | undefined;
  readonly id: string;
}

function Field({ label, error, id, ...inputProps }: FieldProps & React.InputHTMLAttributes<HTMLInputElement>) {
  const errorId = `${id}-error`;

  return (
    <div>
      <label htmlFor={id}>{label}</label>
      <input
        id={id}
        aria-invalid={error !== undefined}
        aria-describedby={error ? errorId : undefined}
        {...inputProps}
      />
      {error && (
        <p id={errorId} role="alert" className="error">
          {error}
        </p>
      )}
    </div>
  );
}

// Usage with blur validation
function EmailField() {
  const [value, setValue] = useState("");
  const [error, setError] = useState<string | undefined>(undefined);

  function handleBlur() {
    if (!value.includes("@")) {
      setError("Please enter a valid email address");
    } else {
      setError(undefined);
    }
  }

  return (
    <Field
      id="email"
      label="Email"
      type="email"
      value={value}
      onChange={(e) => setValue(e.target.value)}
      onBlur={handleBlur}
      error={error}
    />
  );
}
---
aria-invalid: 14
described: 15
timing: 31-37
```

# Submit and loading

{{clear: slide}}

{{show: submit-code typewriter 2s linear}} Forms submit with Enter. Always. A user filling in a single-field search form should press Enter and it should work. Multi-field forms submit with Enter in the last field or with a submit button. Keyboard-driven submission is the baseline.

{{focus: enter}} {{zoom: 1.2x}} {{pulse: enter}} {{annotate: enter "Enter submits"}} `<form onSubmit={handleSubmit}>` — the form handles submission, not the button. Pressing Enter in any input triggers `onSubmit`. The button is `type="submit"`. No JavaScript is needed to wire Enter — the form does it.

{{zoom: 1x}} {{focus: loading}} {{zoom: 1.2x}} {{annotate: loading "Loading state"}} While submitting: disable the button, show a spinner or text change. `disabled={isPending}` prevents double-submit. `{isPending ? "Submitting..." : "Submit"}` gives visual feedback. The form becomes read-only during submission.

{{zoom: 1x}} {{focus: fieldset}} {{zoom: 1.2x}} {{pulse: fieldset}} {{pan: fieldset}} {{annotate: fieldset "Disable all fields"}} `<fieldset disabled={isPending}>` disables every input and button inside it. One attribute, entire form locked. Better than disabling each input individually. The native `<fieldset>` element does the work.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:submit-code lang=tsx
function RegistrationForm() {
  const [isPending, setIsPending] = useState(false);

  async function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    setIsPending(true);
    try {
      const form = e.currentTarget as HTMLFormElement;
      const data = new FormData(form);
      await submitRegistration(Object.fromEntries(data));
    } finally {
      setIsPending(false);
    }
  }

  return (
    <form onSubmit={handleSubmit}>
      <fieldset disabled={isPending}>
        <Field id="name" label="Name" name="name" required />
        <Field id="email" label="Email" type="email" name="email" required />
        <Field id="password" label="Password" type="password" name="password" required />

        <button type="submit">
          {isPending ? "Creating account..." : "Create account"}
        </button>
      </fieldset>
    </form>
  );
}

// Cmd+Enter for multiline (textarea) forms
function CommentForm() {
  function handleKeyDown(e: React.KeyboardEvent) {
    if (e.key === "Enter" && (e.metaKey || e.ctrlKey)) {
      e.currentTarget.closest("form")?.requestSubmit();
    }
  }

  return (
    <form onSubmit={handleSubmit}>
      <textarea name="comment" onKeyDown={handleKeyDown} rows={4} />
      <button type="submit">Post</button>
    </form>
  );
}
---
enter: 4-14
loading: 23-25
fieldset: 18-27
```

# Validation patterns

{{clear: slide}}

{{show: validation-code typewriter 2s linear}} The validation flow: nothing on mount, validate on blur, re-validate on change after first blur, validate all on submit. The pattern respects the user's flow — don't interrupt typing, don't stay silent forever.

{{focus: flow}} {{zoom: 1.2x}} {{pulse: flow}} {{annotate: flow "The flow"}} Mount: no errors shown. User types: no validation. User leaves field (blur): validate that field. If error, show it. User corrects and types: re-validate on change (now they're actively fixing). Submit: validate everything at once.

{{zoom: 1x}} {{focus: touched}} {{zoom: 1.2x}} {{annotate: touched "Touched tracking"}} Track which fields have been "touched" (blurred at least once). Only show errors for touched fields. A field the user hasn't reached yet shouldn't scream at them.

{{zoom: 1x}} {{focus: announce}} {{zoom: 1.2x}} {{pulse: announce}} {{pan: announce}} {{annotate: announce "Announce errors"}} `role="alert"` on the error message announces it to screen readers when it appears. `aria-live="polite"` waits for a pause in screen reader speech. The user hears "Please enter a valid email" without having to navigate to the error.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:validation-code lang=tsx
type FieldName = "name" | "email" | "password";

function useFormValidation() {
  const [values, setValues] = useState<Record<FieldName, string>>({
    name: "", email: "", password: "",
  });
  const [touched, setTouched] = useState<Record<FieldName, boolean>>({
    name: false, email: false, password: false,
  });

  function validate(field: FieldName, value: string): string | undefined {
    switch (field) {
      case "name": return value.length < 2 ? "Name must be at least 2 characters" : undefined;
      case "email": return !value.includes("@") ? "Enter a valid email" : undefined;
      case "password": return value.length < 8 ? "Password must be at least 8 characters" : undefined;
    }
  }

  function getError(field: FieldName): string | undefined {
    if (!touched[field]) return undefined;
    return validate(field, values[field]);
  }

  function handleBlur(field: FieldName) {
    setTouched((prev) => ({ ...prev, [field]: true }));
  }

  function handleChange(field: FieldName, value: string) {
    setValues((prev) => ({ ...prev, [field]: value }));
  }

  function validateAll(): boolean {
    setTouched({ name: true, email: true, password: true });
    return Object.keys(values).every(
      (k) => validate(k as FieldName, values[k as FieldName]) === undefined,
    );
  }

  return { values, getError, handleBlur, handleChange, validateAll };
}
---
flow: 3-17
touched: 19-22
announce: 19-22
```
