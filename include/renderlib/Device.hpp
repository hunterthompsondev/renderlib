#pragma once

#include <cstdint>
#include <memory>
#include <span>

#include <renderlib/RenderingInfo.hpp>
#include <renderlib/resources/Buffer.hpp>
#include <renderlib/resources/CommandBuffer.hpp>
#include <renderlib/resources/Pipeline.hpp>
#include <renderlib/resources/ShaderModule.hpp>
#include <renderlib/resources/Texture.hpp>

struct DeviceCreateInfo
{
};

class Device
{
  public:
    static std::unique_ptr<Device> Create(const DeviceCreateInfo &desc);
    virtual bool Initialize() = 0;
    virtual ~Device() = default;

    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(Device &&) = delete;

    virtual BufferHandle CreateBuffer(const BufferCreateInfo &desc) = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    virtual void UpdateBuffer(BufferHandle hanlde, std::span<const uint8_t> data, size_t offset) = 0;
    virtual void *MapBuffer(BufferHandle handle) = 0;
    virtual void UpmapBuffer(BufferHandle handle) = 0;

    virtual ShaderModuleHandle CreateShaderModule(const ShaderModuleCreateInfo &desc) = 0;
    virtual void DestroyShaderModule(ShaderModuleHandle handle) = 0;

    virtual PipelineHandle CreatePipeline(const PipelineCreateInfo &desc) = 0;
    virtual void DestroyPipeline(PipelineHandle handle) = 0;

    virtual TextureHandle CreateTexture(const TextureCreateInfo &desc) = 0;
    virtual TextureViewHandle CreateTextureView(const TextureViewCreateInfo &desc) = 0;
    virtual void DestroyTexture(TextureHandle handle) = 0;
    virtual void DestroyTextureView(TextureViewHandle handle) = 0;
    // Records but does not submit
    virtual void UpdateTexture(CommandBufferHandle cmdHandle, TextureHandle handle, uint32_t mipLevel,
                               uint32_t arrayLayer, std::span<const uint8_t> data) = 0;
    // Creates a command buffer and submits. Will be deprecated.
    virtual void UpdateTexture(TextureHandle handle, uint32_t mipLevel, uint32_t arrayLayer,
                               std::span<const uint8_t> data) = 0;
    virtual void CopyTextureToBuffer(CommandBufferHandle cmdHandle, TextureHandle handle, BufferHandle dstBuffer,
                                     Offset3D offset, Extent3D extent) = 0;
    virtual void TransitionTextureLayout(CommandBufferHandle cmdHandle, TextureHandle handle, ImageLayout oldLayout,
                                         ImageLayout newLayout, AccessFlags srcAccess, AccessFlags dstAccess,
                                         PipelineStageFlags srcStage, PipelineStageFlags dstStage,
                                         ImageAspectFlags aspectMask) = 0;

    virtual SamplerHandle CreateSampler(const SamplerCreateInfo &createInfo) = 0;
    virtual void DestroySampler(SamplerHandle handle) = 0;

    virtual DescriptorLayoutHandle CreateDescriptorLayout(const DescriptorLayoutCreateInfo &createInfo) = 0;
    virtual void DestroyDescriptorLayout(DescriptorLayoutHandle handle) = 0;

    virtual DescriptorSetHandle CreateDescriptorSet(const DescriptorSetCreateInfo &createInfo) = 0;
    virtual void DestroyDescriptorSet(DescriptorSetHandle handle) = 0;
    virtual void UpdateDescriptorSet(DescriptorSetHandle handle,
                                     const std::vector<DescriptorBindingWriteCreateInfo> &writes) = 0;

    virtual CommandBufferHandle CreateCommandBuffer() = 0;
    virtual void DestroyCommandBuffer(CommandBufferHandle handle) = 0;
    virtual void BeginRecording(CommandBufferHandle handle) = 0;
    virtual void EndRecording(CommandBufferHandle handle) = 0;
    virtual void BeginRendering(CommandBufferHandle cmd, const RenderingInfo &info) = 0;
    virtual void EndRendering(CommandBufferHandle cmd) = 0;
    virtual void OffscreenSubmit(CommandBufferHandle handle) = 0;

    // Command Recording
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

    virtual void WaitIdle() = 0;

  protected:
    Device() = default;
};
