#include "VulkanDevice.hpp"

#include <rhi/vulkan/utils/FromVkFlags.hpp>

namespace Renderlib::Object
{

VulkanDevice::VulkanDevice(const Desc::DeviceDesc &desc, const vk::raii::PhysicalDevice &physicalDevice) : Device(desc)
{
    // Check device extension compatibility
    std::vector<const char *> requiredExtensions = deviceExtensions;
    if (m_desc.debugMode)
    {
        requiredExtensions.insert(requiredExtensions.end(), debugDeviceExtentions.begin(), debugDeviceExtentions.end());
    }

    std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();

    auto isExtensionAvailable = [&extensionProperties](const char *extensionName) {
        return std::ranges::any_of(extensionProperties, [extensionName](const auto &available) {
            return strcmp(available.extensionName, extensionName) == 0;
        });
    };

    auto missingExtensionIt = std::ranges::find_if_not(requiredExtensions, isExtensionAvailable);

    if (missingExtensionIt != requiredExtensions.end())
    {
        throw std::runtime_error(std::string("[Vulkan Device] Required extension not supported: ") +
                                 *missingExtensionIt);
    }

    // Configure queues
    std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;
    for (const auto &queueFamily : desc.queues)
    {
        vk::DeviceQueueCreateInfo createInfo;
        createInfo.setQueueFamilyIndex(queueFamily.familyIndex).setQueuePriorities(queueFamily.priorites);

        deviceQueueCreateInfos.push_back(createInfo);
    }

    // Feature configuration
    vk::PhysicalDeviceFeatures2 f2;
    f2.features.samplerAnisotropy = true;
    f2.features.multiDrawIndirect = true;

    vk::PhysicalDeviceVulkan11Features f11;
    f11.shaderDrawParameters = true;

    vk::PhysicalDeviceVulkan12Features f12;
    f12.timelineSemaphore = true;
    f12.bufferDeviceAddress = true;

    vk::PhysicalDeviceVulkan13Features f13;
    f13.dynamicRendering = true;
    f13.synchronization2 = true;

    vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features,
                       vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features>
        featureChain{f2, f11, f12, f13};

    vk::DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.setPNext(&featureChain.get<vk::PhysicalDeviceFeatures2>())
        .setQueueCreateInfos(deviceQueueCreateInfos)
        .setPEnabledExtensionNames(requiredExtensions);

    m_device = vk::raii::Device(physicalDevice, deviceCreateInfo);

    // Create queues
    for (const auto &queue : desc.queues)
    {
        for (uint32_t i = 0; i < queue.priorites.size(); i++)
        {
            m_queues.emplace_back((Desc::QueueDesc){.familyIndex = queue.familyIndex, .queueIndex = i}, m_device);
        }
    }
}

std::vector<Queue *> VulkanDevice::GetQueues()
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
