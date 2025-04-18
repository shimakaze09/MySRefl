#pragma once

#include "Attr.h"
#include "ElemList.h"

namespace Smkz::MySRefl {
template <typename... Attrs>
struct AttrList : ElemList<Attrs...> {
  constexpr AttrList(Attrs... attrs) : ElemList<Attrs...>{attrs...} {}
};
}  // namespace Smkz::MySRefl
