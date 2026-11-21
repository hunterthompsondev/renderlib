#include <iostream>
#include <memory>

#include <renderlib/rhi/RHI.hpp>

using namespace Renderlib::Type;
using namespace Renderlib::Object;

int main()
{
    std::unique_ptr<Instance> instance = Instance::Create({.backend = Backend::Vulkan, .debugMode = true});

    return 0;
}
