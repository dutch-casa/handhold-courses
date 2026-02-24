import { useState } from "react";
import "./form.css";

// TODO: Build a registration form with:
// - Name input with label (htmlFor + id)
// - Email input with label (type="email")
// - Password input with label + strength indicator
// - Confirm password input with label
//
// Validation:
// - Track touched fields (blur sets touched)
// - Only show errors for touched fields
// - On submit, touch all fields and validate
// - Error <p> elements use: id="fieldname-error", role="alert", aria-describedby on input
//
// Loading:
// - isPending state, set true during submit
// - <fieldset disabled={isPending}> wraps all inputs
// - Button text changes: "Create account" / "Creating..."
//
// Password strength:
// - Check length >= 8, has uppercase, has digit
// - Display strength text (weak/medium/strong)

export function RegistrationForm() {
  return (
    <form>
      {/* Build accessible form here */}
      <button type="submit">Create account</button>
    </form>
  );
}
