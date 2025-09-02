#pragma once

#include "api/buffers.hpp"
#include "api/pipeline.hpp"
#include "api/textures.hpp"

#include "enums.hpp"

namespace mgl::platform {

  struct draw_call
  {
    api::texture_ref tex;
    size_t element_count;
    size_t index_offset;
    glm::vec4 clip_rect;
  };

  struct render_batch
  {
    int32_t count;
    render_mode mode;
    api::vertex_buffer_ref vertex_buffer;
    api::index_buffer_ref index_buffer;
    api::buffer_ref uniform_buffer;
    mgl::list<draw_call> draw_calls;

    render_batch(const api::vertex_buffer_ref& vb = nullptr,
                 const api::index_buffer_ref& ib = nullptr,
                 const api::buffer_ref& ub = nullptr,
                 render_mode m = render_mode::TRIANGLES)
        : count(0)
        , mode(m)
        , vertex_buffer(vb)
        , index_buffer(ib)
        , uniform_buffer(ub)
    { }

    void clear()
    {
      draw_calls.clear();
      count = 0;
    }

    void add_draw_call(api::texture_ref tex,
                       size_t element_count,
                       size_t index_offset,
                       const glm::vec4& clip_rect = glm::vec4(0))
    {
      draw_calls.push_back({ tex, element_count, index_offset, clip_rect });
      count++;
    }

    ~render_batch() { clear(); }
  };

  using render_batch_ref = mgl::ref<render_batch>;

  struct render_state
  {
    render_state()
        : current_pipeline(nullptr)
        , view_matrix(1.0f)
        , projection_matrix(1.0f)
    { }

    // The current shader, view and projection matrices are stored in the render, as they are
    // used by multiple commands
    api::pipeline_ref current_pipeline;

    // Transform matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
  };


}