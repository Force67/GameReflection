
#include "sys_utils.h"
#include <llvm/Support/FileSystem.h>

namespace refl {

type_safe::optional<std::vector<std::string>> FindFiles(const std::string& input_path) {
  std::vector<std::string> results;

  std::error_code EC;
  for (sys::fs::recursive_directory_iterator Dir(input_path, EC), DirEnd; Dir != DirEnd && !EC; Dir.increment(EC)) {
    const std::string& Path = Dir->path();
    sys::fs::file_status Status;
    EC = sys::fs::status(Path, Status);
    switch (Status.type()) {
      case sys::fs::file_type::regular_file:
      case sys::fs::file_type::symlink_file:
      case sys::fs::file_type::type_unknown:
        results.push_back(Path);
        break;
      default: /*ignore*/;
    }
  }

  return std::move(results);
}
}  // namespace refl