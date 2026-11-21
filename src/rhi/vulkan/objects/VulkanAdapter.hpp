#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/Adapter.hpp>

namespace Renderlib::Object
{
class VulkanAdapter : public Adapter
{
  public:
    VulkanAdapter(const Desc::AdapterDesc &desc, vk::raii::PhysicalDevice physicalDevice);
    ~VulkanAdapter() = default;

    VulkanAdapter(const VulkanAdapter &) = delete;
    VulkanAdapter &operator=(const VulkanAdapter &) = delete;
    VulkanAdapter(VulkanAdapter &) = delete;
    VulkanAdapter &operator=(VulkanAdapter &) = delete;

  private:
    vk::raii::PhysicalDevice m_physicalDevice;
};

} // namespace Renderlib::Object
