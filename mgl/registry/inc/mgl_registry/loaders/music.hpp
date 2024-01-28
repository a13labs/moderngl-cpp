#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/sound.hpp"

namespace mgl::registry::loaders
{
  class music_loader;
  using music_loader_ref = mgl::scope<music_loader>;

  struct music_loader_options : public loader_options
  {
    virtual ~music_loader_options() = default;
  };

  class music_loader : public loader
  {
public:
    music_loader() = default;
    virtual ~music_loader() = default;

    virtual resource::type get_type() const override { return resource::type::music; }
  };
} // namespace mgl::registry::loaders