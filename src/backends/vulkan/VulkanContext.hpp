#pragma once

#include <vector>
#include <vulkan/vulkan_raii.hpp>

struct VulkanContext
{
    bool Initialize(bool enableValidationLayers);

    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

    std::vector<char const *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char *> requiredDeviceExtensions = {vk::KHRSwapchainExtensionName};

    // Hardware configuration
    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::raii::Device device = nullptr;

    vk::raii::Queue graphicsQueue = nullptr;
    uint32_t graphicsQueueIndex = UINT32_MAX;

    vk::raii::CommandPool commandPool = nullptr;
    vk::raii::DescriptorPool descriptorPool = nullptr;
};
