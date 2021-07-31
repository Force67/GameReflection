#pragma once

namespace refl {

class Logger final : public cppast::diagnostic_logger {
 public:
  explicit Logger(bool force_silent) : force_silence_(force_silent){};

  using diagnostic_logger::diagnostic_logger;

 private:
  bool do_log(const char* source, const cppast::diagnostic& d) const override;

 private:
  bool force_silence_ = false;
};

std::unique_ptr<refl::Logger> CreateLogger();
}