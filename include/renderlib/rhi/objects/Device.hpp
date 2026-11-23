#pragma once

#include <renderlib/rhi/descriptors/Descriptors.hpp>

namespace Renderlib::Object
{
class Device
{
  public:
    Device(const Desc::DeviceDesc &desc);
    virtual ~Device() = default;

    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;
    Device(Device &) = delete;
    Device &operator=(Device &) = delete;

  protected:
    Desc::DeviceDesc m_desc;
};

} // namespace Renderlib::Object
