#include <iostream>
#include <memory>
#include <vector>

#include <renderlib/rhi/RHI.hpp>

using namespace Renderlib::Type;
using namespace Renderlib::Object;
using namespace Renderlib::Desc;

int main()
{
    std::unique_ptr<Instance> instance = Instance::Create({.backend = Backend::Vulkan, .debugMode = true});
    Adapter *adapter = std::move(instance->GetAdapters()[0]); // Use first adapter for simplicity
    std::unique_ptr<Device> device = adapter->CreateDevice({
        .queues =
            {
                (DeviceQueueDesc){.familyIndex = 0, .priorites = {0.5f}}, // Graphics, Compute, Transfer
            },
    });
    std::vector<QueueFamily *> queueFamilies = device->GetQueueFamilies();
    Queue *gfxQueue = queueFamilies[0]->GetQueues()[0];

    return 0;
}
