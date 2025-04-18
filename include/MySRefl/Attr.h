#pragma once

#include "NamedValue.h"

namespace My::MySRefl {
// [summary]
// attribute for field, class, enum
// [member]
// T value (T == void -> no value)
template <typename Name, typename T>
struct Attr;

template <typename Name>
Attr(Name) -> Attr<Name, void>;

template <typename Name, typename Value>
Attr(Name, Value) -> Attr<Name, Value>;
}  // namespace My::MySRefl

#include "detail/Attr.inl"
