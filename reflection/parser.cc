// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "parser.h"
#include "rl_database.h"
#include "thread_pool.h"
#include "file_collection.h"

namespace refl {

namespace {
// tweak able values from command line.
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
cl::opt<uint32_t> ParseThreadCount("parse-thread-count",
                                   cl::desc("How many threads to use while parsing"),
                                   cl::value_desc("thread-count"),
                                   cl::init(10u));

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

bool IsPrivateFile(const std::string& comment) {
  return comment.find(HideFileMarker) != std::string::npos;
}
}  // namespace

Parser::Parser() : logger_{CreateLogger()} {
  cppast::compile_flags flags;
#if defined(OS_WIN)
  flags |= cppast::compile_flag::ms_extensions;
  flags |= cppast::compile_flag::ms_compatibility;
#endif
  clang_config_.set_flags(cppast::cpp_standard::cpp_latest, flags);
  clang_config_.fast_preprocessing(true);
}

// TODO: use LLVM containers.
bool Parser::TryParseFiles(FileCollection& collection) {
  auto& list = collection.GetList();

  cppast::cpp_entity_index index;
  cppast::libclang_parser parser(type_safe::ref(*logger_));

  std::mutex mutex;
  bool result = true;

  // multi threaded parsing.
  ThreadPool pool(ParseThreadCount);
  for (auto& file_entry : list) {
    standardese_tool::add_job(pool, [&, file_entry] {
      auto parsed_file = parser.parse(index, file_entry, clang_config_);

      std::lock_guard<std::mutex> lock(mutex);
      if (parsed_file)
        parsed_files_.push_back(std::move(parsed_file));
      else
        result = false;
    });
  }

  return result;
}

void Parser::TraverseFiles(RlDatabase& store) {
  ThreadPool pool(ParseThreadCount);
  for (auto& it : parsed_files_) {
    standardese_tool::add_job(pool, [&] { DoTraverse(store, *it); });
  }
}

void Parser::DoTraverse(RlDatabase& store, cppast::cpp_file& file) {
  std::mutex mutex;
  cppast::visit(file, [&](const cppast::cpp_entity& entity, const cppast::visitor_info& info) {
    if (info.event == cppast::visitor_info::container_entity_exit)
      // entity already handled
      return true;
    else if (!cppast::is_templated(entity) && !cppast::is_friended(entity)) {
      switch (entity.kind()) {
        case cppast::cpp_entity_kind::member_function_t: {
          auto& func = static_cast<const cppast::cpp_member_function&>(entity);
          if (IsReflective(func)) {
            std::lock_guard<std::mutex> lock(mutex);

            std::string fullName = PrettyFormatClassMember(func);
            std::string sig = func.signature();
            store.AddSymbol(fullName, &sig);
          }
          break;
        }
        case cppast::cpp_entity_kind::function_t: {
          auto& func = static_cast<const cppast::cpp_function&>(entity);
          if (IsReflective(func)) {
            std::lock_guard<std::mutex> lock(mutex);

            store.AddSymbol(func.name(), &func.signature());
          }
          break;
        }
        case cppast::cpp_entity_kind::variable_t: {
          auto& var = static_cast<const cppast::cpp_variable&>(entity);
          if (IsReflective(var)) {
            std::lock_guard<std::mutex> lock(mutex);

            store.AddSymbol(var.name(), nullptr);
          }
          break;
        }
        default:
          break;
      }
    }

    return true;
  });

  // the first comment of the file must contain the command.
  auto comments = file.unmatched_comments();
  if (comments.size().get() > 0) {
    if (IsPrivateFile(comments.data()->content)) {
      std::lock_guard<std::mutex> lock(mutex);

      store.ExcludeFile(file.name());
    }
  }
}
}  // namespace refl