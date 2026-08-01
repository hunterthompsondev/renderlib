#pragma once

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>

struct SamplerTag
{
};
using SamplerHandle = Handle<SamplerTag>;

struct SamplerCreateInfo
{
    FilterMode magFilter = FilterMode::Linear;
    FilterMode minFilter = FilterMode::Linear;
    AddressMode addressModeU = AddressMode::Repeat;
    AddressMode addressModeV = AddressMode::Repeat;
    AddressMode addressModeW = AddressMode::Repeat;
    float maxAnisotropy = 1.0f; // 1.0 = disabled
};
