#include "mgl_engine/renderer.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_engine/application.hpp"
#include "mgl_engine/commands/common.hpp"
#include "mgl_engine/commands/draw.hpp"
#include "mgl_engine/commands/state.hpp"
#include "mgl_engine/commands/texture.hpp"
namespace mgl::engine
{
  void renderer::flush()
  {
    for(auto& command : m_render_queue)
    {
      command->execute();
    }

    if(m_state_data.current_batch != nullptr)
    {
      m_state_data.current_batch->commit();
      m_state_data.current_batch = nullptr;
    }

    m_render_queue.clear();
  }

  void renderer::begin()
  {
    m_render_queue.clear();
  }

  void renderer::end()
  {
    flush();
  }

  void renderer::enable_state(state state)
  {
    submit(mgl::create_ref<mgl::engine::enable_state>(state));
  }

  void renderer::disable_state(state state)
  {
    submit(mgl::create_ref<mgl::engine::disable_state>(state));
  }

  void renderer::enable_texture(uint32_t slot, const mgl::window::texture_ref& t)
  {
    submit(mgl::create_ref<mgl::engine::enable_texture>(slot, t));
  }

  void renderer::disable_texture(uint32_t slot)
  {
    submit(mgl::create_ref<mgl::engine::disable_texture>(slot));
  }

  void renderer::clear(const glm::vec4& color)
  {
    submit(mgl::create_ref<mgl::engine::clear_command>(color));
  }

  void renderer::set_view(const glm::mat4& view)
  {
    submit(mgl::create_ref<mgl::engine::set_view_command>(view));
  }

  void renderer::set_projection(const glm::mat4& projection)
  {
    submit(mgl::create_ref<mgl::engine::set_projection_command>(projection));
  }

  void renderer::set_blend_func(blend_func src, blend_func dst) { }

  void renderer::set_color_mask(bool r, bool g, bool b, bool a) { }

  void renderer::set_depth_mask(bool mask) { }

  void renderer::set_depth_func(depth_func func) { }

  void renderer::set_stencil_mask(uint32_t mask) { }

  void renderer::set_stencil_func(stencil_func func, int32_t ref, uint32_t mask) { }

  void renderer::set_stencil_op(stencil_op sfail, stencil_op dpfail, stencil_op dppass) { }

  void renderer::set_cull_face(cull_face face) { }

  void renderer::set_polygon_offset(float factor, float units) { }

  void renderer::draw(const vertex_buffer_ref& vertex_array,
                      index_buffer_ref index_buffer,
                      draw_mode mode)
  {
    submit(mgl::create_ref<mgl::engine::draw_command>(vertex_array, index_buffer, mode));
  }

  void renderer::enable_material(material_ref material) { }

  void renderer::disable_material() { }

  void
  batch_list::push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, renderer::draw_mode m)
  {
    if(m_batch_data.size() == 0)
    {
      m_batch_data.push_back({ vb, ib, m });
      return;
    }

    auto& last = m_batch_data.back();
    if(last.vertex_buffer == vb && last.index_buffer == ib && last.mode == m)
    {
      commit();
    }

    m_batch_data.push_back({ vb, ib, m });
  }

  void batch_list::commit()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    auto ctx = renderer->context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    if(m_batch_data.size() == 0)
    {
      return;
    }

    shader_ref shader = renderer->current_state().current_shader;
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");
    mgl::window::program_ref program = shader->program();
    MGL_CORE_ASSERT(program != nullptr, "Program is null");

    // We get the uniform for the transform matrix
    mgl::window::uniform_ref transform_uniform = program->uniform("model");

    // We create a vertex array from the first batch data since all the batches have the same
    // vertex buffer data, index buffer data and draw mode
    mgl::opengl::vertex_buffer_list m_content = { { m_batch_data[0].vertex_buffer->buffer(),
                                                    m_batch_data[0].vertex_buffer->layout(),
                                                    shader->attributes() } };
    mgl::window::vertex_array_ref vao =
        ctx->vertex_array(program, m_content, m_batch_data[0].index_buffer->buffer());

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

} // namespace mgl::engine