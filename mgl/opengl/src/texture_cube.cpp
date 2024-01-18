
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
#include "mgl_opengl/texture_cube.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  texture::type texture_cube::texture_type()
  {
    return texture::TEXTURE_CUBE;
  }

  void texture_cube::release()
  {
    if(m_released)
    {
      return;
    }

    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_released = true;
    glDeleteTextures(1, (GLuint*)&m_texture_obj);
  }

  bool
  texture_cube::read_into(mgl::byte_buffer& dst, int face, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * m_height;
    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, ptr);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_cube::read_into(buffer_ref& dst, int face, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, (char*)write_offset);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const mgl::byte_buffer& src,
                           int face,
                           const mgl::rect& viewport,
                           int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0,
                    x,
                    y,
                    width,
                    height,
                    base_format,
                    pixel_type,
                    src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const mgl::byte_buffer& src, int face, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0,
                    x,
                    y,
                    width,
                    height,
                    base_format,
                    pixel_type,
                    src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool
  texture_cube::write(const buffer_ref& src, int face, const mgl::rect& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const buffer_ref& src, int face, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  void texture_cube::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    glBindImageTexture(unit, m_texture_obj, level, GL_TRUE, 0, access, frmt);
  }

  void texture_cube::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
  }

  void texture_cube::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_cube::swizzle()
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      char_from_swizzle(swizzle_r),
      char_from_swizzle(swizzle_g),
      char_from_swizzle(swizzle_b),
      char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_cube::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
    }

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

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
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }
} // namespace  mgl::opengl
