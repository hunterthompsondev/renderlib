#pragma once

#include <renderlib/rhi/descriptors/Descriptors.hpp>

namespace Renderlib::Object
{
class CommandBuffer
{
  public:
    CommandBuffer();
    virtual ~CommandBuffer() = default;

    CommandBuffer(const CommandBuffer &) = delete;
    CommandBuffer &operator=(const CommandBuffer &) = delete;
    CommandBuffer(CommandBuffer &&) = default;
    CommandBuffer &operator=(CommandBuffer &&) = default;
};

} // namespace Renderlib::Object
