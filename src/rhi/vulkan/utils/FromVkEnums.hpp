#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <renderlib/rhi/types/Types.hpp>

namespace Renderlib
{

constexpr Type::AdapterType FromVk(vk::PhysicalDeviceType t)
{
    switch (t)
    {
    case vk::PhysicalDeviceType::eDiscreteGpu:
        return Type::AdapterType::Discrete;
    case vk::PhysicalDeviceType::eIntegratedGpu:
        return Type::AdapterType::Integrated;
    case vk::PhysicalDeviceType::eVirtualGpu:
        return Type::AdapterType::Virtual;
    case vk::PhysicalDeviceType::eCpu:
        return Type::AdapterType::Cpu;
    case vk::PhysicalDeviceType::eOther:
        return Type::AdapterType::Other;
    }

    return Type::AdapterType::Other;
}

} // namespace Renderlib
