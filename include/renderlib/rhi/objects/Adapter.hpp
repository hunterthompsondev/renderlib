#pragma once

#include <renderlib/rhi/descriptors/Descriptors.hpp>

namespace Renderlib::Object
{
class Adapter
{
  public:
    Adapter(const Desc::AdapterDesc &desc);
    virtual ~Adapter() = default;

    Adapter(const Adapter &) = delete;
    Adapter &operator=(const Adapter &) = delete;
    Adapter(Adapter &) = delete;
    Adapter &operator=(Adapter &) = delete;

    Desc::AdapterDesc GetInfo() const;

  protected:
    Desc::AdapterDesc m_desc;
};

} // namespace Renderlib::Object
