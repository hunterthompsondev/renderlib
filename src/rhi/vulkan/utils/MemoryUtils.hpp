#pragma once

#include <cstdint>

#include <vulkan/vulkan_raii.hpp>

namespace Renderlib
{

inline uint64_t QueryDeviceLocalMemory(vk::PhysicalDevice pd)
{
    const vk::PhysicalDeviceMemoryProperties mem = pd.getMemoryProperties();

    uint64_t total = 0;
    for (uint32_t i = 0; i < mem.memoryHeapCount; ++i)
    {
        const vk::MemoryHeap &heap = mem.memoryHeaps[i];
        if (heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal)
            total += heap.size;
    }
    return total;
}

} // namespace Renderlib
