// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "matchers/tilted_comment_matcher.h"

namespace refl {

namespace {
cl::opt<std::string> HideFileMarker("hide-file-marker",
                                    cl::desc("C++ comment which will trigger the file to be added "
                                             "to the exclusion list"),
                                    cl::value_desc("marker-name"),
                                    cl::init("//!NOPUBLIC"));

bool IsPrivateFile(const std::string& comment) {
  // the very first word must contain the phrase.
  return comment.find(HideFileMarker.c_str(), 0u,
                      HideFileMarker.length()) != std::string::npos;
}
}  // namespace

TiltedCommentMatcher::TiltedCommentMatcher()
    : MatcherBase("TiltedPhoques", "TiltedCommentMatcher") {
}

bool TiltedCommentMatcher::Match(const cppast::cpp_entity& entity, Phase phase) {
  if (phase == Phase::kFirstPass) {
    // currently we don't handle comments which are attached to entities,
    // this may change though.

    return false;
  }

  // free comments are analyzed.
  if (phase == Phase::kSecondPass &&
      entity.kind() == cppast::cpp_entity_kind::file_t) {
    const auto& file = static_cast<const cppast::cpp_file&>(entity);

    auto comments = file.unmatched_comments();
    assert(comments.size().get() > 0);

    #if 0
    if (IsPrivateFile(comments.data()->content)) {
      return true;
    }
    #endif
  }

  return false;
}

}  // namespace refl