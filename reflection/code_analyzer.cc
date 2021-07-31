// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.

#include "code_analyzer.h"
#include "symbol_table.h"

#include <cppast/cpp_preprocessor.hpp>
#include <cppast/cpp_variable.hpp>

namespace refl {

namespace {
cl::opt<std::string> FunctionMarker("func-marker",
                                      cl::desc("C++ attribute function marker"),
                                      cl::value_desc("attribute-name"),
                                      cl::init("refl::override"));
cl::opt<std::string> VariableMarker("var-marker",
                                      cl::desc("C++ attribute variable marker"),
                                      cl::value_desc("attribute-name"),
                                      cl::init("refl::override"));
cl::opt<std::string> HideFileMarker("hide-file-marker",
                                      cl::desc("C++ comment which will trigger the file to be added "
                                      "to the exclusion list"),
                                      cl::value_desc("marker-name"),
                                      cl::init("//!NOPUBLIC"));

// convert a member function to class::name
std::string PrettyFormatClassMember(const cppast::cpp_member_function& func) {
  assert(func.parent());
  auto& parent = static_cast<const cppast::cpp_class&>(func.parent().value());
  return parent.name() + "::" + func.name();
}

// determine if we should ignore given entity.
bool IsGoodEntity(cppast::cpp_entity& entity) {
  return !cppast::is_templated(entity) && 
      !cppast::is_friended(entity) && !entity.name().empty();
}
}  // namespace

CodeAnalyzer::CodeAnalyzer() {
}

bool CodeAnalyzer::IsFreeFuncReflective(const cppast::cpp_function& func) {
  return cppast::has_attribute(func, FunctionMarker) && !func.is_consteval() && !func.is_constexpr();
}

bool CodeAnalyzer::IsMemberFuncReflective(const cppast::cpp_member_function_base& func) {
  return cppast::has_attribute(func, FunctionMarker) && !func.is_consteval() && !func.is_constexpr();
}

bool CodeAnalyzer::IsVariableReflective(const cppast::cpp_variable& var) {
  return cppast::has_attribute(var, VariableMarker) && !var.is_constexpr();
}

bool CodeAnalyzer::IsCommentCommand(const std::string& comment) {
  size_t pos = comment.find(HideFileMarker);
  return pos != std::string::npos;
}

void CodeAnalyzer::Process(const cppast::cpp_file& ast_file, SymbolTable* sym_table) {
  cppast::visit(ast_file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
    if (is_templated(e) || e.name().empty())
      return true;

    switch (e.kind()) {
      case cppast::cpp_entity_kind::member_function_t: {
        auto& func = static_cast<const cppast::cpp_member_function&>(e);
        if (IsMemberFuncReflective(func)) {
          std::string fullName = PrettyFormatClassMember(func);
          std::string sig = func.signature();
          sym_table->AddSymbol(fullName, &sig);
        }
        break;
      }
      case cppast::cpp_entity_kind::function_t: {
        auto& func = static_cast<const cppast::cpp_function&>(e);
        if (IsFreeFuncReflective(func)) {
          sym_table->AddSymbol(func.name(), &func.signature());
        }
        break;
      }
      case cppast::cpp_entity_kind::variable_t: {
        auto& var = static_cast<const cppast::cpp_variable&>(e);
        if (IsVariableReflective(var)) {
          sym_table->AddSymbol(var.name(), nullptr);
        }
        break;
      }
      case cppast::cpp_entity_kind::file_t:
      default:
        auto x = e.comment();
        if (x) {
          std::printf("COMM : %s\n", x.value().c_str());
        }
        break;
    }
    return true;
  });

  IngestComments(ast_file);
}

void CodeAnalyzer::IngestComments(const cppast::cpp_file& file) {
  // If a comment belongs to an entity, it will be found while traversing for comments,
  // else if its a free comment for instance it will be stored in the unmatched_comments()
  // collection
  // see also: https://github.com/standardese/standardese/blob/e7a7fb8f59ba4b1cf59347ac016ec558e5d72ac3/src/comment.cpp
  cppast::visit(file, [&](const cppast::cpp_entity& entity, const cppast::visitor_info& info) {
    if (info.event == cppast::visitor_info::container_entity_exit)
      // entity already handled
      return true;
    else if (!cppast::is_templated(entity) && !cppast::is_friended(entity)) {
      // make sure to read the documentation for this function in order to understand the required syntax!

      // this comment is tied to a cpp entity!
      if (auto ref = entity.comment()) {
        fmt::print("Entity tied comment: {}\n", ref.value());
      }
    }

    return true;
  });

  // add free comments
  for (auto& free : file.unmatched_comments()) {
    fmt::print("Free Comment: {}\n", free.content);
  }
}
}  // namespace refl