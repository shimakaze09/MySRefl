#pragma once

namespace Smkz::MySRefl {
template <typename Signature>
constexpr auto WrapConstructor();
template <typename T>
constexpr auto WrapDestructor();
}  // namespace Smkz::MySRefl

#include "detail/Util.inl"
