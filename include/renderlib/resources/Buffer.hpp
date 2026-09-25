#pragma once

#include <cstddef>

#include <renderlib/Flags.hpp>
#include <renderlib/Handles.hpp>

struct BufferTag
{
};
using BufferHandle = Handle<BufferTag>;

struct BufferCreateInfo
{
    BufferUsageFlags bufferUsage;
    const void *data;
    size_t sizeInBytes;
};
