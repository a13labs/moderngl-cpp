#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/sound.hpp"

namespace mgl::registry::loaders
{
  class sound_loader;
  using sound_loader_ref = mgl::scope<sound_loader>;

  struct sound_loader_options : public loader_options
  {
    virtual ~sound_loader_options() = default;
  };

  class sound_loader : public loader
  {
public:
    sound_loader() = default;
    virtual ~sound_loader() = default;

    virtual resource::type get_type() const override { return resource::type::sound; }
  };
} // namespace mgl::registry::loaders
