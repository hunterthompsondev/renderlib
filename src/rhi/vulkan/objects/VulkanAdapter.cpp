#include "VulkanAdapter.hpp"

#include <rhi/vulkan/utils/FromVkFlags.hpp>

namespace Renderlib::Object
{

VulkanAdapter::VulkanAdapter(const Desc::AdapterDesc &desc, vk::raii::PhysicalDevice physicalDevice)
    : Adapter(desc), m_physicalDevice(std::move(physicalDevice))
{
}

std::vector<Desc::AdapterQueueFamilyInfo> VulkanAdapter::QueryQueueFamilyProperties() const
{
    std::vector<Desc::AdapterQueueFamilyInfo> queueFamilies;

    std::vector<vk::QueueFamilyProperties> queueProperties = m_physicalDevice.getQueueFamilyProperties();
    for (const auto &props : queueProperties)
    {
        Desc::AdapterQueueFamilyInfo desc;
        desc.flags = FromVk(props.queueFlags);
        desc.count = props.queueCount;

        queueFamilies.push_back(desc);
    }

    return queueFamilies;
}

std::unique_ptr<Device> VulkanAdapter::CreateDevice(const Desc::DeviceDesc &desc) const
{
    return std::make_unique<VulkanDevice>(desc, m_physicalDevice);
}
} // namespace Renderlib::Object
