---
title: Why TypeScript?
---

# The runtime error problem

{{split}} {{show: ts-problem-code typewriter 2s linear}} {{show: ts-solution-code reveal 0.5s spring}} JavaScript is dynamically typed. You find type errors when the code runs — in production, in front of users, at 2am.

{{focus: js-bug}} {{zoom: 1.2x}} {{pulse: js-bug}} {{annotate: js-bug "Runtime explosion"}} `user.name.toUpperCase()` crashes if `user` is null. `items.filter(...)` crashes if `items` is a string. These errors don't show up until that exact line runs with that exact data. In production. On a Friday.

{{zoom: 1x}} {{focus: ts-catch}} {{zoom: 1.2x}} {{pulse: ts-catch}} {{annotate: ts-catch "Caught at compile time"}} The same code in TypeScript shows a red underline before you even save the file. `user` might be null — you must check first. `items` is a string, not an array — you can't call `.filter()` on it. The error moves from runtime to development time.

{{zoom: 1x}} {{focus: none}} {{unsplit}} TypeScript doesn't add types to JavaScript. It reveals the types that were always there.

```code:ts-problem-code lang=javascript
// JavaScript: these bugs hide until runtime

function getDisplayName(user) {
  return user.name.toUpperCase();
  // 💥 TypeError if user is null
}

function filterActive(items) {
  return items.filter((i) => i.active);
  // 💥 TypeError if items is not an array
}

// These work fine in development.
// They crash in production when
// user is null or items is a string.
---
js-bug: 3-10
```

```code:ts-solution-code lang=typescript
// TypeScript: caught before you run it

function getDisplayName(user: User | null): string {
  if (!user) return "Anonymous";
  //  ↑ Must check — TS enforces it
  return user.name.toUpperCase();
}

function filterActive(items: Item[]): Item[] {
  return items.filter((i) => i.active);
  // TS knows items is always an array.
  // Passing a string is a compile error.
}

// Red underline in your editor.
// No deploy. No 2am page.
---
ts-catch: 3-12
```

# What TypeScript actually is

{{clear: slide}}

{{show: ts-pipeline typewriter 2s linear}} TypeScript is a compiler. It reads `.ts` files, checks the types, and outputs plain `.js` files. The types disappear at runtime. Browsers never see TypeScript.

{{focus: input}} {{zoom: 1.2x}} {{pulse: input}} {{annotate: input "Source"}} You write `.ts` files. They look like JavaScript with type annotations: `function add(a: number, b: number): number`. The annotation `: number` says "this must be a number." The code is still JavaScript — the types are metadata.

{{zoom: 1x}} {{focus: compiler}} {{zoom: 1.2x}} {{pulse: compiler}} {{annotate: compiler "Type checker"}} `tsc` (the TypeScript compiler) reads your code and checks that every value matches its declared type. If `add("hello", 5)` appears, the compiler flags it — string is not number. If everything checks out, it emits JavaScript.

{{zoom: 1x}} {{focus: output}} {{zoom: 1.2x}} {{annotate: output "JavaScript output"}} The output is plain JavaScript. `function add(a, b) { return a + b; }` — the types are gone. No runtime overhead. No special library. The browser runs normal JavaScript. TypeScript is a development-time guarantee, not a runtime feature.

{{zoom: 1x}} {{focus: inference}} {{zoom: 1.2x}} {{pulse: inference}} {{annotate: inference "Type inference"}} TypeScript infers types when it can. `const x = 5` — the compiler knows x is a number. You didn't write a type annotation. `const name = "Alice"` — string. `[1, 2, 3].map(n => n * 2)` — the compiler infers n is number and the result is number[]. You write types far less than you'd expect.

{{zoom: 1x}} {{focus: none}}

```code:ts-pipeline lang=typescript
/* Input: TypeScript (.ts) */
function add(a: number, b: number): number {
  return a + b;
}
const result = add(2, 3); // result: number

/* Compiler checks:
   add("hello", 5) → Error: string is not number
   add(2)          → Error: expected 2 arguments
   add(2, 3, 4)    → Error: expected 2 arguments */

/* Output: plain JavaScript (.js) */
// function add(a, b) {
//   return a + b;
// }
// const result = add(2, 3);
// Types are GONE. Zero runtime cost.

/* Type inference — you barely write types */
const x = 5;           // inferred: number
const name = "Alice";  // inferred: string
const nums = [1, 2, 3]; // inferred: number[]
const doubled = nums.map((n) => n * 2); // inferred: number[]
---
input: 2-5
compiler: 7-10
output: 12-17
inference: 20-23
```

# IDE superpowers

{{clear: slide}}

{{show: ide-code typewriter 2s linear}} TypeScript transforms your editor from a text editor into a development environment.

{{focus: autocomplete}} {{zoom: 1.2x}} {{pulse: autocomplete}} {{annotate: autocomplete "Autocomplete"}} Type `user.` and the editor shows every property on the user object. Name, age, email, role — all with their types. No guessing. No documentation lookup. The type system is live documentation.

{{zoom: 1x}} {{focus: refactor}} {{zoom: 1.2x}} {{pulse: refactor}} {{annotate: refactor "Safe refactoring"}} Rename a property with F2. Every usage across every file updates. The compiler checks that nothing broke. Without types, renaming is find-and-replace with crossed fingers.

{{zoom: 1x}} {{focus: go-to-def}} {{zoom: 1.2x}} {{annotate: go-to-def "Go to definition"}} Command-click a function name and you jump to its source. Click a type and you see its definition. Click an imported module and you land in the file. Navigation through code becomes instant.

{{zoom: 1x}} {{focus: errors}} {{zoom: 1.2x}} {{pulse: errors}} {{annotate: errors "Instant errors"}} Mistakes show immediately. Red underlines appear as you type. Hover to see the error message. Fix it before running. The feedback loop drops from "run → crash → read stacktrace → fix" to "type → see underline → fix."

{{zoom: 1x}} {{focus: none}}

```code:ide-code lang=typescript
// Autocomplete: type user. and see all properties
interface User {
  name: string;
  age: number;
  email: string;
  role: "admin" | "editor" | "viewer";
}

function displayUser(user: User) {
  // user.  ← editor shows: name, age, email, role
  console.log(user.name);
}

// Refactoring: rename "name" to "displayName"
// Every usage across every file updates safely

// Go to definition: Cmd+click any symbol
// Jump to where it's defined instantly

// Instant errors: underline appears as you type
displayUser({ name: "Alice" });
// ↑ Error: missing age, email, role
---
autocomplete: 2-7
refactor: 15
go-to-def: 18
errors: 21-22
```
