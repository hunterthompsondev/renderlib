#pragma once

#include <cstdint>

template <typename Tag> struct Handle
{
    uint32_t id = 0; // 0 reserved as invalid

    bool isValid() const
    {
        return id != 0;
    }
    bool operator==(const Handle &) const = default;
};
