#pragma once

#include "vulkan/vulkan.hpp"
#include <renderlib/Enums.hpp>
#include <vulkan/vulkan_raii.hpp>

constexpr vk::Format ToVk(Format format)
{

    switch (format)
    {
    case Format::Undefined:
        return vk::Format::eUndefined;
    // 8 bit channels
    case Format::R8Unorm:
        return vk::Format::eR8Unorm;
    case Format::RG8Unorm:
        return vk::Format::eR8G8Unorm;
    case Format::RGB8Unorm:
        return vk::Format::eR8G8B8Unorm;
    case Format::RGBA8Unorm:
        return vk::Format::eR8G8B8A8Unorm;
    case Format::RGBA8Srgb:
        return vk::Format::eR8G8B8A8Srgb;
    case Format::BGRA8Unorm:
        return vk::Format::eB8G8R8A8Unorm;
    case Format::BGRA8Srgb:
        return vk::Format::eB8G8R8A8Srgb;
    // 16 bit channels
    case Format::R16Float:
        return vk::Format::eR16Sfloat;
    case Format::RG16Float:
        return vk::Format::eR16G16Sfloat;
    case Format::RGB16Float:
        return vk::Format::eR16G16B16Sfloat;
    case Format::RGBA16Float:
        return vk::Format::eR16G16B16A16Sfloat;
    // 32 bit channels
    case Format::R32Float:
        return vk::Format::eR32Sfloat;
    case Format::RG32Float:
        return vk::Format::eR32G32Sfloat;
    case Format::RGB32Float:
        return vk::Format::eR32G32B32Sfloat;
    case Format::RGBA32Float:
        return vk::Format::eR32G32B32A32Sfloat;
    // 32 bit aliases
    case Format::Float32:
        return vk::Format::eR32Sfloat;
    case Format::Float32x2:
        return vk::Format::eR32G32Sfloat;
    case Format::Float32x3:
        return vk::Format::eR32G32B32Sfloat;
    case Format::Float32x4:
        return vk::Format::eR32G32B32A32Sfloat;
    // Depth formats
    case Format::D16Unorm:
        return vk::Format::eD16Unorm;
    case Format::D24UnormS8Int:
        return vk::Format::eD24UnormS8Uint;
    case Format::D32Float:
        return vk::Format::eD32Sfloat;
    }
}

constexpr vk::PrimitiveTopology ToVk(PrimitiveTopology topology)
{
    switch (topology)
    {
    case PrimitiveTopology::PointList:
        return vk::PrimitiveTopology::ePointList;
    case PrimitiveTopology::LineList:
        return vk::PrimitiveTopology::eLineList;
    case PrimitiveTopology::TriangleList:
        return vk::PrimitiveTopology::eTriangleList;
    }
}

constexpr vk::CompareOp ToVk(CompareOp op)
{
    switch (op)
    {
    case CompareOp::Always:
        return vk::CompareOp::eAlways;
    case CompareOp::Equal:
        return vk::CompareOp::eEqual;
    case CompareOp::Less:
        return vk::CompareOp::eLess;
    case CompareOp::Greater:
        return vk::CompareOp::eGreater;
    }
}

constexpr vk::DescriptorType ToVk(DescriptorType type)
{
    switch (type)
    {
    case DescriptorType::UniformBuffer:
        return vk::DescriptorType::eUniformBuffer;
    case DescriptorType::StorageBuffer:
        return vk::DescriptorType::eStorageBuffer;
    case DescriptorType::SampledTexture:
        return vk::DescriptorType::eCombinedImageSampler;
    case DescriptorType::StorageTexture:
        return vk::DescriptorType::eStorageImage;
    case DescriptorType::Sampler:
        return vk::DescriptorType::eSampler;
    }
}

constexpr vk::Filter ToVk(Filter filter)
{
    switch (filter)
    {
    case Filter::Nearest:
        return vk::Filter::eNearest;
    case Filter::Linear:
        return vk::Filter::eLinear;
    }
}

constexpr vk::SamplerMipmapMode ToVk(SamplerMipmapMode mode)
{
    switch (mode)
    {
    case SamplerMipmapMode::Nearest:
        return vk::SamplerMipmapMode::eNearest;
    case SamplerMipmapMode::Linear:
        return vk::SamplerMipmapMode::eLinear;
    }
}

constexpr vk::SamplerAddressMode ToVk(SamplerAddressMode mode)
{
    switch (mode)
    {
    case SamplerAddressMode::Repeat:
        return vk::SamplerAddressMode::eRepeat;
    case SamplerAddressMode::ClampToEdge:
        return vk::SamplerAddressMode::eClampToEdge;
    case SamplerAddressMode::ClampToBorder:
        return vk::SamplerAddressMode::eClampToBorder;
    case SamplerAddressMode::MirroredRepeat:
        return vk::SamplerAddressMode::eMirroredRepeat;
    }
}

constexpr vk::ImageType ToVk(ImageType type)
{
    switch (type)
    {
    case ImageType::Texture1D:
        return vk::ImageType::e1D;
    case ImageType::Texture2D:
        return vk::ImageType::e2D;
    case ImageType::Texture3D:
        return vk::ImageType::e3D;
    case ImageType::TextureCube:
        return vk::ImageType::e2D;
    }
}

constexpr vk::ImageViewType ToVk(ImageViewType type)
{
    switch (type)
    {
    case ImageViewType::Texture1D:
        return vk::ImageViewType::e1D;
    case ImageViewType::Texture2D:
        return vk::ImageViewType::e2D;
    case ImageViewType::Texture3D:
        return vk::ImageViewType::e3D;
    case ImageViewType::TextureCube:
        return vk::ImageViewType::e2D;
    }
}

constexpr vk::PresentModeKHR ToVk(PresentMode mode)
{
    switch (mode)
    {
    case PresentMode::Immediate:
        return vk::PresentModeKHR::eImmediate;
    case PresentMode::Fifo:
        return vk::PresentModeKHR::eFifo;
    case PresentMode::Mailbox:
        return vk::PresentModeKHR::eMailbox;
    }
}

constexpr vk::AttachmentLoadOp ToVk(AttachmentLoadOp op)
{
    switch (op)
    {
    case AttachmentLoadOp::Load:
        return vk::AttachmentLoadOp::eLoad;
    case AttachmentLoadOp::Clear:
        return vk::AttachmentLoadOp::eClear;
    case AttachmentLoadOp::DontCare:
        return vk::AttachmentLoadOp::eDontCare;
    }
}

constexpr vk::AttachmentStoreOp ToVk(AttachmentStoreOp op)
{
    switch (op)
    {
    case AttachmentStoreOp::Store:
        return vk::AttachmentStoreOp::eStore;
    case AttachmentStoreOp::DontCare:
        return vk::AttachmentStoreOp::eDontCare;
    }
}

constexpr vk::IndexType ToVk(IndexType type)
{
    switch (type)
    {
    case IndexType::UInt16:
        return vk::IndexType::eUint16;
    case IndexType::UInt32:
        return vk::IndexType::eUint32;
    }
}

inline vk::PipelineColorBlendAttachmentState ToVk(BlendMode mode)
{
    vk::PipelineColorBlendAttachmentState state{};
    state.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    switch (mode)
    {
    case BlendMode::Opaque:
        state.setBlendEnable(vk::False);
        break;

    case BlendMode::Alpha:
        state.setBlendEnable(vk::True)
            .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
            .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
            .setColorBlendOp(vk::BlendOp::eAdd)
            .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
            .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
            .setAlphaBlendOp(vk::BlendOp::eAdd);
        break;

    case BlendMode::Additive:
        state.setBlendEnable(vk::True)
            .setSrcColorBlendFactor(vk::BlendFactor::eOne)
            .setDstColorBlendFactor(vk::BlendFactor::eOne)
            .setColorBlendOp(vk::BlendOp::eAdd)
            .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
            .setDstAlphaBlendFactor(vk::BlendFactor::eOne)
            .setAlphaBlendOp(vk::BlendOp::eAdd);
        break;

    default:
        state.setBlendEnable(vk::False);
        break;
    }

    return state;
}

inline vk::DescriptorSetLayoutBinding ToVk(uint32_t binding, uint32_t count, vk::DescriptorType type,
                                           vk::ShaderStageFlags stages)
{
    vk::DescriptorSetLayoutBinding vkBinding{};

    vkBinding.setBinding(binding).setDescriptorType(type).setDescriptorCount(count).setStageFlags(stages);

    return vkBinding;
}
