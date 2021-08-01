// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include <vector>

namespace refl {

class Store {
 public:
  void AddSymbol(const std::string& name, const std::string* signature);
  void RemoveFile(const std::string& name);

  void ExportJson();
  void ExportHookHeader();
 private:
  struct Node {
    Node(std::string name, std::string s) : 
        name(std::move(name)), signature(std::move(s)) {}

    std::string name;
    std::string signature;
  };
  std::vector<Node> storage_;
  std::vector<std::string> files_;
};
}