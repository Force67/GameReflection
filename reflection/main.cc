// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "config.h"
#include "compile_database.h"
#include "tilted/tweak_export.h"
#include "matchers/match_registry.h"

#include <llvm/Support/StringSaver.h>

#if 0
static cl::list<std::string> InputFilenames(cl::Positional,
                                            cl::desc("<input source files>"),
                                            cl::OneOrMore);
#endif

// point this to where compile_commands.json is located.
static cl::opt<std::string> InputPath(cl::Positional,
                                      cl::desc("<input build path>"),
                                      cl::init("-"),
                                      cl::value_desc("path to where compile_commands.json is located"));
static cl::opt<std::string> CompileCommandsPath("compile-commands",
                                         cl::desc("Clang compile commands file name"),
                                         cl::value_desc("<filename>"),
                                         cl::init("compile_commands.json"));

static type_safe::optional<refl::ClangCompileDatabase> GetCompileCommands() {
  if (InputPath.empty())
    return type_safe::nullopt;

  return refl::ClangCompileDatabase(InputPath, 
      InputPath + "/" + CompileCommandsPath);
}

static constexpr char kProgramDesc[] = 
"GameRefl v0. (C) 2021 Force67.";

int main(int argc, char** argv) {
  using namespace refl;
  InitOpts(argc, argv, kProgramDesc);

  if (InputPath.empty()) {
    cl::PrintHelpMessage();
    return 0;
  }

  // parser based on cppast
  Parser parser;

  std::vector<std::string> file_list;
  if (auto commands = GetCompileCommands()) {
    commands.value().CollectFiles(file_list);

    // ingest from clang compile commands db.
    if (!parser.ParseWithCompilationDatabase(file_list, &commands.value())) {
      fmt::print("Failed to ingest files from compilation database.\n"
          "Database location: {}\n", 
          InputPath);
      return -1;
    }
  }

  if (file_list.empty()) {
    fmt::print("No input files provided. terminating.\n");
    return -2;
  }

  MatchRegistry match_manager;
  match_manager.DoMatchMT(parser);

  // todo: domain switch check.
  ExportTiltedPhoquesTweaks(parser);
  return 0;
}