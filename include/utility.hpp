#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

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

inline constexpr double pi          = 3.14159265358979323846;
inline constexpr double two_pi      = pi * 2.0;
inline constexpr double half_pi     = pi / 2.0;

inline constexpr double deg_to_rad  = pi / 180.0;
inline constexpr double rad_to_deg  = 180.0 / pi;

inline constexpr double in_to_m   = 0.0254;
inline constexpr double m_to_in   = 1.0 / in_to_m;

inline constexpr double in_to_mm  = 25.4;
inline constexpr double mm_to_in  = 1.0 / in_to_mm;

inline constexpr double ft_to_m   = 0.3048;
inline constexpr double m_to_ft   = 1.0 / ft_to_m;

#endif // _UTILITY_HPP_