#include "VulkanDevice.hpp"

vk::Pipeline VulkanDevice::GetVulkanPipeline(PipelineHandle handle)
{
    VulkanPipeline *pipeline = m_pipelinePool.Get(handle);
    return pipeline ? *pipeline->pipeline : nullptr;
}

vk::Image VulkanDevice::GetVulkanImage(TextureHandle handle)
{
    VulkanTexture *image = m_texturePool.Get(handle);
    return image ? image->imageHandle : nullptr;
}

vk::ImageView VulkanDevice::GetVulkanImageView(TextureViewHandle handle)
{
    VulkanTextureView *imageView = m_textureViewPool.Get(handle);
    return imageView ? *imageView->imageView : nullptr;
}

vk::Buffer VulkanDevice::GetVulkanBuffer(BufferHandle handle)
{
    VulkanBuffer *buffer = m_bufferPool.Get(handle);
    return buffer ? *buffer->buffer : nullptr;
}
vk::PipelineLayout VulkanDevice::GetVulkanPipelineLayout(PipelineHandle handle)
{
    auto *pipeline = m_pipelinePool.Get(handle);
    return pipeline ? *pipeline->layout : nullptr;
}

vk::DescriptorSet VulkanDevice::GetVulkanDescriptorSet(DescriptorSetHandle handle)
{
    auto *set = m_descriptorSetPool.Get(handle);
    return set ? *set->set : nullptr;
}

TextureHandle VulkanDevice::GetSourceTextureForView(TextureViewHandle handle)
{
    auto *textureView = m_textureViewPool.Get(handle);
    return textureView ? textureView->sourceTexture : TextureHandle{};
}
void VulkanDevice::WaitIdle()
{
    m_context.device.waitIdle();
}
