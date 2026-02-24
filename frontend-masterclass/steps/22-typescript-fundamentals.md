---
title: TypeScript fundamentals
---

# Primitive types and annotations

{{show: primitives-code typewriter 2s linear}} TypeScript has the same primitive types as JavaScript plus a few extras for type safety.

{{focus: basics}} {{zoom: 1.2x}} {{pulse: basics}} {{annotate: basics "Type annotations"}} A colon after a variable name adds a type annotation. `let age: number = 30`. The compiler enforces this: `age = "hello"` is an error. But you rarely need explicit annotations — TypeScript infers them.

{{zoom: 1x}} {{focus: inference}} {{zoom: 1.2x}} {{pulse: inference}} {{annotate: inference "Inference wins"}} `const name = "Alice"` — TypeScript infers `string`. `const count = 0` — infers `number`. `const items = [1, 2, 3]` — infers `number[]`. Write annotations only when inference can't figure it out: function parameters, complex returns, empty arrays.

{{zoom: 1x}} {{focus: arrays}} {{zoom: 1.2x}} {{annotate: arrays "Arrays and tuples"}} `number[]` is an array of numbers. `string[]` is an array of strings. `[string, number]` is a tuple — exactly two elements, first is string, second is number. Tuples have fixed length and typed positions.

{{zoom: 1x}} {{focus: none}}

```code:primitives-code lang=typescript
// Explicit annotations (rarely needed)
let age: number = 30;
let name: string = "Alice";
let active: boolean = true;

// Type inference (the compiler figures it out)
const count = 0;           // number
const greeting = "hello";  // string (literal "hello")
const items = [1, 2, 3];  // number[]

// Arrays
const names: string[] = ["Alice", "Bob"];
const scores: number[] = [95, 87, 92];

// Tuples: fixed length, typed positions
const pair: [string, number] = ["Alice", 30];
const [pairName, pairAge] = pair;
// pairName: string, pairAge: number
---
basics: 2-4
inference: 7-9
arrays: 12-16
```

# Objects and interfaces

{{clear: slide}}

{{show: objects-code typewriter 2s linear}} Objects need shape definitions. Interfaces describe what properties an object must have.

{{focus: interface}} {{zoom: 1.2x}} {{pulse: interface}} {{annotate: interface "Interface"}} An interface declares the shape of an object. `interface User { name: string; age: number; }`. Any object passed where a `User` is expected must have those properties with those types. Missing properties? Compile error. Extra properties? Compile error.

{{zoom: 1x}} {{focus: optional}} {{zoom: 1.2x}} {{annotate: optional "Optional properties"}} A question mark makes a property optional. `email?: string` means the property can be present (as a string) or absent. When you access it, the type is `string | undefined` — you must check before using it.

{{zoom: 1x}} {{focus: readonly}} {{zoom: 1.2x}} {{pulse: readonly}} {{annotate: readonly "Readonly"}} `readonly` prevents reassignment. `readonly id: number` means once set, the id can't change. This enforces immutability at the type level — the compiler blocks mutations.

{{zoom: 1x}} {{focus: type-alias}} {{zoom: 1.2x}} {{annotate: type-alias "Type aliases"}} `type` creates a name for any type. `type ID = string | number`. Use `type` for unions, intersections, and aliases. Use `interface` for object shapes. Both work — the choice is mostly style.

{{zoom: 1x}} {{focus: none}}

```code:objects-code lang=typescript
// Interface: object shape
interface User {
  readonly id: number;
  name: string;
  age: number;
  email?: string; // optional
}

const user: User = {
  id: 1,
  name: "Alice",
  age: 30,
};
// user.id = 2; // Error: readonly
// user.email → string | undefined

// Type alias: name for any type
type ID = string | number;
type Status = "active" | "inactive" | "pending";

// Extending interfaces
interface Employee extends User {
  role: string;
  department: string;
}

// Intersection types (combine shapes)
type AdminUser = User & { permissions: string[] };
---
interface: 2-7
optional: 6
readonly: 3
type-alias: 18-19
```

# Union types and narrowing

{{clear: slide}}

{{split}} {{show: union-code typewriter 2s linear}} {{show: narrow-code reveal 0.5s spring}} Union types are the backbone of TypeScript. A value can be one of several types. Narrowing determines which one.

{{focus: union}} {{zoom: 1.2x}} {{pulse: union}} {{annotate: union "Union types"}} `string | number` means the value is either a string or a number. `User | null` means either a user object or null. The pipe character combines types. The compiler only lets you do things that work for all members of the union.

{{zoom: 1x}} {{focus: literal}} {{zoom: 1.2x}} {{pulse: literal}} {{annotate: literal "Literal types"}} `"success" | "error" | "loading"` — not just string, but specific strings. `1 | 2 | 3` — not just number, but specific numbers. Literal types are closed sets. The compiler knows every possible value.

{{zoom: 1x}} {{focus: narrow}} {{zoom: 1.2x}} {{pulse: narrow}} {{annotate: narrow "Narrowing"}} `typeof`, `in`, truthiness checks — these narrow the type. After `if (typeof x === "string")`, the compiler knows x is a string inside that block. This is called control flow analysis. The compiler tracks what's possible at every point.

{{zoom: 1x}} {{focus: none}} {{unsplit}}

```code:union-code lang=typescript
// Union types: one of several types
type Result = string | number;
type MaybeUser = User | null;
type Status = "loading" | "success" | "error";

// Literal types: specific values
type Direction = "north" | "south" | "east" | "west";
type DiceRoll = 1 | 2 | 3 | 4 | 5 | 6;

// Only common operations allowed on unions
function format(value: string | number) {
  // value.toUpperCase() → Error!
  // (number doesn't have toUpperCase)
  return String(value); // works on both
}
---
union: 2-4
literal: 7-8
```

```code:narrow-code lang=typescript
// Narrowing with typeof
function format(value: string | number): string {
  if (typeof value === "string") {
    return value.toUpperCase(); // TS knows: string
  }
  return value.toFixed(2);     // TS knows: number
}

// Narrowing with truthiness
function greet(name: string | null): string {
  if (!name) return "Hello, stranger";
  return `Hello, ${name}`; // TS knows: string
}

// Narrowing with "in"
function getArea(shape: Circle | Rectangle) {
  if ("radius" in shape) {
    return Math.PI * shape.radius ** 2; // Circle
  }
  return shape.width * shape.height;    // Rectangle
}

// The unknown type (safe any)
function process(data: unknown) {
  // data.name → Error! (unknown is not typed)
  if (typeof data === "object" && data !== null) {
    // now data is object — still need more checks
  }
}
---
narrow: 2-6
```

# Discriminated unions

{{clear: slide}}

{{show: discrim-code typewriter 2s linear}} The most powerful TypeScript pattern. A shared field tells the compiler which variant it's looking at.

{{focus: tagged}} {{zoom: 1.2x}} {{pulse: tagged}} {{annotate: tagged "Tagged union"}} Each variant has a `kind` (or `type`, or `status`) field with a literal type. `{ kind: "loading" }`, `{ kind: "success", data: User[] }`, `{ kind: "error", message: string }`. The `kind` field is the discriminant — it tags each variant.

{{zoom: 1x}} {{focus: switch}} {{zoom: 1.2x}} {{pulse: switch}} {{annotate: switch "Exhaustive switch"}} Switch on the discriminant. In each case, TypeScript narrows to the specific variant. `case "success"` — the compiler knows `data` exists. `case "error"` — the compiler knows `message` exists. If you add a new variant and forget a case, the compiler tells you.

{{zoom: 1x}} {{focus: impossible}} {{zoom: 1.2x}} {{pulse: impossible}} {{annotate: impossible "Invalid states"}} A loading state has no data. A success state has no error. An error state has no data. These constraints are enforced by the type system — not by convention, not by documentation, by the compiler. Impossible states are unrepresentable.

{{zoom: 1x}} {{focus: none}} This is the "make invalid states unrepresentable" pattern. It's the most important thing TypeScript offers.

```code:discrim-code lang=typescript
// Discriminated union: tagged variants
type RequestState =
  | { readonly kind: "idle" }
  | { readonly kind: "loading" }
  | { readonly kind: "success"; readonly data: User[] }
  | { readonly kind: "error"; readonly message: string };

// Exhaustive switch: compiler checks every case
function renderState(state: RequestState): string {
  switch (state.kind) {
    case "idle":
      return "Ready to load";
    case "loading":
      return "Loading...";
    case "success":
      return `${state.data.length} users`;
      //       ↑ TS knows: data exists here
    case "error":
      return `Error: ${state.message}`;
      //              ↑ TS knows: message exists here
  }
}

// Invalid states are impossible:
// { kind: "loading", data: [...] } → Compile error!
// { kind: "error" }               → Compile error! (missing message)
// { kind: "success" }             → Compile error! (missing data)
---
tagged: 2-6
switch: 9-22
impossible: 25-27
```
