#pragma once
#include <string>

void my_swap(int& a, int& b);
void my_swap_temp(int& a, int& b);
void sort_three(int& a, int& b, int& c);

template <typename T>
void generic_swap(T& a, T& b) {
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}
