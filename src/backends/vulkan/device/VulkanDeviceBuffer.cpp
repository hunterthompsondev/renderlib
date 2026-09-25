#include "VulkanDevice.hpp"

#include <iostream>

#include <backends/vulkan/utils/MemoryUtils.hpp>
#include <backends/vulkan/utils/ToVkFlags.hpp>

// Todo: Implement "Static" buffers
BufferHandle VulkanDevice::CreateBuffer(const BufferCreateInfo &desc)
{
    VulkanBuffer buffer{
        .size = static_cast<vk::DeviceSize>(desc.sizeInBytes),
        .usage = ToVk(desc.bufferUsage),
    };

    vk::BufferCreateInfo bufferCI{};
    bufferCI.setSize(desc.sizeInBytes).setUsage(ToVk(desc.bufferUsage)).setSharingMode(vk::SharingMode::eExclusive);

    // Create buffer
    buffer.buffer = vk::raii::Buffer(m_context.device, bufferCI);

    vk::MemoryRequirements memReqs = buffer.buffer.getMemoryRequirements();
    uint32_t memTypeIdx =
        FindMemoryType(m_context.physicalDevice, memReqs.memoryTypeBits,
                       vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    if (memTypeIdx == UINT32_MAX)
    {
        std::cerr << "[Vulkan Device] Valid memory type for buffer not found\n";
        return BufferHandle{};
    }

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.setAllocationSize(memReqs.size).setMemoryTypeIndex(memTypeIdx);

    // Allocate buffer memory
    buffer.memory = vk::raii::DeviceMemory(m_context.device, allocInfo);

    // Bind buffer memory
    buffer.buffer.bindMemory(*buffer.memory, 0);

    // Map initial data
    if (desc.data != nullptr && desc.sizeInBytes > 0)
    {
        void *mapped = buffer.memory.mapMemory(0, desc.sizeInBytes);
        std::memcpy(mapped, desc.data, desc.sizeInBytes);
        buffer.memory.unmapMemory();
    }

    return m_bufferPool.Create(std::move(buffer));
}

void VulkanDevice::DestroyBuffer(BufferHandle handle)
{
    m_bufferPool.Destroy(handle);
}

void VulkanDevice::UpdateBuffer(BufferHandle handle, std::span<const uint8_t> data, size_t offset)
{
    // Todo: Implement buffer updating to "Static" buffers
}

void *VulkanDevice::MapBuffer(BufferHandle handle)
{
    VulkanBuffer *buffer = m_bufferPool.Get(handle);
    if (!buffer)
    {
        return nullptr;
    }

    return buffer->memory.mapMemory(0, VK_WHOLE_SIZE);
}

void VulkanDevice::UpmapBuffer(BufferHandle handle)
{
    VulkanBuffer *buffer = m_bufferPool.Get(handle);
    if (!buffer)
    {
        return;
    }

    buffer->memory.unmapMemory();
}
