// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "match_maker.h"
#include "matcher_base.h"
#include "parser.h"

#include "tilted/register_tp_matchers.h"
#include "utils/thread_pool.h"

namespace refl {

static cl::opt<uint32_t> AnalysisTC("match-thread-count",
                                    cl::desc("How many threads to use while matching"),
                                    cl::value_desc("thread-count"),
                                    cl::init(10u));

MatchMaker::MatchMaker() {
  RegisterTPMatchers(*this);
}

void MatchMaker::Add(std::unique_ptr<MatcherBase> match) {
  registry_.push_back(std::move(match));
}

void MatchMaker::ResultsByID(uintptr_t id,
                                std::vector<const cppast::cpp_entity*>& results) {
  for (auto& it : analysis_results_) {
    if (it.first == id)
      results.push_back(it.second);
  }
}

void MatchMaker::DoMatchMT(Parser& parse_info) {
  // instantiate a new thread pool
  ThreadPool pool(AnalysisTC);
  for (const std::unique_ptr<cppast::cpp_file>& it :
       parse_info.file_collection()) {
    const cppast::cpp_file& current_file = *it;

    // for each analyzed file, we query one job to the pool
    standardese_tool::add_job(pool, [&] {
      // for thread safety we model our data model to store local data on a per thread basis.
      MatcherBase::LocalContext thread_context{};
      results_map_t local_map;

      cppast::visit(current_file, [&](const cppast::cpp_entity& entity, const cppast::visitor_info& info) {
        if (info.event == cppast::visitor_info::container_entity_exit)
          return true;
        else if (!cppast::is_templated(entity) && !cppast::is_friended(entity)) {
          for (const auto& m : registry_) {
            if (m->Run(thread_context, entity, MatcherBase::Phase::kFirstPass)) {
              local_map.insert(std::make_pair((uintptr_t)m->GetID(), &entity));
            }
          }
        }
        return true;
      });

      auto comments = current_file.unmatched_comments();
      if (comments.size().get() > 0) {
        for (const auto& m : registry_) {
          if (m->Run(thread_context, current_file, MatcherBase::Phase::kSecondPass)) {
            local_map.insert(std::make_pair((uintptr_t)m->GetID(), &current_file));
          }
        }
      }

      // merge thread_locale with global map
      {
        std::lock_guard guard{merge_lock_};
        for (auto& pair : local_map) {
          analysis_results_.insert(std::move(pair));
        }
      }
    });
  }
}

}  // namespace refl