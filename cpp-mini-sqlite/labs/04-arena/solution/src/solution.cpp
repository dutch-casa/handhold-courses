#include "solution.h"
#include <cstdint>

Arena::Arena(size_t capacity)
    : buf_(new uint8_t[capacity]), capacity_(capacity), used_(0) {}

Arena::~Arena() { delete[] buf_; }

void* Arena::alloc(size_t n) {
  if (n == 0 || n > capacity_ - used_) return nullptr;
  void* ptr = buf_ + used_;
  used_ += n;
  return ptr;
}

void Arena::reset() { used_ = 0; }

size_t Arena::used() const { return used_; }
