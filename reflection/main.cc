
#include "parser.h"
#include "rl_database.h"
#include "file_collection.h"

int main(int argc, char** argv) {
  using namespace refl;

  cl::ParseCommandLineOptions(argc, argv, "TiltedPhoques Content tool\n");
  if (!FileCollection::HasPathDirective()) {
    cl::PrintHelpMessage();
    return 0;
  }

  // start assembling a collection of files.
  FileCollection collection;
  collection.SearchFiles();

  Parser parser;
  if (!parser.TryParseFiles(collection)) {
    fmt::print("Failed to parse files");
    return -1;
  }

  { 
    RlDatabase database;
    parser.TraverseFiles(database);

    database.WriteCxxHeader();
    database.UpdateJsonReport();
  }
}