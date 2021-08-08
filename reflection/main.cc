// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "compile_database.h"
#include "tweak/tilted_tweak_export.h"

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

// -tweaks-db-name="test.json"
static cl::opt<std::string> TweaksDbName("tweaks-db-name",
                                         cl::desc("Name of tweaks db"),
                                         cl::value_desc("filename"),
                                         cl::init("tweaks_db.json"));

static type_safe::optional<refl::ClangCompileDatabase> GetCompileCommands() {
  if (InputPath.empty())
    return type_safe::nullopt;

  return refl::ClangCompileDatabase(InputPath, InputPath + "/compile_commands.json");
}

static void InitOpts(int argc, char** argv) {
  llvm::SmallVector<const char*, 64> new_argv;

  // first look at config.
  // LLVM handles absolute pathing for us.
  llvm::BumpPtrAllocator alloc;
  llvm::StringSaver storage(alloc);
  cl::readConfigFile("config.cfg", storage, new_argv);

  // add real cl args while config values take precedence.
  for (int i = 0; i < argc; i++) {
    auto it = std::find_if(new_argv.begin(), new_argv.end(), [&](const char* arg) {
      return std::strcmp(arg, argv[i]) == 0;
    });

    if (it == new_argv.end()) {
      new_argv.push_back(argv[i]);
    }
  }

  // fill in the rest from command line.
  cl::ParseCommandLineOptions(
      static_cast<int>(new_argv.size()), new_argv.data(),
      "GameRefl (C) Force67 2021.\n");
}

int main(int argc, char** argv) {
  using namespace refl;
  InitOpts(argc, argv);

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