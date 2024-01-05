#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::registry
{

  class resource;
  class registry;

  using resource_ref = mgl::ref<resource>;

  class resource
  {
    friend class registry;

public:
    enum class type
    {
      image,
      shader,
      font,
      sound,
      music,
      text,
      json,
      binary,
      custom
    };

public:
    resource() = default;
    resource(const resource&) = delete;
    resource& operator=(const resource&) = delete;
    resource(resource&&) = delete;
    resource& operator=(resource&&) = delete;

    virtual ~resource() = default;

    virtual type get_type() const = 0;
  };
} // namespace mgl::registry