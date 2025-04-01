//
// Created by Admin on 11/03/2025.
//

#include <MySRefl/MySRefl.h>
#include <iostream>

using namespace My::MySRefl;
using namespace std;

struct Point {
  float x;
  float y;
};

template <>
struct My::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr char name[6] = "Point";
#endif
  static constexpr AttrList attrs = {};
  static constexpr FieldList fields = {
      Field{TSTR("x"), &Type::x},
      Field{TSTR("y"), &Type::y},
  };
};

int main() {
  Point p{1.f, 2.f};
  TypeInfo<Point>::ForEachVarOf(p, [](auto field, auto&& var) {
    cout << field.name << ": " << var << endl;
  });
  constexpr const auto& field_x = TypeInfo<Point>::fields.Find(TSTR("x"));
  cout << p.*field_x.value << endl;
  constexpr bool contains_y = TypeInfo<Point>::fields.Contains(TSTR("y"));
  static_assert(contains_y);
}
