//
// Created by Admin on 11/03/2025.
//

#include <iostream>
#include "MySRefl_99.h"

using namespace std;

// ==============
//  basic
// ==============
struct [[size(8)]] Point {
  [[not_serialize]]
  float x;
  [[info("hello")]]
  float y;
  static constexpr size_t id = 1024;

  float Sum() const { return x + y; }
};

template <>
struct TypeInfo<Point> : TypeInfoBase<Point> {
  static constexpr std::string_view name = "Point";

  static constexpr FieldList fields = {
      Field{"x", &Point::x, AttrList{Attr{"not_serialize"}}},
      Field{"y", &Point::y, AttrList{Attr{"info", "hello"}}},
      Field{"id", &Point::id}, Field{"Sum", &Point::Sum}};

  static constexpr AttrList attrs = {Attr{"size", 8}};
};

void test_basic() {
  cout << "====================" << endl
       << " basic" << endl
       << "====================" << endl;

  Point p{1, 2};

  TypeInfo<Point>::fields.ForEach([](auto field) {
    cout << field.name << endl;
    field.attrs.ForEach([](auto attr) {
      cout << attr.name;
      if constexpr (attr.has_value)
        cout << ": " << attr.value;
      cout << endl;
    });
  });

  constexpr auto y_field =
      MySRefl_ElemList_GetByName(TypeInfo<Point>::fields, "y");
  static_assert(y_field.name == "y");

  static_assert(TypeInfo<Point>::fields.Contains("x"));

  TypeInfo<Point>::ForEachVarOf(p, [](auto&& var) { cout << var << endl; });

  TypeInfo<Point>::fields.ForEach([](auto field) {
    if constexpr (field.is_static)
      cout << field.name << ": " << *field.value << endl;
  });

  TypeInfo<Point>::fields.ForEach([p](auto field) {
    if constexpr (field.is_func) {
      if (field.name != "Sum")
        return;
      cout << (p.*(field.value))() << endl;
    }
  });
}

// ==============
//  template
// ==============
template <typename T>
struct Data {
  [[range(std::pair<T, T>{static_cast<T>(0), static_cast<T>(100)})]]
  T value;
};

template <typename T>
struct TypeInfo<Data<T>> : TypeInfoBase<Data<T>> {
  static constexpr std::string_view name = "Data";  // use nameof

  static constexpr FieldList fields = {Field{
      "value", &Data<T>::value,
      AttrList{Attr{"range",
                    std::pair<T, T>{static_cast<T>(0), static_cast<T>(100)}}}}};
};

void test_template() {
  cout << "====================" << endl
       << " template" << endl
       << "====================" << endl;

  cout << TypeInfo<Data<float>>::name << endl;
  constexpr auto valueAttrs =
      MySRefl_ElemList_GetByName(TypeInfo<Data<float>>::fields, "value").attrs;
  constexpr auto range = MySRefl_ElemList_GetByName(valueAttrs, "range").value;
  constexpr float range_min = range.first;
  constexpr float range_max = range.second;
  cout << "range min :" << range_min << endl;
  cout << "range max :" << range_max << endl;
}

// ==============
//  inheritance
// ==============
struct A {
  float a;
};

struct B : A {
  float b;
};

struct C : A {
  float c;
};

struct D : B, C {
  float d;
};

template <>
struct TypeInfo<A> : TypeInfoBase<A> {
  static constexpr std::string_view name = "A";

  static constexpr FieldList fields = {Field{"a", &A::a}};

  static constexpr AttrList attrs = {};
};

template <>
struct TypeInfo<B> : TypeInfoBase<B, A> {
  static constexpr std::string_view name = "B";

  static constexpr FieldList fields = {Field{"b", &B::b}};

  static constexpr AttrList attrs = {};
};

template <>
struct TypeInfo<C> : TypeInfoBase<C, A> {
  static constexpr std::string_view name = "C";

  static constexpr FieldList fields = FieldList{Field{"c", &C::c}};

  static constexpr AttrList attrs = {};
};

template <>
struct TypeInfo<D> : TypeInfoBase<D, B, C> {
  static constexpr std::string_view name = "D";

  static constexpr FieldList fields = FieldList{Field{"d", &D::d}};

  static constexpr AttrList attrs = {};
};

void test_inheritance() {
  cout << "====================" << endl
       << " inheritance" << endl
       << "====================" << endl;

  TypeInfo<D>::DFS([](auto t, size_t depth) {
    for (size_t i = 0; i < depth; i++)
      cout << "  ";
    cout << t.name << endl;
  });

  cout << "[non DFS]" << endl;
  TypeInfo<D>::fields.ForEach([](auto field) { cout << field.name << endl; });

  cout << "[DFS]" << endl;
  TypeInfo<D>::DFS([](auto t, size_t) {
    t.fields.ForEach([](auto field) { cout << field.name << endl; });
  });

  cout << "[var]" << endl;
  D d;
  d.B::a = 1;
  d.C::a = 2;
  d.b = 3;
  d.c = 4;
  d.d = 5;
  TypeInfo<D>::ForEachVarOf(std::move(d), [cnt = 0](auto&& var) mutable {
    static_assert(std::is_rvalue_reference_v<decltype(var)>);
    cout << cnt << ": " << var << endl;
    cnt++;
  });
  TypeInfo<D>::ForEachVarOf(d, [cnt = 0](auto&& var) mutable {
    static_assert(std::is_lvalue_reference_v<decltype(var)>);
    cout << cnt << ": " << var << endl;
    cnt++;
  });
}

// ==============
//  enum
// ==============
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

void test_enum() {
  cout << "====================" << endl
       << " enum" << endl
       << "====================" << endl;

  TypeInfo<Color>::fields.ForEach(
      [](auto field) { cout << field.name << endl; });
  static_assert(
      MySRefl_ElemList_GetByName(TypeInfo<Color>::fields, "RED").value ==
      Color::RED);
  static_assert(
      MySRefl_ElemList_GetByValue(TypeInfo<Color>::fields, Color::RED).name ==
      "RED");

  constexpr Color c = Color::GREEN;
  constexpr auto c_attr =
      MySRefl_ElemList_GetByValue(TypeInfo<Color>::fields, c).attrs;
  static_assert(MySRefl_ElemList_GetByName(c_attr, "func").value() == 2);
}

// ==============
//  function
// ==============
struct FuncList {
  void Func0(float a, float b) {}

  void Func1(int x = 1) {}
};

template <>
struct TypeInfo<FuncList> : TypeInfoBase<FuncList> {
  static constexpr FieldList fields = {
      Field{"Func0", &FuncList::Func0,
            AttrList{Attr{"argument_list",
                          AttrList{
                              Attr{"@0", NamedValue<void>{"a"}},
                              Attr{"@1", NamedValue<void>{"b"}},
                          }}}},
      Field{"Func1", &FuncList::Func1,
            AttrList{Attr{"argument_list",
                          AttrList{Attr{"@0", NamedValue<int>{"x", 1}}}}}}};
};

void test_function() {
  cout << "====================" << endl
       << " function" << endl
       << "====================" << endl;

  constexpr auto f0 =
      MySRefl_ElemList_GetByName(TypeInfo<FuncList>::fields, "Func0");
  cout << f0.name << endl;
  constexpr auto f0_args =
      MySRefl_ElemList_GetByName(f0.attrs, "argument_list");
  f0_args.value.ForEach([](auto arg) {
    cout << arg.name << ": " << arg.value.name;
    if constexpr (arg.value.has_value)
      cout << " = " << arg.value.value;
    cout << endl;
  });

  constexpr auto f1 =
      MySRefl_ElemList_GetByName(TypeInfo<FuncList>::fields, "Func1");
  cout << f1.name << endl;
  constexpr auto f1_args =
      MySRefl_ElemList_GetByName(f1.attrs, "argument_list");
  f1_args.value.ForEach([](auto arg) {
    cout << arg.name << ": " << arg.value.name;
    if constexpr (arg.value.has_value)
      cout << " = " << arg.value.value;
    cout << endl;
  });
}

int main() {
  test_basic();
  test_template();
  test_enum();
  test_inheritance();
  test_function();

  return 0;
}
