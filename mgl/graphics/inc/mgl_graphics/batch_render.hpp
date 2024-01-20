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
        : model_view(1.0f)
        , count(0)
        , offset(0)
    { }

    render_data(glm::mat4 t, size_t count, size_t offset)
        : model_view(t)
        , count(count)
        , offset(offset)
    { }

    render_data(const render_data& other)
        : model_view(other.model_view)
        , count(other.count)
        , offset(other.offset)
    { }

    glm::mat4 model_view;
    size_t count;
    size_t offset;
  };

  class batch_render
  {

public:
    batch_render(const vertex_buffer_ref& vb = nullptr,
                 const index_buffer_ref& ib = nullptr,
                 render_mode m = render_mode::TRIANGLES)
        : m_batch_data()
        , m_mode(m)
        , m_vb(vb)
        , m_ib(ib)
    { }

    ~batch_render() { m_batch_data.clear(); }

    void push(glm::mat4 t = glm::mat4(1.0f), size_t count = 0, size_t offset = 0);

    void reset(const vertex_buffer_ref& vb,
               const index_buffer_ref& ib = nullptr,
               render_mode m = render_mode::TRIANGLES);

    const vertex_buffer_ref& vertex_buffer() const { return m_vb; }

    const index_buffer_ref& index_buffer() const { return m_ib; }

    render_mode mode() const { return m_mode; }

    const mgl::list<render_data>& data() const { return m_batch_data; }

    void commit();

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<render_data> m_batch_data;
    render_mode m_mode;
    vertex_buffer_ref m_vb;
    index_buffer_ref m_ib;
  };
} // namespace mgl::graphics