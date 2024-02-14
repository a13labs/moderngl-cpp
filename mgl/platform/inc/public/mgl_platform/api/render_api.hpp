#pragma once

#include "buffers.hpp"
#include "enums.hpp"
#include "program.hpp"
#include "textures.hpp"
#include "vertex_array.hpp"

#include "mgl_registry/resources/image.hpp"

#include "glm/glm.hpp"

namespace mgl::platform::api
{
  struct render_state
  {
    render_state()
        : current_program(nullptr)
        , view_matrix(1.0f)
        , projection_matrix(1.0f)
    { }

    // The current shader, view and projection matrices are stored in the render, as they are
    // used by multiple commands
    program_ref current_program;

    // Transform matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
  };

  class render_api;
  using render_api_ref = mgl::scope<render_api>;

  class render_api
  {
public:
    enum class dialect
    {
      OPENGL,
      UNKNOWN
    };

    virtual ~render_api() = default;

    virtual dialect api() const = 0;

protected:
    static render_api& instance();

private:
    virtual bool api_init() = 0;

    virtual void api_shutdown() = 0;

    virtual void api_update_window_size(const glm::ivec2& size) = 0;

    virtual void api_bind_screen_framebuffer() = 0;

    virtual void api_enable_scissor() = 0;

    virtual void api_disable_scissor() = 0;

    virtual void api_set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void api_enable_state(int32_t state) = 0;

    virtual void api_disable_state(int32_t state) = 0;

    virtual void api_clear(const glm::vec4& color) = 0;

    virtual void api_set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void api_clear_samplers(int32_t start = 0, int32_t end = -1) = 0;

    virtual void api_set_blend_equation(blend_equation_mode modeRGB,
                                        blend_equation_mode modeAlpha) = 0;

    virtual void api_set_blend_func(blend_factor srcRGB,
                                    blend_factor dstRGB,
                                    blend_factor srcAlpha,
                                    blend_factor dstAlpha) = 0;

    virtual void api_set_view_matrix(const glm::mat4& matrix) = 0;

    virtual void api_set_projection_matrix(const glm::mat4& matrix) = 0;

    virtual void api_enable_program(const mgl::platform::api::program_ref& program) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, bool value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, int32_t value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, float value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::vec4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2x3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat2x4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3x2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat3x4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4x2& value) = 0;

    virtual void api_set_program_uniform(const std::string& uniform, const glm::mat4x3& value) = 0;

    virtual void api_disable_program() = 0;

    virtual void api_bind_texture(int32_t unit, const mgl::platform::api::texture_ref& texture) = 0;

    virtual void api_render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                                 const mgl::platform::api::index_buffer_ref& index_buffer,
                                 int32_t count,
                                 int32_t offset,
                                 render_mode mode) = 0;

    virtual index_buffer_ref
    api_create_index_buffer(size_t size, uint16_t element_size, bool dynamic) = 0;

    virtual vertex_buffer_ref api_create_vertex_buffer(const std::string& layout,
                                                       mgl::string_list attrs,
                                                       size_t size,
                                                       bool dynamic) = 0;

    virtual vertex_array_ref api_create_vertex_array(const vertex_buffer_ref& vbo,
                                                     const index_buffer_ref ibo) = 0;

    virtual buffer_ref api_create_buffer(size_t size, bool dynamic) = 0;

    virtual program_ref api_create_program(const std::string& vs_source,
                                           const std::string& fs_source,
                                           const std::string& gs_source = "",
                                           const std::string& tes_source = "",
                                           const std::string& tcs_source = "",
                                           const std::string& filename = "") = 0;

    virtual texture_2d_ref api_create_texture_2d(int32_t width,
                                                 int32_t height,
                                                 int32_t components,
                                                 int32_t samples = 0) = 0;

public:
    static bool init_api() { return render_api::instance().api_init(); }

    static void shutdown_api() { render_api::instance().api_shutdown(); }

    static void bind_screen_framebuffer() { render_api::instance().api_bind_screen_framebuffer(); }

    static void update_window_size(const glm::vec2& size)
    {
      render_api::instance().api_update_window_size(size);
    }

    static void clear(float r, float g, float b, float a)
    {
      render_api::instance().api_clear(glm::vec4(r, g, b, a));
    }

    static void clear(const glm::vec4& color) { render_api::instance().api_clear(color); }

    static void set_blend_equation(blend_equation_mode mode)
    {
      render_api::instance().api_set_blend_equation(mode, mode);
    }

    static void set_blend_func(blend_factor src, blend_factor dst)
    {
      render_api::instance().api_set_blend_func(src, dst, src, dst);
    }

    static void enable_scissor() { render_api::instance().api_enable_scissor(); }

    static void disable_scissor() { render_api::instance().api_disable_scissor(); }

    static void set_scissor(const glm::vec2& position, const glm::vec2& size)
    {
      render_api::instance().api_set_scissor(position, size);
    }

    static void set_scissor(const glm::vec4& rect)
    {
      render_api::instance().api_set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w));
    }

    static void set_scissor(float x, float y, float width, float height)
    {
      render_api::instance().api_set_scissor(glm::vec2(x, y), glm::vec2(width, height));
    }

    static void enable_state(int32_t state) { render_api::instance().api_enable_state(state); }

    static void disable_state(int32_t state) { render_api::instance().api_disable_state(state); }

    static void set_viewport(const glm::vec2& position, const glm::vec2& size)
    {
      render_api::instance().api_set_viewport(position, size);
    }

    static void clear_samplers(int32_t start = 0, int32_t end = -1)
    {
      render_api::instance().api_clear_samplers(start, end);
    }

    static void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
    {
      render_api::instance().api_set_blend_equation(modeRGB, modeAlpha);
    }

    static void set_blend_func(blend_factor srcRGB,
                               blend_factor dstRGB,
                               blend_factor srcAlpha,
                               blend_factor dstAlpha)
    {
      render_api::instance().api_set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }

    static void set_view_matrix(const glm::mat4& matrix)
    {
      render_api::instance().api_set_view_matrix(matrix);
    }

    static void set_projection_matrix(const glm::mat4& matrix)
    {
      render_api::instance().api_set_projection_matrix(matrix);
    }

    static void enable_program(const mgl::platform::api::program_ref& program)
    {
      render_api::instance().api_enable_program(program);
    }

    static void set_program_uniform(const std::string& uniform, bool value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, int32_t value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, float value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec2& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec3& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::vec4& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2x3& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat2x4& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3x2& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat3x4& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4x2& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void set_program_uniform(const std::string& uniform, const glm::mat4x3& value)
    {
      render_api::instance().api_set_program_uniform(uniform, value);
    }

    static void disable_program() { render_api::instance().api_disable_program(); }

    static void bind_texture(int32_t unit, const mgl::platform::api::texture_ref& texture)
    {
      render_api::instance().api_bind_texture(unit, texture);
    }

    static void render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                            const mgl::platform::api::index_buffer_ref& index_buffer,
                            int32_t count,
                            int32_t offset,
                            render_mode mode)
    {
      render_api::instance().api_render_call(vertex_buffer, index_buffer, count, offset, mode);
    }

    static void render_call(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                            int32_t count,
                            int32_t offset,
                            render_mode mode)
    {
      render_api::instance().api_render_call(vertex_buffer, nullptr, count, offset, mode);
    }

    static program_ref create_program(const std::string& vs_source,
                                      const std::string& fs_source,
                                      const std::string& gs_source = "",
                                      const std::string& tes_source = "",
                                      const std::string& tcs_source = "")
    {
      return render_api::instance().api_create_program(
          vs_source, fs_source, gs_source, tes_source, tcs_source);
    }

    static texture_2d_ref
    create_texture_2d(int32_t width, int32_t height, int32_t components, int32_t samples = 0)
    {
      return render_api::instance().api_create_texture_2d(width, height, components, samples);
    }

    static texture_2d_ref create_texture_2d(int32_t width,
                                            int32_t height,
                                            int32_t components,
                                            const mgl::uint8_buffer& data,
                                            int32_t samples = 0)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid texture data");
      MGL_CORE_ASSERT(data.size() == width * height * components, "Invalid texture data size");
      auto texture =
          render_api::instance().api_create_texture_2d(width, height, components, samples);
      texture->upload(data, { 0, 0, width, height });
      return texture;
    }

    static texture_2d_ref create_texture_2d(const mgl::registry::image_ref& image,
                                            int32_t samples = 0)
    {
      auto texture = render_api::instance().api_create_texture_2d(
          image->width(), image->height(), image->channels(), samples);
      texture->upload(image, { 0, 0, image->width(), image->height() });
      return texture;
    }

    static index_buffer_ref
    create_index_buffer(const uint16_buffer& data, uint16_t element_size = 4, bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid index buffer data");
      auto buffer = render_api::instance().api_create_index_buffer(
          data.size() * sizeof(uint16_t), element_size, dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Index buffer is null");
      buffer->upload(data);
      return buffer;
    }

    static index_buffer_ref
    create_index_buffer(const uint32_buffer& data, uint16_t element_size = 4, bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid index buffer data");
      auto buffer = render_api::instance().api_create_index_buffer(
          data.size() * sizeof(uint32_t), element_size, dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Index buffer is null");
      buffer->upload(data);
      return buffer;
    }

    static index_buffer_ref
    create_index_buffer(size_t size, uint16_t element_size = 4, bool dynamic = false)
    {
      return render_api::instance().api_create_index_buffer(size, element_size, dynamic);
    }

    static vertex_buffer_ref create_vertex_buffer(const float32_buffer& data,
                                                  const std::string& layout,
                                                  mgl::string_list attrs,
                                                  bool dynamic = false)
    {
      MGL_CORE_ASSERT(data.size() > 0, "Invalid vertex buffer data");
      auto buffer = render_api::instance().api_create_vertex_buffer(
          layout, attrs, data.size() * sizeof(float), dynamic);
      MGL_CORE_ASSERT(buffer != nullptr, "Vertex buffer is null");
      buffer->upload(data);
      return buffer;
    }

    static vertex_buffer_ref
    create_vertex_buffer(const std::string& layout, mgl::string_list attrs, bool dynamic = false)
    {
      return render_api::instance().api_create_vertex_buffer(layout, attrs, 0, dynamic);
    }

    static vertex_buffer_ref create_vertex_buffer(size_t size,
                                                  const std::string& layout,
                                                  mgl::string_list attrs,
                                                  bool dynamic = false)
    {
      return render_api::instance().api_create_vertex_buffer(layout, attrs, size, dynamic);
    }

    static vertex_array_ref create_vertex_array(const vertex_buffer_ref& vbo,
                                                const index_buffer_ref ibo)
    {
      return render_api::instance().api_create_vertex_array(vbo, ibo);
    }

    static buffer_ref create_buffer(size_t size, bool dynamic = false)
    {
      return render_api::instance().api_create_buffer(size, dynamic);
    }

protected:
    render_api() = default;
  };

} // namespace mgl::platform::api