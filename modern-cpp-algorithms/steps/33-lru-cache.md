---
title: LRU Cache
---

# Cache eviction problem

{{show: eviction-viz grow 0.5s spring}} A cache holds the most useful data in fast memory. When it's full and you need to insert something new, you must evict something old. LRU evicts the item that was accessed least recently.

{{focus: why-lru}} {{annotate: why-lru "Temporal locality"}} LRU bets on temporal locality: recently accessed data is likely to be accessed again. Evict the oldest untouched item. This works well for most real-world access patterns.

{{focus: constraints}} {{annotate: constraints "O(1) both ops"}} The hard requirement: get and put must both be O(1). A naive sorted list is O(n) to find the LRU item. A hash map alone can't track order. You need both — simultaneously.

{{focus: none}} This is a design problem. The answer is a specific data structure combination: a doubly-linked list for ordering and a hash map for fast access.

```data:eviction-viz type=linked-list
["[MRU] A → B → C → D [LRU]", "[MRU] E → A → B → C [LRU, D evicted]"]
---
why-lru: 0
constraints: 1
```

# The list + map trick

{{clear: slide}}

{{show: structure-viz grow 0.5s spring}} The key insight: a doubly-linked list tracks access order (most recent at front, LRU at back), and an unordered_map gives O(1) access to any node in that list.

{{focus: list-role}} {{annotate: list-role "Order tracker"}} The list maintains the recency order. Moving a node to the front is O(1) with a doubly-linked list — just pointer surgery, no shifting. The tail is always the LRU item.

{{focus: map-role}} {{annotate: map-role "O(1) lookup"}} The map stores key → iterator into the list. Given a key, the map returns an iterator to the node in O(1). Without the map, you'd have to scan the list — O(n) — to find any element.

{{focus: invariant}} {{annotate: invariant "Always in sync"}} The list and map must stay in sync. Every insert updates both. Every eviction removes from both. Every access moves in both. The invariant: map and list contain exactly the same keys.

{{focus: none}} This is a standard technique. The list owns the data. The map borrows iterators into the list. std::list iterators are stable — they never invalidate on splice, insert, or erase of other nodes.

```data:structure-viz type=hash-map
{"map": {"key1": "→ list_node_1", "key2": "→ list_node_2", "key3": "→ list_node_3"}, "list": "[MRU] key2 ↔ key1 ↔ key3 [LRU]"}
---
list-role: list
map-role: map
invariant: list
```

# Get operation

{{clear: slide}}

{{show: get-code typewriter 1.5s linear}} Get: return the value if the key exists, otherwise return -1. On a hit, move the node to the front of the list — it's now the most recently used.

{{focus: lookup}} {{zoom: 1.2x}} {{annotate: lookup "Map check first"}} Look up the key in the map. O(1). If not found, return -1. The map is the entry point — never touch the list directly for lookup.

{{zoom: 1x}} {{focus: move-front}} {{zoom: 1.3x}} {{annotate: move-front "splice = O(1) move"}} On a hit, call `list_.splice(list_.begin(), list_, it->second)`. splice moves the node to the front without copying data. It's pointer relinking — O(1). The iterator stored in the map remains valid after splice. That's why std::list works here and std::vector doesn't.

{{zoom: 1x}} {{focus: none}} The beauty: one map lookup, one splice. Both O(1). No allocation, no copy. The iterator stability of std::list is load-bearing — this entire design relies on it.

```code:get-code lang=cpp
int get(int key) {
    auto it = cache_.find(key);
    if (it == cache_.end()) return -1;

    // Move to front: this key was just accessed
    list_.splice(list_.begin(), list_, it->second);
    return it->second->second;  // iterator->pair.second = value
}
---
lookup: 2-3
move-front: 5-6
```

# Put operation

{{clear: slide}}

{{show: put-code typewriter 2s linear}} Put: insert or update a key-value pair. Always move to the front. If over capacity, evict the tail.

{{focus: update}} {{zoom: 1.2x}} {{annotate: update "Update existing"}} If the key already exists, update its value in place and splice to front. O(1) — no eviction needed, no capacity change.

{{zoom: 1x}} {{focus: insert}} {{zoom: 1.3x}} {{annotate: insert "Insert at front"}} For a new key, push to the front of the list, then store the iterator in the map. Insert front first, then map — if you did it the other way you'd have a dangling iterator.

{{zoom: 1x}} {{focus: evict}} {{zoom: 1.2x}} {{annotate: evict "Evict tail"}} If size exceeds capacity after insert, evict the back. Get the key from the back node, erase it from the map, then pop the back. Both structures stay in sync.

{{zoom: 1x}} {{focus: none}} Insert order: list front → map. Evict order: map erase → list pop back. Get the order wrong and you'll access freed memory. The list owns nodes; the map holds iterators.

```code:put-code lang=cpp
void put(int key, int value) {
    auto it = cache_.find(key);

    if (it != cache_.end()) {
        // Update existing: refresh value and move to front
        it->second->second = value;
        list_.splice(list_.begin(), list_, it->second);
        return;
    }

    // Insert new at front
    list_.emplace_front(key, value);
    cache_[key] = list_.begin();

    // Evict LRU if over capacity
    if (static_cast<int>(cache_.size()) > capacity_) {
        cache_.erase(list_.back().first);
        list_.pop_back();
    }
}
---
update: 4-7
insert: 10-11
evict: 14-16
```

# Full implementation

{{clear: slide}}

{{show: full-code typewriter 2s linear}} The complete LRU cache class. Minimal interface, maximum clarity. The private section holds the state; the public section exposes only get and put.

{{focus: types}} {{zoom: 1.2x}} {{annotate: types "Type aliases"}} Using type aliases for the list and map keeps the declaration readable. The list stores key-value pairs so eviction can find the key to erase from the map. The map stores list iterators.

{{zoom: 1x}} {{focus: class-body}} {{zoom: 1.3x}} {{annotate: class-body "Private state"}} capacity_ is immutable after construction. list_ and cache_ grow and shrink together. The invariant: cache_.size() == list_.size() <= capacity_.

{{zoom: 1x}} {{focus: none}} That's the whole structure. Two data members, two public methods. Deep module: enormous hidden complexity, trivial interface. Ousterhout would approve.

```code:full-code lang=cpp
#include <list>
#include <unordered_map>

class LRUCache {
    using KV   = std::pair<int, int>;
    using List = std::list<KV>;
    using Map  = std::unordered_map<int, List::iterator>;

    int    capacity_;
    List   list_;
    Map    cache_;

public:
    explicit LRUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) return -1;
        list_.splice(list_.begin(), list_, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        if (auto it = cache_.find(key); it != cache_.end()) {
            it->second->second = value;
            list_.splice(list_.begin(), list_, it->second);
            return;
        }
        list_.emplace_front(key, value);
        cache_[key] = list_.begin();
        if (static_cast<int>(cache_.size()) > capacity_) {
            cache_.erase(list_.back().first);
            list_.pop_back();
        }
    }
};
---
types: 4-7
class-body: 9-11
```

# Why std::list

{{clear: slide}}

{{show: list-props grow 0.5s spring}} Why std::list and not std::deque or std::vector? Three properties make std::list uniquely suited here.

{{focus: stable-iters}} {{annotate: stable-iters "Stable iterators"}} std::list iterators never invalidate except when the specific element they point to is erased. splice, insert, erase of other elements — none of these touch your iterator. std::vector invalidates all iterators on any reallocation. std::deque invalidates on most modifications.

{{focus: splice}} {{annotate: splice "O(1) splice"}} std::list::splice moves nodes between positions in O(1) by relinking pointers. No data is copied, no memory is allocated. This is the operation that makes move-to-front O(1). No other standard container offers this.

{{focus: front-back}} {{annotate: front-back "O(1) both ends"}} push_front, pop_back, emplace_front — all O(1). The list is doubly-linked, so both ends are directly accessible. Front = MRU, back = LRU. Eviction is pop_back.

{{focus: none}} std::list's memory overhead is real — two pointers per node plus heap allocation per element. For an LRU cache, correctness and O(1) semantics matter more than raw memory. It's the right tool.

```data:list-props type=array
["Stable iterators: survives splice, insert, erase of others", "splice(): O(1) pointer relink — no copy, no alloc", "push_front + pop_back: O(1) both ends", "Trade-off: 2 pointers overhead + heap alloc per node"]
---
stable-iters: 0
splice: 1
front-back: 2
```
