#include "mgl_graphics/batch.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  void batch::push(glm::mat4 t, size_t count, size_t offset, size_t instance_count)
  {
    m_batch_data.push_back({ t, count, offset, instance_count });
  }

  void batch::reset(const vertex_buffer_ref& vb, const index_buffer_ref& ib, render_mode m)
  {
    m_batch_data.clear();
    m_mode = m;
    m_vb = vb;
    m_ib = ib;
  }

} // namespace mgl::graphics