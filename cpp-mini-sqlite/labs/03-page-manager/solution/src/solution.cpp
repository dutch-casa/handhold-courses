#include "solution.h"

PageManager::PageManager(const char* path) : file_(std::fopen(path, "r+b")) {}

PageManager::~PageManager() {
  if (file_) std::fclose(file_);
}

DbResult<Page> PageManager::read(PageId id) {
  if (!file_) return std::unexpected(DbError::kIo);
  size_t offset = static_cast<size_t>(id) * kPageSize;
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0) {
    return std::unexpected(DbError::kIo);
  }
  Page page{id, {}};
  size_t n = std::fread(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) {
    return std::unexpected(DbError::kIo);
  }
  return page;
}

DbResult<void> PageManager::write(const Page& page) {
  if (!file_) return std::unexpected(DbError::kIo);
  size_t offset = static_cast<size_t>(page.id) * kPageSize;
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0) {
    return std::unexpected(DbError::kIo);
  }
  size_t n = std::fwrite(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) {
    return std::unexpected(DbError::kIo);
  }
  return {};
}
