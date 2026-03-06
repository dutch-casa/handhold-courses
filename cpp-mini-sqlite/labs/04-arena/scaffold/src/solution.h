#pragma once
#include <cstddef>
#include <cstdint>

// Bump-pointer allocator. Owns one contiguous heap buffer.
// Invariant: used_ <= capacity_ at all times.
class Arena {
 public:
  explicit Arena(size_t capacity);
  ~Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;
  Arena(Arena&&) = delete;
  Arena& operator=(Arena&&) = delete;

  // Allocates n bytes. Returns nullptr if out of space.
  void* alloc(size_t n);

  // Resets the bump pointer to zero. Does not free the backing buffer.
  void reset();

  // Returns bytes currently in use.
  size_t used() const;

 private:
  uint8_t* buf_;
  size_t   capacity_;
  size_t   used_;
};
