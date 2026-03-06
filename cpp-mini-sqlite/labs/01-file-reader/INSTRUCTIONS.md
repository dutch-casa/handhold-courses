# Lab 01: File Reader

## What you already know

In Lesson 01 you built `DbResult<T>` — a type alias over `std::expected<T, DbError>` that
makes every failure explicit at the call site. No exceptions, no sentinel values, no
out-parameters. A function either returns a value or it returns a `DbError`, and the
compiler forces the caller to handle both cases.

## Your task

Open `src/solution.cpp` and implement:

```cpp
DbResult<std::vector<uint8_t>> read_file(const char* path);
```

The function must:

1. Open the file at `path` in binary mode.
2. Determine its size.
3. Allocate a buffer and read all bytes into it.
4. Return the buffer on success.
5. Return `std::unexpected(DbError::kIo)` on **any** failure — open, seek, size
   query, or read.

## C standard library functions you will need

Add `#include <cstdio>` to `solution.cpp` — it provides all of the functions below.

| Function | What it does |
|---|---|
| `std::fopen(path, "rb")` | Opens a file for binary reading; returns `nullptr` on failure. |
| `std::fseek(f, 0, SEEK_END)` | Moves the file position indicator to the end of the file; returns non-zero on failure. |
| `std::ftell(f)` | Returns the current file position (i.e. the file size after seeking to end); returns `-1L` on failure. |
| `std::fseek(f, 0, SEEK_SET)` | Moves the file position back to the beginning; returns non-zero on failure. |
| `std::fread(buf, 1, n, f)` | Reads up to `n` bytes into `buf`; returns the number of bytes actually read. |
| `std::fclose(f)` | Closes the file. Call this before every return. |

## Error propagation

Every syscall that can fail must be checked. On failure:

```cpp
std::fclose(f);
return std::unexpected(DbError::kIo);
```

Close the file before returning — don't leak the handle.

## Run the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests should pass once your implementation is correct.
