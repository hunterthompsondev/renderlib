#pragma once

#include <renderlib/rhi/objects/Instance.hpp>

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
};

} // namespace Renderlib::Object
