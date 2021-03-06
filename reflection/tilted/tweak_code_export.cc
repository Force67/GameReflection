// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
// Purpose: Source file exporter for Tweak-Database spec.

#include "tilted/tweak_database.h"
#include "tilted/tweak_code_export.h"

namespace refl {
namespace {

class RenderScope {
 public:
  RenderScope(llvm::raw_ostream& os)
      : os_(os) {
    os << "{\n";
  }

  ~RenderScope() {
    os_ << "}\n";
  }

 private:
  llvm::raw_ostream& os_;
};

constexpr char kHeaderComment[] =
    "//Copyright (C) Force67 2021.\n"
    "//This file was auto generated by gamerefl v0\n";

constexpr char kDirectoryHeaderTemplate[] =
    R"(struct ReflSymbolRecord {
  const char* const name;
  const void* ptr;
} registry[{}] = {
)";
}  // namespace

TweakCodeExporter::TweakCodeExporter(llvm::raw_ostream& os)
    : os_(os) {
}

TweakCodeExporter::~TweakCodeExporter() {
}

void TweakCodeExporter::Render(const TweakDatabase& tweak_info) {
  const auto& symbol_list = tweak_info.attribute_list();

  os_ << kHeaderComment << "#pragma once\n\n";

  for (const auto& r : symbol_list) {
    os_ << "extern " << r.name << r.signature << ";\n";
  }

  // wrap the registry in a private namespace
  // so symbols become local to object file
  os_ << "namespace {\n"
      << fmt::format(kDirectoryHeaderTemplate, symbol_list.size());

  for (const auto& r : symbol_list) {
    os_ << "{" << r.name << ",&" << r.name << ";\n";
  }

  os_ << "};\n}\n  // namespace";
}
}  // namespace refl