#include "solution.h"

PageManager::PageManager(const char* path) : file_(std::fopen(path, "r+b")) {}
PageManager::~PageManager() {
  if (file_) std::fclose(file_);
}

DbResult<Page> PageManager::read(PageId) {
  return std::unexpected(DbError::kIo);
}

DbResult<void> PageManager::write(const Page&) {
  return std::unexpected(DbError::kIo);
}
