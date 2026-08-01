#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/VulkanConversions.hpp>

DescriptorLayoutHandle VulkanDevice::CreateDescriptorLayout(const DescriptorLayoutCreateInfo &desc)
{
    VulkanDescriptorLayout layout{
        .bindings = desc.bindings,
    };

    std::vector<vk::DescriptorSetLayoutBinding> bindings = ToVulkanDescriptorSetLayoutBindings(desc.bindings);

    vk::DescriptorSetLayoutCreateInfo layoutCI{};
    layoutCI.setBindings(bindings);

    // Create descriptor set layout
    layout.layout = vk::raii::DescriptorSetLayout(m_context.device, layoutCI);

    return m_descriptorLayoutPool.Create(std::move(layout));
}

void VulkanDevice::DestroyDescriptorLayout(DescriptorLayoutHandle handle)
{
    m_descriptorLayoutPool.Destroy(handle);
}

DescriptorSetHandle VulkanDevice::CreateDescriptorSet(const DescriptorSetCreateInfo &desc)
{
    VulkanDescriptorSet descSet{
        .layout = desc.layout,
    };

    VulkanDescriptorLayout *layout = m_descriptorLayoutPool.Get(desc.layout);
    if (!layout)
    {
        return {};
    }

    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.setDescriptorPool(*m_context.descriptorPool).setSetLayouts(*layout->layout);

    // Allocate descriptor set
    vk::raii::DescriptorSets sets(m_context.device, allocInfo);
    descSet.set = std::move(sets.front());

    DescriptorSetHandle handle = m_descriptorSetPool.Create(std::move(descSet));

    if (!desc.bindings.empty())
    {
        UpdateDescriptorSet(handle, desc.bindings);
    }

    return handle;
}

void VulkanDevice::DestroyDescriptorSet(DescriptorSetHandle handle)
{
    m_descriptorSetPool.Destroy(handle);
}

void VulkanDevice::UpdateDescriptorSet(DescriptorSetHandle handle,
                                       const std::vector<DescriptorBindingWriteCreateInfo> &writes)
{
    VulkanDescriptorSet *set = m_descriptorSetPool.Get(handle);
    if (!set)
    {
        return;
    }

    std::vector<vk::WriteDescriptorSet> vkWrites;
    std::vector<vk::DescriptorBufferInfo> bufferInfos;
    std::vector<vk::DescriptorImageInfo> imageInfos;

    bufferInfos.reserve(writes.size());
    imageInfos.reserve(writes.size());

    for (const auto &write : writes)
    {
        vk::WriteDescriptorSet vkWrite{};
        vkWrite.setDstSet(*set->set).setDstBinding(write.binding).setDescriptorType(ToVulkanDescriptorType(write.type));

        if (write.type == DescriptorType::UniformBuffer || write.type == DescriptorType::StorageBuffer)
        {
            VulkanBuffer *buffer = m_bufferPool.Get(write.buffer);
            if (!buffer)
            {
                continue;
            }

            vk::DescriptorBufferInfo bufferInfo{};
            bufferInfo.setBuffer(buffer->buffer)
                .setOffset(write.bufferOffset)
                .setRange(write.bufferRange == 0 ? VK_WHOLE_SIZE : write.bufferRange);

            bufferInfos.push_back(bufferInfo);

            vkWrite.setBufferInfo(bufferInfos.back());
        }
        else if (write.type == DescriptorType::SampledTexture || write.type == DescriptorType::StorageTexture)
        {
            VulkanTextureView *view = m_textureViewPool.Get(write.textureView);
            VulkanSampler *sampler = m_samplerPool.Get(write.sampler);
            if (!view)
            {
                continue;
            }

            vk::DescriptorImageInfo imageInfo{};
            imageInfo.setSampler(sampler ? *sampler->sampler : nullptr)
                .setImageView(*view->imageView)
                .setImageLayout(write.type == DescriptorType::StorageTexture ? vk::ImageLayout::eGeneral
                                                                             : vk::ImageLayout::eShaderReadOnlyOptimal);

            imageInfos.push_back(imageInfo);

            vkWrite.setImageInfo(imageInfos.back());
        }

        vkWrites.push_back(vkWrite);
    }

    // Update descriptor set
    m_context.device.updateDescriptorSets(vkWrites, {});
}
