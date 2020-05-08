# Contributing

![Contributions](https://img.shields.io/badge/contributions-open-green)

Contributions are encouraged, however, discussing the proposed changes beforehand via [twitter or discord](https://github.com/VertexEngine/VertexEngine#support) is appreciated.

## Code style

It is important for a healthy code base to have a consistent style through all internal code. This extends to design patterns as well as just the formatting of the code itself.

### Code formatting
* Indents are 4 spaces, and not tabs.

* References `&` and pointers `*` must be attached to the typename or the variable name if the typename is not applicable. For example:

```cpp
const char* foo;
Foo& bar;
void* foo = &bar;
```

* Curly braces must be on the next line of function definitions if not inline. For example:

```cpp
if (condition)
{
    // do something
}

namespace Foo
{

    void Bar()
    {

    }

}
```

* Implicit casts should be done as such: `(foo)bar`.

* All variables, and arguments are in the `snake_case`.

* Static and private member variables must be prefixed with `s_` and `m_` respectively, and be followed by its name in `PascalCase`.

* All classes, structs, functions, and enum classes are in the `PascalCase`.

* All macros are in the `UPPER_CASE_SNAKE_CASE`.

* Macros must be indented.

* Use `#pragma once` instead of include guards.

### Patterns
* Header files must have the `.h` extension.

* File names must be in `PascalCase`.

* Include only necessary headers.

* Include common header files in `Vertex/src/Core/vx_pch.h`.

## Code of conduct
See [CODE_OF_CONDUCT](CODE_OF_CONDUCT.md).
