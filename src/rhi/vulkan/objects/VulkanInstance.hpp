#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/objects/Instance.hpp>

// Maybe move to contstructor argument in the future.
static std::vector<const char *> debugLayers = {"VK_LAYER_KHRONOS_validation"};
static std::vector<const char *> layers = {};

static std::vector<const char *> debugExtentions = {vk::EXTDebugUtilsExtensionName};
static std::vector<const char *> extensions = {};

namespace Renderlib::Object
{

class VulkanInstance : public Instance
{
  public:
    static std::unique_ptr<VulkanInstance> Create(const Desc::InstanceDesc &desc);
    ~VulkanInstance() = default;

    VulkanInstance(const VulkanInstance &) = delete;
    VulkanInstance &operator=(const VulkanInstance &) = delete;
    VulkanInstance(VulkanInstance &) = delete;
    VulkanInstance &operator=(VulkanInstance &) = delete;

  private:
    VulkanInstance(const Desc::InstanceDesc &desc);

    vk::raii::Context m_context;
    vk::raii::Instance m_instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT m_debugMessenger = nullptr;
};

} // namespace Renderlib::Object
