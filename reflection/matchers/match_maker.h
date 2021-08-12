// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {
struct MatcherBase;

class MatchMaker {
  friend struct MatcherBase;

 public:
  MatchMaker();

  // add a new matcher
  void Add(std::unique_ptr<MatcherBase> match);

  // match the parser results in multi threaded mode
  void DoMatchMT(class Parser&);

  // allows you to fetch results without needing to acquire an instance of the matcher
  template <class TMatcher>
  void GetResults(std::vector<const cppast::cpp_entity*>& results) {
    ResultsByID((uintptr_t)TMatcher::ID, results);
  }

 private:
  void ResultsByID(uintptr_t id, std::vector<const cppast::cpp_entity*>& results);

 private:
  std::vector<std::unique_ptr<MatcherBase>> registry_;

  // maps every pass to it's results:
  using results_map_t =
      std::map<uintptr_t, const cppast::cpp_entity*>;
  results_map_t analysis_results_;
  std::mutex merge_lock_;
};
}  // namespace refl