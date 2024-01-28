#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"

namespace mgl::registry
{
  class sound_loader;

  class sound : public resource
  {
public:
    virtual ~sound() = default;

    sound(const sound&) = delete;
    sound(sound&&) = delete;
    sound& operator=(const sound&) = delete;
    sound& operator=(sound&&) = delete;

    virtual resource::type get_type() const override { return resource::type::sound; }
  };

  using sound_ref = mgl::ref<sound>;

} // namespace mgl::registry
