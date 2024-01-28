#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{
  class music_loader;

  class music : public resource
  {
public:
    virtual ~music() = default;

    music(const music&) = delete;
    music(music&&) = delete;
    music& operator=(const music&) = delete;
    music& operator=(music&&) = delete;

    virtual resource::type get_type() const override { return resource::type::music; }
  };

  using music_ref = mgl::ref<music>;

} // namespace mgl::registry