# Lab 12: Full Pipeline Capstone

## Context

You now have a tokenizer, a parser, and an executor. This lab connects them
into a single function: `run_query(sql, rows)`. Feed it a SQL string and a row
set; get back a `vector<ResultRow>`.

All the types from previous labs are reproduced inline in `solution.h`. The
header is self-contained — you do not need to copy other files.

## What to implement

Open `src/solution.cpp` and implement:

1. `tokenize(sql)` — the tokenizer from Lab 8.
2. `Parser::expect`, `Parser::parse_literal`, `Parser::parse_predicate`,
   `Parser::parse_select`, `Parser::parse()` — the parser from Lab 9
   (only SELECT is needed).
3. `execute_select(rows, stmt)` — from Lab 10.
4. `eval_predicate(row, pred)` — from Lab 11.
5. `execute_select_where(rows, stmt)` — from Lab 11.
6. `run_query(sql, rows)` — the pipeline entry point.

## Spec

### run_query

1. Call `tokenize(sql)`. Return its error on failure.
2. Call `Parser(tokens).parse()`. Return its error on failure.
3. If the result holds a `SelectStmt`: call `execute_select_where(rows, stmt)`.
4. Otherwise: return `std::unexpected(DbError::kSyntax)`.

The capstone only handles SELECT. INSERT and CREATE return `kSyntax`.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All ten tests must pass. The stub returns `kSyntax` for everything, so all
ten fail until the pipeline is wired together.
