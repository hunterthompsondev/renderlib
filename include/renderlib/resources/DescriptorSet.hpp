#pragma once

#include <vector>

#include "Buffer.hpp"
#include "Sampler.hpp"
#include "Texture.hpp"
#include <renderlib/Enums.hpp>
#include <renderlib/Flags.hpp>
#include <renderlib/Handles.hpp>

struct DescriptorLayoutTag
{
};
struct DescriptorSetTag
{
};

using DescriptorLayoutHandle = Handle<DescriptorLayoutTag>;
using DescriptorSetHandle = Handle<DescriptorSetTag>;

struct DescriptorBindingCreateInfo
{
    uint32_t binding = 0;
    DescriptorType type;
    uint32_t count = 1;
    ShaderStageFlags stageFlags = ShaderStageFlagBits::Fragment | ShaderStageFlagBits::Vertex;
};

struct DescriptorLayoutCreateInfo
{
    std::vector<DescriptorBindingCreateInfo> bindings;
};

struct PushConstantRange
{
    uint32_t offset = 0;
    uint32_t size = 0;
    ShaderStageFlags stageFlags = ShaderStageFlagBits::Fragment | ShaderStageFlagBits::Vertex;
};

struct DescriptorBindingWriteCreateInfo
{
    uint32_t binding = 0;
    DescriptorType type;

    // Only one of the following should be populated
    BufferHandle buffer;
    TextureViewHandle textureView;
    SamplerHandle sampler;

    uint64_t bufferOffset = 0;
    uint64_t bufferRange = 0; // 0 = whole buffer
};

struct DescriptorSetCreateInfo
{
    DescriptorLayoutHandle layout;
    std::vector<DescriptorBindingWriteCreateInfo> bindings;
};
