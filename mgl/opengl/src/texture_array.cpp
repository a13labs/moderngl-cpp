#include "mgl_opengl/texture_array.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_opengl_internal.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture_array::texture_array(const context_ref& ctx,
                               int32_t w,
                               int32_t h,
                               int32_t layers,
                               int32_t components,
                               const void* data,
                               int32_t align,
                               const std::string& dtype)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(w > 0, "[TextureArray] Width must be greater than 0.");
    MGL_CORE_ASSERT(h > 0, "[TextureArray] Height must be greater than 0.");
    MGL_CORE_ASSERT(components > 0 && components < 5,
                    "[TextureArray] Components must be 1, 2, 3 or 4.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ASSERT(false, "[TextureArray] Invalid data type got '{0}'.", dtype);
      return;
    }

    m_width = w;
    m_height = h;
    m_layers = layers;
    m_components = components;
    m_data_type = data_type;
    m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    m_filter = { filter, filter };

    m_repeat_x = true;
    m_repeat_y = true;

    GLuint glo = 0;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glGenTextures(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[TextureArray] Failed to create texture.");
      return;
    }

    gl_object::set_glo(glo);

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = data_type->internal_format[components];

    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY, 0, internal_format, w, h, layers, 0, base_format, pixel_type, data);

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
  }

  void texture_array::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteTextures(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void texture_array::read(mgl::uint8_buffer& dst, int32_t align, size_t dst_off)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * m_height * m_layers;
    MGL_CORE_ASSERT(dst.size() >= dst_off + expected_size,
                    "[TextureArray] Destination out of bounds.");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + dst_off;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, base_format, pixel_type, ptr);
  }

  void texture_array::read(buffer_ref& dst, int32_t align, size_t dst_off)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, base_format, pixel_type, (void*)dst_off);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void texture_array::write(const mgl::uint8_buffer& src, const mgl::cube& viewport, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    int32_t x = viewport.x;
    int32_t y = viewport.y;
    int32_t z = viewport.z;
    int32_t width = viewport.width;
    int32_t height = viewport.height;
    int32_t layers = viewport.depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * layers;
    MGL_CORE_ASSERT(src.size() >= expected_size, "[TextureArray] Destination out of bounds.");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,
                    x,
                    y,
                    z,
                    width,
                    height,
                    layers,
                    base_format,
                    pixel_type,
                    src.data());
  }

  void texture_array::write(const mgl::uint8_buffer& src, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t layers = m_layers;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * layers;
    MGL_CORE_ASSERT(src.size() >= expected_size, "[TextureArray] Destination out of bounds.");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,
                    x,
                    y,
                    z,
                    width,
                    height,
                    layers,
                    base_format,
                    pixel_type,
                    src.data());
  }

  void texture_array::write(const buffer_ref& src, const mgl::cube& viewport, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    int32_t x = viewport.x;
    int32_t y = viewport.y;
    int32_t z = viewport.z;
    int32_t width = viewport.width;
    int32_t height = viewport.height;
    int32_t layers = viewport.depth;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_array::write(const buffer_ref& src, int32_t align)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[TextureArray] Alignment must be 1, 2, 4 or 8.");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t layers = m_layers;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void
  texture_array::bind_to_image(int32_t unit, bool read, bool write, int32_t level, int32_t format)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(read || write,
                    "[TextureArray] Illegal access mode. Read or write needs to be enabled.");

    int32_t access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int32_t frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, gl_object::glo(), level, GL_TRUE, 0, access, frmt);
  }

  void texture_array::use(int32_t index)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());
  }

  void texture_array::build_mipmaps(int32_t base, int32_t max_level)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(base <= max_level, "[TextureArray] Invalid base.");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, base);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, m_max_level);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_level = max_level;
  }

  void texture_array::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    m_repeat_x = value;

    if(m_repeat_x)
    {
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void texture_array::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    m_repeat_y = value;

    if(m_repeat_y)
    {
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void texture_array::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_array::swizzle()
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    int32_t swizzle_r = 0;
    int32_t swizzle_g = 0;
    int32_t swizzle_b = 0;
    int32_t swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      internal::char_from_swizzle(swizzle_r),
      internal::char_from_swizzle(swizzle_g),
      internal::char_from_swizzle(swizzle_b),
      internal::char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_array::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");
    MGL_CORE_ASSERT(value.size() <= 4, "[TextureArray] Swizzle is too long.");
    MGL_CORE_ASSERT(value.size() > 0, "[TextureArray] Swizzle is empty.");

    int32_t tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int32_t i = 0; value[i]; ++i)
    {
      tex_swizzle[i] = internal::swizzle_from_char(value[i]);
      MGL_CORE_ASSERT(
          tex_swizzle[i] != -1, "[TextureArray] '{0}' is not a valid swizzle parameter.", value[i]);
    }

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }

  void texture_array::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[TextureArray] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[TextureArray] Resource context not current.");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), gl_object::ctx()->max_anisotropy());

    glActiveTexture(GL_TEXTURE0 + gl_object::ctx()->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, gl_object::glo());

    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }
} // namespace  mgl::opengl
