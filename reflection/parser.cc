
#include "parser.h"
#include "symbol_table.h"
#include <cppast/cpp_variable.hpp>

namespace refl {

namespace {
// tweakable values from command line.
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
  return !cppast::is_templated(entity) && !cppast::is_friended(entity) && !entity.name().empty();
}

// specialization for reflectiveness checks.
bool IsReflective(const cppast::cpp_function& func) {
  return cppast::has_attribute(func, FunctionMarker) && !func.is_consteval() && !func.is_constexpr();
}

bool IsReflective(const cppast::cpp_member_function_base& func) {
  return cppast::has_attribute(func, FunctionMarker) && !func.is_consteval() && !func.is_constexpr();
}

bool IsReflective(const cppast::cpp_variable& var) {
  return cppast::has_attribute(var, VariableMarker) && !var.is_constexpr();
}
}  // namespace

Parser::Parser(SymbolTable* tab) : logger_{CreateLogger()}, sym_tab_(tab) {
  cppast::compile_flags flags;

#if defined(OS_WIN)
  flags |= cppast::compile_flag::ms_extensions;
  flags |= cppast::compile_flag::ms_compatibility;
#endif

  // minimum language standard will be cxx17
  cppast::libclang_compile_config config;
  config.set_flags(cppast::cpp_standard::cpp_latest, flags);
  config.fast_preprocessing(true);
  // config.write_preprocessed(true);
}

std::unique_ptr<cppast::cpp_file> Parser::TryParseFile(const std::string& file_name) {
  cppast::cpp_entity_index idx;
  cppast::libclang_parser parser(type_safe::ref(*logger_));

  // logger_.log("Parsing file: {}", file_name);

  // sanitize path so cppast accepts it.
  std::string sanitzed_name = file_name;
  std::replace(sanitzed_name.begin(), sanitzed_name.end(), '\\', '/');
  auto file = parser.parse(idx, sanitzed_name, clang_config_);
  if (parser.error()) {
    return nullptr;
  }

  return file;
}

void Parser::Traverse(cppast::cpp_file& file) {
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

      switch (entity.kind()) {
        case cppast::cpp_entity_kind::member_function_t: {
          auto& func = static_cast<const cppast::cpp_member_function&>(entity);
          if (IsReflective(func)) {
            std::string fullName = PrettyFormatClassMember(func);
            std::string sig = func.signature();
            sym_tab_->AddSymbol(fullName, &sig);
          }
          break;
        }
        case cppast::cpp_entity_kind::function_t: {
          auto& func = static_cast<const cppast::cpp_function&>(entity);
          if (IsReflective(func)) {
            sym_tab_->AddSymbol(func.name(), &func.signature());
          }
          break;
        }
        case cppast::cpp_entity_kind::variable_t: {
          auto& var = static_cast<const cppast::cpp_variable&>(entity);
          if (IsReflective(var)) {
            sym_tab_->AddSymbol(var.name(), nullptr);
          }
          break;
        }
        default:
          break;
      }
    }

    return true;
  });

  // untied comments:
  for (auto& free : file.unmatched_comments()) {
    fmt::print("Free Comment: {}\n", free.content);
  }
}
}  // namespace refl