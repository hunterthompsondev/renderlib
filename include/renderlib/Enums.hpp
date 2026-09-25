#pragma once

#include <cstdint>
#include <vector>

enum class Format : uint8_t
{
    Undefined,
    // 8 bit channels
    R8Unorm,
    RG8Unorm,
    RGB8Unorm,
    RGBA8Unorm,
    RGBA8Srgb,
    BGRA8Unorm,
    BGRA8Srgb,
    // 16 bit channels
    R16Float,
    RG16Float,
    RGB16Float,
    RGBA16Float,
    // 32 bit channels
    R32Float,
    RG32Float,
    RGB32Float,
    RGBA32Float,
    // 32 bit aliases
    Float32,
    Float32x2,
    Float32x3,
    Float32x4,
    // Depth formats
    D16Unorm,
    D24UnormS8Int,
    D32Float,
};

enum class PrimitiveTopology : uint8_t
{
    PointList,
    LineList,
    TriangleList
};

enum class CompareOp : uint8_t
{
    Always,
    Equal,
    Greater,
    Less,
};

enum class DescriptorType : uint8_t
{
    UniformBuffer,
    StorageBuffer,
    SampledTexture,
    StorageTexture,
    Sampler,
};

enum class Filter : uint8_t
{
    Nearest,
    Linear,
};

enum class SamplerMipmapMode : uint8_t
{
    Nearest,
    Linear,
};

enum class SamplerAddressMode : uint8_t
{
    Repeat,
    ClampToEdge,
    ClampToBorder,
    MirroredRepeat,
};

enum class ImageType : uint8_t
{
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
};

enum class ImageViewType : uint8_t
{
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
};

enum class PresentMode : uint8_t
{
    Immediate,
    Fifo,
    Mailbox,
};

enum class AttachmentLoadOp : uint8_t
{
    Load,
    Clear,
    DontCare,
};

enum class AttachmentStoreOp : uint8_t
{
    Store,
    DontCare,
};

enum class IndexType : uint8_t
{
    UInt16,
    UInt32,
};

enum class BlendMode : uint8_t
{
    None,
    Opaque,
    Alpha,
    Additive,
};
