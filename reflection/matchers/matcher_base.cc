// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "matchers/matcher_base.h"

namespace refl {

MatcherBase::MatcherBase(const char* domain, const char *matcher_name)
    : domain_(domain), matcher_name_(matcher_name) {
}

void MatcherBase::CollectEntity(const cppast::cpp_entity& entity) {
  std::lock_guard guard{lock_};
  entity_refs_.push_back(&entity);
}

void MatcherBase::RemoveEntity(const cppast::cpp_entity& entity) {
  std::lock_guard guard{lock_};
  auto it = std::find_if(entity_refs_.begin(), entity_refs_.end(), [&](const cppast::cpp_entity* e) {
    return e->name() == entity.name() && e->kind() == entity.kind();
  });

  if (it != entity_refs_.end()) {
    entity_refs_.erase(it);
  }
}
}  // namespace refl