//
// Created by Admin on 11/03/2025.
//

#include <MySRefl/MySRefl.h>

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
struct My::MySRefl::TypeInfo<Color> : My::MySRefl::TypeInfoBase<Color> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr char name[6] = "Color";
#endif
  static constexpr AttrList attrs = {
      Attr{"enum_attr", "enum_attr_value"},
  };
  static constexpr FieldList fields = {
      Field{"RED", Type::RED,
            AttrList{
                Attr{"enumerator_attr", "enumerator_attr_value"},
                Attr{"func", &Func<1>},
            }},
      Field{"GREEN", Type::GREEN,
            AttrList{
                Attr{"func", &Func<2>},
            }},
      Field{"BLUE", Type::BLUE,
            AttrList{
                Attr{"func", &Func<3>},
            }},
  };
};

int main() {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  cout << TypeInfo<Color>::name << endl;
#endif

  TypeInfo<Color>::fields.ForEach(
      [](auto field) { cout << field.name << endl; });

  static_assert(
      MySRefl_ElemList_GetByValue(TypeInfo<Color>::fields, "RED").value ==
      Color::RED);
  static_assert(
      MySRefl_ElemList_GetByValue(TypeInfo<Color>::fields, Color::RED).name ==
      "RED");

  constexpr Color c = Color::GREEN;
  constexpr auto c_attr =
      MySRefl_ElemList_GetByValue(TypeInfo<Color>::fields, c).attrs;
  static_assert(MySRefl_ElemList_GetByValue(c_attr, "func").value() == 2);
}
