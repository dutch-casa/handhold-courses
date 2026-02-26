# Lab 28 — Real-Time Median

Apply dual-heap and frequency-counting techniques to streaming and scheduling problems.

## Tasks

- [ ] Implement `class MedianFinder` — maintain a streaming median using a max-heap (`lower`) and a min-heap (`upper`); `add_num` inserts a value, `find_median` returns the current median as a `double`
- [ ] Implement `top_k_frequent(nums, k)` — return the `k` most frequent elements in any order; use a frequency map and a min-heap of size `k`
- [ ] Implement `task_scheduler(tasks, cooldown)` — return the minimum number of CPU intervals to execute all tasks where the same task type must be separated by at least `cooldown` idle slots

## Hints

- `MedianFinder`: `lower` is a max-heap (default `priority_queue`), `upper` is a min-heap; after each insert, rebalance so `|lower.size() - upper.size()| <= 1`; if sizes are equal, median is `(lower.top() + upper.top()) / 2.0`, otherwise it is `lower.top()`
- `top_k_frequent`: build a `std::unordered_map<int,int>` of counts; iterate entries into a min-heap of size `k` (evicting when size exceeds k); extract all elements from the heap
- `task_scheduler`: count each task type's frequency; sort frequencies descending; the answer is `max(tasks.size(), (max_freq - 1) * (cooldown + 1) + count_of_max_freq_tasks)`

Use `Run Tests` to check your progress.
