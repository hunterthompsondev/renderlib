#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Utils.hpp"
#include <renderlib/RenderLib.hpp>

struct AppContext
{
    Format colorFormat = Format::BGRA8Srgb;
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

bool InitializeWindow(AppContext *context)
{
    // Create GLFW window
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    context->window = glfwCreateWindow(1280, 720, "hello-triangle", nullptr, nullptr);
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
    });

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
    std::vector<uint8_t> vertBytes = ReadFileToBytes("shaders/hello-triangle.vert.spv");
    std::vector<uint8_t> fragBytes = ReadFileToBytes("shaders/hello-triangle.frag.spv");

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

    if (!InitializeDevice(&context) || !InitializeWindow(&context) || !InitializeGeometry(&context) ||
        !InitializePipeline(&context))
    {
        return 1;
    }

    RenderDevice *renderDevice = context.renderDevice.get();
    Device *device = context.device;

    while (!glfwWindowShouldClose(context.window))
    {
        glfwPollEvents();

        if (!context.renderDevice->AcquireNextImage(context.swapchain))
        {
            // Stale swapchain
            context.renderDevice->ResizeSwapchain(context.swapchain, context.windowWidth, context.windowHeight);
            continue;
        }

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
        };

        renderDevice->BeginRendering(cmd, renderingInfo);
        renderDevice->SetViewport(cmd, 0.0f, 0.0f, static_cast<float>(context.windowWidth),
                                  static_cast<float>(context.windowHeight));
        renderDevice->SetScissor(cmd, 0, 0, context.windowWidth, context.windowHeight);
        renderDevice->BindPipeline(cmd, context.pipeline);
        renderDevice->BindVertexBuffer(cmd, context.vertexBuffer);
        renderDevice->BindIndexBuffer(cmd, context.indexBuffer, IndexType::UInt16);
        renderDevice->DrawIndexed(cmd, 3, 1, 0, 0, 0);
        renderDevice->EndRendering(cmd);

        renderDevice->EndFrame(context.swapchain);
        renderDevice->Present(context.swapchain);
    }

    // Cleanup
    device->WaitIdle();
    device->DestroyPipeline(context.pipeline);
    device->DestroyBuffer(context.vertexBuffer);
    device->DestroyBuffer(context.indexBuffer);
    renderDevice->DestroySwapchain(context.swapchain);

    glfwDestroyWindow(context.window);
    glfwTerminate();

    return 0;
}
