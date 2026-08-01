#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/Buffer.hpp>

struct VulkanBuffer
{
    vk::raii::Buffer buffer = nullptr;
    vk::raii::DeviceMemory memory = nullptr;
    vk::DeviceSize size;

    BufferType type;
    BufferUsage usage;
};
