#include <renderlib/rhi/objects/CommandPool.hpp>

namespace Renderlib::Object
{

CommandPool::CommandPool(const Desc::CommandPoolDesc &desc) : m_desc(desc)
{
}

Desc::CommandPoolDesc CommandPool::GetDesc() const
{
    return m_desc;
}

} // namespace Renderlib::Object
