//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <string_view>
#include <tuple>

namespace My::MySRefl::detail {
template <size_t idx, typename T, typename Func>
static constexpr void ForNonStaticFieldOf(T&& obj, const Func& func) {
  const auto& field = std::get<idx>(Type<std::decay_t<T>>::fields);
  if constexpr (!field.is_static)
    func(obj.*(field.value));
}

template <typename T, typename Func, size_t... Ns>
static constexpr void ForEachVarOf(T&& obj, const Func& func,
                                   std::index_sequence<Ns...>) {
  (ForNonStaticFieldOf<Ns>(obj, func), ...);
}

template <typename List, typename Func, size_t... Ns>
static constexpr void ForEach(const List& list, const Func& func,
                              std::index_sequence<Ns...>) {
  (func(std::get<Ns>(list)), ...);
}

template <typename Indices>
struct IndexSequenceTraits;

template <size_t N0, size_t... Ns>
struct IndexSequenceTraits<std::index_sequence<N0, Ns...>> {
  static constexpr size_t head = N0;
  using tail = std::index_sequence<Ns...>;
};

template <typename List, typename Func, size_t... Ns>
static constexpr size_t FindIf(const List& list, const Func& func,
                               std::index_sequence<Ns...>) {
  if constexpr (sizeof...(Ns) == 0)
    return static_cast<size_t>(-1);
  else {
    constexpr size_t head =
        IndexSequenceTraits<std::index_sequence<Ns...>>::head;
    using tail = typename IndexSequenceTraits<std::index_sequence<Ns...>>::tail;
    return func(std::get<head>(list)) ? head : FindIf(list, func, tail{});
  }
}

// Elems has name
template <typename... Elems>
struct BaseList : std::tuple<Elems...> {
  static constexpr size_t size = sizeof...(Elems);

  constexpr BaseList(Elems... elems) : std::tuple<Elems...>{elems...} {}

  template <typename Func>
  constexpr void ForEach(const Func& func) const {
    detail::ForEach(*this, func, std::make_index_sequence<size>{});
  }

  template <typename Func>
  constexpr size_t FindIf(const Func& func) const {
    return detail::FindIf(*this, func, std::make_index_sequence<size>{});
  }

  constexpr size_t Find(std::string_view name) const {
    return FindIf([name](auto attr) { return attr.name == name; });
  }

  constexpr bool Contains(std::string_view name) const {
    return Find(name) != static_cast<size_t>(-1);
  }

  template <size_t N>
  constexpr auto Get() const {
    static_assert(N != static_cast<size_t>(-1));
    return std::get<N>(*this);
  }
};
}  // namespace My::MySRefl::detail

namespace My::MySRefl {
// field : member variable/function, static or non-static

// name, fields, attrs
template <typename T>
struct Type;

template <typename Func>
struct IsFunc : std::false_type {};

template <typename Ret, typename... Args>
struct IsFunc<Ret(Args...)> : std::true_type {};

template <typename Ret, typename... Args>
struct IsFunc<Ret(Args...) const> : std::true_type {};

template <typename Func>
static constexpr bool IsFunc_v = IsFunc<Func>::value;

template <typename... Args>
struct Overload {
  template <typename R, typename T>
  constexpr auto operator()(R (T::*func_ptr)(Args...)) const {
    return func_ptr;
  }

  template <typename R, typename T>
  constexpr auto operator()(R (T::*func_ptr)(Args...) const) const {
    return func_ptr;
  }

  template <typename R>
  constexpr auto operator()(R (*func_ptr)(Args...)) const {
    return func_ptr;
  }
};

template <typename... Args>
constexpr Overload<Args...> overload_v{};

template <typename T>
struct FieldTraits;

template <typename Object, typename T>
struct FieldTraits<T Object::*> {
  using object_type = Object;
  using value_type = T;
  static constexpr bool is_static = false;
  static constexpr bool is_function = IsFunc_v<T>;
};

template <typename T>
struct FieldTraits<T*> {
  using object_type = void;
  using value_type = T;
  static constexpr bool is_static = true;
  static constexpr bool is_function = IsFunc_v<T>;
};

template <typename T>
struct FieldTraits {
  static_assert(std::is_enum_v<T>);
  using object_type = void;
  using value_type = T;
  static constexpr bool is_static = true;
  static constexpr bool is_function = false;
};

template <typename T = void>
struct Attr;

template <typename T>
struct Attr {
  constexpr Attr(std::string_view name, T value) : name{name}, value{value} {}

  constexpr Attr(std::string_view name) : Attr{name, T{}} {}

  constexpr Attr() : Attr{"", T{}} {}

  std::string_view name;
  T value;
  static constexpr bool has_value = true;
};

template <>
struct Attr<void> {
  constexpr Attr(std::string_view name) : name{name} {}

  constexpr Attr() : Attr{""} {}

  std::string_view name;
  static constexpr bool has_value = false;
};

using AttrStr = Attr<std::string_view>;
template <size_t N>
Attr(std::string_view, const char[N]) -> Attr<std::string_view>;

template <typename T>
struct IsAttr : std::false_type {};

template <typename T>
struct IsAttr<Attr<T>> : std::true_type {};

template <typename... Attrs>
struct AttrList : detail::BaseList<Attrs...> {
  static_assert((IsAttr<Attrs>::value && ...));
  using detail::BaseList<Attrs...>::BaseList;
};
template <class... Attrs>
AttrList(Attrs...) -> AttrList<Attrs...>;

template <typename T>
struct IsAttrList : std::false_type {};

template <typename... Attrs>
struct IsAttrList<AttrList<Attrs...>> : std::true_type {};

template <typename T, typename AList>
struct Field : FieldTraits<T> {
  static_assert(IsAttrList<AList>::value);

  constexpr Field(std::string_view name, T value, AList attrs)
      : name{name}, value{value}, attrs{attrs} {}

  std::string_view name;
  T value;
  AList attrs;

  template <typename U>
  static constexpr bool ValueTypeIs() {
    return std::is_same_v<T, U>;
  }

  template <typename U>
  static constexpr bool ValueTypeIsSameWith(const U& u) {
    return ValueTypeIs<U>();
  }
};

template <typename T, typename AList>
Field(std::string_view, T, AList) -> Field<T, AList>;

template <typename T>
struct IsField : std::false_type {};

template <typename T, typename AList>
struct IsField<Field<T, AList>> : std::true_type {};

// Field's (name, value_type) must be unique
template <typename... Fields>
struct FieldList : detail::BaseList<Fields...> {
  static_assert((IsField<Fields>::value && ...));
  using detail::BaseList<Fields...>::BaseList;
};
template <class... Fields>
FieldList(Fields...) -> FieldList<Fields...>;

// non-static member variables
template <typename T, typename Func>
constexpr void ForEachVarOf(T&& obj, const Func& func) {
  detail::ForEachVarOf(
      std::forward<T>(obj), func,
      std::make_index_sequence<Type<std::decay_t<T>>::fields.size>{});
}
}  // namespace My::MySRefl
