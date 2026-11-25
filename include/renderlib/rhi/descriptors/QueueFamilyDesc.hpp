#pragma once

#include <cstdint>

namespace Renderlib::Desc
{

struct QueueFamilyDesc
{
    uint32_t familyIndex;
    uint32_t queueCount;
};

} // namespace Renderlib::Desc
