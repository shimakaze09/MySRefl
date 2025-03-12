//
// Created by Admin on 11/03/2025.
//

#pragma once

namespace My::MySRefl {
template <typename Signature>
constexpr auto WrapConstructor();
template <typename T>
constexpr auto WrapDestructor();
}  // namespace My::MySRefl

#include "detail/Util.inl"
