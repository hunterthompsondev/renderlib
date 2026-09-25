#pragma once

#include <cstdint>

#include <renderlib/Enums.hpp>
#include <renderlib/Flags.hpp>
#include <renderlib/Handles.hpp>

struct TextureTag
{
};
using TextureHandle = Handle<TextureTag>;

struct TextureViewTag
{
};
using TextureViewHandle = Handle<TextureViewTag>;

struct TextureCreateInfo
{
    ImageType type;
    ImageUsageFlags usage;
    Format format;
    ImageAspectFlags imageAspectFlags;

    uint32_t width;
    uint32_t height;
    uint32_t depth;

    uint32_t mipLevels;
    uint32_t arrayLayers;
};

struct TextureViewCreateInfo
{
    TextureHandle sourceTexture;
    ImageViewType viewType; // May be different from the source texture type
    ImageAspectFlags imageAspectFlags;
    Format format;
};
