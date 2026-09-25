#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>
#include <renderlib/resources/Texture.hpp>

struct ColorAttachmentInfo
{
    TextureViewHandle view;
    AttachmentLoadOp loadOp = AttachmentLoadOp::Clear;
    AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
};

struct DepthAttachmentInfo
{
    TextureViewHandle view;
    AttachmentLoadOp loadOp = AttachmentLoadOp::Clear;
    AttachmentStoreOp storeOp = AttachmentStoreOp::DontCare;
    float clearDepth = 1.0f;
};

struct RenderingInfo
{
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<ColorAttachmentInfo> colorAttachments;
    std::optional<DepthAttachmentInfo> depthAttachment;
};
