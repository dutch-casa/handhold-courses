---
title: The Execution Engine
---

# Parse → Execute

{{clear: slide}}

{{show: pipeline-diagram slide 0.4s ease-out}} The SQL pipeline has three stages. Each one has a narrow input, a narrow output, and one job.

{{focus: tokenizer-stage}} {{annotate: tokenizer-stage "Stage 1: lex"}} The tokenizer converts a raw string into a flat vector of tokens. It knows nothing about rows, tables, or predicates.

{{focus: parser-stage}} {{annotate: parser-stage "Stage 2: parse"}} The parser folds those tokens into a typed AST — a `SelectStmt`, `InsertStmt`, or `CreateStmt`. It knows nothing about storage.

{{focus: executor-stage}} {{annotate: executor-stage "Stage 3: execute"}} The executor takes the AST and the row data and produces results. It is the only stage that touches actual rows.

{{focus: rows-node}} {{annotate: rows-node "Storage is separate"}} The row data comes from storage — it is not part of the SQL pipeline. The executor is the bridge between the two.

{{focus: ast-box}} {{annotate: ast-box "Input: typed AST"}} The executor receives a `SelectStmt` — a struct with a table name and an optional predicate. It does not re-parse anything.

{{focus: result-box}} {{annotate: result-box "Output: result rows"}} The executor returns a `vector<ResultRow>`. Nothing about wire format, null masks, or char arrays escapes through this interface.

{{focus: error-path}} {{annotate: error-path "Errors stay typed"}} If execution fails, it returns a `DbResult` error. No exceptions, no sentinels.

{{focus: none}} {{clear: slide}}

```diagram:pipeline-diagram
tok [service: Tokenizer]
par [service: Parser]
exe [service: Executor]
rows-node [database: B-tree pages]
result-box [client: ResultRow vector]

tok --"vector<Token>"--> par
par --"Stmt variant"--> exe
rows-node --"page reads"--> exe
exe --"vector<ResultRow>"--> result-box
{SQL parsing: tok, par}
{Execution: exe, rows-node}
---
tokenizer-stage: tok
parser-stage: par
executor-stage: exe
ast-box: par
result-box: result-box
error-path: exe
rows-node: rows-node
```

# The ResultRow type

{{clear: slide}}

{{show: result-row-type typewriter 2s linear}} `ResultRow` is not `Row`.

{{focus: row-type}} {{annotate: row-type "Wire format"}} `Row` is the storage-layer type: a fixed 40-byte struct with a `char[32]` name field and a `null_mask` byte encoding nullability as a bitmask. {{annotate: row-type "Unsuitable as API"}} It is efficient for the codec and unsuitable as a public API.

{{focus: result-row-type}} {{annotate: result-row-type "Projected interface"}} `ResultRow` owns its data. {{focus: name-field}} The `name` field is a `std::string` — not a view, not a fixed-size buffer. {{focus: name-null-field}} The `name_null` field is a plain `bool` — not a bitmask bit. {{annotate: name-null-field "Bitmask hidden"}} Callers never need to know that `null_mask & 1` encodes anything.

{{focus: id-field}} {{annotate: id-field "Identity unchanged"}} `id` passes through unchanged. {{annotate: id-field "Same type"}} `uint32_t` is the same in both types — no conversion needed.

{{focus: name-null-field}} {{annotate: name-null-field "Null encoded clearly"}} `name_null` is a dedicated boolean. {{annotate: name-null-field "Hidden in one place"}} The codec's `null_mask & 1` expression is an implementation detail; it never appears outside `execute_select`.

{{focus: name-field}} {{annotate: name-field "String ownership"}} `std::string` allocates and owns the name bytes. {{focus: name-null-field}} When `name_null` is true, `name` is empty. {{annotate: name-null-field "Flag is authority"}} The empty string is not a meaningful value in that case — `name_null` is the authority.

{{focus: invariant-comment}} {{annotate: invariant-comment "Invariant in the type"}} The rule "name is empty when name_null is true" is stated as a comment because it cannot be enforced by the type alone — C++ has no dependent types. {{annotate: invariant-comment "Readable contract"}} The comment encodes the invariant so future readers do not have to reverse-engineer it.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:result-row-type lang=cpp
// Wire-format row from the storage layer.
struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is SQL NULL
  char     name[32];
};

// Projected result row: stable, string-owning public interface type.
// Invariant: name is empty when name_null is true.
struct ResultRow {
  uint32_t    id;         // matches Row::id exactly
  bool        name_null;  // decoded from null_mask bit 0
  std::string name;       // empty when name_null is true
};
---
row-type: 2
result-row-type: 10
id-field: 11
name-null-field: 12
name-field: 13
invariant-comment: 9
```

# The scan loop

{{clear: slide}}

{{show: scan-loop typewriter 2s linear}} The scan is a plain range-for loop over `rows`.

{{focus: reserve-call}} {{zoom: 1.2x}} {{annotate: reserve-call "Pre-allocate"}} `reserve` eliminates reallocation. {{annotate: reserve-call "O(1) push_back"}} The output vector will hold at most `rows.size()` elements; reserving that count up front makes the push_backs free.

{{zoom: 1x}} {{focus: null-decode}} {{zoom: 1.2x}} {{annotate: null-decode "Decode null bit"}} `(row.null_mask & 1u) != 0` extracts bit 0 from the null mask. {{annotate: null-decode "Unsigned arithmetic"}} The `1u` ensures unsigned arithmetic — no signed shift hazard. {{annotate: null-decode "Single occurrence"}} This is the only place the bitmask encoding is visible.

{{zoom: 1x}} {{focus: name-string}} {{zoom: 1.2x}} {{annotate: name-string "Build owned string"}} When not null, `std::string(row.name, strnlen(row.name, kNameMax))` copies exactly the occupied bytes of the name field. {{annotate: name-string "Safe length bound"}} `strnlen` stops at the first null terminator or at `kNameMax` — whichever comes first. {{annotate: name-string "No terminator assumed"}} The char array is not required to be null-terminated, so this is the correct read strategy.

{{zoom: 1x}} {{focus: null-branch}} {{zoom: 1.2x}} {{annotate: null-branch "Null case skips copy"}} When `name_null` is true, the string is left as a default-constructed empty string. {{annotate: null-branch "Zero cost"}} No copy, no allocation. {{annotate: null-branch "Flag is authority"}} The `name_null` flag is the canonical indicator.

{{zoom: 1x}} {{focus: push-result}} {{zoom: 1.2x}} {{annotate: push-result "Aggregate fields"}} Each `ResultRow` is aggregate-initialized and moved into `result`. {{annotate: push-result "Move not copy"}} The move avoids an extra string copy for the common non-null case.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:scan-loop lang=cpp
DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)stmt;

  std::vector<ResultRow> result;
  result.reserve(rows.size());

  for (const Row& row : rows) {
    bool name_null = (row.null_mask & 1u) != 0;
    std::string name;
    if (!name_null) {
      name = std::string(row.name, strnlen(row.name, kNameMax));
    }
    result.push_back(ResultRow{row.id, name_null, std::move(name)});
  }

  return result;
}
---
reserve-call: 7
null-decode: 10
name-string: 13
null-branch: 12
push-result: 15
```

# Error handling

{{clear: slide}}

{{show: error-table typewriter 2s linear}} The executor has a narrow error surface.

{{focus: not-found-row}} {{annotate: not-found-row "Table not found"}} In a full multi-table database, `kNotFound` is returned when the requested table does not exist. {{annotate: not-found-row "Reserved for schema"}} In this lab the table check is omitted — but the error code is reserved for that purpose.

{{focus: corrupted-row}} {{annotate: corrupted-row "Row decode failure"}} `kCorrupted` signals a row that cannot be decoded — malformed null_mask values or truncated name data. {{annotate: corrupted-row "Caught earlier"}} In `execute_select`, the input is already a decoded `vector<Row>`, so corruption is caught by the codec layer before we are called. {{annotate: corrupted-row "Propagates from below"}} The error can propagate from lower layers if the pipeline is extended.

{{focus: oom-row}} {{annotate: oom-row "Allocation failure"}} `kOutOfMemory` would be returned if `result.reserve` or `std::string` construction throws `std::bad_alloc`. {{annotate: oom-row "Embedded targets"}} In practice, most modern systems overcommit memory and throw rarely — but the error code exists for embedded targets where this matters.

{{focus: no-syntax-row}} {{annotate: no-syntax-row "Syntax never here"}} `kSyntax` is never returned by the executor. {{annotate: no-syntax-row "Parser guarantees"}} By the time a `SelectStmt` reaches `execute_select`, the parser has already validated every token. {{annotate: no-syntax-row "Pre-validated input"}} The executor's contract is that its input is structurally valid.

{{focus: execute-signature}} {{annotate: execute-signature "DbResult wraps all"}} The return type `DbResult<vector<ResultRow>>` is `std::expected<vector<ResultRow>, DbError>`. {{annotate: execute-signature "Uniform error type"}} Every error code above maps to the same `DbError` enum. {{annotate: execute-signature "No throw, no errno"}} The caller unwraps with `if (!result)` — no try/catch, no errno.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:error-table type=array
["kNotFound   │  table name not found in schema", "kCorrupted  │  row data malformed (caught by codec layer)", "kOutOfMemory│  allocation failure during projection", "kSyntax     │  NEVER — parser guarantees valid input"]
---
not-found-row: 0
corrupted-row: 1
oom-row: 2
no-syntax-row: 3
execute-signature: 3
```

# The full execute_select

{{clear: slide}}

{{show: execute-full typewriter 2s linear}} `execute_select` is the complete function.

{{focus: stmt-ignored}} {{zoom: 1.2x}} {{annotate: stmt-ignored "WHERE deferred"}} `stmt` is accepted for interface consistency but not consulted. {{annotate: stmt-ignored "Void suppresses warning"}} The `(void)stmt` suppresses the unused-parameter warning without removing the parameter from the signature. {{annotate: stmt-ignored "Next lab adds filter"}} The WHERE pass is added in the next lab on top of this function.

{{zoom: 1x}} {{focus: loop-body}} {{zoom: 1.2x}} {{annotate: loop-body "Three-step projection"}} Each row takes three steps: decode the null bit, conditionally copy the name string, then aggregate into `ResultRow`. {{annotate: loop-body "Null path shorter"}} No branching on the happy path — the null case takes a shorter path through the if-block.

{{zoom: 1x}} {{focus: return-result}} {{zoom: 1.2x}} {{annotate: return-result "Return by value"}} `result` is returned by value. {{annotate: return-result "NRVO"}} Named Return Value Optimization (NRVO) eliminates the copy in all major compilers. {{annotate: return-result "Zero overhead"}} The caller receives the vector directly without a move overhead.

{{zoom: 1x}} {{focus: full-signature}} {{zoom: 1.2x}} {{annotate: full-signature "DbResult wraps output"}} The function always succeeds in this lab — the input rows are already decoded. {{annotate: full-signature "Future-proof"}} The `DbResult` wrapper exists because future callers may supply undecoded data, or the schema check may fail. {{annotate: full-signature "Stable API"}} The interface is stable across that change.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:execute-full lang=cpp
DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)stmt;  // WHERE filtering is added in the next lab

  std::vector<ResultRow> result;
  result.reserve(rows.size());

  for (const Row& row : rows) {
    bool name_null = (row.null_mask & 1u) != 0;
    std::string name;
    if (!name_null) {
      name = std::string(row.name, strnlen(row.name, kNameMax));
    }
    result.push_back(ResultRow{row.id, name_null, std::move(name)});
  }

  return result;
}
---
full-signature: 1
stmt-ignored: 4
loop-body: 9
return-result: 18
```
