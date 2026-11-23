#include <renderlib/rhi/objects/Queue.hpp>

namespace Renderlib::Object
{

Queue::Queue(const Desc::QueueDesc &desc) : m_desc(desc)
{
}

Desc::QueueDesc Queue::GetDesc() const
{
    return m_desc;
}

} // namespace Renderlib::Object
