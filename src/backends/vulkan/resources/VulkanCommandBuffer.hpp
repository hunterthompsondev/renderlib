#pragma once

#include <vulkan/vulkan_raii.hpp>

struct VulkanCommandBuffer
{
    vk::raii::CommandPool pool = nullptr;
    vk::raii::CommandBuffer buffer = nullptr;
};
