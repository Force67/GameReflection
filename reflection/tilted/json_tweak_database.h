// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
// Purpose: JSON Exporter for Tweak-Database spec.
#pragma once

#include <rapidjson/document.h>
#include "tilted/tweak_database.h"

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

// todo: refactor it into a true database x)
class JsonTweakDatabase final : public TweakDatabase {
 public:
  explicit JsonTweakDatabase(const std::string& file_path);
  ~JsonTweakDatabase();

  static std::unique_ptr<JsonTweakDatabase> 
      LoadFromFile(const std::string& path);
  bool StoreToFile();
 private:
  bool Parse(const char* data);
  std::unique_ptr<rapidjson::Document> Compose();

 private:
  std::string file_path_;
};
}  // namespace refl

#include "tilted/json_tweak_database.inl"