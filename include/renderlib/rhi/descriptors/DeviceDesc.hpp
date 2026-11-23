#pragma once

#include <vector>

#include <renderlib/rhi/types/Types.hpp>

namespace Renderlib::Desc
{

struct DeviceQueueDesc
{
    uint32_t familyIndex;
    std::vector<float> priorites; // Length encodes queue amount
};

struct DeviceDesc
{
    std::vector<DeviceQueueDesc> queues;
    bool debugMode = false;
};

} // namespace Renderlib::Desc
