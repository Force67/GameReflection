#pragma once

#include "logger.h"

namespace refl {

class Parser {
 public:
  Parser();

  std::unique_ptr<cppast::cpp_file> TryParseFile(const std::string &file_name);
 
 private:
  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
};
}