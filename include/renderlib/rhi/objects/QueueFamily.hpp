#pragma once

#include <memory>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

#include "CommandPool.hpp"
#include "Queue.hpp"

namespace Renderlib::Object
{
class QueueFamily
{
  public:
    QueueFamily(const Desc::QueueFamilyDesc &desc);
    virtual ~QueueFamily() = default;

    QueueFamily(const QueueFamily &) = delete;
    QueueFamily &operator=(const QueueFamily &) = delete;
    QueueFamily(QueueFamily &&) = default;
    QueueFamily &operator=(QueueFamily &&) = default;

    Desc::QueueFamilyDesc GetDesc() const;

    virtual std::vector<Queue *> GetQueues() = 0;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const Desc::CommandPoolDesc &desc) = 0;

  protected:
    Desc::QueueFamilyDesc m_desc;
};

} // namespace Renderlib::Object
