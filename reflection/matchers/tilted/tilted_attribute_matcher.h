// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "matchers/matcher_base.h"

namespace refl {

struct TiltedAttributeMatcher final : public MatcherBase {
 public:
  TiltedAttributeMatcher();
  ~TiltedAttributeMatcher() = default;

  bool Run(LocalContext&, const cppast::cpp_entity&, Phase) override;

  char* GetID() override { return &ID; }
  static char ID;
};
}  // namespace refl
