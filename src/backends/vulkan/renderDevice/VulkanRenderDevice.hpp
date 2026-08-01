#pragma once

#include <backends/vulkan/device/VulkanDevice.hpp>
#include <backends/vulkan/resources/VulkanCommandBuffer.hpp>
#include <backends/vulkan/resources/VulkanSwapchain.hpp>
#include <renderlib/RenderDevice.hpp>

class VulkanRenderDevice final : public RenderDevice
{
  public:
    VulkanRenderDevice() = default;
    bool Initialize() override;
    ~VulkanRenderDevice() override;

    SwapchainHandle CreateSwapchain(const SwapchainCreateInfo &desc) override;
    void DestroySwapchain(SwapchainHandle handle) override;
    void ResizeSwapchain(SwapchainHandle handle, uint32_t width, uint32_t height) override;
    bool AcquireNextImage(SwapchainHandle handle) override;
    bool Present(SwapchainHandle handle) override;

    TextureHandle GetCurrentSwapchainImage(SwapchainHandle handle) override;
    TextureViewHandle GetCurrentSwapchainImageView(SwapchainHandle handle) override;

    CommandBufferHandle BeginFrame(SwapchainHandle handle) override;
    void EndFrame(SwapchainHandle handle) override;

    void BeginRendering(CommandBufferHandle cmd, const RenderingInfo &info) override;
    void EndRendering(CommandBufferHandle cmd) override;
    void SetViewport(CommandBufferHandle cmd, float x, float y, float width, float height) override;
    void SetScissor(CommandBufferHandle cmd, int32_t x, int32_t y, uint32_t width, uint32_t height) override;
    void Draw(CommandBufferHandle cmd, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
              uint32_t firstInstance) override;
    void DrawIndexed(CommandBufferHandle cmd, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                     int32_t vertexOffset, uint32_t firstInstance) override;
    void PushConstants(CommandBufferHandle cmd, PipelineHandle pipeline, ShaderStageFlags stages, uint32_t offset,
                       uint32_t size, const void *data) override;

    void BindPipeline(CommandBufferHandle cmd, PipelineHandle pipeline) override;
    void BindVertexBuffer(CommandBufferHandle cmd, BufferHandle buffer, uint32_t binding = 0,
                          uint64_t offset = 0) override;
    void BindIndexBuffer(CommandBufferHandle cmd, BufferHandle buffer, IndexType indexType,
                         uint64_t offset = 0) override;
    void BindDescriptorSet(CommandBufferHandle cmd, PipelineHandle pipeline, DescriptorSetHandle set,
                           uint32_t setIndex = 0) override;

  private:
    VulkanDevice *GetVulkanDevice() const
    {
        return static_cast<VulkanDevice *>(m_device.get());
    }

    void BuildSwapchainResources(VulkanSwapchain &impl, const SwapchainCreateInfo &desc);
    void DestroySwapchainResources(VulkanSwapchain &impl);
    void TransitionImageLayout(vk::raii::CommandBuffer &cmd, VkImage image, vk::ImageLayout oldLayout,
                               vk::ImageLayout newLayout, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess,
                               vk::PipelineStageFlags srcStage, vk::PipelineStageFlags dstStage,
                               vk::ImageAspectFlags aspectMask);

    ResourcePool<VulkanSwapchain, SwapchainTag> m_swapchainPool;
    ResourcePool<VulkanCommandBuffer, CommandBufferTag> m_commandBufferPool;
};
