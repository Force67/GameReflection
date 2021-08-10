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

  void FindAllGroupCitizens(llvm::StringRef domain, 
	  std::vector<MatcherBase*>& out);

  void ResultsByID(uintptr_t id, std::vector<const cppast::cpp_entity*> &results);

 private:
  std::vector<std::unique_ptr<MatcherBase>> registry_;

  // maps every pass to it's results:
  using results_map_t = 
	  std::map<uintptr_t, const cppast::cpp_entity*>;
  results_map_t analysis_results_; 
  std::mutex merge_lock_;
};
}  // namespace refl