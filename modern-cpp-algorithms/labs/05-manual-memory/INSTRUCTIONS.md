# Manual Memory

Build a dynamic integer array using raw `new[]` and `delete[]`. This is intentionally painful — it sets up the motivation for smart pointers.

## Tasks

- [ ] Implement the `DynArray` constructor that allocates an array of the given capacity
- [ ] Implement `push(int value)` that appends a value (resize by doubling capacity if full)
- [ ] Implement `at(int index)` that returns the value at the index (throw `std::out_of_range` if invalid)
- [ ] Implement `size()` and `capacity()` getters
- [ ] Implement the destructor that frees the memory

## Hints

When resizing: allocate a new array with double capacity, copy elements, delete the old array, update the pointer.

The constructor should set `size_` to 0 and allocate `capacity_` elements.

`std::out_of_range` is in `<stdexcept>`.

Use `Run Tests` in the testing panel to check your progress.
