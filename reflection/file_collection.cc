
#include "store.h"
#include "file_collection.h"
#include <llvm/Support/FileSystem.h>

namespace refl {
namespace {
cl::opt<std::string> InputPath(cl::Positional, cl::desc("<input source path>"), cl::init("-"), cl::value_desc("path"));
cl::opt<bool> EnabledVSXMake("vsxmake", cl::desc("Utilize Visual Studio for file collection"), cl::init(false));

void FindFilesByDirectory(const std::string& input_path, std::vector<std::string>& out) {
  std::error_code EC;
  for (sys::fs::recursive_directory_iterator Dir(input_path, EC), DirEnd; Dir != DirEnd && !EC; Dir.increment(EC)) {
    const std::string& Path = Dir->path();
    sys::fs::file_status Status;
    EC = sys::fs::status(Path, Status);
    switch (Status.type()) {
      case sys::fs::file_type::regular_file:
      case sys::fs::file_type::symlink_file:
      case sys::fs::file_type::type_unknown:
        out.push_back(Path);
        break;
      default: /*ignore*/;
    }
  }
}
}  // namespace

FileCollection::FileCollection() {}

void FileCollection::SearchFiles() {
  FindFilesByDirectory(InputPath, file_list_);
  // this is done so clang wont throw a fit if it encounters paths it deems bad
  for (auto& str : file_list_) {
    std::replace(str.begin(), str.end(), '\\', '/');
  }
}

bool FileCollection::HasPathDirective() {
  return !InputPath.empty();
}

// TODO: parse MSVC solution.
// premake integration
// xmake integration
}  // namespace refl