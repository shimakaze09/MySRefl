# MySRefl

> **My** **S**tatic **R**eflection

[![repo-size](https://img.shields.io/github/languages/code-size/shimakaze09/MySRefl?style=flat)](https://github.com/shimakaze09/MySRefl/archive/main.zip) [![tag](https://img.shields.io/github/v/tag/shimakaze09/MySRefl)](https://github.com/shimakaze09/MySRefl/tags) [![license](https://img.shields.io/github/license/shimakaze09/MySRefl)](LICENSE)

## Feature

- **header-only**, **tiny (99 lines)** and **powerful
  ** ([MySRefl_99.h](https://github.com/shimakaze09/MySRefl/blob/main/include/MySRefl_99.h))
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

## Example

```c++
#include <MySRefl.h>

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

template<>
struct TypeInfo<Point> : TypeInfoBase<Point> {
	static constexpr std::string_view name = "Point";
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
- 99 line: [MySRefl_99.h](include/MySRefl_99.h)，[test](src/test/06_99/main.cpp)

## Integration

You should add required file [USRefl.h](include/MySRefl.h) or [USRefl_99.h](include/MySRefl_99.h)

## Compiler compatibility

- Clang/LLVM >= 6
- GCC >= 7.3
- MSVC++ >= 19.20 / VS 2019 +