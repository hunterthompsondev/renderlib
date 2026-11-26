#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/CommandPool.hpp>

namespace Renderlib::Object
{

class VulkanCommandPool : public CommandPool
{
  public:
    VulkanCommandPool(const Desc::CommandPoolDesc &desc, const vk::raii::Device &device, uint32_t familyIndex);
    ~VulkanCommandPool() = default;

    VulkanCommandPool(const VulkanCommandPool &) = delete;
    VulkanCommandPool &operator=(const VulkanCommandPool &) = delete;
    VulkanCommandPool(VulkanCommandPool &&) = default;
    VulkanCommandPool &operator=(VulkanCommandPool &&) = default;

  private:
    vk::raii::CommandPool m_pool = nullptr;
};

} // namespace Renderlib::Object
