//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <string_view>
#include "NamedValue.h"

namespace My::MySRefl {
// [summary]
// attribute for field, class, enum
// [member]
// std::string_view name
// T value (T == void -> no value)
template <typename T, typename Char, Char... chars>
struct Attr;

template <size_t N, typename Char, Char... chars>
Attr(std::integer_sequence<Char, chars...>, const char (&)[N])
    -> Attr<std::string_view, Char, chars...>;

template <typename Char, Char... chars>
Attr(std::integer_sequence<Char, chars...>) -> Attr<void, Char, chars...>;
}  // namespace My::MySRefl

#include "detail/Attr.inl"
