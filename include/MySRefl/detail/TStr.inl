//
// Created by Admin on 28/03/2025.
//

#pragma once

#include <string_view>
#include <utility>

namespace My {
template <typename Char, Char... chars>
struct TStr;
}

namespace My::detail {
template <typename STR>
struct TSTRSizeof;

template <typename Char>
struct TSTRSizeof<const std::basic_string_view<Char>&> {
  static constexpr size_t get(
      const std::basic_string_view<Char>& str) noexcept {
    return str.size();
  }
};

template <typename Char, size_t N>
struct TSTRSizeof<const Char (&)[N]> {
  static constexpr size_t get(const Char (&str)[N]) noexcept { return N - 1; }
};

template <typename Char, typename T, size_t... N>
constexpr decltype(auto) TSTRHelperImpl(std::index_sequence<N...>) {
  return TStr<Char, T::get()[N]...>{};
}

template <typename T>
constexpr decltype(auto) TSTRHelper(T) {
  using Char = std::decay_t<decltype(T::get()[0])>;
  return TSTRHelperImpl<Char, T>(
      std::make_index_sequence<TSTRSizeof<decltype(T::get())>::template get(
          T::get())>());
}
}  // namespace My::detail
