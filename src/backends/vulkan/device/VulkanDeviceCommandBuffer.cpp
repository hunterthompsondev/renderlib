#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/ToVkEnums.hpp>
#

CommandBufferHandle VulkanDevice::CreateCommandBuffer()
{
    VulkanCommandBuffer commandBuffer;

    vk::CommandPoolCreateInfo commandPoolInfo = vk::CommandPoolCreateInfo{}
                                                    .setQueueFamilyIndex(m_context.graphicsQueueIndex)
                                                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    commandBuffer.pool = vk::raii::CommandPool(m_context.device, commandPoolInfo);

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.setCommandPool(commandBuffer.pool).setLevel(vk::CommandBufferLevel::ePrimary).setCommandBufferCount(1);

    vk::raii::CommandBuffers commandBuffers(m_context.device, allocInfo);
    commandBuffer.buffer = std::move(commandBuffers[0]);

    return m_commandBufferPool.Create(std::move(commandBuffer));
}

void VulkanDevice::DestroyCommandBuffer(CommandBufferHandle handle)
{
    m_commandBufferPool.Destroy(handle);
}
