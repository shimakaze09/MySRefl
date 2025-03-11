//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "NamedValue.h"

#include <tuple>

namespace My::MySRefl {
// Elems is named value
template <typename... Elems>
struct ElemList {
  std::tuple<Elems...> elems;
  static constexpr size_t size = sizeof...(Elems);

  constexpr ElemList(Elems... elems) : elems{elems...} {}

  template <bool... masks, typename Acc, typename Func>
  constexpr auto Accumulate(Acc&& acc, Func&& func) const;

  template <bool... masks, typename Func>
  constexpr void ForEach(Func&& func) const;

  template <typename Func>
  constexpr size_t FindIf(Func&& func) const;

  constexpr size_t Find(std::string_view name) const;

  template <typename T>
  constexpr size_t FindValue(T value) const;

  constexpr bool Contains(std::string_view name) const;

  template <size_t N>
  constexpr auto Get() const;

  template <typename Elem>
  constexpr auto Push(Elem e) const;

  template <typename Elem>
  constexpr auto Insert(Elem e) const;

  // name must be constexpr std::string_view / const char[N]
  // C++20 support string literal as template arguments
#define MySRefl_ElemList_GetByName(list, name) list.Get<list.Find(name)>()

  // value must be constexpr
  // C++20 support string literal as template arguments
#define MySRefl_ElemList_GetByValue(list, value) \
  list.Get<list.FindValue(value)>()
};
}  // namespace My::MySRefl

#include "detail/ElemList.inl"
