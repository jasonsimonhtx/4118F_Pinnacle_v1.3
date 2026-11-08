#ifndef _UTILITY_CONSTANTS_HPP_
#define _UTILITY_CONSTANTS_HPP_

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

#endif // _UTILITY_CONSTANTS_HPP_