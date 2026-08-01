#include "VulkanRenderDevice.hpp"

bool VulkanRenderDevice::Initialize()
{
    VulkanDevice *device = GetVulkanDevice();
    VulkanContext *context = &device->m_context;

    return true;
}

VulkanRenderDevice::~VulkanRenderDevice()
{
    if (m_device)
    {
        GetVulkanDevice()->WaitIdle();
    }
}
