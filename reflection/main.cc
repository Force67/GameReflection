
#include "symbol_table.h"
#include "parser.h"

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

  parser.Traverse(*file);

  table.ExportHookHeader();
  table.ExportJson();
}