//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "BaseList.h"
#include "_deps/nameof.hpp"

namespace My::MySRefl {
template <typename T, typename... Bases>
struct TypeInfoBase {
  using type = T;
  static constexpr std::string_view name = nameof::nameof_type<T>();
  static constexpr BaseList bases = {Bases{}...};

  template <typename Derived>
  static constexpr auto&& Forward(Derived&& derived) noexcept;

  static constexpr auto VirtualBases();

  template <typename Func, size_t Depth = 0>
  static constexpr void DFS(Func&& func);

  template <typename U, typename Func>
  static constexpr void ForEachVarOf(U&& obj, Func&& func);
};
}  // namespace My::MySRefl

#include "detail/TypeInfoBase.inl"
