// helpers.cpp — pre-implemented components from earlier labs. Do not modify.
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include "solution.h"

// ---------------------------------------------------------------------------
// Tokenizer (Lab 8)
// ---------------------------------------------------------------------------

DbResult<std::vector<Token>> tokenize(std::string_view sql) {
  std::vector<Token> tokens;
  size_t pos = 0;

  while (pos < sql.size()) {
    char c = sql[pos];

    if (std::isspace(static_cast<unsigned char>(c))) { ++pos; continue; }

    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
      size_t start = pos;
      while (pos < sql.size() &&
             (std::isalnum(static_cast<unsigned char>(sql[pos])) || sql[pos] == '_')) {
        ++pos;
      }
      std::string_view word = sql.substr(start, pos - start);
      std::string lower(word.size(), '\0');
      std::transform(word.begin(), word.end(), lower.begin(),
                     [](unsigned char ch) { return std::tolower(ch); });
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
      continue;
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
      size_t start = pos;
      while (pos < sql.size() && std::isdigit(static_cast<unsigned char>(sql[pos]))) ++pos;
      tokens.push_back({TokenKind::kInteger, sql.substr(start, pos - start)});
      continue;
    }

    if (c == '\'') {
      size_t start = pos++;
      while (pos < sql.size() && sql[pos] != '\'') ++pos;
      if (pos == sql.size()) return std::unexpected(DbError::kSyntax);
      ++pos;
      tokens.push_back({TokenKind::kString, sql.substr(start, pos - start)});
      continue;
    }

    if (c == '(') { tokens.push_back({TokenKind::kLparen,    sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ')') { tokens.push_back({TokenKind::kRparen,    sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ',') { tokens.push_back({TokenKind::kComma,     sql.substr(pos, 1)}); ++pos; continue; }
    if (c == '*') { tokens.push_back({TokenKind::kStar,      sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ';') { tokens.push_back({TokenKind::kSemicolon, sql.substr(pos, 1)}); ++pos; continue; }
    if (c == '=') { tokens.push_back({TokenKind::kEq,        sql.substr(pos, 1)}); ++pos; continue; }

    if (c == '<') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kLe, sql.substr(pos, 2)}); pos += 2;
      } else {
        tokens.push_back({TokenKind::kLt, sql.substr(pos, 1)}); ++pos;
      }
      continue;
    }
    if (c == '>') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kGe, sql.substr(pos, 2)}); pos += 2;
      } else {
        tokens.push_back({TokenKind::kGt, sql.substr(pos, 1)}); ++pos;
      }
      continue;
    }
    if (c == '!') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kNe, sql.substr(pos, 2)}); pos += 2;
        continue;
      }
      return std::unexpected(DbError::kSyntax);
    }

    return std::unexpected(DbError::kSyntax);
  }

  tokens.push_back({TokenKind::kEof, sql.substr(pos, 0)});
  return tokens;
}

// ---------------------------------------------------------------------------
// Parser (Lab 9)
// ---------------------------------------------------------------------------

DbResult<Token> Parser::expect(TokenKind kind) {
  Token t = advance();
  if (t.kind != kind) return std::unexpected(DbError::kSyntax);
  return t;
}

DbResult<Literal> Parser::parse_literal() {
  const Token& t = peek();
  if (t.kind == TokenKind::kNull) {
    advance();
    return Literal{.is_null = true};
  }
  if (t.kind == TokenKind::kInteger) {
    advance();
    unsigned long raw = std::stoul(std::string(t.value));
    if (raw > std::numeric_limits<uint32_t>::max()) return std::unexpected(DbError::kSyntax);
    return Literal{.integer = static_cast<uint32_t>(raw)};
  }
  if (t.kind == TokenKind::kString) {
    advance();
    std::string_view stripped = t.value.substr(1, t.value.size() - 2);
    return Literal{.text = std::string(stripped)};
  }
  return std::unexpected(DbError::kSyntax);
}

DbResult<Predicate> Parser::parse_predicate() {
  auto col_tok = expect(TokenKind::kIdent);
  if (!col_tok) return std::unexpected(col_tok.error());
  std::string_view col = col_tok->value;

  const Token& op_tok = advance();
  TokenKind op = op_tok.kind;
  switch (op) {
    case TokenKind::kEq:
    case TokenKind::kLt:
    case TokenKind::kGt:
    case TokenKind::kLe:
    case TokenKind::kGe:
    case TokenKind::kNe:
      break;
    case TokenKind::kIs: {
      bool negate = false;
      if (peek().kind == TokenKind::kNot) { advance(); negate = true; }
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

DbResult<CreateStmt> Parser::parse_create() {
  auto tbl_kw = expect(TokenKind::kTable);
  if (!tbl_kw) return std::unexpected(tbl_kw.error());
  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());
  auto lp = expect(TokenKind::kLparen);
  if (!lp) return std::unexpected(lp.error());
  std::vector<std::string_view> cols;
  while (true) {
    auto col = expect(TokenKind::kIdent);
    if (!col) return std::unexpected(col.error());
    cols.push_back(col->value);
    if (peek().kind == TokenKind::kComma) { advance(); continue; }
    break;
  }
  auto rp = expect(TokenKind::kRparen);
  if (!rp) return std::unexpected(rp.error());
  return CreateStmt{name_tok->value, std::move(cols)};
}

DbResult<InsertStmt> Parser::parse_insert() {
  auto into = expect(TokenKind::kInto);
  if (!into) return std::unexpected(into.error());
  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());
  auto vals_kw = expect(TokenKind::kValues);
  if (!vals_kw) return std::unexpected(vals_kw.error());
  auto lp = expect(TokenKind::kLparen);
  if (!lp) return std::unexpected(lp.error());
  std::vector<Literal> values;
  while (true) {
    auto lit = parse_literal();
    if (!lit) return std::unexpected(lit.error());
    values.push_back(std::move(*lit));
    if (peek().kind == TokenKind::kComma) { advance(); continue; }
    break;
  }
  auto rp = expect(TokenKind::kRparen);
  if (!rp) return std::unexpected(rp.error());
  return InsertStmt{name_tok->value, std::move(values)};
}

DbResult<SelectStmt> Parser::parse_select() {
  auto star = expect(TokenKind::kStar);
  if (!star) return std::unexpected(star.error());
  auto from = expect(TokenKind::kFrom);
  if (!from) return std::unexpected(from.error());
  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());
  SelectStmt stmt{.table = name_tok->value};
  if (peek().kind == TokenKind::kWhere) {
    advance();
    auto pred = parse_predicate();
    if (!pred) return std::unexpected(pred.error());
    stmt.where = std::move(*pred);
  }
  return stmt;
}

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

// ---------------------------------------------------------------------------
// Row codec (Lab 7)
// ---------------------------------------------------------------------------

void encode_row(const Row& row, std::span<uint8_t> out) {
  assert(out.size() >= kRowSize);
  std::memset(out.data(), 0, kRowSize);
  std::memcpy(out.data() + kOffId,       &row.id,        sizeof(row.id));
  std::memcpy(out.data() + kOffNullMask, &row.null_mask, sizeof(row.null_mask));
  std::memcpy(out.data() + kOffName,      row.name,       kNameMax);
}

DbResult<Row> decode_row(std::span<const uint8_t> in) {
  if (in.size() < kRowSize) return std::unexpected(DbError::kCorrupted);
  Row row{};
  std::memcpy(&row.id,        in.data() + kOffId,       sizeof(row.id));
  std::memcpy(&row.null_mask, in.data() + kOffNullMask, sizeof(row.null_mask));
  std::memcpy(row.name,       in.data() + kOffName,     kNameMax);
  return row;
}

// ---------------------------------------------------------------------------
// Executor (Labs 10–11)
// ---------------------------------------------------------------------------

DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)stmt;
  std::vector<ResultRow> result;
  result.reserve(rows.size());
  for (const Row& row : rows) {
    bool name_null = (row.null_mask & 1u) != 0;
    std::string name;
    if (!name_null) name = std::string(row.name, strnlen(row.name, kNameMax));
    result.push_back(ResultRow{row.id, name_null, std::move(name)});
  }
  return result;
}

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
    if (pred.op == TokenKind::kIs) return row.name_null == !pred.negate;
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

DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  auto all = execute_select(rows, stmt);
  if (!all) return std::unexpected(all.error());
  if (!stmt.where) return all;
  const Predicate& pred = *stmt.where;
  std::vector<ResultRow> filtered;
  for (ResultRow& row : *all) {
    if (eval_predicate(row, pred)) filtered.push_back(std::move(row));
  }
  return filtered;
}

// ---------------------------------------------------------------------------
// RowFile — trivial methods (student implements open, insert, all)
// ---------------------------------------------------------------------------

RowFile::RowFile(FILE* file) : file_(file) {}

RowFile::~RowFile() {
  if (file_) std::fclose(file_);
}

RowFile::RowFile(RowFile&& other) noexcept : file_(other.file_) {
  other.file_ = nullptr;
}

RowFile& RowFile::operator=(RowFile&& other) noexcept {
  if (this != &other) {
    if (file_) std::fclose(file_);
    file_ = other.file_;
    other.file_ = nullptr;
  }
  return *this;
}

// ---------------------------------------------------------------------------
// Database — trivial constructor (student implements open and query)
// ---------------------------------------------------------------------------

Database::Database(RowFile row_file) : row_file_(std::move(row_file)) {}
