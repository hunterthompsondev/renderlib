target_sources(${PROJECT_NAME} PRIVATE
    device/VulkanDevice.cpp
    device/VulkanDeviceAccessors.cpp
    device/VulkanDeviceBuffer.cpp
    device/VulkanDeviceDescriptorSet.cpp
    device/VulkanDevicePipeline.cpp
    device/VulkanDeviceSampler.cpp
    device/VulkanDeviceShaderModule.cpp
    device/VulkanDeviceTexture.cpp
    #
    renderDevice/VulkanRenderDevice.cpp
    renderDevice/VulkanRenderDeviceCommands.cpp
    renderDevice/VulkanRenderDeviceSwapchain.cpp
    #
    VulkanContext.cpp
)

find_package(Vulkan REQUIRED)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        VK_USE_PLATFORM_WAYLAND_KHR
        VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Vulkan::Vulkan
)
