#include "mgl_opengl/context.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/buffer_layout.hpp"
#include "mgl_opengl/compute_shader.hpp"
#include "mgl_opengl/data_type.hpp"
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/program.hpp"
#include "mgl_opengl/query.hpp"
#include "mgl_opengl/renderbuffer.hpp"
#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/scope.hpp"
#include "mgl_opengl/shader.hpp"
#include "mgl_opengl/texture_2d.hpp"
#include "mgl_opengl/texture_3d.hpp"
#include "mgl_opengl/texture_array.hpp"
#include "mgl_opengl/texture_cube.hpp"
#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/uniform_block.hpp"
#include "mgl_opengl/vertex_array.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{

  context_ref context::create_context(context_mode::mode mode, int32_t required)
  {

    context* native_ctx = nullptr;

#ifdef MGL_OPENGL_EGL
    MGL_CORE_INFO("Trying EGL context!");
    native_ctx = new ContextEGL(mode, required);

    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("EGL not supported!");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("EGL supported!");
    }
#endif
#ifdef MGL_OPENGL_WGL
    MGL_CORE_INFO("Trying WGL context!");
    native_ctx = new ContextWGL(mode, required);
    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("WGL not supported!");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("WGL supported!");
    }
#endif
#ifdef MGL_OPENGL_CGL
    MGL_CORE_INFO("Trying CGL context!");
    native_ctx = new ContextCGL(mode, required);
    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("CGL not supported!");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("CGL supported!");
    }
#endif

    if(!native_ctx)
    {
      MGL_CORE_ERROR("Error creating context! No more backends available.");
      return nullptr;
    }

    auto ctx = context_ref(native_ctx);

    ctx->m_wireframe = false;

    int32_t major = 0;
    int32_t minor = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    ctx->m_version_code = major * 100 + minor * 10;

    if(ctx->m_version_code < required)
    {
      MGL_CORE_ERROR(
          "OpenGL version {0} not supported. Required {1}", ctx->m_version_code, required);
      return nullptr;
    }

    // Load extensions
    int32_t num_extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int32_t i = 0; i < num_extensions; i++)
    {
      const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
      ctx->m_extensions.push_back(ext);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    if(glPrimitiveRestartIndex)
    {
      glEnable(GL_PRIMITIVE_RESTART);
      glPrimitiveRestartIndex(-1);
    }
    else
    {
      glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }

    ctx->m_max_samples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, (GLint*)&ctx->m_max_samples);

    ctx->m_max_integer_samples = 0;
    glGetIntegerv(GL_MAX_INTEGER_SAMPLES, (GLint*)&ctx->m_max_integer_samples);

    ctx->m_max_color_attachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint*)&ctx->m_max_color_attachments);

    ctx->m_max_texture_units = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&ctx->m_max_texture_units);
    ctx->m_default_texture_unit = ctx->m_max_texture_units - 1;

    ctx->m_max_anisotropy = 0.0;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, (GLfloat*)&ctx->m_max_anisotropy);

    ctx->m_default_framebuffer = framebuffer_ref(new mgl::opengl::framebuffer(ctx));

    ctx->m_bound_framebuffer = ctx->m_default_framebuffer;

    ctx->m_enable_flags = 0;
    ctx->m_front_face = GL_CCW;

    ctx->m_depth_func = GL_LEQUAL;
    ctx->m_blend_func_src = GL_SRC_ALPHA;
    ctx->m_blend_func_dst = GL_ONE_MINUS_SRC_ALPHA;

    ctx->m_wireframe = false;
    ctx->m_multisample = true;

    ctx->m_provoking_vertex = GL_LAST_VERTEX_CONVENTION;

    ctx->m_polygon_offset_factor = 0.0f;
    ctx->m_polygon_offset_units = 0.0f;

    glGetError(); // clear errors

    return ctx;
  }

  buffer_ref context::buffer(void* data, size_t reserve, bool dynamic)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto buffer = new mgl::opengl::buffer(shared_from_this(), data, reserve, dynamic);
    return buffer_ref(buffer);
  }

  compute_shader_ref context::compute_shader(const std::string& source)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto shader = new mgl::opengl::compute_shader(shared_from_this(), source);
    return compute_shader_ref(shader);
  }

  framebuffer_ref context::framebuffer(const attachments_ref& color_attachments,
                                       attachment_ref depth_attachment)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto framebuffer =
        new mgl::opengl::framebuffer(shared_from_this(), color_attachments, depth_attachment);
    return framebuffer_ref(framebuffer);
  }

  program_ref context::program(const shaders& shaders,
                               const shaders_outputs& outputs,
                               const fragment_outputs& fragment_outputs,
                               bool interleaved)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto program = new mgl::opengl::program(
        shared_from_this(), shaders, outputs, fragment_outputs, interleaved);
    return program_ref(program);
  }

  query_ref
  context::query(bool samples, bool any_samples, bool time_elapsed, bool primitives_generated)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto query = new mgl::opengl::query(
        shared_from_this(), samples, any_samples, time_elapsed, primitives_generated);
    return query_ref(query);
  }

  renderbuffer_ref context::renderbuffer(
      int32_t width, int32_t height, int32_t components, int32_t samples, const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto renderbuffer = new mgl::opengl::renderbuffer(
        shared_from_this(), width, height, components, samples, dtype);
    return renderbuffer_ref(renderbuffer);
  }

  renderbuffer_ref context::depth_renderbuffer(int32_t width, int32_t height, int32_t samples)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto renderbuffer = new mgl::opengl::renderbuffer(shared_from_this(), width, height, samples);
    return renderbuffer_ref(renderbuffer);
  }

  sampler_ref context::sampler()
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto sampler = new mgl::opengl::sampler(shared_from_this());
    return sampler_ref(sampler);
  }

  scope_ref context::scope(framebuffer_ref framebuffer,
                           int32_t enable_flags,
                           const texture_bindings& textures,
                           const buffer_bindings& uniform_buffers,
                           const buffer_bindings& storage_buffers,
                           const sampler_bindings& samplers)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto scope = new mgl::opengl::scope(shared_from_this(),
                                        framebuffer,
                                        enable_flags,
                                        textures,
                                        uniform_buffers,
                                        storage_buffers,
                                        samplers);
    return scope_ref(scope);
  }

  texture_2d_ref context::texture2d(int32_t width,
                                    int32_t height,
                                    int32_t components,
                                    const void* data,
                                    int32_t samples,
                                    int32_t alignment,
                                    const std::string& dtype,
                                    int32_t internal_format_override)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto texture = new mgl::opengl::texture_2d(shared_from_this(),
                                               width,
                                               height,
                                               components,
                                               data,
                                               samples,
                                               alignment,
                                               dtype,
                                               internal_format_override);
    return texture_2d_ref(texture);
  }

  texture_2d_ref context::depth_texture2d(
      int32_t width, int32_t height, const void* data, int32_t samples, int32_t alignment)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto texture =
        new mgl::opengl::texture_2d(shared_from_this(), width, height, data, samples, alignment);
    return texture_2d_ref(texture);
  }

  texture_3d_ref context::texture3d(int32_t width,
                                    int32_t height,
                                    int32_t depth,
                                    int32_t components,
                                    const void* data,
                                    int32_t alignment,
                                    const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto texture = new mgl::opengl::texture_3d(
        shared_from_this(), width, height, depth, components, data, alignment, dtype);
    return texture_3d_ref(texture);
  }

  texture_array_ref context::texture_array(int32_t width,
                                           int32_t height,
                                           int32_t layers,
                                           int32_t components,
                                           const void* data,
                                           int32_t alignment,
                                           const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto texture = new mgl::opengl::texture_array(
        shared_from_this(), width, height, layers, components, data, alignment, dtype);
    return texture_array_ref(texture);
  }

  texture_cube_ref context::texture_cube(int32_t width,
                                         int32_t height,
                                         int32_t components,
                                         const void* data,
                                         int32_t alignment,
                                         const std::string& dtype,
                                         int32_t internal_format_override)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto texture = new mgl::opengl::texture_cube(shared_from_this(),
                                                 width,
                                                 height,
                                                 components,
                                                 data,
                                                 alignment,
                                                 dtype,
                                                 internal_format_override);
    return texture_cube_ref(texture);
  }

  vertex_array_ref context::vertex_array(program_ref program,
                                         mgl::opengl::vertex_buffer_list vertex_buffers,
                                         buffer_ref index_buffer,
                                         int32_t index_element_size,
                                         bool skip_errors,
                                         mgl::opengl::render_mode mode)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto vertex_array = new mgl::opengl::vertex_array(shared_from_this(),
                                                      program,
                                                      vertex_buffers,
                                                      index_buffer,
                                                      index_element_size,
                                                      skip_errors,
                                                      mode);
    return vertex_array_ref(vertex_array);
  }

  void context::set_enable_flags(int32_t flags)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    m_enable_flags = flags;

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glEnable(GL_CULL_FACE);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glEnable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      glDisable(GL_RASTERIZER_DISCARD);
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glEnable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      glDisable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void context::enable(int32_t flags)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    m_enable_flags |= flags;

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glEnable(GL_BLEND);
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glEnable(GL_DEPTH_TEST);
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glEnable(GL_CULL_FACE);
    }

    if(flags & mgl::opengl::enable_flag::STENCIL_TEST)
    {
      glEnable(GL_STENCIL_TEST);
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glEnable(GL_RASTERIZER_DISCARD);
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glEnable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void context::disable(int32_t flags)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");

    m_enable_flags &= ~flags;

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glDisable(GL_BLEND);
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glDisable(GL_DEPTH_TEST);
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glDisable(GL_CULL_FACE);
    }

    if(flags & mgl::opengl::enable_flag::STENCIL_TEST)
    {
      glDisable(GL_STENCIL_TEST);
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glDisable(GL_RASTERIZER_DISCARD);
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glDisable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void context::copy_buffer(const buffer_ref& src,
                            const buffer_ref& dst,
                            size_t size,
                            size_t read_offset,
                            size_t write_offset)
  {
    MGL_CORE_ASSERT(read_offset >= 0 && write_offset >= 0, "buffer underflow");
    MGL_CORE_ASSERT(!released(), "Context already released");

    if(size < 0)
    {
      size = src->size() - read_offset;
    }

    MGL_CORE_ASSERT((read_offset + size <= src->size() && write_offset + size <= dst->size()),
                    "buffer overflow");

    glBindBuffer(GL_COPY_READ_BUFFER, src->glo());
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst->glo());
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);
  }

  void context::enable_direct(int32_t value)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    glEnable(value);
  }

  void context::disable_direct(int32_t value)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    glDisable(value);
  }

  void context::finish()
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    glFinish();
  }

  void context::clear_samplers(int32_t start, int32_t end)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");

    start = MGL_MAX(start, 0);
    if(end == -1)
    {
      end = m_max_texture_units;
    }
    else
    {
      end = MGL_MIN(end, m_max_texture_units);
    }

    for(int32_t i = start; i < end; i++)
    {
      glBindSampler(i, 0);
    }
  }

  void context::clear(const glm::vec4& color, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    MGL_CORE_ASSERT(m_bound_framebuffer, "Context already released");
    m_bound_framebuffer->clear(color, depth, viewport);
  }

  void context::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    MGL_CORE_ASSERT(m_bound_framebuffer, "Context already released");
    m_bound_framebuffer->clear(r, g, b, a, depth, viewport);
  }

  void context::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    glBlendEquationSeparate(modeRGB, modeAlpha);
  }

  void context::set_blend_func(blend_factor srcRGB,
                               blend_factor dstRGB,
                               blend_factor srcAlpha,
                               blend_factor dstAlpha)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
  }

} // namespace  mgl::opengl
