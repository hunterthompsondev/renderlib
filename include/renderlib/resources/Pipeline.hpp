#pragma once

#include <vector>

#include "DescriptorSet.hpp"
#include "ShaderModule.hpp"
#include <renderlib/Handles.hpp>

struct PipelineTag
{
};
using PipelineHandle = Handle<PipelineTag>;

struct PipelineCreateInfo
{
    ShaderModuleHandle vertexShader;
    ShaderModuleHandle fragmentShader;

    std::vector<DescriptorLayoutHandle> descriptorLayouts = {};
    std::vector<PushConstantRange> pushConstants = {};

    VertexLayout vertexLayout;

    PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;
    CullMode cullMode = CullMode::Back;

    bool depthTestEnable = true;
    bool depthWriteEnable = true;
    CompareOp depthCompareOp = CompareOp::Less;

    BlendMode blendMode = BlendMode::Opaque;

    std::vector<Format> colorFormats;
    Format depthFormat = Format::Undefined;
};
