---
title: TypeScript for real
---

# Generics

{{show: generics-code typewriter 2s linear}} Generics let you write functions and types that work with any type while preserving type safety.

{{focus: problem}} {{zoom: 1.2x}} {{pulse: problem}} {{annotate: problem "The problem"}} You write a function that wraps any value in an array. `function wrapInArray(value) { return [value]; }`. The return type should match the input. If you pass a string, you get `string[]`. If you pass a number, `number[]`. Without generics, you'd need one function per type — or lose type safety with `any`.

{{zoom: 1x}} {{focus: syntax}} {{zoom: 1.2x}} {{pulse: syntax}} {{annotate: syntax "Type parameter"}} `function wrapInArray<T>(value: T): T[]` — the angle bracket `<T>` declares a type parameter. `T` is a placeholder. When you call `wrapInArray("hello")`, T becomes `string`. The return type becomes `string[]`. The compiler tracks it end to end.

{{zoom: 1x}} {{focus: constraint}} {{zoom: 1.2x}} {{annotate: constraint "Constraints"}} `<T extends { id: number }>` constrains T — it must have an `id` property. `<T extends string | number>` constrains T to strings or numbers. Constraints narrow what generics accept while keeping them flexible.

{{zoom: 1x}} {{focus: builtin}} {{zoom: 1.2x}} {{pulse: builtin}} {{annotate: builtin "Built-in generics"}} `Array<string>` is `string[]`. `Promise<User>` is a promise that resolves to a User. `Map<string, number>` is a map from strings to numbers. Generics are everywhere in TypeScript's built-in types.

{{zoom: 1x}} {{focus: none}}

```code:generics-code lang=typescript
// The problem: need type safety for any type
// function wrapInArray(value: any): any[] ← loses types

// Generic solution: T is a placeholder
function wrapInArray<T>(value: T): T[] {
  return [value];
}
const strings = wrapInArray("hello"); // string[]
const numbers = wrapInArray(42);      // number[]

// Constraint: T must have an id
function getById<T extends { id: number }>(
  items: T[],
  id: number,
): T | undefined {
  return items.find((item) => item.id === id);
}

// Generic interfaces
interface ApiResponse<T> {
  data: T;
  status: number;
  timestamp: string;
}

type UserResponse = ApiResponse<User>;
type PostResponse = ApiResponse<Post[]>;

// Built-in generics
const cache = new Map<string, User>();
const pending: Promise<User[]> = fetchUsers();
---
problem: 2
syntax: 5-9
constraint: 12-17
builtin: 29-30
```

# Mapped and utility types

{{clear: slide}}

{{show: mapped-code typewriter 2s linear}} TypeScript ships utility types that transform existing types. No boilerplate — derive what you need.

{{focus: partial}} {{zoom: 1.2x}} {{pulse: partial}} {{annotate: partial "Partial"}} `Partial<User>` makes every property optional. A user update doesn't require all fields — just the ones being changed. One utility type replaces a separate `UserUpdate` interface.

{{zoom: 1x}} {{focus: required}} {{zoom: 1.2x}} {{annotate: required "Required"}} `Required<User>` makes every property required — the opposite of Partial. `Readonly<User>` makes every property readonly. These compose: `Readonly<Partial<User>>` is a readonly partial user.

{{zoom: 1x}} {{focus: pick-omit}} {{zoom: 1.2x}} {{pulse: pick-omit}} {{annotate: pick-omit "Pick and Omit"}} `Pick<User, "name" | "email">` extracts specific properties. `Omit<User, "password">` removes specific properties. Build API response types by picking what to expose. Build form types by omitting what's auto-generated.

{{zoom: 1x}} {{focus: record}} {{zoom: 1.2x}} {{annotate: record "Record"}} `Record<string, number>` is an object where every key is a string and every value is a number. `Record<Status, string>` maps each status to a string message. Use Record for lookup tables and dictionaries.

{{zoom: 1x}} {{focus: none}}

```code:mapped-code lang=typescript
interface User {
  readonly id: number;
  name: string;
  email: string;
  role: "admin" | "editor" | "viewer";
}

// Partial: all properties optional
type UserUpdate = Partial<User>;
// { id?: number; name?: string; email?: string; role?: ... }

// Pick: extract specific properties
type UserPreview = Pick<User, "name" | "email">;
// { name: string; email: string }

// Omit: remove specific properties
type CreateUser = Omit<User, "id">;
// { name: string; email: string; role: ... }

// Record: typed lookup table
type StatusMessage = Record<User["role"], string>;
// { admin: string; editor: string; viewer: string }

// Composing utility types
type ReadonlyUser = Readonly<User>;
type PublicUser = Omit<Readonly<User>, "role">;

// Extract and Exclude (for unions)
type NonAdmin = Exclude<User["role"], "admin">;
// "editor" | "viewer"
---
partial: 9-10
pick-omit: 13-18
record: 21-22
required: 25-26
```

# Template literal types

{{clear: slide}}

{{show: template-code typewriter 2s linear}} Template literal types combine string types at the type level. The same backtick syntax you use for runtime strings, but for types.

{{focus: basic}} {{zoom: 1.2x}} {{pulse: basic}} {{annotate: basic "String construction"}} `` type EventName = `on${string}` `` matches any string starting with "on": "onClick", "onHover", "onSubmit". `` type Color = `${Channel}-${Shade}` `` where Channel and Shade are unions — the compiler generates every combination.

{{zoom: 1x}} {{focus: combinatorial}} {{zoom: 1.2x}} {{pulse: combinatorial}} {{annotate: combinatorial "Exhaustive combinations"}} Given `type Dir = "top" | "right" | "bottom" | "left"` and `` type Prop = `margin-${Dir}` ``, the result is `"margin-top" | "margin-right" | "margin-bottom" | "margin-left"`. The compiler expands the cartesian product. Every valid combination, no invalid ones.

{{zoom: 1x}} {{focus: satisfies}} {{zoom: 1.2x}} {{annotate: satisfies "satisfies operator"}} `satisfies` checks a value's type without widening it. `const config = { port: 3000 } satisfies Config` — the compiler checks it matches Config, but the inferred type keeps `port` as the literal `3000`, not just `number`. Best of both worlds: validation and narrow inference.

{{zoom: 1x}} {{focus: none}}

```code:template-code lang=typescript
// Template literal types: type-level string construction
type EventName = `on${string}`;
// Matches: "onClick", "onHover", "onLoad", etc.

// Combinatorial types from unions
type Direction = "top" | "right" | "bottom" | "left";
type MarginProp = `margin-${Direction}`;
// "margin-top" | "margin-right" | "margin-bottom" | "margin-left"

type Size = "sm" | "md" | "lg";
type Breakpoint = `${Size}:${string}`;
// "sm:..." | "md:..." | "lg:..."

// const assertion: narrow to literal types
const routes = ["home", "about", "blog"] as const;
type Route = (typeof routes)[number];
// "home" | "about" | "blog"

// satisfies: validate without widening
const config = {
  port: 3000,
  host: "localhost",
} satisfies Record<string, string | number>;
// config.port is 3000 (literal), not number
---
basic: 2-3
combinatorial: 6-12
satisfies: 20-24
```

# Make invalid states unrepresentable

{{clear: slide}}

{{show: invalid-code typewriter 2s linear}} The crowning pattern. Model your domain so the type system prevents bugs. Not catches them — prevents them from being expressible.

{{focus: form-state}} {{zoom: 1.2x}} {{pulse: form-state}} {{annotate: form-state "Form state machine"}} A form is idle, submitting, succeeded, or failed. Each state carries different data. Idle has no results. Submitting has no results. Succeeded has data. Failed has an error. A discriminated union with a `kind` field makes each state a distinct type.

{{zoom: 1x}} {{focus: exhaustive}} {{zoom: 1.2x}} {{pulse: exhaustive}} {{annotate: exhaustive "Exhaustive check"}} The `never` type catches unhandled cases. If you add a new state to the union and forget to handle it, the `default: assertNever(state)` line fails to compile. The compiler forces you to handle every case. Nothing slips through.

{{zoom: 1x}} {{focus: derive}} {{zoom: 1.2x}} {{annotate: derive "Derived types"}} Extract types from your unions. `Extract<FormState, { kind: "success" }>` gives just the success variant. `FormState["kind"]` gives the union of all kind values. Derive what you need — don't duplicate definitions.

{{zoom: 1x}} {{focus: none}} This is the pattern that separates TypeScript users from TypeScript thinkers. Model the domain. Let the compiler enforce it.

```code:invalid-code lang=typescript
// Form state machine: each state has unique data
type FormState =
  | { readonly kind: "idle" }
  | { readonly kind: "submitting"; readonly attempt: number }
  | { readonly kind: "success"; readonly data: Response }
  | { readonly kind: "error"; readonly message: string; readonly retry: boolean };

// Exhaustive handler: compiler ensures all cases covered
function render(state: FormState): string {
  switch (state.kind) {
    case "idle":
      return "Fill out the form";
    case "submitting":
      return `Submitting (attempt ${state.attempt})...`;
    case "success":
      return `Done! ${state.data.id}`;
    case "error":
      return state.retry ? "Failed. Try again?" : state.message;
    default: {
      const _exhaustive: never = state;
      return _exhaustive; // compile error if a case is missing
    }
  }
}

// Derived types: extract from the union
type FormKind = FormState["kind"];
// "idle" | "submitting" | "success" | "error"

type SuccessState = Extract<FormState, { kind: "success" }>;
// { readonly kind: "success"; readonly data: Response }

// Type-safe state transitions
function transition(state: FormState): FormState {
  switch (state.kind) {
    case "idle":
      return { kind: "submitting", attempt: 1 };
    case "submitting":
      return { kind: "success", data: { id: "abc" } };
    case "error":
      return state.retry
        ? { kind: "submitting", attempt: 1 }
        : state;
    case "success":
      return state; // already done
  }
}
---
form-state: 2-6
exhaustive: 9-23
derive: 26-30
```
