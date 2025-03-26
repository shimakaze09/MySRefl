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
struct TypeInfo<Point> : TypeInfoBase<Point> {
#ifdef MY_MYSREFL_NOT_USE_NAMEOF
  static constexpr char name[6] = "Point";
#endif
  static constexpr AttrList attrs = {};
  static constexpr FieldList fields = {
      Field{"x", &Type::x},
      Field{"y", &Type::y},
  };
};

int main() {
  Point p{1.f, 2.f};
  My::MySRefl::TypeInfo<Point>::ForEachVarOf(p, [](auto field, auto&& var) {
    cout << field.name << ": " << var << endl;
  });
}
