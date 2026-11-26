#pragma once

#include <memory>

#include <renderlib/rhi/descriptors/Descriptors.hpp>

#include "CommandBuffer.hpp"

namespace Renderlib::Object
{
class CommandPool
{
  public:
    CommandPool(const Desc::CommandPoolDesc &desc);
    virtual ~CommandPool() = default;

    CommandPool(const CommandPool &) = delete;
    CommandPool &operator=(const CommandPool &) = delete;
    CommandPool(CommandPool &&) = default;
    CommandPool &operator=(CommandPool &&) = default;

    Desc::CommandPoolDesc GetDesc() const;

    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;

  protected:
    Desc::CommandPoolDesc m_desc;
};

} // namespace Renderlib::Object
