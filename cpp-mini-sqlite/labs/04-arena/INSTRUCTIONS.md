# Lab 04: Arena allocator

## Context

In Lesson 4 you saw an `Arena` — a bump-pointer allocator that replaces
many small heap allocations with one large pre-allocation and a single
pointer increment per alloc.

Your job is to implement all four member functions.

## What to implement

Open `src/solution.cpp` and implement:

- `Arena::Arena(size_t capacity)` — allocate the backing buffer
- `Arena::~Arena()` — free the backing buffer
- `void* Arena::alloc(size_t n)` — bump-pointer allocation
- `void Arena::reset()` — reset the bump pointer

The class is declared in `src/solution.h`. Do not modify it.

## Spec

### Constructor

1. Allocate `capacity` bytes: `buf_ = new uint8_t[capacity]`.
2. Store `capacity` in `capacity_` and initialize `used_` to 0.

### Destructor

3. Free the buffer: `delete[] buf_`.

### alloc

4. If `n == 0` or `n > capacity_ - used_`, return `nullptr` — no space.
   - Use `n > capacity_ - used_` (not `used_ + n > capacity_`) to avoid unsigned overflow when `n` is very large.
   - Returning `nullptr` for `n == 0` keeps callers honest: a zero-byte allocation is always a caller bug.
5. Save the current head pointer: `void* ptr = buf_ + used_`.
   - `buf_` is `uint8_t*`, so `buf_ + used_` is byte-accurate pointer arithmetic.
6. Advance the counter: `used_ += n`.
7. Return `ptr`.

### reset

8. Set `used_ = 0`. Do not free `buf_` — it will be reused.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests must pass. The stub's constructor does not allocate the buffer
(`buf_` stays null), so three tests will fail until you implement the functions.
The `used()` accessor is pre-implemented in the stub, so `AllocAdvancesUsed` may
show unexpected results until the allocator is working.
