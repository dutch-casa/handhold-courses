---
title: Algebraic types
---

# std::pair

{{show: pair-code typewriter 1.5s linear}} pair bundles two values into one. The simplest composite type.

{{focus: create-pair}} {{zoom: 1.2x}} {{annotate: create-pair "Two values"}} A pair holds a first and a second. That's it. No names, no semantics. Just positional slots.

{{zoom: 1x}} {{focus: make-pair}} {{zoom: 1.2x}} {{annotate: make-pair "Type deduction"}} make pair deduces the types from the arguments. Or let CTAD handle it — pair with braces works in C++ 17.

{{zoom: 1x}} {{focus: structured}} {{zoom: 1.2x}} {{annotate: structured "Structured binding"}} Structured bindings decompose a pair into named variables. far more readable than dot first and dot second.

{{zoom: 1x}} {{focus: map-use}} {{annotate: map-use "Map returns pairs"}} map iteration yields pairs. Every key-value association in the standard library is a pair. This is where you encounter them most.

{{zoom: 1x}} {{focus: none}} pair is a building block. You'll see it everywhere — map entries, function returns, tuple-like decomposition.

```code:pair-code lang=cpp
#include <utility>
#include <string>
#include <map>

std::pair<std::string, int> p = {"Alice", 30};
auto name = p.first;                  // "Alice"
auto age = p.second;                  // 30

auto p2 = std::make_pair("Bob", 25); // type deduced

// Structured binding (C++17)
auto [n, a] = p;                     // n = "Alice", a = 30

// Maps return pairs
std::map<std::string, int> m = {{"X", 1}, {"Y", 2}};
for (const auto& [key, val] : m) {
    // key: "X", val: 1 — then key: "Y", val: 2
}
---
create-pair: 5-7
make-pair: 9
structured: 12
map-use: 15-18
```

# std::tuple

{{clear: slide}}

{{show: tuple-code typewriter 2s linear}} tuple generalizes pair to N values. Any number of types, any combination.

{{focus: create-tuple}} {{zoom: 1.2x}} {{annotate: create-tuple "N-ary bundle"}} A tuple holds N values of different types. Three, four, ten — no limit. The types are fixed at compile time.

{{zoom: 1x}} {{focus: get-access}} {{zoom: 1.2x}} {{annotate: get-access "Index access"}} get with an index extracts the Nth element. The index is a compile-time constant, not a runtime variable.

{{zoom: 1x}} {{focus: binding}} {{zoom: 1.2x}} {{annotate: binding "Unpack all at once"}} Structured bindings unpack every element in one declaration. Much cleaner than chaining get calls.

{{zoom: 1x}} {{focus: make-tuple}} {{zoom: 1.2x}} {{annotate: make-tuple "Return multiple"}} Functions return tuples to yield multiple values. Callers use structured bindings to unpack. This replaces output parameters.

{{zoom: 1x}} {{focus: none}} tuple is pair's big sibling. Use it for multiple return values and temporary groupings. For anything with semantic meaning, define a struct.

```code:tuple-code lang=cpp
#include <tuple>
#include <string>

std::tuple<std::string, int, double> record = {"Alice", 30, 3.8};

auto name = std::get<0>(record);          // "Alice"
auto age = std::get<1>(record);           // 30
auto gpa = std::get<2>(record);           // 3.8

// Structured binding
auto [n, a, g] = record;                 // n="Alice", a=30, g=3.8

// Return multiple values
auto parse_line(const std::string& line)
    -> std::tuple<std::string, int>
{
    return {"parsed", 42};
}
auto [label, value] = parse_line("input");
---
create-tuple: 4
get-access: 6-8
binding: 11
make-tuple: 14-19
```

# std::optional

{{clear: slide}}

{{show: opt-code typewriter 2s linear}} optional represents "value or nothing." It replaces the sentinel values and boolean flags that plague C code.

{{focus: create-opt}} {{zoom: 1.2x}} {{annotate: create-opt "Maybe a value"}} An optional either holds a T or holds nothing. nullopt signals the empty state.

{{zoom: 1x}} {{focus: has-value}} {{zoom: 1.2x}} {{annotate: has-value "Check first"}} has value or boolean conversion tells you whether the optional is engaged. Always check before accessing the value.

{{zoom: 1x}} {{focus: value-access}} {{zoom: 1.2x}} {{annotate: value-access "Safe extraction"}} value throws if the optional is empty. value or returns a default. Star dereferences without checking — use it only after you've verified engagement.

{{zoom: 1x}} {{focus: find-use}} {{zoom: 1.2x}} {{annotate: find-use "Replaces sentinels"}} Functions that might fail return optional. No special return codes. No negative-one-means-not-found. The type tells you the function can fail.

{{zoom: 1x}} {{focus: none}} optional makes absence explicit. The type system forces callers to handle the empty case. No more "did you check for negative one?"

```code:opt-code lang=cpp
#include <optional>
#include <string>
#include <vector>

std::optional<int> maybe = 42;
std::optional<int> empty = std::nullopt;

if (maybe.has_value()) {
    int val = *maybe;                   // 42
}

int safe = empty.value_or(-1);         // -1 (default)

// Function that might not find a result
auto find_user(const std::vector<std::string>& users,
               const std::string& name) -> std::optional<int>
{
    for (int i = 0; i < static_cast<int>(users.size()); ++i) {
        if (users[i] == name) return i;
    }
    return std::nullopt;
}

auto idx = find_user({"Alice", "Bob"}, "Charlie");
if (!idx) { /* not found */ }
---
create-opt: 5-6
has-value: 8-10
value-access: 12
find-use: 15-22
```

# std::variant and std::visit

{{clear: slide}}

{{show: variant-code typewriter 2s linear}} variant is a type-safe tagged union. It holds exactly one of its alternative types at any time.

{{focus: create-var}} {{zoom: 1.2x}} {{annotate: create-var "One of N types"}} A variant of int, double, and string holds one of those three. The active alternative is tracked at runtime. Default-constructs to the first type.

{{zoom: 1x}} {{focus: get-var}} {{zoom: 1.2x}} {{annotate: get-var "Access by type"}} get by type extracts the value if the variant currently holds that type. Throws bad variant access if you guess wrong.

{{zoom: 1x}} {{focus: holds}} {{zoom: 1.2x}} {{annotate: holds "Check first"}} holds alternative tells you which type is active. Check before you get. Or better — use visit.

{{zoom: 1x}} {{focus: visit}} {{zoom: 1.3x}} {{annotate: visit "Pattern match"}} visit takes a callable that handles every alternative. The overloaded pattern with multiple lambdas gives you something close to pattern matching. The compiler enforces exhaustiveness — miss a type and it won't compile.

{{zoom: 1x}} {{focus: none}} variant replaces C-style unions with type safety. visit replaces switch-on-tag with exhaustive dispatch. Together they eliminate an entire class of bugs.

```code:variant-code lang=cpp
#include <variant>
#include <string>

std::variant<int, double, std::string> v = 42;

v = 3.14;                              // now holds double
v = std::string("hello");             // now holds string

auto s = std::get<std::string>(v);    // "hello"
// std::get<int>(v);                  // throws bad_variant_access

if (std::holds_alternative<std::string>(v)) {
    // yes, it's a string
}

// Exhaustive visitation
auto result = std::visit([](auto&& arg) -> std::string {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>)
        return std::to_string(arg);
    else if constexpr (std::is_same_v<T, double>)
        return std::to_string(arg);
    else
        return arg;
}, v);
---
create-var: 4
get-var: 9
holds: 12-14
visit: 17-25
```

# std::expected (C++23)

{{clear: slide}}

{{show: expected-code typewriter 2s linear}} expected holds either a value or an error. It's optional with a reason for failure.

{{focus: success}} {{zoom: 1.2x}} {{annotate: success "Value path"}} When the operation succeeds, expected holds the value. has value returns true. value extracts it.

{{zoom: 1x}} {{focus: failure}} {{zoom: 1.2x}} {{annotate: failure "Error path"}} When the operation fails, expected holds the error. has value returns false. error extracts the error.

{{zoom: 1x}} {{focus: unexpected}} {{zoom: 1.2x}} {{annotate: unexpected "Signal failure"}} Return unexpected to signal an error. The error type is the second template parameter. It can be an enum, a string, a struct — anything.

{{zoom: 1x}} {{focus: chain}} {{zoom: 1.3x}} {{annotate: chain "Monadic chaining"}} and then chains operations. If the expected holds a value, the function runs. If it holds an error, the error propagates. No nested if-checks.

{{zoom: 1x}} {{focus: none}} expected replaces error codes and exceptions for recoverable errors. The type system forces callers to handle both paths.

```code:expected-code lang=cpp
#include <expected>
#include <string>

enum class ParseError { empty_input, bad_format };

auto parse_int(const std::string& s) -> std::expected<int, ParseError> {
    if (s.empty()) return std::unexpected(ParseError::empty_input);
    // simplified: real parsing would check each char
    return std::stoi(s);
}

auto result = parse_int("42");
if (result.has_value()) {
    int val = *result;                     // 42
}

auto bad = parse_int("");
if (!bad) {
    auto err = bad.error();                // ParseError::empty_input
}

// Monadic chaining (C++23)
auto doubled = parse_int("21")
    .and_then([](int n) -> std::expected<int, ParseError> {
        return n * 2;
    });
// doubled == 42
---
success: 12-15
failure: 17-20
unexpected: 7-8
chain: 23-27
```

# std::any

{{clear: slide}}

{{show: any-code typewriter 1.5s linear}} any holds a single value of any type. Type-erased storage. The escape hatch.

{{focus: store}} {{zoom: 1.2x}} {{annotate: store "Store anything"}} Assign any type to an any object. It type-erases the value — the any doesn't expose the held type in its own type signature.

{{zoom: 1x}} {{focus: cast}} {{zoom: 1.2x}} {{annotate: cast "Cast to extract"}} any cast extracts the value if the type matches. If you guess the wrong type, it throws bad any cast.

{{zoom: 1x}} {{focus: has}} {{annotate: has "Check type"}} has value tells you whether the any is engaged. type returns the type info of the held value. Use it to check before casting.

{{zoom: 1x}} {{focus: none}} any is a last resort. Prefer optional for maybe-values. Prefer variant for known type sets. Use any only when the type is truly unknown at compile time — plugin systems, heterogeneous containers, serialization boundaries.

```code:any-code lang=cpp
#include <any>
#include <string>

std::any a = 42;
a = std::string("hello");
a = 3.14;

auto val = std::any_cast<double>(a);   // 3.14
// std::any_cast<int>(a);              // throws bad_any_cast

if (a.has_value()) {
    // a holds something
}

if (a.type() == typeid(double)) {
    auto d = std::any_cast<double>(a);
}
---
store: 4-6
cast: 8
has: 11-13, 15-17
```

# Making invalid states unrepresentable

{{clear: slide}}

{{show: synth-code typewriter 2s linear}} These types aren't just utilities. They're tools for encoding invariants into the type system.

{{focus: bad-api}} {{zoom: 1.2x}} {{annotate: bad-api "Boolean blindness"}} A bool return says nothing about why it failed. The caller can ignore it. The compiler won't complain.

{{zoom: 1x}} {{focus: better-api}} {{zoom: 1.2x}} {{annotate: better-api "Explicit failure"}} optional says: this might not produce a value. The caller must check. The compiler guides them.

{{zoom: 1x}} {{focus: best-api}} {{zoom: 1.3x}} {{annotate: best-api "Rich error"}} expected says: this produces a value or an error. The error type carries information. The caller must handle both paths.

{{zoom: 1x}} {{focus: variant-state}} {{zoom: 1.3x}} {{annotate: variant-state "State machine"}} variant models a state machine. The connection is either connected, disconnected, or errored. No invalid combinations. No boolean pairs. The type is the state.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}} Use pair and tuple for temporary grouping. Use optional for maybe-values. Use expected for fallible operations. Use variant for closed state machines. Use any almost never. Every type choice is a design choice.

```code:synth-code lang=cpp
#include <optional>
#include <expected>
#include <variant>
#include <string>

// Bad: boolean blindness
bool connect_v1(const std::string& host);    // true = success? maybe?

// Better: explicit absence
auto connect_v2(const std::string& host)
    -> std::optional<Connection>;            // nothing = failed

// Best: rich error
auto connect_v3(const std::string& host)
    -> std::expected<Connection, NetError>;   // value or reason

// State machine via variant
using ConnState = std::variant<
    Disconnected,
    Connected,
    Error
>;
// Exactly one state at a time. No invalid combinations.
---
bad-api: 7
better-api: 10-11
best-api: 14-15
variant-state: 18-22
```
