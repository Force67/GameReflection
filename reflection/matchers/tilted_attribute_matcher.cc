// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "pch.h"
#include "matchers/tilted_attribute_matcher.h"

namespace refl {

namespace {
// tweak able values from command line.
cl::opt<std::string> FunctionMarker("tilted-function-marker",
                                    cl::desc("C++ attribute function marker"),
                                    cl::value_desc("attribute-name"),
                                    cl::init("refl::override"));
cl::opt<std::string> VariableMarker("tilted-variable-marker",
                                    cl::desc("C++ attribute variable marker"),
                                    cl::value_desc("attribute-name"),
                                    cl::init("refl::override"));
}  // namespace

TiltedAttributeMatcher::TiltedAttributeMatcher()
    : MatcherBase("TiltedPhoques", "TiltedAttributeMatcher") {
}

TiltedAttributeMatcher::~TiltedAttributeMatcher() {
}

bool TiltedAttributeMatcher::Match(const cppast::cpp_entity& entity, Phase ignored_phase) {
  switch (entity.kind()) {
    case cppast::cpp_entity_kind::member_function_t: {
      auto& func = static_cast<const cppast::cpp_member_function&>(entity);
      // attribute + not const eval + non constexpr
      return cppast::has_attribute(func, FunctionMarker) &&
             !func.is_consteval() && !func.is_constexpr();
    }
    case cppast::cpp_entity_kind::function_t: {
      auto& func = static_cast<const cppast::cpp_function&>(entity);
      return cppast::has_attribute(func, FunctionMarker) &&
             !func.is_consteval() && !func.is_constexpr();
    }
    case cppast::cpp_entity_kind::variable_t: {
      auto& var = static_cast<const cppast::cpp_variable&>(entity);
      return cppast::has_attribute(var, VariableMarker) && !var.is_constexpr();
    }
    default:
      return false;
  }
}
}  // namespace refl