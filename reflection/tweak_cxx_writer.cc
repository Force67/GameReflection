// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tweak_cxx_writer.h".h"

namespace retk {

namespace {

}

bool WriteTweaksCXXRegistry(const TweaksDatabase &db, const std::string& file_path) {
  std::string tweaks_data =
      "//Copyright (C) Force67 2021.\n"
      "This file is auto generated. Do not edit.";

  std::unique_ptr<WritableMemoryBuffer> buffer = 
	  WritableMemoryBuffer::getNewMemBuffer(tweaks_data.size());

  if (buffer) {
    std::memcpy(buffer->getBufferStart(), 
        tweaks_data.data(), tweaks_data.size());

    return true;
  }

  return false;
}
}  // namespace retk