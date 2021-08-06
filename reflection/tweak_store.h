#pragma once
// tweaks db holds the implementation for all exported symbols

#include <rapidjson/document.h>

namespace refl {

/// A JSON based tweak database for TiltedPhoques.
/// It follows the following format:
/// https://jsoneditoronline.org/#right=local.docele&left=local.xujubo
/* {
  "version": 1.0,
  "file-exclusion-list": [
    "header1.h",
    "header2.hpp"
  ],
  "override-list": [
    {
      "name": "MyFunction",
      "signature": "(void*, int)",
      "code-pattern": "AB BA CD FF"
    },
    {
      "name": "Namespace::MyFunction",
      "signature": "(int)",
      "code-pattern": "FF CC AA FF"
    }]
}*/

struct FuncRecord {
  // TODO: limit name length?
  std::string name;
  std::string signature;

  // TODO: llvm small string?
  std::string pattern;
};

class TweaksDatabase {
 public:
  explicit TweaksDatabase(const std::string& file_path);
  ~TweaksDatabase();

  static std::unique_ptr<TweaksDatabase> 
      LoadFromFile(const std::string& path);

  bool StoreToFile();
 private:
  bool Parse(const char* data);
  std::unique_ptr<rapidjson::Document> Compose();

 private:
  std::string file_path_;
  std::vector<FuncRecord> func_records_;
  std::vector<std::string> file_exclusions_;
};

TweaksDatabase* get_persistant_state();
}  // namespace refl