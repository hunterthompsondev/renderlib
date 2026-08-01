#pragma once

#include <cstdint>

#include <renderlib/Enums.hpp>

constexpr uint32_t GetFormatByteSize(Format format)
{
    switch (format)
    {
    case Format::R8Unorm:
        return 1;
    case Format::RG8Unorm:
        return 2;
    case Format::RGBA8Unorm:
    case Format::RGBA8Srgb:
    case Format::BGRA8Unorm:
    case Format::BGRA8Srgb:
        return 4;
    case Format::R16Float:
        return 2;
    case Format::RG16Float:
        return 4;
    case Format::RGBA16Float:
        return 8;
    case Format::R32Float:
        return 4;
    case Format::RGBA32Float:
        return 16;
    case Format::D16Unorm:
        return 2;
    case Format::D32Float:
        return 4;
    case Format::D24UnormS8Uint:
        return 4;
    case Format::D32FloatS8Uint:
        return 8;
    default:
        return 0;
    }
}

constexpr bool IsDepthFormat(Format format)
{
    switch (format)
    {
    case Format::D16Unorm:
    case Format::D32Float:
    case Format::D24UnormS8Uint:
    case Format::D32FloatS8Uint:
        return true;
    default:
        return false;
    }
}

constexpr bool HasStencilComponent(Format format)
{
    return format == Format::D24UnormS8Uint || format == Format::D32FloatS8Uint;
}
