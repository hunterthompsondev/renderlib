#pragma once

#include <vector>

#include "DescriptorSet.hpp"
#include "ShaderModule.hpp"
#include <renderlib/Flags.hpp>
#include <renderlib/Handles.hpp>

struct PipelineTag
{
};
using PipelineHandle = Handle<PipelineTag>;

struct VertexAttribute
{
    uint32_t location;
    uint32_t offset;
    Format format;
};

struct VertexLayout
{
    uint32_t stride;
    std::vector<VertexAttribute> attributes;
};

struct PipelineCreateInfo
{
    ShaderModuleHandle vertexShader;
    ShaderModuleHandle fragmentShader;

    std::vector<DescriptorLayoutHandle> descriptorLayouts = {};
    std::vector<PushConstantRange> pushConstants = {};

    VertexLayout vertexLayout;

    PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;
    CullModeFlags cullMode = CullModeFlagBits::Back;

    bool depthTestEnable = true;
    bool depthWriteEnable = true;
    CompareOp depthCompareOp = CompareOp::Less;

    BlendMode blendMode = BlendMode::Opaque;

    std::vector<Format> colorFormats;
    Format depthFormat = Format::Undefined;
};
