#include "mgl_window/api/opengl.hpp"
#include "mgl_window/window.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"
namespace mgl::window::api
{
  static render_state s_state_data;

  const static mgl::opengl::blend_factor s_blend_equation_mapping[] = {
    mgl::opengl::blend_factor::ZERO,
    mgl::opengl::blend_factor::ONE,
    mgl::opengl::blend_factor::SRC_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_SRC_COLOR,
    mgl::opengl::blend_factor::DST_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_DST_COLOR,
    mgl::opengl::blend_factor::SRC_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_SRC_ALPHA,
    mgl::opengl::blend_factor::DST_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_DST_ALPHA,
    mgl::opengl::blend_factor::CONSTANT_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_CONSTANT_COLOR,
    mgl::opengl::blend_factor::CONSTANT_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_CONSTANT_ALPHA,
    mgl::opengl::blend_factor::SRC_ALPHA_SATURATE,
    mgl::opengl::blend_factor::SRC1_COLOR,
    mgl::opengl::blend_factor::ONE_MINUS_SRC1_COLOR,
    mgl::opengl::blend_factor::SRC1_ALPHA,
    mgl::opengl::blend_factor::ONE_MINUS_SRC1_ALPHA,
  };

  const static mgl::opengl::blend_equation_mode s_blend_equation_mode[] = {
    mgl::opengl::blend_equation_mode::ADD,
    mgl::opengl::blend_equation_mode::SUBTRACT,
    mgl::opengl::blend_equation_mode::REVERSE_SUBTRACT,
    mgl::opengl::blend_equation_mode::MIN,
    mgl::opengl::blend_equation_mode::MAX,
  };

  const static mgl::opengl::compare_func s_compare_func[] = {
    mgl::opengl::compare_func::NONE,       mgl::opengl::compare_func::NEVER,
    mgl::opengl::compare_func::LESS,       mgl::opengl::compare_func::EQUAL,
    mgl::opengl::compare_func::LESS_EQUAL, mgl::opengl::compare_func::GREATER,
    mgl::opengl::compare_func::NOT_EQUAL,  mgl::opengl::compare_func::GREATER_EQUAL,
    mgl::opengl::compare_func::ALWAYS,
  };

  const static mgl::opengl::render_mode s_render_mode[] = {
    mgl::opengl::render_mode::POINTS,       mgl::opengl::render_mode::LINES,
    mgl::opengl::render_mode::LINE_LOOP,    mgl::opengl::render_mode::LINE_STRIP,
    mgl::opengl::render_mode::TRIANGLES,    mgl::opengl::render_mode::TRIANGLE_STRIP,
    mgl::opengl::render_mode::TRIANGLE_FAN,
  };

  mgl::opengl::compare_func to_api(compare_func func)
  {
    return s_compare_func[static_cast<int>(func)];
  }

  mgl::opengl::blend_factor to_api(blend_factor factor)
  {
    return s_blend_equation_mapping[static_cast<int>(factor)];
  }

  mgl::opengl::blend_equation_mode to_api(blend_equation_mode mode)
  {
    return s_blend_equation_mode[static_cast<int>(mode)];
  }

  mgl::opengl::render_mode to_api(render_mode mode)
  {
    return s_render_mode[static_cast<int>(mode)];
  }

  program_ref create_program(const std::string& vs_source,
                             const std::string& fs_source,
                             const std::string& gs_source,
                             const std::string& tes_source,
                             const std::string& tcs_source)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    mgl::opengl::shaders glsl(vs_source, fs_source, gs_source, tes_source, tcs_source);

    return ctx->program(glsl);
  }

  texture_2d_ref create_texture_2d(
      int width, int height, int components, const mgl::byte_buffer& data, int samples)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    return ctx->texture2d(width, height, components, data);
  }

  void bind_screen_framebuffer()
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->screen().use();
  }

  void enable_scissor()
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->enable_scissor();
  }

  void disable_scissor()
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->disable_scissor();
  }

  void set_scissor(const glm::vec2& position, const glm::vec2& size)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_scissor(position.x, position.y, size.x, size.y);
  }

  void enable_state(int state)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->enable(state);
  }

  void disable_state(int state)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->disable(state);
  }

  void clear(const glm::vec4& color)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->clear(color);
  }

  void set_viewport(const glm::vec2& position, const glm::vec2& size)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_viewport(position.x, position.y, size.x, size.y);
  }

  void clear_samplers(int start, int end)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->clear_samplers(start, end);
  }

  void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_equation(to_api(modeRGB), to_api(modeAlpha));
  }

  void set_blend_func(blend_factor srcRGB,
                      blend_factor dstRGB,
                      blend_factor srcAlpha,
                      blend_factor dstAlpha)
  {
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_func(to_api(srcRGB), to_api(dstRGB), to_api(srcAlpha), to_api(dstAlpha));
  }

  void set_view_matrix(const glm::mat4& matrix)
  {
    s_state_data.view_matrix = matrix;
    if(s_state_data.view_uniform != nullptr)
    {
      s_state_data.view_uniform->set_value(matrix);
    }
  }

  void set_projection_matrix(const glm::mat4& matrix)
  {
    s_state_data.projection_matrix = matrix;
    if(s_state_data.projection_uniform != nullptr)
    {
      s_state_data.projection_uniform->set_value(matrix);
    }
  }

  void set_program_attributes(const mgl::string_list& attributes)
  {
    s_state_data.attributes = attributes;
  }

  void enable_program(const program_ref& prg)
  {
    MGL_CORE_ASSERT(prg != nullptr, "Program is null");
    s_state_data.current_program = prg;
    s_state_data.view_uniform = prg->uniform("view");
    s_state_data.projection_uniform = prg->uniform("projection");
    s_state_data.model_uniform = prg->uniform("model");
    prg->bind();

    // Set the view and projection matrices if the uniforms exist
    if(s_state_data.view_uniform != nullptr)
    {
      s_state_data.view_uniform->set_value(s_state_data.view_matrix);
    }

    if(s_state_data.projection_uniform != nullptr)
    {
      s_state_data.projection_uniform->set_value(s_state_data.projection_matrix);
    }
  }

  void disable_program()
  {
    if(s_state_data.current_program == nullptr)
    {
      return;
    }

    s_state_data.current_program->unbind();
    s_state_data.current_program = nullptr;
    s_state_data.view_uniform = nullptr;
    s_state_data.projection_uniform = nullptr;
    s_state_data.model_uniform = nullptr;
    s_state_data.attributes.clear();
  }

  geom_data::~geom_data()
  {
    if(m_vao != nullptr)
    {
      m_vao->release();
    }
  }

  void geom_data::allocate()
  {
    if(m_vao != nullptr)
    {
      return;
    }
    MGL_PROFILE_FUNCTION("API_DRAW_CALL");
    auto& ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    // We get the uniform for the transform matrix
    auto prg = s_state_data.current_program;
    MGL_CORE_ASSERT(prg != nullptr, "Shader is null");

    mgl::opengl::vertex_buffer_list content = { { m_vb, m_layout, s_state_data.attributes } };
    m_vao = nullptr;

    if(m_ib != nullptr)
    {
      m_vao = ctx->vertex_array(prg, content, m_ib, m_element_size);
    }
    else
    {
      m_vao = ctx->vertex_array(prg, content);
    }
  }

  void geom_data::deallocate()
  {
    if(m_vao == nullptr)
    {
      return;
    }
    m_vao->release();
    m_vao = nullptr;
  }

  void geom_data::draw(const glm::mat4& model_view, int count, int offset, int instance_count)
  {
    MGL_CORE_ASSERT(m_vao != nullptr, "VAO is null");

    // We get the uniform for the transform matrix
    mgl::window::api::uniform_ref transform_uniform = s_state_data.model_uniform;

    if(transform_uniform != nullptr)
    {
      transform_uniform->set_value(model_view);
    }

    m_vao->render((mgl::opengl::render_mode)m_mode, count, offset, instance_count);
  }

  void draw_batch(geom_data& geom, const mgl::list<batch_data>& data)
  {
    for(auto& batch : data)
    {
      geom.draw(batch.model_view, batch.count, batch.offset, batch.instance_count);
    }
  }

}; // namespace mgl::window::api