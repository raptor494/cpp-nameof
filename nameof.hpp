// Authors: github.com/raptor494, Jason Turner
// Adapted from an example given by Jason Turner in https://www.youtube.com/watch?v=ABg4_EV5L3w&t=644s
#ifndef nameof

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace __nameof_utils {

// Only supported compilers are GNU C++, Clang, and MSVC.
#if defined __GNUG__ || defined _MSC_VER

    template<typename T>
    consteval std::string_view get_typename_raw() {
        using namespace std::string_view_literals;
        constexpr std::string_view
        #ifdef __GNUG__
            str = __PRETTY_FUNCTION__,
        # ifndef __clang__
            prefix = "get_typename_raw() [with T = "sv,
            suffix = "; std::string_view = std::basic_string_view<char>]"sv
        # else
            prefix = "get_typename_raw() [T = "sv,
            suffix = "]"sv
        # endif
        #elif defined _MSC_VER
            str = __FUNCSIG__,
            prefix = "get_typename_raw<"sv,
            suffix = ">(void)"sv
        #else
        # error Unsupported compiler!
        #endif
        ;
        // constexpr size_t start_offset = str.find(prefix) + prefix.length();
        // constexpr size_t end_offset = suffix.length();
        return str;
        // return std::string_view(str.begin() + start_offset, str.length() - start_offset - end_offset);
    }

#endif

#ifdef __GNUG__

  #ifndef __clang__
  // compiler is GNU C++ -> full implementation

    struct oversized_array {
        static constexpr size_t max_size = 10 * 1024 * 1024;
        std::array<char, max_size> data{};
        size_t size;
    };

    constexpr auto to_oversized_array(const std::string &str) {   
        oversized_array result;
        std::copy(str.begin(), str.begin() + std::min(oversized_array::max_size - 1, str.length()), result.data.begin());
        result.size = str.length() + 1;
        return result;
    }

    consteval auto to_right_sized_array(auto callable) {
        constexpr auto oversized = to_oversized_array(callable());
        std::array<char, oversized.size> result;
        std::copy(oversized.data.begin(), std::next(oversized.data.begin(), oversized.size), result.begin());
        return result;
    }

    template<auto data>
    consteval const auto &make_static() {
        return data;
    }

    consteval std::string_view to_string_view(auto callable) {
        constexpr auto &static_data = make_static<to_right_sized_array(callable)>();
        return std::string_view{static_data.begin(), static_data.size()};
    }

    template<typename T>
    consteval std::string_view get_typename() {
        return to_string_view([] { return std::string(get_typename_raw<T>()); });
    }

    #define nameof(T, ...) __nameof_utils::get_typename<T,##__VA_ARGS__>()

  #else // __clang__ defined
  // compiler is Clang -> partial implementation

    #define nameof(T, ...) __nameof_utils::get_typename_raw<T __VA_OPT__(,) __VA_ARGS__>()

  #endif
#elif defined _MSC_VER
// compiler is MSVC -> partial implementation

    #define nameof(T, ...) __nameof_utils::get_typename_raw<T,##__VA_ARGS__>()

#else
// unsupported compiler -> unhelpful implementation

    #define nameof(...) std::string_view("nameof(" #__VA_ARGS__ ")")

#endif

}

#endif // include guard
