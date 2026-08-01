#pragma once

#include <memory>

#include "Device.hpp"
#include "RenderingInfo.hpp"
#include "resources/CommandBuffer.hpp"
#include "resources/Swapchain.hpp"

struct RenderDeviceCreateInfo
{
};

class RenderDevice
{
  public:
    static std::unique_ptr<RenderDevice> Create(const RenderDeviceCreateInfo &desc);
    virtual bool Initialize() = 0;
    virtual ~RenderDevice() = default;

    RenderDevice(const RenderDevice &) = delete;
    RenderDevice &operator=(const RenderDevice &) = delete;
    RenderDevice(RenderDevice &&) = delete;
    RenderDevice &operator=(RenderDevice &&) = delete;

    Device *GetDevice();

    // Swapchain lifecycle
    virtual SwapchainHandle CreateSwapchain(const SwapchainCreateInfo &desc) = 0;
    virtual void DestroySwapchain(SwapchainHandle handle) = 0;
    virtual void ResizeSwapchain(SwapchainHandle handle, uint32_t width, uint32_t height) = 0;
    virtual bool AcquireNextImage(SwapchainHandle handle) = 0;
    virtual bool Present(SwapchainHandle handle) = 0;

    // Per-frame access to the current swapchain image
    virtual TextureHandle GetCurrentSwapchainImage(SwapchainHandle handle) = 0;
    virtual TextureViewHandle GetCurrentSwapchainImageView(SwapchainHandle handle) = 0;

    // Frame lifecycle
    virtual CommandBufferHandle BeginFrame(SwapchainHandle handle) = 0;
    virtual void EndFrame(SwapchainHandle handle) = 0;

    // Command recording
    virtual void BeginRendering(CommandBufferHandle cmd, const RenderingInfo &info) = 0;
    virtual void EndRendering(CommandBufferHandle cmd) = 0;
    virtual void SetViewport(CommandBufferHandle cmd, float x, float y, float width, float height) = 0;
    virtual void SetScissor(CommandBufferHandle cmd, int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
    virtual void Draw(CommandBufferHandle cmd, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                      uint32_t firstInstance) = 0;
    virtual void DrawIndexed(CommandBufferHandle cmd, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                             int32_t vertexOffset, uint32_t firstInstance) = 0;
    virtual void PushConstants(CommandBufferHandle cmd, PipelineHandle pipeline, ShaderStageFlags stages,
                               uint32_t offset, uint32_t size, const void *data) = 0;

    virtual void BindPipeline(CommandBufferHandle cmd, PipelineHandle pipeline) = 0;
    virtual void BindVertexBuffer(CommandBufferHandle cmd, BufferHandle buffer, uint32_t binding = 0,
                                  uint64_t offset = 0) = 0;
    virtual void BindIndexBuffer(CommandBufferHandle cmd, BufferHandle buffer, IndexType indexType,
                                 uint64_t offset = 0) = 0;
    virtual void BindDescriptorSet(CommandBufferHandle cmd, PipelineHandle pipeline, DescriptorSetHandle set,
                                   uint32_t setIndex = 0) = 0;

  protected:
    RenderDevice() = default;
    std::unique_ptr<Device> m_device;
};
