#pragma once
#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
public:
    explicit LRUCache(int capacity);

    // Return cached value for key, or -1 if absent. Marks key as most recently used.
    int get(int key);

    // Insert or update key/value. Evicts the least recently used entry if over capacity.
    void put(int key, int value);

private:
    int capacity_;
    // Front = most recently used, back = least recently used.
    std::list<std::pair<int, int>> order_;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> map_;
};
