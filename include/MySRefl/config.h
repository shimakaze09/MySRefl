//
// Created by Admin on 27/03/2025.
//

#pragma once

#ifndef MY_MYSREFL_NOT_USE_NAMEOF
#define MY_MYSREFL_USE_NAMEOF
#endif

// AutoRefl will use these names
namespace MyMeta  {
static constexpr char nameof_namespace[] = "_MySRefl";

static constexpr char initializer[] = "MyMeta_initializer";
static constexpr char default_functions[] = "MyMeta_default_functions";
static constexpr char constructor[] = "MyMeta_constructor";
static constexpr char destructor[] = "MyMeta_destructor";

static constexpr char nameof_initializer[] = "initializer";
static constexpr char nameof_default_functions[] = "default_functions";
static constexpr char nameof_constructor[] = "constructor";
static constexpr char nameof_destructor[] = "destructor";
}  // namespace _MySRefl
