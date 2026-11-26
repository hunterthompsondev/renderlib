#include "VulkanCommandBuffer.hpp"

namespace Renderlib::Object
{

VulkanCommandBuffer::VulkanCommandBuffer(const vk::raii::Device &device, const vk::raii::CommandPool &pool)
    : CommandBuffer()
{
    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.setCommandPool(pool).setLevel(vk::CommandBufferLevel::ePrimary).setCommandBufferCount(1);
    vk::raii::CommandBuffers commandBuffers(device, allocInfo);
    m_commandBuffer = std::move(commandBuffers[0]);
}

} // namespace Renderlib::Object
