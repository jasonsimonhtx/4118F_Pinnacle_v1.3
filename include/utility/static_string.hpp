#ifndef _UTILITY_STATIC_STRING_HPP_
#define _UTILITY_STATIC_STRING_HPP_

#include <cstddef>
#include <string_view>
#include <type_traits>

namespace util {

template <std::size_t Capacity>
class StaticString {
    static_assert(Capacity > 0, "StaticString capacity must be greater than zero.");

public:
    __attribute__((always_inline)) constexpr StaticString() noexcept : m_size(0) {
        m_buffer[0] = '\0';
    }
    __attribute__((always_inline)) constexpr StaticString(const char* str) noexcept : m_size(0) {
        assign(str);
    }
    __attribute__((always_inline)) constexpr StaticString(std::string_view sv) noexcept : m_size(0) {
        assign(sv);
    }

    [[nodiscard]] __attribute__((always_inline)) constexpr const char* c_str() const noexcept { return m_buffer; }
    [[nodiscard]] __attribute__((always_inline)) constexpr const char* data() const noexcept { return m_buffer; }
    [[nodiscard]] __attribute__((always_inline)) constexpr std::size_t size() const noexcept { return m_size; }
    [[nodiscard]] __attribute__((always_inline)) constexpr std::size_t max_size() const noexcept { return Capacity; }
    [[nodiscard]] __attribute__((always_inline)) constexpr bool empty() const noexcept { return m_size == 0; }

    [[nodiscard]] __attribute__((always_inline)) constexpr operator std::string_view() const noexcept {
        return std::string_view(m_buffer, m_size);
    }

    __attribute__((target("arm"))) constexpr void clear() noexcept {
        m_size = 0;
        m_buffer[0] = '\0';
    }

    __attribute__((target("arm"))) constexpr StaticString& assign(std::string_view sv) noexcept {
        m_size = (sv.size() > Capacity) ? Capacity : sv.size();
        for (std::size_t i = 0; i < m_size; ++i) {
            m_buffer[i] = sv[i];
        }
        m_buffer[m_size] = '\0';
        return *this;
    }

    __attribute__((target("arm"))) constexpr StaticString& append(std::string_view sv) noexcept {
        std::size_t remaining = Capacity - m_size;
        if (remaining == 0) return *this;

        std::size_t to_copy = (sv.size() > remaining) ? remaining : sv.size();
        for (std::size_t i = 0; i < to_copy; ++i) {
            m_buffer[m_size + i] = sv[i];
        }
        m_size += to_copy;
        m_buffer[m_size] = '\0';
        return *this;
    }

    constexpr StaticString& operator=(std::string_view sv) noexcept { return assign(sv); }
    constexpr StaticString& operator+=(std::string_view sv) noexcept { return append(sv); }

    [[nodiscard]] constexpr char operator[](std::size_t index) const noexcept {
        return m_buffer[index];
    }
    [[nodiscard]] constexpr bool operator==(std::string_view sv) const noexcept {
        return std::string_view(*this) == sv;
    }
    [[nodiscard]] constexpr bool operator!=(std::string_view sv) const noexcept {
        return std::string_view(*this) != sv;
    }
private:
    char m_buffer[Capacity + 1];
    std::size_t m_size;
};

} // namespace util

using str16_t = util::StaticString<16>;
using str32_t = util::StaticString<16>;
using str64_t = util::StaticString<16>;

#endif // _UTILITY_STATIC_STRING_HPP_