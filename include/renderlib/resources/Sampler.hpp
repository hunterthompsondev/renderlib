#pragma once

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>

struct SamplerTag
{
};
using SamplerHandle = Handle<SamplerTag>;

struct SamplerCreateInfo
{
    Filter magFilter = Filter::Linear;
    Filter minFilter = Filter::Linear;
    SamplerMipmapMode mipMapMode = SamplerMipmapMode::Linear;
    SamplerAddressMode addressModeU = SamplerAddressMode::Repeat;
    SamplerAddressMode addressModeV = SamplerAddressMode::Repeat;
    SamplerAddressMode addressModeW = SamplerAddressMode::Repeat;
    float maxAnisotropy = 1.0f; // 1.0 = disabled
};
