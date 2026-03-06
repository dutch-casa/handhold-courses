---
title: WHERE Predicate Evaluation
---

# Predicates as filters

{{clear: slide}}

{{show: predicate-table typewriter 2s linear}} A WHERE clause is a predicate — a function from a row to a boolean.

{{focus: eq-row}} {{annotate: eq-row "Equality"}} `id = 5` maps to the `kEq` operator. {{annotate: eq-row "uint32_t comparison"}} The executor compares `row.id == 5`. {{focus: comparison-rows}} {{annotate: comparison-rows "Ordered comparisons"}} `kLt`, `kGt`, `kLe`, `kGe` map directly to `<`, `>`, `<=`, `>=` on either `uint32_t` or `std::string`. {{focus: ne-row}} {{annotate: ne-row "Inequality"}} `kNe` maps to `!=`. {{focus: is-null-row}} {{annotate: is-null-row "Null check"}} `IS NULL` and `IS NOT NULL` are a separate operator class — they do not compare a value; they check the null flag.

{{focus: predicate-row}} {{annotate: predicate-row "Single struct"}} All six comparison operators plus `IS` are encoded in one `Predicate` struct. {{annotate: predicate-row "TokenKind reused"}} The `op` field is a `TokenKind`, the same enum used by the tokenizer. {{annotate: predicate-row "No parallel enum"}} No separate operator enum is needed.

{{focus: negate-field}} {{annotate: negate-field "Negate flag"}} The `negate` field handles `IS NOT NULL` without introducing a `kIsNot` operator. {{focus: is-null-row}} When `op == kIs` and `negate == true`, the row must not be null. {{annotate: is-null-row "negate=false: IS NULL"}} When `negate == false`, the row must be null.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:predicate-table type=array
["kEq   │  col = val      │  row.id == rhs  or  row.name == rhs", "kLt   │  col < val      │  row.id <  rhs  or  row.name <  rhs", "kGt   │  col > val      │  row.id >  rhs  or  row.name >  rhs", "kLe   │  col <= val     │  row.id <= rhs  or  row.name <= rhs", "kGe   │  col >= val     │  row.id >= rhs  or  row.name >= rhs", "kNe   │  col != val     │  row.id != rhs  or  row.name != rhs", "kIs   │  IS [NOT] NULL  │  row.name_null == !pred.negate"]
---
eq-row: 0
comparison-rows: 1
ne-row: 5
is-null-row: 6
predicate-row: 0
negate-field: 6
```

# Column dispatch

{{clear: slide}}

{{show: eval-pred typewriter 2s linear}} `eval_predicate` dispatches on the column name first, then on the operator.

{{focus: id-branch}} {{zoom: 1.2x}} {{annotate: id-branch "id column"}} If `pred.col == "id"`, the comparison is numeric: `row.id` (a `uint32_t`) against `pred.rhs.integer`. {{annotate: id-branch "All six operators"}} The switch covers all six operators. {{annotate: id-branch "Unknown op is false"}} The `default` branch returns `false` — a conservative unknown-operator policy.

{{zoom: 1x}} {{focus: name-branch}} {{zoom: 1.2x}} {{annotate: name-branch "name column"}} If `pred.col == "name"`, the comparison is either a null check or a string comparison. {{annotate: name-branch "kIs before switch"}} The `kIs` case is handled before the switch so it does not participate in string ordering — IS NULL is not a string comparison at all.

{{zoom: 1x}} {{focus: string-switch}} {{zoom: 1.2x}} {{annotate: string-switch "String ordering"}} For non-IS operators on `name`, `row.name` is a `std::string` and `pred.rhs.text` is a `std::string`. {{annotate: string-switch "Lexicographic"}} All six comparison operators are valid on `std::string` — lexicographic ordering is well-defined.

{{zoom: 1x}} {{focus: unknown-column}} {{zoom: 1.2x}} {{annotate: unknown-column "Unknown column returns false"}} If the column is neither `"id"` nor `"name"`, the predicate returns false. {{annotate: unknown-column "No error, conservative"}} The executor does not report an error for unknown columns in this design — it treats the predicate as unsatisfiable. {{annotate: unknown-column "Zero rows returned"}} This is conservative: no rows are returned for a column that does not exist.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:eval-pred lang=cpp
bool eval_predicate(const ResultRow& row, const Predicate& pred) {
  if (pred.col == "id") {
    uint32_t rhs = pred.rhs.integer;
    switch (pred.op) {
      case TokenKind::kEq: return row.id == rhs;
      case TokenKind::kLt: return row.id <  rhs;
      case TokenKind::kGt: return row.id >  rhs;
      case TokenKind::kLe: return row.id <= rhs;
      case TokenKind::kGe: return row.id >= rhs;
      case TokenKind::kNe: return row.id != rhs;
      default:             return false;
    }
  }

  if (pred.col == "name") {
    if (pred.op == TokenKind::kIs) {
      return row.name_null == !pred.negate;
    }
    const std::string& rhs = pred.rhs.text;
    switch (pred.op) {
      case TokenKind::kEq: return row.name == rhs;
      case TokenKind::kLt: return row.name <  rhs;
      case TokenKind::kGt: return row.name >  rhs;
      case TokenKind::kLe: return row.name <= rhs;
      case TokenKind::kGe: return row.name >= rhs;
      case TokenKind::kNe: return row.name != rhs;
      default:             return false;
    }
  }

  return false;
}
---
id-branch: 2
name-branch: 15
string-switch: 19
unknown-column: 31
```

# The null check

{{clear: slide}}

{{show: null-check typewriter 2s linear}} `IS NULL` and `IS NOT NULL` share one operator and one boolean flag.

{{focus: is-op}} {{annotate: is-op "kIs operator"}} When the parser sees `col IS NULL`, it produces `Predicate{col, kIs, false, Literal{is_null=true}}`. {{annotate: is-op "NOT sets negate"}} When it sees `col IS NOT NULL`, it sets `negate = true`. {{annotate: is-op "rhs unused here"}} The `rhs` Literal carries `is_null = true` as a marker but is not used by the executor — only `negate` matters.

{{focus: negate-false}} {{annotate: negate-false "IS NULL path"}} `negate == false` means the predicate is `IS NULL`. {{annotate: negate-false "Simplifies to name_null"}} The expression `row.name_null == !pred.negate` simplifies to `row.name_null == true`, which is `row.name_null`. {{annotate: negate-false "Null rows pass"}} The row passes the filter only if its name is null.

{{focus: negate-true}} {{annotate: negate-true "IS NOT NULL path"}} `negate == true` means the predicate is `IS NOT NULL`. {{annotate: negate-true "Simplifies to !name_null"}} The expression `row.name_null == !pred.negate` simplifies to `row.name_null == false`. {{annotate: negate-true "Non-null rows pass"}} The row passes the filter only if its name is not null.

{{focus: single-expression}} {{annotate: single-expression "One expression covers both"}} `row.name_null == !pred.negate` handles both cases without branching. {{annotate: single-expression "Symmetric cases"}} The two cases are symmetric: one checks `name_null == true`, the other `name_null == false`. {{annotate: single-expression "No branch needed"}} Encoding both in one expression eliminates a branch and makes the intent clear.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:null-check lang=cpp
// Inside eval_predicate, name column branch:
if (pred.op == TokenKind::kIs) {
  // negate=false → IS NULL     → row.name_null must be true
  // negate=true  → IS NOT NULL → row.name_null must be false
  return row.name_null == !pred.negate;
}
---
is-op: 1
negate-false: 5
negate-true: 5
single-expression: 5
```

# Composing with execute_select

{{clear: slide}}

{{show: execute-where typewriter 2s linear}} `execute_select_where` composes with `execute_select`.

{{focus: base-call}} {{zoom: 1.2x}} {{annotate: base-call "Call the base scan"}} The first action is calling `execute_select` to get all rows as `ResultRow` objects. {{annotate: base-call "Error propagates"}} If that call fails — impossible in this lab but possible in future layers — the error propagates immediately.

{{zoom: 1x}} {{focus: no-where-early}} {{zoom: 1.2x}} {{annotate: no-where-early "No filter, early return"}} If `stmt.where` is absent, the full scan result is returned unchanged. {{annotate: no-where-early "Zero extra allocation"}} No allocation for the filtered vector, no iteration. {{annotate: no-where-early "optional makes it trivial"}} `std::optional` makes this check trivial.

{{zoom: 1x}} {{focus: filter-loop}} {{zoom: 1.2x}} {{annotate: filter-loop "Single-pass filter"}} The filter is a single pass over the projected rows. {{annotate: filter-loop "Once per row"}} `eval_predicate` is called once per row. {{annotate: filter-loop "Move into filtered"}} Rows that satisfy the predicate are moved into `filtered`. {{annotate: filter-loop "Avoids string copy"}} The move avoids copying the `std::string name` field.

{{zoom: 1x}} {{focus: return-filtered}} {{zoom: 1.2x}} {{annotate: return-filtered "Return filtered vector"}} `filtered` is returned by value. {{annotate: return-filtered "NRVO"}} NRVO applies. {{annotate: return-filtered "Internals hidden"}} The caller cannot observe that the rows were initially collected into `all` and then filtered — the filtering is an implementation detail hidden behind the `DbResult` return.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:execute-where lang=cpp
DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  auto all = execute_select(rows, stmt);
  if (!all) return std::unexpected(all.error());

  if (!stmt.where) return all;

  const Predicate& pred = *stmt.where;
  std::vector<ResultRow> filtered;
  for (ResultRow& row : *all) {
    if (eval_predicate(row, pred)) {
      filtered.push_back(std::move(row));
    }
  }
  return filtered;
}
---
base-call: 4
no-where-early: 7
filter-loop: 11
return-filtered: 16
```
