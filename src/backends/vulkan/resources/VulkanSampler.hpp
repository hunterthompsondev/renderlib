#pragma once

#include <vulkan/vulkan_raii.hpp>

struct VulkanSampler
{
    vk::raii::Sampler sampler = nullptr;
};
