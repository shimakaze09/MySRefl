//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "AttrList.h"
#include "NamedValue.h"
#include "Util.h"
#include "detail/FieldTraits.h"

namespace My::MySRefl {
template <typename Name, typename T, typename AList>
struct Field : detail::FieldTraits<T>, NamedValue<Name, T> {
  static_assert(detail::IsInstance<AList, AttrList>::value);
  static_assert(!std::is_void_v<T>);

  AList attrs;

  constexpr Field(Name name, T value, AList attrs = {})
      : NamedValue<Name, T>{name, value}, attrs{attrs} {}
};

template <typename Name, typename T, typename AList>
Field(Name, T, AList) -> Field<Name, T, AList>;
template <typename Name, typename T>
Field(Name, T) -> Field<Name, T, AttrList<>>;
}  // namespace My::MySRefl
