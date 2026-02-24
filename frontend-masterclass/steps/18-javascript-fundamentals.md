---
title: JavaScript fundamentals
---

# Variables and constants

{{show: vars-code typewriter 2s linear}} JavaScript has three ways to declare a variable. Only two of them are worth using.

{{focus: const-decl}} {{zoom: 1.2x}} {{pulse: const-decl}} {{annotate: const-decl "const: can't reassign"}} `const` declares a binding that can't be reassigned. Once `const name = "Alice"` is set, `name = "Bob"` is an error. Use `const` by default. Most values never need to change.

{{zoom: 1x}} {{focus: let-decl}} {{zoom: 1.2x}} {{annotate: let-decl "let: can reassign"}} `let` declares a binding that can be reassigned. `let count = 0; count = count + 1;` works. Use `let` only when the value will change — loop counters, accumulators, state that evolves.

{{zoom: 1x}} {{focus: var-decl}} {{zoom: 1.2x}} {{pulse: var-decl}} {{annotate: var-decl "var: never use"}} `var` is the old way. It has function scope instead of block scope, hoists to the top of the function, and allows redeclaration. Every JavaScript style guide says the same thing: never use `var`. Use `const` and `let`.

{{zoom: 1x}} {{focus: none}} The rule is simple: `const` unless you need to reassign. Then `let`. Never `var`.

```code:vars-code lang=javascript
// const: can't be reassigned
const name = "Alice";
const age = 30;
const colors = ["red", "green", "blue"];
// name = "Bob"; // TypeError: Assignment to constant

// let: can be reassigned
let count = 0;
count = count + 1;
let message = "hello";
message = "goodbye";

// var: NEVER use this
// var x = 10;
// It has weird scoping rules and causes bugs.
// Pretend it doesn't exist.
---
const-decl: 2-5
let-decl: 8-11
var-decl: 13-16
```

# Primitive types

{{clear: slide}}

{{show: types-tree reveal 0.3s spring}} {{show: types-code typewriter 2s linear}} JavaScript has seven primitive types. Everything that isn't a primitive is an object.

{{focus: string-type}} {{zoom: 1.2x}} {{pulse: string-type}} {{annotate: string-type "Strings"}} Text. Single quotes, double quotes, or backticks. Backticks enable template literals: `` `Hello ${name}` `` embeds expressions. Use backticks when you need interpolation, single or double quotes otherwise.

{{zoom: 1x}} {{focus: number-type}} {{zoom: 1.2x}} {{annotate: number-type "Numbers"}} JavaScript has one number type for integers and decimals. `42`, `3.14`, `-7` are all the same type. No separate int or float. `NaN` (Not a Number) is technically a number type — it's the result of invalid math like `0 / 0`.

{{zoom: 1x}} {{focus: bool-type}} {{zoom: 1.2x}} {{pulse: bool-type}} {{annotate: bool-type "Boolean"}} `true` or `false`. That's it. Used in conditions, comparisons, and toggles. Falsy values: `false`, `0`, `""`, `null`, `undefined`, `NaN`. Everything else is truthy.

{{zoom: 1x}} {{focus: null-undef}} {{zoom: 1.2x}} {{annotate: null-undef "Null and undefined"}} `null` means "intentionally empty." `undefined` means "not yet assigned." A function with no return gives `undefined`. An object property that doesn't exist gives `undefined`. Use `null` when you want to say "nothing here on purpose."

{{zoom: 1x}} {{focus: none}}

```code:types-code lang=javascript
// String
const greeting = "hello";
const name = 'Alice';
const message = `Hello ${name}!`; // template literal

// Number (integers and decimals)
const age = 30;
const pi = 3.14159;
const negative = -7;

// Boolean
const isActive = true;
const isDone = false;

// null: intentionally empty
const result = null;

// undefined: not yet assigned
let value;
console.log(value); // undefined

// typeof checks the type
typeof "hello"   // "string"
typeof 42        // "number"
typeof true      // "boolean"
typeof null      // "object" (a famous JS bug)
typeof undefined // "undefined"
---
string-type: 2-4
number-type: 7-9
bool-type: 12-13
null-undef: 16-20
```

```data:types-tree kind=tree
Types
  Primitives
    string "text"
    number "42, 3.14"
    boolean "true/false"
    null "intentional empty"
    undefined "not assigned"
    symbol "unique key"
    bigint "huge numbers"
  Objects
    Object "{ key: value }"
    Array "[1, 2, 3]"
    Function "() => {}"
    Date "timestamps"
    RegExp "patterns"
---
root: Types
```

# Arrays and objects

{{clear: slide}}

{{show: arrobj-code typewriter 2s linear}} Arrays hold ordered lists. Objects hold named properties. These two structures store almost all data in JavaScript.

{{focus: array}} {{zoom: 1.2x}} {{pulse: array}} {{annotate: array "Arrays"}} Square brackets. Zero-indexed. `colors[0]` is the first element. `.length` gives the count. `.push()` adds to the end. `.pop()` removes from the end. Arrays are objects — they have methods and can hold any type.

{{zoom: 1x}} {{focus: object}} {{zoom: 1.2x}} {{pulse: object}} {{annotate: object "Objects"}} Curly braces. Key-value pairs. `user.name` or `user["name"]` reads a property. Add new properties by assigning: `user.role = "admin"`. Objects are the fundamental data container.

{{zoom: 1x}} {{focus: destructure}} {{zoom: 1.2x}} {{pulse: destructure}} {{annotate: destructure "Destructuring"}} Extract values into variables in one step. `const { name, age } = user` pulls name and age from the object. `const [first, second] = colors` pulls the first two elements from the array. Destructuring is everywhere in modern JavaScript.

{{zoom: 1x}} {{focus: spread}} {{zoom: 1.2x}} {{annotate: spread "Spread and rest"}} Three dots. `...arr` spreads an array into individual elements. `{ ...obj, role: "admin" }` copies an object and adds a property. In function parameters, `...args` collects remaining arguments into an array. Spread copies. Rest collects.

{{zoom: 1x}} {{focus: none}}

```code:arrobj-code lang=javascript
// Arrays: ordered lists
const colors = ["red", "green", "blue"];
colors[0];       // "red"
colors.length;   // 3
colors.push("yellow"); // add to end

// Objects: named properties
const user = { name: "Alice", age: 30 };
user.name;       // "Alice"
user["age"];     // 30
user.role = "admin"; // add property

// Destructuring: extract in one step
const { name, age } = user;
const [first, ...rest] = colors;
// first = "red", rest = ["green", "blue", "yellow"]

// Spread: copy and extend
const updated = { ...user, age: 31 };
const combined = [...colors, "purple"];
---
array: 2-5
object: 8-11
destructure: 14-16
spread: 19-20
```

# Functions

{{clear: slide}}

{{show: func-code typewriter 2s linear}} Functions are values. You can assign them to variables, pass them to other functions, and return them from functions.

{{focus: declaration}} {{zoom: 1.2x}} {{pulse: declaration}} {{annotate: declaration "Declaration"}} `function greet(name) { return ... }` — the classic syntax. Hoisted: you can call it before the line where it's defined. Use this for top-level named functions.

{{zoom: 1x}} {{focus: arrow}} {{zoom: 1.2x}} {{pulse: arrow}} {{annotate: arrow "Arrow functions"}} `const greet = (name) => ...` — shorter syntax. Not hoisted. For single expressions, skip the braces and the return: `(x) => x * 2`. Arrow functions are the default choice for callbacks and inline functions.

{{zoom: 1x}} {{focus: params}} {{zoom: 1.2x}} {{annotate: params "Parameters"}} Default parameters: `(name = "World")`. Destructured parameters: `({ name, age })`. Rest parameters: `(...items)`. These features make function signatures self-documenting.

{{zoom: 1x}} {{focus: higher-order}} {{zoom: 1.2x}} {{pulse: higher-order}} {{annotate: higher-order "Functions as values"}} Pass a function to another function. `[1, 2, 3].map((n) => n * 2)` passes an arrow function to `.map()`. This is called a callback. Higher-order functions — functions that accept or return functions — are the backbone of JavaScript.

{{zoom: 1x}} {{focus: none}}

```code:func-code lang=javascript
// Function declaration (hoisted)
function greet(name) {
  return `Hello, ${name}!`;
}

// Arrow function (not hoisted)
const add = (a, b) => a + b;

const multiply = (a, b) => {
  const result = a * b;
  return result;
};

// Default and destructured parameters
const createUser = (name, role = "viewer") => ({ name, role });
const displayUser = ({ name, age }) => `${name} is ${age}`;

// Functions as values (higher-order)
const numbers = [1, 2, 3, 4, 5];
const doubled = numbers.map((n) => n * 2);
const evens = numbers.filter((n) => n % 2 === 0);
const sum = numbers.reduce((total, n) => total + n, 0);
---
declaration: 2-4
arrow: 7-12
params: 15-16
higher-order: 19-22
```

# Array methods

{{clear: slide}}

{{show: array-methods typewriter 2s linear}} Four methods handle most data transformations. They all take a callback and return a new value — they don't mutate the original array.

{{focus: map}} {{zoom: 1.2x}} {{pulse: map}} {{annotate: map "map: transform each"}} `.map()` transforms every element. The callback receives each item and returns the transformed version. `[1, 2, 3].map(n => n * 2)` gives `[2, 4, 6]`. Same length, different values.

{{zoom: 1x}} {{focus: filter}} {{zoom: 1.2x}} {{pulse: filter}} {{annotate: filter "filter: keep matching"}} `.filter()` keeps elements where the callback returns true. `[1, 2, 3, 4].filter(n => n > 2)` gives `[3, 4]`. Same or fewer elements, same values.

{{zoom: 1x}} {{focus: find}} {{zoom: 1.2x}} {{annotate: find "find: first match"}} `.find()` returns the first element where the callback returns true. `users.find(u => u.name === "Alice")` gives the Alice object — or `undefined` if she's not there. One result, not an array.

{{zoom: 1x}} {{focus: reduce}} {{zoom: 1.2x}} {{pulse: reduce}} {{annotate: reduce "reduce: fold to one"}} `.reduce()` boils an array down to a single value. The callback gets an accumulator and each item. `[1, 2, 3].reduce((sum, n) => sum + n, 0)` gives `6`. The second argument (`0`) is the starting value.

{{zoom: 1x}} {{focus: chaining}} {{zoom: 1.2x}} {{annotate: chaining "Chaining"}} These methods return arrays (except find and reduce), so you can chain them. `.filter().map()` is the bread and butter: filter first, transform second.

{{zoom: 1x}} {{focus: none}}

```code:array-methods lang=javascript
const users = [
  { name: "Alice", age: 30, role: "admin" },
  { name: "Bob", age: 25, role: "viewer" },
  { name: "Carol", age: 35, role: "editor" },
];

// map: transform each element
const names = users.map((u) => u.name);
// ["Alice", "Bob", "Carol"]

// filter: keep elements that pass the test
const adults = users.filter((u) => u.age >= 30);
// [{ name: "Alice"... }, { name: "Carol"... }]

// find: first match (or undefined)
const admin = users.find((u) => u.role === "admin");
// { name: "Alice", age: 30, role: "admin" }

// reduce: fold into a single value
const totalAge = users.reduce((sum, u) => sum + u.age, 0);
// 90

// Chaining: filter then map
const adultNames = users
  .filter((u) => u.age >= 30)
  .map((u) => u.name);
// ["Alice", "Carol"]
---
map: 8-9
filter: 12-13
find: 16-17
reduce: 20-21
chaining: 24-26
```

# Control flow

{{clear: slide}}

{{show: control-code typewriter 1.5s linear}} Conditionals and loops. JavaScript has several, but you need three patterns.

{{focus: if-else}} {{zoom: 1.2x}} {{pulse: if-else}} {{annotate: if-else "if / else"}} Guard clause pattern: check the negative case first, return early. Don't nest. `if (!user) return null;` — the rest of the function assumes `user` exists. No `else` needed after a return.

{{zoom: 1x}} {{focus: ternary}} {{zoom: 1.2x}} {{annotate: ternary "Ternary"}} `condition ? valueIfTrue : valueIfFalse`. One line. Use it for simple value choices: `const label = count === 1 ? "item" : "items"`. Don't nest ternaries — if the logic needs more than one condition, use if/else.

{{zoom: 1x}} {{focus: for-of}} {{zoom: 1.2x}} {{pulse: for-of}} {{annotate: for-of "for...of"}} `for (const item of array)` iterates over values. Cleaner than `for (let i = 0; i < array.length; i++)`. But in practice, `.map()`, `.filter()`, and `.forEach()` replace most for loops. Use `for...of` when you need `break` or `continue`.

{{zoom: 1x}} {{focus: none}}

```code:control-code lang=javascript
// Guard clause: check negative, return early
function getDisplayName(user) {
  if (!user) return "Anonymous";
  if (!user.name) return "Unknown";
  return user.name;
}

// Ternary: simple value choice
const label = count === 1 ? "item" : "items";
const greeting = isLoggedIn ? `Hi, ${name}` : "Sign in";

// for...of: iterate values
for (const color of colors) {
  console.log(color);
}

// for...of with entries (index + value)
for (const [index, color] of colors.entries()) {
  console.log(`${index}: ${color}`);
}

// Prefer array methods over loops:
// colors.forEach((c) => console.log(c));
// colors.map((c) => c.toUpperCase());
---
if-else: 2-6
ternary: 9-10
for-of: 13-20
```
