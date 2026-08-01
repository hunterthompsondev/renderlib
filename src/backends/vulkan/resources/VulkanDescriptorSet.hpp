#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/DescriptorSet.hpp>

struct VulkanDescriptorLayout
{
    vk::raii::DescriptorSetLayout layout = nullptr;
    std::vector<DescriptorBindingCreateInfo> bindings;
};

struct VulkanDescriptorSet
{
    vk::raii::DescriptorSet set = nullptr;
    DescriptorLayoutHandle layout;
};
