import { useState, type FormEvent } from "react";
import "./form.css";

type FieldName = "name" | "email" | "password" | "confirmPassword";

interface FormData {
  readonly name: string;
  readonly email: string;
  readonly password: string;
  readonly confirmPassword: string;
}

interface TouchedState {
  readonly name: boolean;
  readonly email: boolean;
  readonly password: boolean;
  readonly confirmPassword: boolean;
}

function getPasswordStrength(password: string): "weak" | "medium" | "strong" {
  const hasLength = password.length >= 8;
  const hasUppercase = /[A-Z]/.test(password);
  const hasDigit = /\d/.test(password);

  const score = [hasLength, hasUppercase, hasDigit].filter(Boolean).length;

  if (score <= 1) return "weak";
  if (score === 2) return "medium";
  return "strong";
}

function validate(data: FormData): Partial<Record<FieldName, string>> {
  const errors: Partial<Record<FieldName, string>> = {};

  if (!data.name.trim()) {
    errors.name = "Name is required";
  }

  if (!data.email.trim()) {
    errors.email = "Email is required";
  } else if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(data.email)) {
    errors.email = "Enter a valid email address";
  }

  if (!data.password) {
    errors.password = "Password is required";
  } else if (data.password.length >= 8 === false) {
    errors.password = "Password must be at least 8 characters";
  }

  if (!data.confirmPassword) {
    errors.confirmPassword = "Please confirm your password";
  } else if (data.confirmPassword !== data.password) {
    errors.confirmPassword = "Passwords do not match";
  }

  return errors;
}

export function RegistrationForm() {
  const [formData, setFormData] = useState<FormData>({
    name: "",
    email: "",
    password: "",
    confirmPassword: "",
  });

  const [touched, setTouched] = useState<TouchedState>({
    name: false,
    email: false,
    password: false,
    confirmPassword: false,
  });

  const [isPending, setIsPending] = useState(false);
  const [submitSuccess, setSubmitSuccess] = useState(false);

  const errors = validate(formData);
  const strength = formData.password ? getPasswordStrength(formData.password) : null;

  function handleChange(field: FieldName, value: string) {
    setFormData((prev) => ({ ...prev, [field]: value }));
  }

  function handleBlur(field: FieldName) {
    setTouched((prev) => ({ ...prev, [field]: true }));
  }

  function handleSubmit(e: FormEvent) {
    e.preventDefault();

    const allTouched: TouchedState = {
      name: true,
      email: true,
      password: true,
      confirmPassword: true,
    };
    setTouched(allTouched);

    const currentErrors = validate(formData);
    if (Object.keys(currentErrors).length > 0) return;

    setIsPending(true);
    setTimeout(() => {
      setIsPending(false);
      setSubmitSuccess(true);
    }, 1500);
  }

  function shouldShowError(field: FieldName): boolean {
    return touched[field] && field in errors;
  }

  return (
    <form className="registration-form" onSubmit={handleSubmit} noValidate>
      <h2 className="form-title">Create your account</h2>

      {submitSuccess && (
        <div className="success-banner" role="status">
          Account created successfully!
        </div>
      )}

      <fieldset disabled={isPending} className="form-fieldset">
        <legend className="sr-only">Registration details</legend>

        <div className="form-field">
          <label htmlFor="name" className="form-label">Name</label>
          <input
            id="name"
            type="text"
            className="form-input"
            value={formData.name}
            onChange={(e) => handleChange("name", e.target.value)}
            onBlur={() => handleBlur("name")}
            aria-invalid={shouldShowError("name")}
            aria-describedby={shouldShowError("name") ? "name-error" : undefined}
            autoComplete="name"
          />
          {shouldShowError("name") && (
            <p id="name-error" className="form-error" role="alert">
              {errors.name}
            </p>
          )}
        </div>

        <div className="form-field">
          <label htmlFor="email" className="form-label">Email</label>
          <input
            id="email"
            type="email"
            className="form-input"
            value={formData.email}
            onChange={(e) => handleChange("email", e.target.value)}
            onBlur={() => handleBlur("email")}
            aria-invalid={shouldShowError("email")}
            aria-describedby={shouldShowError("email") ? "email-error" : undefined}
            autoComplete="email"
          />
          {shouldShowError("email") && (
            <p id="email-error" className="form-error" role="alert">
              {errors.email}
            </p>
          )}
        </div>

        <div className="form-field">
          <label htmlFor="password" className="form-label">Password</label>
          <input
            id="password"
            type="password"
            className="form-input"
            value={formData.password}
            onChange={(e) => handleChange("password", e.target.value)}
            onBlur={() => handleBlur("password")}
            aria-invalid={shouldShowError("password")}
            aria-describedby={
              shouldShowError("password")
                ? "password-error"
                : strength
                  ? "password-strength"
                  : undefined
            }
            autoComplete="new-password"
          />
          {strength && (
            <p id="password-strength" className={`password-strength strength-${strength}`}>
              Password strength: <strong>{strength}</strong>
            </p>
          )}
          {shouldShowError("password") && (
            <p id="password-error" className="form-error" role="alert">
              {errors.password}
            </p>
          )}
        </div>

        <div className="form-field">
          <label htmlFor="confirmPassword" className="form-label">Confirm Password</label>
          <input
            id="confirmPassword"
            type="password"
            className="form-input"
            value={formData.confirmPassword}
            onChange={(e) => handleChange("confirmPassword", e.target.value)}
            onBlur={() => handleBlur("confirmPassword")}
            aria-invalid={shouldShowError("confirmPassword")}
            aria-describedby={shouldShowError("confirmPassword") ? "confirmPassword-error" : undefined}
            autoComplete="new-password"
          />
          {shouldShowError("confirmPassword") && (
            <p id="confirmPassword-error" className="form-error" role="alert">
              {errors.confirmPassword}
            </p>
          )}
        </div>

        <button type="submit" className="form-submit">
          {isPending ? "Creating..." : "Create account"}
        </button>
      </fieldset>
    </form>
  );
}
