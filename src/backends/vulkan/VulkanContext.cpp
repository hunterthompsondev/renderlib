#include "VulkanContext.hpp"

#include <iostream>

static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                      vk::DebugUtilsMessageTypeFlagsEXT type,
                                                      const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                      void *)
{
    if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError ||
        severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
    {
        std::cerr << "[Vulkan Renderer] validation layer: type " << to_string(type)
                  << " msg: " << pCallbackData->pMessage << std::endl;
    }

    return vk::False;
}

bool VulkanContext::Initialize(bool enableValidationLayers)
{
    vk::ApplicationInfo appInfo = vk::ApplicationInfo{}
                                      .setPApplicationName("renderlib")
                                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setPEngineName("renderlib")
                                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setApiVersion(vk::ApiVersion14);

    // Configure the required layers
    std::vector<char const *> requiredLayers;
    if (enableValidationLayers)
    {
        requiredLayers.assign(validationLayers.begin(), validationLayers.end());
    }

    // Check if the required layers are supported by the Vulkan implementation.
    auto layerProperties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayerIt = std::ranges::find_if(requiredLayers, [&layerProperties](auto const &requiredLayer) {
        return std::ranges::none_of(layerProperties, [requiredLayer](auto const &layerProperty) {
            return strcmp(layerProperty.layerName, requiredLayer) == 0;
        });
    });
    if (unsupportedLayerIt != requiredLayers.end())
    {
        std::cerr << "[Vulkan Context] Required layer not supported: " << std::string(*unsupportedLayerIt) << std::endl;
        return false;
    }

    // Get the required extensions
    std::vector<char const *> requiredExtensions;
    if (enableValidationLayers)
    {
        requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
    }
    requiredExtensions.push_back(vk::KHRSurfaceExtensionName);
    requiredExtensions.push_back(vk::KHRWaylandSurfaceExtensionName);

    // Check if the required extensions are supported by the Vulkan implementation.
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    auto unsupportedPropertyIt =
        std::ranges::find_if(requiredExtensions, [&extensionProperties](auto const &requiredExtension) {
            return std::ranges::none_of(extensionProperties, [requiredExtension](auto const &extensionProperty) {
                return strcmp(extensionProperty.extensionName, requiredExtension) == 0;
            });
        });
    if (unsupportedPropertyIt != requiredExtensions.end())
    {
        std::cerr << "[Vulkan Context] Required extension not supprted:" + std::string(*unsupportedPropertyIt)
                  << std::endl;
        return false;
    }

    // Create Vulkan instance
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo{}
                                            .setPApplicationInfo(&appInfo)
                                            .setEnabledLayerCount(static_cast<uint32_t>(requiredLayers.size()))
                                            .setPpEnabledLayerNames(requiredLayers.data())
                                            .setEnabledExtensionCount(static_cast<uint32_t>(requiredExtensions.size()))
                                            .setPpEnabledExtensionNames(requiredExtensions.data());

    instance = vk::raii::Instance(context, createInfo);

    // Debug messenger setup
    if (enableValidationLayers)
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

        debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }

    // Hardare configration

    // Physical device selection
    // No physical device scoring logic implemented
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    auto const devIter = std::ranges::find_if(physicalDevices, [&](auto const &physicalDevice) {
        bool supportsVulkan1_3 = physicalDevice.getProperties().apiVersion >= VK_API_VERSION_1_3;

        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        bool supportsGraphics = std::ranges::any_of(
            queueFamilies, [](auto const &qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

        auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
        bool supportsAllRequiredExtensions = std::ranges::all_of(
            requiredDeviceExtensions, [&availableDeviceExtensions](auto const &requiredDeviceExtension) {
                return std::ranges::any_of(
                    availableDeviceExtensions, [requiredDeviceExtension](auto const &availableDeviceExtension) {
                        return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0;
                    });
            });

        auto features =
            physicalDevice.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
                                                 vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        bool supportsRequiredFeatures =
            features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
            features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

        return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
    });

    if (devIter == physicalDevices.end())
    {
        std::cerr << "[Vulkan Context] Failed to find suitable GPU\n";
        return false;
    }

    physicalDevice = std::move(*devIter);

    // Queue configuration
    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
    for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i)
    {
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            graphicsQueueIndex = i;
            break;
        }
    }

    if (graphicsQueueIndex == UINT32_MAX)
    {
        std::cerr << "[Vulkan Context] Could not find suitable queue family\n";
        return false;
    }

    // Create logical device and queues
    float queuePriority = 0.5f;
    std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;
    for (uint32_t queueIndex : {graphicsQueueIndex})
    {
        deviceQueueCreateInfos.push_back(vk::DeviceQueueCreateInfo{}
                                             .setQueueFamilyIndex(queueIndex)
                                             .setQueueCount(1)
                                             .setPQueuePriorities(&queuePriority));
    }
    // Enable various features
    vk::PhysicalDeviceFeatures2 features2{};

    vk::PhysicalDeviceVulkan11Features vulkan11Features{};
    vulkan11Features.setShaderDrawParameters(true);

    vk::PhysicalDeviceVulkan13Features vulkan13Features{};
    vulkan13Features.setDynamicRendering(true);
    vulkan13Features.setSynchronization2(true);

    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicState{};
    extendedDynamicState.setExtendedDynamicState(true);

    vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features,
                       vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
        featureChain{features2, vulkan11Features, vulkan13Features, extendedDynamicState};

    vk::DeviceCreateInfo deviceCreateInfo =
        vk::DeviceCreateInfo{}
            .setPNext(&featureChain.get<vk::PhysicalDeviceFeatures2>())
            .setQueueCreateInfoCount(static_cast<uint32_t>(deviceQueueCreateInfos.size()))
            .setPQueueCreateInfos(deviceQueueCreateInfos.data())
            .setEnabledExtensionCount(static_cast<uint32_t>(requiredDeviceExtensions.size()))
            .setPpEnabledExtensionNames(requiredDeviceExtensions.data());

    device = vk::raii::Device(physicalDevice, deviceCreateInfo);

    graphicsQueue = vk::raii::Queue(device, graphicsQueueIndex, 0);

    // Descriptor pool
    // created once, sized generously to use throughout the programs's lifetime
    std::array<vk::DescriptorPoolSize, 4> poolSizes{
        vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer, 256},
        vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 256},
        vk::DescriptorPoolSize{vk::DescriptorType::eCombinedImageSampler, 256},
        vk::DescriptorPoolSize{vk::DescriptorType::eStorageImage, 64},
    };

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.setPoolSizes(poolSizes).setMaxSets(512).setFlags(
        vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet); // allows individual DestroyDescriptorSet calls

    descriptorPool = vk::raii::DescriptorPool(device, poolInfo);

    vk::CommandPoolCreateInfo commandPoolInfo = vk::CommandPoolCreateInfo{}
                                                    .setQueueFamilyIndex(graphicsQueueIndex)
                                                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    commandPool = vk::raii::CommandPool(device, commandPoolInfo);

    std::cout << "[Vulkan Context] Initialization successful\n";

    return true;
}
