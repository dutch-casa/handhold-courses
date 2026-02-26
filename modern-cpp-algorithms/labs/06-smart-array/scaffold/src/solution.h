#pragma once
#include <memory>
#include <stdexcept>

class SmartArray {
public:
    explicit SmartArray(int capacity);
    ~SmartArray() = default;

    void push(int value);
    int at(int index) const;
    int size() const;
    int capacity() const;

private:
    std::unique_ptr<int[]> data_;
    int size_;
    int capacity_;
};
