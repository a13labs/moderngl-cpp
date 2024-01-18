#pragma once
#include "buffers/index.hpp"
#include "buffers/vertex.hpp"
#include "enums.hpp"

#include "mgl_window/api.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::graphics
{
  class batch_render;

  using batch_render_ref = mgl::ref<batch_render>;

  class batch_render
  {
    struct data
    {
      data()
          : vertex_buffer(nullptr)
          , index_buffer(nullptr)
          , mode(draw_mode::TRIANGLES)
      { }

      data(const vertex_buffer_ref& vb, const index_buffer_ref& ib, draw_mode m)
          : vertex_buffer(vb)
          , index_buffer(ib)
          , mode(m)
      { }

      data(const data& other)
          : vertex_buffer(other.vertex_buffer)
          , index_buffer(other.index_buffer)
          , mode(other.mode)
      { }

      bool operator==(const data& other) const
      {
        return vertex_buffer == other.vertex_buffer && index_buffer == other.index_buffer &&
               mode == other.mode;
      }

      bool operator!=(const data& other) const { return !(*this == other); }

      vertex_buffer_ref vertex_buffer;
      index_buffer_ref index_buffer;
      draw_mode mode;
      glm::mat4 transform;
    };

public:
    batch_render()
        : m_batch_data()
    { }

    ~batch_render() { m_batch_data.clear(); }

    void push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, draw_mode m);

    void clear() { m_batch_data.clear(); }

    void commit();

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<data> m_batch_data;
  };
} // namespace mgl::graphics