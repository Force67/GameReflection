// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include <cppast/code_generator.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <cppast/cpp_class.hpp>

namespace refl {
class SymbolTable;

class CodeAnalyzer {
 public:
  CodeAnalyzer();

  void Process(const cppast::cpp_file&, SymbolTable*);

 private:
  void LoadConfig();
  void IngestComments(const cppast::cpp_file&);

  bool IsMemberFuncReflective(const cppast::cpp_member_function_base&);
  bool IsFreeFuncReflective(const cppast::cpp_function&);
  bool IsVariableReflective(const cppast::cpp_variable&);
  bool IsCommentCommand(const std::string&);

 private:
  std::string func_marker_;
  std::string var_marker_;
  std::string hidden_marker_;
};
}