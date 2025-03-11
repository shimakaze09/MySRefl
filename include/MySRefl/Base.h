//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "TypeInfo.h"

namespace My::MySRefl {
template <typename T, bool IsVirtual = false>
struct Base {
  static constexpr auto info = TypeInfo<T>{};
  static constexpr bool is_virtual = IsVirtual;
};
}  // namespace My::MySRefl
