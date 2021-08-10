// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "log_adapter.h"
#include "matchers/matcher_base.h"

namespace refl {
class ClangCompileDatabase;

class Parser {
 public:
  Parser();

  bool ParseWithCompilationDatabase(const std::vector<std::string>& file_list,
                cppast::libclang_compilation_database*);

  using file_collection_t =
      std::vector<std::unique_ptr<cppast::cpp_file>>;

  const file_collection_t& file_collection() const {
    return parsed_files_;
  }

 private:
  file_collection_t parsed_files_;

  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
};
}  // namespace refl