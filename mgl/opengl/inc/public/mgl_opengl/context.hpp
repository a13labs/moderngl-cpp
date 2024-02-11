#pragma once
#include "attachment.hpp"
#include "buffer.hpp"
#include "buffer_layout.hpp"
#include "color_mask.hpp"
#include "compute_shader.hpp"
#include "conditional_render.hpp"
#include "data_type.hpp"
#include "enums.hpp"
#include "framebuffer.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "texture_2d.hpp"
#include "texture_3d.hpp"
#include "texture_array.hpp"
#include "texture_cube.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/string.hpp"

#include "glm/vec4.hpp"

namespace mgl::opengl
{

  using gl_context = void*;

  struct context_mode
  {
    enum mode
    {
      DETECT,
      SHARE,
      ATTACHED,
      STANDALONE
    };
  };

  using context_ref = mgl::ref<context>;

  class context : public mgl::ref_from_this<context>
  {
public:
    virtual ~context() = default;

    bool released() const { return m_released; }

    context_mode::mode mode() const { return m_mode; }

    int32_t version() const { return m_version; }

    int32_t max_samples() const { return m_max_samples; }

    int32_t max_integer_samples() const { return m_max_integer_samples; }

    int32_t max_color_attachments() const { return m_max_color_attachments; }

    int32_t max_texture_units() const { return m_max_texture_units; }

    int32_t default_texture_unit() const { return m_default_texture_unit; }

    float max_anisotropy() const { return m_max_anisotropy; }

    const mgl::string_list& extensions() const { return m_extensions; }

    framebuffer& screen() { return *m_default_framebuffer; }

    framebuffer_ref& current_framebuffer() { return m_bound_framebuffer; }

    int32_t enable_flags() const { return m_enable_flags; }

    void set_enable_flags(int32_t flags);

    void enable(int32_t flags);

    void disable(int32_t flags);

    void enable_direct(int32_t value);

    void disable_direct(int32_t value);

    void finish();

    void clear_samplers(int32_t start = 0, int32_t end = -1);

    int32_t front_face() const { return m_front_face; }

    int32_t cull_face() const { return m_cull_face; }

    int32_t depth_func() const { return m_depth_func; }

    int32_t blend_func_src() const { return m_blend_func_src; }

    int32_t blend_func_dst() const { return m_blend_func_dst; }

    void set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha);

    void set_blend_equation(blend_equation_mode mode) { set_blend_equation(mode, mode); }

    void set_blend_func(blend_factor srcRGB,
                        blend_factor dstRGB,
                        blend_factor srcAlpha,
                        blend_factor dstAlpha);

    void set_blend_func(blend_factor src, blend_factor dst) { set_blend_func(src, dst, src, dst); }

    bool wireframe() const { return m_wireframe; }

    bool multisample() const { return m_multisample; }

    int32_t provoking_vertex() const { return m_provoking_vertex; }

    float polygon_offset_factor() const { return m_polygon_offset_factor; }

    float polygon_offset_units() const { return m_polygon_offset_units; }

    void set_scissor(const mgl::rect& r) { m_bound_framebuffer->set_scissor(r); }

    void set_scissor(int32_t x, int32_t y, int32_t width, int32_t height)
    {
      m_bound_framebuffer->set_scissor({ x, y, width, height });
    }

    void reset_scissor() { m_bound_framebuffer->reset_scissor(); }

    void set_viewport(const mgl::rect& r) { m_bound_framebuffer->set_viewport(r); }

    void set_viewport(int32_t x, int32_t y, int32_t width, int32_t height)
    {
      m_bound_framebuffer->set_viewport({ x, y, width, height });
    }

    void enable_scissor() { m_bound_framebuffer->enable_scissor(); }

    void disable_scissor() { m_bound_framebuffer->disable_scissor(); }

    void clear(const glm::vec4& color,
               float depth = 0.0,
               const mgl::rect& viewport = mgl::null_viewport_2d);

    void clear(float r,
               float g,
               float b,
               float a = 0.0,
               float depth = 0.0,
               const mgl::rect& viewport = mgl::null_viewport_2d);

    // Buffer
    buffer_ref buffer(const mgl::uint8_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint8_t), dynamic);
    }

    buffer_ref buffer(const mgl::uint16_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint16_t), dynamic);
    }

    buffer_ref buffer(const mgl::uint32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(uint32_t), dynamic);
    }

    buffer_ref buffer(const mgl::int8_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int8_t), dynamic);
    }

    buffer_ref buffer(const mgl::int16_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int16_t), dynamic);
    }

    buffer_ref buffer(const mgl::int32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(int32_t), dynamic);
    }

    buffer_ref buffer(const mgl::float32_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(float), dynamic);
    }

    buffer_ref buffer(const mgl::float64_buffer& data, bool dynamic = false)
    {
      return buffer((void*)data.data(), data.size() * sizeof(double), dynamic);
    }

    buffer_ref buffer(size_t reserve = 0, bool dynamic = false)
    {
      return buffer(nullptr, reserve, dynamic);
    }

    buffer_ref buffer(void* data, size_t size, bool dynamic);

    // Compute Shader
    compute_shader_ref compute_shader(const std::string& source, const std::string& filename = "");

    // Create Shader
    static context_ref create_context(context_mode::mode mode, int32_t required = 330);

    // Framebuffer
    framebuffer_ref framebuffer(const attachments_ref& color_attachments,
                                attachment_ref depth_attachment);

    framebuffer_ref framebuffer(const attachments_ref& color_attachments)
    {
      return framebuffer(color_attachments, nullptr);
    }
    framebuffer_ref framebuffer(attachment_ref depth_attachment)
    {
      return framebuffer({}, depth_attachment);
    }

    // Program
    program_ref program(const shaders& shaders,
                        const shaders_outputs& outputs = {},
                        const fragment_outputs& fragment_outputs = {},
                        bool interleaved = true,
                        const std::string& filename = "");

    program_ref program(const shaders& shaders, const std::string& filename)
    {
      return program(shaders, {}, {}, true, filename);
    }

    // Query
    query_ref query(bool samples = false,
                    bool any_samples = false,
                    bool time_elapsed = false,
                    bool primitives_generated = false);

    // Renderbuffer
    renderbuffer_ref renderbuffer(int32_t width,
                                  int32_t height,
                                  int32_t components = 4,
                                  int32_t samples = 0,
                                  const std::string& dtype = "f1");
    renderbuffer_ref depth_renderbuffer(int32_t width, int32_t height, int32_t samples = 0);

    // Sampler
    sampler_ref sampler();

    // Scope
    scope_ref scope(framebuffer_ref framebuffer = nullptr,
                    int32_t enable_flags = 0,
                    const texture_bindings& textures = {},
                    const buffer_bindings& uniform_buffers = {},
                    const buffer_bindings& storage_buffers = {},
                    const sampler_bindings& samplers = {});

    // Texture
    texture_2d_ref texture2d(int32_t width,
                             int32_t height,
                             int32_t components,
                             const void* data = nullptr,
                             int32_t samples = 0,
                             int32_t alignment = 1,
                             const std::string& dtype = "f1",
                             int32_t internal_format_override = 0);

    texture_2d_ref texture2d(int32_t width,
                             int32_t height,
                             int32_t components,
                             const mgl::uint8_buffer& data,
                             int32_t samples = 0,
                             int32_t alignment = 1,
                             const std::string& dtype = "f1",
                             int32_t internal_format_override = 0)
    {
      return texture2d(width,
                       height,
                       components,
                       data.data(),
                       samples,
                       alignment,
                       dtype,
                       internal_format_override);
    }

    texture_2d_ref depth_texture2d(int32_t width,
                                   int32_t height,
                                   const void* data = nullptr,
                                   int32_t samples = 0,
                                   int32_t alignment = 0);

    // Texture3D
    texture_3d_ref texture3d(int32_t width,
                             int32_t height,
                             int32_t depth,
                             int32_t components,
                             const void* data = nullptr,
                             int32_t alignment = 1,
                             const std::string& dtype = "f1");

    // TextureArray
    texture_array_ref texture_array(int32_t width,
                                    int32_t height,
                                    int32_t layers,
                                    int32_t components,
                                    const void* data = nullptr,
                                    int32_t alignment = 1,
                                    const std::string& dtype = "f1");

    // TextureCube
    texture_cube_ref texture_cube(int32_t width,
                                  int32_t height,
                                  int32_t components,
                                  const void* data = nullptr,
                                  int32_t alignment = 1,
                                  const std::string& dtype = "f1",
                                  int32_t internal_format_override = 0);

    // VertexArray
    vertex_array_ref vertex_array(program_ref program,
                                  mgl::opengl::vertex_buffer_list vertex_buffers,
                                  buffer_ref index_buffer = nullptr,
                                  int32_t index_element_size = 4,
                                  bool skip_errors = false,
                                  mgl::opengl::render_mode mode = mgl::opengl::render_mode::POINTS);

    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void release() = 0;
    virtual bool is_valid() = 0;
    virtual bool is_current() = 0;

protected:
    bool m_released;
    context_mode::mode m_mode;

private:
    friend class framebuffer;

    int32_t m_version;
    int32_t m_max_samples;
    int32_t m_max_integer_samples;
    int32_t m_max_color_attachments;
    int32_t m_max_texture_units;
    int32_t m_default_texture_unit;
    float m_max_anisotropy;
    int32_t m_enable_flags;
    int32_t m_front_face;
    int32_t m_cull_face;
    int32_t m_depth_func;
    int32_t m_blend_func_src;
    int32_t m_blend_func_dst;
    bool m_wireframe;
    bool m_multisample;
    int32_t m_provoking_vertex;
    float m_polygon_offset_factor;
    float m_polygon_offset_units;
    mgl::string_list m_extensions;
    framebuffer_ref m_default_framebuffer;
    framebuffer_ref m_bound_framebuffer;
  };

#ifdef MGL_OPENGL_EGL
  class ContextEGL : public context
  {
public:
    ContextEGL(context_mode::mode mode, int32_t required);
    virtual ~ContextEGL() override;

    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;
    virtual bool is_current() override;

private:
    gl_context m_context;
  };
#endif

#ifdef MGL_OPENGL_WGL
  class ContextWGL : public context
  {

public:
    ContextWGL(context_mode::mode mode, int32_t required);
    ContextWGL(){};
    virtual ~ContextWGL() override;

    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;
    virtual bool is_current() override;

private:
    gl_context m_context;
  };
#endif

#ifdef MGL_OPENGL_CGL

  class ContextCGL : public context
  {

public:
    ContextCGL(context_mode::mode mode, int32_t required);
    virtual ~ContextCGL() override;

    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;
    virtual bool is_current() override;

private:
    gl_context m_context;
  };
#endif

  inline context_ref create_context(context_mode::mode mode, int32_t required = 330)
  {
    return context::create_context(mode, required);
  }
} // namespace  mgl::opengl
