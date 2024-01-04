#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/application.hpp"
#include "mgl_graphics/commands/common.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/material.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
namespace mgl::graphics
{
  void render::flush()
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

  void render::begin()
  {
    m_render_queue.clear();
  }

  void render::end()
  {
    flush();
  }

  void render::enable_state(state state)
  {
    submit(mgl::create_ref<mgl::graphics::enable_state>(state));
  }

  void render::disable_state(state state)
  {
    submit(mgl::create_ref<mgl::graphics::disable_state>(state));
  }

  void render::enable_texture(uint32_t slot, const mgl::window::api::texture_ref& t)
  {
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, t));
  }

  void render::disable_texture(uint32_t slot)
  {
    submit(mgl::create_ref<mgl::graphics::disable_texture>(slot));
  }

  void render::clear(const glm::vec4& color)
  {
    submit(mgl::create_ref<mgl::graphics::clear_command>(color));
  }

  void render::set_view(const glm::mat4& view)
  {
    submit(mgl::create_ref<mgl::graphics::set_view_command>(view));
  }

  void render::set_projection(const glm::mat4& projection)
  {
    submit(mgl::create_ref<mgl::graphics::set_projection_command>(projection));
  }

  void render::set_blend_func(blend_func src, blend_func dst) { }

  void render::set_color_mask(bool r, bool g, bool b, bool a) { }

  void render::set_depth_mask(bool mask) { }

  void render::set_depth_func(depth_func func) { }

  void render::set_stencil_mask(uint32_t mask) { }

  void render::set_stencil_func(stencil_func func, int32_t ref, uint32_t mask) { }

  void render::set_stencil_op(stencil_op sfail, stencil_op dpfail, stencil_op dppass) { }

  void render::set_cull_face(cull_face face) { }

  void render::set_polygon_offset(float factor, float units) { }

  void
  render::draw(const vertex_buffer_ref& vertex_array, index_buffer_ref index_buffer, draw_mode mode)
  {
    submit(mgl::create_ref<mgl::graphics::draw_command>(vertex_array, index_buffer, mode));
  }

  void render::enable_material(material_ref material)
  {
    submit(mgl::create_ref<mgl::graphics::enable_material>(material));
  }

  void render::disable_material()
  {
    submit(mgl::create_ref<mgl::graphics::disable_material>());
  }

  void
  batch_render::push(const vertex_buffer_ref& vb, const index_buffer_ref& ib, render::draw_mode m)
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

  void batch_render::commit()
  {
    if(m_batch_data.size() == 0)
    {
      return;
    }

    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");

    auto ctx = render->context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    shader_ref shader = render->current_state().current_shader;
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");

    mgl::window::api::program_ref program = shader->program();
    MGL_CORE_ASSERT(program != nullptr, "Program is null");

    // We get the uniform for the transform matrix
    mgl::window::api::uniform_ref transform_uniform = render->current_state().model_uniform;

    // We create a vertex array from the first batch data since all the batches have the same
    // vertex buffer data, index buffer data and draw mode
    mgl::opengl::vertex_buffer_list m_content = { { m_batch_data[0].vertex_buffer->buffer(),
                                                    m_batch_data[0].vertex_buffer->layout(),
                                                    shader->attributes() } };

    index_buffer_ref index_buffer = m_batch_data[0].index_buffer;
    mgl::opengl::vertex_array_ref vao = nullptr;

    if(index_buffer != nullptr)
    {
      vao = ctx->vertex_array(program, m_content, index_buffer->buffer());
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

} // namespace mgl::graphics