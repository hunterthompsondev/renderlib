#pragma once

#include <cstdint>
#include <unordered_map>

#include "Handles.hpp"

template <typename Tag> class HandleGenerator
{
  public:
    Handle<Tag> next()
    {
        return Handle<Tag>{++m_counter};
    }

  private:
    uint32_t m_counter = 0;
};

template <typename T, typename Tag> class ResourcePool
{
  public:
    Handle<Tag> Create(T &&resource)
    {
        Handle<Tag> handle = m_generator.next();
        m_resources.emplace(handle.id, std::move(resource));
        return handle;
    }

    void Destroy(Handle<Tag> handle)
    {
        m_resources.erase(handle.id);
    }

    T *Get(Handle<Tag> handle)
    {
        auto it = m_resources.find(handle.id);
        return it != m_resources.end() ? &it->second : nullptr;
    }

    const T *Get(Handle<Tag> handle) const
    {
        auto it = m_resources.find(handle.id);
        return it != m_resources.end() ? &it->second : nullptr;
    }

  private:
    HandleGenerator<Tag> m_generator;
    std::unordered_map<uint32_t, T> m_resources;
};
