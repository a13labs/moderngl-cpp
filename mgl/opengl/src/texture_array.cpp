
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mgl_opengl/texture_array.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture_array::texture_array(int32_t w,
                               int32_t h,
                               int32_t layers,
                               int32_t components,
                               const void* data,
                               int32_t align,
                               const std::string& dtype)
  {
    MGL_CORE_ASSERT(w > 0, "width must be greater than 0");
    MGL_CORE_ASSERT(h > 0, "height must be greater than 0");
    MGL_CORE_ASSERT(layers > 0, "depth must be greater than 0");
    MGL_CORE_ASSERT(components > 0 && components < 5, "components must be 1, 2, 3 or 4");

    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    m_glo = 0;

    auto data_type = from_dtype(dtype);

    if(!data_type)
    {
      MGL_CORE_ASSERT(false, "Invalid data type got: '{0}'", dtype);
      return;
    }

    m_width = width;
    m_height = height;
    m_layers = layers;
    m_components = components;
    m_data_type = data_type;
    m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    m_filter = { filter, filter };

    m_repeat_x = true;
    m_repeat_y = true;

    GLuint glo = 0;

    glActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    glGenTextures(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "Failed to create texture");
      return;
    }

    m_glo = glo;

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = data_type->base_format[components];
    int32_t internal_format = data_type->internal_format[components];

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "texture already released");
    glDeleteTextures(1, (GLuint*)&m_glo);
    m_glo = 0;
  }

  void texture_array::read(mgl::uint8_buffer& dst, int32_t align, size_t dst_off)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * m_height * m_layers;
    MGL_CORE_ASSERT(dst.size() >= dst_off + expected_size, "out of bounds");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + dst_off;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, base_format, pixel_type, ptr);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_array::read(buffer_ref& dst, int32_t align, size_t dst_off)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, base_format, pixel_type, (void*)dst_off);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_array::write(const mgl::uint8_buffer& src, const mgl::cube& viewport, int32_t align)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = viewport.x;
    int32_t y = viewport.y;
    int32_t z = viewport.z;
    int32_t width = viewport.width;
    int32_t height = viewport.height;
    int32_t layers = viewport.depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * layers;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_array::write(const mgl::uint8_buffer& src, int32_t align)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t layers = m_layers;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * height * layers;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_array::write(const buffer_ref& src, const mgl::cube& viewport, int32_t align)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = viewport.x;
    int32_t y = viewport.y;
    int32_t z = viewport.z;
    int32_t width = viewport.width;
    int32_t height = viewport.height;
    int32_t layers = viewport.depth;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void texture_array::write(const buffer_ref& src, int32_t align)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "align must be 1, 2, 4 or 8");

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t width = m_width;
    int32_t height = m_height;
    int32_t layers = m_layers;

    int32_t pixel_type = m_data_type->gl_type;
    int32_t base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->glo());
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void
  texture_array::bind_to_image(int32_t unit, bool read, bool write, int32_t level, int32_t format)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");

    int32_t access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int32_t frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, m_glo, level, GL_TRUE, 0, access, frmt);
  }

  void texture_array::use(int32_t index)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);
  }

  void texture_array::build_mipmaps(int32_t base, int32_t max_level)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");
    MGL_CORE_ASSERT(base <= max_level, "invalid base");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "texture already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "texture already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "texture already released");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_array::swizzle()
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    int32_t swizzle_r = 0;
    int32_t swizzle_g = 0;
    int32_t swizzle_b = 0;
    int32_t swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      char_from_swizzle(swizzle_r),
      char_from_swizzle(swizzle_g),
      char_from_swizzle(swizzle_b),
      char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_array::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(m_glo, "texture already released");

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int32_t tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int32_t i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
    }

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "Texture2D already released");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_glo);

    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }
} // namespace  mgl::opengl
