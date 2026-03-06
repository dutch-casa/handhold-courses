# Lab 09: Parse SQL Statements

## Context

In Lesson 8 you saw how a flat token stream is transformed into an AST. Your
job is to implement the `Parser` methods in `src/solution.cpp`.

The `Parser` class and all AST types are declared in `src/solution.h`. Do not
modify that file.

## What to implement

Open `src/solution.cpp` and implement:

1. `Parser::expect(TokenKind kind)` — advance and return the token if it matches `kind`; otherwise return `std::unexpected(DbError::kSyntax)`.
2. `Parser::parse_literal()` — parse a `kNull`, `kInteger`, or `kString` token into a `Literal`.
3. `Parser::parse_predicate()` — parse `col op rhs` or `col IS [NOT] NULL`.
4. `Parser::parse_create()` — parse `TABLE name (col, ...)` (kCreate already consumed).
5. `Parser::parse_insert()` — parse `INTO name VALUES (val, ...)` (kInsert already consumed).
6. `Parser::parse_select()` — parse `* FROM name [WHERE pred]` (kSelect already consumed).
7. `Parser::parse()` — consume the first token, dispatch to the right sub-parser, return a `Stmt`.

## Spec

### expect
1. Call `advance()`.
2. If `t.kind == kind`, return `t`.
3. Otherwise return `std::unexpected(DbError::kSyntax)`.

### parse_literal
1. `peek()` the current token.
2. If `kNull`: `advance()`, return `Literal{.is_null = true}`.
3. If `kInteger`: `advance()`, convert `value` to `unsigned long` via `std::stoul`, check that the value fits in `uint32_t` (return `kSyntax` if not), then return `Literal{.integer = static_cast<uint32_t>(raw)}`.
4. If `kString`: `advance()`, strip the outer quotes (`value.substr(1, value.size()-2)`), return `Literal{.text = std::string(stripped)}`.
5. Otherwise: return `std::unexpected(DbError::kSyntax)`.

### parse_predicate
1. `expect(kIdent)` for the column name.
2. `advance()` for the operator. Must be one of: kEq, kLt, kGt, kLe, kGe, kNe, kIs. Otherwise kSyntax.
3. If operator is `kIs`:
   a. If `peek().kind == kNot`: advance, set `negate = true`.
   b. `expect(kNull)`.
   c. Return `Predicate{col, kIs, negate, Literal{.is_null=true}}`.
4. Otherwise: `parse_literal()` for `rhs`.
5. Return `Predicate{col, op, false, rhs}`.

### parse_create
1. `expect(kTable)`, then `expect(kIdent)` for table name.
2. `expect(kLparen)`.
3. Loop:
   a. `expect(kIdent)`, push to `cols`.
   b. If `peek().kind == kComma`: `advance()` and repeat.
   c. Else: break.
4. `expect(kRparen)`.
5. Return `CreateStmt{table, cols}`.

### parse_insert
1. `expect(kInto)`, then `expect(kIdent)` for table name.
2. `expect(kValues)`, then `expect(kLparen)`.
3. Loop:
   a. `parse_literal()`, push to `values`.
   b. If `peek().kind == kComma`: `advance()` and repeat.
   c. Else: break.
4. `expect(kRparen)`.
5. Return `InsertStmt{table, values}`.

### parse_select
1. `expect(kStar)` — only `SELECT *` is supported.
2. `expect(kFrom)`.
3. `expect(kIdent)` for table name.
4. If `peek().kind == kWhere`: `advance()`, `parse_predicate()`, assign to `stmt.where`.
5. Return `SelectStmt{table, where}`.

### parse
1. `advance()` to get the first token.
2. If `kSelect`: call `parse_select()`, wrap in `Stmt`.
3. If `kCreate`: call `parse_create()`, wrap in `Stmt`.
4. If `kInsert`: call `parse_insert()`, wrap in `Stmt`.
5. Otherwise: return `std::unexpected(DbError::kSyntax)`.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All ten tests must pass. Every stub method returns `kSyntax`, so all ten
fail until you implement the methods.
