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
};

template <>
struct My::MySRefl::TypeInfo<Point> : My::MySRefl::TypeInfoBase<Point> {
  inline static AttrList attrs = {
      Attr{"size", 8},
  };
  inline static FieldList fields = {
      Field{"x", &Point::x,
            AttrList{
                Attr{"not_serialize"},
            }},
      Field{"y", &Point::y,
            AttrList{
                Attr{"info", "hello"},
            }},
  };
};

int main() {
  cout << TypeInfo<Point>::name << endl;

  TypeInfo<Point>::fields.ForEach([](auto field) {
    cout << field.name << endl;
    field.attrs.ForEach([](auto attr) {
      cout << attr.name;
      if constexpr (attr.has_value)
        cout << " : " << attr.value;
      cout << endl;
    });
  });
}
