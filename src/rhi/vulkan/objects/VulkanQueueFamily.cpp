#include "VulkanQueueFamily.hpp"

namespace Renderlib::Object
{

VulkanQueueFamily::VulkanQueueFamily(const Desc::QueueFamilyDesc &desc, const vk::raii::Device &device)
    : QueueFamily(desc)
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

} // namespace Renderlib::Object
