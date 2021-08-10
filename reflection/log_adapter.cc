// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#include "log_adapter.h"

namespace refl {

std::unique_ptr<refl::Logger> CreateLogger() {
  auto logger = std::make_unique<refl::Logger>(false);
#if defined(REFL_DEBUG)
  logger->set_verbose(true);
#endif
  return logger;
}

bool Logger::do_log(const char* source, const cppast::diagnostic& d) const {
  if (force_silence_)
    return false;

  auto loc = d.location.to_string();
  if (loc.empty())
    fmt::print("[{}] [{}] {}\n", source, to_string(d.severity), d.message);
  else
    fmt::print("[{}] [{}] {} {}\n", source, to_string(d.severity), d.location.to_string(), d.message);

  return true;
}
}