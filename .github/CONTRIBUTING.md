# Contributing

![Contributions](https://img.shields.io/badge/contributions-open-green)

Contributions are encouraged, however, discussing the proposed changes beforehand via [twitter or discord](https://github.com/42mix/Vertex#support) is appreciated.

## Code style

It is important for a healthy code base to have a consistent style through all internal code. This extends to design patterns as well as just the formatting of the code itself.

### Code formatting
* Indents are 4 spaces, and not tabs.
* All variables, and arguments are in the `snake_case`.
* Static and private member variables must be prefixed with `s_` or `m_` respectively and followed by its name in `PascalCase`.
* All classes, structs, and enum classes are in the `PascalCase`.
* All macros are in the `UPPER_CASE_SNAKE_CASE`.

### Patterns
* All C++ files have the `.cpp` extension. All header files have the `.hpp` extension and **not** the `.h` extension in order to convey that this library is to be used for C++, and for consistency.
* Attempt to maximize performace.
* Prefer documented code.
* Minimize external dependencies, so that the code can almost work out of the box.

## Code of conduct
See [CODE_OF_CONDUCT](https://github.com/42mix/Vertex/blob/master/CODE_OF_CONDUCT.md).
