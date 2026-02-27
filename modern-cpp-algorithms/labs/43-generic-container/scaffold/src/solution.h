#pragma once
#include <vector>
#include <algorithm>
#include <concepts>
#include <cstddef>

template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template<Comparable T>
class SortedContainer {
public:
    void insert(const T& val) {
        // TODO: insert in sorted position
    }

    bool contains(const T& val) const {
        // TODO: binary search
        return false;
    }

    bool remove(const T& val) {
        // TODO: find and erase first occurrence
        return false;
    }

    std::size_t size() const {
        return data_.size();
    }

    const T& at(std::size_t index) const {
        return data_.at(index);
    }

    std::vector<T> to_vector() const {
        return data_;
    }

private:
    std::vector<T> data_;
};
