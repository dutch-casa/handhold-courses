# Lab 27 — Heap from Scratch

Build a min-heap data structure manually and apply heap-based techniques to classical problems.

## Tasks

- [ ] Implement `class MinHeap` — backed by a `std::vector<int>`; support `push`, `pop`, `top`, `size`, `empty`
- [ ] Implement `kth_largest(nums, k)` — return the kth largest element using `std::nth_element` or a size-k max/min heap
- [ ] Implement `merge_k_sorted(lists)` — merge k sorted vectors into one sorted vector using a min-heap of `{value, list_index, element_index}`

## Hints

- `MinHeap::push`: append to vector, then `sift_up` (swap with parent while smaller than parent); parent of index `i` is `(i-1)/2`
- `MinHeap::pop`: swap root with last element, remove last, then `sift_down` (swap with smaller child while larger than a child); children of `i` are `2*i+1` and `2*i+2`
- `kth_largest`: use `std::nth_element` with `std::greater<>` comparator — after the call, `nums[k-1]` is the answer
- `merge_k_sorted`: push the first element of each non-empty list into the heap as `{value, list_idx, elem_idx}`; each pop gives the next smallest element

Use `Run Tests` to check your progress.
