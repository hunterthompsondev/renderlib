#pragma once

#include <renderlib/Enums.hpp>
#include <renderlib/Flags.hpp>
#include <vulkan/vulkan_raii.hpp>

// NOTE, THIS FILE WAS GENERATED ENTIRELY BY AI

// Branchless, allocation-free conversions from the abstraction's Flags<> types to Vulkan flag types.
//
// Each conversion is a handful of mask+shift operations on the raw bits (no per-bit branching, no loops),
// so it compiles down to a few ALU instructions and folds to a constant when the input is known at compile time.
// The bit layout of every mapping is verified by the static_asserts at the bottom of this file, so if you
// reorder or add a bit in Flags.hpp the build breaks instead of silently producing wrong Vulkan flags.

namespace detail
{
template <typename VkFlagsT> constexpr VkFlagsT FromRaw(uint32_t raw)
{
    return VkFlagsT(static_cast<typename VkFlagsT::MaskType>(raw));
}

template <typename VkFlagsT> constexpr uint32_t ToRaw(VkFlagsT flags)
{
    return static_cast<uint32_t>(static_cast<typename VkFlagsT::MaskType>(flags));
}
} // namespace detail

// Index=1, Vertex=2, Uniform=4, Storage=8, TransferSrc=16, TransferDst=32, Indirect=64
// -> IndexBuffer=0x40, VertexBuffer=0x80, UniformBuffer=0x10, StorageBuffer=0x20,
//    TransferSrc=0x01, TransferDst=0x02, IndirectBuffer=0x100
constexpr vk::BufferUsageFlags ToVk(BufferUsageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::BufferUsageFlags>(((r & 0x3u) << 6)      // Index, Vertex
                                                 | ((r & 0x4Cu) << 2)   // Uniform, Storage, Indirect
                                                 | ((r & 0x30u) >> 4)); // TransferSrc, TransferDst
}

// Back=1, Front=2, None=4
// -> Front=1, Back=2, None=0 (the None bit is simply dropped; Back|Front becomes FrontAndBack)
constexpr vk::CullModeFlags ToVk(CullModeFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::CullModeFlags>(((r & 0x1u) << 1) | ((r & 0x2u) >> 1));
}

// Vertex=1, Fragment=2, Compute=4, Geometry=8, TessellationControl=16, TessellationEvaluation=32
// -> eVertex=0x01, eGeometry=0x08, eFragment=0x10, eCompute=0x20,
//    eTessellationControl=0x02, eTessellationEvaluation=0x04
constexpr vk::ShaderStageFlags ToVk(ShaderStageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::ShaderStageFlags>((r & 0x9u)             // Vertex, Geometry
                                                 | ((r & 0x6u) << 3)    // Fragment, Compute
                                                 | ((r & 0x30u) >> 3)); // TessellationControl, TessellationEvaluation
}

// Sampled=1, ColorAttachment=2, DepthStencil=4, Storage=8, TransferSrc=16, TransferDst=32, InputAttachment=64
// -> Sampled=0x04, ColorAttachment=0x10, DepthStencilAttachment=0x20, Storage=0x08,
//    TransferSrc=0x01, TransferDst=0x02, InputAttachment=0x80
constexpr vk::ImageUsageFlags ToVk(ImageUsageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::ImageUsageFlags>(((r & 0x01u) << 2)     // Sampled
                                                | ((r & 0x06u) << 3)   // ColorAttachment, DepthStencil
                                                | (r & 0x08u)          // Storage (same bit)
                                                | ((r >> 4) & 0x3u)    // TransferSrc, TransferDst
                                                | ((r & 0x40u) << 1)); // InputAttachment
}

// Color=1, Depth=2, Stencil=4 -- identical to Vulkan, so just mask off unknown bits.
constexpr vk::ImageAspectFlags ToVk(ImageAspectFlags flags)
{
    return detail::FromRaw<vk::ImageAspectFlags>(flags.raw() & 0x7u);
}

// ColorAttachmentWrite=1, DepthStencilAttachmentWrite=2, ColorAttachmentRead=4, DepthStencilAttachmentRead=8,
// TransferRead=16, TransferWrite=32, ShaderRead=64, ShaderWrite=128
// -> eColorAttachmentWrite=0x100, eDepthStencilAttachmentWrite=0x400, eColorAttachmentRead=0x80,
//    eDepthStencilAttachmentRead=0x200, eTransferRead=0x800, eTransferWrite=0x1000,
//    eShaderRead=0x20, eShaderWrite=0x40
constexpr vk::AccessFlags ToVk(AccessFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::AccessFlags>(((r & 0x1u) << 8)      // ColorAttachmentWrite
                                            | ((r & 0x2u) << 9)    // DepthStencilAttachmentWrite
                                            | ((r & 0x4u) << 5)    // ColorAttachmentRead
                                            | ((r & 0x8u) << 6)    // DepthStencilAttachmentRead
                                            | ((r & 0x30u) << 7)   // TransferRead, TransferWrite
                                            | ((r & 0xC0u) >> 1)); // ShaderRead, ShaderWrite
}

// TopOfPipe=1, ColorAttachmentOutput=2, BottomOfPipe=4, EarlyFragmentTests=8, LateFragmentTests=16,
// FragmentShader=32, VertexShader=64, Transfer=128
// -> eTopOfPipe=0x1, eColorAttachmentOutput=0x400, eBottomOfPipe=0x2000, eEarlyFragmentTests=0x100,
//    eLateFragmentTests=0x200, eTransfer=0x1000, eFragmentShader=0x80, eVertexShader=0x8
constexpr vk::PipelineStageFlags ToVk(PipelineStageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::PipelineStageFlags>((r & 0x1u)           // TopOfPipe
                                                   | ((r & 0x2u) << 9)  // ColorAttachmentOutput
                                                   | ((r & 0x4u) << 11) // BottomOfPipe
                                                   |
                                                   ((r & 0x98u) << 5) // EarlyFragmentTests, LateFragmentTests, Transfer
                                                   | ((r & 0x20u) << 2)   // FragmentShader
                                                   | ((r & 0x40u) >> 3)); // VertexShader
}

// ---------------------------------------------------------------------------------------------------------------------
// Compile-time verification of every single-bit mapping
// ---------------------------------------------------------------------------------------------------------------------
namespace detail
{
#define VK_FLAG_CHECK(Fn, Bit, VkBit)                                                                                  \
    static_assert(ToRaw(ToVk(Fn(Bit))) == static_cast<uint32_t>(VkBit), "Flag mapping mismatch: " #Bit)

using B = BufferUsageFlagBits;
VK_FLAG_CHECK(BufferUsageFlags, B::Index, vk::BufferUsageFlagBits::eIndexBuffer);
VK_FLAG_CHECK(BufferUsageFlags, B::Vertex, vk::BufferUsageFlagBits::eVertexBuffer);
VK_FLAG_CHECK(BufferUsageFlags, B::Uniform, vk::BufferUsageFlagBits::eUniformBuffer);
VK_FLAG_CHECK(BufferUsageFlags, B::Storage, vk::BufferUsageFlagBits::eStorageBuffer);
VK_FLAG_CHECK(BufferUsageFlags, B::TransferSrc, vk::BufferUsageFlagBits::eTransferSrc);
VK_FLAG_CHECK(BufferUsageFlags, B::TransferDst, vk::BufferUsageFlagBits::eTransferDst);
VK_FLAG_CHECK(BufferUsageFlags, B::Indirect, vk::BufferUsageFlagBits::eIndirectBuffer);

using C = CullModeFlagBits;
VK_FLAG_CHECK(CullModeFlags, C::Back, vk::CullModeFlagBits::eBack);
VK_FLAG_CHECK(CullModeFlags, C::Front, vk::CullModeFlagBits::eFront);
VK_FLAG_CHECK(CullModeFlags, C::None, vk::CullModeFlagBits::eNone);

using S = ShaderStageFlagBits;
VK_FLAG_CHECK(ShaderStageFlags, S::Vertex, vk::ShaderStageFlagBits::eVertex);
VK_FLAG_CHECK(ShaderStageFlags, S::Fragment, vk::ShaderStageFlagBits::eFragment);
VK_FLAG_CHECK(ShaderStageFlags, S::Compute, vk::ShaderStageFlagBits::eCompute);
VK_FLAG_CHECK(ShaderStageFlags, S::Geometry, vk::ShaderStageFlagBits::eGeometry);
VK_FLAG_CHECK(ShaderStageFlags, S::TessellationControl, vk::ShaderStageFlagBits::eTessellationControl);
VK_FLAG_CHECK(ShaderStageFlags, S::TessellationEvaluation, vk::ShaderStageFlagBits::eTessellationEvaluation);

using I = ImageUsageFlagBits;
VK_FLAG_CHECK(ImageUsageFlags, I::Sampled, vk::ImageUsageFlagBits::eSampled);
VK_FLAG_CHECK(ImageUsageFlags, I::ColorAttachment, vk::ImageUsageFlagBits::eColorAttachment);
VK_FLAG_CHECK(ImageUsageFlags, I::DepthStencil, vk::ImageUsageFlagBits::eDepthStencilAttachment);
VK_FLAG_CHECK(ImageUsageFlags, I::Storage, vk::ImageUsageFlagBits::eStorage);
VK_FLAG_CHECK(ImageUsageFlags, I::TransferSrc, vk::ImageUsageFlagBits::eTransferSrc);
VK_FLAG_CHECK(ImageUsageFlags, I::TransferDst, vk::ImageUsageFlagBits::eTransferDst);
VK_FLAG_CHECK(ImageUsageFlags, I::InputAttachment, vk::ImageUsageFlagBits::eInputAttachment);

using A = ImageAspectFlagBits;
VK_FLAG_CHECK(ImageAspectFlags, A::Color, vk::ImageAspectFlagBits::eColor);
VK_FLAG_CHECK(ImageAspectFlags, A::Depth, vk::ImageAspectFlagBits::eDepth);
VK_FLAG_CHECK(ImageAspectFlags, A::Stencil, vk::ImageAspectFlagBits::eStencil);

using Acc = AccessFlagBits;
VK_FLAG_CHECK(AccessFlags, Acc::ColorAttachmentWrite, vk::AccessFlagBits::eColorAttachmentWrite);
VK_FLAG_CHECK(AccessFlags, Acc::DepthStencilAttachmentWrite, vk::AccessFlagBits::eDepthStencilAttachmentWrite);
VK_FLAG_CHECK(AccessFlags, Acc::ColorAttachmentRead, vk::AccessFlagBits::eColorAttachmentRead);
VK_FLAG_CHECK(AccessFlags, Acc::DepthStencilAttachmentRead, vk::AccessFlagBits::eDepthStencilAttachmentRead);
VK_FLAG_CHECK(AccessFlags, Acc::TransferRead, vk::AccessFlagBits::eTransferRead);
VK_FLAG_CHECK(AccessFlags, Acc::TransferWrite, vk::AccessFlagBits::eTransferWrite);
VK_FLAG_CHECK(AccessFlags, Acc::ShaderRead, vk::AccessFlagBits::eShaderRead);
VK_FLAG_CHECK(AccessFlags, Acc::ShaderWrite, vk::AccessFlagBits::eShaderWrite);

using PS = PipelineStageFlagBits;
VK_FLAG_CHECK(PipelineStageFlags, PS::TopOfPipe, vk::PipelineStageFlagBits::eTopOfPipe);
VK_FLAG_CHECK(PipelineStageFlags, PS::ColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput);
VK_FLAG_CHECK(PipelineStageFlags, PS::BottomOfPipe, vk::PipelineStageFlagBits::eBottomOfPipe);
VK_FLAG_CHECK(PipelineStageFlags, PS::EarlyFragmentTests, vk::PipelineStageFlagBits::eEarlyFragmentTests);
VK_FLAG_CHECK(PipelineStageFlags, PS::LateFragmentTests, vk::PipelineStageFlagBits::eLateFragmentTests);
VK_FLAG_CHECK(PipelineStageFlags, PS::FragmentShader, vk::PipelineStageFlagBits::eFragmentShader);
VK_FLAG_CHECK(PipelineStageFlags, PS::VertexShader, vk::PipelineStageFlagBits::eVertexShader);
VK_FLAG_CHECK(PipelineStageFlags, PS::Transfer, vk::PipelineStageFlagBits::eTransfer);

#undef VK_FLAG_CHECK
} // namespace detail
