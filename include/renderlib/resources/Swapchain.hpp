#pragma once

#include <cstdint>

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>

struct SwapchainTag
{
};
using SwapchainHandle = Handle<SwapchainTag>;

struct NativeWindowHandle
{
    void *display = nullptr;
    void *surface = nullptr;
};

struct SwapchainCreateInfo
{
    NativeWindowHandle window;
    uint32_t width;
    uint32_t height;

    Format format = Format::BGRA8Unorm;
    PresentMode presentMode = PresentMode::Fifo;
    uint32_t bufferCount = 2;
};
