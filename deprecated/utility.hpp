#include <concepts>
#include <cmath>

template <typename T>
requires std::integral<T> || std::floating_point<T>
constexpr T deadband(T value, T threshold) {
    if (std::abs(value) <= threshold) {
        return T{0};
    }
    return value;
}
