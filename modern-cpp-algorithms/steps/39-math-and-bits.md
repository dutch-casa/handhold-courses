---
title: Math and bit manipulation
---

# Modular arithmetic

{{show: mod-code typewriter 2s linear}} Large numbers overflow. Modular arithmetic keeps results manageable. The key identity: (a op b) mod m equals ((a mod m) op (b mod m)) mod m for addition and multiplication.

{{focus: mod-add}} {{zoom: 1.2x}} {{annotate: mod-add "Safe addition"}} Addition: take mod after each add. Keeps intermediate values within range. No overflow risk.

{{zoom: 1x}} {{focus: mod-mul}} {{zoom: 1.2x}} {{annotate: mod-mul "Safe multiply"}} Multiplication: cast to long long before multiplying, then take mod. Two 32-bit ints can overflow 32 bits but fit in 64.

{{zoom: 1x}} {{focus: mod-exp}} {{zoom: 1.2x}} {{annotate: mod-exp "Fast exponentiation"}} Modular exponentiation: square-and-multiply. Compute base to the power exp mod m in O(log exp) time. Essential for cryptography and competitive programming.

{{zoom: 1x}} {{focus: none}} Modular arithmetic is the foundation of number theory in programming. Master it and large-number problems become routine.

```code:mod-code lang=cpp
#include <cstdint>

int mod_add(int a, int b, int m) {
    return ((a % m) + (b % m)) % m;
}

int mod_mul(int a, int b, int m) {
    return static_cast<int>(
        (static_cast<int64_t>(a) * b) % m);
}

long long mod_pow(long long base, long long exp, long long m) {
    long long result = 1;
    base %= m;
    while (exp > 0) {
        if (exp & 1) result = result * base % m;
        base = base * base % m;
        exp >>= 1;
    }
    return result;
}
---
mod-add: 3-5
mod-mul: 7-10
mod-exp: 12-20
```

# GCD and LCM

{{clear: slide}}

{{show: gcd-code typewriter 1.5s linear}} Greatest common divisor and least common multiple. C++17 gives them to you for free.

{{focus: std-gcd}} {{zoom: 1.2x}} {{annotate: std-gcd "One function call"}} std gcd computes the greatest common divisor. std lcm computes the least common multiple. Both in the numeric header.

{{zoom: 1x}} {{focus: euclid}} {{zoom: 1.2x}} {{annotate: euclid "The algorithm"}} Under the hood, it's Euclid's algorithm: gcd of a and b equals gcd of b and a mod b. Recurse until b is zero. O(log min(a,b)).

{{zoom: 1x}} {{focus: none}} Use the standard library. Don't reimplement unless you need to modify the algorithm.

```code:gcd-code lang=cpp
#include <numeric>

auto g = std::gcd(12, 8);       // 4
auto l = std::lcm(12, 8);       // 24

// Euclid's algorithm (for understanding)
int gcd(int a, int b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}
---
std-gcd: 3-4
euclid: 7-12
```

# Sieve of Eratosthenes

{{clear: slide}}

{{show: sieve-viz grow 0.5s spring}} Find all primes up to N. The sieve crosses out composites, leaving only primes standing.

{{focus: init}} {{annotate: init "All candidates"}} Start with every number from 2 to N marked as prime. Then systematically eliminate.

{{focus: cross-2}} {{annotate: cross-2 "Multiples of 2"}} Cross out all multiples of 2 starting from 4. Every even number greater than 2 is composite.

{{focus: cross-3}} {{annotate: cross-3 "Multiples of 3"}} Cross out multiples of 3 starting from 9. We start at i squared because smaller multiples were already crossed out by smaller primes.

{{focus: done}} {{annotate: done "Primes remain"}} Continue until i squared exceeds N. Everything still standing is prime. O(n log log n) — nearly linear.

{{focus: none}}

{{show: sieve-code typewriter 1.5s linear}} In C++, use a vector of bool or bitset for the sieve.

{{focus: impl}} {{zoom: 1.2x}} The implementation is compact. Start crossing at i*i, increment by i.

{{zoom: 1x}} {{focus: none}}

```data:sieve-viz type=array
["[2,3,4,5,6,7,8,9,10]", "[2,3,_,5,_,7,_,9,_]", "[2,3,_,5,_,7,_,_,_]", "[2,3,5,7]"]
---
init: 0
cross-2: 1
cross-3: 2
done: 3
```

```code:sieve-code lang=cpp
std::vector<bool> sieve(int n) {
    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return is_prime;
}
---
impl: 4-10
```

# Bit tricks

{{clear: slide}}

{{show: bits-code typewriter 2s linear}} Bits are the lowest level of computation. A few tricks show up constantly in interviews.

{{focus: clear-lowest}} {{zoom: 1.2x}} {{annotate: clear-lowest "x & (x-1)"}} x and x minus one clears the lowest set bit. Use it to count bits: loop until zero, clearing one bit per iteration.

{{zoom: 1x}} {{focus: power-two}} {{zoom: 1.2x}} {{annotate: power-two "One bit set"}} A power of two has exactly one bit set. So x and x minus one is zero if and only if x is a power of two (and x is positive).

{{zoom: 1x}} {{focus: popcount}} {{zoom: 1.2x}} {{annotate: popcount "Hardware instruction"}} popcount counts set bits. The compiler intrinsic maps to a single CPU instruction on modern hardware. Use it.

{{zoom: 1x}} {{focus: none}} These tricks are compact and fast. Memorize the patterns — they come up in interviews.

```code:bits-code lang=cpp
// Clear lowest set bit
int clear_lowest(int x) { return x & (x - 1); }

// Power of two check
bool is_power_of_two(int x) { return x > 0 && (x & (x - 1)) == 0; }

// Count set bits
int count_bits(int x) {
    int count = 0;
    while (x) { x &= (x - 1); count++; }
    return count;
}

// Or use the builtin
int count_bits_fast(unsigned x) { return __builtin_popcount(x); }
---
clear-lowest: 2
power-two: 5
popcount: 14
```

# XOR tricks

{{clear: slide}}

{{show: xor-code typewriter 1.5s linear}} XOR has three properties that make it magical for certain problems.

{{focus: self-cancel}} {{zoom: 1.2x}} {{annotate: self-cancel "x ^ x = 0"}} x XOR x is zero. Any value XOR'd with itself cancels. This is the foundation of the "single number" trick.

{{zoom: 1x}} {{focus: identity}} {{zoom: 1.2x}} {{annotate: identity "x ^ 0 = x"}} x XOR zero is x. Zero is the identity element. Combined with self-cancellation, XOR acts like a toggle.

{{zoom: 1x}} {{focus: single-num}} {{zoom: 1.2x}} {{annotate: single-num "Find the outlier"}} Single number: every element appears twice except one. XOR all elements. Pairs cancel to zero, leaving the unique element.

{{zoom: 1x}} {{focus: none}} XOR is commutative and associative. Order doesn't matter. That's why you can XOR an entire array in any order.

```code:xor-code lang=cpp
// Properties
// x ^ x == 0 (self-cancellation)
// x ^ 0 == x (identity)
// a ^ b ^ a == b (cancellation)

int single_number(const std::vector<int>& nums) {
    int result = 0;
    for (int n : nums) result ^= n;
    return result;
}

// Missing number: array contains 0..n except one
int missing_number(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int result = n;
    for (int i = 0; i < n; i++) {
        result ^= i ^ nums[i];
    }
    return result;
}
---
self-cancel: 2
identity: 3
single-num: 6-9
```

# Bitwise operations in C++

{{clear: slide}}

{{show: bitwise-code typewriter 2s linear}} The full set of bitwise operators. Know them all.

{{focus: basics}} {{zoom: 1.2x}} {{annotate: basics "Six operators"}} AND, OR, XOR, NOT, left shift, right shift. These operate on individual bits.

{{zoom: 1x}} {{focus: bitset-use}} {{zoom: 1.2x}} {{annotate: bitset-use "std::bitset"}} For fixed-size bit manipulation, bitset provides a cleaner interface. It supports count, test, set, reset, and flip. Size is a compile-time constant.

{{zoom: 1x}} {{focus: none}} Raw bitwise operators for low-level code. Bitset for clarity and safety. Choose based on context.

```code:bitwise-code lang=cpp
#include <bitset>

// Bitwise operators
int a = 0b1100, b = 0b1010;
int and_r = a & b;    // 0b1000 (AND)
int or_r  = a | b;    // 0b1110 (OR)
int xor_r = a ^ b;    // 0b0110 (XOR)
int not_r = ~a;       // inverts all bits
int shl   = a << 2;   // 0b110000 (shift left)
int shr   = a >> 1;   // 0b0110 (shift right)

// std::bitset
std::bitset<8> bits(42);         // 00101010
auto count = bits.count();       // 3 (set bits)
bool bit2 = bits.test(2);       // false
bits.set(2);                     // 00101110
bits.flip();                     // 11010001
---
basics: 4-10
bitset-use: 13-17
```
