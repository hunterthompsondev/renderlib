#include <renderlib/RenderDevice.hpp>

#include <backends/vulkan/device/VulkanDevice.hpp>
#include <backends/vulkan/renderDevice/VulkanRenderDevice.hpp>

std::unique_ptr<RenderDevice> RenderDevice::Create(const RenderDeviceCreateInfo &desc)
{
    std::unique_ptr<VulkanRenderDevice> renderDevice = std::make_unique<VulkanRenderDevice>();
    renderDevice->m_device = std::make_unique<VulkanDevice>();
    // if (!renderDevice->Initialize() || !device->Initialize())
    // {
    //     return nullptr;
    // }
    return renderDevice;
}

Device *RenderDevice::GetDevice()
{
    return m_device.get();
}
