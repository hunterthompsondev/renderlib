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

inline std::string ToString(AdapterQueueFlags f)
{
    if (f == AdapterQueueFlags::None)
        return "None";

    std::string s;
    auto add = [&](AdapterQueueFlags bit, const char *name) {
        if (Any(f & bit))
        {
            if (!s.empty())
                s += " | ";
            s += name;
        }
    };
    add(AdapterQueueFlags::Graphics, "Graphics");
    add(AdapterQueueFlags::Compute, "Compute");
    add(AdapterQueueFlags::Transfer, "Transfer");
    return s;
}

int main()
{
    std::unique_ptr<Instance> instance = Instance::Create({.backend = Backend::Vulkan, .debugMode = true});
    std::unique_ptr<Adapter> adapter;
    {
        std::vector<std::unique_ptr<Adapter>> adapters = instance->GetAdapters();

        std::cout << "Adapters (" << adapters.size() << ")\n";
        for (const auto &a : adapters)
        {
            AdapterDesc desc = a->GetInfo();
            std::cout << "Adapter\n"
                      << "  Name   : " << desc.name << '\n'
                      << "  Type   : " << ToString(desc.type) << '\n'
                      << "  Vendor : 0x" << std::hex << desc.vendorId << '\n'
                      << "  Device : 0x" << desc.deviceId << std::dec << '\n'
                      << "  VRAM   : " << desc.dedicatedVideoMemory / (1024 * 1024) << " MB\n";
        }
        adapter = std::move(adapters[0]);
    }

    std::vector<QueueFamilyDesc> queueFamilies = adapter->QueryQueueFamilyProperties();
    std::cout << adapter->GetInfo().name << ": Queue Families (" << queueFamilies.size() << ")\n";
    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        const QueueFamilyDesc &q = queueFamilies[i];
        std::cout << "  [" << i << "] " << ToString(q.flags) << '\n' << "      Queues : " << q.count << '\n';
    }

    return 0;
}
