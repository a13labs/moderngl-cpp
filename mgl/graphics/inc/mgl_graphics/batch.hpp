#pragma once
#include "enums.hpp"
#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_platform/api/buffers.hpp"
#include "mgl_platform/api/vertex_array.hpp"

#include <glm/glm.hpp>

namespace mgl::graphics
{

  struct batch
  {
    int32_t count;
    platform::api::render_mode render_mode;
    platform::api::vertex_buffer_ref vertex_buffer;
    platform::api::index_buffer_ref index_buffer;
    platform::api::buffer_ref uniform_buffer;

    batch(const mgl::platform::api::vertex_buffer_ref& vb = nullptr,
          const mgl::platform::api::index_buffer_ref& ib = nullptr,
          const mgl::platform::api::buffer_ref& ub = nullptr,
          platform::api::render_mode m = render_mode::TRIANGLES)
        : render_mode(m)
        , vertex_buffer(vb)
        , index_buffer(ib)
        , uniform_buffer(ub)
    { }

    ~batch() { }
  };
  using batch_ref = mgl::ref<batch>;
} // namespace mgl::graphics