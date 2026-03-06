# Lab 10: Execute SELECT (no WHERE)

## Context

The parser gives us a `SelectStmt`. The executor's job is to scan a row set
and project each `Row` into a `ResultRow` — a string-owning, public-interface
type that callers can use without knowing about the wire format.

## What to implement

Open `src/solution.cpp` and implement:

1. `execute_select(rows, stmt)` — iterate `rows`, convert each `Row` into a
   `ResultRow`, return the full vector. The `stmt` parameter is accepted but
   the WHERE field is ignored in this lab.

## Spec

### execute_select

1. Allocate a `std::vector<ResultRow> result`.
2. For each `row` in `rows`:
   a. Extract `id = row.id`.
   b. Set `name_null = (row.null_mask & 1) != 0`.
   c. If `name_null`: set `name = ""`. Otherwise: set `name = std::string(row.name, strnlen(row.name, kNameMax))`.
   d. Push `ResultRow{id, name_null, name}` into `result`.
3. Return `result`.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All eight tests must pass. The stub returns `kNotFound`, so all eight fail
until you implement `execute_select`.
