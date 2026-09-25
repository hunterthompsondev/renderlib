#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/Swapchain.hpp>
#include <renderlib/resources/Texture.hpp>

struct VulkanSwapchain
{
    vk::raii::SurfaceKHR surface = nullptr;
    vk::raii::SwapchainKHR swapchain = nullptr;
    vk::Format format;
    Format genericFormat;
    vk::PresentModeKHR presentMode;

    uint32_t width;
    uint32_t height;

    std::vector<TextureHandle> imageHandles;
    std::vector<TextureViewHandle> imageViewHandles;
    std::vector<vk::raii::CommandBuffer> commandBuffers;

    std::vector<vk::raii::Semaphore> imageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> renderCompleteSemaphores;
    std::vector<vk::raii::Fence> inFlightFences;

    uint32_t currentFrameIndex = 0;
    uint32_t currentImageIndex = 0;
    NativeWindowHandle windowHandle;
};
