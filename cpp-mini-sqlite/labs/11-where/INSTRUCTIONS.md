# Lab 11: WHERE Predicate Evaluation

## Context

Lab 10 returned every row. This lab adds `eval_predicate` and
`execute_select_where`, which filter results based on a parsed `Predicate`.

## What to implement

Open `src/solution.cpp` and implement:

1. `eval_predicate(row, pred)` — evaluate the predicate against a single
   result row and return true if the row satisfies it.
2. `execute_select_where(rows, stmt)` — call `execute_select`, then filter the
   result by `eval_predicate`. If `stmt.where` is absent, return all rows.

## Spec

### eval_predicate

Column dispatch:

- If `pred.col == "id"`: compare `row.id` (uint32_t) against
  `pred.rhs.integer` using `pred.op`.
  - kEq: `row.id == rhs`
  - kLt: `row.id < rhs`
  - kGt: `row.id > rhs`
  - kLe: `row.id <= rhs`
  - kGe: `row.id >= rhs`
  - kNe: `row.id != rhs`
  - Anything else: return false.
- If `pred.col == "name"`:
  - If `pred.op == kIs`: return `row.name_null == !pred.negate`.
    - negate=false: IS NULL → row.name_null must be true.
    - negate=true: IS NOT NULL → row.name_null must be false.
  - Otherwise: compare `row.name` string against `pred.rhs.text` using
    `pred.op` (string comparison).
- Unknown column: return false.

### execute_select_where

1. Call `execute_select(rows, stmt)` — return error on failure.
2. If `!stmt.where`, return the full result.
3. Filter with `eval_predicate`: retain only rows where
   `eval_predicate(row, *stmt.where)` is true.
4. Return the filtered vector.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All twelve tests must pass. The stubs return `kNotFound` / `false`, so all
twelve fail until you implement both functions.
