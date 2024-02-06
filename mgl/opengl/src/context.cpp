#include "mgl_opengl/context.hpp"
#include "mgl_opengl/attribute.hpp"
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
#include "mgl_opengl/subroutine.hpp"
#include "mgl_opengl/texture_2d.hpp"
#include "mgl_opengl/texture_3d.hpp"
#include "mgl_opengl/texture_array.hpp"
#include "mgl_opengl/texture_cube.hpp"
#include "mgl_opengl/uniform.hpp"
#include "mgl_opengl/uniform_block.hpp"
#include "mgl_opengl/varying.hpp"
#include "mgl_opengl/vertex_array.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{

  context_ref context::create_context(context_mode::Enum mode, int32_t required)
  {

    context* ctx = nullptr;

#ifdef MGL_OPENGL_EGL
    MGL_CORE_INFO("Trying EGL context!");
    ctx = new ContextEGL(mode, required);
    if(!ctx->is_valid())
    {
      MGL_CORE_INFO("EGL not supported!");
      delete ctx;
      ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("EGL supported!");
    }
#endif
#ifdef MGL_OPENGL_WGL
    MGL_CORE_INFO("Trying WGL context!");
    ctx = new ContextWGL(mode, required);
    if(!ctx->is_valid())
    {
      MGL_CORE_INFO("WGL not supported!");
      delete ctx;
      ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("WGL supported!");
    }
#endif
#ifdef MGL_OPENGL_CGL
    MGL_CORE_INFO("Trying CGL context!");
    ctx = new ContextCGL(mode, required);
    if(!ctx->is_valid())
    {
      MGL_CORE_INFO("CGL not supported!");
      delete ctx;
      ctx = nullptr;
    }
    else
    {
      MGL_CORE_INFO("CGL supported!");
    }
#endif

    if(!ctx)
    {
      MGL_CORE_ERROR("Error creating context! No more backends available.");
      return nullptr;
    }

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
      delete ctx;
      return nullptr;
    }

    // Load extensions
    int32_t num_extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int32_t i = 0; i < num_extensions; i++)
    {
      const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
      MGL_CORE_INFO("Found GL extension: {0}", ext);
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

    int32_t bound_framebuffer = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

#ifdef MGL_PLATFORM_MACOS
    if(mode == context_mode::STANDALONE)
    {
      int32_t renderbuffer = 0;
      glGenRenderbuffers(1, (GLuint*)&renderbuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 4, 4);

      int32_t framebuffer = 0;
      glGenFramebuffers(1, (GLuint*)&framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      glFramebufferRenderbuffer(
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
      bound_framebuffer = framebuffer;
    }
#endif
    {
      auto framebuffer = new mgl::opengl::framebuffer();
      framebuffer->m_released = false;

      framebuffer->m_context = ctx;

      framebuffer->m_framebuffer_obj = 0;
      framebuffer->m_draw_buffers_len = 1;
      framebuffer->m_draw_buffers = new unsigned[1];

      // According to glGet docs:
      // The initial value is GL_BACK if there are back buffers, otherwise it is GL_FRONT.

      // According to glDrawBuffer docs:
      // The symbolic constants GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, and GL_FRONT_AND_BACK
      // are not allowed in the bufs array since they may refer to multiple buffers.

      // GL_COLOR_ATTACHMENT0 is causes error: 1282
      // This value is temporarily ignored

      // framebuffer->draw_buffers[0] = GL_COLOR_ATTACHMENT0;
      // framebuffer->draw_buffers[0] = GL_BACK_LEFT;

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glGetIntegerv(GL_DRAW_BUFFER, (int32_t*)&framebuffer->m_draw_buffers[0]);
      glBindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

      framebuffer->m_color_masks = { { true, true, true, true } };
      framebuffer->m_depth_mask = true;

      int32_t scissor_box[4] = {};
      glGetIntegerv(GL_SCISSOR_BOX, (int32_t*)&scissor_box);

      framebuffer->m_viewport = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

      framebuffer->m_scissor_enabled = false;
      framebuffer->m_scissor = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

      framebuffer->m_width = scissor_box[2];
      framebuffer->m_height = scissor_box[3];
      framebuffer->m_dynamic = true;

      ctx->m_default_framebuffer = framebuffer_ref(framebuffer);
    }

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

    return context_ref(ctx);
  }

  buffer_ref context::buffer(void* data, size_t reserve, bool dynamic)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto buffer = new mgl::opengl::buffer(data, reserve, dynamic);
    return buffer_ref(buffer);
  }

  compute_shader_ref context::compute_shader(const std::string& source)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto shader = new mgl::opengl::compute_shader(source);
    return compute_shader_ref(shader);
  }

  framebuffer_ref context::framebuffer(const attachments_ref& color_attachments,
                                       attachment_ref depth_attachment)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto framebuffer = new mgl::opengl::framebuffer(color_attachments, depth_attachment);
    return framebuffer_ref(framebuffer);
  }

  program_ref context::program(const shaders& shaders,
                               const shaders_outputs& outputs,
                               const fragment_outputs& fragment_outputs,
                               bool interleaved)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto program = new mgl::opengl::program(shaders, outputs, fragment_outputs, interleaved);
    return program_ref(program);
  }

  query_ref
  context::query(bool samples, bool any_samples, bool time_elapsed, bool primitives_generated)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto query = new mgl::opengl::query(samples, any_samples, time_elapsed, primitives_generated);
    return query_ref(query);
  }

  renderbuffer_ref context::renderbuffer(
      int32_t width, int32_t height, int32_t components, int32_t samples, const std::string& dtype)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto renderbuffer = new mgl::opengl::renderbuffer(width, height, components, samples, dtype);
    return renderbuffer_ref(renderbuffer);
  }

  renderbuffer_ref context::depth_renderbuffer(int32_t width, int32_t height, int32_t samples)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto renderbuffer = new mgl::opengl::renderbuffer(width, height, samples);
    return renderbuffer_ref(renderbuffer);
  }

  sampler_ref context::sampler()
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    auto sampler = new mgl::opengl::sampler();
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
    auto scope = new mgl::opengl::scope(
        framebuffer, enable_flags, textures, uniform_buffers, storage_buffers, samplers);
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
    if(components < 1 || components > 4)
    {
      MGL_CORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if((samples & (samples - 1)) || samples > m_max_samples)
    {
      MGL_CORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      MGL_CORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    if(data != nullptr && samples)
    {
      MGL_CORE_ERROR("Multisample textures are not writable directly", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    int32_t texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = internal_format_override ? internal_format_override
                                                       : data_type->internal_format[components];

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);

    auto texture = new texture_2d();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = components;
    texture->m_samples = samples;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;
    texture->m_compare_func = mgl::opengl::compare_func::NONE;
    texture->m_anisotropy = 1.0f;
    texture->m_depth = false;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_texture_obj = 0;

    glGenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      MGL_CORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    glBindTexture(texture_target, texture->m_texture_obj);

    if(samples)
    {
      glTexImage2DMultisample(texture_target, samples, internal_format, width, height, true);
    }
    else
    {
      glPixelStorei(GL_PACK_ALIGNMENT, alignment);
      glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
      glTexImage2D(
          texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, data);
      if(data_type->float_type)
      {
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else
      {
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
    }

    return texture_2d_ref(texture);
  }

  texture_2d_ref context::depth_texture2d(
      int32_t width, int32_t height, const void* data, int32_t samples, int32_t alignment)
  {
    MGL_CORE_ASSERT(!released(), "Context already released");
    if((samples & (samples - 1)) || samples > m_max_samples)
    {
      MGL_CORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      MGL_CORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    if(data != nullptr && samples)
    {
      MGL_CORE_ERROR("Multisample textures are not writable directly", alignment);
      return nullptr;
    }

    auto texture = new texture_2d();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = 1;
    texture->m_samples = samples;
    texture->m_data_type = from_dtype("f4", 2);
    texture->m_max_level = 0;
    texture->m_compare_func = mgl::opengl::compare_func::EQUAL;
    texture->m_anisotropy = 1.0f;
    texture->m_depth = true;
    texture->m_filter = { GL_LINEAR, GL_LINEAR };
    texture->m_repeat_x = false;
    texture->m_repeat_y = false;
    texture->m_texture_obj = 0;

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    glGenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      MGL_CORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int32_t texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int32_t pixel_type = GL_FLOAT;

    glBindTexture(texture_target, texture->m_texture_obj);

    if(samples)
    {
      glTexImage2DMultisample(texture_target, samples, GL_DEPTH_COMPONENT24, width, height, true);
    }
    else
    {
      glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glPixelStorei(GL_PACK_ALIGNMENT, alignment);
      glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
      glTexImage2D(texture_target,
                   0,
                   GL_DEPTH_COMPONENT24,
                   width,
                   height,
                   0,
                   GL_DEPTH_COMPONENT,
                   pixel_type,
                   data);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

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
    if(components < 1 || components > 4)
    {
      MGL_CORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      MGL_CORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new texture_3d();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_depth = depth;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_repeat_z = true;
    texture->m_texture_obj = 0;

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    glGenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      MGL_CORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = data_type->internal_format[components];

    glBindTexture(GL_TEXTURE_3D, texture->m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexImage3D(
        GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, base_format, pixel_type, data);

    if(data_type->float_type)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

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
    if(components < 1 || components > 4)
    {
      MGL_CORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      MGL_CORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new mgl::opengl::texture_array();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_layers = layers;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_texture_obj = 0;

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    glGenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      MGL_CORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = data_type->internal_format[components];

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture->m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,
                 internal_format,
                 width,
                 height,
                 layers,
                 0,
                 base_format,
                 pixel_type,
                 data);

    if(data_type->float_type)
    {
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

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
    if(components < 1 || components > 4)
    {
      MGL_CORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      MGL_CORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new mgl::opengl::texture_cube();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_texture_obj = 0;

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    glGenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      MGL_CORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = internal_format_override ? internal_format_override
                                                       : data_type->internal_format[components];

    int32_t expected_size = width * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * 6;

    const char* ptr[6] = {
      (const char*)data + expected_size * 0 / 6, (const char*)data + expected_size * 1 / 6,
      (const char*)data + expected_size * 2 / 6, (const char*)data + expected_size * 3 / 6,
      (const char*)data + expected_size * 4 / 6, (const char*)data + expected_size * 5 / 6,
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 base_format,
                 pixel_type,
                 ptr[5]);

    if(data_type->float_type)
    {
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

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

    int32_t i = 0;
    for(auto&& v_data : vertex_buffers)
    {
      if(v_data.buffer == nullptr)
      {
        MGL_CORE_ERROR("vertex_buffers[{0}]: empty vertex buffer", i);
        return nullptr;
      }

      buffer_layout layout = v_data.buffer_layout;

      if(layout.is_invalid())
      {
        MGL_CORE_ERROR("vertex_buffers[{0}]: invalid buffer layout", i);
        return nullptr;
      }

      if(!v_data.attributes.size())
      {
        MGL_CORE_ERROR("vertex_buffers[{0}]: attributes must not be empty", i);
        return nullptr;
      }

      if((int32_t)v_data.attributes.size() != layout.size())
      {
        MGL_CORE_ERROR("vertex_buffers[{0}]: format and attributes size mismatch {1} != {2}",
                       i,
                       layout.size(),
                       v_data.attributes.size());
        return nullptr;
      }

      i++;
    }

    if(index_element_size != 1 && index_element_size != 2 && index_element_size != 4)
    {
      MGL_CORE_ERROR("index_element_size must be 1, 2, or 4, not %d", index_element_size);
      return nullptr;
    }

    auto array = new mgl::opengl::vertex_array();
    array->m_released = false;
    array->m_context = this;
    array->m_num_vertices = 0;
    array->m_num_instances = 1;
    array->m_program = program;
    array->m_index_buffer = index_buffer;
    array->m_index_element_size = index_element_size;
    const int32_t element_types[5] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 0, GL_UNSIGNED_INT };
    array->m_index_element_type = element_types[index_element_size];
    array->m_num_vertices = -1;

    array->m_vertex_array_obj = 0;
    glGenVertexArrays(1, (GLuint*)&array->m_vertex_array_obj);

    if(!array->m_vertex_array_obj)
    {
      MGL_CORE_ERROR("cannot create vertex array");
      delete array;
      return nullptr;
    }

    glBindVertexArray(array->m_vertex_array_obj);

    if(array->m_index_buffer != nullptr)
    {
      array->m_num_vertices = (int32_t)(array->m_index_buffer->size() / index_element_size);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, array->m_index_buffer->glo());
    }

    i = 0;
    for(auto&& v_data : vertex_buffers)
    {
      auto buffer = v_data.buffer;
      const char* format = v_data.buffer_layout.c_str();

      buffer_layout layout = v_data.buffer_layout;

      int32_t buf_vertices = (int32_t)(buffer->size() / layout.size());

      if(!layout.divisor() && array->m_index_buffer == nullptr &&
         (!i || array->m_num_vertices > buf_vertices))
      {
        array->m_num_vertices = buf_vertices;
      }

      glBindBuffer(GL_ARRAY_BUFFER, buffer->glo());

      for(size_t j = 0; j < v_data.attributes.size(); ++j)
      {

        auto attribute = array->m_program->attribute(v_data.attributes[j]);

        if(!attribute)
        {
          continue;
        }

        buffer_layout::element element = layout[j];
        int32_t attribute_location = attribute->location();
        int32_t attribute_rows_length = attribute->get_data_type()->rows_length;
        int32_t attribute_scalar_type = attribute->get_data_type()->scalar_type;

        char* ptr = (char*)(intptr_t)element.offset;
        for(int32_t r = 0; r < attribute_rows_length; ++r)
        {
          int32_t location = attribute_location + r;
          int32_t count = element.count / attribute_rows_length;

          switch(attribute_scalar_type)
          {
            case GL_FLOAT:
              glVertexAttribPointer(
                  location, count, element.type, element.normalize, layout.stride(), ptr);
              break;
            case GL_DOUBLE:
              glVertexAttribLPointer(location, count, element.type, layout.stride(), ptr);
              break;
            case GL_INT:
              glVertexAttribIPointer(location, count, element.type, layout.stride(), ptr);
              break;
            case GL_UNSIGNED_INT:
              glVertexAttribIPointer(location, count, element.type, layout.stride(), ptr);
              break;
          }

          glVertexAttribDivisor(location, layout.divisor());
          glEnableVertexAttribArray(location);
          ptr += element.size / attribute_rows_length;
        }
      }
      i++;
    }

    return vertex_array_ref(array);
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
