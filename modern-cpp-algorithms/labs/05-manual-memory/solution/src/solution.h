#pragma once
#include <stdexcept>

class DynArray {
public:
    explicit DynArray(int capacity);
    ~DynArray();

    void push(int value);
    int at(int index) const;
    int size() const;
    int capacity() const;

private:
    int* data_;
    int size_;
    int capacity_;
};
