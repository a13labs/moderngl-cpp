#include "mgl_platform/api/opengl/api.hpp"
#include "mgl_platform/api/opengl/buffers.hpp"
#include "mgl_platform/api/opengl/program.hpp"
#include "mgl_platform/api/opengl/textures.hpp"
#include "mgl_platform/api/opengl/vertex_array.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"
#include "mgl_registry/resources/font.hpp"
#include "mgl_registry/resources/image.hpp"

#include "mgl_platform_internal.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace mgl::platform::api::backends
{
  // const mgl::float32_buffer quad_verts = {
  //   // positions        // texture Coords
  //   -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
  //   0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
  //   0.5f,  0.5f,  1.0f, 1.0f, // top right
  //   -0.5f, 0.5f,  0.0f, 1.0f // top left
  // };

  // const mgl::uint32_buffer quad_indices = {
  //   0, 1, 2, // first triangle
  //   2, 3, 0 // second triangle
  // };

  // static const char* quad_layout = "2f 2f";

  bool ogl_api::api_init()
  {
    m_ctx = mgl::opengl::create_context(mgl::opengl::context_mode::ATTACHED, 330);

    if(!m_ctx)
    {
      MGL_CORE_ASSERT(false, "[OpenGL API] Context is null.");
      return false;
    }

    MGL_CORE_ASSERT(m_ctx->is_valid(), "[OpenGL API] Context is not valid.");

    // auto vb = m_ctx->buffer(quad_verts);
    // auto ib = m_ctx->buffer(quad_indices);

    return true;
  }

  void ogl_api::api_shutdown()
  {
    MGL_PROFILE_FUNCTION("API_SHUTDOWN");
    // s_quad->deallocate();
    // delete s_quad;
  }

  void ogl_api::api_bind_screen_framebuffer()
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->screen().use();
  }

  void ogl_api::api_update_window_size(const glm::ivec2& size)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->screen().set_viewport({ 0, 0, size.x, size.y });
  }

  void ogl_api::api_enable_scissor()
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->enable_scissor();
  }

  void ogl_api::api_disable_scissor()
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->disable_scissor();
  }

  void ogl_api::api_set_scissor(const glm::vec2& position, const glm::vec2& size)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->set_scissor(position.x, position.y, size.x, size.y);
  }

  void ogl_api::api_enable_state(int32_t state)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->enable(state);
  }

  void ogl_api::api_disable_state(int32_t state)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->disable(state);
  }

  void ogl_api::api_clear(const glm::vec4& color)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->clear(color);
  }

  void ogl_api::api_set_viewport(const glm::vec2& position, const glm::vec2& size)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->set_viewport(position.x, position.y, size.x, size.y);
  }

  void ogl_api::api_clear_samplers(int32_t start, int32_t end)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->clear_samplers(start, end);
  }

  void ogl_api::api_set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->set_blend_equation(internal::to_api(modeRGB), internal::to_api(modeAlpha));
  }

  void ogl_api::api_set_blend_func(blend_factor srcRGB,
                                   blend_factor dstRGB,
                                   blend_factor srcAlpha,
                                   blend_factor dstAlpha)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    m_ctx->set_blend_func(internal::to_api(srcRGB),
                          internal::to_api(dstRGB),
                          internal::to_api(srcAlpha),
                          internal::to_api(dstAlpha));
  }

  void ogl_api::api_set_view_matrix(const glm::mat4& matrix)
  {
    m_state_data.view_matrix = matrix;
    if(m_state_data.current_program != nullptr)
    {
      m_state_data.current_program->set_value("view", matrix);
    }
  }

  void ogl_api::api_set_projection_matrix(const glm::mat4& matrix)
  {
    m_state_data.projection_matrix = matrix;
    if(m_state_data.current_program != nullptr)
    {
      m_state_data.current_program->set_value("projection", matrix);
    }
  }

  void ogl_api::api_enable_program(const program_ref& prg)
  {
    MGL_CORE_ASSERT(prg != nullptr, "Program is null");
    m_state_data.current_program = prg;
    prg->bind();

    // Set the view and projection matrices if the uniforms exist
    if(m_state_data.current_program != nullptr)
    {
      m_state_data.current_program->set_value("view", m_state_data.view_matrix);
    }

    if(m_state_data.current_program != nullptr)
    {
      m_state_data.current_program->set_value("projection", m_state_data.projection_matrix);
    }
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, bool value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, int32_t value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, float value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::vec2& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::vec3& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::vec4& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat2& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat2x3& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat2x4& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat3& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat3x2& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat3x4& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat4& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat4x2& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_set_program_uniform(const std::string& uniform, const glm::mat4x3& value)
  {
    MGL_CORE_ASSERT(m_state_data.current_program != nullptr, "Program is null");
    m_state_data.current_program->set_value(uniform, value);
  }

  void ogl_api::api_disable_program()
  {
    if(m_state_data.current_program == nullptr)
    {
      return;
    }

    m_state_data.current_program->unbind();
    m_state_data.current_program = nullptr;
  }

  void ogl_api::api_bind_texture(int32_t unit, const mgl::platform::api::texture_ref& texture)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    switch(texture->texture_type())
    {
      case mgl::platform::api::texture::type::TEXTURE_2D: {
        auto tex = std::static_pointer_cast<ogl_texture_2d>(texture);
        tex->bind(unit);
        break;
      }
      case mgl::platform::api::texture::type::TEXTURE_3D: {
        MGL_CORE_ASSERT(false, "Not implemented");
        break;
      }
      case mgl::platform::api::texture::type::TEXTURE_CUBE: {
        MGL_CORE_ASSERT(false, "Not implemented");
        break;
      }
      default: MGL_CORE_ASSERT(false, "Unknown texture type");
    }
  }

  void ogl_api::api_render_call(const mgl::platform::api::vertex_buffer_ref& vb,
                                const mgl::platform::api::index_buffer_ref& ib,
                                int32_t count,
                                int32_t offset,
                                render_mode mode)
  {
    MGL_PROFILE_FUNCTION("API_RENDER_CALL");
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    auto vao = create_vertex_array(vb, ib);
    vao->render(mode, count, offset);
    vao->release();
  }

  index_buffer_ref
  ogl_api::api_create_index_buffer(size_t size, uint16_t element_size, bool dynamic)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    return mgl::create_ref<ogl_index_buffer>(size, element_size, dynamic);
  }

  vertex_buffer_ref ogl_api::api_create_vertex_buffer(const std::string& layout,
                                                      mgl::string_list attrs,
                                                      size_t size,
                                                      bool dynamic)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    return mgl::create_ref<ogl_vertex_buffer>(layout, attrs, size, dynamic);
  }

  vertex_array_ref ogl_api::api_create_vertex_array(const vertex_buffer_ref& vbo,
                                                    const index_buffer_ref ibo)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    MGL_CORE_ASSERT(m_state_data.current_program, "No program bound");
    return mgl::create_ref<ogl_vertex_array>(m_state_data.current_program, vbo, ibo);
  }

  buffer_ref ogl_api::api_create_buffer(size_t size, bool dynamic)
  {
    // MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    // return mgl::create_ref<ogl_buffer>(size, dynamic);
    MGL_CORE_ASSERT(false, "Not implemented");
    return nullptr;
  }

  program_ref ogl_api::api_create_program(const std::string& vs_source,
                                          const std::string& fs_source,
                                          const std::string& gs_source,
                                          const std::string& tes_source,
                                          const std::string& tcs_source,
                                          const std::string& filename)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");

    return mgl::create_ref<ogl_program>(
        vs_source, fs_source, gs_source, tes_source, tcs_source, filename);
  }

  texture_2d_ref
  ogl_api::api_create_texture_2d(int32_t width, int32_t height, int32_t components, int32_t samples)
  {
    MGL_CORE_ASSERT(m_ctx != nullptr, "[OpenGL API] Context is null.");
    return mgl::create_ref<ogl_texture_2d>(mgl::size{ width, height }, components, samples);
  }

}; // namespace mgl::platform::api::backends
