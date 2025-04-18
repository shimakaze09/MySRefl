#pragma once

#include "Base.h"
#include "ElemList.h"

namespace Smkz::MySRefl {
template <typename... Bases>
struct BaseList : ElemList<Bases...> {
  constexpr BaseList(Bases... bases) : ElemList<Bases...>{bases...} {};
};
}  // namespace Smkz::MySRefl
