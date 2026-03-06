---
title: Error handling with std::expected
---

# The problem with error handling

{{show: return-code typewriter 2s linear}} Every function that can fail needs a way to report that failure. The oldest approach is return codes — a special sentinel value meaning "something went wrong."

{{focus: open-fn}} {{annotate: open-fn "Sentinel on failure"}} The function returns `-1` on error, `0` on success. The caller checks the return value. Simple enough.

{{focus: caller-ignores}} {{zoom: 1.3x}} {{annotate: caller-ignores "Silently ignored"}} But nothing forces the caller to check. The compiler won't warn. The program keeps running. The error disappears. You find out at 2am when production is down.

{{zoom: 1x}} {{focus: none}} The error and the value share the same channel. `-1` means failure, but only if you know the convention. And the convention is invisible in the type.

{{focus: none}} {{clear: slide}}

{{show: exception-code typewriter 2s linear}} {{split}} {{show: return-code none 0s}} Exceptions fix the "can be ignored" problem. But they introduce a different one.

{{focus: throwing-fn}} {{annotate: throwing-fn "Throws on failure"}} The function throws on failure. The caller can't silently ignore it — if they don't catch, the stack unwinds. Sounds better.

{{focus: exception-callers}} {{annotate: exception-callers "Type says nothing"}} Look at the signatures. Can you tell which one throws? Nothing in the type says so. You have to read the documentation, or the implementation, or discover it at runtime.

{{focus: throwing-fn}} {{annotate: throwing-fn "Runtime overhead"}} Exceptions also carry runtime overhead. They're expensive to throw. In systems code — a database reading millions of rows — that cost adds up.

{{focus: none}} {{unsplit}} The aha: neither approach forces the caller to handle the error. Return codes can be ignored. Exception signatures are invisible. We want a type that makes ignoring the error a compile error.

{{focus: none}} {{clear: slide}}

{{show: problem-summary typewriter 1.5s linear}} Here's the gap both approaches leave open.

{{focus: rc-gap}} {{annotate: rc-gap "Ignored at call site"}} Return codes: the compiler sees an unused return value and says nothing. The error evaporates.

{{focus: exc-gap}} {{annotate: exc-gap "No signature visibility"}} Exceptions: the function signature doesn't say it can fail. The contract is hidden.

{{focus: none}} We want a type that encodes "this can fail" in the signature itself, and forces the caller to look at the result before using it.

```code:return-code lang=cpp
#include <cstdio>

// Returns 0 on success, -1 on error.
// Convention known only to readers of the docs.
int open_file(const char* path) {
  FILE* f = std::fopen(path, "rb");
  if (!f) {
    return -1;
  }
  // ... use f ...
  return 0;
}

// The caller can ignore the return value entirely — no warning.
void load_database(const char* path) {
  open_file(path);   // error silently discarded
  // program continues as if nothing happened
}
---
open-fn: 4-10
caller-ignores: 14-16
```

```code:exception-code lang=cpp
#include <stdexcept>
#include <cstdio>

// Which of these throws? The type doesn't say.
int   open_file(const char* path);    // throws on failure?
void  read_page(int fd, int page);    // throws on failure?
float parse_float(const char* s);     // throws on failure?

// The one that actually throws:
int open_file_impl(const char* path) {
  FILE* f = std::fopen(path, "rb");
  if (!f) {
    throw std::runtime_error("cannot open file");
  }
  return 0;
}
---
throwing-fn: 9-16
exception-callers: 4-7
```

```data:problem-summary type=array
["return codes  → caller can ignore the return value (no warning)", "exceptions    → throws/noexcept invisible in the type signature", "std::expected → failure is in the type; [[nodiscard]] enables warnings"]
---
rc-gap: 0
exc-gap: 1
```

# std::expected — the type that holds either

{{clear: slide}}

{{show: either-viz grow 0.5s spring}} What if the return type itself encoded success or failure? That's `std::expected<T, E>`.

{{focus: value-branch}} {{annotate: value-branch "Holds value T"}} On the happy path, it holds a `T`. Use it like any value.

{{focus: error-branch}} {{annotate: error-branch "Holds error E"}} On the failure path, it holds an `E`. Never both. Never neither. One or the other.

{{focus: none}} The type is self-documenting. Mark returning functions `[[nodiscard]]` and the compiler will warn when the result is silently discarded. The function signature tells you everything.

{{focus: none}} {{clear: slide}}

{{show: expected-basics typewriter 2s linear}} Here's what construction looks like.

{{focus: success-return}} {{zoom: 1.3x}} {{annotate: success-return "Implicit T conversion"}} Returning a success is just returning the value. `std::expected<int, DbError>` converts from `int` implicitly. No wrapper needed.

{{zoom: 1x}} {{focus: failure-return}} {{zoom: 1.3x}} {{annotate: failure-return "Explicit unexpected"}} Returning a failure uses `std::unexpected`. The name is intentional — it makes failures visible at every return site.

{{zoom: 1x}} {{focus: sig-contract}} {{zoom: 1.2x}} {{annotate: sig-contract "Contract in the type"}} Read the signature: `DbResult<int>` means "either an int, or a `DbError`." No documentation needed. The type says it.

{{zoom: 1x}} {{focus: none}} That's the shift. The function doesn't throw, and it doesn't return a sentinel. It returns a type that carries both possibilities. The caller has to reckon with both.

{{focus: none}} {{clear: slide}}

{{show: either-viz grow 0.5s spring}} Visualize the type. A `DbResult<Page>` is a tagged union — one slot for the value, one for the error, and a discriminant bit that says which one is live.

{{focus: value-branch}} {{annotate: value-branch "Value path"}} When the operation succeeded, the value slot is populated. The error slot is uninhabited.

{{focus: error-branch}} {{annotate: error-branch "Error path"}} When it failed, the error slot is populated. The value slot is uninhabited.

{{focus: neither}} {{annotate: neither "No null, no throw"}} There's no null state. There's no exception. Either you have a page, or you have an error. That's the entire surface.

{{focus: none}} The type makes invalid states unrepresentable. You can't have a `DbResult<Page>` that's neither a page nor an error. The type won't allow it.

```code:expected-basics lang=cpp
#pragma once
#include <expected>
#include <cstdint>

enum class DbError {
  kNotFound,
  kCorrupted,
  kOutOfMemory,
  kIo,
  kSyntax,
  kFull,
};

template <typename T>
using DbResult = std::expected<T, DbError>;

// Signature tells the whole story: returns a page, or a DbError.
DbResult<int> read_page_count(int fd);

// Returning success: implicit conversion from int
DbResult<int> count_pages(int fd) {
  int n = /* read from header */ 42;
  return n;                                   // success — implicit T
}

// Returning failure: explicit unexpected()
DbResult<int> count_pages_fail(int fd) {
  if (fd < 0) {
    return std::unexpected(DbError::kIo);     // failure — must be explicit
  }
  return 0;
}
---
success-return: 20-24
failure-return: 26-32
sig-contract: 17-18
```

```data:either-viz type=array
["DbResult<Page>", "  ├─ ok  → Page { bytes: [...] }", "  └─ err → DbError::kIo", "", "Never: null | uninitialized | exception"]
---
value-branch: 1
error-branch: 2
neither: 4
```

# Consuming an expected value

{{clear: slide}}

{{show: consume-code typewriter 2s linear}} Reading a `DbResult` means checking which branch you're on before you use anything.

{{focus: bool-check}} {{zoom: 1.3x}} {{annotate: bool-check "Truthy = has value"}} `if (result)` is truthy when the expected holds a value. That's the idiomatic first check.

{{zoom: 1x}} {{focus: value-access}} {{zoom: 1.3x}} {{annotate: value-access "value() — panics if empty"}} `.value()` extracts the `T`. But call it on an empty expected and it throws `std::bad_expected_access`. Never call it without checking first.

{{zoom: 1x}} {{focus: error-access}} {{zoom: 1.3x}} {{annotate: error-access "error() — only if failed"}} `.error()` extracts the `E`. Only valid when the expected holds an error. Same rule: check first.

{{zoom: 1x}} {{focus: none}} These three are the primitives. In practice, you rarely use them directly. There's a better pattern for propagation.

{{focus: none}} {{clear: slide}}

{{show: propagate-code typewriter 2s linear}} The most common pattern in this entire course is this: if a sub-call fails, return its error up the chain.

{{focus: init-stmt}} {{zoom: 1.3x}} {{annotate: init-stmt "Init in condition"}} `if (auto result = call(); !result)` — the initializer runs the call, the condition checks it. One line. The result is scoped to the if-block.

{{zoom: 1x}} {{focus: early-return}} {{zoom: 1.3x}} {{annotate: early-return "Propagate the error"}} `return std::unexpected(count.error())` on failure. We extract the error from the result and forward it up. The count's value is discarded — only the error escapes the guard.

{{zoom: 1x}} {{focus: happy-use}} {{zoom: 1.2x}} {{annotate: happy-use "Safe to use value"}} After the guard clause, we know the call succeeded. `.value()` is safe — we've already verified the expected holds a value.

{{zoom: 1x}} {{focus: none}} Learn this pattern. Every lab function in this course uses it. Sub-call fails → early return with its error. Sub-call succeeds → continue with the value.

{{focus: none}} {{clear: slide}}

{{show: consume-patterns typewriter 1.5s linear}} Here's all three consumption patterns together for reference.

{{focus: pattern-check}} {{zoom: 1.2x}} {{annotate: pattern-check "Branch on result"}} The explicit branch — check, then use. Readable when you need different behavior on success and failure.

{{zoom: 1x}} {{focus: pattern-guard}} {{zoom: 1.2x}} {{annotate: pattern-guard "Guard and continue"}} The guard clause — return on failure, continue on success. This is the one you'll write a hundred times.

{{zoom: 1x}} {{focus: pattern-value-or}} {{zoom: 1.2x}} {{annotate: pattern-value-or "Default on failure"}} `.value_or(default)` returns the value if present, otherwise the default. Useful when failure is recoverable with a fallback.

{{zoom: 1x}} {{focus: none}} Three patterns. The guard clause is the workhorse.

```code:consume-code lang=cpp
#include <expected>
#include "db_error.h"

DbResult<int> read_count(int fd);

void example(int fd) {
  // Basic check: truthy = has value
  auto result = read_count(fd);
  if (result) {
    int count = result.value();   // safe: we checked
    use(count);
  } else {
    DbError err = result.error(); // safe: we know it failed
    log(err);
  }
}
---
bool-check: 8-9
value-access: 10
error-access: 12-13
```

```code:propagate-code lang=cpp
#include "db_error.h"

DbResult<int>   read_count(int fd);
DbResult<float> read_fill_factor(int fd);

// Early-return propagation pattern
DbResult<float> load_stats(int fd) {
  // Init-statement: count only needed for the guard.
  if (auto count = read_count(fd); !count) {
    return std::unexpected(count.error());   // propagate DbError up
  }

  // Declare fill outside; needed after the guard.
  auto fill = read_fill_factor(fd);
  if (!fill) {
    return std::unexpected(fill.error());
  }

  // fill is in scope — safe to use .value()
  return fill.value();
}
---
init-stmt: 8-10
early-return: 10
happy-use: 20
```

```code:consume-patterns lang=cpp
#include "db_error.h"

DbResult<int> read_count(int fd);

void patterns(int fd) {
  auto r = read_count(fd);

  // Pattern 1: explicit branch
  if (r) {
    use(r.value());
  } else {
    log(r.error());
  }

  // Pattern 2: guard clause (most common)
  if (!r) { return; }
  use(r.value());

  // Pattern 3: value_or — fallback on failure
  int count = r.value_or(0);
  use(count);
}
---
pattern-check: 8-12
pattern-guard: 14-16
pattern-value-or: 18-20
```

# Chaining with and_then

{{clear: slide}}

{{show: nested-chain typewriter 2s linear}} Let's say you need three operations in sequence. Each one can fail. The naive approach nests fast.

{{focus: nested-depth}} {{zoom: 1.2x}} {{annotate: nested-depth "Three levels deep"}} Each new operation adds another level of indentation. The happy path is buried. The structure of the code no longer matches the structure of the problem.

{{zoom: 1x}} {{focus: none}} The nesting is the smell. The logic is straightforward — open, read, decode. But the code looks like an accordion.

{{focus: none}} {{clear: slide}}

{{show: andthen-chain typewriter 2s linear}} {{split}} {{show: nested-chain none 0s}} `and_then` chains operations. If the expected holds a value, apply the function to it and return the new expected. If it holds an error, propagate the error without calling the function at all.

{{focus: andthen-call}} {{zoom: 1.2x}} {{annotate: andthen-call "Short-circuits on error"}} The second `and_then` never runs if the first call returned an error. The chain short-circuits. Errors propagate automatically.

{{zoom: 1x}} {{focus: final-match}} {{zoom: 1.3x}} {{annotate: final-match "One error handler"}} One match at the end of the chain handles all failure cases. The happy path reads as a pipeline: open, then read, then decode.

{{zoom: 1x}} {{focus: none}} {{unsplit}} The aha: `and_then` lets you describe the happy path as a pipeline. The unhappy path is invisible until you choose to handle it at the end. The chain is honest — every step can fail, and the type proves it.

{{focus: none}} {{clear: slide}}

{{show: andthen-transform typewriter 1.5s linear}} `transform` is `and_then`'s cousin. Where `and_then` maps `T → DbResult<U>`, `transform` maps `T → U`. Use it when the transformation can't fail.

{{focus: transform-use}} {{zoom: 1.2x}} {{annotate: transform-use "Infallible map"}} The lambda returns a plain value, not a `DbResult`. `transform` wraps it in an `expected` automatically. Errors still propagate.

{{zoom: 1x}} {{focus: none}} Two tools: `and_then` for fallible steps, `transform` for infallible ones. Compose them into a readable pipeline.

```code:nested-chain lang=cpp
#include "db_error.h"

DbResult<int>    open_db(const char* path);
DbResult<int>    read_root_page(int fd);
DbResult<float>  decode_fill_factor(int page_id);

// Nested approach: hard to follow
DbResult<float> load_fill(const char* path) {
  if (auto fd = open_db(path)) {
    if (auto page = read_root_page(*fd)) {
      if (auto fill = decode_fill_factor(*page)) {
        return *fill;
      }
      return fill;                              // error from decode
    }
    return std::unexpected(page.error());       // error from read
  }
  return std::unexpected(fd.error());           // error from open
}
---
nested-depth: 9-19
```

```code:andthen-chain lang=cpp
#include <expected>
#include "db_error.h"

DbResult<int>    open_db(const char* path);
DbResult<int>    read_root_page(int fd);
DbResult<float>  decode_fill_factor(int page_id);

// and_then chain: reads like prose
DbResult<float> load_fill(const char* path) {
  return open_db(path)
    .and_then(read_root_page)
    .and_then(decode_fill_factor);
}

// Handle the final result once, at the call site:
void run(const char* path) {
  auto result = load_fill(path);
  if (!result) {
    log_error(result.error());
    return;
  }
  use(*result);
}
---
andthen-call: 10-12
final-match: 16-21
```

```code:andthen-transform lang=cpp
#include <string>
#include "db_error.h"

DbResult<int>  read_page_count(int fd);

// transform: T → U (cannot fail — no DbResult wrapper needed)
DbResult<std::string> page_count_str(int fd) {
  return read_page_count(fd)
    .transform([](int n) {
      return std::to_string(n);   // int → string, can't fail
    });
}
---
transform-use: 8-11
```

# Our error type — DbError

{{clear: slide}}

{{show: dberror-code typewriter 2s linear}} Every function in this course that can fail returns a `DbResult<T>`. Here's what that means.

{{focus: error-enum}} {{zoom: 1.2x}} {{annotate: error-enum "Closed set of failures"}} `DbError` is an enum class — a closed set. There are exactly six ways a database operation can fail. No more. The type says so.

{{zoom: 1x}} {{focus: kNotFound}} {{annotate: kNotFound "Row or key absent"}} `kNotFound` — the key or row you asked for doesn't exist. Normal in a lookup. Not a programmer error.

{{focus: kCorrupted}} {{annotate: kCorrupted "Invariant broken"}} `kCorrupted` — the data on disk violated a structural invariant. A page checksum mismatch. A node pointer that points nowhere. Something is wrong with the file.

{{focus: kOutOfMemory}} {{annotate: kOutOfMemory "Allocation failed"}} `kOutOfMemory` — the allocator returned null. On embedded targets this happens. We don't hide it.

{{focus: kIo}} {{annotate: kIo "Syscall failed"}} `kIo` — a read or write syscall failed. Disk full, permission error, hardware fault.

{{focus: kSyntax}} {{annotate: kSyntax "Bad SQL input"}} `kSyntax` — the SQL string the user provided is malformed. The tokenizer or parser rejected it.

{{focus: kFull}} {{annotate: kFull "Capacity reached"}} `kFull` — the database file has reached its maximum page count, or a page has no room for another cell.

{{focus: none}} {{clear: slide}}

{{show: dbresult-alias typewriter 1.5s linear}} The alias is one line.

{{focus: alias-def}} {{zoom: 1.4x}} {{annotate: alias-def "Single contract"}} `template <typename T> using DbResult = std::expected<T, DbError>;` — that's it. Every function that can fail returns this. One alias. One error type. No surprises.

{{zoom: 1x}} {{focus: example-sigs}} {{zoom: 1.2x}} {{annotate: example-sigs "Type describes contract"}} The signatures tell you everything. `DbResult<Page>` — either a `Page` or a `DbError`. `DbResult<uint32_t>` — either a page count or a `DbError`. No documentation needed.

{{zoom: 1x}} {{focus: none}} Every function in this course has the same contract. The caller always knows what to expect. No surprises.

{{focus: none}} {{clear: slide}}

{{show: error-handling-strategy typewriter 1.5s linear}} Notice what none of these functions do. They don't pick a recovery strategy. They don't log. They don't retry.

{{focus: report-only}} {{zoom: 1.3x}} {{annotate: report-only "Reports, doesn't decide"}} The function just reports what went wrong. That's it. The caller decides what to do — retry, log, surface to the user, abort. The function has no opinion.

{{zoom: 1x}} {{focus: caller-decides}} {{annotate: caller-decides "Recovery at the boundary"}} This is the right split. The database layer knows what went wrong. Only the application layer knows what to do about it.

{{zoom: 1x}} {{focus: none}} A function that logs and retries on `kIo` has taken on responsibilities that don't belong to it. Keep the function shallow. Let the caller be deep.

```code:dberror-code lang=cpp
#pragma once
#include <expected>

enum class DbError {
  kNotFound,
  kCorrupted,
  kOutOfMemory,
  kIo,
  kSyntax,
  kFull,
};

template <typename T>
using DbResult = std::expected<T, DbError>;
---
error-enum: 4-11
kNotFound: 5
kCorrupted: 6
kOutOfMemory: 7
kIo: 8
kSyntax: 9
kFull: 10
```

```code:dbresult-alias lang=cpp
#pragma once
#include <expected>

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

// Every failing function uses DbResult<T>:
DbResult<Page>     read_page(PageId id);
DbResult<uint32_t> page_count(int fd);
DbResult<Row>      decode_row(std::span<const uint8_t> bytes);
DbResult<void>     write_page(PageId id, std::span<const uint8_t> data);
---
alias-def: 6-7
example-sigs: 9-12
```

```code:error-handling-strategy lang=cpp
#include "db_result.h"

// This function reports failure. It does not recover.
DbResult<Page> read_page(PageId id) {
  auto bytes = syscall_read(id);
  if (!bytes) {
    return std::unexpected(DbError::kIo);   // report — nothing more
  }
  if (!checksum_valid(*bytes)) {
    return std::unexpected(DbError::kCorrupted);
  }
  return Page{std::move(*bytes)};
}

// The caller decides what recovery means.
// This function knows nothing about that.
---
report-only: 3-12
caller-decides: 14-15
```

# The contract in practice

{{clear: slide}}

{{show: full-chain typewriter 2s linear}} Let's put it together. A complete sequence: open the file, read the root page, decode the first row. Each step can fail. The chain propagates errors automatically.

{{focus: open-step}} {{zoom: 1.2x}} {{annotate: open-step "Fallible open"}} `open_db` tries to open the file. Failure means `kIo`. The rest of the chain never runs.

{{zoom: 1x}} {{focus: read-step}} {{zoom: 1.2x}} {{annotate: read-step "Fallible read"}} `read_page` reads bytes off disk. Failure means `kIo` or `kCorrupted`. The chain short-circuits.

{{zoom: 1x}} {{focus: decode-step}} {{zoom: 1.2x}} {{annotate: decode-step "Fallible decode"}} `decode_row` interprets the bytes as a `Row`. Failure means `kCorrupted` or `kSyntax`. The chain short-circuits.

{{zoom: 1x}} {{focus: chain-result}} {{zoom: 1.3x}} {{annotate: chain-result "One result surface"}} The final `DbResult<Row>` carries either the successfully decoded row, or the first error that fired in the chain. One type. One check. The entire error surface of a three-step operation.

{{zoom: 1x}} {{focus: none}} {{annotate: chain-result "Whole surface visible"}} The type encodes the entire error surface. Three operations, one result, one check.

{{focus: none}} {{clear: slide}}

{{show: full-match typewriter 1.5s linear}} Handling the result at the call site.

{{focus: match-block}} {{zoom: 1.2x}} {{annotate: match-block "Exhaustive error check"}} Every `DbError` variant handled. Not because the compiler forces a switch, but because we're being explicit about what each failure means at this level of the system.

{{zoom: 1x}} {{focus: success-branch}} {{zoom: 1.3x}} {{annotate: success-branch "Value used safely"}} We only reach `.value()` after confirming the result is a success. No panics. No undefined behavior.

{{zoom: 1x}} {{focus: none}} Notice what none of these functions do — they don't decide how to recover. They just report. The decision belongs to the caller. `load_first_row` reports. `main` decides.

{{focus: none}} {{clear: slide}}

{{show: lab-bridge typewriter 1.5s linear}} This is the exact pattern Lab 1 uses. You'll write a file reader that returns `DbResult<std::vector<uint8_t>>`.

{{focus: lab-sig}} {{zoom: 1.3x}} {{annotate: lab-sig "Lab 1 target"}} `read_file` — opens a path, reads all bytes, returns them. Any syscall failure returns `DbError::kIo`. The caller handles it.

{{zoom: 1x}} {{focus: lab-usage}} {{zoom: 1.2x}} {{annotate: lab-usage "Chain the result"}} The file bytes feed into a page parser. Each step is a `DbResult`. Propagation is mechanical: check, return on failure, continue on success.

{{zoom: 1x}} {{focus: none}} Every function in the storage engine follows this contract. You've just learned the one pattern that runs through the entire course.

```code:full-chain lang=cpp
#pragma once
#include <cstdint>
#include <span>
#include "db_result.h"

using PageId = uint32_t;
struct Row  { std::span<const uint8_t> key; std::span<const uint8_t> value; };
struct Page { std::span<const uint8_t> as_span() const; };

DbResult<int>  open_db(const char* path);
DbResult<Page> read_page(int fd, PageId id);
DbResult<Row>  decode_row(std::span<const uint8_t> bytes);

// Three operations, one expression.
DbResult<Row> load_first_row(const char* path) {
  return open_db(path)
    .and_then([](int fd)          { return read_page(fd, 0); })
    .and_then([](const Page& p)   { return decode_row(p.as_span()); });
}
---
open-step: 10, 16
read-step: 11, 17
decode-step: 12, 18
chain-result: 15-19
```

```code:full-match lang=cpp
#include <cstdio>
#include "db_result.h"

DbResult<Row> load_first_row(const char* path);

int main(int argc, const char** argv) {
  if (argc < 2) { return 1; }

  auto result = load_first_row(argv[1]);

  if (!result) {
    switch (result.error()) {
      case DbError::kIo:          std::fputs("io error\n", stderr);          break;
      case DbError::kCorrupted:   std::fputs("file corrupted\n", stderr);    break;
      case DbError::kNotFound:    std::fputs("no rows found\n", stderr);     break;
      case DbError::kOutOfMemory: std::fputs("out of memory\n", stderr);     break;
      case DbError::kSyntax:      std::fputs("bad sql syntax\n", stderr);    break;
      case DbError::kFull:        std::fputs("database full\n", stderr);     break;
    }
    return 1;
  }

  const Row& row = result.value();
  use_row(row);
  return 0;
}
---
match-block: 11-20
success-branch: 23-24
```

```code:lab-bridge lang=cpp
#pragma once
#include <cstdint>
#include <vector>
#include "db_result.h"

// Lab 1: implement this function.
// Opens path, reads all bytes, returns them.
// Returns DbError::kIo on any syscall failure.
DbResult<std::vector<uint8_t>> read_file(const char* path);

// Usage in the storage engine:
DbResult<Page> parse_first_page(const char* path) {
  auto bytes = read_file(path);
  if (!bytes) {
    return std::unexpected(bytes.error());
  }
  return parse_page(*bytes);
}
---
lab-sig: 8-9
lab-usage: 12-18
```
