#pragma once

#include "logger.h"

namespace refl {
class Store;
class FileCollection;

class Parser {
 public:
  using file_collection_t = std::vector<std::unique_ptr<cppast::cpp_file>>;

  Parser();
  
  bool TryParseFiles(FileCollection&);
  void TraverseFiles(Store&);

 private:
  void DoTraverse(Store&, cppast::cpp_file&);

 private:
  file_collection_t parsed_files_;
  cppast::libclang_compile_config clang_config_;
  std::unique_ptr<Logger> logger_;
};
}  // namespace refl