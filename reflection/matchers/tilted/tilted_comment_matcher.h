// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "matchers/matcher_base.h"

namespace refl {

class TiltedCommentMatcher final : public MatcherBase {
 public:
  TiltedCommentMatcher();
  ~TiltedCommentMatcher() = default;

  bool Match(const cppast::cpp_entity&, Phase) override;
};
}  // namespace refl
