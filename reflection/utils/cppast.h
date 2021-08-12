// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
// Purpose: simple extensions for comfort
#pragma once

namespace cppast {
// unchecked conversion
// this shall be used only for cases where the type is clear
// such as in switch cases.
template <typename T>
inline const T& asa(const cpp_entity& entity) {
  return static_cast<const T&>(entity);
}
// checked conversion
template <typename T>
inline const T& asa_checked(const cpp_entity& entity) {
  if (entity.kind() != T.kind()) {
    __debugbreak();
  }

  return static_cast<const T&>(entity);
}
}  // namespace cppast