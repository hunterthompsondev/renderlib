#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/resources/Texture.hpp>

struct VulkanTexture
{
    vk::Image imageHandle = nullptr; // always valid

    vk::raii::Image ownedImage = nullptr; // populated only when managesOwnMemory is true
    vk::raii::DeviceMemory memory = nullptr;
    vk::DeviceSize size = 0;
    bool managesOwnMemory = false;

    vk::ImageType type;
    vk::ImageUsageFlags usage;
    vk::Format format;
    vk::ImageAspectFlags imageAspectFlags;

    uint32_t width;
    uint32_t height;
    uint32_t depth;

    uint32_t mipLevels;
    uint32_t arrayLayers;
};

struct VulkanTextureView
{
    TextureHandle sourceTexture;
    vk::raii::ImageView imageView = nullptr;
};
