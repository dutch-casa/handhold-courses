#include "solution.h"

DynArray::DynArray(int capacity)
    : data_(new int[capacity]), size_(0), capacity_(capacity) {}

DynArray::~DynArray() {
    delete[] data_;
}

void DynArray::push(int value) {
    if (size_ == capacity_) {
        int new_cap = capacity_ * 2;
        int* new_data = new int[new_cap];
        for (int i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }
    data_[size_] = value;
    ++size_;
}

int DynArray::at(int index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("index out of range");
    }
    return data_[index];
}

int DynArray::size() const { return size_; }
int DynArray::capacity() const { return capacity_; }
