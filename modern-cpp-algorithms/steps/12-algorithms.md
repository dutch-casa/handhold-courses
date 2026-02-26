---
title: Algorithms and lambdas
---

# The algorithm header

{{show: algo-overview typewriter 2s linear}} The algorithm header contains over a hundred generic algorithms. They operate on iterator pairs. They work with any container.

{{focus: non-modifying}} {{zoom: 1.2x}} {{annotate: non-modifying "Read-only"}} Non-modifying algorithms inspect without changing. find, count, any of, all of, none of. They answer questions about data.

{{zoom: 1x}} {{focus: modifying}} {{zoom: 1.2x}} {{annotate: modifying "Transform"}} Modifying algorithms change elements in place. sort, reverse, transform, fill, rotate, partition. They reshape data.

{{zoom: 1x}} {{focus: numeric}} {{zoom: 1.2x}} {{annotate: numeric "Reduce"}} Numeric algorithms live in the numeric header. accumulate, reduce, inner product, partial sum. They collapse data into single values.

{{zoom: 1x}} {{focus: pattern}} {{annotate: pattern "Iterator pairs"}} Every algorithm takes an iterator pair — begin and end. The algorithm doesn't know or care which container produced those iterators. This is the generic programming contract.

{{zoom: 1x}} {{focus: none}} You don't write loops for common operations. You call algorithms. The names are standardized. The implementations are optimized. The intent is clear.

```code:algo-overview lang=cpp
#include <algorithm>
#include <numeric>
#include <vector>

std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

// Non-modifying
auto it = std::find(v.begin(), v.end(), 8);        // iterator to 8
auto cnt = std::count(v.begin(), v.end(), 3);      // 1

// Modifying
std::sort(v.begin(), v.end());                     // {1,2,3,4,5,6,7,8,9}
std::reverse(v.begin(), v.end());                  // {9,8,7,6,5,4,3,2,1}

// Numeric
auto sum = std::accumulate(v.begin(), v.end(), 0); // 45
---
non-modifying: 8-9
modifying: 12-13
numeric: 16
pattern: 8, 9, 12, 13, 16
```

# Lambda anatomy

{{clear: slide}}

{{show: lambda-code typewriter 2s linear}} A lambda is an inline function object. It has four parts: captures, parameters, return type, and body.

{{focus: basic}} {{zoom: 1.2x}} {{annotate: basic "Minimal lambda"}} Square brackets, parentheses, body. The simplest lambda takes parameters and returns a result. The compiler deduces the return type.

{{zoom: 1x}} {{focus: capture-val}} {{zoom: 1.2x}} {{annotate: capture-val "Capture by value"}} Equals in the capture list copies all referenced local variables into the lambda. The lambda owns its own copies. Changes to the originals don't affect the lambda.

{{zoom: 1x}} {{focus: capture-ref}} {{zoom: 1.2x}} {{annotate: capture-ref "Capture by reference"}} Ampersand in the capture list captures by reference. The lambda sees the original variables. Mutations inside the lambda affect the originals.

{{zoom: 1x}} {{focus: explicit-cap}} {{zoom: 1.3x}} {{annotate: explicit-cap "Explicit captures"}} Name specific variables in the capture list. This is the preferred style — it documents exactly what the lambda depends on.

{{zoom: 1x}} {{focus: trailing}} {{annotate: trailing "Explicit return"}} Trailing return type specifies the return explicitly. Use it when the compiler can't deduce or when you want documentation.

{{zoom: 1x}} {{focus: none}} Lambdas are the glue between data and algorithms. Every algorithm that takes a predicate or transform function works with lambdas.

```code:lambda-code lang=cpp
#include <vector>
#include <algorithm>

// Basic lambda
auto square = [](int x) { return x * x; };
int result = square(5);                       // 25

// Capture by value (=)
int threshold = 10;
auto above = [=](int x) { return x > threshold; };

// Capture by reference (&)
int total = 0;
auto add_to_total = [&](int x) { total += x; };

// Explicit captures (preferred)
int min = 3, max = 8;
auto in_range = [min, max](int x) { return x >= min && x <= max; };

// Trailing return type
auto divide = [](double a, double b) -> double {
    return a / b;
};
---
basic: 5-6
capture-val: 9-10
capture-ref: 13-14
explicit-cap: 17-18
trailing: 21-23
```

# sort with custom comparators

{{clear: slide}}

{{show: sort-trace grow 0.5s spring}} {{show: sort-code typewriter 1.5s linear}} sort rearranges elements in place. Default is ascending. A custom comparator changes the order.

{{focus: before-sort}} {{annotate: before-sort "Unsorted"}} The array starts in arbitrary order. sort will rearrange it.

{{focus: after-sort}} {{annotate: after-sort "Sorted"}} After sort with a descending comparator, the largest element is first.

{{focus: default-sort}} {{zoom: 1.2x}} {{annotate: default-sort "Ascending"}} Default sort uses less-than. Ascending order. O of n log n.

{{zoom: 1x}} {{focus: custom-sort}} {{zoom: 1.2x}} {{annotate: custom-sort "Descending"}} A lambda comparator reverses the order. Return true when a should come before b. Greater-than gives descending.

{{zoom: 1x}} {{focus: struct-sort}} {{zoom: 1.3x}} {{annotate: struct-sort "Sort by field"}} Sort structs by any member. The lambda extracts the comparison key. Sort by age, by name, by score — the comparator decides.

{{zoom: 1x}} {{focus: stable}} {{annotate: stable "Preserves ties"}} stable sort preserves the relative order of equal elements. Regular sort doesn't guarantee this. Use stable sort when tie-breaking matters.

{{zoom: 1x}} {{focus: none}} sort is the most-used algorithm. Learn to write comparators and you unlock every sorting problem.

```data:sort-trace type=array
["[5|3|8|1|9|2|7|4|6]", "[9|8|7|6|5|4|3|2|1]"]
---
before-sort: 0
after-sort: 1
```

```code:sort-code lang=cpp
#include <algorithm>
#include <vector>
#include <string>

std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

// Default: ascending
std::sort(v.begin(), v.end());

// Custom: descending
std::sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;
});

// Sort structs by field
struct Student { std::string name; int age; };
std::vector<Student> students = {{"Alice", 22}, {"Bob", 19}, {"Eve", 25}};
std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.age < b.age;
});

// Stable sort: preserves relative order of equal elements
std::stable_sort(v.begin(), v.end());
---
default-sort: 8
custom-sort: 11-13
struct-sort: 16-20
stable: 23
```

# find, find_if, count_if

{{clear: slide}}

{{show: find-code typewriter 2s linear}} Search algorithms locate elements by value or by predicate.

{{focus: find-val}} {{zoom: 1.2x}} {{annotate: find-val "By value"}} find returns an iterator to the first element matching the value. Returns end if not found. Linear scan — O of n.

{{zoom: 1x}} {{focus: find-if}} {{zoom: 1.2x}} {{annotate: find-if "By predicate"}} find if takes a lambda predicate. It returns the first element where the predicate returns true. This is find with custom logic.

{{zoom: 1x}} {{focus: find-if-not}} {{zoom: 1.2x}} {{annotate: find-if-not "Invert predicate"}} find if not returns the first element where the predicate is false. Saves you from writing a negated lambda.

{{zoom: 1x}} {{focus: count-if}} {{zoom: 1.2x}} {{annotate: count-if "Count matches"}} count if counts how many elements satisfy the predicate. Unlike find, it doesn't stop at the first match. It scans everything.

{{zoom: 1x}} {{focus: none}} find for locating. count for tallying. Both take predicates for flexible matching.

```code:find-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {4, 7, 2, 9, 1, 8, 3, 6, 5};

// find: first element equal to 9
auto it = std::find(v.begin(), v.end(), 9);
// it points to 9, or v.end() if not found

// find_if: first element greater than 7
auto big = std::find_if(v.begin(), v.end(),
    [](int x) { return x > 7; });
// big points to 9

// find_if_not: first element that is NOT even
auto odd = std::find_if_not(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; });
// odd points to 7

// count_if: how many are even?
auto even_count = std::count_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; });
// even_count == 4
---
find-val: 7-8
find-if: 11-13
find-if-not: 16-18
count-if: 21-23
```

# transform

{{clear: slide}}

{{show: transform-code typewriter 2s linear}} transform applies a function to every element and writes the results to a destination. It's map in functional programming terms.

{{focus: in-place}} {{zoom: 1.2x}} {{annotate: in-place "Mutate in place"}} When source and destination are the same range, transform modifies in place. Every element is replaced by the function's return value.

{{zoom: 1x}} {{focus: to-other}} {{zoom: 1.2x}} {{annotate: to-other "To new container"}} When the destination is a different range, transform writes results there. Use back inserter to append to an empty output vector.

{{zoom: 1x}} {{focus: binary}} {{zoom: 1.2x}} {{annotate: binary "Two inputs"}} Binary transform takes two input ranges and a two-argument function. Element-wise operations across paired ranges.

{{zoom: 1x}} {{focus: none}} transform replaces manual loops. The intent — apply a function to every element — is explicit in the name.

```code:transform-code lang=cpp
#include <algorithm>
#include <vector>
#include <string>

std::vector<int> v = {1, 2, 3, 4, 5};

// In-place: square every element
std::transform(v.begin(), v.end(), v.begin(),
    [](int x) { return x * x; });
// v = {1, 4, 9, 16, 25}

// To new container
std::vector<int> src = {1, 2, 3, 4, 5};
std::vector<std::string> out;
std::transform(src.begin(), src.end(), std::back_inserter(out),
    [](int x) { return std::to_string(x); });
// out = {"1", "2", "3", "4", "5"}

// Binary: element-wise addition
std::vector<int> a = {1, 2, 3};
std::vector<int> b = {10, 20, 30};
std::vector<int> sums;
std::transform(a.begin(), a.end(), b.begin(),
    std::back_inserter(sums),
    [](int x, int y) { return x + y; });
// sums = {11, 22, 33}
---
in-place: 8-10
to-other: 14-17
binary: 20-25
```

# accumulate and reduce

{{clear: slide}}

{{show: accum-code typewriter 2s linear}} accumulate folds a range into a single value. reduce does the same but allows parallel execution.

{{focus: basic-sum}} {{zoom: 1.2x}} {{annotate: basic-sum "Sum with init"}} accumulate takes a range, an initial value, and optionally a binary operation. Default is addition. The initial value also sets the return type.

{{zoom: 1x}} {{focus: custom-op}} {{zoom: 1.2x}} {{annotate: custom-op "Custom fold"}} Supply a lambda to change the operation. Multiply for product. Custom logic for building strings. Any binary operation works.

{{zoom: 1x}} {{focus: reduce}} {{zoom: 1.2x}} {{annotate: reduce "Parallelizable"}} reduce allows the implementation to reorder operations. For associative and commutative operations like addition, this means the work can run in parallel.

{{zoom: 1x}} {{focus: par-exec}} {{zoom: 1.3x}} {{annotate: par-exec "Execution policy"}} Pass an execution policy as the first argument. par means parallel. seq means sequential. par unseq means parallel and vectorized.

{{zoom: 1x}} {{focus: none}} Use accumulate for sequential folds. Use reduce with an execution policy when the operation is associative and you want parallelism.

```code:accum-code lang=cpp
#include <numeric>
#include <execution>
#include <vector>
#include <string>

std::vector<int> v = {1, 2, 3, 4, 5};

// Sum with initial value 0
auto sum = std::accumulate(v.begin(), v.end(), 0);       // 15

// Product with custom operation
auto product = std::accumulate(v.begin(), v.end(), 1,
    [](int acc, int x) { return acc * x; });
// product = 120

// reduce: potentially parallel, order may vary
auto r_sum = std::reduce(v.begin(), v.end(), 0);         // 15

// Parallel execution policy
auto par_sum = std::reduce(std::execution::par,
    v.begin(), v.end(), 0);
---
basic-sum: 9
custom-op: 12-14
reduce: 17
par-exec: 20-21
```

# Remove-erase idiom

{{clear: slide}}

{{show: erase-code typewriter 2s linear}} remove doesn't actually remove elements. It shuffles the keepers to the front and returns an iterator to the new logical end. You must erase the tail.

{{focus: remove-step}} {{zoom: 1.2x}} {{annotate: remove-step "Shuffle forward"}} remove moves non-matching elements to the front. The return value points to the first element in the "garbage" tail. Elements past this iterator are unspecified.

{{zoom: 1x}} {{focus: erase-step}} {{zoom: 1.2x}} {{annotate: erase-step "Chop the tail"}} erase from the remove iterator to end actually shrinks the container. This is the remove-erase idiom. Two steps because algorithms can't modify container size.

{{zoom: 1x}} {{focus: erase-if}} {{zoom: 1.3x}} {{annotate: erase-if "C++20 one-liner"}} C++ 20 added erase if as a free function. One call. No idiom. It handles both the shuffle and the erase. Use this when you can.

{{zoom: 1x}} {{focus: none}} Pre-C++ 20: remove plus erase. C++ 20 and later: erase if. The idiom exists because algorithms operate on iterators, not containers.

```code:erase-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Step 1: remove shuffles keepers to front, returns new logical end
auto new_end = std::remove_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; });
// v = {1, 3, 5, 7, 9, ?, ?, ?, ?}  — tail is garbage

// Step 2: erase the garbage tail
v.erase(new_end, v.end());
// v = {1, 3, 5, 7, 9}

// C++20: erase_if does both steps
std::vector<int> w = {1, 2, 3, 4, 5, 6, 7, 8, 9};
std::erase_if(w, [](int x) { return x % 2 == 0; });
// w = {1, 3, 5, 7, 9}
---
remove-step: 7-9
erase-step: 12-13
erase-if: 16-17
```

# Predicate algorithms

{{clear: slide}}

{{show: pred-code typewriter 2s linear}} all of, any of, none of — three algorithms that answer yes-or-no questions about a range.

{{focus: all-of}} {{zoom: 1.2x}} {{annotate: all-of "Every element"}} all of returns true only if the predicate holds for every element. One failure and it short-circuits to false.

{{zoom: 1x}} {{focus: any-of}} {{zoom: 1.2x}} {{annotate: any-of "At least one"}} any of returns true if the predicate holds for at least one element. One success and it short-circuits to true.

{{zoom: 1x}} {{focus: none-of}} {{zoom: 1.2x}} {{annotate: none-of "Zero matches"}} none of returns true only if the predicate holds for no elements. It's the negation of any of.

{{zoom: 1x}} {{focus: compose}} {{zoom: 1.2x}} {{annotate: compose "Guard clauses"}} These algorithms replace loops with guard-clause checks. Is the input valid? Are all scores above threshold? Is any entry corrupted? The algorithm name communicates the question.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}} Predicate algorithms turn validation loops into one-liners. The intent lives in the algorithm name, not in loop boilerplate.

```code:pred-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> scores = {85, 92, 78, 96, 88};

// all_of: every score above 70?
bool all_pass = std::all_of(scores.begin(), scores.end(),
    [](int s) { return s >= 70; });
// true

// any_of: any score above 95?
bool has_star = std::any_of(scores.begin(), scores.end(),
    [](int s) { return s > 95; });
// true (96)

// none_of: no score below 50?
bool no_fail = std::none_of(scores.begin(), scores.end(),
    [](int s) { return s < 50; });
// true

// Compose into guard clauses
bool valid_input(const std::vector<int>& data) {
    if (data.empty()) return false;
    if (!std::all_of(data.begin(), data.end(),
        [](int x) { return x >= 0 && x <= 100; }))
        return false;
    return true;
}
---
all-of: 7-9
any-of: 12-14
none-of: 17-19
compose: 22-28
```
