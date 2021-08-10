// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "thread_pool.h"

namespace refl {

namespace {
cl::opt<uint32_t> ParseThreadCount("parse-thread-count",
                                   cl::desc("How many threads to use while parsing"),
                                   cl::value_desc("thread-count"),
                                   cl::init(10u));
}  // namespace

Parser::Parser()
    : logger_{CreateLogger()} {
  InitializeConfig(clang_config_);
}

void Parser::DebugLogStats() {
    #if 0
  auto str = fmt::format("Log stats:\nMatched Entities:{}\n==\n",
                         stats_.entity_match_count);

  logger_->log("[Refl::Parser]", cppast::diagnostic{str, {}, cppast::severity::info});
  #endif
}

void Parser::InitializeConfig(cppast::libclang_compile_config& config) {
  cppast::compile_flags flags;
#if defined(OS_WIN)
  flags |= cppast::compile_flag::ms_extensions;
  flags |= cppast::compile_flag::ms_compatibility;
#endif
  config.set_flags(cppast::cpp_standard::cpp_latest, flags);
  config.fast_preprocessing(true);
}

bool Parser::TryParse(const std::vector<std::string>& file_list, cppast::libclang_compilation_database* compile_database) {
  // create the parser.
  cppast::cpp_entity_index index;
  cppast::libclang_parser parser(type_safe::ref(*logger_));

  std::mutex mutex;
  bool result = true;

  // batch jobs in parallel.
  ThreadPool pool(ParseThreadCount);
  for (auto& file_entry : file_list) {
    standardese_tool::add_job(pool, [&, file_entry] {
      type_safe::optional<cppast::libclang_compile_config> override_config = type_safe::nullopt;

      if (compile_database) {
        override_config = cppast::find_config_for(*compile_database, file_entry);
      }

      auto actual_config = override_config.value_or(clang_config_);
      auto parsed_file = parser.parse(index, file_entry, actual_config);

      std::lock_guard<std::mutex> lock(mutex);
      if (parsed_file)
        parsed_files_.push_back(std::move(parsed_file));
      else
        result = false;
    });
  }

  return result;
}

void Parser::TraverseFiles() {
  ThreadPool pool(ParseThreadCount);
  for (auto& it : parsed_files_) {
    standardese_tool::add_job(pool, [&] { DoTraverse(*it); });
  }
}

void Parser::DoTraverse(cppast::cpp_file& file) {
}
}  // namespace refl