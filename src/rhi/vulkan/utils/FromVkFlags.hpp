#pragma once

#include <array>

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/types/Types.hpp>

template <typename VkBits, typename RhiEnum> struct FlagPair
{
    VkBits vk;
    RhiEnum rhi;
};

// Vulkan -> RHI: set the RHI bit for every Vulkan bit that is present
template <typename VkBits, typename RhiEnum, size_t N>
constexpr RhiEnum FromVk(vk::Flags<VkBits> flags, const std::array<FlagPair<VkBits, RhiEnum>, N> &table)
{
    RhiEnum out{}; // zero value must be None
    for (const auto &p : table)
        if (flags & p.vk)
            out |= p.rhi;
    return out;
}

// RHI -> Vulkan: set the Vulkan bit for every RHI bit that is present
template <typename VkBits, typename RhiEnum, size_t N>
constexpr vk::Flags<VkBits> ToVk(RhiEnum flags, const std::array<FlagPair<VkBits, RhiEnum>, N> &table)
{
    vk::Flags<VkBits> out{};
    for (const auto &p : table)
        if (Any(flags & p.rhi))
            out |= p.vk;
    return out;
}

namespace Renderlib
{

constexpr std::array AdapterQueueFlagsTable = {
    FlagPair<vk::QueueFlagBits, Type::AdapterQueueFlags>{vk::QueueFlagBits::eGraphics,
                                                         Type::AdapterQueueFlags::Graphics},
    FlagPair<vk::QueueFlagBits, Type::AdapterQueueFlags>{vk::QueueFlagBits::eCompute, Type::AdapterQueueFlags::Compute},
    FlagPair<vk::QueueFlagBits, Type::AdapterQueueFlags>{vk::QueueFlagBits::eTransfer,
                                                         Type::AdapterQueueFlags::Transfer},
};

inline Type::AdapterQueueFlags FromVk(vk::QueueFlags f)
{
    Type::AdapterQueueFlags c = FromVk(f, AdapterQueueFlagsTable);
    // Spec: graphics and compute families implicitly support transfer
    if (Any(c & (Type::AdapterQueueFlags::Graphics | Type::AdapterQueueFlags::Compute)))
        c |= Type::AdapterQueueFlags::Transfer;
    return c;
}

} // namespace Renderlib
