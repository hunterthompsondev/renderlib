#include "VulkanAdapter.hpp"

namespace Renderlib::Object
{

VulkanAdapter::VulkanAdapter(const Desc::AdapterDesc &desc, vk::raii::PhysicalDevice physicalDevice)
    : Adapter(desc), m_physicalDevice(std::move(physicalDevice))
{
}

} // namespace Renderlib::Object
