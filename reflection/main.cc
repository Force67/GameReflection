#include <iostream>
#include <clang-c/Index.h>
using namespace std;

ostream& operator<<(ostream& stream, const CXString& str) {
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

const char* const CXErrorToString(CXErrorCode err) {
  switch (err) { 
     case CXError_Failure:
      return "Generic Failure";
    case CXError_Crashed:
      return "Crashed while performing requested operation";
    case CXError_InvalidArguments:
      return "Arguments violate the function contract";
    case CXError_ASTReadError:
      return "AST deserilization error";
      break;
    default:
      return nullptr;
  }
}

auto AstVisitor(CXCursor c, CXCursor parent, CXClientData data) {
  auto return_val = CXChildVisit_Continue;



  if ((clang_isDeclaration(clang_getCursorKind(c)) != 0 && clang_isInvalidDeclaration(c) == 0 &&
       clang_isCursorDefinition(c) != 0) ||
      (clang_getCursorKind(c) == CXCursor_InclusionDirective) || (clang_getCursorKind(c) == CXCursor_MacroDefinition) ||
      (clang_getCursorKind(c) == CXCursor_NamespaceAlias)) {
    if (clang_getCursorKind(c) == CXCursor_Namespace) {
      return_val = CXChildVisit_Recurse;
    }

    // extend macro definition to include "#define "
    if (clang_getCursorKind(c) == CXCursor_MacroDefinition) {
      __debugbreak();
    }
  }

  return return_val;
}

bool print_function_prototype(CXCursor cursor) {
  CXType type = clang_getCursorType(cursor);
  // printf("%s\n", clang_getCString(clang_getCursorSpelling(cursor)));
  // printf("%s\n", clang_getCString(clang_getTypeSpelling(type)));

  enum CXCursorKind kind = clang_getCursorKind(cursor);
  if (kind == CXCursor_FunctionDecl || 
        kind == CXCursor_CXXMethod || 
        kind == CXCursor_FunctionTemplate || 
        kind == CXCursor_Constructor ||
        kind == CXCursor_MacroDefinition || 
        kind == CXCursor_MacroExpansion || 
        kind == CXCursor_MacroInstantiation) {
    const char* function_name = clang_getCString(clang_getCursorSpelling(cursor));
    const char* return_type = clang_getCString(clang_getTypeSpelling(clang_getResultType(type)));
    printf("%s,%s(", return_type, function_name);

    int num_args = clang_Cursor_getNumArguments(cursor);
    for (int i = 0; i < num_args - 1; ++i) {
      // CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
      // const char *arg_name = clang_getCString(clang_getCursorSpelling(arg_cursor));
      // if (strcmp(arg_name, "") == 0) {
      //     arg_name = "no name!";
      // }
      const char* arg_data_type = clang_getCString(clang_getTypeSpelling(clang_getArgType(type, i)));
      printf("%s,", arg_data_type);
    }
    const char* arg_data_type = clang_getCString(clang_getTypeSpelling(clang_getArgType(type, num_args - 1)));
    printf("%s", arg_data_type);

    printf(")\n");
  }
  return true;
}

enum CXChildVisitResult functionVisitor(CXCursor cursor, CXCursor parent, CXClientData clientData) {
  //if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0)
  //  return CXChildVisit_Continue;

  enum CXCursorKind kind = clang_getCursorKind(cursor);
  if ((kind == CXCursor_FunctionDecl || kind == CXCursor_CXXMethod || kind == CXCursor_FunctionTemplate ||
       kind == CXCursor_Constructor || kind == CXCursor_MacroDefinition || kind == CXCursor_MacroExpansion ||
       kind == CXCursor_MacroInstantiation)) {
    print_function_prototype(cursor);
  }

  return CXChildVisit_Recurse;
}


int main() {
    // create context
  CXIndex index = clang_createIndex(0, 0);

  CXTranslationUnit unit = nullptr;
  auto err = clang_parseTranslationUnit2(index, "api.hpp", nullptr, 0, nullptr, 0, CXTranslationUnit_None, &unit);
  if (err != CXError_Success) {
    errs() << "Failed to parse translation unit: " << CXErrorToString(err) << "\n";
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      [](CXCursor c, CXCursor parent, CXClientData client_data) {
        cout << "Cursor_recuse parent '" << clang_getCursorSpelling(parent) << "' of kind '"
             << clang_getCursorKindSpelling(clang_getCursorKind(parent)) << "'\n";


        cout << "Cursor_recuse child '" << clang_getCursorSpelling(c) << "' of kind '"
             << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
        return CXChildVisit_Recurse;
      },
      nullptr);

   clang_visitChildren(cursor, functionVisitor, nullptr);

 // clang_visitChildren(cursor, AstVisitor, nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}