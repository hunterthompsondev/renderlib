#include "VulkanCommandPool.hpp"

namespace Renderlib::Object
{

VulkanCommandPool::VulkanCommandPool(const Desc::CommandPoolDesc &desc, const vk::raii::Device &device,
                                     uint32_t familyIndex)
    : CommandPool(desc), m_device(device)
{
    vk::CommandPoolCreateFlags flags;
    if (desc.transient)
    {
        flags |= vk::CommandPoolCreateFlagBits::eTransient;
    }

    vk::CommandPoolCreateInfo poolCreateInfo;
    poolCreateInfo.setQueueFamilyIndex(familyIndex).setFlags(flags);
    m_pool = vk::raii::CommandPool(device, poolCreateInfo);
}

std::unique_ptr<CommandBuffer> VulkanCommandPool::CreateCommandBuffer()
{
    return std::make_unique<VulkanCommandBuffer>(m_device, m_pool);
}

} // namespace Renderlib::Object
