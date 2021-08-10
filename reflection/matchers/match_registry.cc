// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "match_registry.h"
#include "matcher_base.h"
#include "parser.h"

#include "tilted/register_tp_matchers.h"
#include "utils/thread_pool.h"

namespace refl {

static cl::opt<uint32_t> AnalysisTC("match-thread-count",
                                    cl::desc("How many threads to use while matching"),
                                    cl::value_desc("thread-count"),
                                    cl::init(10u));

MatchRegistry::MatchRegistry() {
  RegisterTPMatchers(*this);
}

void MatchRegistry::Add(std::unique_ptr<MatcherBase> match) {
  registry_.push_back(std::move(match));
}

void MatchRegistry::FindAllMatchersOfDomain(llvm::StringRef domain_name,
                                            std::vector<MatcherBase*>& out) {
  for (auto i = registry_.begin(), toofar = registry_.end(); i != toofar; ++i)
    if ((*i)->GetDomainName() == domain_name)
      out.push_back(i->get());
}

void MatchRegistry::InvokeMatcher(MatcherBase& m, const cppast::cpp_entity& e) {
  MatcherBase::FileContext local_context;
  // TODO: invoke a match pass for the whole file.
  if (m.MatchRules(e, MatcherBase::Phase::kFirstPass)) {
    m.CollectEntity(e);
  }
}

void MatchRegistry::DoMatchMT(Parser& parse_info) {
  // instantiate a new thread pool
  ThreadPool pool(AnalysisTC);
  for (const std::unique_ptr<cppast::cpp_file>& it :
       parse_info.file_collection()) {
    const cppast::cpp_file& current_file = *it;

    // for each analyzed file, we query one job to the pool
    standardese_tool::add_job(pool, [&] {
      cppast::visit(current_file, [&](const cppast::cpp_entity& entity, const cppast::visitor_info& info) {
        if (info.event == cppast::visitor_info::container_entity_exit)
          // entity already handled
          return true;
        else if (!cppast::is_templated(entity) && !cppast::is_friended(entity)) {
          for (const auto& m : registry_) {
            InvokeMatcher(*m, entity);

            // TODO: allow thread local storage passed via param
            if (m->Match(entity, MatcherBase::Phase::kFirstPass)) {
              stats_.entity_match_count++;

              m->CollectEntity(entity);
            }
          }
        }
        return true;
      });

      // TODO: this is rather tacky. refine this for an universal
      // second process.
      auto comments = current_file.unmatched_comments();
      if (comments.size().get() > 0) {
        for (const auto& m : registry_) {
          if (m->MatchRules(current_file, MatcherBase::Phase::kSecondPass)) {
            m->CollectEntity(current_file);
          }
        }
      }
    });
  }
}

}  // namespace refl