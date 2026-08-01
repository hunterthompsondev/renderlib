#include "VulkanDevice.hpp"

#include <iostream>

#include <backends/vulkan/utils/VulkanConversions.hpp>

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

    pipeline.vertexBindingDescription = ToVulkanBindingDescription(desc.vertexLayout);
    pipeline.vertexAttributeDescription = ToVulkanAttributeDescriptions(desc.vertexLayout);

    vk::PipelineVertexInputStateCreateInfo vertInputCI{};
    vertInputCI.setVertexBindingDescriptions(pipeline.vertexBindingDescription)
        .setVertexAttributeDescriptions(pipeline.vertexAttributeDescription);

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCI{};
    inputAssemblyCI.setTopology(ToVulkanPrimitiveTopology(desc.primitiveTopology));

    vk::PipelineViewportStateCreateInfo viewportStateCI{};
    viewportStateCI.setViewportCount(1).setScissorCount(1);

    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.setDepthClampEnable(vk::False)
        .setRasterizerDiscardEnable(vk::False)
        .setPolygonMode(vk::PolygonMode::eFill)
        .setCullMode(ToVulkanCullMode(desc.cullMode))
        .setFrontFace(vk::FrontFace::eCounterClockwise)
        .setDepthBiasEnable(vk::False)
        .setLineWidth(1.0f);

    vk::PipelineMultisampleStateCreateInfo multisampling{};
    multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e1).setSampleShadingEnable(vk::False);

    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.setDepthTestEnable(desc.depthTestEnable)
        .setDepthWriteEnable(desc.depthWriteEnable)
        .setDepthCompareOp(ToVulkanCompareOp(desc.depthCompareOp))
        .setDepthBoundsTestEnable(vk::False)
        .setStencilTestEnable(vk::False);

    pipeline.colorBlendAttachments =
        std::vector(desc.colorFormats.size(), ToVulkanColorBlendAttachment(desc.blendMode));

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
        pushConstantRanges.emplace_back(ToVulkanShaderStageFlags(range.stageFlags), range.offset, range.size);
    }

    vk::PipelineLayoutCreateInfo layoutCI{};
    layoutCI.setSetLayouts(setLayouts).setPushConstantRanges(pushConstantRanges);

    // Create pipeline layout
    pipeline.layout = vk::raii::PipelineLayout(m_context.device, layoutCI);

    std::vector<vk::Format> colorFormats;
    colorFormats.reserve(desc.colorFormats.size());

    for (Format format : desc.colorFormats)
    {
        colorFormats.push_back(ToVulkanFormat(format));
    }

    vk::PipelineRenderingCreateInfo renderingInfo{};
    renderingInfo.setColorAttachmentFormats(colorFormats)
        .setDepthAttachmentFormat(desc.depthFormat != Format::Undefined ? ToVulkanFormat(desc.depthFormat)
                                                                        : vk::Format::eUndefined)
        .setStencilAttachmentFormat(HasStencilComponent(desc.depthFormat) ? ToVulkanFormat(desc.depthFormat)
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
