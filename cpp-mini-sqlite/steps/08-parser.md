---
title: SQL Parser
---

# From tokens to structure

{{clear: slide}}

{{show: token-stream grow 0.5s spring}} The tokenizer gives us a flat sequence of tokens. The parser's job is to consume that sequence and produce something with shape.

{{focus: tok-select}} {{annotate: tok-select "Statement type"}} `SELECT` tells the parser which grammar rule to apply. It is consumed first.

{{focus: tok-star}} {{focus: tok-from}} {{annotate: tok-from "Table name follows"}} `*` and `FROM` are structural markers — they validate the grammar but carry no data. The identifier after `FROM` is the table name.

{{focus: tok-where}} {{focus: tok-ident}} {{focus: tok-eq}} {{focus: tok-int}} {{annotate: tok-where "Predicate starts here"}} `WHERE` triggers predicate parsing. `id = 1` becomes a `Predicate` struct with `col="id"`, `op=kEq`, `rhs.integer=1`.

{{focus: none}} {{clear: slide}}

{{show: ast-tree grow 0.5s spring}} This is the AST that results. Not a string, not a flat list — a structured tree of typed C++ values. Every field has a known type.

{{focus: stmt-node}} {{annotate: stmt-node "Root: statement kind"}} `SelectStmt` is the root. The `Stmt` variant holds it. The caller dispatches on it with `std::visit`.

{{focus: table-node}} {{annotate: table-node "Table as string_view"}} `table` is a `string_view` into the original SQL buffer — no copy. The parser's zero-allocation property is preserved here.

{{focus: where-node}} {{annotate: where-node "optional<Predicate>"}} `where` is `std::optional`. Its presence or absence is explicit in the type — no sentinel value, no null pointer.

{{focus: pred-col}} {{focus: pred-op}} {{focus: pred-rhs}} {{annotate: pred-col "Fully decomposed"}} The predicate is three fields: column name, operator, and right-hand side. Nothing is left as raw text. The executor will use `pred.op` as a `TokenKind` in a switch.

{{focus: none}} {{clear: slide}}

```data:token-stream type=linked-list
(tok-select "SELECT") -> (tok-star "*") -> (tok-from "FROM") -> (tok-table "users") -> (tok-where "WHERE") -> (tok-ident "id") -> (tok-eq "=") -> (tok-int "1") -> (tok-eof "EOF") -> null
---
tok-select: tok-select
tok-star: tok-star
tok-from: tok-from
tok-where: tok-where
tok-ident: tok-ident
tok-eq: tok-eq
tok-int: tok-int
```

```data:ast-tree type=graph layout=tree
(stmt-node "SelectStmt") -> (table-node "table: 'users'")
(stmt-node "SelectStmt") -> (where-node "where: Predicate")
(where-node "where: Predicate") -> (pred-col "col: id")
(where-node "where: Predicate") -> (pred-op "op: =")
(where-node "where: Predicate") -> (pred-rhs "rhs: 1")
---
stmt-node: stmt-node
table-node: table-node
where-node: where-node
pred-col: pred-col
pred-op: pred-op
pred-rhs: pred-rhs
tokens-row: tok-select
ast-row: stmt-node
```

# The AST node types

{{clear: slide}}

{{show: ast-types typewriter 2s linear}} The AST is built from five plain structs and one `std::variant`. Each struct maps directly to one SQL construct.

{{focus: literal-struct}} {{zoom: 1.2x}} {{annotate: literal-struct "Single SQL value"}} `Literal` holds one SQL value. The `is_null` flag distinguishes the SQL `NULL` keyword from an empty string or a zero integer — both of which are distinct, valid values. Only one of `integer` or `text` carries meaning at a time, controlled by which token kind was parsed.

{{zoom: 1x}} {{focus: predicate-struct}} {{zoom: 1.2x}} {{annotate: predicate-struct "WHERE comparison"}} `Predicate` represents a single WHERE comparison: `col op rhs`. The `op` field is a `TokenKind` — `kEq`, `kLt`, and so on — which avoids a parallel operator enum. `negate` handles `IS NOT NULL` as a modifier on the `kIs` operator without adding a new operator kind.

{{zoom: 1x}} {{focus: create-struct}} {{zoom: 1.2x}} {{annotate: create-struct "CREATE TABLE"}} `CreateStmt` stores the table name and the list of column names. Both are `string_view`s into the original SQL buffer — no string copies.

{{zoom: 1x}} {{focus: insert-struct}} {{zoom: 1.2x}} {{annotate: insert-struct "INSERT INTO"}} `InsertStmt` stores the table name and the value list. Values are `Literal` structs, so each can be NULL, an integer, or a string.

{{zoom: 1x}} {{focus: select-struct}} {{zoom: 1.2x}} {{annotate: select-struct "SELECT *"}} `SelectStmt` stores the table name and an optional predicate. `std::optional<Predicate>` makes the WHERE clause's presence or absence explicit in the type — there is no sentinel value or null pointer.

{{zoom: 1x}} {{focus: stmt-variant}} {{zoom: 1.2x}} {{annotate: stmt-variant "Top-level union"}} `Stmt` is a `std::variant` over the three statement kinds. The variant carries a tag internally; `std::visit` dispatches to the right handler. The caller never holds a raw pointer to a base class.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:ast-types lang=cpp
// Value in a WHERE predicate: either a bare integer or a string literal.
struct Literal {
  bool        is_null;   // true if the SQL NULL keyword was used
  uint32_t    integer;   // valid when !is_null && token was kInteger
  std::string text;      // valid when !is_null && token was kString
};

// A single WHERE predicate: col op value, or col IS [NOT] NULL
struct Predicate {
  std::string_view col;      // column name (view into original SQL)
  TokenKind        op;       // kEq, kLt, kGt, kLe, kGe, kNe, kIs
  bool             negate;   // true for IS NOT NULL
  Literal          rhs;
};

// CREATE TABLE name (col1, col2, ...)
struct CreateStmt {
  std::string_view              table;
  std::vector<std::string_view> cols;
};

// INSERT INTO name VALUES (val1, val2, ...)
struct InsertStmt {
  std::string_view         table;
  std::vector<Literal>     values;
};

// SELECT * FROM name [WHERE predicate]
struct SelectStmt {
  std::string_view          table;
  std::optional<Predicate>  where;
};

// Top-level statement: one of the three kinds
using Stmt = std::variant<CreateStmt, InsertStmt, SelectStmt>;
---
literal-struct: 2
predicate-struct: 10
create-struct: 18
insert-struct: 24
select-struct: 30
stmt-variant: 36
```

# The parser class

{{clear: slide}}

{{show: parser-class typewriter 2s linear}} The `Parser` class owns nothing but a position. It holds a `span<const Token>` — a non-owning view into the caller's token vector — and an integer cursor `pos_`.

{{focus: tokens-field}} {{zoom: 1.2x}} {{annotate: tokens-field "Non-owning view"}} `std::span<const Token>` carries a pointer and a length. The parser never allocates, copies, or extends the token sequence. The caller retains ownership of the vector; the span is just a safe window into it.

{{zoom: 1x}} {{focus: pos-field}} {{zoom: 1.2x}} {{annotate: pos-field "Single cursor"}} `pos_` is the only mutable state. Every token consumption advances it by one. No stack of saved positions, no backtracking — the grammar is simple enough that one pass suffices.

{{zoom: 1x}} {{focus: peek-fn}} {{zoom: 1.2x}} {{annotate: peek-fn "Look without consuming"}} `peek()` returns `tokens_[pos_]` without incrementing. It is used to test what the next token is before deciding whether to consume it — for example, checking whether a WHERE clause is present.

{{zoom: 1x}} {{focus: advance-fn}} {{zoom: 1.2x}} {{annotate: advance-fn "Consume and return"}} `advance()` returns the token at `pos_` and then increments. It is the low-level primitive; `expect()` wraps it with a kind check.

{{zoom: 1x}} {{focus: expect-decl}} {{zoom: 1.2x}} {{annotate: expect-decl "Consume or fail"}} `expect()` is the key error-checking primitive. It calls `advance()` and checks the kind. If the kind matches, it returns the token. If it does not match, it returns `std::unexpected(DbError::kSyntax)`. All sub-parsers are built on top of this single primitive.

{{focus: expect-decl}} {{annotate: expect-decl "Value type required"}} The return type is `DbResult<Token>` — a value, not a reference. `std::expected` requires its `T` parameter to be a regular, copyable type; a reference cannot be stored inside the wrapper.

{{zoom: 1x}} {{focus: ctor}} {{zoom: 1.2x}} {{annotate: ctor "Caller supplies tokens"}} The constructor takes the span and sets `pos_` to zero. There is nothing to initialize except the view and the cursor. The `parse()` call does all the work.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:parser-class lang=cpp
class Parser {
 public:
  explicit Parser(std::span<const Token> tokens) : tokens_(tokens), pos_(0) {}

  DbResult<Stmt> parse();

 private:
  std::span<const Token> tokens_;
  size_t                 pos_;

  const Token& peek() const { return tokens_[pos_]; }
  const Token& advance() { return tokens_[pos_++]; }

  // Consume the next token if it has the expected kind; else return kSyntax.
  DbResult<Token> expect(TokenKind kind);
};
---
ctor: 3
parse-decl: 5
tokens-field: 8
pos-field: 9
peek-fn: 11
advance-fn: 12
expect-decl: 14
```

# Parsing a SELECT

{{clear: slide}}

{{show: parse-select typewriter 2s linear}} `parse_select` is called after `parse()` has already consumed the `kSelect` token. The function handles the rest of the SELECT statement: the wildcard, the FROM clause, and an optional WHERE clause.

{{focus: star-expect}} {{zoom: 1.2x}} {{annotate: star-expect "Only SELECT * supported"}} The first thing `parse_select` does is expect a `kStar` token. This database only supports `SELECT *` — no column lists, no aliases. Expecting kStar here enforces that constraint at parse time.

{{zoom: 1x}} {{focus: from-expect}} {{zoom: 1.2x}} {{annotate: from-expect "FROM is required"}} After the star, a `kFrom` keyword must appear. Its value is discarded — the token serves only as a structural marker. If it is missing, `expect` returns `kSyntax` and the function propagates the error immediately.

{{zoom: 1x}} {{focus: name-expect}} {{zoom: 1.2x}} {{annotate: name-expect "Table name is an identifier"}} The table name is an identifier token. `name->value` is a `string_view` into the original SQL buffer — no copy. The struct is initialized with designated initializers so every unspecified field gets its default value.

{{zoom: 1x}} {{focus: stmt-init}} {{zoom: 1.2x}} {{annotate: stmt-init "Struct built incrementally"}} `SelectStmt` is initialized with only the table name. The `where` field starts as `std::nullopt`. If a WHERE clause is present, it will be filled in next.

{{zoom: 1x}} {{focus: where-check}} {{zoom: 1.2x}} {{annotate: where-check "Optional WHERE"}} `peek()` checks whether the next token is `kWhere` without consuming it. If it is not present, the function returns the statement as-is. If it is present, `advance()` consumes the keyword before parsing the predicate.

{{zoom: 1x}} {{focus: pred-call}} {{zoom: 1.2x}} {{annotate: pred-call "Delegate to parse_predicate"}} `parse_predicate` parses the full `col op rhs` or `col IS [NOT] NULL` expression. On success the predicate is moved into `stmt.where`. On failure the error propagates immediately.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:parse-select lang=cpp
DbResult<SelectStmt> Parser::parse_select() {
  // kSelect already consumed by parse().
  auto star = expect(TokenKind::kStar);
  if (!star) return std::unexpected(star.error());
  auto from = expect(TokenKind::kFrom);
  if (!from) return std::unexpected(from.error());
  auto name = expect(TokenKind::kIdent);
  if (!name) return std::unexpected(name.error());

  SelectStmt stmt{.table = name->value};

  // Optional WHERE clause.
  if (peek().kind == TokenKind::kWhere) {
    advance();  // consume kWhere
    auto pred = parse_predicate();
    if (!pred) return std::unexpected(pred.error());
    stmt.where = std::move(*pred);
  }

  return stmt;
}
---
star-expect: 3
from-expect: 5
name-expect: 7
stmt-init: 10
where-check: 13
pred-call: 15
```

# Parsing a predicate

{{clear: slide}}

{{show: parse-predicate typewriter 2s linear}} `parse_predicate` handles the WHERE clause body. It covers two forms: a comparison like `col = val` and the null check `col IS [NOT] NULL`.

{{focus: col-ident}} {{zoom: 1.2x}} {{annotate: col-ident "Column name first"}} Every predicate starts with a column identifier. `expect(kIdent)` consumes it and returns `kSyntax` immediately if something else appears.

{{zoom: 1x}} {{focus: op-advance}} {{zoom: 1.2x}} {{annotate: op-advance "Operator consumed raw"}} The operator is consumed with bare `advance()` rather than `expect()` because the valid set spans multiple kinds. A switch dispatches on the result; anything outside the recognized set hits `default` and returns `kSyntax`.

{{zoom: 1x}} {{focus: is-branch}} {{zoom: 1.2x}} {{annotate: is-branch "IS [NOT] NULL path"}} The `kIs` branch differs from the comparison operators. It uses `peek()` to check for an optional `kNot` before requiring `kNull`. The `negate` flag captures the IS NOT distinction without adding a separate operator kind.

{{focus: not-peek}} {{zoom: 1.2x}} {{annotate: not-peek "One-token lookahead"}} `peek()` reads without consuming. Only if `kNot` is actually present does `advance()` consume it. This is the only place in the parser where the next token is examined before the decision to consume.

{{zoom: 1x}} {{focus: cmp-branch}} {{zoom: 1.2x}} {{annotate: cmp-branch "Comparison falls through"}} For the six comparison operators, execution falls through the switch to `parse_literal()`. The rhs is whatever literal follows — an integer, a string, or NULL.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:parse-predicate lang=cpp
DbResult<Predicate> Parser::parse_predicate() {
  auto col_tok = expect(TokenKind::kIdent);
  if (!col_tok) return std::unexpected(col_tok.error());
  std::string_view col = col_tok->value;

  const Token& op_tok = advance();
  TokenKind op = op_tok.kind;
  switch (op) {
    case TokenKind::kEq: case TokenKind::kLt: case TokenKind::kGt:
    case TokenKind::kLe: case TokenKind::kGe: case TokenKind::kNe:
      break;
    case TokenKind::kIs: {
      bool negate = false;
      if (peek().kind == TokenKind::kNot) {
        advance();
        negate = true;
      }
      auto null_tok = expect(TokenKind::kNull);
      if (!null_tok) return std::unexpected(null_tok.error());
      return Predicate{col, TokenKind::kIs, negate, Literal{.is_null = true}};
    }
    default:
      return std::unexpected(DbError::kSyntax);
  }

  auto rhs = parse_literal();
  if (!rhs) return std::unexpected(rhs.error());
  return Predicate{col, op, false, std::move(*rhs)};
}
---
col-ident: 2
op-advance: 6
is-branch: 12
not-peek: 14
cmp-branch: 26
```

# The dispatch and expect

{{clear: slide}}

{{show: parse-dispatch typewriter 2s linear}} `parse()` is the entry point. It consumes the first token and dispatches to the appropriate sub-parser. `expect()` is the single error-checking primitive used everywhere below it.

{{focus: advance-first}} {{zoom: 1.2x}} {{annotate: advance-first "First token determines kind"}} `parse()` calls `advance()` immediately to consume the leading keyword. That keyword is the only information needed to select the right sub-parser. All three statement kinds begin with a unique keyword.

{{zoom: 1x}} {{focus: select-branch}} {{zoom: 1.2x}} {{annotate: select-branch "SELECT dispatch"}} If the first token is `kSelect`, `parse_select` handles the rest. The returned `SelectStmt` is implicitly converted to `Stmt` by the variant constructor — no explicit wrapping needed.

{{zoom: 1x}} {{focus: create-branch}} {{zoom: 1.2x}} {{annotate: create-branch "CREATE dispatch"}} Same pattern for `kCreate`. `parse_create` expects `TABLE name (cols...)`. The leading `kCreate` has already been consumed.

{{zoom: 1x}} {{focus: insert-branch}} {{zoom: 1.2x}} {{annotate: insert-branch "INSERT dispatch"}} Same pattern for `kInsert`. `parse_insert` expects `INTO name VALUES (vals...)`.

{{zoom: 1x}} {{focus: unknown-branch}} {{zoom: 1.2x}} {{annotate: unknown-branch "Anything else is kSyntax"}} If the first token is not one of the three known keywords, the SQL is structurally invalid. There is no error recovery — the function returns `kSyntax` immediately.

{{zoom: 1x}} {{focus: expect-impl}} {{zoom: 1.2x}} {{annotate: expect-impl "One line of logic"}} `expect` is deliberately minimal. It calls `advance()`, checks the kind, and either returns the token or returns `kSyntax`. Every caller handles the error immediately with the same `if (!result) return ...` pattern. There is no exception, no recovery, no retry.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:parse-dispatch lang=cpp
DbResult<Stmt> Parser::parse() {
  const Token& first = advance();
  if (first.kind == TokenKind::kSelect) {
    auto s = parse_select();
    if (!s) return std::unexpected(s.error());
    return *s;
  }
  if (first.kind == TokenKind::kCreate) {
    auto s = parse_create();
    if (!s) return std::unexpected(s.error());
    return *s;
  }
  if (first.kind == TokenKind::kInsert) {
    auto s = parse_insert();
    if (!s) return std::unexpected(s.error());
    return *s;
  }
  return std::unexpected(DbError::kSyntax);
}

DbResult<Token> Parser::expect(TokenKind kind) {
  Token t = advance();
  if (t.kind != kind) return std::unexpected(DbError::kSyntax);
  return t;
}
---
advance-first: 2
select-branch: 3
create-branch: 8
insert-branch: 13
unknown-branch: 18
expect-impl: 21
```
