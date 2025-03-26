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
static constexpr char constructor[] = "__constructor";
static constexpr char destructor[] = "__destructor";
static constexpr char default_value[] = "__default_value";
static constexpr char name[] = "__name";  // function argument name
// compile time
#define MY_MYSREFL_NAME_ARG(x) "__arg_" #x
}  // namespace Name
}  // namespace My::MySRefl

#include "detail/Util.inl"
