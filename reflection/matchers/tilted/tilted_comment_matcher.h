// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "matchers/matcher_base.h"

namespace refl {

struct TiltedCommentMatcher final : public MatcherBase {
  TiltedCommentMatcher();
  ~TiltedCommentMatcher() = default;

  bool Run(LocalContext &, const cppast::cpp_entity&, Phase) override;

  char* GetID() override { return &ID; }
  static char ID;
};
}  // namespace refl
