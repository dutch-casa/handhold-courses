# Swap Without Temporaries

Practice references by implementing swap and related functions.

## Tasks

- [ ] Implement `my_swap(int& a, int& b)` that swaps two integers using references (no temporary variable — use arithmetic or XOR)
- [ ] Implement `my_swap_temp(int& a, int& b)` that swaps using a temporary variable (the normal way)
- [ ] Implement `sort_three(int& a, int& b, int& c)` that sorts three values in ascending order using only swaps
- [ ] Implement the template `generic_swap(T& a, T& b)` that works with any type

## Hints

XOR swap: `a ^= b; b ^= a; a ^= b;` — swaps without a temporary.

For `sort_three`, compare pairs and swap if out of order. Three comparisons suffice.

The template function goes in the header file since templates must be visible at the point of use.

Use `Run Tests` in the testing panel to check your progress.
