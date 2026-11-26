#pragma once

#include <renderlib/rhi/descriptors/Descriptors.hpp>

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

  protected:
    Desc::CommandPoolDesc m_desc;
};

} // namespace Renderlib::Object
