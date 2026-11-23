#pragma once

#include <renderlib/rhi/descriptors/Descriptors.hpp>

namespace Renderlib::Object
{
class Queue
{
  public:
    Queue(const Desc::QueueDesc &desc);
    virtual ~Queue() = default;

    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;
    Queue(Queue &&) = default;
    Queue &operator=(Queue &&) = default;

    Desc::QueueDesc GetDesc() const;

  protected:
    Desc::QueueDesc m_desc;
};

} // namespace Renderlib::Object
