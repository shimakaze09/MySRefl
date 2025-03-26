//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "BaseList.h"

#ifdef MY_MYSREFL_NOT_USE_NAMEOF
#include <nameof.hpp>
#endif

namespace My::MySRefl {
template <typename T, typename... Bases>
struct TypeInfoBase {
  using Type = T;
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr std::string_view name = nameof::nameof_type<T>();
#endif
  static constexpr BaseList bases = {Bases{}...};

  template <typename Derived>
  static constexpr auto&& Forward(Derived&& derived) noexcept;

  static constexpr auto VirtualBases();

  template <typename Init, typename Func>
  static constexpr auto DFS_Acc(Init&& init, Func&& func);

  template <typename Func>
  static constexpr void DFS_ForEach(Func&& func);

  template <typename U, typename Func>
  static constexpr void ForEachVarOf(U&& obj, Func&& func);
};
}  // namespace My::MySRefl

#include "detail/TypeInfoBase.inl"
