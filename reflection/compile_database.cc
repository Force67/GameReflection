// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "compile_database.h"
#include <clang-c/CXCompilationDatabase.h>

namespace refl {

ClangCompileDatabase::ClangCompileDatabase(const std::string& build_dir, const std::string& commands_json)
    : cppast::libclang_compilation_database(build_dir), build_dir_(build_dir), commands_file_(commands_json) {}

void* ClangCompileDatabase::db_handle() {
  // this is a rather big hack but we don't want to break abi.
  // not a vtable so we can access it at null
  return this;
}

void ClangCompileDatabase::CollectFiles(std::vector<std::string>& files) {
  using cppast::libclang_compilation_database;

  CXCompileCommands compile_commands =
      clang_CompilationDatabase_getCompileCommands(db_handle(), commands_file_.c_str());

  for (uint32_t i = 0; i < clang_CompileCommands_getSize(compile_commands); i++) {
    CXCompileCommand command = clang_CompileCommands_getCommand(compile_commands, i);
    CXString str = clang_CompileCommand_getFilename(command);

    files.push_back(clang_getCString(str));
  }

  clang_CompileCommands_dispose(compile_commands);
}

}  // namespace refl