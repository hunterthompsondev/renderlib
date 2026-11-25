#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/QueueFamily.hpp>

#include "VulkanQueue.hpp"

namespace Renderlib::Object
{

class VulkanQueueFamily : public QueueFamily
{
  public:
    VulkanQueueFamily(const Desc::QueueFamilyDesc &desc, const vk::raii::Device &device);
    ~VulkanQueueFamily() = default;

    VulkanQueueFamily(const VulkanQueueFamily &) = delete;
    VulkanQueueFamily &operator=(const VulkanQueueFamily &) = delete;
    VulkanQueueFamily(VulkanQueueFamily &&) = default;
    VulkanQueueFamily &operator=(VulkanQueueFamily &&) = default;

    std::vector<Queue *> GetQueues() override;

  private:
    std::vector<VulkanQueue> m_queues;
};

} // namespace Renderlib::Object
