# MySRefl

**My** **S**tatic **R**eflection

[![repo-size](https://img.shields.io/github/languages/code-size/shimakaze09/MySRefl?style=flat)](https://github.com/shimakaze09/MySRefl/archive/main.zip) [![tag](https://img.shields.io/github/v/tag/shimakaze09/MySRefl)](https://github.com/shimakaze09/MySRefl/tags) [![license](https://img.shields.io/github/license/shimakaze09/MySRefl)](LICENSE)

## Feature

- the **best** C++ static reflection library in the whole world
- tiny, handy, elegant
- basic
    - (non-static / static) member variable
    - (non-static / static) member function
- attribute
    - class
        - member variable
        - member function
    - enum
        - enumerator
- template
    - basic
    - attribute
- inheritance
    - inherit field (member variable, member function)
    - iterate subclasses recursively

## Example

```c++
#include <MySRefl.h>

#include <iostream>

using namespace My::MySRefl;
using namespace std;

struct [[size(8)]] Point {
	[[not_serialize]]
	float x;
	[[info("hello")]]
	float y;
};

template<>
struct Type<Point> {
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
	Point p{ 1,2 };

	Type<Point>::fields.ForEach([](auto field) {
		cout << field.name << endl;
		field.attrs.ForEach([](auto attr) {
			cout << attr.name << endl;
			if constexpr (attr.has_value)
				cout << ": " << attr.value << endl;
		});
	});

	static_assert(Type<Point>::fields.Contains("x"));

	Type<Point>::attrs.ForEach([](auto attr) {
		cout << "name   : " << attr.name << endl;
		if constexpr (!attr.has_value)
			cout << "value : " << attr.value << endl;
	});

	ForEachVarOf(p, [](auto&& var) {
		cout << var << endl;
	});
}
```

**other example**

- [template](src/test/01_template/main.cpp)
- [static](src/test/02_static/main.cpp)
- [func](src/test/03_func/main.cpp)
- [enum](src/test/04_enum/main.cpp)
- [inheritance](src/test/05_subclass/main.cpp)
- 99 line: [MySRefl_99.h](src/test/06_99/MySRefl_99.h)，test [MySRefl_99.h](src/test/06_99/main.cpp) 