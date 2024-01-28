#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/palette.hpp"

namespace mgl::registry::loaders
{
  class palette_loader;
  using palette_loader_ref = mgl::scope<palette_loader>;

  struct palette_loader_options : public loader_options
  {
    virtual ~palette_loader_options() = default;
  };

  class palette_loader : public loader
  {
public:
    palette_loader() = default;
    virtual ~palette_loader() = default;

    virtual resource::type get_type() const override { return resource::type::palette; }
  };
} // namespace mgl::registry::loaders