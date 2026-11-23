#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/Device.hpp>

// Maybe move to contstructor argument in the future.
static std::vector<const char *> debugDeviceExtentions = {};
static std::vector<const char *> deviceExtensions = {};

namespace Renderlib::Object
{
class VulkanDevice : public Device
{
  public:
    VulkanDevice(const Desc::DeviceDesc &desc, const vk::raii::PhysicalDevice &physicalDevice);
    ~VulkanDevice() = default;

    VulkanDevice(const VulkanDevice &) = delete;
    VulkanDevice &operator=(const VulkanDevice &) = delete;
    VulkanDevice(VulkanDevice &) = delete;
    VulkanDevice &operator=(VulkanDevice &) = delete;

  private:
    vk::raii::Device m_device = nullptr;
};

} // namespace Renderlib::Object
