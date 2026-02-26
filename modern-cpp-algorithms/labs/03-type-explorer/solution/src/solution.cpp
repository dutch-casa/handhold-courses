#include "solution.h"
#include <limits>

std::size_t size_of_int() { return sizeof(int); }
std::size_t size_of_double() { return sizeof(double); }
std::size_t size_of_char() { return sizeof(char); }

int int_max() { return std::numeric_limits<int>::max(); }
int int_min() { return std::numeric_limits<int>::min(); }

bool is_within_int_range(long long value) {
    return value >= std::numeric_limits<int>::min()
        && value <= std::numeric_limits<int>::max();
}

std::string classify(double value) {
    if (value > 0.0) return "positive";
    if (value < 0.0) return "negative";
    return "zero";
}
