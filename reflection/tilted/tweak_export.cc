// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tilted/json_tweak_database.h"

#include "matchers/match_maker.h"
#include "matchers/tilted/tilted_attribute_matcher.h"
#include "matchers/tilted/tilted_comment_matcher.h"

namespace {
// convert a member function to class::name
std::string PrettyFormatClassMember(const cppast::cpp_member_function& func) {
  assert(func.parent());
  auto& parent = static_cast<const cppast::cpp_class&>(func.parent().value());
  return parent.name() + "::" + func.name();
}
}  // namespace

namespace refl {

void ExportTiltedPhoquesTweaks(MatchMaker& match_maker) {
  std::vector<const cppast::cpp_entity*> attribute_results;
  match_maker.GetResults<TiltedAttributeMatcher>(attribute_results);

  std::vector<const cppast::cpp_entity*> comment_results;
  match_maker.GetResults<TiltedCommentMatcher>(comment_results);

  #if 0
  if (std::unique_ptr<JsonTweakDatabase> database =
          JsonTweakDatabase::LoadFromFile("lol.json")) {
    for (const cppast::cpp_entity* e : comment_findings) {
      auto* as_file = static_cast<const cppast::cpp_file*>(e);
      database->AddFileExclusion(as_file->name());
    }

    for (const cppast::cpp_entity* e : attribute_findings) {
      switch (e->kind()) {
        case cppast::cpp_entity_kind::function_t: {
          const auto* f = static_cast<const cppast::cpp_function*>(e);
          database->AddAttribRecord(f->name(), f->signature());
          break;
        }
        case cppast::cpp_entity_kind::member_function_t: {
          const auto* f = static_cast<const cppast::cpp_member_function*>(e);
          database->AddAttribRecord(PrettyFormatClassMember(*f), f->signature());
          break;
        }
        case cppast::cpp_entity_kind::variable_t: {
          const auto* v = static_cast<const cppast::cpp_variable*>(e);
          database->AddAttribRecord(v->name(), "");
          break;
        }
        default:
          break;
      }
    }

    database->StoreToFile();
  }
  #endif
}
}  // namespace refl
