#include "mgl_graphics/batch_render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  void batch_render::commit()
  {
    auto& render = render::current_render();
    render.draw_list(m_batch_data);
    m_batch_data.clear();
  }

  void batch_render::push(const vertex_buffer_ref& vb,
                          const index_buffer_ref& ib,
                          render_mode m,
                          glm::mat4 t,
                          size_t count,
                          size_t offset)
  {
    if(m_batch_data.size() == 0)
    {
      m_batch_data.push_back({ vb, ib, m, t, count, offset });
      return;
    }

    auto& last = m_batch_data.back();
    if(last.vertex_buffer == vb && last.index_buffer == ib)
    {
      m_batch_data.push_back({ vb, ib, m, t, count, offset });
      return;
    }

    commit();
  }

} // namespace mgl::graphics