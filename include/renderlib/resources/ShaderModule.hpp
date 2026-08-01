#pragma once

#include <cstdint>
#include <span>
#include <string>

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>

struct ShaderModuleTag
{
};
using ShaderModuleHandle = Handle<ShaderModuleTag>;

struct ShaderModuleCreateInfo
{
    ShaderStage stage;
    std::string entryPoint;
    std::span<const uint8_t> bytecode;
};
