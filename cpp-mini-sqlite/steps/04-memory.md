---
title: Memory without fear
---

# Stack vs heap

{{clear: slide}}

{{show: alloc-compare typewriter 2s linear}} Every C++ value lives in one of two places: the stack or the heap. These two lines make the difference concrete.

{{focus: stack-alloc}} {{annotate: stack-alloc "Compiler manages lifetime"}} The stack version — `std::array<uint8_t, 4096> buf{}` — requires the size to be known at compile time. The compiler allocates the buffer when the function is called and destroys it when the function returns. You pay nothing at runtime for that lifecycle.

{{focus: heap-alloc}} {{annotate: heap-alloc "Manual delete required"}} The heap version — `new uint8_t[4096]` — works even when the size is only known at runtime. But ownership is now yours entirely. That `delete[]` on line 6 is not optional.

{{focus: heap-alloc}} Missing `delete[]` is a memory leak: the program holds memory it will never return to the OS. Calling `delete[]` twice is undefined behavior: the allocator's internal bookkeeping is corrupted, and anything can happen next.

{{focus: none}} {{clear: slide}}

{{show: mem-regions typewriter 2s linear}} The performance difference is not subtle. The stack is deterministic and has zero allocation overhead — the compiler adjusts the stack pointer at compile time. The heap is flexible, but `malloc` and `new` carry lock contention, fragmentation overhead, and roughly 50–100 nanoseconds of latency per call.

{{focus: stack-row}} {{annotate: stack-row "Zero allocation latency"}} Stack allocation is a single pointer decrement. No lock, no metadata, no bookkeeping. The cost is already paid at function entry.

{{focus: heap-row}} {{annotate: heap-row "Allocator has nontrivial cost"}} Heap allocation goes through the global allocator, which must find a free block of the right size, potentially acquiring a lock, and write metadata adjacent to the allocation. That latency adds up inside a hot database query loop.

{{focus: none}} {{clear: slide}}

```code:alloc-compare lang=cpp
// Stack allocation: size known at compile time, freed at scope end.
std::array<uint8_t, 4096> buf{};

// Heap allocation: size can be a runtime variable — here using 4096 to match.
auto* raw = new uint8_t[4096];
delete[] raw;
---
stack-alloc: 1-2
heap-alloc: 4-6
```

```data:mem-regions type=array
["stack  │ fixed size, auto cleanup │ deterministic, zero allocation overhead", "heap   │ dynamic size, manual free │ flexible, but allocator has latency"]
---
stack-row: 0
heap-row: 1
```

# RAII — owning through lifetime

{{clear: slide}}

{{show: raii-class typewriter 2s linear}} {{zoom: 1.2x}} `PageManager` used a pattern without naming it. That pattern is RAII — Resource Acquisition Is Initialization. Here it is in the abstract, applied to a `FILE*`.

{{focus: ctor}} {{zoom: 1.2x}} {{annotate: ctor "Acquire in constructor"}} The constructor calls `std::fopen`. From the moment the `FileHandle` object exists, the file is open. There is no "constructed but not yet opened" state. The invariant is: if the object exists, the resource is valid.

{{zoom: 1x}} {{focus: dtor}} {{zoom: 1.2x}} {{annotate: dtor "Release in destructor"}} The destructor calls `std::fclose`. It doesn't matter how the object is destroyed — scope exit, stack unwind, `delete`, the enclosing object's destructor — `std::fclose` runs unconditionally. You cannot forget it.

{{zoom: 1x}} {{focus: delete-copy}} {{zoom: 1.2x}} {{annotate: delete-copy "No copies — double-close"}} Copying a `FileHandle` would duplicate the `FILE*`. Both copies would call `std::fclose` on the same handle — double-close, undefined behavior. So the copy constructor and copy assignment are deleted.

{{zoom: 1x}} {{focus: get-fn}} {{zoom: 1.2x}} {{annotate: get-fn "Caller borrows, owner keeps"}} `get()` returns the raw pointer for callers that need it. The `FileHandle` still owns the resource — the caller only borrows it for the duration of a call. When the `FileHandle` is destroyed, the pointer becomes invalid.

{{zoom: 1x}} {{focus: file-field}} {{zoom: 1.2x}} {{annotate: file-field "Private: ownership is hidden"}} `file_` is private. No caller can reassign it, null it, or free it. The class enforces the invariant by hiding the resource behind controlled access.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

{{show: raii-class none 0s}} `PageManager` is exactly `FileHandle` with a richer interface. The constructor opens the file; the destructor closes it; the members read and write. The RAII pattern is the same. Now you see why `PageManager` has no `open()` or `close()` methods — those states don't exist.

{{focus: dtor}} The destructor always runs. Stack unwinding during an exception, early return from a function, the object going out of scope — all of these trigger the destructor. RAII makes cleanup automatic by encoding it in a type lifecycle.

{{focus: none}} {{clear: slide}}

```code:raii-class lang=cpp
// The resource (FILE*) is acquired in the constructor,
// released in the destructor. Lifetime = ownership.
struct FileHandle {
  explicit FileHandle(const char* path)
      : file_(std::fopen(path, "rb")) {}

  ~FileHandle() {
    if (file_) std::fclose(file_);
  }

  // Copying would duplicate FILE* — both destructors would fclose it.
  FileHandle(const FileHandle&) = delete;
  FileHandle& operator=(const FileHandle&) = delete;

  FILE* get() const { return file_; }

 private:
  FILE* file_;
};
---
ctor: 4-5
dtor: 7-9
delete-copy: 12-13
get-fn: 15
file-field: 18
```

# std::unique_ptr — the modern owner

{{clear: slide}}

{{show: unique-ptr-code typewriter 2s linear}} Writing a custom RAII wrapper for every resource type is unnecessary. The standard library provides `std::unique_ptr` — a generic RAII owner for any heap-allocated object.

{{focus: make-unique}} {{annotate: make-unique "Factory, not new"}} `std::make_unique<T>()` allocates on the heap and wraps the result in a `unique_ptr`. There is no raw `new`, and there will be no raw `delete`. The destructor handles it.

{{focus: move-ownership}} {{annotate: move-ownership "Transfer, not copy"}} `unique_ptr` is move-only: copying is deleted. `std::move(buf)` transfers ownership to `buf2`. After the move, `buf` is null — it no longer owns anything. There is exactly one owner at every moment.

{{focus: move-ownership}} `std::move` does not move anything by itself. It is a cast to an rvalue reference — a signal to the compiler that the source can be moved from. The actual transfer happens inside the `unique_ptr` move constructor.

{{focus: auto-cleanup}} {{annotate: auto-cleanup "Destructor fires at scope end"}} When `buf2` goes out of scope, its destructor runs and frees the array. No `delete`, no `delete[]`, no manual cleanup. The resource lifetime is tied to the variable lifetime.

{{focus: none}} {{clear: slide}}

{{show: unique-ptr-code none 0s}} `unique_ptr` is the right default for heap-allocated ownership. Use it instead of raw `new`/`delete` everywhere. The only time to reach for a raw pointer is when you are expressing non-owning access — borrowing a resource that someone else owns.

{{focus: make-unique}} The return type is `std::unique_ptr<std::array<uint8_t, 4096>>`. The `auto` deduces that type. The inner array lives on the heap; the `unique_ptr` itself is a pointer-sized value that can live on the stack.

{{focus: none}} {{clear: slide}}

```code:unique-ptr-code lang=cpp
#include <memory>

// Heap allocate with automatic cleanup — no delete needed.
auto buf = std::make_unique<std::array<uint8_t, 4096>>();

// unique_ptr is move-only: copy is deleted, move transfers ownership.
auto buf2 = std::move(buf);  // buf is now null; buf2 owns the array.

// buf2's destructor runs at scope end — array is freed automatically.
---
make-unique: 3-4
move-ownership: 6-7
auto-cleanup: 9
```

# Move semantics — ownership without copying

{{clear: slide}}

{{show: move-ctor typewriter 2s linear}} To understand why `unique_ptr` is move-only, you need to see what a move constructor does. Here is a `Buffer` class with an explicit one.

{{focus: normal-ctor}} {{zoom: 1.2x}} {{annotate: normal-ctor "Allocates owned array"}} The regular constructor allocates a heap array and stores the pointer. `data_` is the owner. The invariant: `data_` is non-null for any valid `Buffer`.

{{zoom: 1x}} {{focus: move-ctor-fn}} {{zoom: 1.2x}} {{annotate: move-ctor-fn "Takes ownership from other"}} The move constructor signature is `Buffer(Buffer&& other)`. The `&&` means the caller is passing an rvalue — something it is willing to give up. `noexcept` is required for standard containers to use move instead of copy.

{{zoom: 1x}} {{focus: steal-ptr}} {{zoom: 1.2x}} {{annotate: steal-ptr "Copy the pointer"}} The initializer list copies `other.data_` and `other.size_` into this object. At this point, both `this` and `other` hold the same pointer — a dangerous state that must be resolved immediately.

{{zoom: 1x}} {{focus: null-out}} {{zoom: 1.2x}} {{annotate: null-out "Null the source"}} `other.data_ = nullptr` and `other.size_ = 0` resolve the danger. Now only `this` owns the buffer. When `other`'s destructor runs, `delete[] nullptr` is a no-op — safe and defined.

{{zoom: 1x}} {{focus: dtor}} {{zoom: 1.2x}} {{annotate: dtor "delete[] nullptr is safe"}} The destructor runs for every `Buffer` — moved-from ones included. Because the move constructor nulled out the source, `delete[] nullptr` fires for the moved-from object. No double free.

{{zoom: 1x}} {{focus: data-fields}} {{zoom: 1.2x}} {{annotate: data-fields "Two fields, one owner"}} The class has exactly two fields. Ownership is singular: at any moment, exactly one `Buffer` holds a non-null `data_`. Move transfers that singularity without copying a single byte of the array.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

{{show: move-ctor none 0s}} This is exactly what `std::unique_ptr`'s move constructor does — copy the raw pointer, null the source. The only difference is that `unique_ptr` is a template and handles arbitrary types. The mechanism is identical.

{{focus: null-out}} If you forget to null the source, the moved-from `Buffer`'s destructor runs `delete[]` on the same pointer that the new owner also holds. That is a double free — undefined behavior, typically a crash or heap corruption. Nulling the source is not optional.

{{focus: none}} {{clear: slide}}

```code:move-ctor lang=cpp
class Buffer {
 public:
  explicit Buffer(size_t n) : data_(new uint8_t[n]), size_(n) {}

  // Move constructor: take the pointer, null out the source.
  // Postcondition: other.data_ == nullptr (caller must not use other).
  Buffer(Buffer&& other) noexcept
      : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
  }

  ~Buffer() { delete[] data_; }

 private:
  uint8_t* data_;
  size_t   size_;
};
---
normal-ctor: 3
move-ctor-fn: 7-11
steal-ptr: 8
null-out: 9-10
dtor: 13
data-fields: 16-17
```

# Arena allocation — bump the pointer

{{clear: slide}}

{{split}}

{{show: arena-code typewriter 2s linear}} {{show: arena-diagram typewriter 2s linear}} {{zoom: 1.2x}} An `Arena` is a block allocator: allocate one large buffer up front, then hand out slices of it one at a time. This is the concept you will implement in Lab 4.

{{focus: arena-ctor}} {{annotate: arena-ctor "One allocation, up front"}} The constructor allocates `capacity` bytes once. Every subsequent `alloc` call carves out a slice of that buffer. There is one heap allocation total, no matter how many objects you allocate from the arena.

{{focus: arena-dtor}} {{annotate: arena-dtor "One free for everything"}} The destructor frees the entire buffer in one `delete[]`. Every object ever allocated from this arena is freed simultaneously. There is no need to track individual allocations.

{{focus: delete-copy}} {{annotate: delete-copy "No copies — double-free"}} Copying an `Arena` would duplicate `buf_`. Both destructors would call `delete[]` on the same pointer — a double-free. So copy construction and copy assignment are deleted.

{{focus: alloc-fn}} {{annotate: alloc-fn "Bump-pointer allocation"}} `alloc(n)` does three things: bounds check, pointer arithmetic, counter increment. That's the entire allocator. No linked list of free blocks, no coalescing, no lock.

{{focus: bounds-check}} {{annotate: bounds-check "Out of space returns null"}} If `used_ + n` would exceed `capacity_`, return `nullptr`. The caller must handle allocation failure. The arena never overflows its buffer.

{{focus: bump-ptr}} {{annotate: bump-ptr "O(1) pointer advance"}} `buf_ + used_` is the next free address. Incrementing `used_` by `n` reserves the slice. The pointer just bumps forward — hence "bump-pointer allocator."

{{focus: initial-state}} {{annotate: initial-state "Buffer empty at start"}} The arena starts fully empty. `used_` is zero; all bytes are available.

{{focus: after-alloc}} {{annotate: after-alloc "Slice claimed by alloc(12)"}} After `alloc(12)`, `used_` advances by 12 bytes. The allocated region is marked. The remainder is still free.

{{focus: reset-fn}} {{annotate: reset-fn "Reset in O(1)"}} `reset()` sets `used_` back to zero. Every allocation is instantly "freed" — no loop, no individual frees, no bookkeeping. The buffer is ready to be reused from the start.

{{focus: after-reset}} {{annotate: after-reset "Entire buffer reclaimed"}} After `reset()`, the diagram shows a fully empty buffer again. The bytes are not zeroed — they still contain old data — but from the allocator's point of view, the space is free.

{{focus: arena-fields}} {{annotate: arena-fields "Three fields, one pointer"}} The arena holds three fields: the buffer pointer, the capacity, and the bump counter `used_`. The bump counter is the allocator's entire state.

{{zoom: 1x}} {{focus: none}} {{unsplit}} {{clear: slide}}

```code:arena-code lang=cpp
class Arena {
 public:
  explicit Arena(size_t capacity)
      : buf_(new uint8_t[capacity]), capacity_(capacity), used_(0) {}

  ~Arena() { delete[] buf_; }

  // Copying would duplicate buf_ — both destructors would delete[] it.
  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  // Bump-pointer allocation: O(1), no lock, no fragmentation.
  // Returns null if out of space.
  void* alloc(size_t n) {
    if (used_ + n > capacity_) return nullptr;
    void* ptr = buf_ + used_;
    used_ += n;
    return ptr;
  }

  // Free all allocations in O(1) — just reset the pointer.
  void reset() { used_ = 0; }

 private:
  uint8_t* buf_;
  size_t   capacity_;
  size_t   used_;   // bytes in use (the bump pointer)
};
---
arena-ctor: 3-4
arena-dtor: 6
delete-copy: 9-10
alloc-fn: 14-19
bounds-check: 15
bump-ptr: 16-17
reset-fn: 22
arena-fields: 25-27
```

```data:arena-diagram type=array
["[XXXXXXXX                    ]   used=8,  free=capacity-8", "[XXXXXXXXXXXXXXXXXXXX        ]   used=20, free=capacity-20  (after alloc(12))", "[                            ]   used=0,  free=capacity    (after reset())"]
---
initial-state: 0
after-alloc: 1
after-reset: 2
```

# Why arenas win in databases

{{clear: slide}}

{{show: alloc-compare-table typewriter 2s linear}} Three operations, three cost profiles. The numbers explain why every high-performance database uses an arena for per-query state.

{{focus: malloc-row}} {{annotate: malloc-row "~50-100 ns per call"}} Each `malloc` or `new` call acquires the global allocator lock, searches a free list, carves a block, and writes metadata adjacent to the allocation. A query that allocates 500 nodes on a B-tree traversal pays that cost 500 times.

{{focus: arena-row}} {{annotate: arena-row "~1 ns per call"}} Each arena `alloc` is an addition and an increment — two instructions. No lock, no search, no metadata. The same 500-node traversal pays roughly 500 nanoseconds total instead of 25–50 microseconds.

{{focus: reset-row}} {{annotate: reset-row "One assignment, all freed"}} `arena.reset()` sets one counter to zero. Every allocation ever made from that arena is freed in that single instruction. The allocator does not know or care what was allocated — it just resets the watermark.

{{focus: none}} {{clear: slide}}

{{show: alloc-compare-table none 0s}} Database queries have bounded lifetimes. A query starts, traverses B-tree nodes, produces a result, and ends. Every object it allocates serves only that query. At query end, all of it can be discarded at once.

{{focus: reset-row}} Replacing hundreds of individual `free()` calls with one `reset()` is not a micro-optimization — it is a structural insight about query lifetime. The arena models that lifetime directly in the allocator.

{{focus: none}} SQLite, PostgreSQL, and most high-performance databases use arena allocators — or their close cousins, memory pools — for per-query state. In Lab 4 you will implement an `Arena` like the one shown here. Later lessons will use it for per-query allocations as the engine gets more complex.

{{focus: none}} {{clear: slide}}

```data:alloc-compare-table type=array
["malloc/new  │ ~50-100 ns each  │ global lock, metadata overhead, fragmentation", "arena alloc │ ~1 ns each       │ one pointer add, no lock, no metadata", "arena reset │ ~1 ns total      │ frees all at once — reuse the same buffer"]
---
malloc-row: 0
arena-row: 1
reset-row: 2
```
