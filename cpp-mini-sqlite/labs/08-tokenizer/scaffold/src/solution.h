#pragma once
#include <expected>
#include <string_view>
#include <vector>

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

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

// Tokenizes a SQL string into a flat vector of tokens ending with kEof.
// Returns kSyntax on unterminated string literal or unrecognized character.
DbResult<std::vector<Token>> tokenize(std::string_view sql);
