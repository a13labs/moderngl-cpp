#include "mgl_opengl/texture_cube.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_opengl_internal.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture_cube::texture_cube(const context_ref& ctx,
                             int32_t w,
                             int32_t h,
                             int32_t components,
                             const void* data,
                             int32_t align,
                             const std::string& dtype,
                             int32_t internal_format_override)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(w > 0, "[TextureCube] Width must be greater than 0.");
    MGL_CORE_ASSERT(h > 0, "[TextureCube] Height must be greater than 0.");
    MGL_CORE_ASSERT(components > 0 && components < 5,
                    "[TextureCube] Components must be 1, 2, 3 or 4.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ASSERT(false, "[TextureCube] Invalid data type got '{0}'.", dtype);
      return;
    }

    m_width = w;
    m_height = h;
    m_components = components;
    m_data_type = data_type;
    m_max_lvl = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    m_filter = { filter, filter };

    GLuint glo = 0;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->max_texture_units());
    glGenTextures(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[TextureCube] Cannot create texture.");
      return;
    }

    gl_object::set_glo(glo);

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = internal_format_override ? internal_format_override
                                                       : data_type->internal_format[components];

    int32_t expected_size = w * components * data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * h * 6;

    const char* ptr[6] = {
      (const char*)data + expected_size * 0 / 6, (const char*)data + expected_size * 1 / 6,
      (const char*)data + expected_size * 2 / 6, (const char*)data + expected_size * 3 / 6,
      (const char*)data + expected_size * 4 / 6, (const char*)data + expected_size * 5 / 6,
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                 0,
                 internal_format,
                 w,
                 h,
                 0,
                 base_format,
                 pixel_type,
                 ptr[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                 0,
                 internal_format,
                 w,
                 h,
                 0,
                 base_format,
                 pixel_type,
                 ptr[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                 0,
                 internal_format,
                 w,
                 h,
                 0,
                 base_format,
                 pixel_type,
                 ptr[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                 0,
                 internal_format,
                 w,
                 h,
                 0,
                 base_format,
                 pixel_type,
                 ptr[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                 0,
                 internal_format,
                 w,
                 h,
                 0,
                 base_format,
                 pixel_type,
                 ptr[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                 0,
                 internal_format,
                 w,
                 h,
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
  }

  void texture_cube::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteTextures(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void texture_cube::read(mgl::uint8_buffer& dst, int face, int align, size_t write_offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * m_height;
    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size,
                    "[TextureCube] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, ptr);
  }

  void texture_cube::read(buffer_ref& dst, int face, int align, size_t write_offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());

    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());
    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, (char*)write_offset);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void
  texture_cube::write(const mgl::uint8_buffer& src, int face, const mgl::rect& viewport, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height;
    MGL_CORE_ASSERT(src.size() >= expected_size, "[TextureCube] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0,
                    x,
                    y,
                    width,
                    height,
                    base_format,
                    pixel_type,
                    src.data());
  }

  void texture_cube::write(const mgl::uint8_buffer& src, int face, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height;
    MGL_CORE_ASSERT(src.size() >= expected_size, "[TextureCube] Destination out of bounds.");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0,
                    x,
                    y,
                    width,
                    height,
                    base_format,
                    pixel_type,
                    src.data());
  }

  void texture_cube::write(const buffer_ref& src, int face, const mgl::rect& viewport, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_cube::write(const buffer_ref& src, int face, int align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureCube] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "[TextureCube] Face must be 0, 1, 2, 3, 4 or 5.");

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_cube::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(read || write,
                    "[TextureCube] Illegal access mode. Read or write needs to be enabled.");

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, gl_object::glo(), level, GL_TRUE, 0, access, frmt);
  }

  void texture_cube::use(int index)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());
  }

  void texture_cube::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_cube::swizzle()
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      internal::char_from_swizzle(swizzle_r),
      internal::char_from_swizzle(swizzle_g),
      internal::char_from_swizzle(swizzle_b),
      internal::char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_cube::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");
    MGL_CORE_ASSERT(value.size() <= 4, "[TextureCube] Swizzle is too long.");
    MGL_CORE_ASSERT(value.size() > 0, "[TextureCube] Swizzle is empty.");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; value[i]; ++i)
    {
      tex_swizzle[i] = internal::swizzle_from_char(value[i]);
      MGL_CORE_ASSERT(
          tex_swizzle[i] != -1, "[TextureCube] '{0}' is not a valid swizzle parameter.", value[i]);
    }

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }

  void texture_cube::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureCube] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureCube] Resource context not current.");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), gl_object::ctx()->max_anisotropy());

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_object::glo());

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }
} // namespace  mgl::opengl
