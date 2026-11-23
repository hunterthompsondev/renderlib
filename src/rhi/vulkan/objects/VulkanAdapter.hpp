#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/Adapter.hpp>

#include "VulkanDevice.hpp"

namespace Renderlib::Object
{
class VulkanAdapter : public Adapter
{
  public:
    VulkanAdapter(const Desc::AdapterDesc &desc, vk::raii::PhysicalDevice physicalDevice);
    ~VulkanAdapter() = default;

    VulkanAdapter(const VulkanAdapter &) = delete;
    VulkanAdapter &operator=(const VulkanAdapter &) = delete;
    VulkanAdapter(VulkanAdapter &&) = default;
    VulkanAdapter &operator=(VulkanAdapter &&) = default;

    std::vector<Desc::AdapterQueueFamilyInfo> QueryQueueFamilyProperties() const override;
    std::unique_ptr<Device> CreateDevice(const Desc::DeviceDesc &desc) const override;

  private:
    vk::raii::PhysicalDevice m_physicalDevice;
};

} // namespace Renderlib::Object
