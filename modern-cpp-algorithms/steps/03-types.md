---
title: Types and variables
---

# The type system

{{show: type-sizes grow 0.5s spring}} Every value in C++ has a type. The type determines how many bytes it occupies and what operations you can perform on it.

{{focus: int-row}} {{annotate: int-row "4 bytes"}} An int is 4 bytes. It holds whole numbers from roughly negative two billion to positive two billion.

{{focus: double-row}} {{annotate: double-row "8 bytes"}} A double is 8 bytes. Floating-point. Roughly 15 significant digits of precision.

{{focus: char-row}} {{annotate: char-row "1 byte"}} A char is 1 byte. A single character, or a small integer from negative 128 to 127.

{{focus: bool-row}} {{annotate: bool-row "1 byte"}} A bool is 1 byte. True or false. That's it.

{{focus: none}} These sizes matter. When you allocate a million ints, that's four megabytes. Know your sizes.

```data:type-sizes type=array
["int: 4B", "double: 8B", "char: 1B", "bool: 1B", "long long: 8B", "float: 4B"]
---
int-row: 0
double-row: 1
char-row: 2
bool-row: 3
```

# Type safety and auto

{{clear: slide}}

{{show: auto-code typewriter 1.5s linear}} C++ is statically typed. The compiler checks types at compile time, not runtime. No surprises in production.

{{focus: explicit-types}} {{zoom: 1.2x}} {{annotate: explicit-types "Explicit"}} You can write the type yourself. Int x equals 42. Double pi equals 3.14. Clear and unambiguous.

{{zoom: 1x}} {{focus: auto-types}} {{zoom: 1.2x}} {{annotate: auto-types "Deduced"}} Or let the compiler figure it out. Auto tells the compiler: "look at the right side and deduce the type." Auto x equals 42 gives you an int. Auto pi equals 3.14 gives you a double.

{{zoom: 1x}} {{focus: must-init}} {{zoom: 1.3x}} {{annotate: must-init "No default"}} Auto requires an initializer. The compiler needs the right side to deduce the type. auto x with no initializer is a compile error. This is a feature — it prevents uninitialized variables.

{{zoom: 1x}} {{focus: none}} Use auto when the type is obvious from context. Use explicit types when clarity matters.

```code:auto-code lang=cpp
int x = 42;
double pi = 3.14159;
char c = 'A';
bool done = false;

auto count = 100;        // int
auto rate = 0.05;        // double
auto letter = 'Z';       // char
auto flag = true;         // bool

// auto mystery;          // ERROR: no initializer
---
explicit-types: 1-4
auto-types: 6-9
must-init: 11
```

# Const and constexpr

{{clear: slide}}

{{show: const-code typewriter 1.5s linear}} Two keywords that prevent mutation. Different guarantees, different power.

{{focus: const-vars}} {{zoom: 1.2x}} {{annotate: const-vars "Runtime constant"}} Const means "this value won't change after initialization." It's a promise to the compiler. Break the promise and the code won't compile.

{{zoom: 1x}} {{focus: constexpr-vars}} {{zoom: 1.3x}} {{annotate: constexpr-vars "Compile-time constant"}} Constexpr is stronger. The value is computed at compile time. Not at startup. Not at first use. At compile time. The compiler bakes it into the binary.

{{zoom: 1x}} {{focus: constexpr-fn}} {{zoom: 1.2x}} {{annotate: constexpr-fn "Evaluated at compile time"}} Functions can be constexpr too. If you call them with compile-time arguments, the result is computed during compilation. Zero runtime cost.

{{zoom: 1x}} {{focus: none}} Default to constexpr for values you know at compile time. Fall back to const for runtime values that shouldn't change.

```code:const-code lang=cpp
const int max_retries = 3;         // runtime constant
const double tax_rate = 0.085;     // runtime constant

constexpr int table_size = 1024;   // compile-time constant
constexpr double pi = 3.14159265;  // compile-time constant

constexpr int square(int n) {      // compile-time function
    return n * n;
}

constexpr int area = square(5);    // computed at compile time: 25
---
const-vars: 1-2
constexpr-vars: 4-5
constexpr-fn: 7-11
```

# Structured bindings

{{clear: slide}}

{{show: bindings-code typewriter 1.5s linear}} C++ 17 introduced structured bindings. They unpack composite values into named variables in one line.

{{focus: pair-binding}} {{zoom: 1.3x}} {{annotate: pair-binding "Unpack a pair"}} A pair holds two values. Structured bindings give each one a name. Auto bracket name comma age bracket equals the pair. Name gets the first element. Age gets the second.

{{zoom: 1x}} {{focus: array-binding}} {{zoom: 1.2x}} {{annotate: array-binding "Unpack an array"}} Works with arrays too. Three elements, three names.

{{zoom: 1x}} {{focus: map-binding}} {{zoom: 1.2x}} {{annotate: map-binding "Iterate maps"}} The real power shows in loops. Iterating a map gives you pairs. Structured bindings unpack each pair into key and value. No more dot first and dot second.

{{zoom: 1x}} {{focus: none}} Structured bindings reduce noise. They turn dot first into a real name.

```code:bindings-code lang=cpp
#include <map>
#include <string>
#include <utility>

std::pair<std::string, int> person{"Alice", 30};
auto [name, age] = person;  // name = "Alice", age = 30

int coords[] = {10, 20, 30};
auto [x, y, z] = coords;    // x = 10, y = 20, z = 30

std::map<std::string, int> scores{{"math", 95}, {"cs", 88}};
for (const auto& [subject, score] : scores) {
    // subject = "cs" or "math", score = 88 or 95
}
---
pair-binding: 5-6
array-binding: 8-9
map-binding: 11-13
```

# Scoped enums

{{clear: slide}}

{{show: enum-code typewriter 1.5s linear}} Old-style enums leak their names into the enclosing scope. Scoped enums fix this.

{{focus: old-enum}} {{zoom: 1.2x}} {{annotate: old-enum "Leaks names"}} The old way. Red, Green, Blue float in the global namespace. If another enum has Red, you get a collision.

{{zoom: 1x}} {{focus: new-enum}} {{zoom: 1.3x}} {{annotate: new-enum "Scoped names"}} Enum class. The values live inside the enum's scope. Color colon colon Red. No leaking. No collisions.

{{zoom: 1x}} {{focus: switch-use}} {{zoom: 1.2x}} {{annotate: switch-use "Exhaustive switch"}} Use scoped enums with switch statements. The compiler can warn you if you miss a case. This is how you make invalid states unrepresentable — the enum defines the complete set of possibilities.

{{zoom: 1x}} {{focus: none}} Always use enum class. Never use plain enum.

```code:enum-code lang=cpp
// Old style — don't use
enum OldColor { Red, Green, Blue };

// Modern — always use this
enum class Color { red, green, blue };
enum class Direction { north, south, east, west };

Color paint = Color::red;
Direction heading = Direction::north;

switch (paint) {
    case Color::red:   /* ... */ break;
    case Color::green: /* ... */ break;
    case Color::blue:  /* ... */ break;
}
---
old-enum: 2
new-enum: 5-6
switch-use: 11-15
```

# If with initializer

{{clear: slide}}

{{show: if-init typewriter 1.5s linear}} C++ 17 lets you declare a variable inside an if statement. The variable's scope is the if-else block — it doesn't leak.

{{focus: old-way}} {{zoom: 1.2x}} {{annotate: old-way "Leaks scope"}} The old pattern. Declare the iterator outside the if. It's visible after the if block, polluting the scope. You might accidentally use it later.

{{zoom: 1x}} {{focus: new-way}} {{zoom: 1.3x}} {{annotate: new-way "Scoped to if"}} With init. The iterator is declared inside the if statement. It exists only inside the if and else blocks. The moment you leave the if, the variable is gone.

{{zoom: 1x}} {{focus: none}} Tighter scopes mean fewer bugs. Declare variables as close to their use as possible. If-with-initializer is one more tool for that.

```code:if-init lang=cpp
#include <map>
#include <string>

std::map<std::string, int> scores{{"math", 95}};

// Old way — 'it' leaks past the if block
auto it = scores.find("math");
if (it != scores.end()) {
    // use it->second
}
// 'it' still visible here

// C++17 — 'it' scoped to the if-else
if (auto it = scores.find("math"); it != scores.end()) {
    // use it->second
} else {
    // 'it' valid here too, but end()
}
// 'it' doesn't exist here
---
old-way: 7-11
new-way: 14-19
```

# Numeric limits

{{clear: slide}}

{{show: limits-code typewriter 1.5s linear}} Every numeric type has a range. The standard library tells you exactly what it is.

{{focus: int-limits}} {{zoom: 1.2x}} {{annotate: int-limits "Min and max"}} numeric limits gives you the minimum and maximum value for any type. Int max is roughly 2.1 billion. Int min is roughly negative 2.1 billion.

{{zoom: 1x}} {{focus: overflow}} {{zoom: 1.3x}} {{annotate: overflow "Silent disaster"}} Overflow is silent in C++. If you add one to int max, you don't get an error. You get undefined behavior. The program might wrap around, crash, or launch missiles. The compiler makes no promises.

{{zoom: 1x}} {{focus: safe-check}} {{zoom: 1.2x}} {{annotate: safe-check "Check before"}} Check before arithmetic if overflow is possible. This matters in algorithms that accumulate sums or compute products.

{{zoom: 1x}} {{focus: none}} Know your limits. Literally.

```code:limits-code lang=cpp
#include <limits>
#include <cstdint>

constexpr int int_max = std::numeric_limits<int>::max();        // 2147483647
constexpr int int_min = std::numeric_limits<int>::min();        // -2147483648
constexpr double dbl_max = std::numeric_limits<double>::max();  // ~1.8e308

int x = std::numeric_limits<int>::max();
x = x + 1;  // undefined behavior — no error, no warning at runtime

// Safe: check before adding
bool safe_add(int a, int b) {
    if (b > 0 && a > std::numeric_limits<int>::max() - b) return false;
    if (b < 0 && a < std::numeric_limits<int>::min() - b) return false;
    return true;
}
---
int-limits: 4-6
overflow: 8-9
safe-check: 12-16
```
