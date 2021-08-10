// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tilted/json_tweak_database.h"

#include "matchers/match_registry.h"
#include "matchers/tilted/tilted_attribute_matcher.h"

namespace refl {

void ExportTiltedV2(MatchRegistry& match) {
  std::vector<const cppast::cpp_entity*> results;
  // proposal to fetch objects without needing any sort of matcher instance
  match.ResultsByID((uintptr_t)TiltedAttributeMatcher::ID, results);


}

namespace {
// convert a member function to class::name
std::string PrettyFormatClassMember(const cppast::cpp_member_function& func) {
  assert(func.parent());
  auto& parent = static_cast<const cppast::cpp_class&>(func.parent().value());
  return parent.name() + "::" + func.name();
}
#if 0
bool WriteTweaksCXXRegistry(const MatcherBase::entity_collection_t& findings, const std::string& file_path) {
  std::string tweaks_data =
      "//Copyright (C) Force67 2021.\n"
      "This file is auto generated. Do not edit.";

  std::unique_ptr<WritableMemoryBuffer> buffer =
      WritableMemoryBuffer::getNewMemBuffer(tweaks_data.size());

  if (buffer) {
    std::memcpy(buffer->getBufferStart(),
                tweaks_data.data(), tweaks_data.size());

    return true;
  }

  return false;
}
#endif
}  // namespace

void ExportTiltedPhoquesTweaks(Parser& parser) {
#if 0
  std::vector<MatcherBase*> matches;
  parser.FindAllMatchersOfDomain("TiltedPhoques", matches);

  // TODO: yeet hacky array bound access.
  // view results of matchers.
  auto& attribute_findings = matches[0]->GetMatchedResults();
  auto& comment_findings = matches[1]->GetMatchedResults();

  WriteTweaksCXXRegistry(attribute_findings, "lol.cxx");

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
