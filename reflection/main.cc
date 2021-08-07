// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "config.h"
#include "compile_database.h"
#include "tweak/tilted_tweak_export.h"

#if 0
static cl::list<std::string> InputFilenames(cl::Positional,
                                            cl::desc("<input source files>"),
                                            cl::OneOrMore);
#endif

// point this to where compile_commands.json is located.
cl::opt<std::string> InputPath(cl::Positional,
                               cl::desc("<input build path>"),
                               cl::init("-"),
                               cl::value_desc("path to where compile_commands.json is located"));

cl::opt<std::string> TweaksDbName("tweaks-db-name",
                                    cl::desc("Name of tweaks db"),
                                    cl::value_desc("filename"),
                                    cl::init("tweaks_db.json"));

type_safe::optional<refl::ClangCompileDatabase> GetCompileCommands() {
  if (InputPath.empty())
    return type_safe::nullopt;

  return refl::ClangCompileDatabase(InputPath, InputPath + "/compile_commands.json");
}

/*
  cppast::libclang_compile_config new_config(compilation_db,
      InputPath + "/compile_commands.json");
*/

int main(int argc, char** argv) {
  using namespace refl;
  // init command line
  cl::ParseCommandLineOptions(argc, argv, "TiltedPhoques Content tool\n");
  config::InitConfig();

  // currently we need to require this..
  if (InputPath.empty()) {
    cl::PrintHelpMessage();
    return 0;
  }

  std::vector<std::string> file_list;
  type_safe::optional<ClangCompileDatabase> commands;
  // we have a commands db, that makes our life much easier.
  if (commands = GetCompileCommands()) {
    commands.value().CollectFiles(file_list);
  }

  // rather hacky conversion..
  // init parser.
  Parser parser;
  if (!parser.TryParse(file_list, &commands.value())) {
    fmt::print("Failed to parse files");
    return -1;
  }

  // todo: domain switch check.
  ExportTiltedPhoquesTweaks(parser);
  return 0;
}