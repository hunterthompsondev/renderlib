#pragma once

#include <cstdint>
#include <memory>
#include <span>

#include <renderlib/resources/Buffer.hpp>
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
    virtual void UpdateTexture(TextureHandle handle, uint32_t mipLevel, uint32_t arrayLayer,
                               std::span<const uint8_t> data) = 0;

    virtual SamplerHandle CreateSampler(const SamplerCreateInfo &createInfo) = 0;
    virtual void DestroySampler(SamplerHandle handle) = 0;

    virtual DescriptorLayoutHandle CreateDescriptorLayout(const DescriptorLayoutCreateInfo &createInfo) = 0;
    virtual void DestroyDescriptorLayout(DescriptorLayoutHandle handle) = 0;

    virtual DescriptorSetHandle CreateDescriptorSet(const DescriptorSetCreateInfo &createInfo) = 0;
    virtual void DestroyDescriptorSet(DescriptorSetHandle handle) = 0;
    virtual void UpdateDescriptorSet(DescriptorSetHandle handle,
                                     const std::vector<DescriptorBindingWriteCreateInfo> &writes) = 0;

    virtual void WaitIdle() = 0;

  protected:
    Device() = default;
};
