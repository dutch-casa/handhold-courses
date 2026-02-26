#include "solution.h"

SmartArray::SmartArray(int capacity)
    : data_(std::make_unique<int[]>(capacity)), size_(0), capacity_(capacity) {}

void SmartArray::push(int value) {
    if (size_ == capacity_) {
        int new_cap = capacity_ * 2;
        auto new_data = std::make_unique<int[]>(new_cap);
        for (int i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        data_ = std::move(new_data);
        capacity_ = new_cap;
    }
    data_[size_] = value;
    ++size_;
}

int SmartArray::at(int index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("index out of range");
    }
    return data_[index];
}

int SmartArray::size() const { return size_; }
int SmartArray::capacity() const { return capacity_; }
