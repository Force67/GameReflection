
#include "file_collection.h"
#include <llvm/Support/FileSystem.h>

namespace refl {
namespace {
cl::opt<std::string> InputPath(cl::Positional, cl::desc("<input source path>"), cl::init("-"), cl::value_desc("path"));
cl::opt<std::string> FileProvider("file-provider",
                                  cl::desc(
                                      R"(Which provider to use to supply a file list.
           Values:DirectorySeek,VisualStudio,CompileCommands)"),
                                  cl::init("DirectorySeek"));
cl::opt<bool> DisableExtentionFilterHack(
    "disable-file-finder-limit",
    cl::desc("Disable the filter active when discovering files by stupid directory search"),
    cl::init(false));

constexpr char* kKnownExtensions[] = {".h", ".hpp", ".cpp", ".h++", ".c++", ".hxx", ".cxx", ".hh", ".cc", ".H", ".C"};

// TODO: track by end name. e.g cc, cpp
void FindFilesByDirectory(const std::string& input_path, std::vector<std::string>& out) {
  std::error_code EC;
  for (sys::fs::recursive_directory_iterator Dir(input_path, EC), DirEnd; Dir != DirEnd && !EC; Dir.increment(EC)) {
    const std::string& Path = Dir->path();
    sys::fs::file_status Status;
    EC = sys::fs::status(Path, Status);
    switch (Status.type()) {
      case sys::fs::file_type::regular_file:
      case sys::fs::file_type::symlink_file:
      case sys::fs::file_type::type_unknown: {
        if (!DisableExtentionFilterHack) {
          size_t pos = Path.find_last_of('.');
          if (pos != std::string::npos) {
            for (char* k : kKnownExtensions) {
              if (Path.find(k, pos - 1) != std::string::npos)
                out.push_back(Path);
            }
          }
        } else {
          out.push_back(Path);
        }

        break;
      }
      default: /*ignore*/;
    }
  }
}
}  // namespace

FileCollection::FileCollection() {}

FileCollection::Provider FileCollection::MapActiveProvider() {
  if (FileProvider == "VisualStudio")
    return Provider::VisualStudio;
  if (FileProvider == "CompileCommands")
    return Provider::CompileCommands;

  return Provider::DirectorySeek;
}

bool FileCollection::SearchFiles() {
  // ask a provider to give us the most up to date file list
  const Provider provider = MapActiveProvider();
  switch (provider) {
    // most crude way of going about this, should be avoided
    // as it will try to parse anything in its way.
    case Provider::DirectorySeek:
      FindFilesByDirectory(InputPath, file_list_);
      break;
    default:
    case Provider::CompileCommands:
    case Provider::VisualStudio:
      return false;
  }

  // this is done so clang wont throw a fit if it encounters paths it deems bad
  for (auto& str : file_list_) {
    std::replace(str.begin(), str.end(), '\\', '/');
  }

  return true;
}

void FileCollection::RemoveFile(const std::string& file) {
  auto it = std::find_if(file_list_.begin(), file_list_.end(), [&file](std::string& str) { return str == file; });

  if (it != file_list_.end()) {
    file_list_.erase(it);
  }
}

bool FileCollection::HasPathDirective() {
  return !InputPath.empty();
}
}  // namespace refl