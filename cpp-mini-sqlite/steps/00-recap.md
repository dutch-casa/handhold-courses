---
title: C++ Recap — Tools You Already Know
---

# auto — friend or liar?

{{show: verbose-iter typewriter 2s linear}} Here's a type you've probably written. `std::unordered_map<std::string, std::vector<int>>::iterator`. This type name alone to say "iterator."

{{focus: iter-decl}} {{annotate: iter-decl "Type noise"}} That type is correct. It's also unreadable. And when the container type changes, every declaration breaks. This is the problem `auto` solves.

{{focus: none}} {{clear: slide}}

{{show: auto-split typewriter 2s linear}} {{split}} Let's see where `auto` helps, and where it surprises you.

{{focus: good-auto}} {{annotate: good-auto "Type inferred cleanly"}} On the left — iterators, structured bindings, return types from complex expressions. `auto` is perfect here. The type is obvious from context. No information is lost.

{{focus: bad-auto}} {{annotate: bad-auto "Strips ref and const"}} On the right — assigning from a `const std::string&`. The type `auto` infers is `std::string`. Not a reference. A copy. If the string is a gigabyte, you just copied a gigabyte. The `const` and the `&` both disappear.

{{focus: bad-auto}} {{zoom: 1x}} The aha: `auto` doesn't make types disappear. It infers them from the right-hand side, and the inference rules strip qualifiers. The type is still there. You just can't see it.

{{focus: fix-auto}} {{annotate: fix-auto "Explicit qualifiers"}} `auto&` keeps the reference. `const auto&` keeps both. When you want a reference, say so explicitly. `auto` alone will always give you a value.

{{focus: none}} {{unsplit}} {{clear: slide}}

{{show: auto-rules typewriter 1.5s linear}} Here's the cheat sheet. Four cases. Memorize them and `auto` never surprises you again.

{{focus: rule-plain}} {{annotate: rule-plain "May copy heap data"}} `auto x = expr` — always a value. Copies ints and pointers cheaply. Copies strings and vectors expensively. Know which you have.

{{focus: rule-ref}} {{annotate: rule-ref "Mutable reference"}} `auto& x = expr` — a reference. Modifies through it. Works when the expression is an lvalue.

{{focus: rule-cref}} {{annotate: rule-cref "Read-only, no copy"}} `const auto& x = expr` — read-only reference. No copy. Works even with temporaries. This is the safe default for read access.

{{focus: rule-move}} {{annotate: rule-move "Sink — takes ownership"}} `auto x = std::move(y)` — moves. Takes ownership of y's resources. Use when you're done with y.

{{focus: none}} `auto` is a verbosity tool, not a type-erasure tool. Use it where the type is obvious. Add qualifiers when you need them.

```code:verbose-iter lang=cpp
#include <string>
#include <unordered_map>
#include <vector>

// Before auto: naming every type explicitly
std::unordered_map<std::string, std::vector<int>> table;
std::unordered_map<std::string, std::vector<int>>::iterator it = table.begin();
---
iter-decl: 7
```

```code:auto-split lang=cpp
#include <string>
#include <unordered_map>
#include <vector>

// Good: auto manages verbosity
auto it = table.begin();              // iterator type inferred
auto [key, value] = *it;             // structured binding
auto result = compute_sum(data);     // complex return type

// Surprising: auto strips ref and const
const std::string& get_name();
auto name = get_name();              // std::string — a COPY
const auto& name_ref = get_name();  // const std::string& — always safe
const auto& name_cref = get_name(); // const std::string& — explicit
---
good-auto: 5-8
bad-auto: 11
fix-auto: 12-13
```

```data:auto-rules type=array
["auto x = expr        → value copy (may be expensive)", "auto& x = expr       → mutable reference", "const auto& x = expr → read-only, no copy (safe default)", "auto x = std::move(y)→ move — takes ownership from y"]
---
rule-plain: 0
rule-ref: 1
rule-cref: 2
rule-move: 3
```

# const all the way down

{{clear: slide}}

{{show: const-overview typewriter 2s linear}} Bugs from accidental mutation are the worst kind — silent, runtime-only, and hard to reproduce. `const` kills them at compile time, for free.

{{focus: const-param}} {{zoom: 1.3x}} {{annotate: const-param "No copy, no mutation"}} A `const std::string&` parameter reads the caller's string without copying it and without being able to modify it. The compiler enforces both. The caller never has to wonder if their data changed.

{{zoom: 1x}} {{focus: const-method}} {{zoom: 1.3x}} {{annotate: const-method "Promise to the caller"}} A `const` method promises not to modify the object. The caller knows: calling `size()` on a `Buffer` will never change the buffer. That's a guarantee encoded in the type, not a comment.

{{zoom: 1x}} {{focus: const-member}} {{zoom: 1.3x}} {{annotate: const-member "Set once"}} A `const` data member is set in the constructor and never changed again. It's an invariant: this value doesn't drift. The compiler enforces it. You don't have to trust discipline.

{{focus: none}} {{clear: slide}}

{{show: const-cascade typewriter 1.5s linear}} Here's the design principle: push `const` as deep as it can go. When a function doesn't need to mutate, make it `const`. When a parameter is read-only, take it by `const&`. When a member never changes, make it `const`.

{{focus: non-const-mistake}} {{zoom: 1.2x}} {{annotate: non-const-mistake "Accidental mutation possible"}} A non-const reference parameter can be modified. The caller can't tell from the call site whether their data will change. They have to read the implementation. That's a leaky abstraction.

{{zoom: 1x}} {{focus: const-correct}} {{zoom: 1.2x}} {{annotate: const-correct "Self-documenting contract"}} A `const&` is self-documenting. At the call site, the reader knows nothing is modified. No implementation digging required.

{{zoom: 1x}} {{focus: none}} `const` is a compile-time contract. The compiler enforces it without runtime overhead. Use it everywhere it fits, and you eliminate an entire class of bugs before the program runs.

```code:const-overview lang=cpp
#pragma once
#include <cstdint>
#include <string>

class Buffer {
public:
  // const& param: reads caller's string, no copy, no mutation
  explicit Buffer(const std::string& label);

  // const method: promise not to modify the object
  std::size_t size() const;
  const uint8_t* data() const;

  // non-const method: explicitly allows mutation
  uint8_t* mutable_data();

private:
  const std::string label_;     // set once in constructor, never again
  std::size_t size_;
  uint8_t* data_;
};
---
const-param: 8
const-method: 11-12
const-member: 17
```

```code:const-cascade lang=cpp
#include <string>

// Non-const ref: caller can't tell if data changes
void process(std::string& s);         // mutation possible — ambiguous

// const ref: contract is clear at the call site
void process(const std::string& s);   // read-only — self-documenting

// At the call site:
std::string name = "handhold";
process(name);   // does this change name? Check the signature.
---
non-const-mistake: 3-4
const-correct: 6-7
```

# Move semantics — transfer, don't copy

{{clear: slide}}

{{show: move-problem typewriter 2s linear}} You return a `std::vector<uint8_t>` from a function. A million bytes. Does C++ copy all of it? That used to be the answer. Not anymore.

{{focus: old-return}} {{annotate: old-return "Old C++: heap copy"}} In old C++, returning by value triggered a copy. A new heap buffer allocated, a million bytes memcpy'd, the old buffer freed. Three operations for one return.

{{focus: none}} {{clear: slide}}

{{show: move-viz grow 0.5s spring}} Visualize what a vector actually is: a pointer to a heap buffer, a size, and a capacity. Three words on the stack. The data lives on the heap.

{{focus: before-transfer}} {{annotate: before-transfer "Pointer + size + cap"}} Before the move, source holds the pointer. It knows where the heap buffer is.

{{focus: after-transfer}} {{annotate: after-transfer "Pointer transferred"}} After the move, destination holds the pointer. Source's pointer is set to null. The heap buffer didn't move. Only the pointer did. Three words copied instead of a million bytes.

{{focus: none}} {{zoom: 1x}} Moving is like handing someone a book. Copying is like photocopying the book first. Modern C++ always moves when it can.

{{clear: slide}}

{{show: move-code typewriter 2s linear}} In code, `std::move` is a cast, not an operation. It tells the compiler "treat this as an rvalue." That's it. The actual transfer happens in the move constructor.

{{focus: vector-return}} {{zoom: 1.2x}} {{annotate: vector-return "NRVO or move"}} Returning a local vector. The compiler applies Named Return Value Optimization — the vector is constructed directly in the caller's slot. If that's not possible, it moves. Either way, no copy.

{{zoom: 1x}} {{focus: explicit-move}} {{zoom: 1.3x}} {{annotate: explicit-move "Cast to rvalue"}} `std::move(source)` doesn't move anything. It casts `source` to an rvalue reference. The vector's move constructor does the actual pointer transfer.

{{zoom: 1x}} {{focus: moved-from}} {{zoom: 1.2x}} {{annotate: moved-from "Valid but empty"}} After the move, `source` is in a valid-but-empty state. For `std::vector`, that means size is 0. Don't read from a moved-from object. The type doesn't stop you — discipline does.

{{zoom: 1x}} {{focus: none}} The compiler moves automatically in most return cases. `std::move` is for the cases where you need to be explicit — transferring into a container, passing to a sink function, or giving up ownership.

```data:move-viz type=array
["source: ptr=0xDEAD  size=1000000  cap=1000000", "dest:   ptr=null   size=0        cap=0", "→  std::move(source)  →", "source: ptr=null   size=0        cap=0", "dest:   ptr=0xDEAD  size=1000000  cap=1000000"]
---
before-transfer: 0, 1
after-transfer: 3, 4
```

```code:move-code lang=cpp
#include <cstdint>
#include <utility>
#include <vector>

// Return by value: NRVO eliminates the copy entirely
std::vector<uint8_t> read_page(int page_id) {
  std::vector<uint8_t> buf(4096);
  // ... fill buf ...
  return buf;                        // no copy — NRVO or implicit move
}

// Explicit move: transferring ownership to a container
std::vector<uint8_t> source(1'000'000, 0xFF);
std::vector<std::vector<uint8_t>> pages;
pages.push_back(std::move(source)); // pointer transfer, not memcpy

// After: source.size() == 0
---
vector-return: 5-9
explicit-move: 12-14
moved-from: 16
```

```code:move-problem lang=cpp
#include <vector>

// Old C++: returning large data required a heap copy
std::vector<int> load_data() {
  std::vector<int> result;
  result.reserve(1'000'000);
  // ... fill result ...
  return result;   // copies 1 million ints in old C++
}

std::vector<int> data = load_data();  // expensive copy
---
old-return: 8
old-copy: 11
```

# unique_ptr — the default owner

{{clear: slide}}

{{show: raw-problem typewriter 2s linear}} Raw `new` and `delete` are a contract enforced by nothing. You write `new`, you promise to write `delete`. Forget the `delete`, or take an early return, or throw — and you've leaked.

{{focus: raw-danger}} {{zoom: 1.3x}} {{annotate: raw-danger "Three ways to fail"}} One early return leaks. One exception leaks. One wrong path leaks. The compiler doesn't help. The linter might catch it. The sanitizer might catch it at runtime. But the type system? Silent.

{{focus: none}} {{clear: slide}}

{{show: unique-code typewriter 2s linear}} `std::unique_ptr` wraps a heap allocation. When the `unique_ptr` goes out of scope — for any reason, early return, exception, normal exit — it calls `delete`. RAII handles what discipline can't.

{{focus: make-unique}} {{zoom: 1.3x}} {{annotate: make-unique "One allocation"}} `std::make_unique<T>()` allocates and wraps in one call. No raw `new`. No possibility of writing the allocation without the wrapper.

{{zoom: 1x}} {{focus: no-copy}} {{zoom: 1.3x}} {{annotate: no-copy "Compile-time: one owner"}} You cannot copy a `unique_ptr`. That would mean two owners, two `delete` calls on the same pointer — a double-free. The compiler rejects the copy. The invariant is enforced by the type.

{{zoom: 1x}} {{focus: move-transfer}} {{zoom: 1.2x}} {{annotate: move-transfer "Ownership transferred"}} Moving transfers ownership. After the move, `p1` is null. `p2` owns the allocation. There is always exactly one owner.

{{zoom: 1x}} {{focus: auto-cleanup}} {{annotate: auto-cleanup "Destructor fires"}} When `p2` goes out of scope, the destructor fires, `delete` is called, memory is freed. No explicit cleanup code. The caller never needs to think about this.

{{focus: none}} {{clear: slide}}

{{show: ownership-tree grow 0.5s spring}} Here are the three cases.

{{focus: unique}} {{annotate: unique "Default — use this"}} `unique_ptr` by default. Sole ownership, zero overhead over a raw pointer. Covers ninety percent of cases.

{{focus: shared}} {{annotate: shared "Shared ownership"}} `shared_ptr` only when multiple owners genuinely share a resource with unknown lifetimes. Reference counting isn't free.

{{focus: raw}} {{annotate: raw "Non-owning borrow"}} Raw pointer or reference for non-owning access. Someone else owns it. You're just looking.

{{focus: none}} {{zoom: 1x}} If you see a `unique_ptr` in a codebase, you know exactly who owns that allocation. The type encodes it.

```code:raw-problem lang=cpp
void process() {
  auto* buf = new uint8_t[4096];

  if (error_condition) {
    return;   // LEAK — delete never called
  }

  use(buf);
  delete[] buf;   // only reached on the happy path
}
---
raw-danger: 4-6
```

```code:unique-code lang=cpp
#include <memory>
#include <cstdint>

void process() {
  auto buf = std::make_unique<uint8_t[]>(4096);   // allocated + wrapped

  if (error_condition) {
    return;   // OK — unique_ptr destructor fires, memory freed
  }

  use(buf.get());
  // no delete needed — cleaned up automatically
}

auto p1 = std::make_unique<int>(42);
// auto p2 = p1;                      // ERROR: cannot copy unique_ptr
auto p2 = std::move(p1);             // p1 is null, p2 owns the int
// p2 goes out of scope → memory freed
---
make-unique: 5
no-copy: 16
move-transfer: 17
auto-cleanup: 18
```

```diagram:ownership-tree
start [service]
unique [service]
shared [service]
raw [service]
start --sole owner?--> unique
start --shared ownership?--> shared
start --non-owning view?--> raw
---
unique: unique
shared: shared
raw: raw
```

# std::span — look without owning

{{clear: slide}}

{{show: pass-problem typewriter 2s linear}} You have a buffer. A function needs to read it. Do you pass a `const uint8_t*` and a length? A `const std::vector<uint8_t>&`? Make a copy? Each choice has problems.

{{focus: ptr-plus-len}} {{annotate: ptr-plus-len "Pointer + length: error prone"}} Pointer plus length is the C approach. Nothing stops the caller from passing the wrong length. Nothing stops you from going past the end. Undefined behavior is silent.

{{focus: vector-ref}} {{annotate: vector-ref "vector ref: too specific"}} A `const std::vector<uint8_t>&` forces the caller to have a vector. Arrays don't work. Stack buffers don't work. It's artificially restrictive.

{{focus: none}} {{clear: slide}}

{{show: span-code typewriter 2s linear}} `std::span<const uint8_t>` is a non-owning view. A pointer and a length, packaged safely. It constructs from anything contiguous — vector, array, raw buffer. No allocation.

{{focus: span-param}} {{zoom: 1.3x}} {{annotate: span-param "Zero overhead view"}} The function takes a `span`. It reads bytes. It doesn't care whether the caller has a vector or a stack array or a memory-mapped file. All of those are contiguous memory.

{{zoom: 1x}} {{focus: span-from-vec}} {{zoom: 1.2x}} {{annotate: span-from-vec "Implicit from vector"}} A `span` constructs implicitly from a vector. No explicit conversion. The caller passes `buf` and the compiler handles it.

{{zoom: 1x}} {{focus: span-from-arr}} {{zoom: 1.2x}} {{annotate: span-from-arr "From stack array too"}} From a `std::array`, same syntax. The span wraps both with one interface.

{{zoom: 1x}} {{focus: span-subspan}} {{zoom: 1.3x}} {{annotate: span-subspan "Window into buffer"}} `subspan(offset, length)` gives you a view of a sub-range. A window into the buffer. No copy, no allocation, no new ownership.

{{focus: none}} {{clear: slide}}

{{show: page-preview typewriter 1.5s linear}} Here's a preview of what we'll write. A 4096-byte page buffer on the heap, owned by a `unique_ptr`. A `span` lets any function look at those bytes without taking ownership.

{{focus: page-buf}} {{zoom: 1.3x}} {{annotate: page-buf "Stack size, heap data"}} The `Page` struct is small on the stack. The bytes live on the heap. The `unique_ptr` owns them.

{{zoom: 1x}} {{focus: span-window}} {{zoom: 1.3x}} {{annotate: span-window "Borrow, don't own"}} `as_span()` returns a view of the page bytes. The caller can read and write through the span. They don't own the bytes. The `unique_ptr` still does.

{{zoom: 1x}} {{focus: string-view}} {{zoom: 1.2x}} {{annotate: string-view "Same idea, for strings"}} `std::string_view` is the same concept for strings. A non-owning view of a character sequence. Works with `std::string`, string literals, everything.

{{zoom: 1x}} {{focus: none}} Views let you look at memory you don't own. We'll use `span` on every page operation in this course.

```code:pass-problem lang=cpp
#include <cstdint>
#include <vector>

// C approach: pointer + length. Nothing enforces bounds.
void read_bytes(const uint8_t* data, std::size_t len);

// Too specific: caller must have a vector.
void read_bytes(const std::vector<uint8_t>& data);
---
ptr-plus-len: 4-5
vector-ref: 7-8
```

```code:span-code lang=cpp
#include <array>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

// Works with anything contiguous — caller decides the storage
void read_bytes(std::span<const uint8_t> data) {
  for (uint8_t byte : data) { /* ... */ }
  auto header = data.subspan(0, 4);      // first 4 bytes — no copy
}

// All of these compile:
std::vector<uint8_t> buf(4096);
read_bytes(buf);                          // from vector

std::array<uint8_t, 4096> arr{};
read_bytes(arr);                          // from stack array

uint8_t raw[4096];
read_bytes(std::span{raw, 4096});         // from raw buffer

// string_view: same idea for strings
void log(std::string_view msg);           // works with string, literal, etc.
---
span-param: 7-9
span-from-vec: 13-14
span-from-arr: 16-17
span-subspan: 10
string-view: 22-24
```

```code:page-preview lang=cpp
#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <span>
#include <string_view>

static constexpr std::size_t kPageSize = 4096;

struct Page {
  std::unique_ptr<std::array<uint8_t, kPageSize>> bytes;

  std::span<uint8_t> as_span() {
    return std::span{*bytes};
  }

  std::span<const uint8_t> as_span() const {
    return std::span{*bytes};
  }
};

// Non-owning view of a char sequence — same idea as span
void log(std::string_view msg);
---
page-buf: 10-12
span-window: 13-15
string-view: 23
```

# The ownership rule

{{clear: slide}}

{{show: chaos-diagram grow 0.5s spring}} In a complex system, ownership questions become hard. Who frees the page buffer? The B-tree that read it, or the page manager that allocated it? When you lose track, you get double-frees and leaks.

{{focus: unclear-owner}} {{annotate: unclear-owner "Who cleans up?"}} No single answer. Two components think they own it. Or neither does. Both outcomes are bugs.

{{focus: none}} The rule is simple. Every object has exactly one owner at all times. Always.

{{clear: slide}}

{{show: rule-summary typewriter 2s linear}} Three tools. Three relationships. No ambiguity.

{{focus: own-rule}} {{zoom: 1.3x}} {{annotate: own-rule "Sole owner"}} `unique_ptr` encodes ownership. When you see one, you know: this component is responsible for this allocation. When it's destroyed, the allocation is freed. No negotiation.

{{zoom: 1x}} {{focus: move-rule}} {{zoom: 1.3x}} {{annotate: move-rule "Transfer ownership"}} `std::move` transfers ownership from one `unique_ptr` to another. After the transfer, the source is null. There's still exactly one owner.

{{zoom: 1x}} {{focus: borrow-rule}} {{zoom: 1.3x}} {{annotate: borrow-rule "Borrow without owning"}} `const&` and `span` are borrows. The function reads or writes through the reference, but ownership doesn't change. The caller still owns the data.

{{focus: none}} {{clear: slide}}

{{show: system-diagram grow 0.5s spring}} Here's the ownership map for the system we're about to build.

{{focus: page-mgr}} {{annotate: page-mgr "Single owner"}} The `PageManager` owns every page. It allocates them, frees them, decides which page maps to which ID. Nothing else calls `new` or `delete` on page memory.

{{focus: btree-node}} {{annotate: btree-node "Borrows pages"}} The B-tree borrows pages from the `PageManager`. It reads and writes through a `span`. It never frees a page. It can't — it doesn't own them.

{{focus: exec-node}} {{annotate: exec-node "Borrows the tree"}} The executor borrows the B-tree. A `const&` for reads, a reference for writes. It executes queries. It doesn't own the tree.

{{focus: none}} {{zoom: 1x}} If you know who owns what, you know who cleans up. And in this system, ownership is explicit in every type signature.

{{clear: slide}}

{{show: final-code typewriter 2s linear}} The ownership rule in code looks like this. The `PageManager` holds the pages. Everything else borrows.

{{focus: manager-owns}} {{zoom: 1.2x}} {{annotate: manager-owns "Owner at the top"}} The `PageManager` stores pages in a `vector` of `unique_ptr`. It is the single source of truth for page lifetimes.

{{zoom: 1x}} {{focus: btree-borrows}} {{zoom: 1.2x}} {{annotate: btree-borrows "Reference, not ownership"}} The `BTree` receives a reference to the manager. Not a `unique_ptr`, not a copy. A reference. The `BTree` can use the manager, but doesn't own it. The manager outlives the tree by contract.

{{zoom: 1x}} {{focus: span-borrow}} {{zoom: 1.3x}} {{annotate: span-borrow "Borrow a page"}} `get_page()` returns a `span` — a window into the page bytes. The `BTree` can read and write through the span. The `PageManager` still owns the allocation.

{{zoom: 1x}} {{focus: none}} Ownership is design. When the ownership map is clear, the cleanup code writes itself. That's the system we're about to build.

```code:rule-summary lang=cpp
#include <cstdint>
#include <memory>
#include <span>
#include <string>

class Resource {
public:
  explicit Resource(std::size_t size);
  std::span<uint8_t> view();
};

// Ownership: unique_ptr holds the allocation
auto owner = std::make_unique<Resource>(4096);

// Transfer: std::move passes the baton
auto new_owner = std::move(owner);    // owner is null after this

// Borrow: const& and span — no ownership change
void read(const Resource& r);        // borrows, can't take ownership
void process(std::span<uint8_t> s);  // view into bytes, no ownership
---
own-rule: 13
move-rule: 16
borrow-rule: 18-19
```

```code:final-code lang=cpp
#pragma once
#include <cstdint>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

static constexpr std::size_t kPageSize = 4096;
using PageId = uint32_t;

class PageManager {
public:
  std::span<uint8_t> get_page(PageId id);
  PageId allocate_page();

private:
  // Owns every page — sole allocator and deallocator
  std::unordered_map<PageId, std::unique_ptr<std::vector<uint8_t>>> pages_;
};

class BTree {
public:
  // Borrows the manager — does not own it
  explicit BTree(PageManager& manager) : manager_(manager) {}

  void insert(std::span<const uint8_t> key,
              std::span<const uint8_t> value);

private:
  PageManager& manager_;   // reference, not unique_ptr
};
---
manager-owns: 16-18
btree-borrows: 22-23
span-borrow: 13
```

```diagram:chaos-diagram
raw [service]
btree [service]
manager [service]
raw --owns?--> btree
raw --owns?--> manager
---
unclear-owner: raw
```

```diagram:system-diagram
PageManager [service]
Page [service]
BTree [service]
Executor [service]
PageManager --owns--> Page
BTree --borrows span--> PageManager
Executor --borrows ref--> BTree
---
page-mgr: PageManager
btree-node: BTree
exec-node: Executor
```
