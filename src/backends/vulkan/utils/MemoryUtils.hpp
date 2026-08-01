#pragma once

#include <vulkan/vulkan_raii.hpp>

inline uint32_t FindMemoryType(vk::raii::PhysicalDevice &physicalDevice, uint32_t typeFilter,
                               vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    return UINT32_MAX;
}
