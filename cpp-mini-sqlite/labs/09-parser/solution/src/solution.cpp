#include <cstdlib>
#include <limits>
#include <string>
#include <utility>
#include "solution.h"

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
    // Tokenizer guarantees value length >= 2 (the surrounding single quotes).
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

DbResult<CreateStmt> Parser::parse_create() {
  // kCreate already consumed by parse().
  auto tbl_kw = expect(TokenKind::kTable);
  if (!tbl_kw) return std::unexpected(tbl_kw.error());

  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());
  std::string_view table = name_tok->value;

  auto lp = expect(TokenKind::kLparen);
  if (!lp) return std::unexpected(lp.error());

  std::vector<std::string_view> cols;
  while (true) {
    auto col = expect(TokenKind::kIdent);
    if (!col) return std::unexpected(col.error());
    cols.push_back(col->value);
    if (peek().kind == TokenKind::kComma) {
      advance();
      continue;
    }
    break;
  }

  auto rp = expect(TokenKind::kRparen);
  if (!rp) return std::unexpected(rp.error());

  return CreateStmt{table, std::move(cols)};
}

DbResult<InsertStmt> Parser::parse_insert() {
  // kInsert already consumed by parse().
  auto into = expect(TokenKind::kInto);
  if (!into) return std::unexpected(into.error());

  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());
  std::string_view table = name_tok->value;

  auto vals_kw = expect(TokenKind::kValues);
  if (!vals_kw) return std::unexpected(vals_kw.error());

  auto lp = expect(TokenKind::kLparen);
  if (!lp) return std::unexpected(lp.error());

  std::vector<Literal> values;
  while (true) {
    auto lit = parse_literal();
    if (!lit) return std::unexpected(lit.error());
    values.push_back(std::move(*lit));
    if (peek().kind == TokenKind::kComma) {
      advance();
      continue;
    }
    break;
  }

  auto rp = expect(TokenKind::kRparen);
  if (!rp) return std::unexpected(rp.error());

  return InsertStmt{table, std::move(values)};
}

DbResult<SelectStmt> Parser::parse_select() {
  // kSelect already consumed by parse().
  auto star = expect(TokenKind::kStar);
  if (!star) return std::unexpected(star.error());

  auto from = expect(TokenKind::kFrom);
  if (!from) return std::unexpected(from.error());

  auto name_tok = expect(TokenKind::kIdent);
  if (!name_tok) return std::unexpected(name_tok.error());

  SelectStmt stmt{.table = name_tok->value};

  if (peek().kind == TokenKind::kWhere) {
    advance();  // consume kWhere
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
