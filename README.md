# cpp-nameof
A small C++ header file which provides the nameof(T) macro.

Tested with `x86-64 gcc 14.1 -std=c++23`, `x86-64 clang 18.1.0 -std=c++23`, and `x64 MSVC.latest /std:c++latest` as of May 5, 2024.

## Usage
To use this header file, simply `#include` it at the top of any of your C++ source files and the `nameof` macro will be defined.

The `nameof(T)` macro evaluates to a compile-time `std::string_view` value containing the name of the type `T` such that `nameof(std::string)` evaluates to `"std::string"sv`.


## Compiler Support
The most optimized compilers this header file supports are GNU C++ and MSVC. If `nameof(T)` is compiled with one of those, then the resulting binary with have a `.string "T"` declaration somewhere. No extra characters, C-style null terminated.

If you use Clang, `nameof(T)` will still work but extra unused characters will be emitted to the left and right of the data location for the string view. That means `nameof(T).data()` cannot be used as a C-style string. This is due to Clang incorrectly raising errors that an argument to a `consteval` function is not a constant expression.

For all other compilers, `nameof(T)` will be converted into `std::string_view("nameof(T)")`. 
