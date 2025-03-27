//
// Created by Admin on 11/03/2025.
//

#pragma once

namespace My::MySRefl {
template <typename Name, typename T>
struct Attr : NamedValue<Name, T> {
  constexpr Attr(Name name, T value) : NamedValue<Name, T>{name, value} {}
};

template <typename Name>
struct Attr<Name, void> : NamedValue<Name, void> {
  constexpr Attr(Name name) : NamedValue<Name, void>{name} {}
};
}  // namespace My::MySRefl
