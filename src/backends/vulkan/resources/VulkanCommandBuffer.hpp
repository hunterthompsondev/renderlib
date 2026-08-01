#pragma once

#include <vulkan/vulkan_raii.hpp>

struct VulkanCommandBuffer
{
    vk::raii::CommandBuffer *commandBuffer = nullptr;
};
