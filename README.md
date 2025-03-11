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

Header-only, tiny (99 lines) and powerful C++17 static reflection library.

## Feature

- **header-only**, **tiny (99 lines)** and **powerful
  ** ([MySRefl_99.h](include/MySRefl_99.h))
- **noninvasive**
- basic
    - (non-static / static) member variable
    - (non-static / static) member function
- **attribute**
- **enum**
    - string <-> key
    - static dispatch
- **template**
- **inheritance**
    - diamond inheritance
    - iterate bases recursively
    - virtual inheritance

## Example

```c++
#include <MySRefl/MySRefl.h>

#include <iostream>

using namespace My::MySRefl;
using namespace std;

// [[...]] act as (structured) command
// they are useless in code
struct [[size(8)]] Point {
	[[not_serialize]]
	float x;
	[[info("hello")]]
	float y;
};

// declaration
template<>
struct TypeInfo<Point> : TypeInfoBase<Point> {
	// if you use <USRefl/USRefl.h>, the name is declared by TypeInfoBase<Point>
    // if you use <USRefl_99.h>, you should declare it here.
    // static constexpr std::string_view name = "struct Point";
	using type = Point;

	static constexpr FieldList fields = {
		Field{"x", &Point::x, AttrList{ Attr{ "not_serialize" } }},
		Field{"y", &Point::y, AttrList{ Attr{ "info", "hello" } }}
	};

	static constexpr AttrList attrs = {
		Attr{ "size", 8 }
	};
};

int main() {
	TypeInfo<Point>::fields.ForEach([](auto field) {
		cout << field.name << endl;
		field.attrs.ForEach([](auto attr) {
			cout << attr.name;
			if constexpr (attr.has_value)
				 cout << ": " << attr.value;
            cout << endl;
		});
	});

	static_assert(TypeInfo<Point>::fields.Contains("x"));

	TypeInfo<Point>::attrs.ForEach([](auto attr) {
        cout << attr.name;
		if constexpr (!attr.has_value)
			cout << ": " << attr.value;
        cout << endl;
	});

	TypeInfo<Point>::ForEachVarOf(Point{ 1,2 }, [](auto&& var) {
		cout << var << endl;
	});
}
```

**other example**

- [template](src/test/01_template/main.cpp)
- [static](src/test/02_static/main.cpp)
- [func](src/test/03_func/main.cpp)
- [enum](src/test/04_enum/main.cpp)
- [inheritance](src/test/05_inheritance/main.cpp)
- [virtual inheritance](src/test/07_virtual/main.cpp)
- 99 line: [MySRefl_99.h](include/MySRefl_99.h)，[test](src/test/06_99/main.cpp)

## Documentation

- [Introduction](doc/intro.md)

## Integration

- You can choose one of the following three methods

- **method 0**: add required file [MySRefl_99.h](include/MySRefl_99.h)
- run CMake (download [nameof.hpp](https://github.com/Neargye/nameof/blob/master/include/nameof.hpp)
  to [include/MySRefl/_deps/](include/MySRefl/_deps/))
    - **method 1**: directly use [include/MySRefl/](include/MySRefl/)
    - **method 2**: cmake install, `find package(MySRefl REQUIRED)` to get imported target `My::MySRefl_core`

## Compiler compatibility

- Clang/LLVM >= 6
- GCC >= 7.3
- MSVC++ >= 19.20 / VS 2019 + (not fully support virtual inheritance because of a MSVC++ bug)