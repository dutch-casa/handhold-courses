#include "solution.h"

Arena::Arena(size_t capacity) : buf_(nullptr), capacity_(capacity), used_(0) {}

Arena::~Arena() {}

void* Arena::alloc(size_t) { return nullptr; }

void Arena::reset() {}

size_t Arena::used() const { return used_; }
