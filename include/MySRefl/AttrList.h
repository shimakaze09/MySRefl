//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "ElemList.h"

#include "Attr.h"

namespace My::MySRefl {
template <typename... Attrs>
struct AttrList : ElemList<Attrs...> {
  // static_assert((detail::IsInstance<Attrs, Attr>::value && ...));

  constexpr AttrList(Attrs... attrs) : ElemList<Attrs...>{attrs...} {}
};
}  // namespace My::MySRefl
