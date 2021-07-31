#pragma once

#include "logger.h"

namespace refl {
class SymbolTable;

class Parser {
 public:
  Parser(SymbolTable*);

  std::unique_ptr<cppast::cpp_file> TryParseFile(const std::string &file_name);
 
  void Traverse(cppast::cpp_file&);
 private:
  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
  SymbolTable* sym_tab_; //< non owning.
};
}