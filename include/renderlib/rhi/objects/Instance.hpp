#pragma once

#include <memory>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

namespace Renderlib::Object
{

class Instance
{
  public:
    static std::unique_ptr<Instance> Create(const Desc::InstanceDesc &desc);
    virtual ~Instance() = default;

    Instance(const Instance &) = delete;
    Instance &operator=(const Instance &) = delete;
    Instance(Instance &) = delete;
    Instance &operator=(Instance &) = delete;

  protected:
    Instance(const Desc::InstanceDesc &desc);

    Desc::InstanceDesc m_desc;
};

} // namespace Renderlib::Object
