#pragma once

#include <backends/vulkan/VulkanContext.hpp>
#include <backends/vulkan/resources/VulkanBuffer.hpp>
#include <backends/vulkan/resources/VulkanCommandBuffer.hpp>
#include <backends/vulkan/resources/VulkanDescriptorSet.hpp>
#include <backends/vulkan/resources/VulkanPipeline.hpp>
#include <backends/vulkan/resources/VulkanSampler.hpp>
#include <backends/vulkan/resources/VulkanShaderModule.hpp>
#include <backends/vulkan/resources/VulkanTexture.hpp>
#include <renderlib/Device.hpp>
#include <renderlib/ResourcePool.hpp>

class VulkanDevice final : public Device
{
  public:
    VulkanDevice() = default;
    bool Initialize() override;

    // Resource management
    BufferHandle CreateBuffer(const BufferCreateInfo &desc) override;
    void DestroyBuffer(BufferHandle handle) override;
    void UpdateBuffer(BufferHandle hanlde, std::span<const uint8_t> data, size_t offset) override;
    void *MapBuffer(BufferHandle handle) override;
    void UpmapBuffer(BufferHandle handle) override;

    ShaderModuleHandle CreateShaderModule(const ShaderModuleCreateInfo &desc) override;
    void DestroyShaderModule(ShaderModuleHandle handle) override;

    PipelineHandle CreatePipeline(const PipelineCreateInfo &desc) override;
    void DestroyPipeline(PipelineHandle handle) override;

    TextureHandle CreateTexture(const TextureCreateInfo &desc) override;
    TextureViewHandle CreateTextureView(const TextureViewCreateInfo &desc) override;
    void DestroyTexture(TextureHandle handle) override;
    void DestroyTextureView(TextureViewHandle handle) override;
    void UpdateTexture(CommandBufferHandle cmdHanlder, TextureHandle handle, uint32_t mipLevel, uint32_t arrayLayer,
                       std::span<const uint8_t> data) override;
    void UpdateTexture(TextureHandle handle, uint32_t mipLevel, uint32_t arrayLayer,
                       std::span<const uint8_t> data) override;
    void CopyTextureToBuffer(CommandBufferHandle cmdHandle, TextureHandle handle, BufferHandle dstBuffer,
                             Offset3D offset, Extent3D extent) override;
    void TransitionTextureLayout(CommandBufferHandle cmdHandle, TextureHandle handle, ImageLayout oldLayout,
                                 ImageLayout newLayout, AccessFlags srcAccess, AccessFlags dstAccess,
                                 PipelineStageFlags srcStage, PipelineStageFlags dstStage,
                                 ImageAspectFlags aspectMask) override;

    SamplerHandle CreateSampler(const SamplerCreateInfo &desc) override;
    void DestroySampler(SamplerHandle handle) override;

    DescriptorLayoutHandle CreateDescriptorLayout(const DescriptorLayoutCreateInfo &desc) override;
    void DestroyDescriptorLayout(DescriptorLayoutHandle handle) override;

    DescriptorSetHandle CreateDescriptorSet(const DescriptorSetCreateInfo &desc) override;
    void DestroyDescriptorSet(DescriptorSetHandle handle) override;
    void UpdateDescriptorSet(DescriptorSetHandle handle,
                             const std::vector<DescriptorBindingWriteCreateInfo> &writes) override;

    CommandBufferHandle CreateCommandBuffer() override;
    void DestroyCommandBuffer(CommandBufferHandle handle) override;
    void BeginRecording(CommandBufferHandle handle) override;
    void EndRecording(CommandBufferHandle handle) override;
    void BeginRendering(CommandBufferHandle cmd, const RenderingInfo &info) override;
    void EndRendering(CommandBufferHandle cmd) override;
    void OffscreenSubmit(CommandBufferHandle handle) override;

    // Command recording
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

    TextureHandle RegisterExternalTexture(VkImage image, ImageAspectFlags imageAspectFlags, Format format,
                                          uint32_t width, uint32_t height);
    VulkanContext m_context;

    // Accessors
    vk::Pipeline GetVulkanPipeline(PipelineHandle handle);
    vk::Image GetVulkanImage(TextureHandle handle);
    vk::ImageView GetVulkanImageView(TextureViewHandle handle);
    vk::Buffer GetVulkanBuffer(BufferHandle handle);
    vk::PipelineLayout GetVulkanPipelineLayout(PipelineHandle handle);
    vk::DescriptorSet GetVulkanDescriptorSet(DescriptorSetHandle handle);
    TextureHandle GetSourceTextureForView(TextureViewHandle handle);
    void WaitIdle() override;

  private:
    ResourcePool<VulkanBuffer, BufferTag> m_bufferPool;
    ResourcePool<VulkanPipeline, PipelineTag> m_pipelinePool;
    ResourcePool<VulkanShaderModule, ShaderModuleTag> m_shaderModulePool;
    ResourcePool<VulkanTexture, TextureTag> m_texturePool;
    ResourcePool<VulkanTextureView, TextureViewTag> m_textureViewPool;
    ResourcePool<VulkanSampler, SamplerTag> m_samplerPool;
    ResourcePool<VulkanDescriptorLayout, DescriptorLayoutTag> m_descriptorLayoutPool;
    ResourcePool<VulkanDescriptorSet, DescriptorSetTag> m_descriptorSetPool;
    ResourcePool<VulkanCommandBuffer, CommandBufferTag> m_commandBufferPool;

    void TransitionTextureLayout(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout oldLayout,
                                 vk::ImageLayout newLayout, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess,
                                 vk::PipelineStageFlags srcStage, vk::PipelineStageFlags dstStage,
                                 vk::ImageAspectFlags aspectMask);
};
