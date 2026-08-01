#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/ShaderModule.hpp>

struct VulkanShaderModule
{
    vk::raii::ShaderModule module = nullptr;
    ShaderStage stage;
    std::string entryPoint;
};
