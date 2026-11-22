#pragma once

#include <cstdint>

#include <renderlib/utils/Bitmask.hpp>

namespace Renderlib::Type
{

enum class AdapterType : uint8_t
{
    Discrete,
    Integrated,
    Virtual,
    Cpu,
    Other,
};

enum class AdapterQueueFlags : uint8_t
{
    None = 0,
    Graphics = 1 << 0,
    Compute = 1 << 1,
    Transfer = 1 << 2,
};
RENDERLIB_ENABLE_BITMASK(AdapterQueueFlags);

} // namespace Renderlib::Type
