#pragma once

namespace refl {
class FileCollection {
 public:
  FileCollection();

  enum class Provider { 
	  DirectorySeek,
	  VisualStudio,
	  CompileCommands,
  };

  bool SearchFiles();
  void RemoveFile(const std::string& file);

  auto& GetList() { return file_list_; }

  static bool HasPathDirective();
 private:
  static Provider MapActiveProvider();

 private:
  std::vector<std::string> file_list_;
};
}  // namespace refl