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

static std::string ConfigPath(char** argv) {
  void* main_address = reinterpret_cast<void*>(&ConfigPath);
  std::string path = llvm::sys::fs::getMainExecutable(argv[0], main_address);
  llvm::SmallString<256> buf{sys::path::parent_path(path)};
  llvm::sys::path::append(buf, "refl_config.cfg");
  return std::string(buf.str());
}

static void InitOpts(int argc, char** argv) {
  llvm::SmallVector<const char*, 64> new_argv;
  // exe path
  new_argv.push_back(argv[0]);

  // if the next entry is a positional argument
  // add it. since there is only one positional
  // argument, it's fine
  // this is a bit hacky. :(
  #if 0
  if (argc > 1) {
    llvm::StringRef arg0(argv[1]);
    if (arg0[0] != '-' && arg0.find("--") != 
        StringRef::npos) {
      new_argv.push_back(argv[1]);
    }
  }
  #endif

  // first look at config.
  llvm::BumpPtrAllocator alloc;
  llvm::StringSaver storage(alloc);

  if (cl::readConfigFile(ConfigPath(argv), storage, new_argv)) {
    // add real cl args while config values take precedence.
    for (int i = 1; i < argc; i++) {
      auto it = std::find_if(new_argv.begin(), new_argv.end(), [&](const char* arg) {
        const StringRef lhs(argv[i]);
        const StringRef rhs(arg);

        size_t equal_pos = lhs.find('=');
        if (equal_pos == StringRef::npos) {
          // non complex- direct compare
          return lhs == rhs;
        }

        size_t equal_pos_rhs = rhs.find('=');
        if (equal_pos == equal_pos_rhs) {
          // compare the bits before the parameter
          return lhs.substr(0, equal_pos) == rhs.substr(0, equal_pos_rhs);
        }

        return false;
      });

      // by excluding we override
      if (it == new_argv.end()) {
        new_argv.push_back(argv[i]);
      }
    }
  } else {
    for (int i = 1; i < argc; ++i)
      new_argv.push_back(argv[i]);
  }

  // fill in the rest from command line.
  cl::ParseCommandLineOptions(
      static_cast<int>(new_argv.size()), new_argv.data(),
      "GameRefl (C) Force67 2021.\n");
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cl::PrintHelpMessage();
    return 0;
  }

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