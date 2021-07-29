// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.

#include "code_analyzer.h"
#include "symbol_table.h"

#include <cppast/cpp_variable.hpp>

namespace refl {

namespace {
constexpr char kDefaultFuncMarker[] = "refl::override";
constexpr char kDefaultVarMarker[] = "refl::override";

// convert a member function to class::name
std::string PrettyFormatClassMember(const cppast::cpp_member_function& func) {
  assert(func.parent());
  auto& parent = static_cast<const cppast::cpp_class&>(func.parent().value());
  return parent.name() + "::" + func.name();
}
}  // namespace

CodeAnalyzer::CodeAnalyzer() {
  LoadConfig();
}

bool CodeAnalyzer::IsFreeFuncReflective(const cppast::cpp_function& func) {
  return cppast::has_attribute(func, func_marker_) && !func.is_consteval() && !func.is_constexpr();
}

bool CodeAnalyzer::IsMemberFuncReflective(const cppast::cpp_member_function_base& func) {
  return cppast::has_attribute(func, func_marker_) && !func.is_consteval() && !func.is_constexpr();
}

bool CodeAnalyzer::IsVariableReflective(const cppast::cpp_variable& var) {
  return cppast::has_attribute(var, var_marker_) && !var.is_constexpr();
}

void CodeAnalyzer::Process(const cppast::cpp_file& ast_file, SymbolTable* sym_table) {
  cppast::visit(ast_file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
    switch (e.kind()) {
      case cppast::cpp_entity_kind::member_function_t: {
        auto& func = static_cast<const cppast::cpp_member_function&>(e);
        if (IsMemberFuncReflective(func)) {
          std::string fullName = PrettyFormatClassMember(func);
          std::printf("Reflective member function found: %s\n", fullName.c_str());
          std::printf("Func args: %s\n", func.signature().c_str());

          std::string sig = func.signature();
          sym_table->AddSymbol(fullName, &sig);
        }
        break;
      }
      case cppast::cpp_entity_kind::function_t: {
        auto& func = static_cast<const cppast::cpp_function&>(e);
        if (IsFreeFuncReflective(func)) {
          std::printf("Reflective function found: %s\n", func.name().c_str());
          sym_table->AddSymbol(func.name(), &func.signature());
        }
        break;
      }
      case cppast::cpp_entity_kind::variable_t: {
        auto& var = static_cast<const cppast::cpp_variable&>(e);
        if (IsVariableReflective(var)) {
          std::printf("Reflective variable found: %s\n", var.name().c_str());
          sym_table->AddSymbol(var.name(), nullptr);
        }
        break;
      }
        // ignore unknown cases.
      default:
        break;
    }
    return true;
  });
}

void CodeAnalyzer::LoadConfig() {
  if (func_marker_.empty()) {
    func_marker_ = kDefaultFuncMarker;
  }

  if (var_marker_.empty()) {
    var_marker_ = kDefaultVarMarker;
  }
}
}  // namespace refl