// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace refl {
class SymbolTable;

class CodeAnalyzer {
 public:
  CodeAnalyzer();

  void Process(const cppast::cpp_file&, SymbolTable*);

 private:
  void IngestComments(const cppast::cpp_file&);

  bool IsMemberFuncReflective(const cppast::cpp_member_function_base&);
  bool IsFreeFuncReflective(const cppast::cpp_function&);
  bool IsVariableReflective(const cppast::cpp_variable&);
  bool IsCommentCommand(const std::string&);
};
}