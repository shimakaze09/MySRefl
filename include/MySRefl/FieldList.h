#pragma once

#include "ElemList.h"
#include "Field.h"
#include "Util.h"

namespace My::MySRefl {
// Field's (name, value_type) must be unique
template <typename... Fields>
struct FieldList : ElemList<Fields...> {
  constexpr FieldList(Fields... fields) : ElemList<Fields...>{fields...} {};
};
}  // namespace My::MySRefl
