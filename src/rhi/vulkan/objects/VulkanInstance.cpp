#include "VulkanInstance.hpp"

#include <iostream>

namespace Renderlib::Object
{

std::unique_ptr<VulkanInstance> VulkanInstance::Create(const Desc::InstanceDesc &desc)
{
    return std::unique_ptr<VulkanInstance>(new VulkanInstance(desc));
}

VulkanInstance::VulkanInstance(const Desc::InstanceDesc &desc) : Instance(desc)
{
    std::cout << "Vulkan Instance Created\n";
}

} // namespace Renderlib::Object
