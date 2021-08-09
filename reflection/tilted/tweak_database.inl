// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tilted/tweak_database.h"

namespace refl {

void TweakDatabase::AddAttribRecord(const std::string& name, const std::string& sig) {
  // TODO: pls fix me.
  auto& r = attrib_records_.emplace_back();
  r.name = name;
  r.signature = sig;
}

void TweakDatabase::AddFileExclusion(const std::string& perfect_string) {
  file_exclusions_.push_back(perfect_string);
}
}  // namespace refl