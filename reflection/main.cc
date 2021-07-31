
#include "symbol_table.h"
#include "parser.h"

#include "sys_utils.h"

// gamerefl -disable-opt
static cl::opt<std::string> InputFilename(cl::Positional,
                                          cl::desc("<input source file>"),
                                          cl::init("-"),
                                          cl::value_desc("filename"));

int main(int argc, char** argv) {
  cl::ParseCommandLineOptions(
      argc, argv, "TiltedPhoques Content tool\n");

  if (InputFilename.empty()) {
    cl::PrintHelpMessage();
    return 0;
  }

  refl::SymbolTable table;

  refl::Parser parser(&table);
  auto file = parser.TryParseFile(InputFilename);
  if (!file) {
    fmt::print("Failed to consume file {}", file->name());
    return -1;
  }

  #if 0
  auto results = refl::FindFiles(R"(C:\Users\vince\Documents\Development\Tilted\LibCreation\dev\src)");
  for (auto& it : *results) {
    fmt::print("FilePath: {}\n", it);
  }
  #endif

  parser.Traverse(*file);

  table.ExportHookHeader();
  table.ExportJson();
}