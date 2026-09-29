#include "VulkanDevice.hpp"

#include <backends/vulkan/utils/ToVkEnums.hpp>

CommandBufferHandle VulkanDevice::CreateCommandBuffer()
{
    VulkanCommandBuffer commandBuffer;

    vk::CommandPoolCreateInfo commandPoolInfo = vk::CommandPoolCreateInfo{}
                                                    .setQueueFamilyIndex(m_context.graphicsQueueIndex)
                                                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    commandBuffer.pool = vk::raii::CommandPool(m_context.device, commandPoolInfo);

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.setCommandPool(commandBuffer.pool).setLevel(vk::CommandBufferLevel::ePrimary).setCommandBufferCount(1);

    vk::raii::CommandBuffers commandBuffers(m_context.device, allocInfo);
    commandBuffer.buffer = std::move(commandBuffers[0]);

    return m_commandBufferPool.Create(std::move(commandBuffer));
}

void VulkanDevice::DestroyCommandBuffer(CommandBufferHandle handle)
{
    m_commandBufferPool.Destroy(handle);
}

void VulkanDevice::BeginRecording(CommandBufferHandle handle)
{
    VulkanCommandBuffer *commandBuffer = m_commandBufferPool.Get(handle);
    if (!commandBuffer)
    {
        return;
    }

    commandBuffer->buffer.reset();

    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    commandBuffer->buffer.begin(beginInfo);
}

void VulkanDevice::EndRecording(CommandBufferHandle handle)
{
    VulkanCommandBuffer *commandBuffer = m_commandBufferPool.Get(handle);
    if (!commandBuffer)
    {
        return;
    }

    commandBuffer->buffer.end();
}

void VulkanDevice::BeginRendering(CommandBufferHandle cmdHandle, const RenderingInfo &info)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    std::vector<vk::RenderingAttachmentInfo> colorAttachments;
    colorAttachments.reserve(info.colorAttachments.size());

    for (const auto &color : info.colorAttachments)
    {
        vk::RenderingAttachmentInfo attachment{};
        vk::ImageView colorView = m_textureViewPool.Get(color.view)->imageView;
        attachment.setImageView(colorView)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setLoadOp(ToVk(color.loadOp))
            .setStoreOp(ToVk(color.storeOp))
            .setClearValue(vk::ClearValue{
                vk::ClearColorValue{
                    std::array{
                        color.clearColor[0],
                        color.clearColor[1],
                        color.clearColor[2],
                        color.clearColor[3],
                    },
                },
            });

        colorAttachments.push_back(attachment);
    }

    vk::RenderingInfo renderingInfo{};
    renderingInfo.setRenderArea(vk::Rect2D{{0, 0}, {info.width, info.height}})
        .setLayerCount(1)
        .setColorAttachments(colorAttachments);

    vk::RenderingAttachmentInfo depthAttachment{};
    if (info.depthAttachment)
    {
        VulkanTextureView *depthTextureView = m_textureViewPool.Get(info.depthAttachment->view);
        vk::Image depthTexture = m_texturePool.Get(depthTextureView->sourceTexture)->imageHandle;
        TransitionTextureLayout(cmd->buffer, depthTexture, vk::ImageLayout::eUndefined,
                                vk::ImageLayout::eDepthAttachmentOptimal, {},
                                vk::AccessFlagBits::eDepthStencilAttachmentWrite, vk::PipelineStageFlagBits::eTopOfPipe,
                                vk::PipelineStageFlagBits::eEarlyFragmentTests, vk::ImageAspectFlagBits::eDepth);

        depthAttachment.setImageView(depthTextureView->imageView)
            .setImageLayout(vk::ImageLayout::eDepthAttachmentOptimal)
            .setLoadOp(ToVk(info.depthAttachment->loadOp))
            .setStoreOp(ToVk(info.depthAttachment->storeOp))
            .setClearValue(vk::ClearValue{
                vk::ClearDepthStencilValue{
                    info.depthAttachment->clearDepth,
                    0,
                },
            });

        renderingInfo.setPDepthAttachment(&depthAttachment);
    }

    cmd->buffer.beginRendering(renderingInfo);
}

void VulkanDevice::EndRendering(CommandBufferHandle cmdHandle)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->buffer.endRendering();
}

void VulkanDevice::OffscreenSubmit(CommandBufferHandle handle)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(handle);
    if (!cmd)
    {
        return;
    }
    vk::FenceCreateInfo fenceInfo{};
    vk::raii::Fence fence(m_context.device, fenceInfo);

    vk::SubmitInfo submitInfo{};
    submitInfo.setCommandBuffers(*cmd->buffer);

    m_context.graphicsQueue.submit(submitInfo, *fence);

    static_cast<void>(m_context.device.waitForFences(*fence, VK_TRUE, UINT64_MAX));
}
