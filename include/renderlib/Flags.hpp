#pragma once

#include <cstdint>
#include <type_traits>

// Helper method to define bit mask operators for enums
template <typename E> class Flags
{
    static_assert(std::is_enum_v<E>);
    using U = std::underlying_type_t<E>;
    U m_bits = 0;

    constexpr explicit Flags(U raw) : m_bits(raw)
    {
    }

  public:
    constexpr Flags() = default;
    constexpr Flags(E bit) : m_bits(static_cast<U>(bit))
    {
    }

    static constexpr Flags fromRaw(U raw)
    {
        return Flags(raw);
    }
    constexpr U raw() const
    {
        return m_bits;
    }

    constexpr explicit operator bool() const
    {
        return m_bits != 0;
    }
    constexpr bool operator!() const
    {
        return m_bits == 0;
    }

    constexpr Flags operator|(Flags o) const
    {
        return Flags(m_bits | o.m_bits);
    }
    constexpr Flags operator&(Flags o) const
    {
        return Flags(m_bits & o.m_bits);
    }
    constexpr Flags operator^(Flags o) const
    {
        return Flags(m_bits ^ o.m_bits);
    }
    constexpr Flags operator~() const
    {
        return Flags(~m_bits);
    }

    constexpr Flags &operator|=(Flags o)
    {
        m_bits |= o.m_bits;
        return *this;
    }
    constexpr Flags &operator&=(Flags o)
    {
        m_bits &= o.m_bits;
        return *this;
    }
    constexpr Flags &operator^=(Flags o)
    {
        m_bits ^= o.m_bits;
        return *this;
    }

    constexpr bool hasAll(Flags f) const
    {
        return (m_bits & f.m_bits) == f.m_bits;
    }
    constexpr bool hasAny(Flags f) const
    {
        return (m_bits & f.m_bits) != 0;
    }
    constexpr Flags without(Flags f) const
    {
        return Flags(m_bits & ~f.m_bits);
    }

    friend constexpr bool operator==(Flags, Flags) = default;
};

#define DEFINE_FLAGS(Bit, Name)                                                                                        \
    using Name = Flags<Bit>;                                                                                           \
    constexpr Name operator|(Bit a, Bit b)                                                                             \
    {                                                                                                                  \
        return Name(a) | Name(b);                                                                                      \
    }

enum class BufferUsageFlagBits : uint8_t
{
    Index = 1,
    Vertex = 1 << 1,
    Uniform = 1 << 2,
    Storage = 1 << 3,
};
DEFINE_FLAGS(BufferUsageFlagBits, BufferUsageFlags)

enum class CullModeFlagBits : uint8_t
{
    Back = 1,
    Front = 1 << 1,
    None = 1 << 2,
};
DEFINE_FLAGS(CullModeFlagBits, CullModeFlags)

enum class ShaderStageFlagBits : uint8_t
{
    Vertex = 1,
    Fragment = 1 << 1,
};
DEFINE_FLAGS(ShaderStageFlagBits, ShaderStageFlags)

enum class ImageUsageFlagBits : uint8_t
{
    Sampled = 1,
    RenderTarget = 1 << 1,
    DepthStencil = 1 << 2,
    Storage = 1 << 3,
    TransferSrc = 1 << 4,
    TransferDst = 1 << 5,
};
DEFINE_FLAGS(ImageUsageFlagBits, ImageUsageFlags)

enum class ImageAspectFlagBits : uint8_t
{
    Color = 1,
    Depth = 1 << 1,
    Stencil = 1 << 2,
};
DEFINE_FLAGS(ImageAspectFlagBits, ImageAspectFlags)
