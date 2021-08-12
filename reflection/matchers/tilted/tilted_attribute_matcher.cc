// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "pch.h"
#include "tilted_attribute_matcher.h"

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
cl::opt<std::string> ExperimentalMarker("tilted-class-marker",
                                        cl::desc("C++ attribute class marker"),
                                        cl::value_desc("attribute-name"),
                                        cl::init("refl::override"));
}  // namespace

char TiltedAttributeMatcher::ID = 0;

TiltedAttributeMatcher::TiltedAttributeMatcher()
    : MatcherBase("TiltedPhoques", "TiltedAttributeMatcher") {
}

bool TiltedAttributeMatcher::Run(LocalContext& LC, const cppast::cpp_entity& entity, Phase phase) {
  REFL_UNUSED(phase);
  using namespace cppast;

  switch (entity.kind()) {
    case cpp_entity_kind::class_t: {
      LC.selected = &entity;
      break;
    }
    case cpp_entity_kind::member_function_t: {
      const auto& function = asa<cpp_member_function>(entity);

      // we are marked as a child by the parent.
      // watch this shit blow up.
      if (&function.parent().value() == LC.selected) {
        __debugbreak();
        return true;
      }

      // attribute + not const eval + non constexpr
      return has_attribute(function, FunctionMarker) &&
             !function.is_consteval() && !function.is_constexpr();
    }
    case cpp_entity_kind::function_t: {
      const auto& function = asa<cpp_function>(entity);
      return cppast::has_attribute(function, FunctionMarker) &&
             !function.is_consteval() && !function.is_constexpr();
    }
    case cpp_entity_kind::variable_t: {
      const auto& variable = asa<cpp_variable>(entity);
      return cppast::has_attribute(variable, VariableMarker) && !variable.is_constexpr();
    }
    default:
      return false;
  }

  return false;
}
}  // namespace refl