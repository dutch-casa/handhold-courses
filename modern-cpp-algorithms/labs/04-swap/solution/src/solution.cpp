#include "solution.h"

void my_swap(int& a, int& b) {
    a ^= b;
    b ^= a;
    a ^= b;
}

void my_swap_temp(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void sort_three(int& a, int& b, int& c) {
    if (a > b) my_swap_temp(a, b);
    if (b > c) my_swap_temp(b, c);
    if (a > b) my_swap_temp(a, b);
}
