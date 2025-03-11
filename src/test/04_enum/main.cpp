//
// Created by Admin on 11/03/2025.
//

#include <MySRefl.h>

#include <iostream>

#include <cassert>

using namespace My::MySRefl;
using namespace std;

template <size_t N>
constexpr size_t Func() {
  return N;
}

enum class [[enum_attr("enum_attr_value")]] Color {
  RED [[enumerator_attr("enumerator_attr_value"), func(&Func<1>)]] = 1,
  GREEN [[func(&Func<2>)]] = 2,
  BLUE [[func(&Func<3>)]] = 4
};

template <>
struct TypeInfo<Color> : TypeInfoBase<Color> {
  static constexpr std::string_view name = "Color";

  static constexpr FieldList fields = {
      Field{"RED", Color::RED,
            AttrList{Attr{"enumerator_attr", "enumerator_attr_value"},
                     Attr{"func", &Func<1>}}},
      Field{"GREEN", Color::GREEN, AttrList{Attr{"func", &Func<2>}}},
      Field{"BLUE", Color::BLUE, AttrList{Attr{"func", &Func<3>}}}};

  static constexpr AttrList attrs = {Attr{"enum_attr", "enum_attr_value"}};
};

int main() {
  TypeInfo<Color>::fields.ForEach(
      [](auto field) { cout << field.name << endl; });
  constexpr size_t idx = TypeInfo<Color>::fields.Find("RED");
  constexpr size_t idx2 = TypeInfo<Color>::fields.FindByValue(Color::RED);
  static_assert(
      TypeInfo<Color>::fields.Get<TypeInfo<Color>::fields.Find("RED")>()
          .value == Color::RED);
  static_assert(TypeInfo<Color>::fields
                    .Get<TypeInfo<Color>::fields.FindByValue(Color::RED)>()
                    .name == "RED");

  constexpr Color c = Color::GREEN;
  constexpr auto c_attr =
      TypeInfo<Color>::fields.Get<TypeInfo<Color>::fields.FindByValue(c)>()
          .attrs;
  static_assert(c_attr.Get<c_attr.Find("func")>().value() == 2);
}
