#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/CommandPool.hpp>

#include "VulkanCommandBuffer.hpp"

namespace Renderlib::Object
{

class VulkanCommandPool : public CommandPool
{
  public:
    VulkanCommandPool(const Desc::CommandPoolDesc &desc, const vk::raii::Device &device, uint32_t familyIndex);
    ~VulkanCommandPool() = default;

    VulkanCommandPool(const VulkanCommandPool &) = delete;
    VulkanCommandPool &operator=(const VulkanCommandPool &) = delete;

    std::unique_ptr<CommandBuffer> CreateCommandBuffer();

  private:
    const vk::raii::Device &m_device;

    vk::raii::CommandPool m_pool = nullptr;
};

} // namespace Renderlib::Object
