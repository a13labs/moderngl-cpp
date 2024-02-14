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

#include "mgl_opengl_internal.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{

#ifdef MGL_DEBUG
  const static std::string opengl_debug_source_str[6] = {
    "API", "WINDOW_SYSTEM", "SHADER_COMPILER", "THIRD_PARTY", "APPLICATION", "OTHER",
  };

  const static std::string opengl_debug_type_str[6] = {
    "ERROR", "DEPRECATED BEHAVIOR", "UNDEFINED BEHAVIOR", "PORTABILITY", "PERFORMANCE", "OTHER"
  };

  const char* opengl_debug_severity_str[4] = { "HIGH", "MEDIUM", "LOW", "INFO" };

  void opengl_message_callback(GLenum source,
                               GLenum type,
                               GLuint id,
                               GLenum severity,
                               GLsizei length,
                               const GLchar* message,
                               const void* userParam)
  {
    int sev_idx, t_idx, src_idx;

    switch(severity)
    {
      case GL_DEBUG_SEVERITY_HIGH: sev_idx = 0; break;
      case GL_DEBUG_SEVERITY_MEDIUM: sev_idx = 1; break;
      case GL_DEBUG_SEVERITY_LOW: sev_idx = 2; break;
      default: sev_idx = 3; break;
    }

    switch(type)
    {
      case GL_DEBUG_TYPE_ERROR: t_idx = 0; break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: t_idx = 1; break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: t_idx = 2; break;
      case GL_DEBUG_TYPE_PORTABILITY: t_idx = 3; break;
      case GL_DEBUG_TYPE_PERFORMANCE: t_idx = 4; break;
      default: t_idx = 5; break;
    }

    switch(source)
    {
      case GL_DEBUG_SOURCE_API: src_idx = 0; break;
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src_idx = 1; break;
      case GL_DEBUG_SOURCE_SHADER_COMPILER: src_idx = 2; break;
      case GL_DEBUG_SOURCE_THIRD_PARTY: src_idx = 3; break;
      case GL_DEBUG_SOURCE_APPLICATION: src_idx = 4; break;
      default: src_idx = 5; break;
    }

    if(t_idx == 0)
    {
      MGL_CORE_ERROR("[GL DEBUG] ({0}) {1}, {2}: {3}",
                     opengl_debug_severity_str[sev_idx],
                     opengl_debug_source_str[src_idx],
                     opengl_debug_type_str[t_idx],
                     message);
    }
    else
    {
      MGL_CORE_ERROR("[GL DEBUG] ({0}) {1}, {2}: {3}\n",
                     opengl_debug_severity_str[sev_idx],
                     opengl_debug_source_str[src_idx],
                     opengl_debug_type_str[t_idx],
                     message);
    }
  }
#endif

  context_ref context::create_context(context_mode::mode mode, int32_t required)
  {

    context* native_ctx = nullptr;

#ifdef MGL_OPENGL_EGL
    MGL_CORE_INFO("[GL Context] Trying EGL context.");
    native_ctx = new ContextEGL(mode, required);

    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("[GL Context] EGL not supported.");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("[GL Context] EGL supported.");
    }
#endif
#ifdef MGL_OPENGL_WGL
    MGL_CORE_INFO("[GL Context] Trying WGL context.");
    native_ctx = new ContextWGL(mode, required);
    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("[GL Context] WGL not supported.");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("[GL Context] WGL supported.");
    }
#endif
#ifdef MGL_OPENGL_CGL
    MGL_CORE_INFO("[GL Context] Trying CGL context.");
    native_ctx = new ContextCGL(mode, required);
    if(!native_ctx->is_valid())
    {
      MGL_CORE_INFO("[GL Context] CGL not supported.");
      delete native_ctx;
      native_ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("[GL Context] CGL supported.");
    }
#endif

    if(!native_ctx)
    {
      MGL_CORE_ERROR("[GL Context] Error creating context. No more backends available.");
      return nullptr;
    }

    auto ctx = context_ref(native_ctx);

    ctx->m_wireframe = false;

    int32_t major = 0;
    int32_t minor = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    ctx->m_version = major * 100 + minor * 10;

    if(ctx->m_version < required)
    {
      MGL_CORE_ERROR(
          "[GL Context] OpenGL version {0} not supported. Required {1}", ctx->m_version, required);
      return nullptr;
    }

#ifdef MGL_DEBUG
    if(ctx->m_version >= 430)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(opengl_message_callback, nullptr);
      glDebugMessageControl(
          GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    }
    else
    {
      MGL_CORE_WARN("[GL Context] Debug output not supported in OpenGL version {0}",
                    ctx->m_version);
    }
#endif

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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto buffer = new mgl::opengl::buffer(shared_from_this(), data, reserve, dynamic);
    return buffer_ref(buffer);
  }

  compute_shader_ref context::compute_shader(const std::string& source, const std::string& filename)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto shader = new mgl::opengl::compute_shader(shared_from_this(), source, filename);
    return compute_shader_ref(shader);
  }

  framebuffer_ref context::framebuffer(const attachments_ref& color_attachments,
                                       attachment_ref depth_attachment)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto framebuffer =
        new mgl::opengl::framebuffer(shared_from_this(), color_attachments, depth_attachment);
    return framebuffer_ref(framebuffer);
  }

  program_ref context::program(const shaders& shaders,
                               const shaders_outputs& outputs,
                               const fragment_outputs& fragment_outputs,
                               bool interleaved,
                               const std::string& filename)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto program = new mgl::opengl::program(
        shared_from_this(), shaders, outputs, fragment_outputs, interleaved, filename);
    return program_ref(program);
  }

  query_ref
  context::query(bool samples, bool any_samples, bool time_elapsed, bool primitives_generated)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto query = new mgl::opengl::query(
        shared_from_this(), samples, any_samples, time_elapsed, primitives_generated);
    return query_ref(query);
  }

  renderbuffer_ref context::renderbuffer(
      int32_t width, int32_t height, int32_t components, int32_t samples, const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto renderbuffer = new mgl::opengl::renderbuffer(
        shared_from_this(), width, height, components, samples, dtype);
    return renderbuffer_ref(renderbuffer);
  }

  renderbuffer_ref context::depth_renderbuffer(int32_t width, int32_t height, int32_t samples)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto renderbuffer = new mgl::opengl::renderbuffer(shared_from_this(), width, height, samples);
    return renderbuffer_ref(renderbuffer);
  }

  sampler_ref context::sampler()
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
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
                                         int32_t index_element_size)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    auto vertex_array = new mgl::opengl::vertex_array(
        shared_from_this(), program, vertex_buffers, index_buffer, index_element_size);
    return vertex_array_ref(vertex_array);
  }

  void context::enable(int32_t flags)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    m_enable_flags |= flags;

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glEnable(GL_BLEND);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on enabling GL_BLEND");
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glEnable(GL_DEPTH_TEST);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on enabling GL_DEPTH_TEST");
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glEnable(GL_CULL_FACE);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on enabling GL_CULL_FACE");
    }

    if(flags & mgl::opengl::enable_flag::STENCIL_TEST)
    {
      glEnable(GL_STENCIL_TEST);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on enabling GL_STENCIL_TEST");
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glEnable(GL_RASTERIZER_DISCARD);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR,
                      "[GL Context] Fail on enabling RASTERIZER_DISCARD");
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glEnable(GL_PROGRAM_POINT_SIZE);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR,
                      "[GL Context] Fail on enabling GL_PROGRAM_POINT_SIZE");
    }
  }

  void context::disable(int32_t flags)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");

    m_enable_flags &= ~flags;

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glDisable(GL_BLEND);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on disabling GL_BLEND");
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glDisable(GL_DEPTH_TEST);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on disabling GL_DEPTH_TEST");
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glDisable(GL_CULL_FACE);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on disabling GL_CULL_FACE");
    }

    if(flags & mgl::opengl::enable_flag::STENCIL_TEST)
    {
      glDisable(GL_STENCIL_TEST);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR,
                      "[GL Context] Fail on disabling GL_STENCIL_TEST");
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glDisable(GL_RASTERIZER_DISCARD);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR,
                      "[GL Context] Fail on disabling RASTERIZER_DISCARD");
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glDisable(GL_PROGRAM_POINT_SIZE);
      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR,
                      "[GL Context] Fail on disabling GL_PROGRAM_POINT_SIZE");
    }
  }

  void context::enable_direct(int32_t value)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    glEnable(value);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on enable_direct");
  }

  void context::disable_direct(int32_t value)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    glDisable(value);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on disable_direct");
  }

  void context::finish()
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    glFinish();
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on glFinish");
  }

  void context::clear_samplers(int32_t start, int32_t end)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");

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
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    m_bound_framebuffer->clear(color, depth, viewport);
  }

  void context::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    m_bound_framebuffer->clear(r, g, b, a, depth, viewport);
  }

  void context::set_blend_equation(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    glBlendEquationSeparate(modeRGB, modeAlpha);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on glBlendEquationSeparate");
  }

  void context::set_blend_func(blend_factor srcRGB,
                               blend_factor dstRGB,
                               blend_factor srcAlpha,
                               blend_factor dstAlpha)
  {
    MGL_CORE_ASSERT(!released(), "[GL Context] Context already released or not valid.");
    MGL_CORE_ASSERT(is_current(), "[GL Context] Resource context not current.");
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Context] Fail on glBlendFuncSeparate");
  }

} // namespace  mgl::opengl
