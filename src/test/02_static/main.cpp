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

  static constexpr size_t id = 1024;
};

template <>
struct My::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr char name[6] = "Point";
#endif
  static constexpr AttrList attrs = {
      Attr{MYSTR("size"), 8},
  };
  static constexpr FieldList fields = {
      Field{MYSTR("x"), &Type::x,
            AttrList{
                Attr{MYSTR("not_serialize")},
            }},
      Field{MYSTR("y"), &Type::y,
            AttrList{
                Attr{MYSTR("info"), "hello"},
            }},
      Field{MYSTR("id"), Type::id},
  };
};

int main() {
  Point p{1, 2};

  TypeInfo<Point>::fields.ForEach([](auto field) {
    cout << field.name << endl;
    field.attrs.ForEach([](auto attr) {
      cout << "name   : " << attr.name << endl;
      if constexpr (attr.has_value)
        cout << "value : " << attr.value << endl;
    });
  });

  constexpr auto y_field = TypeInfo<Point>::fields.Find(MYSTR("y"));
  static_assert(y_field.name == "y");

  static_assert(TypeInfo<Point>::fields.Contains(MYSTR("x")));

  TypeInfo<Point>::attrs.ForEach([](auto attr) {
    cout << "name   : " << attr.name << endl;
    if constexpr (!attr.has_value)
      cout << "value : " << attr.value << endl;
  });

  TypeInfo<Point>::ForEachVarOf(p, [](auto field, auto&& var) {
    cout << field.name << " : " << var << endl;
  });

  TypeInfo<Point>::fields.ForEach([](auto field) {
    if constexpr (field.is_static)
      cout << field.name << ": " << field.value << endl;
  });
}
