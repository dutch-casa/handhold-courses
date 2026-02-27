# Generic Container — Templates and Concepts

A sorted container that stays sorted after every insertion. Constrained by a concept so only comparable types compile.

## Tasks

- [ ] Define concept `Comparable` — requires that `a < b` is valid and returns bool for type T
- [ ] Implement `SortedContainer<T>` template class with `insert(val)`, `contains(val)`, `remove(val)`, `size()`, `at(index)`, `to_vector()`
- [ ] Elements are always maintained in sorted order
- [ ] `at(index)` returns a const reference to the element at the given index (0-based)
- [ ] `remove(val)` removes first occurrence and returns true, or false if not found

## Hints

- Use `std::vector<T>` internally with `std::lower_bound` for binary search insertion
- `insert`: find position with `lower_bound`, use `vec.insert(it, val)`
- `contains`: `lower_bound` + check if the element at that position equals val
- `remove`: `lower_bound` + check + `vec.erase`
- Concept: `template<typename T> concept Comparable = requires(T a, T b) { { a < b } -> std::convertible_to<bool>; };`

Use `Run Tests` to check your progress.
