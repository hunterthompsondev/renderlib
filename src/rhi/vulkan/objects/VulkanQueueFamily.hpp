#pragma once

#include <deque>
#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/QueueFamily.hpp>

#include "VulkanCommandPool.hpp"
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

    std::vector<Queue *> GetQueues() override;
    std::unique_ptr<CommandPool> CreateCommandPool(const Desc::CommandPoolDesc &desc) override;

  private:
    const vk::raii::Device &m_device;

    std::deque<VulkanQueue> m_queues;
};

} // namespace Renderlib::Object
