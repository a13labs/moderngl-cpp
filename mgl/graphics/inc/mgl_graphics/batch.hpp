#pragma once
#include "enums.hpp"
#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_platform/api/batch_data.hpp"
#include "mgl_platform/api/buffers.hpp"

#include <glm/glm.hpp>

namespace mgl::graphics
{

  class batch
  {
public:
    batch(const mgl::platform::api::vertex_buffer_ref& vb = nullptr,
          const mgl::platform::api::index_buffer_ref& ib = nullptr,
          render_mode m = render_mode::TRIANGLES)
        : m_batch_data()
        , m_mode(m)
        , m_vb(vb)
        , m_ib(ib)
    { }

    ~batch() { m_batch_data.clear(); }

    void push(glm::mat4 t = glm::mat4(1.0f),
              size_t count = 0,
              size_t offset = 0,
              size_t instance_count = 1);

    void reset(const mgl::platform::api::vertex_buffer_ref& vb,
               const mgl::platform::api::index_buffer_ref& ib = nullptr,
               render_mode m = render_mode::TRIANGLES);

    const mgl::platform::api::vertex_buffer_ref& vertex_buffer() const { return m_vb; }

    const mgl::platform::api::index_buffer_ref& index_buffer() const { return m_ib; }

    render_mode mode() const { return m_mode; }

    const mgl::list<mgl::platform::api::batch_data>& get() const { return m_batch_data; }

    bool empty() const { return m_batch_data.empty(); }

private:
    mgl::list<mgl::platform::api::batch_data> m_batch_data;
    render_mode m_mode;
    mgl::platform::api::vertex_buffer_ref m_vb;
    mgl::platform::api::index_buffer_ref m_ib;
  };

  using batch_ref = mgl::ref<batch>;
} // namespace mgl::graphics