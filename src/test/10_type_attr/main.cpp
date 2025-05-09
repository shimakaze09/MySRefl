#include <MySRefl/MySRefl.h>

#include <iostream>

using namespace Smkz::MySRefl;
using namespace std;

//
// Util
/////////

namespace MyMeta {
template <typename Str>
struct Typeof;
template <typename Str>
using Typeof_t = typename Typeof<Str>::type;
}  // namespace MyMeta

template <typename T>
struct is_std_tuple : std::false_type {};
template <typename... Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type {};
template <typename T>
constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

template <typename T, typename Tuple>
constexpr auto tuple_init(Tuple&& t) {
  return std::apply(
      [](auto&&... elems) {
        return T{std::forward<decltype(elems)>(elems)...};
      },
      std::forward<Tuple>(t));
}

template <typename Str, typename Value>
constexpr auto attr_init(Str, Value&& v) {
  using T = MyMeta::Typeof_t<Str>;
  if constexpr (is_std_tuple_v<std::decay_t<Value>>)
    return tuple_init<T>(std::forward<Value>(v));
  else
    return T{std::forward<Value>(v)};
}

#define TYPEOF_REGISTER(X)                             \
  template <>                                          \
  struct MyMeta::Typeof<typename TypeInfo<X>::TName> { \
    using type = X;                                    \
  }

//
// MyMeta::Range
/////////////////

namespace MyMeta {
struct Range {
  float minV;
  float maxV;
};
}  // namespace MyMeta

template <>
struct Smkz::MySRefl::TypeInfo<MyMeta::Range> : TypeInfoBase<MyMeta::Range> {
  static constexpr AttrList attrs = {};
  static constexpr FieldList fields = {
      Field{TSTR("minV"), &Type::minV},
      Field{TSTR("maxV"), &Type::maxV},
  };
};

TYPEOF_REGISTER(MyMeta::Range);

//
// Point
//////////

struct Point {
  [[Attr::Range(1.f, 2.f)]]
  float x;
  float y;
};

template <>
struct Smkz::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
  static constexpr AttrList attrs = {};
  static constexpr FieldList fields = {
      Field{TSTR("x"), &Type::x,
            AttrList{Attr{TSTR("MyMeta::Range"), std::tuple{1.f, 2.f}}}},
      Field{TSTR("y"), &Type::y},
  };
};

//
// main
//////////

int main() {
  Point p{1.f, 2.f};
  TypeInfo<Point>::ForEachVarOf(p, [](const auto& field, auto&& var) {
    constexpr auto tstr_range = TSTR("MyMeta::Range");
    if constexpr (decltype(field.attrs)::Contains(tstr_range)) {
      auto r = attr_init(tstr_range, field.attrs.Find(tstr_range).value);
      cout << "[" << tstr_range.View() << "] " << r.minV << ", " << r.maxV
           << endl;
    }
    cout << field.name << ": " << var << endl;
  });
}
