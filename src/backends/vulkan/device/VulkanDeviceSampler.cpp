#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/VulkanConversions.hpp>

SamplerHandle VulkanDevice::CreateSampler(const SamplerCreateInfo &desc)
{
    VulkanSampler sampler{};

    vk::SamplerCreateInfo samplerCI{};
    samplerCI.setMagFilter(ToVulkanFilter(desc.magFilter))
        .setMinFilter(ToVulkanFilter(desc.minFilter))
        .setMipmapMode(ToVulkanMipmapMode(desc.minFilter))
        .setAddressModeU(ToVulkanAddressMode(desc.addressModeU))
        .setAddressModeV(ToVulkanAddressMode(desc.addressModeV))
        .setAddressModeW(ToVulkanAddressMode(desc.addressModeW))
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
