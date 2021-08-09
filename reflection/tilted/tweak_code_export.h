// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
// Purpose: Source file exporter for Tweak-Database spec.
#pragma once

namespace refl {
class TweakDatabase;

class TweakCodeExporter {
 public:
  TweakCodeExporter(llvm::raw_ostream&);
  ~TweakCodeExporter();

  void Render(const TweakDatabase& findings);

 private:
  llvm::raw_ostream& os_;
};
}  // namespace refl