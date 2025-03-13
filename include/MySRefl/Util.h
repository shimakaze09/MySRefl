//
// Created by Admin on 11/03/2025.
//

#pragma once

namespace My::MySRefl {
template <typename Signature>
constexpr auto WrapConstructor();
template <typename T>
constexpr auto WrapDestructor();

namespace Name {
static constexpr const char constructor[] = "__constructor";
static constexpr const char destructor[] = "__destructor";
static constexpr const char default_value[] =
    "__default_value";  // function argument default value
static constexpr const char name[] = "__name";  // function argument name
// compile time
#define MY_MYSREFL_NAME_ARG(x) "__arg_" #x
}  // namespace Name
}  // namespace My::MySRefl

#include "detail/Util.inl"
