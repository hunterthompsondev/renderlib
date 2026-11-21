#include <renderlib/rhi/objects/Instance.hpp>

#include <rhi/vulkan/objects/VulkanInstance.hpp>

namespace Renderlib::Object
{
std::unique_ptr<Instance> Instance::Create(const Desc::InstanceDesc &desc)
{
    switch (desc.backend)
    {
    case Type::Backend::Vulkan:
        return VulkanInstance::Create(desc);
    default:
        return VulkanInstance::Create(desc);
    }
}

Instance::Instance(const Desc::InstanceDesc &desc) : m_desc(desc)
{
}

}; // namespace Renderlib::Object
