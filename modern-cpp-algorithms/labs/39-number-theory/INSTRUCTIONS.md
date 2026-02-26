# Lab 39 — Number Theory

Bit manipulation and prime sieves appear constantly in systems programming and cryptography. These five functions cover the canonical patterns: XOR for cancellation, popcount, power-of-two detection, modular exponentiation, and the Sieve of Eratosthenes.

## Tasks

- [ ] Implement `sieve(n)` — return a boolean vector of size n+1 where `result[i]` is true if i is prime; use the Sieve of Eratosthenes
- [ ] Implement `single_number(nums)` — every element appears exactly twice except one; find it using XOR in O(n) time and O(1) space
- [ ] Implement `count_bits(n)` — return the number of 1-bits in the binary representation of n (popcount)
- [ ] Implement `is_power_of_two(n)` — return true if n is a positive power of two; use the `n & (n-1)` trick
- [ ] Implement `mod_pow(base, exp, mod)` — compute (base^exp) % mod efficiently using repeated squaring

## Hints

- `sieve`: initialize all true, mark 0 and 1 false, then for each prime p starting at 2, mark all multiples of p starting at p*p as false
- `single_number`: XOR all elements together; every duplicate cancels to 0, leaving only the unique element
- `count_bits`: use `n & (n-1)` in a loop (clears the lowest set bit each iteration), or use `__builtin_popcount` / `std::popcount` from `<bit>`
- `is_power_of_two`: n > 0 and `(n & (n-1)) == 0`
- `mod_pow`: repeatedly square the base, halve the exponent; multiply the result whenever the exponent bit is set; take mod at each step to avoid overflow

Use `Run Tests` to check your progress.
