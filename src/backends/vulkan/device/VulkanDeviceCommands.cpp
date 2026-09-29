#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/ToVkEnums.hpp>
#include <backends/vulkan/utils/ToVkFlags.hpp>

void VulkanDevice::SetViewport(CommandBufferHandle cmdHandle, float x, float y, float width, float height)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->buffer.setViewport(0, vk::Viewport{x, y, width, height, 0.0f, 1.0f});
}

void VulkanDevice::SetScissor(CommandBufferHandle cmdHandle, int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->buffer.setScissor(0, vk::Rect2D{{x, y}, {width, height}});
}

void VulkanDevice::Draw(CommandBufferHandle cmdHandle, uint32_t vertexCount, uint32_t instanceCount,
                        uint32_t firstVertex, uint32_t firstInstance)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->buffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanDevice::DrawIndexed(CommandBufferHandle cmdHandle, uint32_t indexCount, uint32_t instanceCount,
                               uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    cmdImpl->buffer.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanDevice::PushConstants(CommandBufferHandle cmdHandle, PipelineHandle pipeline, ShaderStageFlags stages,
                                 uint32_t offset, uint32_t size, const void *data)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
        return;
    vk::PipelineLayout layout = GetVulkanPipelineLayout(pipeline);
    cmdImpl->buffer.pushConstants(layout, ToVk(stages), offset, size, data);
}

void VulkanDevice::BindPipeline(CommandBufferHandle cmdHandle, PipelineHandle pipeline)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, GetVulkanPipeline(pipeline));
}

void VulkanDevice::BindVertexBuffer(CommandBufferHandle cmdHandle, BufferHandle buffer, uint32_t binding,
                                    uint64_t offset)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    vk::Buffer vkBuffer = GetVulkanBuffer(buffer);
    vk::DeviceSize vkOffset = offset;
    cmdImpl->buffer.bindVertexBuffers(binding, vkBuffer, vkOffset);
}

void VulkanDevice::BindIndexBuffer(CommandBufferHandle cmdHandle, BufferHandle buffer, IndexType indexType,
                                   uint64_t offset)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    vk::Buffer vkBuffer = GetVulkanBuffer(buffer);
    cmdImpl->buffer.bindIndexBuffer(vkBuffer, offset, ToVk(indexType));
}

void VulkanDevice::BindDescriptorSet(CommandBufferHandle cmdHandle, PipelineHandle pipeline,
                                     DescriptorSetHandle setHandle, uint32_t setIndex)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    vk::PipelineLayout layout = GetVulkanPipelineLayout(pipeline);
    vk::DescriptorSet set = GetVulkanDescriptorSet(setHandle);

    cmdImpl->buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, setIndex, set, {});
}
