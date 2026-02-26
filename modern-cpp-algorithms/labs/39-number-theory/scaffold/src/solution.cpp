#include "solution.h"

std::vector<bool> sieve(int n) {
    // TODO: Sieve of Eratosthenes
    return std::vector<bool>(static_cast<std::size_t>(n + 1), false);
}

int single_number(const std::vector<int>& nums) {
    // TODO: XOR all elements; duplicates cancel to 0
    return 0;
}

int count_bits(int n) {
    // TODO: count 1-bits using n & (n-1) or std::popcount
    return 0;
}

bool is_power_of_two(long long n) {
    // TODO: use n > 0 && (n & (n-1)) == 0
    return false;
}

long long mod_pow(long long base, long long exp, long long mod) {
    // TODO: repeated squaring — halve exp each iteration, square base, multiply result when bit is set
    return 0;
}
