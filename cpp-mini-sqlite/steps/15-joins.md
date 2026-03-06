---
title: Joins
---

# What is a join

{{clear: slide}}

{{show: join-diagram typewriter 2s linear}} A join combines rows from two tables where a column in one table matches a column in the other.

{{focus: users-table}} {{annotate: users-table "Left table: users"}} The `users` table has columns `id` and `name`. Each row represents one user. {{focus: orders-table}} {{annotate: orders-table "Right table: orders"}} The `orders` table also has an `id` column. In this simplified model, both tables use the same `id` type — `ResultRow` — so the join matches on the shared `id` field. {{focus: join-condition}} {{annotate: join-condition "Join condition"}} `users.id = orders.id` is the join condition. For every pair of rows where this condition holds, the join produces one output row containing fields from both input rows.

{{focus: result-table}} {{annotate: result-table "Joined output"}} The result contains one row for each matching pair. User 1 with name "alice" matches both orders where `user_id = 1`, producing two output rows. User 2 matches one order. User 3 matches no orders and is absent from the output — this is an inner join. {{annotate: result-table "Inner join semantics"}} An inner join discards rows with no match in the other table. An outer join would retain them with null columns.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```diagram:join-diagram
[users-table] -> [join-condition]
[orders-table] -> [join-condition]
[join-condition] -> [result-table]
---
users-table: users-table
orders-table: orders-table
join-condition: join-condition
result-table: result-table
```

# Nested loop join

{{clear: slide}}

{{show: nested-loop typewriter 2s linear}} The nested loop join is the simplest correct join algorithm.

{{focus: outer-loop}} {{zoom: 1.2x}} {{annotate: outer-loop "Outer loop: left table"}} For every row in the left table — the `users` table — the outer loop iterates once. {{zoom: 1x}} {{focus: inner-loop}} {{zoom: 1.2x}} {{annotate: inner-loop "Inner loop: right table"}} For each left row, the inner loop scans every row in the right table — the `orders` table. {{annotate: inner-loop "O(n times m)"}} With N left rows and M right rows, the total comparisons are N times M. For 1000 users and 10000 orders, that is 10 million comparisons.

{{zoom: 1x}} {{focus: join-test}} {{zoom: 1.2x}} {{annotate: join-test "Predicate test"}} The join predicate `left.id == right.id` is evaluated once per pair. {{annotate: join-test "Inner join: emit on match"}} When the predicate is true, the pair is emitted as a result row. {{zoom: 1x}} {{focus: result-push}} {{zoom: 1.2x}} {{annotate: result-push "Composite result row"}} The result row contains fields from both the left and right row. In a full implementation this is a separate projected type with all selected columns.

{{zoom: 1x}} {{focus: complexity-note}} {{zoom: 1.2x}} {{annotate: complexity-note "Correct for small tables"}} The nested loop join is always correct. For tables with a few hundred rows it is fast enough. For large tables, use a hash join.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:nested-loop lang=cpp
struct JoinedRow {
  uint32_t    user_id;
  std::string user_name;
  uint32_t    order_id;
};

std::vector<JoinedRow> nested_loop_join(
    const std::vector<ResultRow>& users,
    const std::vector<ResultRow>& orders) {
  std::vector<JoinedRow> result;

  for (const ResultRow& u : users) {     // outer: every user
    for (const ResultRow& o : orders) {  // inner: every order
      if (u.id == o.id) {                // join predicate
        result.push_back(JoinedRow{u.id, u.name, o.id});
      }
    }
  }

  return result;
}
---
outer-loop: 12
inner-loop: 13
join-test: 14
result-push: 15
complexity-note: 7
```

# Hash join

{{clear: slide}}

{{show: hash-join typewriter 2s linear}} The hash join reduces the nested loop's order N times M cost to order N plus M.

{{focus: build-phase}} {{zoom: 1.2x}} {{annotate: build-phase "Build phase"}} The build phase scans the inner table — typically the smaller one — and inserts every row into a hash map keyed by the join column. {{annotate: build-phase "One pass over inner table"}} Building the hash map is O of M where M is the inner table size. {{zoom: 1x}} {{focus: probe-phase}} {{zoom: 1.2x}} {{annotate: probe-phase "Probe phase"}} The probe phase scans the outer table. For each outer row, it looks up the join column in the hash map. A lookup is O of 1 average. The probe phase is O of N where N is the outer table size.

{{zoom: 1x}} {{focus: hash-map-decl}} {{zoom: 1.2x}} {{annotate: hash-map-decl "unordered_map by join key"}} `std::unordered_map<uint32_t, std::vector<ResultRow>>` maps each `user_id` value to all orders with that value. {{annotate: hash-map-decl "Duplicate row support"}} The value is a vector because multiple orders can share the same `user_id`.

{{zoom: 1x}} {{focus: emit-matches}} {{zoom: 1.2x}} {{annotate: emit-matches "All matching right rows"}} For each left row, look up its `id` in the map. All right rows in the bucket are matches. Emit one result row per match.

{{zoom: 1x}} {{focus: total-complexity}} {{zoom: 1.2x}} {{annotate: total-complexity "O(n + m) total"}} Build: O of M. Probe: O of N. Total: O of N plus M. For 1000 users and 10000 orders, that is 11000 operations instead of 10 million. {{annotate: total-complexity "Memory tradeoff"}} The hash map uses O of M memory. For very large inner tables, this can exhaust memory — the nested loop join uses O of 1 extra memory.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:hash-join lang=cpp
std::vector<JoinedRow> hash_join(
    const std::vector<ResultRow>& users,
    const std::vector<ResultRow>& orders) {
  // Build phase: index orders by user_id.
  std::unordered_map<uint32_t, std::vector<const ResultRow*>> index;
  for (const ResultRow& o : orders) {
    index[o.id].push_back(&o);
  }

  // Probe phase: look up each user in the index.
  std::vector<JoinedRow> result;
  for (const ResultRow& u : users) {
    auto it = index.find(u.id);
    if (it == index.end()) continue;  // no matching orders

    for (const ResultRow* o : it->second) {
      result.push_back(JoinedRow{u.id, u.name, o->id});
    }
  }

  return result;
}
---
build-phase: 4
hash-map-decl: 5
probe-phase: 12
emit-matches: 17
total-complexity: 1
```

# Integrating with our executor

{{clear: slide}}

{{show: join-stmt typewriter 2s linear}} Adding join support to the executor requires extending the AST and the execution dispatch.

{{focus: join-stmt-struct}} {{zoom: 1.2x}} {{annotate: join-stmt-struct "JoinStmt in the AST"}} `JoinStmt` carries the two table names and the two column names that form the join condition. {{annotate: join-stmt-struct "Structured condition"}} The join condition is not a raw string — it is a structured pair of table-column references. {{zoom: 1x}} {{focus: stmt-variant}} {{zoom: 1.2x}} {{annotate: stmt-variant "Stmt variant extended"}} `JoinStmt` is added as a fourth alternative in the `Stmt` variant. The parser produces it when it sees `SELECT ... FROM a JOIN b ON a.col = b.col`. {{annotate: stmt-variant "Closed enum"}} The variant is closed: every possible statement kind is known at compile time.

{{zoom: 1x}} {{focus: run-query-dispatch}} {{zoom: 1.2x}} {{annotate: run-query-dispatch "Visitor dispatch"}} `run_query` uses `std::visit` to dispatch on the active alternative. The `JoinStmt` arm calls `execute_join`, which performs the hash join. {{annotate: run-query-dispatch "Zero overhead abstraction"}} `std::visit` compiles to a jump table — no virtual dispatch, no heap allocation.

{{zoom: 1x}} {{focus: execute-join-stub}} {{zoom: 1.2x}} {{annotate: execute-join-stub "execute_join body"}} `execute_join` projects both tables with `execute_select`, passes them to `hash_join`, and converts each `JoinedRow` back to a `ResultRow`. {{annotate: execute-join-stub "Uniform interface"}} The return type is identical to `execute_select` — the caller cannot tell whether the result came from a scan or a join.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:join-stmt lang=cpp
// Extended AST with join support.
struct JoinStmt {
  std::string_view left_table;
  std::string_view right_table;
  std::string_view left_col;   // join condition: left_col = right_col
  std::string_view right_col;
};

// Stmt now covers four statement kinds.
using Stmt = std::variant<CreateStmt, InsertStmt, SelectStmt, JoinStmt>;

// execute_join: project both tables, hash-join, convert to ResultRow.
DbResult<std::vector<ResultRow>> execute_join(
    const std::vector<Row>& left_rows,
    const std::vector<Row>& right_rows,
    const JoinStmt& stmt) {
  auto left  = execute_select(left_rows,  SelectStmt{stmt.left_table});
  if (!left)  return std::unexpected(left.error());
  auto right = execute_select(right_rows, SelectStmt{stmt.right_table});
  if (!right) return std::unexpected(right.error());

  auto joined = hash_join(*left, *right);

  std::vector<ResultRow> result;
  result.reserve(joined.size());
  for (const JoinedRow& jr : joined)
    result.push_back(ResultRow{jr.user_id, false, jr.user_name});
  return result;
}

// run_query: tokenize, parse, dispatch via std::visit.
DbResult<std::vector<ResultRow>> run_query(
    std::string_view sql,
    const std::vector<Row>& left_rows,
    const std::vector<Row>& right_rows) {
  auto toks = tokenize(sql);
  if (!toks) return std::unexpected(toks.error());
  auto stmt = Parser(*toks).parse();
  if (!stmt) return std::unexpected(stmt.error());

  return std::visit([&](auto& s) -> DbResult<std::vector<ResultRow>> {
    using T = std::decay_t<decltype(s)>;
    if constexpr (std::is_same_v<T, SelectStmt>)
      return execute_select_where(left_rows, s);
    else if constexpr (std::is_same_v<T, JoinStmt>)
      return execute_join(left_rows, right_rows, s);
    else
      return std::unexpected(DbError::kSyntax);
  }, *stmt);
}
---
join-stmt-struct: 2
stmt-variant: 10
execute-join-stub: 13
run-query-dispatch: 41
```
