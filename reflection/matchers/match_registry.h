// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {
class MatcherBase;

class MatchRegistry {
  friend class MatcherBase;
 public:
  MatchRegistry();

  void Add(std::unique_ptr<MatcherBase> match);
  void DoMatchMT(class Parser&);

  void FindAllMatchersOfDomain(llvm::StringRef domain, 
	  std::vector<MatcherBase*>& out);
 private:
  void InvokeMatcher(MatcherBase& m, const cppast::cpp_entity&);

private:
  std::vector<std::unique_ptr<MatcherBase>> registry_;
};
}  // namespace refl