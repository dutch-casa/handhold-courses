# Type Explorer

Implement functions that report properties of C++ fundamental types.

## Tasks

- [ ] Implement `size_of_int()`, `size_of_double()`, `size_of_char()` returning `sizeof` each type
- [ ] Implement `int_max()` and `int_min()` returning `std::numeric_limits<int>::max()` and `min()`
- [ ] Implement `is_within_int_range(long long value)` that returns true if the value fits in an int
- [ ] Implement `classify(double value)` returning `"positive"`, `"negative"`, or `"zero"`

## Hints

Use `<limits>` for `std::numeric_limits`.

`sizeof(T)` returns `std::size_t`, which is an unsigned integer — cast or compare carefully.

Use `Run Tests` in the testing panel to check your progress.
