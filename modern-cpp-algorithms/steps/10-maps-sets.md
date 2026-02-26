---
title: Maps and sets
---

# std::map

{{show: map-code typewriter 2s linear}} map is an ordered associative container. Keys are sorted. Operations are log n. Under the hood, a red-black tree.

{{focus: insert-ops}} {{zoom: 1.2x}} {{annotate: insert-ops "Insert key-value"}} Bracket insert creates or overwrites. insert only inserts if the key doesn't exist. try emplace constructs the value in place and also skips existing keys.

{{zoom: 1x}} {{focus: lookup}} {{zoom: 1.2x}} {{annotate: lookup "Log n lookup"}} find returns an iterator to the key-value pair, or end if not found. Bracket access is dangerous — it default-constructs a value if the key is missing.

{{zoom: 1x}} {{focus: ordered-iter}} {{zoom: 1.2x}} {{annotate: ordered-iter "Sorted order"}} Iteration visits keys in sorted order. This is the guarantee. If you need ordered traversal, map gives it to you for free.

{{zoom: 1x}} {{focus: structured}} {{annotate: structured "Structured binding"}} Structured bindings unpack each pair into key and value. Clean syntax for iteration.

{{zoom: 1x}} {{focus: none}} map is for when order matters. Log n operations, sorted iteration, stable iterators.

```code:map-code lang=cpp
#include <map>
#include <string>

std::map<std::string, int> ages;
ages["Alice"] = 30;                        // insert or overwrite
ages["Bob"] = 25;
ages.insert({"Charlie", 35});              // insert if absent
ages.try_emplace("Alice", 99);             // skipped — Alice exists

auto it = ages.find("Bob");               // iterator to {"Bob", 25}
if (it != ages.end()) {
    int age = it->second;                 // 25
}

// Iteration: sorted by key
for (const auto& [name, age] : ages) {
    // Alice:30, Bob:25, Charlie:35 — alphabetical
}
---
insert-ops: 4-8
lookup: 10-13
ordered-iter: 16-18
structured: 16
```

# std::set

{{clear: slide}}

{{show: rbtree-viz grow 0.5s spring}} {{show: set-code typewriter 1.5s linear}} set is a map without values. Unique sorted elements in a red-black tree.

{{focus: tree-root}} {{annotate: tree-root "Balanced"}} The red-black tree self-balances on every insert and erase. No operation degrades to O of n. Height stays log n.

{{focus: tree-left}} {{focus: tree-right}} {{annotate: tree-left "Sorted left"}} Left children are smaller. Right children are larger. In-order traversal yields sorted output.

{{focus: set-ops}} {{zoom: 1.2x}} {{annotate: set-ops "Insert and find"}} Insert returns a pair: an iterator and a boolean. The boolean tells you whether the insertion happened. If the element already exists, it returns false and the iterator points to the existing element.

{{zoom: 1x}} {{focus: contains}} {{zoom: 1.2x}} {{annotate: contains "C++20"}} contains is the clean way to check membership. Replaces the count-equals-one idiom from older code.

{{zoom: 1x}} {{focus: none}} set for sorted unique elements. Same red-black tree as map, same log n performance.

```data:rbtree-viz type=tree variant=red-black
{value: 30, left: {value: 15, left: {value: 10}, right: {value: 20}}, right: {value: 50, left: {value: 40}, right: {value: 60}}}
---
tree-root: 30
tree-left: 15
tree-right: 50
```

```code:set-code lang=cpp
#include <set>

std::set<int> s = {30, 10, 50, 20, 40};

auto [it, inserted] = s.insert(25);       // inserted = true
auto [it2, dup] = s.insert(30);           // dup = false (already exists)

s.erase(10);                               // remove element

if (s.contains(50)) {                      // C++20
    // yes, 50 is in the set
}

// Iteration: sorted order
for (int val : s) {
    // 20, 25, 30, 40, 50
}
---
set-ops: 5-6
contains: 10-12
```

# std::unordered_map

{{clear: slide}}

{{show: umap-code typewriter 2s linear}} unordered map is a hash table. Expected O of 1 for insert, find, and erase. No ordering guarantee.

{{focus: basic-use}} {{zoom: 1.2x}} {{annotate: basic-use "Same interface"}} The API mirrors map. Bracket access, find, insert, structured bindings — all the same. The difference is internal: hashing instead of tree comparison.

{{zoom: 1x}} {{focus: perf-note}} {{zoom: 1.2x}} {{annotate: perf-note "Expected O(1)"}} Expected O of 1 means amortized constant time when the hash function distributes well. Worst case is O of n if every key hashes to the same bucket.

{{zoom: 1x}} {{focus: no-order}} {{annotate: no-order "Unordered"}} Iteration order is arbitrary. Don't depend on it. Don't even expect it to be stable across insertions.

{{zoom: 1x}} {{focus: none}} Use unordered map when you don't need sorted keys and want average constant-time operations.

```code:umap-code lang=cpp
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> scores;
scores["Alice"] = 95;
scores["Bob"] = 87;
scores["Charlie"] = 92;

auto it = scores.find("Bob");
if (it != scores.end()) {
    int s = it->second;               // 87
}

// Expected O(1) insert, find, erase
scores.erase("Charlie");
scores["Diana"] = 88;

// Iteration order: arbitrary, not sorted
for (const auto& [name, score] : scores) {
    // order is implementation-defined
}
---
basic-use: 4-12
perf-note: 15-16
no-order: 19-21
```

# Hash internals

{{clear: slide}}

{{show: hash-buckets grow 0.5s spring}} A hash table is a bucket array. The hash function maps keys to bucket indices. Collisions are resolved by chaining.

{{focus: bucket-0}} {{annotate: bucket-0 "Hash → index"}} The hash function produces an integer. Modulo the bucket count gives the index. Key goes into that bucket.

{{focus: bucket-2}} {{annotate: bucket-2 "Chain on collision"}} When two keys hash to the same bucket, they form a chain — a linked list within the bucket. More collisions mean longer chains and slower lookups.

{{focus: bucket-4}} {{annotate: bucket-4 "Empty bucket"}} Empty buckets waste space. The load factor — elements divided by buckets — controls when rehashing occurs. Default max load factor is 1.0.

{{focus: none}} When the load factor exceeds the threshold, the table rehashes: allocates more buckets and redistributes every element. Expensive but amortized.

{{clear: slide}}

{{show: hash-code typewriter 1.5s linear}} You can inspect and tune the hash table's internals.

{{focus: bucket-info}} {{zoom: 1.2x}} {{annotate: bucket-info "Inspect buckets"}} bucket count tells you how many buckets exist. bucket size tells you how many elements are in a specific bucket. A bucket size greater than 1 means collisions.

{{zoom: 1x}} {{focus: load}} {{zoom: 1.2x}} {{annotate: load "Load factor"}} load factor is elements divided by buckets. When it exceeds max load factor, the table rehashes. You can lower max load factor to trade memory for speed.

{{zoom: 1x}} {{focus: reserve-hint}} {{annotate: reserve-hint "Preallocate"}} reserve preallocates bucket space. Use it when you know the final size. It prevents rehashing during insertion.

{{zoom: 1x}} {{focus: none}} Good hash functions spread keys evenly. The standard provides hashes for built-in types and strings. For custom types, you must supply your own.

```data:hash-buckets type=hash-map
{buckets: ["Alice:95", "", "Bob:87, Eve:91", "", "", "Charlie:92"]}
---
bucket-0: 0
bucket-2: 2
bucket-4: 4
```

```code:hash-code lang=cpp
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> m = {
    {"Alice", 95}, {"Bob", 87}, {"Charlie", 92}
};

auto bc = m.bucket_count();           // number of buckets
auto bs = m.bucket_size(0);           // elements in bucket 0

auto lf = m.load_factor();           // elements / buckets
m.max_load_factor(0.5f);             // rehash sooner, fewer collisions

m.reserve(1000);                     // preallocate for 1000 elements
---
bucket-info: 8-9
load: 11-12
reserve-hint: 14
```

# Custom comparators and hash functions

{{clear: slide}}

{{show: custom-code typewriter 2s linear}} map needs a comparator. unordered map needs a hash and an equality check. Both default to standard implementations.

{{focus: custom-comp}} {{zoom: 1.2x}} {{annotate: custom-comp "Custom ordering"}} A custom comparator is a function object with a call operator that returns true when the left argument comes before the right. Pass it as the third template parameter.

{{zoom: 1x}} {{focus: custom-hash}} {{zoom: 1.2x}} {{annotate: custom-hash "Custom hash"}} A custom hash is a function object that returns a size t. Combine member hashes using XOR and bit shifting. Pass it as the third template parameter for unordered map.

{{zoom: 1x}} {{focus: custom-eq}} {{zoom: 1.2x}} {{annotate: custom-eq "Custom equality"}} The hash function alone isn't enough. unordered map also needs an equality function — the fourth template parameter. Two keys that hash to the same bucket must be compared for identity.

{{zoom: 1x}} {{focus: none}} For simple structs, specialize std hash in the std namespace. For complex cases, pass the hash and equality as template parameters.

```code:custom-code lang=cpp
#include <map>
#include <unordered_map>
#include <string>

// Custom comparator: reverse alphabetical
struct ReverseCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return a > b;
    }
};
std::map<std::string, int, ReverseCompare> reverse_map;

// Custom hash for a struct
struct Point {
    int x, y;
};

struct PointHash {
    std::size_t operator()(const Point& p) const {
        auto h1 = std::hash<int>{}(p.x);
        auto h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

struct PointEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

std::unordered_map<Point, std::string, PointHash, PointEqual> points;
---
custom-comp: 6-11
custom-hash: 18-24
custom-eq: 26-30
```

# multimap and multiset

{{clear: slide}}

{{show: multi-code typewriter 1.5s linear}} multimap and multiset allow duplicate keys. Same tree or hash structure, but duplicates coexist.

{{focus: multi-insert}} {{zoom: 1.2x}} {{annotate: multi-insert "Duplicates allowed"}} Inserting a duplicate key adds another entry. No overwrite. No rejection. Both entries live in the container.

{{zoom: 1x}} {{focus: equal-range}} {{zoom: 1.2x}} {{annotate: equal-range "Range of matches"}} equal range returns a pair of iterators: the first matching entry and one past the last. Iterate between them to visit all values for a key.

{{zoom: 1x}} {{focus: count-key}} {{annotate: count-key "Count duplicates"}} count returns how many entries share a key. For multiset, this tells you the multiplicity. For multimap, the number of values associated with that key.

{{zoom: 1x}} {{focus: none}} Use multimap when a key maps to multiple values and you want them sorted. Consider unordered map of vectors as an alternative — often simpler.

```code:multi-code lang=cpp
#include <map>
#include <set>
#include <string>

std::multimap<std::string, int> grades;
grades.insert({"Alice", 95});
grades.insert({"Alice", 87});              // duplicate key allowed
grades.insert({"Bob", 92});

// Find all grades for Alice
auto [begin, end] = grades.equal_range("Alice");
for (auto it = begin; it != end; ++it) {
    // it->second: 87, 95 (sorted)
}

auto count = grades.count("Alice");        // 2

std::multiset<int> ms = {5, 3, 5, 1, 3, 5};
auto fives = ms.count(5);                  // 3
---
multi-insert: 5-8
equal-range: 11-14
count-key: 16, 19
```

# Choosing the right container

{{clear: slide}}

{{show: choose-map grow 0.5s spring}} The decision tree for associative containers.

{{focus: need-order}} {{annotate: need-order "Need sorted keys?"}} First question: do you need keys in sorted order? If yes, use map or set. If no, use unordered variants for speed.

{{focus: need-dup}} {{annotate: need-dup "Duplicate keys?"}} Second question: do you allow duplicate keys? If yes, multi variants. If no, the regular ones.

{{focus: need-value}} {{annotate: need-value "Key-value or key-only?"}} Third question: do you store values with keys? If yes, map or unordered map. If no, set or unordered set.

{{focus: default-pick}} {{annotate: default-pick "Start here"}} Default pick: unordered map. Constant-time operations. Switch to map only when sorted iteration is required.

{{focus: none}} unordered map is the workhorse for lookup tables. map is for when you need range queries or sorted output. set and unordered set for membership testing.

```data:choose-map type=array
["Sorted keys? → map / set (red-black tree, log n)", "Unsorted keys? → unordered_map / unordered_set (hash, O(1))", "Duplicate keys? → multimap / multiset", "Key + value? → map variants | Key only? → set variants", "Default: unordered_map — fast, simple, sufficient"]
---
need-order: 0
need-dup: 2
need-value: 3
default-pick: 4
```
