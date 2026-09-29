#include <cstring>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stb_image_write.h>

#include "Utils.hpp"
#include <renderlib/RenderLib.hpp>

struct AppContext
{
    Format colorFormat = Format::RGBA8Unorm;
    std::unique_ptr<RenderDevice> renderDevice = nullptr;
    Device *device = nullptr;

    uint32_t imageWidth = 1920;
    uint32_t imageHeight = 1080;
    TextureHandle renderTarget;
    TextureViewHandle renderTargetView;
    CommandBufferHandle commandBuffer;

    PipelineHandle pipeline;
    BufferHandle vertexBuffer;
    BufferHandle indexBuffer;

    BufferHandle imageOutputBuffer;
};

bool InitializeDevice(AppContext *context)
{
    context->renderDevice = RenderDevice::Create({});
    if (!context->renderDevice || !context->renderDevice->Initialize())
    {
        std::cerr << "Failed to initialize RenderDevice\n";
        return false;
    }

    context->device = context->renderDevice->GetDevice();
    if (!context->device->Initialize())
    {
        std::cerr << "Failed to initialize Device\n";
        return false;
    }

    return true;
}

bool InitializeRenderTarget(AppContext *context)
{
    context->renderTarget = context->device->CreateTexture({
        .type = ImageType::Texture2D,
        .usage = ImageUsageFlagBits::ColorAttachment | ImageUsageFlagBits::TransferSrc,
        .format = context->colorFormat,
        .imageAspectFlags = ImageAspectFlagBits::Color,
        .width = context->imageWidth,
        .height = context->imageHeight,
        .depth = 1,
        .mipLevels = 1,
        .arrayLayers = 1,
    });
    if (!context->renderTarget.isValid())
    {
        std::cerr << "Failed to create render target\n";
        return false;
    }

    context->renderTargetView = context->device->CreateTextureView({
        .sourceTexture = context->renderTarget,
        .viewType = ImageViewType::Texture2D,
        .imageAspectFlags = ImageAspectFlagBits::Color,
        .format = context->colorFormat,
    });
    if (!context->renderTargetView.isValid())
    {
        std::cerr << "Failed to create render target\n";
        return false;
    }

    context->commandBuffer = context->device->CreateCommandBuffer();
    if (!context->commandBuffer.isValid())
    {
        std::cerr << "Failed to create command Buffer\n";
        return false;
    }

    context->imageOutputBuffer = context->device->CreateBuffer({
        .bufferUsage = BufferUsageFlagBits::TransferDst,
        .sizeInBytes = context->imageWidth * context->imageHeight * sizeof(uint32_t),
    });
    if (!context->imageOutputBuffer.isValid())
    {
        std::cerr << "Failed to create image output buffer\n";
        return false;
    }

    return true;
}

bool InitializeGeometry(AppContext *context)
{
    // Geometry
    std::vector<float> vertices = {
        0.0f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    };
    std::vector<uint16_t> indices = {0, 1, 2};

    // Create vertex buffer
    context->vertexBuffer = context->device->CreateBuffer({
        .bufferUsage = BufferUsageFlagBits::Vertex,
        .data = vertices.data(),
        .sizeInBytes = vertices.size() * sizeof(float),
    });
    if (!context->vertexBuffer.isValid())
    {
        std::cerr << "Failed to create vertex buffer\n";
        return false;
    }

    // Create index buffer
    context->indexBuffer = context->device->CreateBuffer({
        .bufferUsage = BufferUsageFlagBits::Index,
        .data = indices.data(),
        .sizeInBytes = indices.size() * sizeof(uint16_t),
    });
    if (!context->indexBuffer.isValid())
    {
        std::cerr << "Failed to create index buffer\n";
        return false;
    }

    return true;
}

bool InitializePipeline(AppContext *context)
{
    // Create shader modules
    std::vector<uint8_t> vertBytes = ReadFileToBytes("shaders/static-hello-triangle.vert.spv");
    std::vector<uint8_t> fragBytes = ReadFileToBytes("shaders/static-hello-triangle.frag.spv");

    ShaderModuleHandle vertexShader = context->device->CreateShaderModule({
        .stage = ShaderStageFlagBits::Vertex,
        .entryPoint = "main",
        .bytecode = vertBytes,
    });
    if (!vertexShader.isValid())
    {
        std::cerr << "Failed to create vertex shader\n";
        return false;
    }

    ShaderModuleHandle fragmentShader = context->device->CreateShaderModule({
        .stage = ShaderStageFlagBits::Fragment,
        .entryPoint = "main",
        .bytecode = fragBytes,
    });
    if (!fragmentShader.isValid())
    {
        std::cerr << "Failed to create fragment shader\n";
        return false;
    }

    // Create pipeline
    context->pipeline = context->device->CreatePipeline({
        .vertexShader = vertexShader,
        .fragmentShader = fragmentShader,
        .descriptorLayouts = {},
        .pushConstants = {},
        .vertexLayout =
            {
                .stride = sizeof(float) * 2,
                .attributes =
                    {
                        {
                            .location = 0,
                            .offset = 0,
                            .format = Format::Float32x2,
                        },
                    },
            },
        .cullMode = CullModeFlagBits::None,
        .depthTestEnable = false,
        .depthWriteEnable = false,
        .depthCompareOp = CompareOp::Always,
        .colorFormats = {context->colorFormat},
    });
    if (!context->pipeline.isValid())
    {
        std::cerr << "Failed to create pipeline\n";
        return false;
    }

    // Free shader modules
    context->device->DestroyShaderModule(vertexShader);
    context->device->DestroyShaderModule(fragmentShader);

    return true;
}

int main()
{
    AppContext context;

    if (!InitializeDevice(&context) || !InitializeRenderTarget(&context) || !InitializeGeometry(&context) ||
        !InitializePipeline(&context))
    {
        return 1;
    }

    RenderDevice *renderDevice = context.renderDevice.get();
    Device *device = context.device;
    CommandBufferHandle cmd = context.commandBuffer;

    RenderingInfo renderingInfo{
        .width = context.imageWidth,
        .height = context.imageHeight,
        .colorAttachments = {{
            .view = context.renderTargetView,
            .loadOp = AttachmentLoadOp::Clear,
            .storeOp = AttachmentStoreOp::Store,
            .clearColor = {0.01f, 0.01f, 0.02f, 1.0f},
        }},
    };

    device->BeginRecording(cmd);
    device->TransitionTextureLayout(cmd, context.renderTarget, ImageLayout::Undefined,
                                    ImageLayout::ColorAttachmentOptimal, {}, AccessFlagBits::ColorAttachmentWrite,
                                    PipelineStageFlagBits::TopOfPipe, PipelineStageFlagBits::ColorAttachmentOutput,
                                    ImageAspectFlagBits::Color);
    device->BeginRendering(cmd, renderingInfo);
    device->SetViewport(cmd, 0.0f, 0.0f, static_cast<float>(context.imageWidth),
                        static_cast<float>(context.imageHeight));

    device->SetScissor(cmd, 0, 0, context.imageWidth, context.imageHeight);
    device->BindPipeline(cmd, context.pipeline);
    device->BindVertexBuffer(cmd, context.vertexBuffer);
    device->BindIndexBuffer(cmd, context.indexBuffer, IndexType::UInt16);
    device->DrawIndexed(cmd, 3, 1, 0, 0, 0);
    device->EndRendering(cmd);

    device->TransitionTextureLayout(cmd, context.renderTarget, ImageLayout::ColorAttachmentOptimal,
                                    ImageLayout::TransferSrcOptimal, AccessFlagBits::ColorAttachmentWrite,
                                    AccessFlagBits::TransferRead, PipelineStageFlagBits::ColorAttachmentOutput,
                                    PipelineStageFlagBits::Transfer, ImageAspectFlagBits::Color);
    device->CopyTextureToBuffer(cmd, context.renderTarget, context.imageOutputBuffer, {0, 0, 0},
                                {context.imageWidth, context.imageHeight, 1});

    device->EndRecording(cmd);

    device->OffscreenSubmit(cmd);

    device->WaitIdle();

    void *mapped = device->MapBuffer(context.imageOutputBuffer);
    std::vector<uint8_t> pixels(context.imageWidth * context.imageHeight * 4);
    std::memcpy(pixels.data(), mapped, pixels.size());
    device->UpmapBuffer(context.imageOutputBuffer);

    stbi_write_png("offscreen_output.png", context.imageWidth, context.imageHeight, 4, pixels.data(),
                   context.imageWidth * 4);

    std::cout << "Render complete\n";

    // renderDevice->BeginRendering(cmd, renderingInfo);
    // renderDevice->SetViewport(cmd, 0.0f, 0.0f, static_cast<float>(context.windowWidth),
    //                           static_cast<float>(context.windowHeight));

    // renderDevice->SetScissor(cmd, 0, 0, context.windowWidth, context.windowHeight);
    // renderDevice->BindPipeline(cmd, context.pipeline);
    // renderDevice->BindVertexBuffer(cmd, context.vertexBuffer);
    // renderDevice->BindIndexBuffer(cmd, context.indexBuffer, IndexType::UInt16);
    // renderDevice->DrawIndexed(cmd, 3, 1, 0, 0, 0);
    // renderDevice->EndRendering(cmd);
    //
    // renderDevice->EndFrame(context.swapchain);
    // renderDevice->Present(context.swapchain);

    // Cleanup
    device->WaitIdle();
    device->DestroyPipeline(context.pipeline);
    device->DestroyBuffer(context.vertexBuffer);
    device->DestroyBuffer(context.indexBuffer);
    // renderDevice->DestroySwapchain(context.swapchain);

    return 0;
}
