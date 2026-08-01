#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

struct VulkanPipeline
{
    vk::VertexInputBindingDescription vertexBindingDescription;
    std::vector<vk::VertexInputAttributeDescription> vertexAttributeDescription;
    std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;
    std::vector<vk::DynamicState> dynamicStates;
    vk::raii::Pipeline pipeline = nullptr;
    vk::raii::PipelineLayout layout = nullptr;
};
