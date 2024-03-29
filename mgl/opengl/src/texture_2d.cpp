#include "mgl_opengl/texture_2d.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"
#include "mgl_opengl/framebuffer.hpp"

#include "mgl_opengl_internal.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture_2d::texture_2d(const context_ref& ctx,
                         int32_t w,
                         int32_t h,
                         int32_t components,
                         const void* data,
                         int32_t samples,
                         int32_t align,
                         const std::string& dtype,
                         int32_t internal_format_override)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(components > 0 && components <= 4,
                    "[Texture2D] Components must be between 1 and 4.");
    MGL_CORE_ASSERT(w > 0, "[Texture2D] Width must be greater than 0.");
    MGL_CORE_ASSERT(h > 0, "[Texture2D] Height must be greater than 0.");
    MGL_CORE_ASSERT(!samples || (samples & (samples - 1)) == 0,
                    "[Texture2D] Samples must be a power of 2.");
    MGL_CORE_ASSERT(!samples || samples <= gl_object::ctx()->max_samples(),
                    "[Texture2D] Samples must be less than or equal to {0}.",
                    gl_object::ctx()->max_samples());

    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(!samples || !data,
                    "[Texture2D] Multisample textures are not writable directly.");

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ASSERT(false, "[Texture2D] Invalid data type got '{0}'.", dtype);
      return;
    }

    int32_t texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = internal_format_override ? internal_format_override
                                                       : data_type->internal_format[components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());

    m_width = w;
    m_height = h;
    m_components = components;
    m_samples = samples;
    m_data_type = data_type;
    m_max_lvl = 0;
    m_compare_func = mgl::opengl::compare_func::NONE;
    m_anisotropy = 1.0f;
    m_depth = false;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    m_filter = { filter, filter };

    m_repeat_x = true;
    m_repeat_y = true;

    GLuint glo = 0;

    glGenTextures(1, (GLuint*)&glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[Texture2D] Cannot create texture.");
      return;
    }

    gl_object::set_glo(glo);

    glBindTexture(texture_target, gl_object::glo());

    if(samples)
    {
      glTexImage2DMultisample(texture_target, samples, internal_format, w, h, true);
    }
    else
    {
      glPixelStorei(GL_PACK_ALIGNMENT, align);
      glPixelStorei(GL_UNPACK_ALIGNMENT, align);
      glTexImage2D(texture_target, 0, internal_format, w, h, 0, base_format, pixel_type, data);
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
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on creating texture 2d.");
  }

  texture_2d::texture_2d(const context_ref& ctx,
                         int32_t w,
                         int32_t h,
                         const void* data,
                         int32_t samples,
                         int32_t align)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(w > 0, "[Texture2D] Width must be greater than 0.");
    MGL_CORE_ASSERT(h > 0, "[Texture2D] Height must be greater than 0.");
    MGL_CORE_ASSERT(!samples || (samples & (samples - 1)) == 0,
                    "[Texture2D] Samples must be a power of 2.");
    MGL_CORE_ASSERT(!samples || samples <= ctx->max_samples(),
                    "[Texture2D] Samples must be less than or equal to {0}.",
                    ctx->max_samples());

    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(!samples || !data,
                    "[Texture2D] Multisample textures are not writable directly.");

    gl_object::set_glo(0);
    m_width = w;
    m_height = h;
    m_components = 1;
    m_samples = samples;
    m_data_type = from_dtype("f4", 2);
    m_max_lvl = 0;
    m_compare_func = mgl::opengl::compare_func::EQUAL;
    m_anisotropy = 1.0f;
    m_depth = true;
    m_filter = { GL_LINEAR, GL_LINEAR };
    m_repeat_x = false;
    m_repeat_y = false;

    GLuint glo = 0;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glGenTextures(1, (GLuint*)&glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[Texture2D] Cannot create texture.");
      return;
    }

    gl_object::set_glo(glo);

    int32_t texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int32_t pixel_type = GL_FLOAT;

    glBindTexture(texture_target, gl_object::glo());

    if(samples)
    {
      glTexImage2DMultisample(texture_target, samples, GL_DEPTH_COMPONENT24, w, h, true);
    }
    else
    {
      glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glPixelStorei(GL_PACK_ALIGNMENT, align);
      glPixelStorei(GL_UNPACK_ALIGNMENT, align);
      glTexImage2D(
          texture_target, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, pixel_type, data);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on creating texture 2d.");
  }

  void texture_2d::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteTextures(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void texture_2d::read(mgl::uint8_buffer& dst, int lvl, int align, size_t dst_off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int width = m_width / (1 << lvl);
    int height = m_height / (1 << lvl);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(dst.size() >= dst_off + expected_size,
                    "[Texture2D] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + dst_off;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D, lvl, base_format, pixel_type, ptr);
  }

  void texture_2d::read(buffer_ref& dst, int lvl, int align, size_t dst_off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int width = m_width / (1 << lvl);
    int height = m_height / (1 << lvl);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D, lvl, base_format, pixel_type, (void*)dst_off);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void texture_2d::write(const mgl::uint8_buffer& src, const mgl::rect& v, int lvl, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int x = v.x;
    int y = v.y;
    int width = v.width;
    int height = v.height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(src.size() >= expected_size, "[Texture2D] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_2D, lvl, x, y, width, height, format, pixel_type, src.data());
  }

  void texture_2d::write(const mgl::uint8_buffer& src, int lvl, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int x = 0;
    int y = 0;
    int width = m_width / (1 << lvl);
    int height = m_height / (1 << lvl);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(src.size() >= expected_size, "[Texture2D] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_2D, lvl, x, y, width, height, format, pixel_type, src.data());
  }

  void texture_2d::write(const buffer_ref& src, const mgl::rect& v, int lvl, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int x = v.x;
    int y = v.y;
    int width = v.width;
    int height = v.height;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_2D, lvl, x, y, width, height, format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_2d::write(const buffer_ref& src, int lvl, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Texture2D] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(lvl <= m_max_lvl, "[Texture2D] Invalid level.");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Multisample textures cannot be read directly.");

    int x = 0;
    int y = 0;
    int width = m_width / (1 << lvl);
    int height = m_height / (1 << lvl);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_2D, lvl, x, y, width, height, format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_2d::bind_to_image(int unit, bool read, bool write, int lvl, int format)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, gl_object::glo(), lvl, 0, 0, access, frmt);
  }

  void texture_2d::use(int index)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(texture_target, gl_object::glo());
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::build_mipmaps(int base, int max_level)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(base <= max_level, "invalid base");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    glTexParameteri(texture_target, GL_TEXTURE_BASE_LEVEL, base);
    glTexParameteri(texture_target, GL_TEXTURE_MAX_LEVEL, max_level);

    glGenerateMipmap(texture_target);

    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_lvl = max_level;
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    m_repeat_x = value;

    if(m_repeat_x)
    {
      glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    m_repeat_y = value;

    if(m_repeat_y)
    {
      glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");

    m_filter = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());
    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  std::string texture_2d::swizzle() const
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(!m_depth, "[Texture2D] Cannot get swizzle of depth textures.");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      internal::char_from_swizzle(swizzle_r),
      internal::char_from_swizzle(swizzle_g),
      internal::char_from_swizzle(swizzle_b),
      internal::char_from_swizzle(swizzle_a),
      0,
    };

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
    return swizzle;
  }

  void texture_2d::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(!m_depth, "[Texture2D] Cannot set swizzle for depth textures.");
    MGL_CORE_ASSERT(value.size() <= 4, "[Texture2D] Swizzle is too long.");
    MGL_CORE_ASSERT(value.size() > 0, "[Texture2D] Swizzle is empty.");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; value[i]; ++i)
    {
      tex_swizzle[i] = internal::swizzle_from_char(value[i]);
      MGL_CORE_ASSERT(
          tex_swizzle[i] != -1, "[Texture2D] '{0}' is not a valid swizzle parameter.", value[i]);
    }

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(texture_target, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::set_compare_func(mgl::opengl::compare_func value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(m_depth, "[Texture2D] Only depth textures have compare function.");

    m_compare_func = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    if(m_compare_func == 0)
    {
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }
    else
    {
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      glTexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
    }
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), gl_object::ctx()->max_anisotropy());
    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    glTexParameterf(texture_target, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

  void texture_2d::resize(int width, int height, int components, const mgl::uint8_buffer& data)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Texture2D] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Texture2D] Resource context not current.");
    MGL_CORE_ASSERT(width > 0, "[Texture2D] Width must be greater than 0.");
    MGL_CORE_ASSERT(height > 0, "[Texture2D] Height must be greater than 0.");
    MGL_CORE_ASSERT(components > 0 && components <= 4,
                    "[Texture2D] components must be between 1 and 4");
    MGL_CORE_ASSERT(!m_samples, "[Texture2D] Cannot resize multisample textures.");
    MGL_CORE_ASSERT(!m_depth, "[Texture2D] Cannot resize depth textures.");

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    if(m_width == width && m_height == height && m_components == components)
    {
      glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
      glBindTexture(texture_target, gl_object::glo());

      if(data.size() > 0)
      {
        int pixel_type = m_data_type->gl_type;
        int format = m_data_type->base_format[m_components];

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, pixel_type, data.data());
      }

      MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
    }

    int internal_format = m_data_type->internal_format[components];
    int format = m_data_type->base_format[components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(texture_target, gl_object::glo());

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 format,
                 m_data_type->gl_type,
                 data.data());

    m_width = width;
    m_height = height;
    m_components = components;
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[GL Texture2D] Error on binding texture 2d.");
  }

} // namespace  mgl::opengl
