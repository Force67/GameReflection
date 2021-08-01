
#include "store.h"
#include "parser.h"
#include "file_collection.h"

int main(int argc, char** argv) {
  using namespace refl;

  cl::ParseCommandLineOptions(argc, argv, "TiltedPhoques Content tool\n");
  if (!FileCollection::HasPathDirective()) {
    cl::PrintHelpMessage();
    return 0;
  }

  FileCollection collection;
  collection.SearchFiles();

  Parser parser;
  if (!parser.TryParseFiles(collection)) {
    fmt::print("Failed to parse files");
    return -1;
  }

  { 
    Store store;
    parser.TraverseFiles(store);

    store.ExportHookHeader();
    store.ExportJson();
  }
}