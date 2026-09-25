#include <cstring>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.hpp"
#include "renderlib/Enums.hpp"
#include "renderlib/Flags.hpp"
#include <renderlib/RenderLib.hpp>

struct Camera
{
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewProj;
};

struct ObjectPushConstants
{
    glm::mat4 model;
};

struct AppContext
{
    Format colorFormat = Format::BGRA8Srgb;
    Format depthFormat = Format::D32Float;
    std::unique_ptr<RenderDevice> renderDevice = nullptr;
    Device *device = nullptr;

    GLFWwindow *window = nullptr;
    NativeWindowHandle nativeWindowHandle;
    SwapchainHandle swapchain;
    uint32_t windowWidth = 0;
    uint32_t windowHeight = 0;

    PipelineHandle pipeline;
    BufferHandle vertexBuffer;
    BufferHandle indexBuffer;
    uint32_t vertexCount;
    uint32_t indexCount;

    TextureHandle sphereTexture;
    TextureViewHandle sphereTextureView;
    SamplerHandle sphereTextureSampler;

    BufferHandle uniformBuffer;

    DescriptorLayoutHandle layout;
    DescriptorSetHandle set;

    TextureHandle depthTexture;
    TextureViewHandle depthTextureView;

    glm::vec3 cameraPos;
    Camera camera;
};

// Utility methods
void RecreateDepthBuffer(AppContext *context)
{
    if (context->depthTextureView.isValid())
    {
        context->device->DestroyTextureView(context->depthTextureView);
    }
    if (context->depthTexture.isValid())
    {
        context->device->DestroyTexture(context->depthTexture);
    }

    context->depthTexture = context->device->CreateTexture({
        .type = ImageType::Texture2D,
        .usage = ImageUsageFlagBits::DepthStencil,
        .format = context->depthFormat,
        .imageAspectFlags = ImageAspectFlagBits::Depth,
        .width = context->windowWidth,
        .height = context->windowHeight,
        .depth = 1,
        .mipLevels = 1,
        .arrayLayers = 1,
    });

    context->depthTextureView = context->device->CreateTextureView({
        .sourceTexture = context->depthTexture,
        .viewType = ImageViewType::Texture2D,
        .imageAspectFlags = ImageAspectFlagBits::Depth,
        .format = context->depthFormat,
    });
}

void UpdateCamera(AppContext *context)
{
    glm::mat4 view = glm::lookAt(context->cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(
        glm::radians(60.0f), static_cast<float>(context->windowWidth) / static_cast<float>(context->windowHeight), 0.1f,
        100.0f);
    proj[1][1] *= -1.0f;

    context->camera = {
        .view = view,
        .proj = proj,
        .viewProj = proj * view,
    };
};

// Initialization methods
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

bool InitializeWindow(AppContext *context)
{
    // Create GLFW window
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    context->window = glfwCreateWindow(1280, 720, "hello-sphere", nullptr, nullptr);
    if (context->window == nullptr)
    {
        std::cerr << "Failed to initialize window\n";
        return false;
    }

    // Get window preferred size
    int width, height;
    glfwGetFramebufferSize(context->window, &width, &height);
    context->windowWidth = static_cast<uint32_t>(width);
    context->windowHeight = static_cast<uint32_t>(height);
    context->nativeWindowHandle = {
        .display = glfwGetWaylandDisplay(),
        .surface = glfwGetWaylandWindow(context->window),
    };

    // Create swapchain
    context->swapchain = context->renderDevice->CreateSwapchain({
        .window = context->nativeWindowHandle,
        .width = context->windowWidth,
        .height = context->windowHeight,
        .format = context->colorFormat,
    });
    if (!context->swapchain.isValid())
    {
        std::cerr << "Failed to create swapchain\n";
        return false;
    }

    // Register swapchain resize callback
    glfwSetWindowUserPointer(context->window, context);
    glfwSetFramebufferSizeCallback(context->window, [](GLFWwindow *w, int newWidth, int newHeight) {
        AppContext *context = static_cast<AppContext *>(glfwGetWindowUserPointer(w));
        context->windowWidth = static_cast<uint32_t>(newWidth);
        context->windowHeight = static_cast<uint32_t>(newHeight);
        context->renderDevice->ResizeSwapchain(context->swapchain, newWidth, newHeight);
        RecreateDepthBuffer(context);
    });

    return true;
}

bool InitializeGeometry(AppContext *context)
{
    // Geometry
    std::vector<float> vertices;
    std::vector<uint16_t> indices;
    LoadOBJ("models/Sphere.obj", vertices, indices);
    context->vertexCount = static_cast<uint32_t>(vertices.size());
    context->indexCount = static_cast<uint32_t>(indices.size());

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

bool InitializeUniformBuffers(AppContext *context)
{
    context->uniformBuffer = context->device->CreateBuffer({
        .bufferUsage = BufferUsageFlagBits::Uniform,
        .data = &context->camera,
        .sizeInBytes = sizeof(Camera),
    });
    if (!context->uniformBuffer.isValid())
    {
        std::cerr << "Failed to create uniform buffer\n";
        return false;
    }

    return true;
}

bool InitializeTextures(AppContext *context)
{
    // Load texture
    LoadedImage colorGridImage = LoadImageRGBA8("textures/ColorGrid.png");

    // Create texture
    context->sphereTexture = context->device->CreateTexture({
        .type = ImageType::Texture2D,
        .usage = ImageUsageFlagBits::Sampled | ImageUsageFlagBits::TransferDst,
        .format = Format::RGBA8Srgb,
        .imageAspectFlags = ImageAspectFlagBits::Color,
        .width = static_cast<uint32_t>(colorGridImage.width),
        .height = static_cast<uint32_t>(colorGridImage.height),
        .depth = 1,
        .mipLevels = 1,
        .arrayLayers = 1,
    });
    if (!context->sphereTexture.isValid())
    {
        std::cerr << "Failed to create sphere texture\n";
        return false;
    }

    context->device->UpdateTexture(context->sphereTexture, 0, 0, colorGridImage.pixels);

    // Create texture view
    context->sphereTextureView = context->device->CreateTextureView({
        .sourceTexture = context->sphereTexture,
        .viewType = ImageViewType::Texture2D,
        .imageAspectFlags = ImageAspectFlagBits::Color,
        .format = Format::RGBA8Srgb,
    });
    if (!context->sphereTextureView.isValid())
    {
        std::cerr << "Failed to create sphere exture view\n";
        return false;
    }

    // Create texture sampler
    context->sphereTextureSampler = context->device->CreateSampler({
        .magFilter = Filter::Linear,
        .minFilter = Filter::Linear,
        .mipMapMode = SamplerMipmapMode::Linear,
        .addressModeU = SamplerAddressMode::Repeat,
        .addressModeV = SamplerAddressMode::Repeat,
        .addressModeW = SamplerAddressMode::Repeat,
        .maxAnisotropy = 1.0f,
    });
    if (!context->sphereTextureSampler.isValid())
    {
        std::cerr << "Failed to create sphere texture sampler\n";
        return false;
    }

    return true;
}

bool InitializeDescriptors(AppContext *context)
{
    // Create layout
    context->layout = context->device->CreateDescriptorLayout({
        .bindings =
            {
                {
                    .binding = 0,
                    .type = DescriptorType::UniformBuffer,
                    .count = 1,
                    .stageFlags = ShaderStageFlagBits::Vertex,
                },
                {
                    .binding = 1,
                    .type = DescriptorType::SampledTexture,
                    .count = 1,
                    .stageFlags = ShaderStageFlagBits::Fragment,
                },
            },
    });
    if (!context->layout.isValid())
    {
        std::cerr << "Failed to create layout\n";
        return false;
    }

    // Create set
    context->set = context->device->CreateDescriptorSet({
        .layout = context->layout,
        .bindings =
            {
                {
                    .binding = 0,
                    .type = DescriptorType::UniformBuffer,
                    .buffer = context->uniformBuffer,
                    .bufferOffset = 0,
                    .bufferRange = 0,
                },
                {
                    .binding = 1,
                    .type = DescriptorType::SampledTexture,
                    .textureView = context->sphereTextureView,
                    .sampler = context->sphereTextureSampler,
                },
            },
    });
    if (!context->set.isValid())
    {
        std::cerr << "Failed to create set\n";
        return false;
    }

    return true;
}

bool InitializePipeline(AppContext *context)
{
    // Create shader modules
    std::vector<uint8_t> vertBytes = ReadFileToBytes("shaders/hello-sphere.vert.spv");
    std::vector<uint8_t> fragBytes = ReadFileToBytes("shaders/hello-sphere.frag.spv");

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
        .descriptorLayouts = {context->layout},
        .pushConstants =
            {
                {.offset = 0, .size = sizeof(ObjectPushConstants), .stageFlags = ShaderStageFlagBits::Vertex},
            },
        .vertexLayout =
            {
                .stride = sizeof(float) * 8,
                .attributes =
                    {
                        {.location = 0, .offset = 0, .format = Format::Float32x3},
                        {.location = 1, .offset = sizeof(float) * 3, .format = Format::Float32x2},
                        {.location = 2, .offset = sizeof(float) * 5, .format = Format::Float32x3},
                    },
            },
        .colorFormats = {context->colorFormat},
        .depthFormat = context->depthFormat,
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

    context.cameraPos = glm::vec3{0.0f, 2.0f, 6.0f};

    if (!InitializeDevice(&context) || !InitializeWindow(&context) || !InitializeGeometry(&context) ||
        !InitializeUniformBuffers(&context) || !InitializeTextures(&context) || !InitializeDescriptors(&context) ||
        !InitializePipeline(&context))
    {
        return 1;
    }

    // Create depth buffer for the first time
    RecreateDepthBuffer(&context);

    RenderDevice *renderDevice = context.renderDevice.get();
    Device *device = context.device;

    float angle = 0.0f;
    while (!glfwWindowShouldClose(context.window))
    {
        glfwPollEvents();

        if (!context.renderDevice->AcquireNextImage(context.swapchain))
        {
            // Stale swapchain
            context.renderDevice->ResizeSwapchain(context.swapchain, context.windowWidth, context.windowHeight);
            RecreateDepthBuffer(&context);
            continue;
        }

        // Update
        UpdateCamera(&context);
        void *mappedCamera = device->MapBuffer(context.uniformBuffer);
        std::memcpy(mappedCamera, &context.camera, sizeof(Camera));
        device->UpmapBuffer(context.uniformBuffer);

        angle += 0.01f;
        ObjectPushConstants objectData{
            .model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.3f, 1.0f, 0.0f)),
        };

        // Render
        CommandBufferHandle cmd = context.renderDevice->BeginFrame(context.swapchain);

        RenderingInfo renderingInfo{
            .width = context.windowWidth,
            .height = context.windowHeight,
            .colorAttachments = {{
                .view = renderDevice->GetCurrentSwapchainImageView(context.swapchain),
                .loadOp = AttachmentLoadOp::Clear,
                .storeOp = AttachmentStoreOp::Store,
                .clearColor = {0.01f, 0.01f, 0.02f, 1.0f},
            }},
            .depthAttachment =
                DepthAttachmentInfo{
                    .view = context.depthTextureView,
                    .loadOp = AttachmentLoadOp::Clear,
                    .storeOp = AttachmentStoreOp::DontCare,
                    .clearDepth = 1.0f,
                },
        };

        renderDevice->BeginRendering(cmd, renderingInfo);
        renderDevice->SetViewport(cmd, 0.0f, 0.0f, static_cast<float>(context.windowWidth),
                                  static_cast<float>(context.windowHeight));
        renderDevice->SetScissor(cmd, 0, 0, context.windowWidth, context.windowHeight);

        renderDevice->BindPipeline(cmd, context.pipeline);
        renderDevice->BindDescriptorSet(cmd, context.pipeline, context.set);
        renderDevice->PushConstants(cmd, context.pipeline, ShaderStageFlagBits::Vertex, 0, sizeof(ObjectPushConstants),
                                    &objectData);
        renderDevice->BindVertexBuffer(cmd, context.vertexBuffer);
        renderDevice->BindIndexBuffer(cmd, context.indexBuffer, IndexType::UInt16);

        renderDevice->DrawIndexed(cmd, context.indexCount, 1, 0, 0, 0);

        renderDevice->EndRendering(cmd);
        renderDevice->EndFrame(context.swapchain);
        renderDevice->Present(context.swapchain);
    }

    // Cleanup
    device->WaitIdle();
    device->DestroyPipeline(context.pipeline);
    device->DestroyDescriptorSet(context.set);
    device->DestroyDescriptorLayout(context.layout);
    device->DestroySampler(context.sphereTextureSampler);
    device->DestroyTextureView(context.sphereTextureView);
    device->DestroyTexture(context.sphereTexture);
    device->DestroyBuffer(context.vertexBuffer);
    device->DestroyBuffer(context.indexBuffer);
    device->DestroyBuffer(context.uniformBuffer);
    device->DestroyTextureView(context.depthTextureView);
    device->DestroyTexture(context.depthTexture);
    renderDevice->DestroySwapchain(context.swapchain);

    glfwDestroyWindow(context.window);
    glfwTerminate();

    return 0;
}
