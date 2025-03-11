//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "NamedValue.h"

namespace My::MySRefl {
// [summary]
// attribute for field, class, enum
// [member]
// std::string_view name
// T value (T == void -> no value)
template <typename T>
struct Attr;

template <size_t N>
Attr(std::string_view, const char (&)[N]) -> Attr<std::string_view>;

Attr(std::string_view) -> Attr<void>;
}  // namespace My::MySRefl

#include "detail/Attr.inl"
