#pragma once

#include "core.hpp"
#include "enums.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

#include "glm/glm.hpp"

namespace mgl::platform::api
{
  struct render_state
  {
    render_state()
        : current_program(nullptr)
        , view_matrix(1.0f)
        , view_uniform(nullptr)
        , projection_matrix(1.0f)
        , projection_uniform(nullptr)
        , model_uniform(nullptr)
    { }

    // The current shader, view and projection matrices are stored in the render, as they are
    // used by multiple commands
    program_ref current_program;
    mgl::string_list attributes;

    // Transform matrices
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

    // Program uniforms
    uniform_ref view_uniform;
    uniform_ref projection_uniform;
    uniform_ref model_uniform;
  };

  struct batch_data
  {
    batch_data()
        : model_view(1.0f)
        , count(0)
        , offset(0)
        , instance_count(1)
    { }

    batch_data(glm::mat4 t, size_t count, size_t offset, size_t instance_count)
        : model_view(t)
        , count(count)
        , offset(offset)
        , instance_count(instance_count)
    { }

    batch_data(const batch_data& other)
        : model_view(other.model_view)
        , count(other.count)
        , offset(other.offset)
        , instance_count(other.instance_count)
    { }

    glm::mat4 model_view;
    size_t count;
    size_t offset;
    size_t instance_count;
  };

  class render_api;
  using render_api_ref = mgl::scope<render_api>;

  class render_api
  {
public:
    virtual ~render_api() = default;

    virtual void init() = 0;

    virtual void shutdown() = 0;

    virtual void bind_screen_framebuffer() = 0;

    virtual void enable_scissor() = 0;

    virtual void disable_scissor() = 0;

    virtual void set_scissor(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void enable_state(int state) = 0;

    virtual void disable_state(int state) = 0;

    virtual void clear(const glm::vec4& color) = 0;

    virtual void set_viewport(const glm::vec2& position, const glm::vec2& size) = 0;

    virtual void clear_samplers(int start = 0, int end = -1) = 0;

    virtual void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha) = 0;

    virtual void set_blend_func(blend_factor srcRGB,
                                blend_factor dstRGB,
                                blend_factor srcAlpha,
                                blend_factor dstAlpha) = 0;

    virtual void set_view_matrix(const glm::mat4& matrix) = 0;

    virtual void set_projection_matrix(const glm::mat4& matrix) = 0;

    virtual void enable_program(const program_ref& program) = 0;

    virtual void set_program_attributes(const mgl::string_list& attributes) = 0;

    virtual void set_program_uniform(const std::string& uniform, bool value) = 0;

    virtual void set_program_uniform(const std::string& uniform, int value) = 0;

    virtual void set_program_uniform(const std::string& uniform, float value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::vec2& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::vec3& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::vec4& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat2& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat2x3& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat2x4& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat3& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat3x2& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat3x4& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat4& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat4x2& value) = 0;

    virtual void set_program_uniform(const std::string& uniform, const glm::mat4x3& value) = 0;

    virtual void disable_program() = 0;

    virtual void draw_batch(vertex_array_ref& vao, const mgl::list<batch_data>& data) = 0;

    virtual program_ref create_program(const std::string& vs_source,
                                       const std::string& fs_source,
                                       const std::string& gs_source = "",
                                       const std::string& tes_source = "",
                                       const std::string& tcs_source = "",
                                       const std::string& filename = "") = 0;

    virtual texture_2d_ref create_texture_2d(
        int width, int height, int components, const mgl::uint8_buffer& data, int samples = 0) = 0;

    virtual texture_2d_ref
    create_texture_2d(int width, int height, int components, int samples = 0) = 0;

    virtual vertex_array_ref
    create_vertex_array(const buffer_ref& vb, const std::string& layout, uint32_t mode) = 0;

    virtual vertex_array_ref create_vertex_array(const buffer_ref& vb,
                                                 const std::string& layout,
                                                 const buffer_ref& ib,
                                                 uint16_t element_size,
                                                 uint32_t mode) = 0;

    inline void clear(float r, float g, float b, float a) { clear(glm::vec4(r, g, b, a)); }

    inline void set_blend_equation(blend_equation_mode mode) { set_blend_equation(mode, mode); }

    inline void set_blend_func(blend_factor src, blend_factor dst)
    {
      set_blend_func(src, dst, src, dst);
    }

    inline void set_scissor(const glm::vec4& rect)
    {
      set_scissor(glm::vec2(rect), glm::vec2(rect.z, rect.w));
    }

    inline void set_scissor(float x, float y, float width, float height)
    {
      set_scissor(glm::vec4(x, y, width, height));
    }

    inline void draw(vertex_array_ref& vao,
                     const glm::mat4& model_view,
                     int count,
                     int offset,
                     int instance_count)
    {
      vao->draw(model_view, count, offset, instance_count);
    }

    static render_api& instance();

protected:
    render_api() = default;
  };

  inline void init_api()
  {
    render_api::instance().init();
  }

  inline void shutdown_api()
  {
    render_api::instance().shutdown();
  }

  inline void bind_screen_framebuffer()
  {
    render_api::instance().bind_screen_framebuffer();
  }

  inline void enable_scissor()
  {
    render_api::instance().enable_scissor();
  }

  inline void disable_scissor()
  {
    render_api::instance().disable_scissor();
  }

  inline void set_scissor(const glm::vec2& position, const glm::vec2& size)
  {
    render_api::instance().set_scissor(position, size);
  }

  inline void enable_state(int state)
  {
    render_api::instance().enable_state(state);
  }

  inline void disable_state(int state)
  {
    render_api::instance().disable_state(state);
  }

  inline void clear(const glm::vec4& color)
  {
    render_api::instance().clear(color);
  }

  inline void set_viewport(const glm::vec2& position, const glm::vec2& size)
  {
    render_api::instance().set_viewport(position, size);
  }

  inline void clear_samplers(int start = 0, int end = -1)
  {
    render_api::instance().clear_samplers(start, end);
  }

  inline void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    render_api::instance().set_blend_equation(modeRGB, modeAlpha);
  }

  inline void set_blend_func(blend_factor srcRGB,
                             blend_factor dstRGB,
                             blend_factor srcAlpha,
                             blend_factor dstAlpha)
  {
    render_api::instance().set_blend_func(srcRGB, dstRGB, srcAlpha, dstAlpha);
  }

  inline void set_scissor(const glm::vec4& rect)
  {
    render_api::instance().set_scissor(rect);
  }

  inline void set_scissor(float x, float y, float width, float height)
  {
    render_api::instance().set_scissor(x, y, width, height);
  }

  inline void clear(float r, float g, float b, float a)
  {
    render_api::instance().clear(r, g, b, a);
  }

  inline void set_blend_equation(blend_equation_mode mode)
  {
    render_api::instance().set_blend_equation(mode);
  }

  inline void set_blend_func(blend_factor src, blend_factor dst)
  {
    render_api::instance().set_blend_func(src, dst);
  }

  inline void set_view_matrix(const glm::mat4& matrix)
  {
    render_api::instance().set_view_matrix(matrix);
  }

  inline void set_projection_matrix(const glm::mat4& matrix)
  {
    render_api::instance().set_projection_matrix(matrix);
  }

  inline void enable_program(const program_ref& program)
  {
    render_api::instance().enable_program(program);
  }

  inline void set_program_attributes(const mgl::string_list& attributes)
  {
    render_api::instance().set_program_attributes(attributes);
  }

  inline void set_program_uniform(const std::string& uniform, bool value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, int value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, float value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::vec2& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::vec3& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::vec4& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat2& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat2x3& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat2x4& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat3& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat3x2& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat3x4& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat4& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat4x2& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void set_program_uniform(const std::string& uniform, const glm::mat4x3& value)
  {
    render_api::instance().set_program_uniform(uniform, value);
  }

  inline void disable_program()
  {
    render_api::instance().disable_program();
  }

  inline void draw(
      vertex_array_ref& vao, const glm::mat4& model_view, int count, int offset, int instance_count)
  {
    render_api::instance().draw(vao, model_view, count, offset, instance_count);
  }

  inline void draw_batch(vertex_array_ref& vao, const mgl::list<batch_data>& data)
  {
    render_api::instance().draw_batch(vao, data);
  }

  inline program_ref create_program(const std::string& vs_source,
                                    const std::string& fs_source,
                                    const std::string& gs_source = "",
                                    const std::string& tes_source = "",
                                    const std::string& tcs_source = "")
  {
    return render_api::instance().create_program(
        vs_source, fs_source, gs_source, tes_source, tcs_source);
  }

  inline texture_2d_ref create_texture_2d(
      int width, int height, int components, const mgl::uint8_buffer& data, int samples = 0)
  {
    return render_api::instance().create_texture_2d(width, height, components, data, samples);
  }

  inline texture_2d_ref create_texture_2d(int width, int height, int components, int samples = 0)
  {
    return render_api::instance().create_texture_2d(width, height, components, samples);
  }

  inline vertex_array_ref
  create_vertex_array(const buffer_ref& vb, const std::string& layout, uint32_t mode)
  {
    return render_api::instance().create_vertex_array(vb, layout, mode);
  }

  inline vertex_array_ref create_vertex_array(const buffer_ref& vb,
                                              const std::string& layout,
                                              const buffer_ref& ib,
                                              uint16_t element_size,
                                              uint32_t mode)
  {
    return render_api::instance().create_vertex_array(vb, layout, ib, element_size, mode);
  }

} // namespace mgl::platform::api