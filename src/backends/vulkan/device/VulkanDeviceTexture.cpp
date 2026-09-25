#include "VulkanDevice.hpp"

#include <iostream>

#include <backends/vulkan/utils/MemoryUtils.hpp>
#include <backends/vulkan/utils/ToVkEnums.hpp>
#include <backends/vulkan/utils/ToVkFlags.hpp>

TextureHandle VulkanDevice::CreateTexture(const TextureCreateInfo &desc)
{
    VulkanTexture texture{
        .managesOwnMemory = true,
        .type = ToVk(desc.type),
        .format = ToVk(desc.format),
        .imageAspectFlags = ToVk(desc.imageAspectFlags),
        .width = desc.width,
        .height = desc.height,
        .depth = desc.depth,
        .mipLevels = desc.mipLevels,
        .arrayLayers = desc.arrayLayers,
    };

    vk::ImageCreateFlags flags{};
    if (desc.type == ImageType::TextureCube)
    {
        flags |= vk::ImageCreateFlagBits::eCubeCompatible;
    }

    vk::ImageCreateInfo imageCI{};
    imageCI.setFlags(flags)
        .setImageType(ToVk(desc.type))
        .setFormat(ToVk(desc.format))
        .setExtent(vk::Extent3D{desc.width, desc.height, desc.depth})
        .setMipLevels(desc.mipLevels)
        .setArrayLayers(desc.arrayLayers)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setTiling(vk::ImageTiling::eOptimal)
        .setUsage(ToVk(desc.usage))
        .setSharingMode(vk::SharingMode::eExclusive)
        .setInitialLayout(vk::ImageLayout::eUndefined);

    // Create image
    texture.ownedImage = vk::raii::Image(m_context.device, imageCI);
    texture.imageHandle = *texture.ownedImage;

    vk::MemoryRequirements memReqs = texture.ownedImage.getMemoryRequirements();
    uint32_t memTypeIdx =
        FindMemoryType(m_context.physicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (memTypeIdx == UINT32_MAX)
    {
        std::cerr << "[Vulkan Device] Valid memory type for texture not found\n";
        return {};
    }

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.setAllocationSize(memReqs.size).setMemoryTypeIndex(memTypeIdx);

    // Allocate image memory
    texture.memory = vk::raii::DeviceMemory(m_context.device, allocInfo);
    texture.size = memReqs.size;

    // Bind image memory
    texture.ownedImage.bindMemory(*texture.memory, 0);

    return m_texturePool.Create(std::move(texture));
}

void VulkanDevice::DestroyTexture(TextureHandle handle)
{
    m_texturePool.Destroy(handle);
}

TextureViewHandle VulkanDevice::CreateTextureView(const TextureViewCreateInfo &desc)
{
    VulkanTexture *texture = m_texturePool.Get(desc.sourceTexture);
    if (!texture)
    {
        return {};
    }

    VulkanTextureView textureView{
        .sourceTexture = desc.sourceTexture,
    };

    vk::ImageSubresourceRange subresourceRange{};
    subresourceRange.setAspectMask(ToVk(desc.imageAspectFlags))
        .setBaseMipLevel(0)
        .setLevelCount(texture->mipLevels)
        .setBaseArrayLayer(0)
        .setLayerCount(texture->arrayLayers);

    vk::ImageViewCreateInfo viewCI{};
    viewCI.setImage(texture->imageHandle)
        .setViewType(ToVk(desc.viewType))
        .setFormat(ToVk(desc.format))
        .setSubresourceRange(subresourceRange);

    // Create image view
    textureView.imageView = vk::raii::ImageView(m_context.device, viewCI);

    return m_textureViewPool.Create(std::move(textureView));
}

void VulkanDevice::DestroyTextureView(TextureViewHandle handle)
{
    m_textureViewPool.Destroy(handle);
}

void VulkanDevice::UpdateTexture(TextureHandle handle, uint32_t mipLevel, uint32_t arrayLayer,
                                 std::span<const uint8_t> data)
{
    VulkanTexture *textureImpl = m_texturePool.Get(handle);
    if (!textureImpl)
    {
        std::cerr << "[Vulkan Device] UpdateTexture called with invalid handle\n";
        return;
    }

    if (data.empty())
    {
        return;
    }

    // Create a host-visible staging buffer
    vk::BufferCreateInfo stagingBufferInfo{};
    stagingBufferInfo.setSize(data.size())
        .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
        .setSharingMode(vk::SharingMode::eExclusive);
    vk::raii::Buffer stagingBuffer(m_context.device, stagingBufferInfo);

    vk::MemoryRequirements stagingMemReq = stagingBuffer.getMemoryRequirements();
    uint32_t stagingMemType =
        FindMemoryType(m_context.physicalDevice, stagingMemReq.memoryTypeBits,
                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    if (stagingMemType == UINT32_MAX)
    {
        std::cerr << "[Vulkan Device] UpdateTexture: no suitable staging memory type found\n";
        return;
    }

    vk::MemoryAllocateInfo stagingAllocInfo{};
    stagingAllocInfo.setAllocationSize(stagingMemReq.size).setMemoryTypeIndex(stagingMemType);
    vk::raii::DeviceMemory stagingMemory(m_context.device, stagingAllocInfo);
    stagingBuffer.bindMemory(*stagingMemory, 0);

    void *mapped = stagingMemory.mapMemory(0, data.size());
    std::memcpy(mapped, data.data(), data.size());
    stagingMemory.unmapMemory();

    // Record a one-shot command buffer: transition -> copy -> transition
    vk::CommandBufferAllocateInfo cmdAllocInfo{};
    cmdAllocInfo.setCommandPool(*m_context.commandPool)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(1);
    vk::raii::CommandBuffers cmdBuffers(m_context.device, cmdAllocInfo);
    vk::raii::CommandBuffer &cmd = cmdBuffers.front();

    cmd.begin(vk::CommandBufferBeginInfo{}.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

    vk::ImageSubresourceRange range{};
    range.setAspectMask(textureImpl->imageAspectFlags)
        .setBaseMipLevel(mipLevel)
        .setLevelCount(1)
        .setBaseArrayLayer(arrayLayer)
        .setLayerCount(1);

    // Undefined -> TransferDstOptimal
    vk::ImageMemoryBarrier toTransferDst{};
    toTransferDst.setOldLayout(vk::ImageLayout::eUndefined)
        .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
        .setImage(textureImpl->imageHandle)
        .setSubresourceRange(range)
        .setSrcAccessMask({})
        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
    cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, {}, {},
                        toTransferDst);

    // Copy staging buffer -> image
    vk::BufferImageCopy copyRegion{};
    copyRegion.setBufferOffset(0)
        .setBufferRowLength(0)   // 0 = tightly packed
        .setBufferImageHeight(0) // 0 = tightly packed
        .setImageSubresource(vk::ImageSubresourceLayers{textureImpl->imageAspectFlags, mipLevel, arrayLayer, 1})
        .setImageOffset({0, 0, 0})
        .setImageExtent({
            std::max(textureImpl->width >> mipLevel, 1u),
            std::max(textureImpl->height >> mipLevel, 1u),
            1,
        });
    cmd.copyBufferToImage(*stagingBuffer, textureImpl->imageHandle, vk::ImageLayout::eTransferDstOptimal, copyRegion);

    // TransferDstOptimal -> ShaderReadOnlyOptimal
    vk::ImageMemoryBarrier toShaderRead{};
    toShaderRead.setOldLayout(vk::ImageLayout::eTransferDstOptimal)
        .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
        .setImage(textureImpl->imageHandle)
        .setSubresourceRange(range)
        .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
    cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {},
                        toShaderRead);

    cmd.end();

    // Submit and wait (synchronous upload)
    vk::SubmitInfo submitInfo{};
    submitInfo.setCommandBuffers(*cmd);
    m_context.graphicsQueue.submit(submitInfo);
    m_context.graphicsQueue.waitIdle();
}

TextureHandle VulkanDevice::RegisterExternalTexture(VkImage image, ImageAspectFlags imageAspectFlags, Format format,
                                                    uint32_t width, uint32_t height)
{
    VulkanTexture texture{
        .imageHandle = image,
        .managesOwnMemory = false,
        .type = ToVk(ImageType::Texture2D),
        .format = ToVk(format),
        .imageAspectFlags = ToVk(imageAspectFlags),
        .width = width,
        .height = height,
        .depth = 1,
        .mipLevels = 1,
        .arrayLayers = 1,
    };

    return m_texturePool.Create(std::move(texture));
}
