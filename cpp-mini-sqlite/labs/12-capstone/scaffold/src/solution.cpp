// solution.cpp — implement the four functions marked TODO.
// tokenize, the parser, encode_row/decode_row, and the executor are all
// pre-implemented in helpers.cpp. You do not need to touch them.

#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include "solution.h"

// ---------------------------------------------------------------------------
// RowFile::open
// ---------------------------------------------------------------------------
// Open an existing file or create a new one.
//   - Try fopen(path, "r+b"). If it returns null, use fopen(path, "w+b").
//   - For a new file, write uint32_t 0 at offset 0 to initialize row_count.
// Return kIo on any fopen or fwrite failure.

DbResult<RowFile> RowFile::open(const char* path) {
  (void)path;
  return std::unexpected(DbError::kIo);
}

// ---------------------------------------------------------------------------
// RowFile::insert
// ---------------------------------------------------------------------------
// File layout: bytes 0–3 = uint32_t row_count; row i at offset 4 + i*kRowSize.
//
//   1. fseek to 0, fread 4 bytes → row_count.
//   2. encode_row(row, buf) into a kRowSize-byte buffer.
//   3. fseek to (4 + row_count * kRowSize), fwrite the buffer.
//   4. Increment row_count. fseek to 0, fwrite the updated count.
//   5. fflush so the data hits the OS page cache.
// Return kIo on any seek, read, or write failure.

DbResult<void> RowFile::insert(const Row& row) {
  (void)row;
  return std::unexpected(DbError::kIo);
}

// ---------------------------------------------------------------------------
// RowFile::all
// ---------------------------------------------------------------------------
//   1. fseek to 0, fread 4 bytes → row_count.
//   2. For each i in [0, row_count):
//        fseek to (4 + i*kRowSize), fread kRowSize bytes, decode_row().
// Return kIo on seek/read failure; propagate decode_row's error on corruption.

DbResult<std::vector<Row>> RowFile::all() const {
  return std::unexpected(DbError::kIo);
}

// ---------------------------------------------------------------------------
// Database::open
// ---------------------------------------------------------------------------
// Convert path (a string_view) to a null-terminated std::string,
// call RowFile::open(), and wrap the result in a Database.
// Propagate RowFile::open()'s error on failure.

DbResult<Database> Database::open(std::string_view path) {
  (void)path;
  return std::unexpected(DbError::kIo);
}

// ---------------------------------------------------------------------------
// Database::query
// ---------------------------------------------------------------------------
//   1. tokenize(sql)            — return error on failure.
//   2. Parser(tokens).parse()   — return error on failure.
//   3. Visit the Stmt variant:
//        SelectStmt:  row_file_.all() → execute_select_where(rows, stmt) → return result.
//        InsertStmt:  build a Row from values[0] (id integer) and values[1] (name or NULL),
//                     call row_file_.insert(row), return an empty vector on success.
//        otherwise:   return kSyntax.

DbResult<std::vector<ResultRow>> Database::query(std::string_view sql) {
  (void)sql;
  return std::unexpected(DbError::kSyntax);
}
