//
// Created by Admin on 11/03/2025.
//

#include <MySRefl/MySRefl.h>

#include <iostream>

using namespace My::MySRefl;
using namespace std;

template <typename T>
struct [[size(8)]] Point {
  [[not_serialize]]
  T x;
  [[info("hello")]]
  T y;
};

template <typename T>
struct My::MySRefl::TypeInfo<Point<T>> : TypeInfoBase<Point<T>> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  // [!] all instance types have the same name
  static constexpr char name[6] = "Point";
#endif
  static constexpr AttrList attrs = {
      Attr{MYSTR("size"), 8},
  };
  static constexpr FieldList fields = {
      Field{MYSTR("x"), &Point<T>::x,
            AttrList{
                Attr{MYSTR("not_serialize")},
            }},
      Field{MYSTR("y"), &Point<T>::y,
            AttrList{
                Attr{MYSTR("info"), "hello"},
            }},
  };
};

int main() {
  Point<float> p{1, 2};

  TypeInfo<Point<float>>::fields.ForEach([](auto field) {
    cout << field.name << endl;
    field.attrs.ForEach([](auto attr) {
      cout << "name  : " << attr.name << endl;
      if constexpr (attr.has_value)
        cout << "value : " << attr.value << endl;
    });
  });

  constexpr auto y_field = TypeInfo<Point<float>>::fields.Find(MYSTR("y"));
  static_assert(y_field.name == "y");

  static_assert(TypeInfo<Point<float>>::fields.Contains(MYSTR("x")));

  TypeInfo<Point<float>>::attrs.ForEach([](auto attr) {
    cout << "name  : " << attr.name << endl;
    if constexpr (attr.has_value)
      cout << "value : " << attr.value << endl;
  });

  TypeInfo<Point<float>>::ForEachVarOf(p, [](auto field, auto&& var) {
    cout << field.name << " : " << var << endl;
  });
}
