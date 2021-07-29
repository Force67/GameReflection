// Copyright (C) Force67 <github.com/Force67>.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include <vector>

namespace refl {

class SymbolTable {
 public:
  SymbolTable(const std::string& json_name, const std::string& header_name);

  void AddSymbol(const std::string& name, const std::string* signature);

  void ExportJson();
  void ExportHookHeader();
 private:
  std::string json_name_;
  std::string header_name_;

  struct Node {
    Node(std::string name, std::string s) : 
        name(std::move(name)), signature(std::move(s)) {}

    std::string name;
    std::string signature;
  };
  std::vector<Node> storage_;
};
}