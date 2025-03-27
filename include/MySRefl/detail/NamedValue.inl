//
// Created by Admin on 27/03/2025.
//

#pragma once

#include <string_view>
#include <utility>

namespace My::MySRefl::detail {
template <typename Str>
struct MYSTRSizeof;

template <typename Char>
struct MYSTRSizeof<const std::basic_string_view<Char>&> {
  static constexpr size_t get(
      const std::basic_string_view<Char>& str) noexcept {
    return str.size();
  }
};

template <typename Char, size_t N>
struct MYSTRSizeof<const Char (&)[N]> {
  static constexpr size_t get(const Char (&str)[N]) noexcept { return N - 1; }
};

template <typename Char, typename T, size_t... N>
constexpr decltype(auto) MYSTRHelperImpl(std::index_sequence<N...>) {
  return std::integer_sequence<Char, T::get()[N]...>();
}

template <typename T>
constexpr decltype(auto) MySTRHelper(T) {
  using Char = std::decay_t<decltype(T::get()[0])>;
  return MYSTRHelperImpl<Char, T>(
      std::make_index_sequence<MYSTRSizeof<decltype(T::get())>::template get(
          T::get())>());
}
}  // namespace My::MySRefl::detail
