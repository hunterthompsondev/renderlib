target_sources(${PROJECT_NAME} PRIVATE
    objects/VulkanInstance.cpp
    objects/VulkanAdapter.cpp
    objects/VulkanDevice.cpp
    objects/VulkanQueueFamily.cpp
    objects/VulkanQueue.cpp
    objects/VulkanCommandPool.cpp
)

find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Vulkan::Vulkan
)

