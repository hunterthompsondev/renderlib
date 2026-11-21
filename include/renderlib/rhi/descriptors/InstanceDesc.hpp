#pragma once

#include <renderlib/rhi/types/Types.hpp>

namespace Renderlib::Desc
{

struct InstanceDesc
{
    Type::Backend backend = Type::Backend::Vulkan;
};

} // namespace Renderlib::Desc
