
#include "rl_database.h"

namespace refl {
void RlDatabase::AddSymbol(const std::string& name, const std::string* signature) {
  storage_.emplace_back(std::move(name), signature ? std::move(*signature) : "");
}

void RlDatabase::ExcludeFile(const std::string& name) {
  file_exclusion_list_.push_back(name);
}
}  // namespace refl