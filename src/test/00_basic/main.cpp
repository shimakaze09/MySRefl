//
// Created by Admin on 11/03/2025.
//

#include <MySRefl/MySRefl.h>

#include <iostream>

using namespace My::MySRefl;
using namespace std;

struct [[size(8)]] Point {
  [[not_serialize]]
  float x;
  [[info("hello")]]
  float y;
  static constexpr size_t dim = 2;
};

template <>
struct My::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr char name[6] = "Point";
#endif
  static constexpr AttrList attrs = {
      Attr{"size", 8},
  };
  static constexpr FieldList fields = {
      Field{"x", &Type::x,
            AttrList{
                Attr{"not_serialize"},
            }},
      Field{"y", &Type::y,
            AttrList{
                Attr{"info", "hello"},
            }},
      Field{"dim", Type::dim},
  };
};

int main() {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  cout << TypeInfo<Point>::name << endl;
#endif

  TypeInfo<Point>::fields.ForEach([](auto field) {
    cout << field.name << ", " << field.value << endl;
    field.attrs.ForEach([](auto attr) {
      cout << attr.name;
      if constexpr (attr.has_value)
        cout << " : " << attr.value;
      cout << endl;
    });
  });
}
