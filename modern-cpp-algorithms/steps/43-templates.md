---
title: Templates and concepts
---

# Function templates

{{show: func-template-code typewriter 2s linear}} A function template is a recipe. You write it once. The compiler generates concrete versions for each type you use it with.

{{focus: template-decl}} {{zoom: 1.2x}} {{annotate: template-decl "Type parameter"}} The template parameter T is a placeholder. When you call max value with two ints, the compiler generates a version with T replaced by int. Call it with doubles, it generates another version.

{{zoom: 1x}} {{focus: deduction}} {{zoom: 1.2x}} {{annotate: deduction "Implicit types"}} Template argument deduction: the compiler figures out T from the arguments. You rarely need to specify it explicitly. The call looks like a normal function call.

{{zoom: 1x}} {{focus: explicit}} {{zoom: 1.2x}} {{annotate: explicit "Force the type"}} Sometimes you need explicit types. When the return type can't be deduced. When you want a specific conversion. Angle brackets after the function name.

{{zoom: 1x}} {{focus: none}} Templates are compile-time code generation. No runtime cost. The generated code is identical to what you'd write by hand for each type.

```code:func-template-code lang=cpp
template<typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// Deduction — compiler infers T
auto m1 = max_value(3, 7);         // T = int
auto m2 = max_value(3.14, 2.71);   // T = double

// Explicit type
auto m3 = max_value<double>(3, 2.71);  // T = double
---
template-decl: 1-4
deduction: 7-8
explicit: 11
```

# Class templates

{{clear: slide}}

{{show: class-template-code typewriter 2s linear}} Class templates parameterize entire classes. The standard library is built on them — vector, map, optional are all class templates.

{{focus: class-decl}} {{zoom: 1.2x}} {{annotate: class-decl "Parameterized type"}} A Stack of T. The type parameter appears in the member types, the method signatures, and the internal storage. One definition, infinite concrete types.

{{zoom: 1x}} {{focus: methods}} {{zoom: 1.2x}} {{annotate: methods "Same interface"}} Methods are defined inside the class or outside with the template prefix. Each method is itself a template, instantiated only when called.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Concrete types"}} Each instantiation is a completely separate type. Stack of int and Stack of string share no code at runtime. The compiler generates distinct classes.

{{zoom: 1x}} {{focus: none}} Class templates are the backbone of generic programming in C++. Every container, every smart pointer, every adaptor is a class template.

```code:class-template-code lang=cpp
template<typename T>
class Stack {
public:
    void push(const T& val) { data_.push_back(val); }

    T pop() {
        T top = std::move(data_.back());
        data_.pop_back();
        return top;
    }

    bool empty() const { return data_.empty(); }
    int size() const { return static_cast<int>(data_.size()); }

private:
    std::vector<T> data_;
};

// Usage — each is a distinct type
Stack<int> ints;
Stack<std::string> strings;
ints.push(42);
strings.push("hello");
---
class-decl: 1-16
methods: 4-13
usage: 19-22
```

# Template specialization

{{clear: slide}}

{{show: special-code typewriter 2s linear}} Sometimes a generic implementation doesn't work for a specific type. Specialization lets you provide a custom version.

{{focus: primary}} {{zoom: 1.2x}} {{annotate: primary "Generic version"}} The primary template handles the general case. It works for most types.

{{zoom: 1x}} {{focus: full-spec}} {{zoom: 1.2x}} {{annotate: full-spec "Specific override"}} Full specialization: template angle brackets empty, then the specific type after the class name. This version is used exclusively for bool. Different storage, different interface if needed.

{{zoom: 1x}} {{focus: partial-spec}} {{zoom: 1.2x}} {{annotate: partial-spec "Pattern match"}} Partial specialization: specialize for a pattern, not a specific type. Formatter for any pointer type. The template parameter T remains, but the pattern T star is specialized.

{{zoom: 1x}} {{focus: none}} Full specialization for specific types. Partial specialization for type patterns. The compiler selects the most specific match.

```code:special-code lang=cpp
// Primary template
template<typename T>
struct Formatter {
    static std::string format(const T& val) {
        return std::to_string(val);
    }
};

// Full specialization for std::string
template<>
struct Formatter<std::string> {
    static std::string format(const std::string& val) {
        return "\"" + val + "\"";
    }
};

// Partial specialization for pointers
template<typename T>
struct Formatter<T*> {
    static std::string format(T* ptr) {
        if (!ptr) return "nullptr";
        return "&" + Formatter<T>::format(*ptr);
    }
};
---
primary: 2-6
full-spec: 9-14
partial-spec: 17-23
```

# constexpr if

{{clear: slide}}

{{show: constexpr-code typewriter 1.5s linear}} C++17 introduced if constexpr — compile-time branching inside templates. The branch not taken is discarded entirely. It doesn't even need to compile for the given type.

{{focus: old-way}} {{zoom: 1.2x}} {{annotate: old-way "SFINAE"}} Before C++17, you needed SFINAE or tag dispatch to branch on types. Verbose, indirect, hard to read. Template metaprogramming for what should be a simple if statement.

{{zoom: 1x}} {{focus: new-way}} {{zoom: 1.2x}} {{annotate: new-way "Just an if"}} With if constexpr, you write a normal if. The condition is evaluated at compile time. The false branch is discarded — it can contain code that wouldn't compile for the current type.

{{zoom: 1x}} {{focus: none}} Use if constexpr whenever you need type-dependent behavior inside a template. It's the single biggest readability improvement in generic C++ code.

```code:constexpr-code lang=cpp
#include <type_traits>
#include <string>

// Old way: overloads or SFINAE
template<typename T>
std::string old_describe(T val) {
    // Can't branch on type without SFINAE tricks
    return std::to_string(val);
}

// C++17: if constexpr
template<typename T>
std::string describe(T val) {
    if constexpr (std::is_integral_v<T>) {
        return "int: " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "float: " + std::to_string(val);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "string: " + val;
    } else {
        return "unknown";
    }
}
---
old-way: 5-8
new-way: 11-22
```

# Concepts

{{clear: slide}}

{{show: concept-code typewriter 2s linear}} C++20 concepts replace SFINAE with named, readable constraints. A concept is a compile-time predicate on types. If the type doesn't satisfy the concept, the compiler gives a clear error.

{{focus: define-concept}} {{zoom: 1.2x}} {{annotate: define-concept "Named constraint"}} A concept is defined with the concept keyword and a requires expression. The body lists what the type must support — operations, return types, nested types.

{{zoom: 1x}} {{focus: use-concept}} {{zoom: 1.2x}} {{annotate: use-concept "Constrained template"}} Use a concept in place of typename. The function only accepts types that satisfy the concept. Error messages are clear — "T does not satisfy Numeric" instead of pages of template substitution failures.

{{zoom: 1x}} {{focus: auto-concept}} {{zoom: 1.2x}} {{annotate: auto-concept "Terse syntax"}} The terse syntax: concept name before auto in the parameter list. Same meaning, less ceremony. Use it for simple constraints.

{{zoom: 1x}} {{focus: none}} Concepts make templates readable. They document intent, catch errors early, and replace the dark art of SFINAE with plain English.

```code:concept-code lang=cpp
#include <concepts>

// Define a concept
template<typename T>
concept Numeric = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a < b } -> std::convertible_to<bool>;
};

// Use in template
template<Numeric T>
T sum(const std::vector<T>& vals) {
    T total{};
    for (const auto& v : vals) total = total + v;
    return total;
}

// Terse syntax
void print_value(std::integral auto val) {
    std::cout << val << "\n";
}
---
define-concept: 4-9
use-concept: 12-17
auto-concept: 20-22
```

# Standard concepts

{{clear: slide}}

{{show: std-concept-code typewriter 1.5s linear}} The standard library provides a rich set of concepts in the concepts header. Use them before writing your own.

{{focus: core-concepts}} {{zoom: 1.2x}} {{annotate: core-concepts "Type relationships"}} Core language concepts: same as, derived from, convertible to, integral, floating point. These test fundamental type properties.

{{zoom: 1x}} {{focus: comparison}} {{zoom: 1.2x}} {{annotate: comparison "Ordering"}} Comparison concepts: equality comparable, totally ordered. These check that a type supports the expected comparison operators with correct semantics.

{{zoom: 1x}} {{focus: callable}} {{zoom: 1.2x}} {{annotate: callable "Function objects"}} Callable concepts: invocable, predicate, regular invocable. These constrain function objects and lambdas. Use them to constrain callback parameters.

{{zoom: 1x}} {{focus: none}} Standard concepts cover the common cases. Write custom concepts only for domain-specific constraints.

```code:std-concept-code lang=cpp
#include <concepts>
#include <ranges>

// Core concepts
template<std::integral T>
T bit_count(T val) { return __builtin_popcountll(val); }

// Comparison concepts
template<std::totally_ordered T>
T clamp_value(T val, T lo, T hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

// Callable concepts
template<typename F, typename T>
    requires std::predicate<F, T>
std::vector<T> filter(const std::vector<T>& data, F pred) {
    std::vector<T> result;
    for (const auto& v : data)
        if (pred(v)) result.push_back(v);
    return result;
}

// Range concepts
template<std::ranges::input_range R>
auto first_element(R&& rng) {
    return *std::ranges::begin(rng);
}
---
core-concepts: 4-6
comparison: 9-13
callable: 16-24
```

# Concept composition

{{clear: slide}}

{{show: compose-code typewriter 1.5s linear}} Concepts compose with logical operators. Build complex constraints from simple ones.

{{focus: and-compose}} {{zoom: 1.2x}} {{annotate: and-compose "Both must hold"}} Conjunction: concept A and concept B. The type must satisfy both. Use double ampersand in requires clauses or define a new concept combining them.

{{zoom: 1x}} {{focus: or-compose}} {{zoom: 1.2x}} {{annotate: or-compose "Either suffices"}} Disjunction: concept A or concept B. The type must satisfy at least one. Useful for accepting multiple type families in a single template.

{{zoom: 1x}} {{focus: nested}} {{zoom: 1.2x}} {{annotate: nested "Build layers"}} Layer concepts: define a base concept, then build stricter ones on top. Hashable requires equality and a hash function. Cacheable requires hashable plus copy constructible.

{{zoom: 1x}} {{focus: none}} Concept composition creates a type-level algebra. Simple concepts combine into precise constraints. The result reads like a specification.

```code:compose-code lang=cpp
#include <concepts>
#include <functional>

// Conjunction: both must hold
template<typename T>
concept Printable = requires(T a) {
    { std::cout << a } -> std::same_as<std::ostream&>;
};

template<typename T>
concept PrintableNumeric = Printable<T> && std::integral<T>;

// Disjunction: either suffices
template<typename T>
concept NumberLike = std::integral<T> || std::floating_point<T>;

// Layered concepts
template<typename T>
concept Hashable = std::equality_comparable<T> && requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept Cacheable = Hashable<T> && std::copy_constructible<T>;
---
and-compose: 5-11
or-compose: 14-15
nested: 18-24
```

# Designated initializers

{{clear: slide}}

{{show: desig-code typewriter 1.5s linear}} C++20 designated initializers bring named-field construction to aggregates. Clear, self-documenting, and order-independent in intent.

{{focus: aggregate}} {{zoom: 1.2x}} {{annotate: aggregate "Plain struct"}} An aggregate is a struct or class with no user-declared constructors, no private members, no virtual functions. Plain data. Most configuration types are aggregates.

{{zoom: 1x}} {{focus: init}} {{zoom: 1.2x}} {{annotate: init "Named fields"}} Dot field-name equals value. Each field is named at the call site. No more counting constructor parameters or remembering argument order.

{{zoom: 1x}} {{focus: defaults}} {{zoom: 1.2x}} {{annotate: defaults "Skip fields"}} Fields not mentioned get their default value. Combine designated initializers with default member initializers for expressive, minimal construction.

{{zoom: 1x}} {{focus: none}} Designated initializers make configuration structs self-documenting. Use them for any struct with more than two fields.

```code:desig-code lang=cpp
struct Config {
    std::string host = "localhost";
    int port = 8080;
    int max_connections = 100;
    bool use_tls = false;
    int timeout_ms = 5000;
};

// Designated initializers — named, clear, minimal
auto cfg = Config{
    .host = "api.example.com",
    .port = 443,
    .use_tls = true,
};
// max_connections = 100, timeout_ms = 5000 (defaults)

// Compare with positional — which field is which?
// Config("api.example.com", 443, 100, true, 5000)  // Fragile
---
aggregate: 1-7
init: 10-14
defaults: 15
```
