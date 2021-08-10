// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "register_tp_matchers.h"
#include "matchers/match_registry.h"

#include "tilted_attribute_matcher.h"
#include "tilted_comment_matcher.h"

namespace refl {

cl::OptionCategory TPCategory("Tilted Options",
                              "Configure the tilted passes");

static cl::opt<bool> TPAttributeMatcher(
    "tp-enable-attrib",
    cl::desc("Enable attribute analysis"),
    cl::init(true),
    cl::cat(TPCategory));

static cl::opt<bool> TPCommentMatcher(
    "tp-enable-comment",
    cl::desc("Enable comment analysis"),
    cl::init(true),
    cl::cat(TPCategory));

void RegisterTPMatchers(MatchRegistry& reg) {
  if (TPAttributeMatcher) {
    reg.Add(std::make_unique<TiltedAttributeMatcher>());
  }

  if (TPCommentMatcher) {
    reg.Add(std::make_unique<TiltedCommentMatcher>());
  }
}
}  // namespace refl