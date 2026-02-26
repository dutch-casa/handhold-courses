#pragma once
#include <vector>

// Prime sieve: result[i] is true if i is prime (size = n+1).
std::vector<bool> sieve(int n);

// Find the one element that appears once; all others appear exactly twice.
int single_number(const std::vector<int>& nums);

// Count the number of 1-bits in the binary representation of n.
int count_bits(int n);

// True if n is a positive power of two.
bool is_power_of_two(long long n);

// Compute (base^exp) % mod using repeated squaring.
long long mod_pow(long long base, long long exp, long long mod);
