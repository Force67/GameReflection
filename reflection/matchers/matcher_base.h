// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {
class MatcherBase {
  friend class Parser;
 public:
  explicit MatcherBase(const char* domain, const char *matcher_name);
  virtual ~MatcherBase() = default;

  // not every detail is caught during the first run
  // therefor some files may require a second pass
  enum class Phase {
	  kFirstPass,
	  kSecondPass,
  };

  virtual bool Match(const cppast::cpp_entity&, Phase) = 0;

  using entity_collection_t = std::vector<const cppast::cpp_entity*>;
  const entity_collection_t& GetMatchedResults() const { return entity_refs_; }

  llvm::StringRef GetDomainName() const { return domain_; }
  llvm::StringRef GetMatcherName() const { return matcher_name_; }
 private:
  // thread safe way of adding an entity to the queue of interest
  void CollectEntity(const cppast::cpp_entity&);
  // thread safe removal of entity of interest queue
  void RemoveEntity(const cppast::cpp_entity&);

 private:
  // each matcher has to belong to a specific domain
  // in order to be selected/deselected
  const char* const domain_;
  // matcher name
  // this might require changing to some uuid format
  const char* const matcher_name_;

  // we must protect the collection with a lock since matchers may
  // be invoked in parallel
  std::mutex lock_;
  entity_collection_t entity_refs_;
};
}  // namespace refl