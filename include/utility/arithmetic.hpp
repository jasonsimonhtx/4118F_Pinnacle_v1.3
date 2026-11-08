#ifndef _UTILITY_FUNCTIONS_HPP_
#define _UTILITY_FUNCTIONS_HPP_

#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr T ceiling(T val, T ceiling) noexcept {
    return (val > ceiling) ? ceiling : val;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr T floor(T val, T floor) noexcept {
    return (val < floor) ? floor : val;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr T clamp(T val, T low, T high) noexcept {
    return floor(ceiling(val, high), low);
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr T abs(T val) noexcept {
    return (val < 0) ? -val : val;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr T deadband(T val, T threshold) noexcept {
    return (abs(val) < threshold) ? T(0) : val;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
__attribute__((always_inline, target("arm"))) inline constexpr int sign(T val) noexcept {
    return (T(0) < val) - (val < T(0));
}

#endif // _UTILITY_FUNCTIONS_HPP_