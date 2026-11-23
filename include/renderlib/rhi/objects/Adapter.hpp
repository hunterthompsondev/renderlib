#pragma once

#include <memory>
#include <vector>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

#include "Device.hpp"

namespace Renderlib::Object
{
class Adapter
{
  public:
    Adapter(const Desc::AdapterDesc &desc);
    virtual ~Adapter() = default;

    Adapter(const Adapter &) = delete;
    Adapter &operator=(const Adapter &) = delete;
    Adapter(Adapter &&) = default;
    Adapter &operator=(Adapter &&) = default;

    Desc::AdapterDesc GetInfo() const;

    virtual std::vector<Desc::AdapterQueueFamilyInfo> QueryQueueFamilyProperties() const = 0;
    virtual std::unique_ptr<Device> CreateDevice(const Desc::DeviceDesc &desc) const = 0;

  protected:
    Desc::AdapterDesc m_desc;
};

} // namespace Renderlib::Object
