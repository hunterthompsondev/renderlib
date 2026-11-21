#include <renderlib/rhi/objects/Adapter.hpp>

namespace Renderlib::Object
{

Adapter::Adapter(const Desc::AdapterDesc &desc) : m_desc(desc)
{
}

Desc::AdapterDesc Adapter::GetInfo() const
{
    return m_desc;
}

} // namespace Renderlib::Object
