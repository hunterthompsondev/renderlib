#include "VulkanDevice.hpp"

#include <iostream>

#include "backends/vulkan/resources/VulkanShaderModule.hpp"

ShaderModuleHandle VulkanDevice::CreateShaderModule(const ShaderModuleCreateInfo &desc)
{
    if (desc.bytecode.size() % 4 != 0)
    {
        std::cerr << "[Vulkan Device] Shader bytecode size must be a multiple of 4\n";
        return {};
    }

    VulkanShaderModule shaderModule{
        .stage = desc.stage,
        .entryPoint = desc.entryPoint,
    };

    vk::ShaderModuleCreateInfo moduleCI{};
    moduleCI.setCodeSize(desc.bytecode.size()).setPCode(reinterpret_cast<const uint32_t *>(desc.bytecode.data()));

    // Create shader module
    shaderModule.module = vk::raii::ShaderModule(m_context.device, moduleCI);

    return m_shaderModulePool.Create(std::move(shaderModule));
}

void VulkanDevice::DestroyShaderModule(ShaderModuleHandle handle)
{
    m_shaderModulePool.Destroy(handle);
}
