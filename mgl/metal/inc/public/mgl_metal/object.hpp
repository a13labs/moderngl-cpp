#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::metal
{
  class context;
  using context_ref = mgl::ref<context>;

  class object
  {
public:
    virtual ~object() = default;

    virtual void release() = 0;

protected:
    object() = default;
  };

} // namespace mgl::metal
