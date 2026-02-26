#include "solution.h"

LRUCache::LRUCache(int capacity) : capacity_(capacity) {}

int LRUCache::get(int key) {
    auto it = map_.find(key);
    if (it == map_.end()) return -1;
    // Move to front (most recently used)
    order_.splice(order_.begin(), order_, it->second);
    return it->second->second;
}

void LRUCache::put(int key, int value) {
    auto it = map_.find(key);
    if (it != map_.end()) {
        it->second->second = value;
        order_.splice(order_.begin(), order_, it->second);
        return;
    }
    if (static_cast<int>(order_.size()) == capacity_) {
        // Evict least recently used (back of list)
        auto& [evict_key, evict_val] = order_.back();
        map_.erase(evict_key);
        order_.pop_back();
    }
    order_.emplace_front(key, value);
    map_[key] = order_.begin();
}
