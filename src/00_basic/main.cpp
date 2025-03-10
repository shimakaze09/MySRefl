//
// Created by Admin on 11/03/2025.
//

#include <MySRefl.h>

#include <iostream>

using namespace My::MySRefl;
using namespace std;

struct [[size("8")]] Point {
  [[not_serialize]]
  float x;
  [[info("hello")]]
  float y;
};

template <>
struct FieldDecl<float, Point, &Point::x> {
  static constexpr string_view name = "x";
  static constexpr std::array<Meta, 1> metas{{{"not_serialize"}}};
};

template <>
struct FieldDecl<float, Point, &Point::y> {
  static constexpr string_view name = "y";
  static constexpr std::array<Meta, 1> metas{{{"info", "hello"}}};
};

template <>
struct TypeDecl<Point> {
  static constexpr std::tuple fields{MYSREFL_FIELD_OF(&Point::x),
                                     MYSREFL_FIELD_OF(&Point::y)};
  static constexpr std::array<Meta, 1> metas{{{"size", "8"}}};
};

int main() {
  const Point p{1, 2};

  Type<Point>::ForEachMeta([](const Meta& meta) {
    cout << "key   : " << meta.key << endl;
    cout << "value : " << meta.value << endl;
  });

  Type<Point>::ForEachField([](auto&& field) {
    cout << field.name << endl;
    field.ForEachMeta([](const Meta& meta) {
      cout << "key   : " << meta.key << endl;
      cout << "value : " << meta.value << endl;
    });
  });

  ForEachFieldOf(p, [](auto&& field) { cout << field << endl; });
}
