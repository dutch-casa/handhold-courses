#pragma once
#include <cstddef>
#include <string>

std::size_t size_of_int();
std::size_t size_of_double();
std::size_t size_of_char();

int int_max();
int int_min();

bool is_within_int_range(long long value);

std::string classify(double value);
