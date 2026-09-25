#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/ToVkEnums.hpp>

SamplerHandle VulkanDevice::CreateSampler(const SamplerCreateInfo &desc)
{
    VulkanSampler sampler{};

    vk::SamplerCreateInfo samplerCI{};
    samplerCI.setMagFilter(ToVk(desc.magFilter))
        .setMinFilter(ToVk(desc.minFilter))
        .setMipmapMode(ToVk(desc.mipMapMode))
        .setAddressModeU(ToVk(desc.addressModeU))
        .setAddressModeV(ToVk(desc.addressModeV))
        .setAddressModeW(ToVk(desc.addressModeW))
        .setAnisotropyEnable(desc.maxAnisotropy > 1.0f)
        .setMaxAnisotropy(desc.maxAnisotropy)
        .setMinLod(0.0f)
        .setMaxLod(vk::LodClampNone)
        .setBorderColor(vk::BorderColor::eIntOpaqueBlack);

    // Create sampler
    sampler.sampler = vk::raii::Sampler(m_context.device, samplerCI);

    return m_samplerPool.Create(std::move(sampler));
}

void VulkanDevice::DestroySampler(SamplerHandle handle)
{
    m_samplerPool.Destroy(handle);
}
