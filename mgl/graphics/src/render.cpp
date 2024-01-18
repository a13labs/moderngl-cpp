#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/material.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
namespace mgl::graphics
{
  render* s_instance = nullptr;

  render::render(const mgl::window::api::context_ref& context)
      : m_render_queue()
      , m_state_data()
      , m_context(context)
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Render already exists");
    s_instance = this;
  }

  render::~render()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    s_instance = nullptr;
  }

  void render::flush()
  {
    m_render_queue.execute();

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

  void render::enable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::enable_state>(state));
  }

  void render::disable_state(int state)
  {
    submit(mgl::create_ref<mgl::graphics::disable_state>(state));
  }

  void render::enable_texture(uint32_t slot, const texture_ref& tex)
  {
    submit(mgl::create_ref<mgl::graphics::enable_texture>(slot, tex));
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

  void render::set_blend_func(blend_factor srcRGB,
                              blend_factor dstRGB,
                              blend_factor srcAlpha,
                              blend_factor dstAlpha)
  {
    submit(
        mgl::create_ref<mgl::graphics::set_blend_func_command>(srcRGB, dstRGB, srcAlpha, dstAlpha));
  }

  void render::clear_samplers(int start, int end)
  {
    submit(mgl::create_ref<mgl::graphics::clear_samplers_command>(start, end));
  }

  void render::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    submit(mgl::create_ref<mgl::graphics::set_blend_equation_command>(modeRGB, modeAlpha));
  }

  void render::draw(const vertex_buffer_ref& vertex_array,
                    const index_buffer_ref& index_buffer,
                    draw_mode mode)
  {
    submit(mgl::create_ref<mgl::graphics::draw_command>(vertex_array, index_buffer, mode));
  }

  void render::enable_shader(shader_ref shader)
  {
    submit(mgl::create_ref<mgl::graphics::enable_shader>(shader));
  }

  void render::disable_shader()
  {
    submit(mgl::create_ref<mgl::graphics::disable_shader>());
  }

  void render::enable_material(material_ref material)
  {
    submit(mgl::create_ref<mgl::graphics::enable_material>(material));
  }

  void render::disable_material()
  {
    submit(mgl::create_ref<mgl::graphics::disable_material>());
  }

  void render::push_render_script(const render_script_ref& script)
  {
    submit(script);
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

  void render_script::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    if(m_render_target != nullptr)
    {
      m_render_target->use();
    }
    else
    {
      ctx->screen()->use();
    }

    for(auto& command : m_commands)
    {
      command->execute();
    }

    ctx->screen()->use();
  }

  render& render::current_render()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    return *s_instance;
  }

  void render::release()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    m_shader_manager.clear();
    m_render_queue.clear();
  }
} // namespace mgl::graphics