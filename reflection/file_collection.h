#pragma once

namespace refl {
class FileCollection {
 public:
  FileCollection();

  void SearchFiles();

  auto& GetList() { return file_list_; }

  static bool HasPathDirective();
 private:

 private:
  std::vector<std::string> file_list_;
};
}  // namespace refl