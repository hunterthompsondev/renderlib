#pragma once

#include <renderlib/rhi/types/Types.hpp>

namespace Renderlib::Desc
{

struct AdapterQueueFamilyInfo
{
    Type::AdapterQueueFlags flags;
    uint32_t count;
};

struct AdapterDesc
{
    char name[256] = {};
    Type::AdapterType type = Type::AdapterType::Other;
    uint32_t vendorId = 0; // raw PCI id
    uint32_t deviceId = 0;
    uint64_t dedicatedVideoMemory = 0; // bytes
};

} // namespace Renderlib::Desc
