---
title: SQL Tokenizer
---

# Why tokenize?

{{clear: slide}}

{{show: pipeline slide 0.4s ease-out}} A SQL string like `"SELECT * FROM users"` cannot be interpreted directly.

{{focus: tokenize-row}} The database cannot look at a raw sequence of characters and know where one word ends and the next begins. {{focus: none}} The first stage of the SQL layer solves this: the tokenizer splits the raw string into a flat list of tokens, each carrying a kind and the text span it covers.

{{focus: tokenize-row}} {{annotate: tokenize-row "Stage 1: string → tokens"}} Tokenization is purely mechanical — scan left to right, emit one token per recognized unit. The tokenizer makes no judgment about whether the token sequence is valid SQL. That is the parser's job.

{{focus: parse-row}} {{annotate: parse-row "Stage 2: tokens → AST"}} The parser takes the flat token stream and builds a structured tree — an abstract syntax tree that represents the grammatical shape of the statement.

{{focus: execute-row}} {{annotate: execute-row "Stage 3: AST → rows"}} The executor walks the AST and performs the actual database operation: scanning pages, evaluating predicates, returning result rows.

{{focus: none}} Splitting into three stages gives each one a single, narrow responsibility. Tokenizer errors (unrecognized character, unterminated string) are reported at the character level. Parser errors (missing keyword, wrong structure) are reported at the token level. The executor never sees raw text at all.

{{clear: slide}}

```diagram:pipeline
sql [client: SQL string]
tok [service: Tokenizer]
par [service: Parser]
exe [service: Executor]
out [client: ResultRow vector]

sql --"string_view"--> tok
tok --"vector<Token>"--> par
par --"Stmt variant"--> exe
exe --"vector<ResultRow>"--> out
{SQL Layer: sql, tok, par}
{Execution: exe, out}
---
tokenize-row: tok
parse-row: par
execute-row: exe
```

# Token types

{{clear: slide}}

{{show: token-stream slide 0.5s ease-out}} Before the code: here is what tokenizing `SELECT id FROM users WHERE id = 42` actually produces. One token per slot.

{{focus: kw-select}} {{annotate: kw-select "kSelect"}} `SELECT` — keyword, kind=kSelect.

{{focus: kw-id}} {{annotate: kw-id "kIdent"}} `id` — not a keyword, kind=kIdent. The value is the original-case source span.

{{focus: kw-from}} {{annotate: kw-from "kFrom"}} `FROM` — keyword, kind=kFrom. Case-insensitive — `from` would produce the same kind.

{{focus: kw-users}} {{annotate: kw-users "kIdent"}} `users` — table name, kind=kIdent. The tokenizer doesn't know it's a table. That's the parser's job.

{{focus: kw-42}} {{annotate: kw-42 "kInteger"}} `42` — literal, kind=kInteger. Value is the raw string `"42"`. The executor converts it to a number when needed.

{{focus: kw-eof}} {{annotate: kw-eof "kEof — always last"}} Every stream ends with kEof. The parser reads until it sees this sentinel and never over-runs the vector.

{{focus: none}} {{clear: slide}}

```data:token-stream type=array
["kSelect: SELECT", "kIdent: id", "kFrom: FROM", "kIdent: users", "kWhere: WHERE", "kIdent: id", "kEq: =", "kInteger: 42", "kEof: «end»"]
---
kw-select: 0
kw-id: 1
kw-from: 2
kw-users: 3
kw-42: 7
kw-eof: 8
```

{{show: token-types typewriter 2s linear}} Every token has two fields: a `kind` that classifies it, and a `value` — a `std::string_view` pointing into the original SQL string. No copying. The entire token stream is a view into one allocation.

{{focus: keyword-group}} {{zoom: 1.2x}} {{annotate: keyword-group "13 SQL keywords"}} The keyword group covers every reserved word the mini-SQLite SQL subset needs: `SELECT`, `INSERT`, `INTO`, `CREATE`, `TABLE`, `FROM`, `WHERE`, `VALUES`, `NULL`, `NOT`, `IS`, `AND`, `OR`. Keywords are matched case-insensitively — `SELECT` and `select` both produce `kSelect`.

{{zoom: 1x}} {{focus: literal-group}} {{zoom: 1.2x}} {{annotate: literal-group "3 value types"}} `kInteger` is a run of decimal digits. `kString` is a single-quoted literal including both quote characters in `value`. `kIdent` is any unquoted alphanumeric-plus-underscore word that does not match a keyword.

{{zoom: 1x}} {{focus: punct-group}} {{zoom: 1.2x}} {{annotate: punct-group "5 single-char tokens"}} `kLparen`, `kRparen`, `kComma`, `kStar`, and `kSemicolon` map to exactly one character each.

{{zoom: 1x}} {{focus: op-group}} {{zoom: 1.2x}} {{annotate: op-group "6 comparison operators"}} `kEq` (`=`), `kLt` (`<`), `kGt` (`>`), `kLe` (`<=`), `kGe` (`>=`), `kNe` (`!=`). Two-character operators require peeking one character ahead.

{{zoom: 1x}} {{focus: eof-line}} {{zoom: 1.2x}} {{annotate: eof-line "Stream terminator"}} `kEof` is always the last token. It lets the parser consume tokens without bounds-checking the vector — when it sees `kEof`, it knows the input is exhausted.

{{zoom: 1x}} {{focus: token-struct}} {{zoom: 1.2x}} {{annotate: token-struct "Zero-copy value"}} `Token::value` is a `std::string_view` into the caller's SQL buffer. The tokenizer never copies or allocates string data. Original case is preserved in `value` even for keywords — the `kind` field carries the semantic classification.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:token-types lang=cpp
enum class TokenKind {
  // keywords
  kSelect, kInsert, kInto, kCreate, kTable, kFrom, kWhere, kValues, kNull, kNot, kIs, kAnd, kOr,
  // literals / identifier
  kIdent, kInteger, kString,
  // punctuation
  kLparen, kRparen, kComma, kStar, kSemicolon,
  // operators
  kEq, kLt, kGt, kLe, kGe, kNe,
  // sentinel
  kEof,
};

struct Token {
  TokenKind        kind;
  std::string_view value;  // view into source SQL; original case preserved
};
---
keyword-group: 3
literal-group: 5
punct-group: 7
op-group: 9
eof-line: 11
token-struct: 14
```

# The tokenize loop

{{clear: slide}}

{{show: tokenize-loop typewriter 2s linear}} The `tokenize` function takes the raw SQL string and returns a `DbResult` containing the token vector. The outer structure is a single `while` loop that walks `pos` through every character.

{{focus: fn-sig}} {{annotate: fn-sig "DbResult wraps the vector"}} The return type is `DbResult<std::vector<Token>>`. On success the caller gets the token vector. On failure — an unterminated string literal, an unrecognized character — it gets a `DbError` without any partial result.

{{focus: loop-header}} {{annotate: loop-header "Advance until exhausted"}} The loop runs while `pos < sql.size()`. Every branch inside the loop must advance `pos` by at least one character, or the loop would spin forever. Whitespace, keywords, literals, and punctuation all move `pos` forward before re-entering the condition.

{{focus: skip-ws}} {{annotate: skip-ws "Whitespace is never a token"}} If the current character is whitespace, increment `pos` and `continue`. Whitespace separates tokens but carries no meaning. The tokenizer produces no whitespace tokens — the parser never has to filter them out.

{{focus: eof-push}} {{annotate: eof-push "Sentinel always appended"}} After the loop, `kEof` is pushed with an empty `string_view` at the final position. The parser treats `kEof` as the natural end of input — it can `peek()` without checking vector bounds.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:tokenize-loop lang=cpp
DbResult<std::vector<Token>> tokenize(std::string_view sql) {
  std::vector<Token> tokens;
  size_t pos = 0;
  while (pos < sql.size()) {
    char c = sql[pos];
    if (std::isspace(static_cast<unsigned char>(c))) { ++pos; continue; }
    // dispatch on c ...
  }
  tokens.push_back({TokenKind::kEof, sql.substr(pos, 0)});
  return tokens;
}
---
fn-sig: 1
loop-header: 4
skip-ws: 6
eof-push: 9
```

# Keyword recognition

{{clear: slide}}

{{show: keyword-fn typewriter 2s linear}} When the current character is alphabetic or an underscore, it begins an identifier or a keyword. The tokenizer collects the full word before deciding which it is.

{{focus: collect-loop}} {{zoom: 1.2x}} {{annotate: collect-loop "Collect alphanumeric + underscore"}} The inner loop advances `pos` while the character is alphanumeric or `_`. The span from `start` to `pos` is the raw word from the source SQL — original case, no copy.

{{zoom: 1x}} {{focus: lower-xform}} {{zoom: 1.2x}} {{annotate: lower-xform "Case-insensitive matching"}} A lowercase copy of `word` is produced with `std::transform` and `std::tolower`. SQL keywords are case-insensitive by specification: `SELECT`, `Select`, and `select` must all resolve to `kSelect`.

{{zoom: 1x}} {{focus: keyword-map}} {{zoom: 1.2x}} {{annotate: keyword-map "Static lookup table"}} The map is `static const` — it is initialized once and reused across every call to `tokenize`. The entries key on `string_view` pointing directly into string literals — no heap allocation for the map's key strings. All 13 keywords are present.

{{zoom: 1x}} {{focus: map-lookup}} {{zoom: 1.2x}} {{annotate: map-lookup "Hit → keyword, miss → identifier"}} `kKeywords.find(lower)` returns end on a miss. A miss means the word is a user-defined identifier: `kIdent`. A hit means it is a reserved keyword: use the mapped `TokenKind`. Either way, `value` is always the original-case `word` from the source.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:keyword-fn lang=cpp
// Collect the identifier/keyword text.
size_t start = pos;
while (pos < sql.size() && (std::isalnum(static_cast<unsigned char>(sql[pos])) || sql[pos] == '_'))
  ++pos;
std::string_view word = sql.substr(start, pos - start);

// Case-insensitive keyword lookup.
std::string lower(word.size(), '\0');
std::transform(word.begin(), word.end(), lower.begin(),
  [](unsigned char c){ return std::tolower(c); });

static const std::unordered_map<std::string_view, TokenKind> kKeywords = {
  {"select", TokenKind::kSelect}, {"insert", TokenKind::kInsert},
  {"into",   TokenKind::kInto},   {"create", TokenKind::kCreate},
  {"table",  TokenKind::kTable},  {"from",   TokenKind::kFrom},
  {"where",  TokenKind::kWhere},  {"values", TokenKind::kValues},
  {"null",   TokenKind::kNull},   {"not",    TokenKind::kNot},
  {"is",     TokenKind::kIs},     {"and",    TokenKind::kAnd},
  {"or",     TokenKind::kOr},
};
auto it = kKeywords.find(lower);
tokens.push_back({it != kKeywords.end() ? it->second : TokenKind::kIdent, word});
---
collect-loop: 2
lower-xform: 8
keyword-map: 12
map-lookup: 21
```

# Literals and errors

{{clear: slide}}

{{show: literals-fn typewriter 2s linear}} After keywords and punctuation, the remaining cases are numeric literals, string literals, and the error path. Each has a distinct first character that makes the dispatch unambiguous.

{{focus: int-branch}} {{zoom: 1.2x}} {{annotate: int-branch "Digit starts an integer"}} `std::isdigit` on the current character triggers the integer path. Integers in this SQL subset are non-negative decimal sequences — no sign, no hex, no floats.

{{focus: int-scan}} {{zoom: 1.2x}} {{annotate: int-scan "Scan until non-digit"}} The inner loop advances `pos` while digits continue. The resulting span from `start` to `pos` is the integer literal text. No conversion happens here — the executor parses the numeric value when it needs it.

{{zoom: 1x}} {{focus: str-start}} {{zoom: 1.2x}} {{annotate: str-start "Single-quote opens a string"}} A `'` character opens a string literal. `pos` is saved as `start`, then incremented past the opening quote before scanning begins. The `value` of the resulting token will include both the opening and closing quote.

{{focus: str-scan}} {{zoom: 1.2x}} {{annotate: str-scan "Scan for closing quote"}} The inner loop advances `pos` until it finds another `'` or exhausts the input. This tokenizer does not support escape sequences — a literal `'` always terminates the string.

{{focus: str-error}} {{zoom: 1.2x}} {{annotate: str-error "EOF inside string is kSyntax"}} If `pos == sql.size()` after the scan loop, no closing quote was found. The SQL is malformed — return `std::unexpected(DbError::kSyntax)`. The caller receives an error, not a partial token vector.

{{zoom: 1x}} {{focus: unknown-error}} {{zoom: 1.2x}} {{annotate: unknown-error "Unrecognized char is kSyntax"}} Any character that is not whitespace, alphanumeric, a recognized punctuation character, or a quote is illegal in this SQL subset. The function returns `kSyntax` immediately. No partial output is returned.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:literals-fn lang=cpp
// Integer literal: one or more decimal digits.
if (std::isdigit(static_cast<unsigned char>(c))) {
  size_t start = pos;
  while (pos < sql.size() && std::isdigit(static_cast<unsigned char>(sql[pos]))) ++pos;
  tokens.push_back({TokenKind::kInteger, sql.substr(start, pos - start)});
  continue;
}

// String literal: 'text' — no escape sequences.
if (c == '\'') {
  size_t start = pos++;          // skip opening quote
  while (pos < sql.size() && sql[pos] != '\'') ++pos;
  if (pos == sql.size()) return std::unexpected(DbError::kSyntax);
  ++pos;                         // skip closing quote
  tokens.push_back({TokenKind::kString, sql.substr(start, pos - start)});
  continue;
}

// Unknown character.
return std::unexpected(DbError::kSyntax);
---
int-branch: 2
int-scan: 4
str-start: 10
str-scan: 12
str-error: 13
unknown-error: 19
```
