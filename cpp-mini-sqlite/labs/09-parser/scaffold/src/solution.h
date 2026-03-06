#pragma once
#include <cassert>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

enum class TokenKind {
  kSelect, kInsert, kInto, kCreate, kTable, kFrom, kWhere, kValues, kNull, kNot, kIs, kAnd, kOr,
  kIdent, kInteger, kString,
  kLparen, kRparen, kComma, kStar, kSemicolon,
  kEq, kLt, kGt, kLe, kGe, kNe,
  kEof,
};

struct Token {
  TokenKind        kind;
  std::string_view value;
};

struct Literal {
  bool        is_null  = false;
  uint32_t    integer  = 0;
  std::string text;
};

struct Predicate {
  std::string_view col;
  TokenKind        op      = TokenKind::kEq;
  bool             negate  = false;
  Literal          rhs;
};

struct CreateStmt {
  std::string_view              table;
  std::vector<std::string_view> cols;
};

struct InsertStmt {
  std::string_view       table;
  std::vector<Literal>   values;
};

struct SelectStmt {
  std::string_view         table;
  std::optional<Predicate> where;
};

using Stmt = std::variant<CreateStmt, InsertStmt, SelectStmt>;

class Parser {
 public:
  explicit Parser(std::span<const Token> tokens) : tokens_(tokens), pos_(0) {}

  DbResult<Stmt> parse();

 private:
  std::span<const Token> tokens_;
  size_t                 pos_;

  const Token& peek() const { return tokens_[pos_]; }
  const Token& advance() { assert(pos_ < tokens_.size()); return tokens_[pos_++]; }

  DbResult<Token> expect(TokenKind kind);

  DbResult<Literal>     parse_literal();
  DbResult<Predicate>   parse_predicate();
  DbResult<CreateStmt>  parse_create();
  DbResult<InsertStmt>  parse_insert();
  DbResult<SelectStmt>  parse_select();
};
