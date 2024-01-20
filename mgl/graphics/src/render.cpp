#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
namespace mgl::graphics
{
  static mgl::scope<render> s_instance = nullptr;

  render::render(const mgl::window::api::context_ref& context)
      : m_state_data()
      , m_context(context)
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Render already exists");
    s_instance = mgl::scope<render>(this);
  }

  render::~render()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    s_instance.release();
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
  }

  void render::enable_scissor()
  {
    m_context->enable_scissor();
  }

  void render::disable_scissor()
  {
    m_context->disable_scissor();
  }

  void render::set_scissor(const glm::vec2& position, const glm::vec2& size)
  {
    m_context->set_scissor(position.x, position.y, size.x, size.y);
  }

  void render::enable_state(int state)
  {
    m_context->enable(state);
  }

  void render::disable_state(int state)
  {
    m_context->disable(state);
  }

  void render::clear(const glm::vec4& color)
  {
    m_context->clear(color);
  }

  void render::set_viewport(const glm::vec2& position, const glm::vec2& size)
  {
    m_context->set_viewport(position.x, position.y, size.x, size.y);
  }

  void render::clear_samplers(int start, int end)
  {
    m_context->clear_samplers(start, end);
  }

  void render::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    m_context->set_blend_equation(to_api(modeRGB), to_api(modeAlpha));
  }

  void render::set_blend_func(blend_factor srcRGB,
                              blend_factor dstRGB,
                              blend_factor srcAlpha,
                              blend_factor dstAlpha)
  {
    m_context->set_blend_func(to_api(srcRGB), to_api(dstRGB), to_api(srcAlpha), to_api(dstAlpha));
  }

  void render::draw(const vertex_buffer_ref& vb,
                    const index_buffer_ref& ib,
                    render_mode mode,
                    const glm::mat4& model_view,
                    size_t count,
                    size_t offset,
                    size_t instance_count)
  {
    shader_ref shader = m_state_data.current_shader;
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");

    mgl::window::api::program_ref program = shader->native();
    MGL_CORE_ASSERT(program != nullptr, "Program is null");

    // We get the uniform for the transform matrix
    mgl::window::api::uniform_ref transform_uniform = m_state_data.model_uniform;

    if(transform_uniform != nullptr)
    {
      transform_uniform->set_value(model_view);
    }

    mgl::opengl::vertex_buffer_list m_content = {
      { vb->native(), vb->layout(), shader->attributes() }
    };

    mgl::opengl::vertex_array_ref vao = nullptr;

    if(ib != nullptr)
    {
      vao = m_context->vertex_array(program, m_content, ib->native(), ib->element_size());
    }
    else
    {
      vao = m_context->vertex_array(program, m_content);
    }

    vao->render((mgl::opengl::render_mode)mode, count, offset, instance_count);
    vao->release();
  }

  void render::draw_batch(const batch_ref& batch)
  {
    if(batch->get().size() == 0)
    {
      return;
    }

    shader_ref shader = m_state_data.current_shader;
    MGL_CORE_ASSERT(shader != nullptr, "Shader is null");

    mgl::window::api::program_ref program = shader->native();
    MGL_CORE_ASSERT(program != nullptr, "Program is null");

    // We get the uniform for the transform matrix
    mgl::window::api::uniform_ref transform_uniform = m_state_data.model_uniform;

    // We create a vertex array from the first batch data since all the batches have the same
    // vertex buffer data, index buffer data and draw mode
    mgl::opengl::vertex_buffer_list m_content = {
      { batch->vertex_buffer()->native(), batch->vertex_buffer()->layout(), shader->attributes() }
    };

    mgl::opengl::vertex_array_ref vao = nullptr;

    if(batch->index_buffer() != nullptr)
    {
      vao = m_context->vertex_array(program,
                                    m_content,
                                    batch->index_buffer()->native(),
                                    batch->index_buffer()->element_size());
    }
    else
    {
      vao = m_context->vertex_array(program, m_content);
    }

    for(auto& draw_call : batch->get())
    {
      if(transform_uniform != nullptr)
      {
        transform_uniform->set_value(draw_call.model_view);
      }

      vao->render((mgl::opengl::render_mode)batch->mode(),
                  draw_call.count,
                  draw_call.offset,
                  draw_call.instance_count);
    }
    vao->release();
  }

} // namespace mgl::graphics