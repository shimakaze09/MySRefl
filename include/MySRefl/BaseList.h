//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "Base.h"
#include "ElemList.h"

namespace My::MySRefl {
template <typename... Bases>
struct BaseList : ElemList<Bases...> {
  constexpr BaseList(Bases... bases) : ElemList<Bases...>{bases...} {};
};
}  // namespace My::MySRefl
