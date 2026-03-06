#include <iostream>
#include <string>
#include "solution.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "usage: mini-sqlite <db-file>\n";
    return 1;
  }

  auto db = Database::open(argv[1]);
  if (!db) {
    std::cerr << "cannot open database: " << argv[1] << "\n";
    return 1;
  }

  std::string line;
  while (std::cout << "> ", std::getline(std::cin, line)) {
    if (line.empty()) continue;

    auto result = db->query(line);
    if (!result) {
      switch (result.error()) {
        case DbError::kSyntax:    std::cerr << "error: syntax\n";    break;
        case DbError::kIo:        std::cerr << "error: I/O\n";       break;
        case DbError::kCorrupted: std::cerr << "error: corrupted\n"; break;
        case DbError::kFull:      std::cerr << "error: table full\n"; break;
        default:                  std::cerr << "error\n";             break;
      }
      continue;
    }

    if (result->empty()) {
      std::cout << "(0 rows)\n";
      continue;
    }

    std::cout << "id  | name\n";
    std::cout << "----+--------------------\n";
    for (const auto& row : *result) {
      std::cout << row.id << "   | ";
      if (row.name_null) std::cout << "NULL";
      else               std::cout << row.name;
      std::cout << "\n";
    }
    const auto n = result->size();
    std::cout << "(" << n << " row" << (n == 1 ? "" : "s") << ")\n";
  }

  return 0;
}
