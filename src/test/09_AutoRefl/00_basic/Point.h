struct [[size(8)]] Point {
  [[not_serialize]]
  float x;
  [[info("hello")]]
  float y;
};

// CMake will generate this file before building
// it contains the reflection declaration of Point
#include "Point_AutoRefl.inl"

// This file is generated by My::MySRefl::AutoRefl

// #pragma once

// #include <MySRefl/MySRefl.h>

// template <>
// struct My::MySRefl::TypeInfo<Point> : My::MySRefl::TypeInfoBase<Point> {
//   static constexpr AttrList attrs = {
//       Attr{"size", 8},
//   };

//   static constexpr FieldList fields = {
//       Field{"x", &Point::x,
//             AttrList{
//                 Attr{"not_serialize"},
//             }},
//       Field{"y", &Point::y,
//             AttrList{
//                 Attr{"info", "hello"},
//             }},
//   };
// };
