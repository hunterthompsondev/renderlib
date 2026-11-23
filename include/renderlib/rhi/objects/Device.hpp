#pragma once

#include <vector>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

#include "Queue.hpp"

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

    virtual std::vector<Queue *> GetQueues() = 0;

  protected:
    Desc::DeviceDesc m_desc;
};

} // namespace Renderlib::Object
