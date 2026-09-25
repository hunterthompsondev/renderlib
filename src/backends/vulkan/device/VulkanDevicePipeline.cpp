#include "VulkanDevice.hpp"

#include <iostream>

#include <backends/vulkan/utils/ToVkEnums.hpp>
#include <backends/vulkan/utils/ToVkFlags.hpp>

PipelineHandle VulkanDevice::CreatePipeline(const PipelineCreateInfo &desc)
{
    VulkanShaderModule *vertShader = m_shaderModulePool.Get(desc.vertexShader);
    VulkanShaderModule *fragShader = m_shaderModulePool.Get(desc.fragmentShader);
    if (!vertShader || !fragShader)
    {
        std::cerr << "[Vulkan Device] Required shader modules not found\n";
        return {};
    }

    VulkanPipeline pipeline{};

    vk::PipelineShaderStageCreateInfo vertShaderCI{};
    vertShaderCI.setStage(vk::ShaderStageFlagBits::eVertex)
        .setModule(vertShader->module)
        .setPName(vertShader->entryPoint.data());

    vk::PipelineShaderStageCreateInfo fragShaderCI{};
    fragShaderCI.setStage(vk::ShaderStageFlagBits::eFragment)
        .setModule(fragShader->module)
        .setPName(fragShader->entryPoint.data());

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages{
        vertShaderCI,
        fragShaderCI,
    };

    vk::VertexInputBindingDescription bindingDesc{};
    bindingDesc.binding = 0;
    bindingDesc.stride = desc.vertexLayout.stride;
    bindingDesc.inputRate = vk::VertexInputRate::eVertex;
    pipeline.vertexBindingDescription = bindingDesc;

    std::vector<vk::VertexInputAttributeDescription> attributeDescs;
    attributeDescs.reserve(desc.vertexLayout.attributes.size());

    for (const auto &attr : desc.vertexLayout.attributes)
    {
        vk::VertexInputAttributeDescription attributeDesc{};
        attributeDesc.location = attr.location;
        attributeDesc.binding = 0;
        attributeDesc.format = ToVk(attr.format);
        attributeDesc.offset = attr.offset;

        attributeDescs.push_back(attributeDesc);
    }
    pipeline.vertexAttributeDescription = attributeDescs;

    vk::PipelineVertexInputStateCreateInfo vertInputCI{};
    vertInputCI.setVertexBindingDescriptions(pipeline.vertexBindingDescription)
        .setVertexAttributeDescriptions(pipeline.vertexAttributeDescription);

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCI{};
    inputAssemblyCI.setTopology(ToVk(desc.primitiveTopology));

    vk::PipelineViewportStateCreateInfo viewportStateCI{};
    viewportStateCI.setViewportCount(1).setScissorCount(1);

    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.setDepthClampEnable(vk::False)
        .setRasterizerDiscardEnable(vk::False)
        .setPolygonMode(vk::PolygonMode::eFill)
        .setCullMode(ToVk(desc.cullMode))
        .setFrontFace(vk::FrontFace::eCounterClockwise)
        .setDepthBiasEnable(vk::False)
        .setLineWidth(1.0f);

    vk::PipelineMultisampleStateCreateInfo multisampling{};
    multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e1).setSampleShadingEnable(vk::False);

    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.setDepthTestEnable(desc.depthTestEnable)
        .setDepthWriteEnable(desc.depthWriteEnable)
        .setDepthCompareOp(ToVk(desc.depthCompareOp))
        .setDepthBoundsTestEnable(vk::False)
        .setStencilTestEnable(vk::False);

    pipeline.colorBlendAttachments = std::vector(desc.colorFormats.size(), ToVk(desc.blendMode));

    vk::PipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.setLogicOpEnable(vk::False)
        .setLogicOp(vk::LogicOp::eCopy)
        .setAttachments(pipeline.colorBlendAttachments);

    pipeline.dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };

    vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};
    dynamicStateInfo.setDynamicStates(pipeline.dynamicStates);

    std::vector<vk::DescriptorSetLayout> setLayouts;
    setLayouts.reserve(desc.descriptorLayouts.size());

    for (DescriptorLayoutHandle layoutHandle : desc.descriptorLayouts)
    {
        VulkanDescriptorLayout *layout = m_descriptorLayoutPool.Get(layoutHandle);
        if (!layout)
        {
            std::cerr << "[Vulkan Device] Invalid descriptor layout handle\n";
            return {};
        }

        setLayouts.push_back(*layout->layout);
    }

    std::vector<vk::PushConstantRange> pushConstantRanges;
    pushConstantRanges.reserve(desc.pushConstants.size());

    for (const PushConstantRange &range : desc.pushConstants)
    {
        pushConstantRanges.emplace_back(ToVk(range.stageFlags), range.offset, range.size);
    }

    vk::PipelineLayoutCreateInfo layoutCI{};
    layoutCI.setSetLayouts(setLayouts).setPushConstantRanges(pushConstantRanges);

    // Create pipeline layout
    pipeline.layout = vk::raii::PipelineLayout(m_context.device, layoutCI);

    std::vector<vk::Format> colorFormats;
    colorFormats.reserve(desc.colorFormats.size());

    for (Format format : desc.colorFormats)
    {
        colorFormats.push_back(ToVk(format));
    }

    vk::PipelineRenderingCreateInfo renderingInfo{};
    // TODO: more elegant solution1
    renderingInfo.setColorAttachmentFormats(colorFormats)
        .setDepthAttachmentFormat(desc.depthFormat != Format::Undefined ? ToVk(desc.depthFormat)
                                                                        : vk::Format::eUndefined)
        .setStencilAttachmentFormat(desc.depthFormat == Format::D24UnormS8Int ? ToVk(desc.depthFormat)
                                                                              : vk::Format::eUndefined);

    vk::GraphicsPipelineCreateInfo pipelineCI{};
    pipelineCI.setStages(shaderStages)
        .setPVertexInputState(&vertInputCI)
        .setPInputAssemblyState(&inputAssemblyCI)
        .setPViewportState(&viewportStateCI)
        .setPRasterizationState(&rasterizer)
        .setPMultisampleState(&multisampling)
        .setPDepthStencilState(&depthStencil)
        .setPColorBlendState(&colorBlendInfo)
        .setPDynamicState(&dynamicStateInfo)
        .setLayout(*pipeline.layout)
        .setPNext(&renderingInfo)
        .setSubpass(0);

    try
    {
        pipeline.pipeline = vk::raii::Pipeline(m_context.device, nullptr, pipelineCI);
    }
    catch (const vk::SystemError &e)
    {
        std::cerr << "[Vulkan Device] Failed to create graphics pipeline: " << e.what() << "\n";
        return PipelineHandle{};
    }

    return m_pipelinePool.Create(std::move(pipeline));
}

void VulkanDevice::DestroyPipeline(PipelineHandle handle)
{
    m_pipelinePool.Destroy(handle);
}
