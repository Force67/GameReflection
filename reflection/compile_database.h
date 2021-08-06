// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {

class ClangCompileDatabase final : public cppast::libclang_compilation_database {
 public:
  explicit ClangCompileDatabase(const std::string& build_dir, const std::string& commands_json);

  void CollectFiles(std::vector<std::string>& files);

 private:
  void* db_handle();

  std::string build_dir_;
  std::string commands_file_;
};
}  // namespace refl