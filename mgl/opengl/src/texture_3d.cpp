
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
#include "mgl_opengl/texture_3d.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void texture_3d::release()
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

  texture::type texture_3d::texture_type()
  {
    return texture::TEXTURE_3D;
  }

  bool texture_3d::read_into(mgl::uint8_buffer& dst, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * m_height * m_depth;
    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, ptr);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_3d::read_into(buffer_ref& dst, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glGetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, (void*)write_offset);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_3d::write(const mgl::uint8_buffer& src, const mgl::cube& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int base_format = m_data_type->base_format[m_components];
    int pixel_type = m_data_type->gl_type;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage3D(
        GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_3d::write(const mgl::uint8_buffer& src, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage3D(
        GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_3d::write(const buffer_ref& src, const mgl::cube& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  bool texture_3d::write(const buffer_ref& src, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  void texture_3d::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
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

  void texture_3d::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);
  }

  void texture_3d::build_mipmaps(int base, int max_level)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(base <= max_level, "invalid base");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, base);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, m_max_level);

    glGenerateMipmap(GL_TEXTURE_3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_level = max_level;
  }

  void texture_3d::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_x = value;

    if(m_repeat_x)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_y = value;

    if(m_repeat_y)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_z = value;

    if(m_repeat_z)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void texture_3d::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_filter = value;

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  std::string texture_3d::swizzle()
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      char_from_swizzle(swizzle_r),
      char_from_swizzle(swizzle_g),
      char_from_swizzle(swizzle_b),
      char_from_swizzle(swizzle_a),
      0,
    };

    return swizzle;
  }

  void texture_3d::set_swizzle(const std::string& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
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
    glBindTexture(GL_TEXTURE_3D, m_texture_obj);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }
} // namespace  mgl::opengl
