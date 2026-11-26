#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/CommandBuffer.hpp>

namespace Renderlib::Object
{
class VulkanCommandBuffer : public CommandBuffer
{
  public:
    VulkanCommandBuffer(const vk::raii::Device &device, const vk::raii::CommandPool &pool);
    virtual ~VulkanCommandBuffer() = default;

    VulkanCommandBuffer(const VulkanCommandBuffer &) = delete;
    VulkanCommandBuffer &operator=(const VulkanCommandBuffer &) = delete;
    VulkanCommandBuffer(VulkanCommandBuffer &&) = default;
    VulkanCommandBuffer &operator=(VulkanCommandBuffer &&) = default;

  private:
    vk::raii::CommandBuffer m_commandBuffer = nullptr;
};

} // namespace Renderlib::Object
