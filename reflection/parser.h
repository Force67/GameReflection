// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "logger.h"
#include "matchers/matcher_base.h"

namespace refl {
class ClangCompileDatabase;

class Parser {
 public:
  Parser();

  bool TryParse(const std::vector<std::string>& file_list, 
      cppast::libclang_compilation_database*);
  
  void TraverseFiles();
  void DebugLogStats();

  void FindAllMatchersOfDomain(llvm::StringRef domain, std::vector<MatcherBase*>& out);
  // extract domain specific
 private:
  void InitializeConfig(cppast::libclang_compile_config&);
  void DoTraverse(cppast::cpp_file&);

 private:
  using file_collection_t = std::vector<std::unique_ptr<cppast::cpp_file>>;
  file_collection_t parsed_files_;

  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;

  struct Stats {
    std::atomic<uint32_t> entity_match_count = 0u;
  };

  Stats stats_{};

  std::vector<std::unique_ptr<MatcherBase>> matcher_registry_;
};
}  // namespace refl