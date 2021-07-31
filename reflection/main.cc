
#include "code_analyzer.h"
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

  refl::Parser parser;
  auto file_ast = parser.TryParseFile(InputFilename);
  if (!file_ast) {
    fmt::print("Failed to consume file {}", file_ast->name());
    return -1;
  }

  refl::CodeAnalyzer analyzer;
  analyzer.Process(*file_ast, &table);

  table.ExportHookHeader();
  table.ExportJson();
}