#pragma once

#include <cstdint>
#include <vector>

// Buffer Enums
enum class BufferType
{
    Vertex,
    Index,
    Uniform,
    Storage,
};

enum class BufferUsage
{
    Static,
    Dynamic,
};

// Descriptor Set
enum class DescriptorType
{
    UniformBuffer,
    StorageBuffer,
    SampledTexture,
    StorageTexture,
    Sampler,
};

enum class ShaderStageFlags : uint32_t
{
    None = 0,
    Vertex = 1 << 0,
    Fragment = 1 << 1,
    Compute = 1 << 2,
    All = Vertex | Fragment | Compute,
};

// Pipeline
enum class VertexFormat
{
    Float32,
    Float32x2,
    Float32x3,
    Float32x4,
    UInt8x4Norm,
};

struct VertexAttribute
{
    uint32_t location;
    uint32_t offset;
    VertexFormat format;
};

struct VertexLayout
{
    uint32_t stride;
    std::vector<VertexAttribute> attributes;
};

enum class PrimitiveTopology
{
    PointList,
    LineList,
    TriangleList,
};

enum class CullMode
{
    None,
    Front,
    Back,
};

enum class BlendMode
{
    Opaque,
    AlphaBlend,
    Additive,
};

// Sampler
enum class FilterMode
{
    Nearest,
    Linear,
};

enum class AddressMode
{
    Repeat,
    ClampToEdge,
    ClampToBorder,
    MirroredRepeat,
};

// Shader module
enum class ShaderStage
{
    Vertex,
    Fragment,
};

// Swapchain
enum class PresentMode
{
    Immediate,
    Fifo,
    Mailbox,
};

// Texture
enum class TextureType
{
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
};

enum class TextureUsage : uint32_t
{
    Sampled = 1 << 0,
    RenderTarget = 1 << 1,
    DepthStencil = 1 << 2,
    Storage = 1 << 3,
    TransferSrc = 1 << 4,
    TransferDst = 1 << 5,
};

// Misc
enum class Format
{
    Undefined = 0,
    R8Unorm,
    RG8Unorm,
    RGBA8Unorm,
    RGBA8Srgb,
    BGRA8Unorm,
    BGRA8Srgb,
    R16Float,
    RG16Float,
    RGBA16Float,
    R32Float,
    RGBA32Float,
    D16Unorm,
    D32Float,
    D24UnormS8Uint,
    D32FloatS8Uint,
};

enum class CompareOp
{
    Less,
    Greater,
    Always,
    Equal,
};

enum class LoadOp
{
    Load,
    Clear,
    DontCare,
};

enum class StoreOp
{
    Store,
    DontCare,
};

enum class IndexType : uint8_t
{
    UInt16,
    UInt32,
};
