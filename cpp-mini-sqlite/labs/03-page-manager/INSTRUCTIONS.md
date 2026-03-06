# Lab 03: PageManager

## Context

In Lesson 3 you learned the `PageManager` design. Its invariant:
every page lives at byte offset `id × kPageSize`.

The stub in `src/solution.cpp` already provides the constructor and destructor.
Your job is to implement `read` and `write`.

## What to implement

Open `src/solution.cpp` and implement:

- `DbResult<Page> PageManager::read(PageId id)` — read one page
- `DbResult<void> PageManager::write(const Page& page)` — write one page

The types are declared in `src/solution.h`.

## Spec

### Constructor

1. Call `std::fopen(path, "r+b")` and store the result in `file_`.
   The caller ensures the file exists; `r+b` opens for read+write without truncating.

### Destructor

2. If `file_` is non-null, call `std::fclose(file_)`.

### read

3. If `file_` is null (constructor failed), return `std::unexpected(DbError::kIo)`.
4. Compute the byte offset: `size_t offset = static_cast<size_t>(id) * kPageSize`.
   Cast `id` to `size_t` **before** multiplying to avoid 32-bit overflow on files larger than 4 GB.
5. Seek: `std::fseek(file_, static_cast<long>(offset), SEEK_SET)`.
   If it returns non-zero, return `std::unexpected(DbError::kIo)`.
6. Construct `Page page{id, {}}` — the `{}` zero-initialises the data buffer.
7. Read: `std::fread(page.data.data(), 1, kPageSize, file_)`.
   If it returns fewer than `kPageSize` bytes (partial read or EOF), return `std::unexpected(DbError::kIo)`.
8. Return `page`.

### write

9. If `file_` is null, return `std::unexpected(DbError::kIo)`.
10. Compute `size_t offset = static_cast<size_t>(page.id) * kPageSize`.
11. Seek to offset; return kIo on failure.
12. `std::fwrite(page.data.data(), 1, kPageSize, file_)`.
    If it returns fewer than `kPageSize` bytes, return `std::unexpected(DbError::kIo)`.
13. Return `{}` (a `DbResult<void>` in its success state).

## Key C API

| Function | Purpose |
|---|---|
| `std::fopen(path, "r+b")` | Open for read+write binary (file must exist) |
| `std::fclose(f)` | Close the file handle |
| `std::fseek(f, offset, SEEK_SET)` | Seek to absolute byte offset; returns non-zero on failure |
| `std::fread(buf, 1, n, f)` | Read up to `n` bytes; returns bytes read |
| `std::fwrite(buf, 1, n, f)` | Write `n` bytes; returns bytes written |

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests must pass. The stub returns kIo for read and write, so three tests
will fail until you implement the functions.
