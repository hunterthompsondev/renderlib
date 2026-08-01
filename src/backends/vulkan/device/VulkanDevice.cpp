#include "VulkanDevice.hpp"

#include <iostream>

bool VulkanDevice::Initialize()
{
    if (!m_context.Initialize(true))
    {
        std::cerr << "[Vulkan Device] Failed to initialize Vulkan context\n";
        return false;
    }
    return true;
}
