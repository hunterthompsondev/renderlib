#pragma once

#include <cstdint>

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

} // namespace Renderlib::Type
