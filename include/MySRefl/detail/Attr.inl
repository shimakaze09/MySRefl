//
// Created by Admin on 11/03/2025.
//

#pragma once

namespace My::MySRefl {
template <typename T>
struct Attr : NamedValue<T> {
  constexpr Attr(std::string_view name, T value) : NamedValue<T>{name, value} {}
};

template <>
struct Attr<void> : NamedValue<void> {
  constexpr Attr(std::string_view name) : NamedValue<void>{name} {}
};
}  // namespace My::MySRefl
