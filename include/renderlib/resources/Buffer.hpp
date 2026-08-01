#pragma once

#include <cstddef>

#include <renderlib/Enums.hpp>
#include <renderlib/Handles.hpp>

struct BufferTag
{
};
using BufferHandle = Handle<BufferTag>;

struct BufferCreateInfo
{
    BufferType bufferType;
    BufferUsage bufferUsage;
    const void *data;
    size_t sizeInBytes;
};
