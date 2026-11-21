#include <iostream>
#include <memory>
#include <vector>

#include <renderlib/rhi/RHI.hpp>

using namespace Renderlib::Type;
using namespace Renderlib::Object;
using namespace Renderlib::Desc;

inline const char *ToString(AdapterType t)
{
    switch (t)
    {
    case AdapterType::Discrete:
        return "Discrete";
    case AdapterType::Integrated:
        return "Integrated";
    case AdapterType::Virtual:
        return "Virtual";
    case AdapterType::Cpu:
        return "Cpu";
    case AdapterType::Other:
        return "Other";
    }
    return "Unknown";
}

int main()
{
    std::unique_ptr<Instance> instance = Instance::Create({.backend = Backend::Vulkan, .debugMode = true});
    std::vector<std::unique_ptr<Adapter>> adapters = instance->GetAdapters();

    for (const auto &adapter : adapters)
    {
        AdapterDesc desc = adapter->GetInfo();
        std::cout << "Adapter\n"
                  << "  Name   : " << desc.name << '\n'
                  << "  Type   : " << ToString(desc.type) << '\n'
                  << "  Vendor : 0x" << std::hex << desc.vendorId << '\n'
                  << "  Device : 0x" << desc.deviceId << std::dec << '\n'
                  << "  VRAM   : " << desc.dedicatedVideoMemory / (1024 * 1024) << " MB\n";
    }

    return 0;
}
