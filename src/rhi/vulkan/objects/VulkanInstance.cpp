#include "VulkanInstance.hpp"

#include <iostream>

#include <vulkan/vulkan_raii.hpp>

#include <rhi/vulkan/utils/MemoryUtils.hpp>
#include <rhi/vulkan/utils/ToCommon.hpp>

static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                      vk::DebugUtilsMessageTypeFlagsEXT type,
                                                      const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                      void *)
{
    if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError ||
        severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
    {
        std::cerr << "[Vulkan Instance] validation layer: type " << to_string(type)
                  << " msg: " << pCallbackData->pMessage << std::endl;
    }

    return vk::False;
}

namespace Renderlib::Object
{

std::unique_ptr<VulkanInstance> VulkanInstance::Create(const Desc::InstanceDesc &desc)
{
    return std::unique_ptr<VulkanInstance>(new VulkanInstance(desc));
}

VulkanInstance::VulkanInstance(const Desc::InstanceDesc &desc) : Instance(desc)
{
    vk::ApplicationInfo appInfo = vk::ApplicationInfo{}
                                      .setPApplicationName("renderlib")
                                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setPEngineName("renderlib")
                                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setApiVersion(vk::ApiVersion14);

    // Check layer compatibility
    std::vector<const char *> requiredLayers = layers;
    if (m_desc.debugMode)
    {
        requiredLayers.insert(requiredLayers.end(), debugLayers.begin(), debugLayers.end());
    }

    std::vector<vk::LayerProperties> layerProperties = m_context.enumerateInstanceLayerProperties();

    auto isLayerAvailable = [&layerProperties](const char *layerName) {
        return std::ranges::any_of(layerProperties, [layerName](const auto &available) {
            return strcmp(available.layerName, layerName) == 0;
        });
    };

    auto missingLayerIt = std::ranges::find_if_not(requiredLayers, isLayerAvailable);

    if (missingLayerIt != requiredLayers.end())
    {
        throw std::runtime_error(std::string("[Vulkan Instance] Required layer not supported: ") + *missingLayerIt);
    }

    // Check extension compatibility
    std::vector<const char *> requiredExtensions = extensions;
    if (m_desc.debugMode)
    {
        requiredExtensions.insert(requiredExtensions.end(), debugExtentions.begin(), debugExtentions.end());
    }

    std::vector<vk::ExtensionProperties> extensionProperties = m_context.enumerateInstanceExtensionProperties();

    auto isExtensionAvailable = [&extensionProperties](const char *extensionName) {
        return std::ranges::any_of(extensionProperties, [extensionName](const auto &available) {
            return strcmp(available.extensionName, extensionName) == 0;
        });
    };

    auto missingExtensionIt = std::ranges::find_if_not(requiredExtensions, isExtensionAvailable);

    if (missingExtensionIt != requiredExtensions.end())
    {
        throw std::runtime_error(std::string("[Vulkan Instance] Required extension not supported: ") +
                                 *missingExtensionIt);
    }

    // Create instance
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo{}
                                            .setPApplicationInfo(&appInfo)
                                            .setEnabledLayerCount(static_cast<uint32_t>(requiredLayers.size()))
                                            .setPpEnabledLayerNames(requiredLayers.data())
                                            .setEnabledExtensionCount(static_cast<uint32_t>(requiredExtensions.size()))
                                            .setPpEnabledExtensionNames(requiredExtensions.data());

    m_instance = vk::raii::Instance(m_context, createInfo);

    if (m_desc.debugMode)
    {
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = vk::DebugUtilsMessengerCreateInfoEXT{}
                                                                                    .setMessageSeverity(severityFlags)
                                                                                    .setMessageType(messageTypeFlags)
                                                                                    .setPfnUserCallback(&DebugCallback);

        m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }
}

std::vector<std::unique_ptr<Object::Adapter>> VulkanInstance::GetAdapters() const
{
    std::vector<vk::raii::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
    if (physicalDevices.empty())
    {
        return {};
    }

    std::vector<std::unique_ptr<Object::Adapter>> adapters;
    for (int i = 0; i < physicalDevices.size(); ++i)
    {
        const vk::PhysicalDeviceProperties props = physicalDevices[i].getProperties();

        Desc::AdapterDesc desc{};

        std::strncpy(desc.name, props.deviceName.data(), sizeof(desc.name) - 1);
        desc.name[sizeof(desc.name) - 1] = '\0';

        desc.type = ToCommon(props.deviceType);
        desc.vendorId = props.vendorID;
        desc.deviceId = props.deviceID;
        desc.dedicatedVideoMemory = QueryDeviceLocalMemory(physicalDevices[i]);

        adapters.push_back(std::make_unique<Object::VulkanAdapter>(desc, std::move(physicalDevices[i])));
    }

    return adapters;
}

} // namespace Renderlib::Object
