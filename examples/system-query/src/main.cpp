#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <renderlib/rhi/RHI.hpp>

using namespace Renderlib::Type;
using namespace Renderlib::Object;
using namespace Renderlib::Desc;

// ---------------------------------------------------------------------------
// String helpers
// ---------------------------------------------------------------------------

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

// "0x10DE": built in its own stream so std::hex never leaks into std::cout
inline std::string HexId(uint32_t value)
{
    std::ostringstream ss;
    ss << "0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << value;
    return ss.str();
}

// "11.41 GB (11688 MB)"
inline std::string FormatMemory(uint64_t bytes)
{
    const uint64_t mb = bytes / (1024ull * 1024ull);
    const double gb = static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << gb << " GB (" << mb << " MB)";
    return ss.str();
}

// ---------------------------------------------------------------------------
// Printing
// ---------------------------------------------------------------------------

constexpr int kLabelWidth = 12;
constexpr size_t kRuleWidth = 60;

inline void PrintField(const char *label, const std::string &value)
{
    std::cout << "  " << std::left << std::setw(kLabelWidth) << label << ": " << value << '\n';
}

inline void PrintQueueFamilies(const std::vector<AdapterQueueFamilyInfo> &families)
{
    std::cout << "\n  Queue Families (" << families.size() << ")\n";

    if (families.empty())
    {
        std::cout << "    (none reported)\n";
        return;
    }

    // Size the capabilities column to the longest entry so the table stays aligned
    size_t capWidth = std::string("Capabilities").size();
    for (const auto &f : families)
        capWidth = std::max(capWidth, ToString(f.flags).size());

    const std::string rule = "    +-----+-" + std::string(capWidth, '-') + "-+--------+";

    std::cout << rule << '\n'
              << "    | " << std::left << std::setw(3) << "Idx"
              << " | " << std::setw(static_cast<int>(capWidth)) << "Capabilities"
              << " | " << std::setw(6) << "Queues" << " |\n"
              << rule << '\n';

    for (size_t i = 0; i < families.size(); ++i)
    {
        std::cout << "    | " << std::right << std::setw(3) << i << " | " << std::left
                  << std::setw(static_cast<int>(capWidth)) << ToString(families[i].flags) << " | " << std::right
                  << std::setw(6) << families[i].count << " |\n";
    }
    std::cout << rule << '\n' << std::left;
}

inline void PrintAdapter(const Adapter &adapter, size_t index, size_t total)
{
    const AdapterDesc desc = adapter.GetInfo();

    std::cout << std::string(kRuleWidth, '=') << '\n'
              << " Adapter " << index << " of " << total << '\n'
              << std::string(kRuleWidth, '=') << '\n';

    PrintField("Name", desc.name);
    PrintField("Type", ToString(desc.type));
    PrintField("Vendor ID", HexId(desc.vendorId));
    PrintField("Device ID", HexId(desc.deviceId));
    PrintField("Memory", FormatMemory(desc.dedicatedVideoMemory));

    PrintQueueFamilies(adapter.QueryQueueFamilyProperties());
    std::cout << '\n';
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int main()
{
    std::unique_ptr<Instance> instance = Instance::Create({.backend = Backend::Vulkan, .debugMode = true});
    if (!instance)
    {
        std::cerr << "Failed to create instance\n";
        return 1;
    }

    const std::vector<Adapter *> adapters = instance->GetAdapters();

    std::cout << "\nSystem Query\n"
              << "Adapters found: " << adapters.size() << "\n\n";

    if (adapters.empty())
    {
        std::cout << "No adapters available.\n";
        return 0;
    }

    for (size_t i = 0; i < adapters.size(); ++i)
        PrintAdapter(*adapters[i], i + 1, adapters.size());

    return 0;
}
