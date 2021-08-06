// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "logger.h"

namespace refl {
class RlDatabase;
class ClangCompileDatabase;

class Parser {
 public:
  using file_collection_t = std::vector<std::unique_ptr<cppast::cpp_file>>;

  Parser();

  bool TryParse(const std::vector<std::string>& file_list,
                type_safe::optional<cppast::libclang_compilation_database> db);
  
  void TraverseFiles(RlDatabase&);

 private:
  void InitializeConfig(cppast::libclang_compile_config&);
  void DoTraverse(RlDatabase&, cppast::cpp_file&);

 private:
  file_collection_t parsed_files_;
  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
};
}  // namespace refl