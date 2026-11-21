#pragma once

#include <memory>
#include <vector>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

#include "Adapter.hpp"

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

    virtual std::vector<std::unique_ptr<Object::Adapter>> GetAdapters() const = 0;

  protected:
    Instance(const Desc::InstanceDesc &desc);

    Desc::InstanceDesc m_desc;
};

} // namespace Renderlib::Object
