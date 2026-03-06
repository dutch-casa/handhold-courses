#pragma once
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

// ---------------------------------------------------------------------------
// Core error type
// ---------------------------------------------------------------------------

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

// ---------------------------------------------------------------------------
// Tokenizer
// ---------------------------------------------------------------------------

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

DbResult<std::vector<Token>> tokenize(std::string_view sql);

// ---------------------------------------------------------------------------
// AST types
// ---------------------------------------------------------------------------

struct Literal {
  bool        is_null = false;
  uint32_t    integer = 0;
  std::string text;
};

struct Predicate {
  std::string_view col;
  TokenKind        op     = TokenKind::kEq;
  bool             negate = false;
  Literal          rhs;
};

struct CreateStmt {
  std::string_view              table;
  std::vector<std::string_view> cols;
};

struct InsertStmt {
  std::string_view     table;
  std::vector<Literal> values;
};

struct SelectStmt {
  std::string_view         table;
  std::optional<Predicate> where;
};

using Stmt = std::variant<CreateStmt, InsertStmt, SelectStmt>;

// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------

class Parser {
 public:
  explicit Parser(std::span<const Token> tokens) : tokens_(tokens), pos_(0) {}

  DbResult<Stmt> parse();

 private:
  std::span<const Token> tokens_;
  size_t                 pos_;

  const Token& peek() const { return tokens_[pos_]; }
  const Token& advance() { assert(pos_ < tokens_.size()); return tokens_[pos_++]; }

  DbResult<Token>       expect(TokenKind kind);
  DbResult<Literal>     parse_literal();
  DbResult<Predicate>   parse_predicate();
  DbResult<CreateStmt>  parse_create();
  DbResult<InsertStmt>  parse_insert();
  DbResult<SelectStmt>  parse_select();
};

// ---------------------------------------------------------------------------
// Row codec
// ---------------------------------------------------------------------------

constexpr uint32_t kNameMax     = 32;
constexpr uint32_t kRowSize     = 40;   // encoded bytes per row
constexpr uint32_t kOffId       = 0;    // offset of id field (4 bytes)
constexpr uint32_t kOffNullMask = 4;    // offset of null_mask (1 byte)
constexpr uint32_t kOffName     = 8;    // offset of name field (32 bytes)

struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is SQL NULL
  char     name[kNameMax];
};

// encode_row: serializes row into out[0..kRowSize-1]. out.size() must be >= kRowSize.
void encode_row(const Row& row, std::span<uint8_t> out);

// decode_row: deserializes a Row from in[0..kRowSize-1]. Returns kCorrupted if too short.
DbResult<Row> decode_row(std::span<const uint8_t> in);

// ---------------------------------------------------------------------------
// Executor
// ---------------------------------------------------------------------------

struct ResultRow {
  uint32_t    id;
  bool        name_null;
  std::string name;  // empty when name_null is true
};

DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt);

bool eval_predicate(const ResultRow& row, const Predicate& pred);

DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt);

// ---------------------------------------------------------------------------
// RowFile: persistent row storage
// ---------------------------------------------------------------------------
//
// File layout:
//   bytes 0..3:              uint32_t row_count  (number of rows stored)
//   bytes 4 + i*kRowSize:    encoded Row i
//
// RowFile owns its FILE* and flushes after every write.

class RowFile {
 public:
  // Opens an existing file or creates a new one.
  // New files are initialized with row_count = 0.
  static DbResult<RowFile> open(const char* path);

  ~RowFile();
  RowFile(const RowFile&)            = delete;
  RowFile& operator=(const RowFile&) = delete;
  RowFile(RowFile&&) noexcept;
  RowFile& operator=(RowFile&&) noexcept;

  // Appends one row. Reads row_count, writes the encoded row, updates row_count.
  DbResult<void> insert(const Row& row);

  // Returns all rows in insertion order.
  DbResult<std::vector<Row>> all() const;

 private:
  explicit RowFile(FILE* file);
  FILE* file_;
};

// ---------------------------------------------------------------------------
// Database: integration facade
// ---------------------------------------------------------------------------
//
// Database wraps a RowFile and exposes a single query() method.
// Supports INSERT INTO users VALUES (id, name) and SELECT * FROM users [WHERE ...].

class Database {
 public:
  // Opens (or creates) the database at path.
  static DbResult<Database> open(std::string_view path);

  Database(const Database&)            = delete;
  Database& operator=(const Database&) = delete;
  Database(Database&&)                 = default;
  Database& operator=(Database&&)      = default;

  // Executes sql. INSERT returns an empty result. SELECT returns matching rows.
  DbResult<std::vector<ResultRow>> query(std::string_view sql);

 private:
  explicit Database(RowFile row_file);
  RowFile row_file_;
};
