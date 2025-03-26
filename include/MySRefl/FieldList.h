//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "ElemList.h"
#include "Field.h"
#include "Util.h"

namespace My::MySRefl {
// Field's (name, value_type) must be unique
template <typename... Fields>
struct FieldList : ElemList<Fields...> {
  // static_assert((detail::IsInstance<Fields, Field>::value && ...));
  constexpr FieldList(Fields... fields) : ElemList<Fields...>{fields...} {};
};
}  // namespace My::MySRefl
