// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.
#pragma once
#include <vector>

namespace refl {

class RlDatabase {
 public:
  // add/remove to storage
  void AddSymbol(const std::string& name, const std::string* signature);
  void ExcludeFile(const std::string& name);

  void WriteCxxHeader();
  void UpdateJsonReport();
 private:
  std::vector<std::string> file_exclusion_list_;

  struct Node {
    Node(std::string name, std::string s) : 
        name(std::move(name)), signature(std::move(s)) {}

    std::string name;
    std::string signature;
  };
  std::vector<Node> storage_;
};
}
#include "rl_database.inl"