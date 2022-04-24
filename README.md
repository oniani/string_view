# `string_view`

An alternative `std::basic_string_view` implementation.

## API

```cpp
#include "include/basic_string_view.hpp"

int main() {
  std::string_view s{"Hello, world!"};
  std::cout << s << '\n';
}
```

## Build

```console
$ mkdir build
$ cd build
$ cmake -DCOMPILER=clang -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=VCPKG_TOOLCHAIN_FILE ..
$ cmake --build .
```

## References

- [`std::basic_string_view`][https://en.cppreference.com/w/cpp/string/basic_string_view]

## License

[MIT License][license]

[string_view]: https://en.cppreference.com/w/cpp/string/basic_string_view
[license]: LICENSE
