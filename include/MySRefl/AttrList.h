#pragma once

#include "Attr.h"
#include "ElemList.h"

namespace My::MySRefl {
template <typename... Attrs>
struct AttrList : ElemList<Attrs...> {
  constexpr AttrList(Attrs... attrs) : ElemList<Attrs...>{attrs...} {}
};
}  // namespace My::MySRefl
