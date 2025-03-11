//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "Base.h"
#include "ElemList.h"

// #include "detail/Util.h"

namespace My::MySRefl {
template <typename... Bases>
struct BaseList : ElemList<Bases...> {
  // static_assert((detail::IsInstance<Bases, Base>::value&&...));
  constexpr BaseList(Bases... bases) : ElemList<Bases...>{bases...} {};
};
}  // namespace My::MySRefl
