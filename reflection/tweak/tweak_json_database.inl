// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tweak_json_database.h"

namespace refl {

void TweaksDatabase::AddAttribRecord(const std::string& name, const std::string& sig) {
	// TODO: pls fix me.
  auto &r = func_records_.emplace_back();
  r.name = name;
  r.signature = sig;
}

void TweaksDatabase::AddFileExclusion(const std::string& perfect_string) {
  file_exclusions_.push_back(perfect_string);
}
}