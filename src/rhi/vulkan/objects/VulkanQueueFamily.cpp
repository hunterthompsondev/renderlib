#include "VulkanQueueFamily.hpp"

namespace Renderlib::Object
{

VulkanQueueFamily::VulkanQueueFamily(const Desc::QueueFamilyDesc &desc, const vk::raii::Device &device)
    : QueueFamily(desc), m_device(device)
{
    for (uint32_t i = 0; i < m_desc.queueCount; i++)
    {
        m_queues.emplace_back((Desc::QueueDesc){.familyIndex = m_desc.familyIndex, .queueIndex = i}, device);
    }
}

std::vector<Queue *> VulkanQueueFamily::GetQueues()
{
    std::vector<Queue *> ptrs;
    ptrs.reserve(m_queues.size());
    for (VulkanQueue &q : m_queues)
    {
        ptrs.push_back(&q);
    }
    return ptrs;
}

std::unique_ptr<CommandPool> VulkanQueueFamily::CreateCommandPool(const Desc::CommandPoolDesc &desc)
{
    return std::make_unique<VulkanCommandPool>(desc, m_device, m_desc.familyIndex);
}

} // namespace Renderlib::Object
