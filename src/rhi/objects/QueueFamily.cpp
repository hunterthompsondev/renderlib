#include <renderlib/rhi/objects/QueueFamily.hpp>

namespace Renderlib::Object
{

QueueFamily::QueueFamily(const Desc::QueueFamilyDesc &desc) : m_desc(desc)
{
}

Desc::QueueFamilyDesc QueueFamily::GetDesc() const
{
    return m_desc;
}

} // namespace Renderlib::Object
