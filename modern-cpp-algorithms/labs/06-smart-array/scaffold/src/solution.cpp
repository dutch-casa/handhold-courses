#include "solution.h"

SmartArray::SmartArray(int capacity) {
    // TODO: Use std::make_unique<int[]>(capacity), set size_ to 0
}

void SmartArray::push(int value) {
    // TODO: If full, create new unique_ptr with double capacity,
    //       copy elements, move-assign data_ = std::move(new_data)
    // Then append value and increment size_
}

int SmartArray::at(int index) const {
    // TODO: Return element at index, or throw std::out_of_range
    return 0;
}

int SmartArray::size() const {
    // TODO
    return 0;
}

int SmartArray::capacity() const {
    // TODO
    return 0;
}
