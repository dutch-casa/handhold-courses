# Lab 08: Implement the SQL Tokenizer

## Context

In Lesson 7 you saw how a SQL string is split into tokens before parsing.
Your job is to implement `tokenize` in `src/solution.cpp`.

The function is declared in `src/solution.h`. Do not modify that file.

## What to implement

Open `src/solution.cpp` and implement:

    DbResult<std::vector<Token>> tokenize(std::string_view sql)

## Spec

1. Skip all whitespace (`std::isspace`).
2. If the current character starts an identifier or keyword (`std::isalpha` or `_`):
   - Collect all `std::isalnum` or `_` characters.
   - Lowercase the collected text and look it up in the keyword map.
   - Push a `kIdent` token if not found, or the matching keyword kind if found.
3. If the current character is a digit (`std::isdigit`):
   - Collect all digit characters.
   - Push a `kInteger` token.
4. If the current character is `'`:
   - Scan forward for the matching `'`. If end-of-input is reached first, return `std::unexpected(DbError::kSyntax)`.
   - Push a `kString` token whose `value` includes both quotes.
5. Handle punctuation: `(` `)` `,` `*` `;` `=` `<` `>` `<=` `>=` `!=`
6. Any other character: return `std::unexpected(DbError::kSyntax)`.
7. After the loop, push `{TokenKind::kEof, sql.substr(pos, 0)}` and return the token vector.

The keyword map (static, case-insensitive): select, insert, into, create, table, from, where, values, null, not, is, and, or.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All nine tests must pass. The stub always returns `kSyntax`, so eight tests fail
until you implement the function (`UnterminatedString` passes immediately).
