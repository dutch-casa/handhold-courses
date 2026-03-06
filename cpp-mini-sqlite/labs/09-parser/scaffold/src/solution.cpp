#include "solution.h"

DbResult<Token> Parser::expect(TokenKind kind) {
  // TODO: see INSTRUCTIONS.md
  (void)kind;
  return std::unexpected(DbError::kSyntax);
}

DbResult<Literal> Parser::parse_literal() {
  (void)0;
  return std::unexpected(DbError::kSyntax);
}

DbResult<Predicate> Parser::parse_predicate() {
  (void)0;
  return std::unexpected(DbError::kSyntax);
}

DbResult<CreateStmt> Parser::parse_create() {
  (void)0;
  return std::unexpected(DbError::kSyntax);
}

DbResult<InsertStmt> Parser::parse_insert() {
  (void)0;
  return std::unexpected(DbError::kSyntax);
}

DbResult<SelectStmt> Parser::parse_select() {
  (void)0;
  return std::unexpected(DbError::kSyntax);
}

DbResult<Stmt> Parser::parse() {
  // TODO: see INSTRUCTIONS.md
  return std::unexpected(DbError::kSyntax);
}
