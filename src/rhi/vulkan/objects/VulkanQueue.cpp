#include "VulkanQueue.hpp"

namespace Renderlib::Object
{

VulkanQueue::VulkanQueue(const Desc::QueueDesc &desc, const vk::raii::Device &device)
    : Queue(desc), m_queue(device, desc.familyIndex, desc.queueIndex)
{
}

} // namespace Renderlib::Object
