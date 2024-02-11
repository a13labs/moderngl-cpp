#include "mgl_platform/api/backends/opengl.hpp"
#include "mgl_platform/window.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"
#include "mgl_registry/resources/font.hpp"
#include "mgl_registry/resources/image.hpp"

#include "mgl_platform_internal.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace mgl::platform::api::backends
{
  // Text rendering stuff

  // This is the data that is shared between all the draw calls
  static render_state s_state_data;

  const mgl::float32_buffer quad_verts = {
    // positions        // texture Coords
    -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
    0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
    0.5f,  0.5f,  1.0f, 1.0f, // top right
    -0.5f, 0.5f,  0.0f, 1.0f // top left
  };

  const mgl::uint32_buffer quad_indices = {
    0, 1, 2, // first triangle
    2, 3, 0 // second triangle
  };

  static const char* quad_layout = "2f 2f";

  static vertex_array* s_quad = nullptr;

  void opengl_api::init()
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    auto vb = ctx->buffer(quad_verts);
    auto ib = ctx->buffer(quad_indices);

    s_quad = new vertex_array(vb, quad_layout, ib, 4, mgl::opengl::render_mode::TRIANGLES);
    // s_quad->allocate();
  }

  void opengl_api::shutdown()
  {
    MGL_PROFILE_FUNCTION("API_SHUTDOWN");
    // s_quad->deallocate();
    delete s_quad;
  }

  void opengl_api::bind_screen_framebuffer()
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->screen().use();
  }

  void opengl_api::enable_scissor()
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->enable_scissor();
  }

  void opengl_api::disable_scissor()
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->disable_scissor();
  }

  void opengl_api::set_scissor(const glm::vec2& position, const glm::vec2& size)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_scissor(position.x, position.y, size.x, size.y);
  }

  void opengl_api::enable_state(int state)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->enable(state);
  }

  void opengl_api::disable_state(int state)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->disable(state);
  }

  void opengl_api::clear(const glm::vec4& color)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->clear(color);
  }

  void opengl_api::set_viewport(const glm::vec2& position, const glm::vec2& size)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_viewport(position.x, position.y, size.x, size.y);
  }

  void opengl_api::clear_samplers(int start, int end)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->clear_samplers(start, end);
  }

  void opengl_api::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_equation(internal::to_api(modeRGB), internal::to_api(modeAlpha));
  }

  void opengl_api::set_blend_func(blend_factor srcRGB,
                                  blend_factor dstRGB,
                                  blend_factor srcAlpha,
                                  blend_factor dstAlpha)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->set_blend_func(internal::to_api(srcRGB),
                        internal::to_api(dstRGB),
                        internal::to_api(srcAlpha),
                        internal::to_api(dstAlpha));
  }

  void opengl_api::set_view_matrix(const glm::mat4& matrix)
  {
    s_state_data.view_matrix = matrix;
    if(s_state_data.view_uniform != nullptr)
    {
      s_state_data.view_uniform->set_value(matrix);
    }
  }

  void opengl_api::set_projection_matrix(const glm::mat4& matrix)
  {
    s_state_data.projection_matrix = matrix;
    if(s_state_data.projection_uniform != nullptr)
    {
      s_state_data.projection_uniform->set_value(matrix);
    }
  }

  void opengl_api::enable_program(const program_ref& prg)
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

  void opengl_api::set_program_attributes(const mgl::string_list& attributes)
  {
    s_state_data.attributes = attributes;
  }

  void opengl_api::set_program_uniform(const std::string& uniform, bool value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, int value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, float value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::vec2& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::vec3& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::vec4& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat2& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat2x3& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat2x4& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat3& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat3x2& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat3x4& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat4& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat4x2& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::set_program_uniform(const std::string& uniform, const glm::mat4x3& value)
  {
    MGL_CORE_ASSERT(s_state_data.current_program != nullptr, "Program is null");
    s_state_data.current_program->uniform(uniform)->set_value(value);
  }

  void opengl_api::disable_program()
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

  void opengl_api::draw_batch(vertex_array_ref& vao, const mgl::list<batch_data>& data)
  {
    for(auto& batch : data)
    {
      vao->draw(batch.model_view, batch.count, batch.offset, batch.instance_count);
    }
  }

  program_ref opengl_api::create_program(const std::string& vs_source,
                                         const std::string& fs_source,
                                         const std::string& gs_source,
                                         const std::string& tes_source,
                                         const std::string& tcs_source,
                                         const std::string& filename)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    mgl::opengl::shaders glsl(vs_source, fs_source, gs_source, tes_source, tcs_source);

    return ctx->program(glsl, filename);
  }

  texture_2d_ref opengl_api::create_texture_2d(
      int width, int height, int components, const mgl::uint8_buffer& data, int samples)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    return ctx->texture2d(width, height, components, data);
  }

  texture_2d_ref opengl_api::create_texture_2d(int width, int height, int components, int samples)
  {
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    return ctx->texture2d(width, height, components);
  }

  vertex_array_ref
  opengl_api::create_vertex_array(const buffer_ref& vb, const std::string& layout, uint32_t mode)
  {
    return mgl::create_ref<vertex_array>(vb, layout, mode);
  }

  vertex_array_ref opengl_api::create_vertex_array(const buffer_ref& vb,
                                                   const std::string& layout,
                                                   const buffer_ref& ib,
                                                   uint16_t element_size,
                                                   uint32_t mode)
  {
    return mgl::create_ref<vertex_array>(vb, layout, ib, element_size, mode);
  }

  vertex_array::~vertex_array()
  {
    if(m_vao != nullptr)
    {
      m_vao->release();
    }
  }

  void vertex_array::allocate(program_ref prg)
  {
    if(m_vao != nullptr)
    {
      return;
    }
    MGL_PROFILE_FUNCTION("API_DRAW_CALL");
    auto& ctx = mgl::platform::current_context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");

    // We get the uniform for the transform matrix
    if(prg == nullptr)
    {
      prg = s_state_data.current_program;
    }

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
    MGL_CORE_ASSERT(m_vao != nullptr, "VAO is null");
  }

  void vertex_array::deallocate()
  {
    if(m_vao == nullptr)
    {
      return;
    }
    m_vao->release();
    m_vao = nullptr;
  }

  void vertex_array::draw(const glm::mat4& model_view, int count, int offset, int instance_count)
  {
    MGL_CORE_ASSERT(m_vao != nullptr, "VAO is null");

    // We get the uniform for the transform matrix
    mgl::platform::api::uniform_ref transform_uniform = s_state_data.model_uniform;

    if(transform_uniform != nullptr)
    {
      transform_uniform->set_value(model_view);
    }

    m_vao->render((mgl::opengl::render_mode)m_mode, count, offset, instance_count);
  }

}; // namespace mgl::platform::api::backends