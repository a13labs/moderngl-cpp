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

  struct render_data
  {
    render_data()
        : vertex_buffer(nullptr)
        , index_buffer(nullptr)
        , mode(render_mode::TRIANGLES)
        , model_view(1.0f)
        , count(0)
        , offset(0)
    { }

    render_data(const vertex_buffer_ref& vb,
                const index_buffer_ref& ib,
                render_mode m,
                glm::mat4 t,
                size_t count,
                size_t offset)
        : vertex_buffer(vb)
        , index_buffer(ib)
        , mode(m)
        , model_view(t)
        , count(count)
        , offset(offset)
    { }

    render_data(const render_data& other)
        : vertex_buffer(other.vertex_buffer)
        , index_buffer(other.index_buffer)
        , mode(other.mode)
        , model_view(other.model_view)
        , count(other.count)
        , offset(other.offset)
    { }

    bool operator==(const render_data& other) const
    {
      return vertex_buffer == other.vertex_buffer && index_buffer == other.index_buffer &&
             mode == other.mode && model_view == other.model_view && count == other.count;
    }

    bool operator!=(const render_data& other) const { return !(*this == other); }

    vertex_buffer_ref vertex_buffer;
    index_buffer_ref index_buffer;
    render_mode mode;
    glm::mat4 model_view;
    size_t count;
    size_t offset;
  };

  class batch_render
  {

public:
    batch_render()
        : m_batch_data()
    { }

    ~batch_render() { m_batch_data.clear(); }

    void push(const vertex_buffer_ref& vb,
              const index_buffer_ref& ib = nullptr,
              render_mode m = render_mode::TRIANGLES,
              glm::mat4 t = glm::mat4(1.0f),
              size_t count = 0,
              size_t offset = 0);

    void clear() { m_batch_data.clear(); }

    void commit();

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<render_data> m_batch_data;
  };
} // namespace mgl::graphics