#include <renderlib/Device.hpp>

#include <backends/vulkan/device/VulkanDevice.hpp>

std::unique_ptr<Device> Device::Create(const DeviceCreateInfo &desc)
{
    std::unique_ptr<VulkanDevice> device = std::make_unique<VulkanDevice>();
    // if (!device->Initialize())
    // {
    //     return nullptr;
    // }
    return device;
}
