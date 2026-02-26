---
title: Smart pointers and RAII
---

# The RAII idiom

{{show: raii-diagram grow 0.5s spring}} RAII stands for Resource Acquisition Is Initialization. The name is terrible. The idea is simple.

{{focus: acquire}} {{annotate: acquire "Constructor acquires"}} When an object is created, its constructor acquires the resource. Memory, a file handle, a network connection.

{{focus: release}} {{annotate: release "Destructor releases"}} When the object is destroyed, its destructor releases the resource. Automatically. No manual cleanup.

{{focus: scope}} {{annotate: scope "Scope = lifetime"}} The object's scope controls the resource's lifetime. When the variable goes out of scope, the destructor runs. The resource is freed. No leaks. No forgetting.

{{focus: none}} This is the most important pattern in C++. Every bug from the last lesson — leaks, double frees, use after free — RAII prevents them all.

```diagram:raii-diagram
constructor [service]
use [service]
destructor [service]
constructor --acquires--> use
use --scope ends--> destructor
{RAII Lifetime: constructor, use, destructor}
---
acquire: constructor
release: destructor
scope: use
```

# unique_ptr

{{clear: slide}}

{{show: unique-code typewriter 2s linear}} unique ptr is the standard smart pointer. It owns heap memory exclusively. When it dies, the memory is freed.

{{focus: make-unique}} {{zoom: 1.3x}} {{annotate: make-unique "Allocate safely"}} make unique allocates on the heap and wraps the pointer in a unique ptr. No raw new. No chance of forgetting delete.

{{zoom: 1x}} {{focus: use-ptr}} {{zoom: 1.2x}} {{annotate: use-ptr "Use like a pointer"}} You use it like a raw pointer. Arrow for member access. Star for dereference. The syntax is identical.

{{zoom: 1x}} {{focus: no-copy}} {{zoom: 1.3x}} {{annotate: no-copy "Cannot copy"}} You cannot copy a unique ptr. Copy would mean two owners of the same memory. Two deletes. Double free. The compiler prevents this.

{{zoom: 1x}} {{focus: move-line}} {{zoom: 1.3x}} {{annotate: move-line "Transfer ownership"}} You can move it. std move transfers ownership from one unique ptr to another. After the move, the source is null. One owner at all times.

{{zoom: 1x}} {{focus: auto-free}} {{annotate: auto-free "Automatic cleanup"}} When p2 goes out of scope, the memory is freed. No delete. No cleanup code. RAII handles it.

{{zoom: 1x}} {{focus: none}} unique ptr is the default smart pointer. Use it unless you need shared ownership.

```code:unique-code lang=cpp
#include <memory>
#include <string>

auto p = std::make_unique<int>(42);

std::cout << *p << "\n";         // 42

// auto p2 = p;                  // ERROR: cannot copy unique_ptr

auto p2 = std::move(p);         // p is now null, p2 owns the int
// p is nullptr after move

// when p2 goes out of scope → memory freed automatically
---
make-unique: 4
use-ptr: 6
no-copy: 8
move-line: 10
auto-free: 13
```

# Move semantics

{{clear: slide}}

{{show: move-viz grow 0.5s spring}} Move semantics let you transfer resources instead of copying them.

{{focus: before-move}} {{annotate: before-move "Before"}} Before the move, source owns the data. It holds a pointer to heap memory.

{{focus: after-move}} {{annotate: after-move "After"}} After the move, destination owns the data. Source is empty. The pointer was transferred. No data was copied.

{{focus: none}} Moving is like handing someone a book. Copying is like photocopying the book then handing them the photocopy. Moving is cheaper.

{{clear: slide}}

{{show: move-code typewriter 1.5s linear}} In code, std move casts an lvalue to an rvalue reference, enabling the move.

{{focus: string-move}} {{zoom: 1.2x}} {{annotate: string-move "Transfer the buffer"}} Moving a string transfers the internal character buffer. The source string becomes empty. No allocation, no copy.

{{zoom: 1x}} {{focus: vector-move}} {{zoom: 1.2x}} {{annotate: vector-move "Transfer the array"}} Moving a vector transfers the backing array. A million elements moved in constant time.

{{zoom: 1x}} {{focus: after-state}} {{zoom: 1.3x}} {{annotate: after-state "Source is empty"}} After a move, the source is in a "valid but unspecified state." For standard types, that means empty. Don't read from a moved-from object.

{{zoom: 1x}} {{focus: none}} Move semantics are the reason modern C++ is fast without sacrificing safety.

```data:move-viz type=array
["source: [data ●]", "destination: [empty]", "→ move →", "source: [empty]", "destination: [data ●]"]
---
before-move: 0, 1
after-move: 3, 4
```

```code:move-code lang=cpp
#include <string>
#include <vector>
#include <utility>

std::string a = "hello world";
std::string b = std::move(a);   // a is now empty, b owns "hello world"

std::vector<int> v1 = {1, 2, 3, 4, 5};
std::vector<int> v2 = std::move(v1);  // v1 is now empty

// After move: source is valid but empty
// a == ""
// v1.size() == 0
---
string-move: 5-6
vector-move: 8-9
after-state: 11-13
```

# shared_ptr

{{clear: slide}}

{{show: shared-code typewriter 1.5s linear}} Sometimes multiple owners need the same resource. shared ptr uses reference counting.

{{focus: make-shared}} {{zoom: 1.2x}} {{annotate: make-shared "One allocation"}} make shared allocates the object and the reference count in one block. Efficient.

{{zoom: 1x}} {{focus: copy-shared}} {{zoom: 1.3x}} {{annotate: copy-shared "Refcount: 2"}} Copying a shared ptr increments the reference count. Both p1 and p2 point to the same int. Refcount is 2.

{{zoom: 1x}} {{focus: scope-end}} {{zoom: 1.2x}} {{annotate: scope-end "Refcount: 1"}} When p2 goes out of scope, the refcount drops to 1. The memory stays alive — p1 still needs it.

{{zoom: 1x}} {{focus: last-owner}} {{zoom: 1.3x}} {{annotate: last-owner "Refcount: 0 → freed"}} When p1 goes out of scope, the refcount drops to 0. The memory is freed. The last owner cleans up.

{{zoom: 1x}} {{focus: none}} shared ptr is for shared ownership. Use it when multiple parts of your program need the same resource and you can't determine which one dies last.

```code:shared-code lang=cpp
#include <memory>

auto p1 = std::make_shared<int>(42);   // refcount: 1

{
    auto p2 = p1;                       // refcount: 2
    std::cout << *p2 << "\n";           // 42
}   // p2 destroyed → refcount: 1

// p1 still valid, refcount: 1
std::cout << *p1 << "\n";              // 42

// when p1 goes out of scope → refcount: 0 → freed
---
make-shared: 3
copy-shared: 6
scope-end: 8
last-owner: 13
```

# weak_ptr and cycles

{{clear: slide}}

{{show: cycle-diagram grow 0.5s spring}} shared ptr has a problem. Circular references leak.

{{focus: node-a}} {{annotate: node-a "Refcount: 1"}} Node A holds a shared ptr to B. Node B holds a shared ptr to A. Each keeps the other alive.

{{focus: node-b}} {{annotate: node-b "Refcount: 1"}} When you drop your external references, neither refcount reaches zero. Both nodes leak. Memory is never freed.

{{focus: none}} This is a cycle. shared ptr can't handle it.

{{clear: slide}}

{{show: weak-code typewriter 1.5s linear}} weak ptr breaks cycles. It observes without owning.

{{focus: weak-decl}} {{zoom: 1.3x}} {{annotate: weak-decl "Non-owning"}} A weak ptr doesn't increment the refcount. It can see the object but doesn't keep it alive.

{{zoom: 1x}} {{focus: lock-line}} {{zoom: 1.2x}} {{annotate: lock-line "Temporary ownership"}} To use a weak ptr, call lock. It returns a shared ptr if the object is alive, or nullptr if it's been freed.

{{zoom: 1x}} {{focus: none}} Use weak ptr to break ownership cycles in graph-like structures.

```diagram:cycle-diagram
A [service]
B [service]
A --shared_ptr--> B
B --shared_ptr--> A
---
node-a: A
node-b: B
```

```code:weak-code lang=cpp
#include <memory>

auto shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;       // doesn't increment refcount

if (auto locked = weak.lock()) {        // temporary shared_ptr
    std::cout << *locked << "\n";       // 42
}

shared.reset();                         // refcount → 0, freed

if (auto locked = weak.lock()) {
    // won't enter — object is gone
}
---
weak-decl: 4
lock-line: 6
```

# Rule of Zero

{{clear: slide}}

{{show: rule-zero typewriter 1.5s linear}} If your class uses smart pointers and standard containers, you don't need to write a destructor, copy constructor, or move constructor. The defaults work.

{{focus: good-class}} {{zoom: 1.2x}} {{annotate: good-class "Zero special members"}} This class has a unique ptr and a vector. Both clean up automatically. No destructor needed. No copy constructor. No move constructor. The compiler generates correct defaults.

{{zoom: 1x}} {{focus: auto-cleanup}} {{zoom: 1.3x}} {{annotate: auto-cleanup "RAII all the way"}} When a Document goes out of scope, the compiler destroys data first, then content. Each member cleans up its own resources. Correct. Automatic.

{{zoom: 1x}} {{focus: none}} The Rule of Zero: prefer classes that don't need special member functions. Use smart pointers and containers, and let RAII do the work. If you're writing a destructor, ask why.

```code:rule-zero lang=cpp
#include <memory>
#include <string>
#include <vector>

class Document {
public:
    Document(std::string title)
        : title_(std::move(title))
        , content_(std::make_unique<std::string>())
        , data_(std::vector<int>{}) {}

    // No destructor needed
    // No copy/move constructors needed
    // The compiler generates correct defaults

private:
    std::string title_;
    std::unique_ptr<std::string> content_;
    std::vector<int> data_;
};

// When a Document goes out of scope:
// 1. data_ (vector) frees its backing array
// 2. content_ (unique_ptr) frees its string
// 3. title_ (string) frees its buffer
---
good-class: 5-20
auto-cleanup: 22-25
```

# The decision tree

{{clear: slide}}

{{show: decision grow 0.5s spring}} Which pointer when?

{{focus: unique-choice}} {{annotate: unique-choice "Default"}} Start with unique ptr. Sole ownership. Zero overhead. This covers ninety percent of cases.

{{focus: shared-choice}} {{annotate: shared-choice "Shared ownership"}} Use shared ptr only when multiple owners genuinely need the resource. Reference counting has overhead. Don't pay for what you don't use.

{{focus: weak-choice}} {{annotate: weak-choice "Break cycles"}} Use weak ptr to break cycles in shared ptr graphs. That's its only job.

{{focus: raw-choice}} {{annotate: raw-choice "Non-owning view"}} Raw pointers are for non-owning access. When someone else owns the object and you just need to look at it. Never use raw new and delete for ownership.

{{focus: none}} Default to unique ptr. Escalate to shared ptr only when you must. Never use raw new and delete for ownership.

```data:decision type=array
["unique_ptr — sole owner (default)", "shared_ptr — shared ownership", "weak_ptr — break cycles", "T* — non-owning view only"]
---
unique-choice: 0
shared-choice: 1
weak-choice: 2
raw-choice: 3
```
