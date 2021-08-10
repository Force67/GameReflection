// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {

struct MatcherBase {
  explicit MatcherBase(const char* group, const char* name)
      : group_(group), name_(name) {}
  virtual ~MatcherBase() = default;

  // not every detail is caught during the first run
  // therefor some files may require a second pass
  enum class Phase {
    kFirstPass,
    kSecondPass,
  };

  struct LocalContext {
    const cppast::cpp_entity* selected;
  };

  // This is called for every function
  virtual bool Run(LocalContext& local_context, const cppast::cpp_entity&, Phase) = 0;
  // Returns the bound id, which always shall be static char ID = 0;
  // the value doesn't matter, we only use it to uniquely identify the pass
  virtual char* GetID() = 0;

  llvm::StringRef GetGroupName() const { return group_; }
  llvm::StringRef GetMatcherName() const { return name_; }

 private:
  // each matcher has to belong to a specific group
  // in order to be selected/deselected
  const char* const group_;
  // matcher name
  // this might require changing to some uuid format
  const char* const name_;
};
}  // namespace refl