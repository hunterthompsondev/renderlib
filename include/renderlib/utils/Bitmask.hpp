#pragma once

#include <type_traits>

namespace Renderlib::Type
{

template <typename E> struct EnableBitMask : std::false_type
{
};

template <typename E> inline constexpr bool IsBitMask = std::is_enum_v<E> && EnableBitMask<E>::value;

// Use at global namespace scope, after the enum definition.
#define RENDERLIB_ENABLE_BITMASK(E)                                                                                    \
    template <> struct EnableBitMask<E> : std::true_type                                                               \
    {                                                                                                                  \
    }

template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr E operator|(E a, E b) noexcept
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) | static_cast<U>(b));
}

template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr E operator&(E a, E b) noexcept
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) & static_cast<U>(b));
}

template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr E operator^(E a, E b) noexcept
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) ^ static_cast<U>(b));
}

template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr E operator~(E a) noexcept
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<U>(a));
}

template <typename E>
    requires IsBitMask<E>
constexpr E &operator|=(E &a, E b) noexcept
{
    return a = a | b;
}

template <typename E>
    requires IsBitMask<E>
constexpr E &operator&=(E &a, E b) noexcept
{
    return a = a & b;
}

template <typename E>
    requires IsBitMask<E>
constexpr E &operator^=(E &a, E b) noexcept
{
    return a = a ^ b;
}

// True if any bit is set.
template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr bool Any(E v) noexcept
{
    return static_cast<std::underlying_type_t<E>>(v) != 0;
}

// True if every bit in `bits` is set in `flags`.
template <typename E>
    requires IsBitMask<E>
[[nodiscard]] constexpr bool Has(E flags, E bits) noexcept
{
    return (flags & bits) == bits;
}

} // namespace Renderlib::Type
