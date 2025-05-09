#pragma once

#include "TypeInfo.h"

namespace Smkz::MySRefl {
template <typename T, bool IsVirtual = false>
struct Base {
  static constexpr auto info = TypeInfo<T>{};
  static constexpr bool is_virtual = IsVirtual;
};
}  // namespace Smkz::MySRefl
