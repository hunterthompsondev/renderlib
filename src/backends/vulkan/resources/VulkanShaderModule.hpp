#pragma once

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/ShaderModule.hpp>

struct VulkanShaderModule
{
    vk::raii::ShaderModule module = nullptr;
    vk::ShaderStageFlags stage;
    std::string entryPoint;
};
