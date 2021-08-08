// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include "matchers/matcher_base.h"

namespace refl {

class TiltedAttributeMatcher final : public MatcherBase {
 public:
  TiltedAttributeMatcher();
  ~TiltedAttributeMatcher();

  bool Match(const cppast::cpp_entity&, Phase) override;

 private:
  // this is so buggy with multithreading.. expect the worst
  const cppast::cpp_entity* current_class_ = nullptr;
};
}  // namespace refl
