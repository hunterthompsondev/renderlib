#include "VulkanRenderDevice.hpp"

#include <backends/vulkan/utils/ToVkEnums.hpp>
#include <backends/vulkan/utils/ToVkFlags.hpp>

void VulkanRenderDevice::TransitionImageLayout(vk::raii::CommandBuffer &cmd, VkImage image, vk::ImageLayout oldLayout,
                                               vk::ImageLayout newLayout, vk::AccessFlags srcAccess,
                                               vk::AccessFlags dstAccess, vk::PipelineStageFlags srcStage,
                                               vk::PipelineStageFlags dstStage, vk::ImageAspectFlags aspectMask)
{
    vk::ImageMemoryBarrier barrier{};
    barrier.setOldLayout(oldLayout)
        .setNewLayout(newLayout)
        .setImage(image)
        .setSubresourceRange({aspectMask, 0, 1, 0, 1})
        .setSrcAccessMask(srcAccess)
        .setDstAccessMask(dstAccess);

    cmd.pipelineBarrier(srcStage, dstStage, {}, {}, {}, barrier);
}

CommandBufferHandle VulkanRenderDevice::BeginFrame(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return {};
    }

    vk::raii::CommandBuffer &cmd = swapchain->commandBuffers[swapchain->currentFrameIndex];
    cmd.reset();

    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    cmd.begin(beginInfo);

    TransitionImageLayout(cmd, GetVulkanDevice()->GetVulkanImage(swapchain->imageHandles[swapchain->currentImageIndex]),
                          vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, {},
                          vk::AccessFlagBits::eColorAttachmentWrite, vk::PipelineStageFlagBits::eTopOfPipe,
                          vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::ImageAspectFlagBits::eColor);

    return m_commandBufferPool.Create(VulkanCommandBuffer{.commandBuffer = &cmd});
}

void VulkanRenderDevice::EndFrame(SwapchainHandle handle)
{
    VulkanSwapchain *swapchain = m_swapchainPool.Get(handle);
    if (!swapchain)
    {
        return;
    }

    VulkanContext *context = &GetVulkanDevice()->m_context;
    uint32_t frameIdx = swapchain->currentFrameIndex;
    vk::raii::CommandBuffer &cmd = swapchain->commandBuffers[frameIdx];

    TransitionImageLayout(cmd, GetVulkanDevice()->GetVulkanImage(swapchain->imageHandles[swapchain->currentImageIndex]),
                          vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR,
                          vk::AccessFlagBits::eColorAttachmentWrite, {},
                          vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe,
                          vk::ImageAspectFlagBits::eColor);

    cmd.end();

    vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::SubmitInfo submitInfo{};
    submitInfo.setWaitSemaphores(*swapchain->imageAvailableSemaphores[frameIdx])
        .setWaitDstStageMask(waitStage)
        .setCommandBuffers(*cmd)
        .setSignalSemaphores(*swapchain->renderCompleteSemaphores[frameIdx]);

    context->graphicsQueue.submit(submitInfo, *swapchain->inFlightFences[frameIdx]);
}

void VulkanRenderDevice::BeginRendering(CommandBufferHandle cmdHandle, const RenderingInfo &info)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    VulkanDevice *device = GetVulkanDevice();

    std::vector<vk::RenderingAttachmentInfo> colorAttachments;
    colorAttachments.reserve(info.colorAttachments.size());

    for (const auto &color : info.colorAttachments)
    {
        vk::RenderingAttachmentInfo attachment{};
        attachment.setImageView(device->GetVulkanImageView(color.view))
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
        TextureHandle depthTexture = device->GetSourceTextureForView(info.depthAttachment->view);
        TransitionImageLayout(*cmd->commandBuffer, device->GetVulkanImage(depthTexture), vk::ImageLayout::eUndefined,
                              vk::ImageLayout::eDepthAttachmentOptimal, {},
                              vk::AccessFlagBits::eDepthStencilAttachmentWrite, vk::PipelineStageFlagBits::eTopOfPipe,
                              vk::PipelineStageFlagBits::eEarlyFragmentTests, vk::ImageAspectFlagBits::eDepth);

        depthAttachment.setImageView(device->GetVulkanImageView(info.depthAttachment->view))
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

    cmd->commandBuffer->beginRendering(renderingInfo);
}

void VulkanRenderDevice::EndRendering(CommandBufferHandle cmdHandle)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->commandBuffer->endRendering();
}

void VulkanRenderDevice::SetViewport(CommandBufferHandle cmdHandle, float x, float y, float width, float height)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->commandBuffer->setViewport(0, vk::Viewport{x, y, width, height, 0.0f, 1.0f});
}

void VulkanRenderDevice::SetScissor(CommandBufferHandle cmdHandle, int32_t x, int32_t y, uint32_t width,
                                    uint32_t height)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->commandBuffer->setScissor(0, vk::Rect2D{{x, y}, {width, height}});
}

void VulkanRenderDevice::Draw(CommandBufferHandle cmdHandle, uint32_t vertexCount, uint32_t instanceCount,
                              uint32_t firstVertex, uint32_t firstInstance)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->commandBuffer->draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanRenderDevice::DrawIndexed(CommandBufferHandle cmdHandle, uint32_t indexCount, uint32_t instanceCount,
                                     uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    cmdImpl->commandBuffer->drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanRenderDevice::PushConstants(CommandBufferHandle cmdHandle, PipelineHandle pipeline, ShaderStageFlags stages,
                                       uint32_t offset, uint32_t size, const void *data)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
        return;
    vk::PipelineLayout layout = GetVulkanDevice()->GetVulkanPipelineLayout(pipeline);
    cmdImpl->commandBuffer->pushConstants(layout, ToVk(stages), offset, size, data);
}

void VulkanRenderDevice::BindPipeline(CommandBufferHandle cmdHandle, PipelineHandle pipeline)
{
    VulkanCommandBuffer *cmd = m_commandBufferPool.Get(cmdHandle);
    if (!cmd)
    {
        return;
    }

    cmd->commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, GetVulkanDevice()->GetVulkanPipeline(pipeline));
}

void VulkanRenderDevice::BindVertexBuffer(CommandBufferHandle cmdHandle, BufferHandle buffer, uint32_t binding,
                                          uint64_t offset)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    vk::Buffer vkBuffer = GetVulkanDevice()->GetVulkanBuffer(buffer);
    vk::DeviceSize vkOffset = offset;
    cmdImpl->commandBuffer->bindVertexBuffers(binding, vkBuffer, vkOffset);
}

void VulkanRenderDevice::BindIndexBuffer(CommandBufferHandle cmdHandle, BufferHandle buffer, IndexType indexType,
                                         uint64_t offset)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    vk::Buffer vkBuffer = GetVulkanDevice()->GetVulkanBuffer(buffer);
    cmdImpl->commandBuffer->bindIndexBuffer(vkBuffer, offset, ToVk(indexType));
}

void VulkanRenderDevice::BindDescriptorSet(CommandBufferHandle cmdHandle, PipelineHandle pipeline,
                                           DescriptorSetHandle setHandle, uint32_t setIndex)
{
    auto *cmdImpl = m_commandBufferPool.Get(cmdHandle);
    if (!cmdImpl)
    {
        return;
    }
    VulkanDevice *device = GetVulkanDevice();
    vk::PipelineLayout layout = device->GetVulkanPipelineLayout(pipeline);
    vk::DescriptorSet set = device->GetVulkanDescriptorSet(setHandle);

    cmdImpl->commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, setIndex, set, {});
}
