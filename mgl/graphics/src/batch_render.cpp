#include "mgl_graphics/batch_render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  void batch_render::commit()
  {
    if(m_batch_data.size() == 0)
    {
      return;
    }

    auto& render = mgl::graphics::current_render();

    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    shader_ref shader = render.current_state().current_shader;
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");

    mgl::window::api::program_ref program = shader->native();
    MGL_CORE_ASSERT(program != nullptr, "Program is null");

    // We get the uniform for the transform matrix
    mgl::window::api::uniform_ref transform_uniform = render.current_state().model_uniform;

    // We create a vertex array from the first batch data since all the batches have the same
    // vertex buffer data, index buffer data and draw mode
    mgl::opengl::vertex_buffer_list m_content = { { m_batch_data[0].vertex_buffer->native(),
                                                    m_batch_data[0].vertex_buffer->layout(),
                                                    shader->attributes() } };

    index_buffer_ref index_buffer = m_batch_data[0].index_buffer;
    mgl::opengl::vertex_array_ref vao = nullptr;

    if(index_buffer != nullptr)
    {
      vao = ctx->vertex_array(program, m_content, index_buffer->native());
    }
    else
    {
      vao = ctx->vertex_array(program, m_content);
    }

    for(auto& draw_call : m_batch_data)
    {
      if(transform_uniform != nullptr)
      {
        transform_uniform->set_value(draw_call.transform);
      }

      vao->render((mgl::opengl::render_mode)draw_call.mode);
    }
    vao->release();

    m_batch_data.clear();
  }

  void batch_render::push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, draw_mode m)
  {
    if(m_batch_data.size() == 0)
    {
      m_batch_data.push_back({ vb, ib, m });
      return;
    }

    auto& last = m_batch_data.back();
    if(last.vertex_buffer == vb && last.index_buffer == ib && last.mode == m)
    {
      m_batch_data.push_back({ vb, ib, m });
    }

    commit();
  }

} // namespace mgl::graphics