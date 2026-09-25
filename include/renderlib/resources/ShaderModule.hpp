#pragma once

#include <cstdint>
#include <span>
#include <string>

#include <renderlib/Flags.hpp>
#include <renderlib/Handles.hpp>

struct ShaderModuleTag
{
};
using ShaderModuleHandle = Handle<ShaderModuleTag>;

struct ShaderModuleCreateInfo
{
    ShaderStageFlags stage;
    std::string entryPoint;
    std::span<const uint8_t> bytecode;
};
