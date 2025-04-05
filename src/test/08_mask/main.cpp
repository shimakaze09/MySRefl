//
// Created by Admin on 11/03/2025.
//

#include <MySRefl/MySRefl.h>

#include <array>
#include <iostream>

using namespace My::MySRefl;
using namespace std;

struct [[size(8)]] Point {
  [[not_serialize]]
  float x;
  [[info("hello")]]
  float y;
  float z;
};

template <>
struct My::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
  static constexpr AttrList attrs = {
      Attr{TSTR("size"), 8},
  };
  static constexpr FieldList fields = {
      Field{TSTR("x"), &Type::x,
            AttrList{
                Attr{TSTR("not_serialize")},
            }},
      Field{TSTR("y"), &Type::y,
            AttrList{
                Attr{TSTR("info"), "hello"},
            }},
      Field{TSTR("z"), &Type::z},
  };
};

template <typename T, std::size_t... Ns>
constexpr auto GetXZ(std::index_sequence<Ns...>) {
  // get fields with name "x" or "z"
  constexpr auto masks = TypeInfo<T>::fields.Accumulate(
      std::array<bool, TypeInfo<T>::fields.size>{},
      [&, idx = 0](auto acc, auto field) mutable {
        acc[idx] = field.name == "x" || field.name == "z";
        idx++;
        return acc;
      });
  constexpr auto fields = TypeInfo<T>::fields.Accumulate<masks[Ns]...>(
      ElemList<>{}, [&](auto acc, auto field) { return acc.Push(field); });
  return fields;
}

template <typename T>
constexpr auto GetXZ() {
  return GetXZ<T>(std::make_index_sequence<TypeInfo<T>::fields.size>{});
}

int main() {
  // get fields with name "x" or "z"
  constexpr auto fields = GetXZ<Point>();
  fields.ForEach([](auto field) { cout << field.name << endl; });
}
