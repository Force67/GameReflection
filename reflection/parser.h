#pragma once

#include "logger.h"

namespace refl {
class SymbolTable;

class Parser {
 public:
  Parser(SymbolTable*);

  std::unique_ptr<cppast::cpp_file> TryParseFile(const std::string &file_name);

  using file_collection_t = std::vector<std::unique_ptr<cppast::cpp_file>>;
  type_safe::optional<file_collection_t> TryParseMultiple(const std::vector<std::string> &file_names);
 
  void Traverse(cppast::cpp_file&);
 private:
  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
  SymbolTable* sym_tab_; //< non owning.
};
}