//
// Created by Admin on 11/03/2025.
//

#pragma once  // My Static Reflection 99 (now 72)
#include <string_view>
#include <tuple>

template <typename List, typename Func>
constexpr size_t detail_FindIf(const List& list, Func&& func,
                               std::index_sequence<>) {
  return static_cast<size_t>(-1);
}

template <typename List, typename Func, size_t N0, size_t... Ns>
constexpr size_t detail_FindIf(const List& list, Func&& func,
                               std::index_sequence<N0, Ns...>) {
  return func(list.template Get<N0>())
             ? N0
             : detail_FindIf(list, std::forward<Func>(func),
                             std::index_sequence<Ns...>{});
}

template <typename T>
struct NamedValue {  // named value
  std::string_view name;
  T value;
  static constexpr bool has_value = true;

  template <typename U>
  constexpr bool operator==(U v) const {
    if constexpr (std::is_same_v<T, U>)
      return value == v;
    else
      return false;
  }
};

template <>
struct NamedValue<void> {
  std::string_view name; /*T value;*/
  static constexpr bool has_value = false;

  template <typename U>
  constexpr bool operator==(U) const {
    return false;
  }
};

template <typename... Elems>
struct ElemList {  // Elems is a named value
  std::tuple<Elems...> elems;

  constexpr ElemList(Elems... elems) : elems{elems...} {}

  template <typename Func>
  constexpr void ForEach(Func&& func) const {
    std::apply([&](auto... elems) { (std::forward<Func>(func)(elems), ...); },
               elems);
  }

  template <typename Func>
  constexpr size_t FindIf(Func&& func) const {
    return detail_FindIf(*this, std::forward<Func>(func),
                         std::make_index_sequence<sizeof...(Elems)>{});
  }

  constexpr size_t Find(std::string_view n) const {
    return FindIf([n](auto e) { return e.name == n; });
  }

  template <typename T>
  constexpr size_t FindValue(T v) const {
    return FindIf([v](auto e) { return e == v; });
  }

  constexpr bool Contains(std::string_view name) const {
    return Find(name) != static_cast<size_t>(-1);
  }

  template <size_t N>
  constexpr auto Get() const {
    return std::get<N>(elems);
  }

#define MySRefl_ElemList_GetByName(list, name) list.Get<list.Find(name)>()
#define MySRefl_ElemList_GetByValue(list, value) \
  list.Get<list.FindValue(value)>()
};

template <typename T>
struct Attr : NamedValue<T> {
  constexpr Attr(std::string_view n, T v) : NamedValue<T>{n, v} {}
};

template <>
struct Attr<void> : NamedValue<void> {
  constexpr Attr(std::string_view n) : NamedValue<void>{n} {}
};

template <typename... As>
struct AttrList : ElemList<As...> {
  constexpr AttrList(As... as) : ElemList<As...>{as...} {}
};

template <bool s, bool f>
struct FTraitsB {
  static constexpr bool is_static = s, is_func = f;
};

template <typename T>
struct FTraits : FTraitsB<true, false> {};  // default is enum

template <typename U, typename T>
struct FTraits<T U::*> : FTraitsB<false, std::is_function_v<T>> {};

template <typename T>
struct FTraits<T*> : FTraitsB<true, std::is_function_v<T>> {};  // static member

template <typename T, typename AList>
struct Field : FTraits<T>, NamedValue<T> {
  AList attrs;

  constexpr Field(std::string_view n, T v, AList as = {})
      : NamedValue<T>{n, v}, attrs{as} {}
};

template <typename... Fs>
struct FieldList : ElemList<Fs...> {
  constexpr FieldList(Fs... fs) : ElemList<Fs...>{fs...} {}
};
template <typename T>
struct TypeInfo;  // TypeInfoBase, name, fields, attrs

template <typename... Ts>
struct TypeInfoList : ElemList<Ts...> {
  constexpr TypeInfoList(Ts... ts) : ElemList<Ts...>{ts...} {}
};

template <typename T, typename... Bases>
struct TypeInfoBase {
  using type = T;
  static constexpr TypeInfoList bases = {TypeInfo<Bases>{}...};

  template <typename U>
  static constexpr auto&& Forward(U&& derived) noexcept {
    if constexpr (std::is_same_v<const std::decay_t<U>&, U>)
      return static_cast<const type&>(derived);
    else if constexpr (std::is_same_v<std::decay_t<U>&, U>)
      return static_cast<type&>(derived);
    else
      return static_cast<type&&>(
          derived);  // if constexpr (std::is_same_v<DecayU, U>)
  }

  template <typename Func>
  static constexpr void DFS(Func&& func, size_t depth = 0) {
    func(TypeInfo<type>{}, depth);
    TypeInfo<type>::bases.ForEach(
        [&](auto base) { base.DFS(std::forward<Func>(func), depth + 1); });
  }

  template <typename U, typename Func>
  static constexpr void ForEachVarOf(U&& obj, Func&& func) {
    TypeInfo<type>::fields.ForEach([&](auto f) {
      if constexpr (!f.is_static && !f.is_func)
        std::forward<Func>(func)(std::forward<U>(obj).*(f.value));
    });
    TypeInfo<type>::bases.ForEach([&](auto base) {
      base.ForEachVarOf(base.Forward(std::forward<U>(obj)),
                        std::forward<Func>(func));
    });
  }
};

template <size_t N>
Attr(std::string_view, const char (&)[N]) -> Attr<std::string_view>;
Attr(std::string_view) -> Attr<void>;
template <typename T, typename AList>
Field(std::string_view, T, AList) -> Field<T, AList>;
template <typename T>
Field(std::string_view, T) -> Field<T, AttrList<>>;
