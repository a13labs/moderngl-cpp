#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{
  class palette_loader;

  class palette : public resource
  {
public:
    virtual ~palette() = default;

    palette(const palette&) = delete;
    palette(palette&&) = delete;
    palette& operator=(const palette&) = delete;
    palette& operator=(palette&&) = delete;

    virtual resource::type get_type() const override { return resource::type::palette; }
  };

  using palette_ref = mgl::ref<palette>;

} // namespace mgl::registry