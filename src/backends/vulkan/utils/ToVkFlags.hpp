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

// Index=1, Vertex=2, Uniform=4, Storage=8
// -> IndexBuffer=0x40, VertexBuffer=0x80, UniformBuffer=0x10, StorageBuffer=0x20
constexpr vk::BufferUsageFlags ToVk(BufferUsageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::BufferUsageFlags>(((r & 0x3u) << 6) | ((r & 0xCu) << 2));
}

// Back=1, Front=2, None=4
// -> Front=1, Back=2, None=0 (the None bit is simply dropped; Back|Front becomes FrontAndBack)
constexpr vk::CullModeFlags ToVk(CullModeFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::CullModeFlags>(((r & 0x1u) << 1) | ((r & 0x2u) >> 1));
}

// Vertex=1, Fragment=2 -> Vertex=0x01, Fragment=0x10
constexpr vk::ShaderStageFlags ToVk(ShaderStageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::ShaderStageFlags>((r & 0x1u) | ((r & 0x2u) << 3));
}

// Sampled=1, RenderTarget=2, DepthStencil=4, Storage=8, TransferSrc=16, TransferDst=32
// -> Sampled=0x04, ColorAttachment=0x10, DepthStencilAttachment=0x20, Storage=0x08, TransferSrc=0x01, TransferDst=0x02
constexpr vk::ImageUsageFlags ToVk(ImageUsageFlags flags)
{
    const uint32_t r = flags.raw();
    return detail::FromRaw<vk::ImageUsageFlags>(((r & 0x01u) << 2)    // Sampled
                                                | ((r & 0x06u) << 3)  // RenderTarget, DepthStencil
                                                | (r & 0x08u)         // Storage (same bit)
                                                | ((r >> 4) & 0x3u)); // TransferSrc, TransferDst
}

// Color=1, Depth=2, Stencil=4 -- identical to Vulkan, so just mask off unknown bits.
constexpr vk::ImageAspectFlags ToVk(ImageAspectFlags flags)
{
    return detail::FromRaw<vk::ImageAspectFlags>(flags.raw() & 0x7u);
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

using C = CullModeFlagBits;
VK_FLAG_CHECK(CullModeFlags, C::Back, vk::CullModeFlagBits::eBack);
VK_FLAG_CHECK(CullModeFlags, C::Front, vk::CullModeFlagBits::eFront);
VK_FLAG_CHECK(CullModeFlags, C::None, vk::CullModeFlagBits::eNone);
static_assert(ToRaw(ToVk(CullModeFlags(C::Back) | C::Front)) ==
                  static_cast<uint32_t>(vk::CullModeFlagBits::eFrontAndBack),
              "Back|Front must map to FrontAndBack");

using S = ShaderStageFlagBits;
VK_FLAG_CHECK(ShaderStageFlags, S::Vertex, vk::ShaderStageFlagBits::eVertex);
VK_FLAG_CHECK(ShaderStageFlags, S::Fragment, vk::ShaderStageFlagBits::eFragment);

using I = ImageUsageFlagBits;
VK_FLAG_CHECK(ImageUsageFlags, I::Sampled, vk::ImageUsageFlagBits::eSampled);
VK_FLAG_CHECK(ImageUsageFlags, I::RenderTarget, vk::ImageUsageFlagBits::eColorAttachment);
VK_FLAG_CHECK(ImageUsageFlags, I::DepthStencil, vk::ImageUsageFlagBits::eDepthStencilAttachment);
VK_FLAG_CHECK(ImageUsageFlags, I::Storage, vk::ImageUsageFlagBits::eStorage);
VK_FLAG_CHECK(ImageUsageFlags, I::TransferSrc, vk::ImageUsageFlagBits::eTransferSrc);
VK_FLAG_CHECK(ImageUsageFlags, I::TransferDst, vk::ImageUsageFlagBits::eTransferDst);

using A = ImageAspectFlagBits;
VK_FLAG_CHECK(ImageAspectFlags, A::Color, vk::ImageAspectFlagBits::eColor);
VK_FLAG_CHECK(ImageAspectFlags, A::Depth, vk::ImageAspectFlagBits::eDepth);
VK_FLAG_CHECK(ImageAspectFlags, A::Stencil, vk::ImageAspectFlagBits::eStencil);

#undef VK_FLAG_CHECK
} // namespace detail
