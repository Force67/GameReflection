// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {

class TweakDatabase {
  friend class TweakCodeExporter;
 public:
  TweakDatabase() = default;
  ~TweakDatabase() = default;

  inline void AddAttribRecord(const std::string& name, const std::string& sig);
  inline void AddFileExclusion(const std::string&);

  auto& attribute_list() const {
    return attrib_records_;
  }

 protected:
  struct AttributeRecord {
    // TODO: limit name length?
    std::string name;
    std::string signature;

    // TODO: llvm small string?
    std::string pattern = "";
  };

  std::vector<AttributeRecord> attrib_records_;
  std::vector<std::string> file_exclusions_;
};
}  // namespace refl

#include "tilted/tweak_database.inl"