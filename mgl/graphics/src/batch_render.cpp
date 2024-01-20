#include "mgl_graphics/batch_render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  void batch_render::commit()
  {
    auto& render = render::current_render();
    render.draw_batch(m_vb, m_ib, m_mode, m_batch_data);
    m_batch_data.clear();
  }

  void batch_render::push(glm::mat4 t, size_t count, size_t offset)
  {
    m_batch_data.push_back({ t, count, offset });
  }

  void batch_render::reset(const vertex_buffer_ref& vb, const index_buffer_ref& ib, render_mode m)
  {
    m_batch_data.clear();
    m_mode = m;
    m_vb = vb;
    m_ib = ib;
  }

} // namespace mgl::graphics