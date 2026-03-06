#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>
#include "solution.h"

DbResult<std::vector<Token>> tokenize(std::string_view sql) {
  std::vector<Token> tokens;
  size_t pos = 0;

  while (pos < sql.size()) {
    char c = sql[pos];

    // Whitespace is never a token — skip and continue.
    if (std::isspace(static_cast<unsigned char>(c))) {
      ++pos;
      continue;
    }

    // Identifier or keyword: starts with alpha or underscore.
    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
      size_t start = pos;
      while (pos < sql.size() &&
             (std::isalnum(static_cast<unsigned char>(sql[pos])) || sql[pos] == '_')) {
        ++pos;
      }
      std::string_view word = sql.substr(start, pos - start);

      // Produce a lowercase copy for case-insensitive keyword lookup.
      std::string lower(word.size(), '\0');
      std::transform(word.begin(), word.end(), lower.begin(),
                     [](unsigned char ch) { return std::tolower(ch); });

      // Static map initialized once per process.
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

    // Integer literal: one or more decimal digits.
    if (std::isdigit(static_cast<unsigned char>(c))) {
      size_t start = pos;
      while (pos < sql.size() && std::isdigit(static_cast<unsigned char>(sql[pos]))) ++pos;
      tokens.push_back({TokenKind::kInteger, sql.substr(start, pos - start)});
      continue;
    }

    // String literal: 'text' — no escape sequences supported.
    if (c == '\'') {
      size_t start = pos++;  // save start including opening quote; advance past it
      while (pos < sql.size() && sql[pos] != '\'') ++pos;
      if (pos == sql.size()) return std::unexpected(DbError::kSyntax);
      ++pos;  // advance past closing quote
      tokens.push_back({TokenKind::kString, sql.substr(start, pos - start)});
      continue;
    }

    // Single-character punctuation.
    if (c == '(') { tokens.push_back({TokenKind::kLparen,    sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ')') { tokens.push_back({TokenKind::kRparen,    sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ',') { tokens.push_back({TokenKind::kComma,     sql.substr(pos, 1)}); ++pos; continue; }
    if (c == '*') { tokens.push_back({TokenKind::kStar,      sql.substr(pos, 1)}); ++pos; continue; }
    if (c == ';') { tokens.push_back({TokenKind::kSemicolon, sql.substr(pos, 1)}); ++pos; continue; }
    if (c == '=') { tokens.push_back({TokenKind::kEq,        sql.substr(pos, 1)}); ++pos; continue; }

    // Operators that may be one or two characters.
    if (c == '<') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kLe, sql.substr(pos, 2)});
        pos += 2;
      } else {
        tokens.push_back({TokenKind::kLt, sql.substr(pos, 1)});
        ++pos;
      }
      continue;
    }
    if (c == '>') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kGe, sql.substr(pos, 2)});
        pos += 2;
      } else {
        tokens.push_back({TokenKind::kGt, sql.substr(pos, 1)});
        ++pos;
      }
      continue;
    }
    if (c == '!') {
      if (pos + 1 < sql.size() && sql[pos + 1] == '=') {
        tokens.push_back({TokenKind::kNe, sql.substr(pos, 2)});
        pos += 2;
        continue;
      }
      return std::unexpected(DbError::kSyntax);
    }

    // Unrecognized character — malformed SQL.
    return std::unexpected(DbError::kSyntax);
  }

  tokens.push_back({TokenKind::kEof, sql.substr(pos, 0)});
  return tokens;
}
