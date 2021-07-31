
#include "parser.h"

namespace refl {

Parser::Parser() : 
    logger_{CreateLogger()} {
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
}  // namespace refl