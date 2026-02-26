#include "solution.h"
#include <bit>
#include <numeric>

std::vector<bool> sieve(int n) {
    if (n < 0) return {};
    std::vector<bool> is_prime(static_cast<std::size_t>(n + 1), true);
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (int p = 2; p * p <= n; ++p) {
        if (!is_prime[static_cast<std::size_t>(p)]) continue;
        for (int multiple = p * p; multiple <= n; multiple += p) {
            is_prime[static_cast<std::size_t>(multiple)] = false;
        }
    }

    return is_prime;
}

int single_number(const std::vector<int>& nums) {
    int result = 0;
    for (int x : nums) {
        result ^= x;
    }
    return result;
}

int count_bits(int n) {
    return std::popcount(static_cast<unsigned int>(n));
}

bool is_power_of_two(long long n) {
    return n > 0 && (n & (n - 1)) == 0;
}

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1) {
            result = result * base % mod;
        }
        base = base * base % mod;
        exp >>= 1;
    }

    return result;
}
