// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "tweak/tweak_json_database.h"

namespace refl {

namespace {
// convert a member function to class::name
std::string PrettyFormatClassMember(const cppast::cpp_member_function& func) {
  assert(func.parent());
  auto& parent = static_cast<const cppast::cpp_class&>(func.parent().value());
  return parent.name() + "::" + func.name();
}

bool WriteTweaksCXXRegistry(MatcherBase::entity_collection_t& findings, const std::string& file_path) {
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
}  // namespace

void ExportTiltedPhoquesTweaks(Parser& parser) {
  std::vector<MatcherBase*> matches;
  parser.FindAllMatchersOfDomain("TiltedPhoques", matches);

  // this is rather hacky

  // view results of matchers.
  MatcherBase::entity_collection_t attribute_findings;
  matches[0]->View(attribute_findings);

  MatcherBase::entity_collection_t comment_findings;
  matches[1]->View(comment_findings);

  WriteTweaksCXXRegistry(attribute_findings, "lol.cxx");

  if (std::unique_ptr<TweaksDatabase> database =
          TweaksDatabase::LoadFromFile("lol.json")) {
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
}
}  // namespace refl
