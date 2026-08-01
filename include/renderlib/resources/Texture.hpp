#pragma once

#include <cstdint>

#include <renderlib/Enums.hpp>
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
    TextureType type;
    TextureUsage usage;
    Format format;

    uint32_t width;
    uint32_t height;
    uint32_t depth;

    uint32_t mipLevels;
    uint32_t arrayLayers;
};

struct TextureViewCreateInfo
{
    TextureHandle sourceTexture;
    TextureType viewType; // May be different from the source texture type
    Format format;        // Source texture format is used when format is Undefined
};

// Utility functions
constexpr TextureUsage operator|(TextureUsage a, TextureUsage b)
{
    return static_cast<TextureUsage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
constexpr TextureUsage operator&(TextureUsage a, TextureUsage b)
{
    return static_cast<TextureUsage>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
constexpr bool hasFlag(TextureUsage value, TextureUsage flag)
{
    return (value & flag) == flag;
}
