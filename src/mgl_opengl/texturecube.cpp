
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
#include "texturecube.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"
#include "mgl_core/log.hpp"

namespace mgl::opengl
{
  texture::type texture_cube::texture_type()
  {
    return texture::TEXTURE_CUBE;
  }

  void texture_cube::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;
    gl.DeleteTextures(1, (GLuint*)&m_texture_obj);
  }

  bool texture_cube::read_into(mgl::core::mem_buffer<uint8_t>& dst, int face, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * m_height;
    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, ptr);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool texture_cube::read_into(buffer_ref& dst, int face, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());

    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, base_format, pixel_type, (char*)write_offset);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool
  texture_cube::write(const mgl::core::mem_buffer<uint8_t>& src, int face, const mgl::core::viewport_2d& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

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

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, src.data());

    return gl.GetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const mgl::core::mem_buffer<uint8_t>& src, int face, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

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

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, src.data());

    return gl.GetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const buffer_ref& src, int face, const mgl::core::viewport_2d& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

    int x = viewport.x;
    int y = viewport.y;
    int width = viewport.width;
    int height = viewport.height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool texture_cube::write(const buffer_ref& src, int face, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    MGL_CORE_ASSERT(face >= 0 && face <= 5, "the face must be 0, 1, 2, 3, 4 or 5");
    const GLMethods& gl = m_context->gl();

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, x, y, width, height, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  void texture_cube::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");
    const GLMethods& gl = m_context->gl();

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    gl.BindImageTexture(unit, m_texture_obj, level, GL_TRUE, 0, access, frmt);
  }

  void texture_cube::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + index);
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
  }

  void texture_cube::set_filter(const texture::filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    m_filter = value;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  mgl::core::string texture_cube::swizzle()
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    gl.GetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    gl.GetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    gl.GetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    gl.GetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
      char_from_swizzle(swizzle_r), char_from_swizzle(swizzle_g), char_from_swizzle(swizzle_b), char_from_swizzle(swizzle_a), 0,
    };

    return swizzle;
  }

  void texture_cube::set_swizzle(const mgl::core::string& value)
  {
    MGL_CORE_ASSERT(!m_released, "TextureCube already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
    }

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }

  void texture_cube::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture2D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);

    gl.TexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }
} // namespace  mgl::opengl
