#include "VulkanAdapter.hpp"

#include <rhi/vulkan/utils/FromVkFlags.hpp>

namespace Renderlib::Object
{

VulkanAdapter::VulkanAdapter(const Desc::AdapterDesc &desc, vk::raii::PhysicalDevice physicalDevice)
    : Adapter(desc), m_physicalDevice(std::move(physicalDevice))
{
}

std::vector<Desc::QueueFamilyDesc> VulkanAdapter::QueryQueueFamilyProperties() const
{
    std::vector<Desc::QueueFamilyDesc> queueFamilies;

    std::vector<vk::QueueFamilyProperties> queueProperties = m_physicalDevice.getQueueFamilyProperties();
    for (const auto &props : queueProperties)
    {
        Desc::QueueFamilyDesc desc;
        desc.flags = FromVk(props.queueFlags);
        desc.count = props.queueCount;

        queueFamilies.push_back(desc);
    }

    return queueFamilies;
}

} // namespace Renderlib::Object
