#include "VulkanRenderDevice.hpp"

#include <backends/vulkan/resources/VulkanSwapchain.hpp>
#include <backends/vulkan/utils/VulkanConversions.hpp>

void VulkanRenderDevice::BuildSwapchainResources(VulkanSwapchain &swapchain, const SwapchainCreateInfo &desc)
{
    VulkanDevice *device = GetVulkanDevice();
    VulkanContext *context = &device->m_context;

    vk::SurfaceCapabilitiesKHR capabilities = context->physicalDevice.getSurfaceCapabilitiesKHR(*swapchain.surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = context->physicalDevice.getSurfaceFormatsKHR(*swapchain.surface);
    std::vector<vk::PresentModeKHR> presentModes =
        context->physicalDevice.getSurfacePresentModesKHR(*swapchain.surface);

    vk::Format desiredFormat = ToVulkanFormat(desc.format);
    vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

    bool formatSupported = std::ranges::any_of(surfaceFormats, [&](const auto &format) {
        return format.format == desiredFormat && format.colorSpace == colorSpace;
    });

    if (!formatSupported)
    {
        desiredFormat = surfaceFormats.front().format;
        colorSpace = surfaceFormats.front().colorSpace;
    }

    vk::PresentModeKHR desiredPresentMode = ToVulkanPresentMode(desc.presentMode);
    if (!std::ranges::any_of(presentModes, [&](auto mode) { return mode == desiredPresentMode; }))
    {
        desiredPresentMode = vk::PresentModeKHR::eFifo;
    }

    uint32_t imageCount = std::max(desc.bufferCount, capabilities.minImageCount);
    if (capabilities.maxImageCount > 0)
    {
        imageCount = std::min(imageCount, capabilities.maxImageCount);
    }

    vk::SwapchainCreateInfoKHR swapchainCI{};
    swapchainCI.setSurface(*swapchain.surface)
        .setMinImageCount(imageCount)
        .setImageFormat(desiredFormat)
        .setImageColorSpace(colorSpace)
        .setImageExtent(vk::Extent2D{desc.width, desc.height})
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst)
        .setImageSharingMode(vk::SharingMode::eExclusive)
        .setPreTransform(capabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(desiredPresentMode)
        .setClipped(vk::True)
        .setOldSwapchain(*swapchain.swapchain);

    // Create swapchain
    swapchain.swapchain = vk::raii::SwapchainKHR(context->device, swapchainCI);
    swapchain.format = desiredFormat;
    swapchain.width = desc.width;
    swapchain.height = desc.height;
    swapchain.windowHandle = desc.window;

    std::vector<vk::Image> images = swapchain.swapchain.getImages();

    for (vk::Image image : images)
    {
        TextureHandle texture =
            device->RegisterExternalTexture(image, FromVulkanFormat(desiredFormat), desc.width, desc.height);

        swapchain.imageHandles.push_back(texture);

        TextureViewHandle view = device->CreateTextureView({
            .sourceTexture = texture,
            .viewType = TextureType::Texture2D,
            .format = Format::Undefined,
        });

        swapchain.imageViewHandles.push_back(view);
    }

    if (swapchain.imageAvailableSemaphores.empty())
    {
        for (size_t i = 0; i < images.size(); ++i)
        {
            swapchain.imageAvailableSemaphores.emplace_back(context->device, vk::SemaphoreCreateInfo{});
            swapchain.renderCompleteSemaphores.emplace_back(context->device, vk::SemaphoreCreateInfo{});
            swapchain.inFlightFences.emplace_back(context->device,
                                                  vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled));
        }

        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.setCommandPool(*context->commandPool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(static_cast<uint32_t>(images.size()));

        swapchain.commandBuffers = vk::raii::CommandBuffers(context->device, allocInfo);
    }

    swapchain.currentFrameIndex = 0;
}

void VulkanRenderDevice::DestroySwapchainResources(VulkanSwapchain &swapchain)
{
    VulkanDevice *device = GetVulkanDevice();

    for (TextureViewHandle view : swapchain.imageViewHandles)
    {
        device->DestroyTextureView(view);
    }

    for (TextureHandle texture : swapchain.imageHandles)
    {
        device->DestroyTexture(texture);
    }

    swapchain.imageHandles.clear();
    swapchain.imageViewHandles.clear();
}

SwapchainHandle VulkanRenderDevice::CreateSwapchain(const SwapchainCreateInfo &desc)
{
    VulkanDevice *device = GetVulkanDevice();
    VulkanContext *context = &device->m_context;

    vk::WaylandSurfaceCreateInfoKHR surfaceCI{};
    surfaceCI.setDisplay(reinterpret_cast<struct wl_display *>(desc.window.display))
        .setSurface(reinterpret_cast<struct wl_surface *>(desc.window.surface));

    VulkanSwapchain swapchain{};
    swapchain.surface = vk::raii::SurfaceKHR(context->instance, surfaceCI);

    BuildSwapchainResources(swapchain, desc);

    return m_swapchainPool.Create(std::move(swapchain));
}

void VulkanRenderDevice::DestroySwapchain(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return;
    }

    GetVulkanDevice()->WaitIdle();

    DestroySwapchainResources(*swapchain);
    m_swapchainPool.Destroy(handle);
}

void VulkanRenderDevice::ResizeSwapchain(SwapchainHandle handle, uint32_t width, uint32_t height)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return;
    }

    GetVulkanDevice()->WaitIdle();

    DestroySwapchainResources(*swapchain);

    SwapchainCreateInfo desc{
        .window = swapchain->windowHandle,
        .width = width,
        .height = height,
        .format = FromVulkanFormat(swapchain->format),
        .presentMode = PresentMode::Fifo,
        .bufferCount = static_cast<uint32_t>(swapchain->inFlightFences.size()),
    };

    BuildSwapchainResources(*swapchain, desc);
}

bool VulkanRenderDevice::AcquireNextImage(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return false;
    }

    VulkanContext *context = &GetVulkanDevice()->m_context;
    uint32_t frameIdx = swapchain->currentFrameIndex;

    vk::Result result = context->device.waitForFences(*swapchain->inFlightFences[frameIdx], vk::True, UINT64_MAX);
    if (result != vk::Result::eSuccess)
    {
        return false;
    }

    auto [acquireResult, imageIdx] =
        swapchain->swapchain.acquireNextImage(UINT64_MAX, *swapchain->imageAvailableSemaphores[frameIdx], nullptr);

    if (acquireResult != vk::Result::eSuccess && acquireResult != vk::Result::eSuboptimalKHR)
    {
        return false;
    }

    context->device.resetFences(*swapchain->inFlightFences[frameIdx]);

    swapchain->currentImageIndex = imageIdx;

    return true;
}

bool VulkanRenderDevice::Present(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return false;
    }

    VulkanContext *context = &GetVulkanDevice()->m_context;
    uint32_t frameIdx = swapchain->currentFrameIndex;

    vk::PresentInfoKHR presentInfo{};
    presentInfo.setWaitSemaphores(*swapchain->renderCompleteSemaphores[frameIdx])
        .setSwapchains(*swapchain->swapchain)
        .setImageIndices(swapchain->currentImageIndex);

    vk::Result result = context->graphicsQueue.presentKHR(presentInfo);

    swapchain->currentFrameIndex = (frameIdx + 1) % swapchain->inFlightFences.size();

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    {
        return false;
    }

    return result == vk::Result::eSuccess;
}

TextureHandle VulkanRenderDevice::GetCurrentSwapchainImage(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    return swapchain ? swapchain->imageHandles[swapchain->currentImageIndex] : TextureHandle{};
}

TextureViewHandle VulkanRenderDevice::GetCurrentSwapchainImageView(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    return swapchain ? swapchain->imageViewHandles[swapchain->currentImageIndex] : TextureViewHandle{};
}
