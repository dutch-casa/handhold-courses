# Smart Array

Rewrite the manual DynArray from Lab 5 using `unique_ptr`. Same tests, dramatically less code, zero manual memory management.

## Tasks

- [ ] Implement `SmartArray` constructor using `std::make_unique<int[]>(capacity)`
- [ ] Implement `push(int value)` with resize (create new unique_ptr, copy, move-assign)
- [ ] Implement `at(int index)` with bounds checking
- [ ] Implement `size()` and `capacity()` getters
- [ ] No destructor needed — Rule of Zero

## Hints

`std::make_unique<int[]>(n)` creates a unique_ptr to an array of n ints.

To resize: create a new `std::make_unique<int[]>(new_cap)`, copy elements in a loop, then `data_ = std::move(new_data)`.

The destructor is provided as `= default` — unique_ptr handles cleanup.

Use `Run Tests` in the testing panel to check your progress.
