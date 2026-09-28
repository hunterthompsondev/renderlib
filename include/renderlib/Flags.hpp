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

// NOTE: all flag-bit enums below are backed by uint8_t (8 bits). Several are now fully or nearly
// packed (see per-enum comments) -- widen the underlying type if you need to add more bits later.

enum class BufferUsageFlagBits : uint8_t
{
    Index = 1,
    Vertex = 1 << 1,
    Uniform = 1 << 2,
    Storage = 1 << 3,
    TransferSrc = 1 << 4,
    TransferDst = 1 << 5,
    Indirect = 1 << 6, // buffer usable as the argument source for indirect draw/dispatch calls
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
    Compute = 1 << 2,
    Geometry = 1 << 3,
    TessellationControl = 1 << 4,
    TessellationEvaluation = 1 << 5,
};
DEFINE_FLAGS(ShaderStageFlagBits, ShaderStageFlags)

enum class ImageUsageFlagBits : uint8_t
{
    Sampled = 1,
    ColorAttachment = 1 << 1,
    DepthStencil = 1 << 2,
    Storage = 1 << 3,
    TransferSrc = 1 << 4,
    TransferDst = 1 << 5,
    InputAttachment = 1 << 6, // readable as a subpass input attachment (e.g. deferred shading G-buffer read)
};
DEFINE_FLAGS(ImageUsageFlagBits, ImageUsageFlags)

enum class ImageAspectFlagBits : uint8_t
{
    Color = 1,
    Depth = 1 << 1,
    Stencil = 1 << 2,
};
DEFINE_FLAGS(ImageAspectFlagBits, ImageAspectFlags)

// Fully packed: all 8 bits of uint8_t are now in use.
enum class AccessFlagBits : uint8_t
{
    ColorAttachmentWrite = 1,
    DepthStencilAttachmentWrite = 1 << 1,
    ColorAttachmentRead = 1 << 2,
    DepthStencilAttachmentRead = 1 << 3,
    TransferRead = 1 << 4,
    TransferWrite = 1 << 5,
    ShaderRead = 1 << 6,
    ShaderWrite = 1 << 7,
};
DEFINE_FLAGS(AccessFlagBits, AccessFlags)

// Fully packed: all 8 bits of uint8_t are now in use.
enum class PipelineStageFlagBits : uint8_t
{
    TopOfPipe = 1,
    ColorAttachmentOutput = 1 << 1,
    BottomOfPipe = 1 << 2,
    EarlyFragmentTests = 1 << 3,
    LateFragmentTests = 1 << 4,
    FragmentShader = 1 << 5,
    VertexShader = 1 << 6,
    Transfer = 1 << 7,
};
DEFINE_FLAGS(PipelineStageFlagBits, PipelineStageFlags)
