```

 __  __       ____  ____       __ _ 
|  \/  |_   _/ ___||  _ \ ___ / _| |
| |\/| | | | \___ \| |_) / _ \ |_| |
| |  | | |_| |___) |  _ <  __/  _| |
|_|  |_|\__, |____/|_| \_\___|_| |_|
        |___/                       

```

[![repo-size](https://img.shields.io/github/languages/code-size/shimakaze09/MySRefl?style=flat)](https://github.com/shimakaze09/MySRefl/archive/main.zip) [![tag](https://img.shields.io/github/v/tag/shimakaze09/MySRefl)](https://github.com/shimakaze09/MySRefl/tags) [![license](https://img.shields.io/github/license/shimakaze09/MySRefl)](LICENSE)

# MySRefl

> **My** **S**tatic **R**eflection

Header-only, tiny (99 lines) and powerful C++20 static reflection library.

## Feature

- **header-only**, **tiny (99 lines)** and **powerful** ([MySRefl_99.h](include/MySRefl_99.h)) (MSVC &
  GCC), [MySRefl_99_clang.h](include/MySRefl_99_clang.h) (Clang))
- **noninvasive**
- basic
    - (non-static / static) member variable
    - (non-static / static) member function
- **attribute**
- **enum**
    - string <-> enumerator
    - static dispatch
- **template**
- **inheritance**
    - diamond inheritance
    - iterate bases recursively
    - virtual inheritance
- **parser**

## Example

```c++
#include <MySRefl/MySRefl.h>
#include <iostream>

struct Point {
	float x;
	float y;
};

template<>
struct My::MySRefl::TypeInfo<Point> : TypeInfoBase<Point> {
  static constexpr AttrList attrs = {};
  static constexpr FieldList fields = {
    Field {TSTR("x"), &Type::x},
    Field {TSTR("y"), &Type::y},
  };
};

int main() {
  Point p{ 1.f, 2.f };
  My::MySRefl::TypeInfo<Point>::ForEachVarOf(
    p,
    [](const auto& field, auto&& var) {
      std::cout << field.name << ": " << var << std::endl;
    }
  );
}
```

result is

```
x : 1
y : 2
```

**other example**

- 99 line: [MySRefl_99.h](include/MySRefl_99.h), [test](src/test/06_99/main.cpp)
- [template](src/test/01_template/main.cpp)
- [static](src/test/02_static/main.cpp)
- [func](src/test/03_func/main.cpp)
- [enum](src/test/04_enum/main.cpp)
- [inheritance](src/test/05_inheritance/main.cpp)
- [virtual inheritance](src/test/07_virtual/main.cpp)
- [attribute name as type](src/test/10_type_attr/main.cpp)
- AutoRefl : [app](src/AutoRefl), [example](src/test/09_AutoRefl/00_basic) ([Vec.h](src/test/09_AutoRefl/00_basic/Vec.h),
Vec_AutoRefl.inl (generated by AutoRefl), [main.cpp](src/test/09_AutoRefl/00_basic/main.cpp))

## Documentation

- [Introduction](doc/intro.md)
- parser (TODO) (check CMake option `My_MySRefl_Build_AutoRefl` to generate the parser)

## Integration

You can choose one of the following two methods

- ⭐ **method 0**: add the required file
    - MSVC and GCC: [MySRefl_99.h](include/MySRefl_99.h)
    - Clang : [MySRefl_99_clang.h](include/MySRefl_99_clang.h)
- ⭐ **method 1**: cmake install, use `find package(MySRefl REQUIRED)` to get imported target `My::MySRefl_core`

## Compiler compatibility

- Clang/LLVM >= 10
- GCC >= 10
- MSVC >= 1926 (not fully support virtual inheritance because
  of [a MSVC bug](https://developercommunity.visualstudio.com/content/problem/1116835/member-pointer-of-a-class-with-a-virtual-base-1.html))