#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/Queue.hpp>

namespace Renderlib::Object
{

class VulkanQueue : public Queue
{
  public:
    VulkanQueue(const Desc::QueueDesc &desc, const vk::raii::Device &device);
    ~VulkanQueue() = default;

    VulkanQueue(const VulkanQueue &) = delete;
    VulkanQueue &operator=(const VulkanQueue &) = delete;
    VulkanQueue(VulkanQueue &&) = default;
    VulkanQueue &operator=(VulkanQueue &&) = default;

  private:
    vk::raii::Queue m_queue;
};

} // namespace Renderlib::Object
