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
        auto it = std::lower_bound(data_.begin(), data_.end(), val);
        data_.insert(it, val);
    }

    bool contains(const T& val) const {
        auto it = std::lower_bound(data_.begin(), data_.end(), val);
        return it != data_.end() && !(*it < val) && !(val < *it);
    }

    bool remove(const T& val) {
        auto it = std::lower_bound(data_.begin(), data_.end(), val);
        if (it == data_.end() || *it < val || val < *it) return false;
        data_.erase(it);
        return true;
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
