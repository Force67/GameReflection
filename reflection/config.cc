
#include "config.h"
#include <unordered_set>

#include <llvm/ADT/UniqueVector.h>
#include <llvm/Support/StringSaver.h>

namespace refl {

namespace {
// file name of config on disk
constexpr char kConfigFileName[] = "gamerefl.cfg";
// maximum amount of argument a command line is allowed to have
constexpr uint32_t kMaxArgs = 32u;
// mirrored as per llvm source in lib/Support/CommandLine.cpp
constexpr StringRef ArgPrefix = "-";
constexpr StringRef ArgPrefixLong = "--";

std::string ConfigPath(char* arg0) {
  void* main_address = reinterpret_cast<void*>(&ConfigPath);
  std::string path = llvm::sys::fs::getMainExecutable(arg0, main_address);
  llvm::SmallString<256> buf{sys::path::parent_path(path)};
  llvm::sys::path::append(buf, kConfigFileName);
  return std::string(buf.str());
}

// positional args don't start with those two characters.
bool IsPositionalArg(llvm::StringRef ref) {
  if (ref.find_first_of(ArgPrefix) == StringRef::npos &&
      ref.find_first_of(ArgPrefixLong) == StringRef::npos) {
    return true;
  }

  return false;
}
}  // namespace

void InitOpts(int argc, char** argv, const char* const desc) {
  assert(argc <= kMaxArgs);

  llvm::BumpPtrAllocator alloc;
  llvm::StringSaver storage(alloc);
  // in order to preserve the order - pun intended, we need to use
  // vector since neither set nor unordered_set can keep insertion
  // order stable
  // UniqueVector also doesn't solve the problem of comparing substrings,
  // so now we use a regular vector with our own uniqueness check
  llvm::SmallVector<const char*, kMaxArgs> new_args;
  auto unique_add = [&new_args](const llvm::StringRef ref) {
    const size_t pos = ref.find_first_of('=');
    // this is since we want to store the whole switch
    // and not a subset, so we cant ensure uniqueness.
    auto it = std::find_if(new_args.begin(), new_args.end(), [&](const char* a) {
      const StringRef arg(a);
      // bool is_found
      if (pos != StringRef::npos) {
        const size_t p2 = arg.find_first_of('=');
        if (p2 != std::string::npos) {
          // is_found = true;
          return ref.substr(0, pos).str() == arg.substr(0, p2);
        }
      }
      return ref == arg;
    });
    // not found? lets add it
    if (it == new_args.end()) {
      new_args.push_back(ref.data());
    }
  };

  llvm::SmallVector<const char*, 16> cfg_args;
  cl::readConfigFile(ConfigPath(argv[0]), storage, cfg_args);
  // first we add the real command line args
  // these will take precedence since they are added first
  for (int i = 0; i < argc; i++) {
    const llvm::StringRef arg(argv[i]);
    unique_add(arg);
  }
  // now we add the remaining args from config
  // note that positional arguments are explicitly forbidden and
  // are therefore ignored
  for (const char* c : cfg_args) {
    const llvm::StringRef arg(c);
    if (!IsPositionalArg(arg)) {
      unique_add(arg);
    }
  }
  // fill in the rest from command line.
  cl::ParseCommandLineOptions(
      static_cast<int>(new_args.size()),
      new_args.data(),
      desc);
}
}  // namespace refl